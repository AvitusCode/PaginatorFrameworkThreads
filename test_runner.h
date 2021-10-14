#ifndef TEST_RUNNER_H_
#define TEST_RUNNER_H_
#include <iostream>
#include <sstream>
#include <functional>
#include <exception>
#include <string>
#include <map>
#include <vector>
#include <set>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s);
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s);
template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m);

template<typename T, typename U>
void AssertEqual(const T &t, const U &u, const std::string &hint)
{
	if (t != u)
	{
		std::ostringstream UNIQUE_NAME;
		UNIQUE_NAME << "Assertion failed in hint: " << hint;
		//UNIQUE_NAME << "Assertion failed: " << t << " != " << u << " hint: " << hint;
		throw std::runtime_error(UNIQUE_NAME.str());
	}
}

void Assert(bool b, const std::string& hint);

class TestRunner
{
public:

	TestRunner() = default;
	void RunTest(std::function<void(void)> func, const std::string &test_name);
	~TestRunner();

private:
	TestRunner(const TestRunner &a) = delete;
	TestRunner& operator=(const TestRunner &a) = delete;
	static size_t fail_count;
};

// Улучшенный интерфейс

#define ASSERT_EQUAL(x, y) {                     \
  std::ostringstream UNIQUE_NAME;                \
  UNIQUE_NAME << #x << " != " << #y << ", "      \
    << __FILE__ << ":" << __LINE__;              \
  AssertEqual(x, y, UNIQUE_NAME.str());          \
}

#define ASSERT(x) {                              \
  std::ostringstream UNIQUE_NAME;                \
  UNIQUE_NAME << #x << " is false, "             \
    << __FILE__ << ":" << __LINE__;              \
  Assert(x, UNIQUE_NAME.str());                  \
}

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)

#endif /* TEST_RUNNER_H_ */
