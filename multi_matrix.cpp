#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <future>

#include "my_utilities.h"
#include "test_runner.h"


// BEGIN Multy thread matrix generation
template<typename Container>
void makeSingleThread(Container& c, size_t row_pos, size_t column_size)
{
	for (auto& row : c)
	{
		row.resize(column_size);

		// Для тестов. В идеале нужно добавить возможность менять способ генерации данных
		for (size_t column = 0; column < column_size; column++) {
			row.push_back(row_pos ^ column);
		}

		row_pos++;
	}
}

std::vector<std::vector<int>> makeMultiThread(size_t size, size_t page_size)
{
	std::vector<std::vector<int>> result(size);
	std::vector<std::future<void>> threads;

	size_t row_pos = 0;

	for (auto page : Paginate(result, page_size))
	{
		threads.push_back(std::async([page, row_pos, size] {
			makeSingleThread(page, row_pos, size);
			}));

		row_pos += page_size;
	}

	return result;
}

// END Multy thread matrix gemeration

void TestPageCounts()
{
	std::vector<int> v(15);

	ASSERT_EQUAL(Paginate(v, 1).size(), v.size());
	ASSERT_EQUAL(Paginate(v, 3).size(), 5u);
	ASSERT_EQUAL(Paginate(v, 5).size(), 3u);
	ASSERT_EQUAL(Paginate(v, 4).size(), 4u);
	ASSERT_EQUAL(Paginate(v, 15).size(), 1u);
	ASSERT_EQUAL(Paginate(v, 150).size(), 1u);
	ASSERT_EQUAL(Paginate(v, 14).size(), 2u);
}

void TestLooping()
{
	std::vector<int> v(15);
	std::iota(std::begin(v), std::end(v), 1);

	Paginator<std::vector<int>::iterator> paginate_v(v.begin(), v.end(), 6);
	std::ostringstream os;
	for (const auto& page : paginate_v)
	{
		for (int x : page)
		{
			os << x << ' ';
		}
		os << '\n';
	}

	ASSERT_EQUAL(os.str(), "1 2 3 4 5 6 \n7 8 9 10 11 12 \n13 14 15 \n");
}

void TestModification()
{
	std::vector<std::string> vs = { "one", "two", "three", "four", "five" };
	for (auto page : Paginate(vs, 2))
	{
		for (auto& word : page)
		{
			word[0] = toupper(word[0]);
		}
	}

	const std::vector<std::string> expected = { "One", "Two", "Three", "Four", "Five" };
	ASSERT_EQUAL(vs, expected);
}

void TestPageSizes()
{
	std::string letters(26, ' ');

	Paginator<std::string::iterator> letters_pagination(letters.begin(), letters.end(), 11);
	std::vector<size_t> page_sizes;
	for (const auto& page : letters_pagination)
	{
		page_sizes.push_back(page.size());
	}

	const std::vector<size_t> expected = { 11, 11, 4 };
	ASSERT_EQUAL(page_sizes, expected);
}

void TestConstContainer()
{
	const std::string letters = "abcdefghijklmnopqrstuvwxyz";

	std::vector<std::string> pages;
	for (const auto& page : Paginate(letters, 10))
	{
		pages.push_back(std::string(page.begin(), page.end()));
	}

	const std::vector<std::string> expected = { "abcdefghij", "klmnopqrst", "uvwxyz" };
	ASSERT_EQUAL(pages, expected);
}

void TestPagePagination()
{
	std::vector<int> v(22);
	std::iota(std::begin(v), std::end(v), 1);

	std::vector<std::vector<int>> lines;
	for (const auto& split_by_9 : Paginate(v, 9))
	{
		for (const auto& split_by_4 : Paginate(split_by_9, 4))
		{
			lines.push_back({ });
			for (int item : split_by_4)
			{
				lines.back().push_back(item);
			}
		}
	}

	const std::vector<std::vector<int>> expected = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 },
			{ 9 }, { 10, 11, 12, 13 }, { 14, 15, 16, 17 }, { 18 }, { 19, 20, 21,
					22 } };
	ASSERT_EQUAL(lines, expected);
}

void testMultiThread()
{
	std::vector<std::vector<int>> matrix;
	const size_t matrix_size = 7000;
	const size_t page_size = 2000;

	matrix = makeMultiThread(matrix_size, page_size);
}

int main(void)
{
	TestRunner tr;
	RUN_TEST(tr, TestPageCounts);
	RUN_TEST(tr, TestLooping);
	RUN_TEST(tr, TestModification);
	RUN_TEST(tr, TestPageSizes);
	RUN_TEST(tr, TestConstContainer);
	RUN_TEST(tr, TestPagePagination);

	testMultiThread();

	return 0;
}