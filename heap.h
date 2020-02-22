#pragma once
#ifndef _TINYSTL_HEAP_H_
#define _TINYSTL_HEAP_H_

#include "iterator.h"

namespace TinySTL
{
	template <class RandomIter, class Distance, class T, class Compare>
	void _push_heap(RandomIter first, Distance now, Distance limit, T val, Compare comp)
	{
		Distance parent = (now - 1) / 2;
		while (now > limit && comp(*(first + parent), val))
		{
			*(first + now) = *(first + parent);
			now = parent;
			parent = (now - 1) / 2;
		}
		*(first + now) = val;
	}

	template <class RandomIter>
	void push_heap(RandomIter first, RandomIter last)
	{
		return push_heap(first, last, less<>());
	}

	template <class RandomIter, class Compare>
	void push_heap(RandomIter first, RandomIter last, Compare comp)
	{
		typedef typename iterator_traits<RandomIter>::difference_type Distance;
		_push_heap(first, last, Distance(last - first - 1), Distance(0), *(last - 1), comp);
	}

	template <class RandomIter, class Distance, class T, class Compare>
	void _adjust_heap(RandomIter first, Distance now, Distance limit, T val, Compare comp)
	{
		Distance aimchild = 2 * now + 2;
		Distance next_limit = now;
		while (aimchild < limit)
		{
			if (comp(*(first + aimchild), *(first + aimchild - 1)))
				--aimchild;
			*(first + now) = *(first + aimchild);
			now = aimchild;
			aimchild = now * 2 + 2;
		}
		if (aimchild == limit)
		{
			--aimchild;
			*(first + now) = *(first + aimchild);
			now = aimchild;
		}
		_push_heap(first, now, next_limit, val, comp);
	}

	template <class RandomIter, class Compare, class T>
	void _pop_heap(RandomIter first, RandomIter last,
				   RandomIter result, T val, Compare comp)
	{
		typedef typename iterator_traits<RandomIter>::difference_type Distance;
		*result = *first;
		_adjust_heap(first, Distance(0), Distance(last - first), val, comp);
	}

	template <class RandomIter>
	void pop_heap(RandomIter first, RandomIter last)
	{
		pop_heap(first, last, less<>());
	}

	template <class RandomIter, class Compare>
	void pop_heap(RandomIter first, RandomIter last, Compare comp)
	{
		typedef typename iterator_traits<RandomIter>::difference_type Distance;
		auto val = *--last;
		*last = *first;
		_adjust_heap(first, Distance(0), Distance(last - first), val, comp);
	}

	template <class RandomIter>
	void make_heap(RandomIter first, RandomIter last)
	{
		make_heap(first, last, less<>());
	}

	template <class RandomIter, class Compare>
	void make_heap(RandomIter first, RandomIter last, Compare comp)
	{
		if (last - first < 2)return;
		auto limit = last - first;
		auto parent = (limit - 2) / 2;
		for (;;)
		{
			_adjust_heap(first, parent, limit, *(first + parent), comp);
			if (parent == 0)return;
			--parent;
		}
	}

	template <class RandomIter>
	void sort_heap(RandomIter first, RandomIter last)
	{
		sort_heap(first, last, less<>());
	}

	template <class RandomIter, class Compare>
	void sort_heap(RandomIter first, RandomIter last, Compare comp)
	{
		while (last - first > 1)
		{
			pop_heap(first, last--, comp);
		}
	}
}

#endif /* _TINYSTL_HEAP_H_ */