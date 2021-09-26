#pragma once
#include <list>
#include <vector>
#include <string>

typedef std::vector<std::string> IPV4;

class IPV4Pool
{
private:
	std::list<IPV4> mStorage;
private:
	static bool reverseCMP(const IPV4& first, const IPV4& second);
	IPV4 split(const std::string& str, char d);
	void writeIPV4(const IPV4& addr) const;
	bool checkFilter(const IPV4& addr, const std::vector<int> & values);
	bool checkFilterAny(const IPV4& addr, int value);
public:
	void readFromFile(const std::string & fileName);
	std::string getIPV4Str(const IPV4& ip) const;
	IPV4Pool filter(const std::string& mask);
	IPV4Pool filterAny(int value);
	IPV4Pool filter(int first, int second = -1, int third = -1, int fourth = -1);
	void lexicSort();
	friend std::istream& operator>> (std::istream& in,  IPV4Pool& point);
	friend std::ostream& operator<< (std::ostream& out, const IPV4Pool& point);
};