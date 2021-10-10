#include "IPV4Pool.h"
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

std::string getIPV4Str(const IPV4& ip)
{
    std::stringstream ss;
    for (auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
    {
        if (ip_part != ip.cbegin())
        {
            ss << ".";
        }
        ss << std::to_string(*ip_part);
    }
    return ss.str();
}

void writeIPV4(const IPV4& ip)
{
    std::cout << getIPV4Str(ip) << std::endl;
}

std::string getAddr(const std::string & str)
{
    std::stringstream ss(str);
    std::string ip;
    if (!std::getline(ss, ip, '\t'))
        throw std::runtime_error("Parse ip from stream return error code");
    return ip;
}

std::vector<uint8_t> parseIPAddr(const std::string& str)
{
    std::stringstream ss(str);
    std::vector<uint8_t> ret(IPV4OctetNumber);
    uint8_t i = 0;
    for (std::string line; std::getline(ss, line, '.') && (i < IPV4OctetNumber) && !line.empty();i++)
    {
        ret.at(i) = std::atoi(line.c_str());
    }
    return ret;
}

void IPV4Pool::lexicSort()
{
    mStorage.sort(std::greater<IPV4>());
}

void IPV4Pool::readFromFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        throw std::runtime_error("File " + fileName + " was not found");

    for (std::string line; std::getline(file, line) && !line.empty();)
    {
        mStorage.push_back(parseIPAddr(getAddr(line)));
    }
}

std::istream& operator>> (std::istream& in, IPV4Pool& point)
{
    for (std::string line; std::getline(in, line) && !line.empty();)
    {
        point.mStorage.push_back(parseIPAddr(getAddr(line)));
    }
    return in;
}

std::ostream& operator<< (std::ostream& out, const IPV4Pool& point)
{
    for (auto ip : point.mStorage)
    {
        out << getIPV4Str(ip)<<std::endl;
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
    for (int i = 0; i < IPV4OctetNumber; i++)
    {
        auto & value = values.at(i);
        if (value == mEmptyFilter)
            continue;

        if(value != addr.at(i))
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

bool IPV4Pool::checkFilterAny(const IPV4& addr, uint8_t value)
{
    for (int i = 0; i < IPV4OctetNumber; i++)
    {
        if (value == addr.at(i))
            return true;
    }
    return false;
}

IPV4Pool IPV4Pool::filterAny(uint8_t value)
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
