#pragma once
#include <list>
#include <vector>
#include <string>

using IPV4 = std::vector<uint8_t>;

class IPV4Pool
{
private:
	static const int8_t mEmptyFilter = -1;
	std::list<IPV4> mStorage;
private:
	bool checkFilter(const IPV4& addr, const std::vector<int> & values);
	bool checkFilterAny(const IPV4& addr, uint8_t value);
public:
	void readFromFile(const std::string & fileName);
	IPV4Pool filter(const std::string& mask);
	IPV4Pool filterAny(uint8_t value);
	IPV4Pool filter(int first, int second = mEmptyFilter, int third = mEmptyFilter, int fourth = mEmptyFilter);
	void lexicSort();
	friend std::istream& operator>> (std::istream& in,  IPV4Pool& point);
	friend std::ostream& operator<< (std::ostream& out, const IPV4Pool& point);
};