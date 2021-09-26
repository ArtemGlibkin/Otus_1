#include "IPV4Storage.h"
#include <algorithm>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
/*
IPV4 IPV4Pool::split(const std::string& str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}
*/

IPV4 IPV4Pool::split(const std::string& str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

bool IPV4Pool::reverseCMP(const IPV4& a, const IPV4& b)
{
    for (int8_t i = 0; i < 4; i++)
    {
        int first = std::atoi(a.at(i).c_str());
        int second = std::atoi(b.at(i).c_str());
        if (first > second)
            return true;
        else if (first < second)
            return false;
    }

    return false;
}

void IPV4Pool::lexicSort()
{
    //std::sort(mStorage.begin(), mStorage.end(), reverseCMP);
    mStorage.sort(reverseCMP);
}

void IPV4Pool::readFromFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        throw std::runtime_error("File " + fileName + " was not found");

    for (std::string line; std::getline(file, line) && !line.empty();)
    {
        std::vector<std::string> v = split(line, '\t');
        mStorage.push_back(split(v.at(0), '.'));
    }
}

std::string IPV4Pool::getIPV4Str(const IPV4& ip) const
{
    std::stringstream ss;
    for (IPV4::const_iterator ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
    {
        if (ip_part != ip.cbegin())
        {
            ss << ".";
        }
        ss << *ip_part;
    }
    return ss.str();
}

void IPV4Pool::writeIPV4(const IPV4& ip) const
{
    std::cout << getIPV4Str(ip)<<std::endl;
}

std::istream& operator>> (std::istream& in, IPV4Pool& point)
{
    for (std::string line; std::getline(in, line) && !line.empty();)
    {
        std::vector<std::string> v = point.split(line, '\t');
        point.mStorage.push_back(point.split(v.at(0), '.'));
    }
    return in;
}

std::ostream& operator<< (std::ostream& out, const IPV4Pool& point)
{
    for (auto ip : point.mStorage)
    {
        out << point.getIPV4Str(ip)<<std::endl;
    }
    return out;
}

IPV4Pool IPV4Pool::filter(const std::string& mask_str)
{
    IPV4Pool ret(*this);
    std::regex regex(mask_str.c_str());
    auto pool = ret.mStorage;
    for (auto it = pool.begin(); it != pool.end(); it++)
    {
        auto ip_str = getIPV4Str(*it);
        if (!std::regex_search(ip_str, regex))
            pool.erase(it);
    }

    return ret;
 }

bool IPV4Pool::checkFilter(const IPV4& addr, const std::vector<int> & values)
{
    for (int i = 0; i<4; i++)
    {
        auto & value = values.at(i);
        if (value == -1)
            continue;

        if(value != std::atoi(addr.at(i).c_str()))
            return false;
    }
    return true;
}

IPV4Pool IPV4Pool::filter(int first, int second, int third, int fourth)
{
    IPV4Pool ret(*this);
    auto & pool = ret.mStorage;
    for (auto it = pool.begin(); it != pool.end();)
    {
        if (!checkFilter(*it, { first, second, third, fourth }))
        {
            it = pool.erase(it);
        }
        else
            ++it;
    }

    return ret;
}

bool IPV4Pool::checkFilterAny(const IPV4& addr, int value)
{
    for (int i = 0; i < 4; i++)
    {
        if (value == std::atoi(addr.at(i).c_str()))
            return true;
    }
    return false;
}

IPV4Pool IPV4Pool::filterAny(int value)
{
    IPV4Pool ret(*this);
    auto & pool = ret.mStorage;
    for (auto it = pool.begin(); it != pool.end();)
    {
        if (!checkFilterAny(*it, value))
        {
            it = pool.erase(it);
        }
        else
            ++it;
    }

    return ret;
}
