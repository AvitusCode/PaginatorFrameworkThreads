#include "test_runner.h"

size_t TestRunner::fail_count;

TestRunner::~TestRunner()
{
	if (fail_count > 0)
	{
		std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
		exit(1);
	}
}

void TestRunner::RunTest(std::function<void(void)> func, const std::string &test_name)
{
	try
	{
		func();
		std::cerr << test_name << " OK" << std::endl;
	} catch (std::runtime_error &e)
	{
		++fail_count;
		std::cerr << test_name << " fail: " << e.what() << std::endl;
	}
}


template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s)
{
  os << "{";
  bool first = true;
  for (const auto& x : s)
  {
    if (!first) {
      os << ", ";
    }

    first = false;
    os << x;
  }
  return os << "}";
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s)
{
  os << "[";
  bool first = true;
  for (const auto& x : s)
  {
    if (!first) {
      os << "; ";
    }

    first = false;
    os << x;
  }
  return os << "]";
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m)
{
  os << "(";
  bool first = true;
  for (const auto& kv : m)
  {
    if (!first) {
      os << ", ";
    }

    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << ")";
}

void Assert(bool b, const std::string& hint) {
  AssertEqual(b, true, hint);
}





