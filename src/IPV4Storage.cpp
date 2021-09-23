#include "IPV4Storage.h"
#include <algorithm>
#include <iostream>
#include <regex>
#include <sstream>

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
    std::sort(mStorage.begin(), mStorage.end(), reverseCMP);
}

void IPV4Pool::read()
{
    for (std::string line; std::getline(std::cin, line) && !line.empty();)
    {
        std::vector<std::string> v = split(line, '\t');
        mStorage.push_back(split(v.at(0), '.'));
    }
}
std::string IPV4Pool::getIPV4Str(const IPV4& ip)
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

void IPV4Pool::writeIPV4(const IPV4& ip)
{
    std::cout << getIPV4Str(ip)<<std::endl;
}

void IPV4Pool::write()
{
    for (auto ip : mStorage)
    {
        writeIPV4(ip);
    }
}

void IPV4Pool::filterWrite(const std::string& mask_str)
{
    std::regex regex(mask_str.c_str());
    for (auto ip : mStorage)
    {
        auto ip_str = getIPV4Str(ip);
        if (std::regex_search(ip_str, regex))
            std::cout << ip_str << std::endl;
    }
 }