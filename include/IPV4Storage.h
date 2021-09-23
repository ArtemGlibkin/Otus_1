#pragma once
#include <vector>
#include <string>

typedef std::vector<std::string> IPV4;

class IPV4Pool
{
private:
	std::vector<IPV4> mStorage;
private:
	static bool reverseCMP(const IPV4& first, const IPV4& second);
	IPV4 split(const std::string& str, char d);
	void writeIPV4(const IPV4& addr);
public:
	void read();
	std::string getIPV4Str(const IPV4& ip);
	void write();
	void filterWrite(const std::string& mask);
	void lexicSort();
};