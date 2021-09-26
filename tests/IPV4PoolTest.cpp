#include <gtest/gtest.h>
#include "IPV4Storage.h"
#include <fstream>
#include <sstream>
#include <filesystem>

#ifdef _WIN32
	const std::string testCasesPath = ".\\tests\\test_cases\\";
#else
	const std::string testCasesPath = "./tests/test_cases/";
#endif

std::string readFromFile(const std::string& fileName)
{
	std::ifstream file(fileName);
	if(!file.is_open())
		throw std::runtime_error("File " + fileName + " was not found");

	std::stringstream ss;
	for (std::string line; std::getline(file, line) && !line.empty();)
	{
		ss << line <<std::endl;
	}

	return ss.str();
}

TEST(IPV4PoolTest, Sorting) {
	IPV4Pool pool;
	pool.readFromFile(testCasesPath + "test_sorting.tsv");
	std::stringstream result;
	pool.lexicSort();
	result << pool;
	ASSERT_EQ(result.str(), readFromFile(testCasesPath + "test_sorting_result.tsv"));
}

TEST(IPV4PoolTest, Filter) {

	IPV4Pool pool;
	pool.readFromFile(testCasesPath + "test_filter.tsv");
	std::stringstream result;
	result << pool.filter(46, 70);
	ASSERT_EQ(result.str(), readFromFile(testCasesPath + "test_filter_result.tsv"));
}

TEST(IPV4PoolTest, FilterAny) {
	IPV4Pool pool;
	pool.readFromFile(testCasesPath + "test_filter_any.tsv");
	std::stringstream result;
	result << pool.filterAny(1);
	ASSERT_EQ(result.str(), readFromFile(testCasesPath + "test_filter_any_result.tsv"));
}

TEST(IPV4PoolTest, Final) {
	IPV4Pool pool;
	pool.readFromFile(testCasesPath + "ip_filter.tsv");
	std::stringstream result;
	pool.lexicSort();
	result << pool;
	result << pool.filter(1);
	result << pool.filter(46, 70);
	result << pool.filterAny(46);
	ASSERT_EQ(result.str(), readFromFile(testCasesPath + "ip_filter_result.tsv"));
}