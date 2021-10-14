#pragma once
#include <vector>

template<typename Iterator>
class IteratorRange
{
public:
	IteratorRange(Iterator b, Iterator e) : _begin(b), _end(e) {
		_size = std::distance(_begin, _end);
	};

	Iterator begin() const {
		return _begin;
	}

	Iterator end() const {
		return _end;
	}

	size_t size() const {
		return _size;
	}

private:
	Iterator _begin;
	Iterator _end;
	size_t _size;
};


template<typename Iterator>
class Paginator
{
public:
	Paginator(Iterator b, Iterator e, size_t pages)
	{
		Iterator first = b;

		while (first != e)
		{
			size_t left = std::distance(first, e);
			Iterator second = std::next(first, std::min(left, pages));
			ranges.emplace_back(first, second);
			first = second;
		}
	}

	auto begin() const {
		return ranges.begin();
	}

	auto end() const {
		return ranges.end();
	}

	size_t size() const {
		return ranges.size();
	}

private:
	std::vector<IteratorRange<Iterator>> ranges;
};


// Разбивает контейнер на страницы
template<typename Container>
auto Paginate(Container& c, size_t pages) {
	return Paginator<decltype(c.begin())>(c.begin(), c.end(), pages);
}

// Пзять префикс контейнера определенного размера
template<typename Container>
auto Head(Container& c, size_t size) {
	return IteratorRange<decltype(c.begin())>(c.begin(), c.begin() + std::min<size_t>(std::max(size, 0), c.size()));
}

