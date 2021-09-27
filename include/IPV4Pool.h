#pragma once
#include <list>
#include <vector>
#include <string>

typedef std::vector<uint8_t> IPV4;

class IPV4Pool
{
private:
	static const int8_t mEmptyFilter = -1;
	std::list<IPV4> mStorage;
private:
	bool checkFilter(const IPV4& addr, const std::vector<int8_t> & values);
	bool checkFilterAny(const IPV4& addr, int8_t value);
public:
	void readFromFile(const std::string & fileName);
	IPV4Pool filter(const std::string& mask);
	IPV4Pool filterAny(int8_t value);
	IPV4Pool filter(int8_t first, int8_t second = mEmptyFilter, int8_t third = mEmptyFilter, int8_t fourth = mEmptyFilter);
	void lexicSort();
	friend std::istream& operator>> (std::istream& in,  IPV4Pool& point);
	friend std::ostream& operator<< (std::ostream& out, const IPV4Pool& point);
};