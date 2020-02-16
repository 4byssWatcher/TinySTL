#pragma once
#ifndef _TINYSTL_ALGORITHM_H_
#define _TINYSTL_ALGORITHM_H_

#include <random>

#include "functional.h"
#include "heap.h"
#include "iterator.h"
#include "utility.h"

const int SORT_THRESHOLD = 16;
const int STABLE_SORT_THRESHOLD = 15;

namespace TinySTL
{
	template <class InputIter, class UnaryPredicate>
	bool all_of(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (!pred(*first))
				return false;
		}
		return true;
	}

	template <class InputIter, class UnaryPredicate>
	bool any_of(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))
				return true;
		}
		return false;
	}

	template <class InputIter, class UnaryPredicate>
	bool none_of(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))
				return false;
		}
		return true;
	}

	template <class T>
	inline const T& median(const T& x, const T& y, const T& z)
	{
		return median(x, y, z, less<>());
	}

	template <class T, class Compare>
	inline const T& median(const T& x, const T& y,
						   const T& z, Compare comp)
	{
		if (comp(x, y))
		{
			if (comp(y, z))return y;
			else if (comp(x, z))return z;
			else return x;
		}
		else if (comp(x, z))return x;
		else if (comp(y, z))return z;
		else return y;
	}

	/* return func : for purpose like getting the sum/total execution count? */
	template <class InputIter, class Function>
	Function for_each(InputIter first, InputIter last, Function func)
	{
		for (; first != last; ++first)
			func(first);
		return move(func);
	}

	/* 
		SGI-STL has another implementation of find()/find_...() 
			for random-access iterator, not sure why
	*/
	template <class InputIter, class T>
	InputIter find(InputIter first, InputIter last, const T& val)
	{
		for (; first != last; ++first)
		{
			if (*first == val)return first;
		}
		return last;
	}

	template <class InputIter, class UnaryPredicate>
	InputIter find_if(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))return first;
		}
		return last;
	}

	template <class InputIter, class UnaryPredicate>
	InputIter find_if_not(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (!pred(*first))return first;
		}
		return last;
	}

	template <class ForwardIter1, class ForwardIter2>
	ForwardIter1 find_end(ForwardIter1 first1, ForwardIter1 last1,
						  ForwardIter2 first2, ForwardIter2 last2)
	{
		return find_end(first1, last1, first2, last2, equal_to<>());
	}

	template <class ForwardIter1, class ForwardIter2, class BinaryPredicate>
	ForwardIter1 find_end(ForwardIter1 first1, ForwardIter1 last1,
						  ForwardIter2 first2, ForwardIter2 last2,
						  BinaryPredicate pred)
	{
		if (first2 == last2)return last1;
		ForwardIter1 ret = last1;
		while (first1 != last1)
		{
			ForwardIter1 it1 = first1;
			ForwardIter2 it2 = first2;
			while (pred(*it1, *it2))
			{
				++it1;
				++it2;
				if (it2 == last2) { ret = first1; break; }
				if (it1 == last1)return ret;
			}
			++first1;
		}
		return ret;
	}

	/*  not sure
	template <class BidirectIter1,class BidirectIter2>
	BidirectIter find_end(BidirectIter1 first1,BidirectIter1 last1,
						  BidirectIter2 first2,BidirectIter2 last2)
	{
		if(first2==last2)return last1;
		while(first1!=last1)
		{
			InputIter1 it1=last1;it1--;
			InputIter2 it2=last2;it2--;
			while(*it1==*it2)
			{
				if(it2==first2)return it1;
				if(it1==first1)return last1;
				--it1;
				--it2;
			}
			--last1;
		}
	}
	*/

	template <class InputIter1, class ForwardIter2>
	InputIter1 find_first_of(InputIter1 first1, InputIter1 last1,
							 ForwardIter2 first2, ForwardIter2 last2)
	{
		return find_first_of(first1, last1, first2, last2, equal_to<>());
	}

	template <class InputIter1, class ForwardIter2, class BinaryPredicate>
	InputIter1 find_first_of(InputIter1 first1, InputIter1 last1,
							 ForwardIter2 first2, ForwardIter2 last2,
							 BinaryPredicate pred)
	{
		if (first2 == last2)return last1;
		while (first1 != last1)
		{
			for (ForwardIter2 it2 = first2; it2 != last2; ++it2)
			{
				if (pred(*it2, *first1))
					return first1;
			}
			++first1;
		}
		return last1;
	}

	template <class ForwardIter>
	ForwardIter adjacent_find(ForwardIter first, ForwardIter last)
	{
		return adjacent_find(first, last, equal_to<>());
	}

	template <class ForwardIter, class BinaryPredicate>
	ForwardIter adjacent_find(ForwardIter first, ForwardIter last, BinaryPredicate pred)
	{
		ForwardIter it = first++;
		while (first != last)
		{
			if (pred(*first, *it))return it;
			it = first++;
		}
		return last;
	}

	template <class InputIter, class T>
	typename iterator_traits<InputIter>::difference_type
	count(InputIter first, InputIter last, const T& val)
	{
		typename iterator_traits<InputIter>::difference_type ret = 0;
		while (first != last)
		{
			if (*first == val)++ret;
			++first;
		}
		return ret;
	}

	template <class InputIter, class UnaryPredicate>
	typename iterator_traits<InputIter>::difference_type
	count_if(InputIter first, InputIter last, UnaryPredicate pred)
	{
		typename iterator_traits<InputIter>::difference_type ret = 0;
		while (first != last)
		{
			if (pred(*first))++ret;
			++first;
		}
		return ret;
	}

	template <class InputIter1, class InputIter2>
	pair<InputIter1, InputIter2>
	mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		return mismatch(first1, last1, first2, equal_to<>());
	}

	template <class InputIter1, class InputIter2, class BinaryPredicate>
	pair<InputIter1, InputIter2>
	mismatch(InputIter1 first1, InputIter1 last1,
			 InputIter2 first2, BinaryPredicate pred)
	{
		while (first1 != last1 && pred(*first1, *first2))
		{
			++first1;
			++first2;
		}
		return make_pair(first1, first2);
	}

	template <class InputIter1, class InputIter2>
	bool equal(InputIter1 first1, InputIter1 last1,
			   InputIter2 first2)
	{
		return equal(first1, last1, first2, equal_to<>());
	}

	template <class InputIter1, class InputIter2, class BinaryPredicate>
	bool equal(InputIter1 first1, InputIter1 last1,
			   InputIter2 first2, BinaryPredicate pred)
	{
		while (first1 != last1)
		{
			if (!pred(*first1, *first2))return false;
			++first1;
			++first2;
		}
		return true;
	}

	template <class InputIter1, class InputIter2>
	bool is_permutation(InputIter1 first1, InputIter1 last1,
						InputIter2 first2)
	{
		return is_permutation(first1, last1, first2, less<>());
	}

	template <class InputIter1, class InputIter2, class BinaryPredicate>
	bool is_permutation(InputIter1 first1, InputIter1 last1,
						InputIter2 first2, BinaryPredicate pred)
	{
		tie(first1, first2) = mismatch(first1, last1, first2, pred);
		if (first1 == last1)return true;
		InputIter2 last2 = first2;
		advance(last2, distance(first1, last1));
		for (InputIter1 it1 = first1; it1 != last1; ++it1)
		{
			if (find_if(first1, last1, [&](InputIter1 _i1) {return pred(*_i1, *it1); }) == it1)
			{
				auto n = count_if(first2, last2, [&](InputIter2 _i2) {return pred(*_i2, *it1); });
				if (n == 0 || count_if(first1, last1, [&](InputIter1 _i1) {return pred(*_i1, *it1); }) != n)
					return false;
			}
		}
		return true;
	}

	template <class ForwardIter1, class ForwardIter2>
	ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
						ForwardIter2 first2, ForwardIter2 last2)
	{
		return search(first1, last1, first2, last2, equal_to<>());
	}

	template <class ForwardIter1, class ForwardIter2, class BinaryPredicate>
	ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
						ForwardIter2 first2, ForwardIter2 last2,
						BinaryPredicate pred)
	{
		if (first2 == last2)return last1;
		while (first1 != last1)
		{
			ForwardIter1 it1 = first1;
			ForwardIter2 it2 = first2;
			while (pred(*it1, *it2))
			{
				++it1;
				++it2;
				if (it2 == last2)return first1;
				if (it1 == last1)return last1;
			}
		}
		return last1;
	}

	template <class ForwardIter, class Size, class T>
	ForwardIter search_n(ForwardIter first, ForwardIter last, Size n, const T& val)
	{
		Size i;
		ForwardIter limit = first;
		advance(limit, distance(first, last) - n);
		while (first != limit)
		{
			ForwardIter it = first;
			while (*it == val)
			{
				++it;
				if (++i == n)return first;
			}
			++first;
		}
		return last;
	}

	template <class ForwardIter, class Size, class UnaryPredicate>
	ForwardIter search_n(ForwardIter first, ForwardIter last,
						 Size n, UnaryPredicate pred)
	{
		Size i;
		ForwardIter limit = first;
		advance(limit, distance(first, last) - n);
		while (first != limit)
		{
			ForwardIter it = first;
			while (pred(*it))
			{
				++it;
				if (++i == n)return first;
			}
			++first;
		}
		return last;
	}

	template <class InputIter, class OutputIter>
	OutputIter copy(InputIter first, InputIter last, OutputIter result)
	{
		while (first != last)
		{
			*result = *first;
			++first;
			++result;
		}
		return result;
	}

	template <class InputIter, class Size, class OutputIter>
	OutputIter copy_n(InputIter first, Size n, OutputIter result)
	{
		while (n > 0)
		{
			*result = *first;
			++first;
			++result;
			--n;
		}
		return result;
	}

	template <class InputIter, class OutputIter, class UnaryPredicate>
	OutputIter copy_if(InputIter first, InputIter last,
					   OutputIter result, UnaryPredicate pred)
	{
		while (first != last)
		{
			if (pred(*first))
			{
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}

	template <class BidirectIter1, class BidirectIter2>
	BidirectIter2 copy_backward(BidirectIter1 first, BidirectIter1 last,
								BidirectIter2 result)
	{
		while (first != last)
			*(--result) = *(--last);
		return result;
	}

	template <class InputIter, class Size, class OutputIter>
	OutputIter move(InputIter first, InputIter last, OutputIter result)
	{
		while (first != last)
		{
			*result = move(*first);
			++first;
			++result;
		}
		return result;
	}

	template <class BidirectIter1, class BidirectIter2>
	BidirectIter2 move_backward(BidirectIter1 first, BidirectIter1 last,
								BidirectIter2 result)
	{
		while (first != last)
			*(--result) = move(*(--last));
		return result;
	}

	template <class InputIter1, class InputIter2>
	InputIter2 swap_range(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		while (first1 != last1)
		{
			swap(*first1, *first2);
			++first1;
			++first2;
		}
		return first2;
	}

	template <class InputIter1, class InputIter2>
	void iter_swap(InputIter1 x, InputIter2 y)
	{
		swap(*x, *y);
	}

	template <class InputIter, class OutputIter, class UnaryOperator>
	OutputIter transform(InputIter first1, InputIter last1,
						 OutputIter first2, UnaryOperator op)
	{
		while (first1 != last1)
		{
			*first2 = op(*first1);
			++first1;
			++first2;
		}
		return first2;
	}

	template <class ForwardIter, class T>
	void replace(ForwardIter first, ForwardIter last,
				 const T& old_value, const T& new_value)
	{
		while (first != last)
		{
			if (*first == old_value)
				*first = new_value;
			++first;
		}
	}

	template <class ForwardIter, class T, class UnaryPredicate>
	void replace_if(ForwardIter first, ForwardIter last,
					const T& new_value, UnaryPredicate pred)
	{
		while (first != last)
		{
			if (pred(*first))
				*first = new_value;
			++first;
		}
	}

	template <class InputIter, class OutputIter, class T>
	OutputIter replace_copy(InputIter first, InputIter last, OutputIter result,
							const T& old_value, const T& new_value)
	{
		while (first != last)
		{
			*result = (*first == old_value) ? new_value : *first;
			++first;
			++result;
		}
		return result;
	}

	template <class InputIter, class OutputIter, class T, class UnaryPredicate>
	OutputIter replace_copy_if(InputIter first, InputIter last, OutputIter result,
							   UnaryPredicate pred, const T& new_value)
	{
		while (first != last)
		{
			*result = (pred(*first)) ? new_value : *first;
			++first;
			++result;
		}
		return result;
	}

	template <class ForwardIter, class T>
	void fill(ForwardIter first, ForwardIter last, const T& val)
	{
		while (first != last)
		{
			*first = val;
			++first;
		}
	}

	template <class ForwardIter, class Size, class T>
	ForwardIter fill_n(ForwardIter first, ForwardIter last, Size n, const T& val)
	{
		while (n > 0)
		{
			*first = val;
			++first; --n;
		}
		return first;
	}

	template <class ForwardIter, class Generator>
	void generate(ForwardIter first, ForwardIter last, Generator gen)
	{
		while (first != last)
		{
			*first = gen();
			++first;
		}
	}

	template <class ForwardIter, class Size, class Generator>
	ForwardIter generate_n(ForwardIter first, Size n, Generator gen)
	{
		while (n > 0)
		{
			*first = gen();
			++first; --n;
		}
		return first;
	}

	template <class ForwardIter, class T>
	ForwardIter remove(ForwardIter first, ForwardIter last, const T& val)
	{
		ForwardIter result = first;
		while (first != last)
		{
			if (!(*first == val))
			{
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}

	template <class ForwardIter, class UnaryPredicate>
	ForwardIter remove_if(ForwardIter first, ForwardIter last, UnaryPredicate pred)
	{
		ForwardIter result = first;
		while (first != last)
		{
			if (!pred(*first))
			{
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}

	template <class InputIter, class OutputIter, class T>
	OutputIter remove_copy(InputIter first, InputIter last,
						   OutputIter result, const T& val)
	{
		while (first != last)
		{
			if (!(*first == val))
			{
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}

	template <class InputIter, class OutputIter, class T, class UnaryPredicate>
	OutputIter remove_copy_if(InputIter first, InputIter last,
							  OutputIter result, UnaryPredicate pred)
	{
		while (first != last)
		{
			if (!pred(*first))
			{
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}

	template <class ForwardIter>
	ForwardIter unique(ForwardIter first, ForwardIter last)
	{
		return unique(first, last, equal_to<>());
	}

	template <class ForwardIter, class BinaryPredicate>
	ForwardIter unique(ForwardIter first, ForwardIter last,
					   BinaryPredicate pred)
	{
		if (first == last)return last;
		ForwardIter result = first;
		while (++first != last)
		{
			if (!pred(*result, *first))
				*(++result) = *first;
		}
		return ++result;
	}

	template <class InputIter, class OutputIter>
	OutputIter unique_copy(InputIter first, InputIter last, OutputIter result)
	{
		return unique_copy(first, last, result, equal_to<>());
	}

	template <class InputIter, class OutputIter, class BinaryPredicate>
	OutputIter unique_copy(InputIter first, InputIter last,
						   OutputIter result, BinaryPredicate pred)
	{
		if (first == last)return result;
		*result = *first;
		while (++first != last)
		{
			if (!pred(*result, *first))
				*(++result) = *first;
		}
		return ++result;
	}

	template <class BidirectIter>
	void reverse(BidirectIter first, BidirectIter last)
	{
		while ((first != last) && (first != --last))
		{
			iter_swap(first, last);
			++first;
		}
	}

	template <class BidirectIter, class OutputIter>
	OutputIter reverse_copy(BidirectIter first, BidirectIter last, OutputIter result)
	{
		while (first != last)
		{
			--last;
			*result = *last;
			++result;
		}
		return result;
	}

	template <class ForwardIter>
	ForwardIter rotate(ForwardIter first, ForwardIter middle, ForwardIter last)
	{
		ForwardIter next = middle;
		while (first != next)
		{
			iter_swap(first++, next++);
			if (next == last)next = middle;
			else if (first == middle)middle = next;
		}
	}

	template <class ForwardIter, class OutputIter>
	ForwardIter rotate_copy(ForwardIter first, ForwardIter middle,
							ForwardIter last, OutputIter result)
	{
		result = copy(middle, last, result);
		return copy(first, middle, result);
	}

	/*
		-better use shuffle(), for numbers generated by rand()%(n+1)
			aren't uniformly distributed in most cases.
		-may produce the same result each time because of the seed.
	*/
	template <class RandomIter>
	void random_shuffle(RandomIter first, RandomIter last)
	{
		for (auto i = (last - first) - 1; i > 0; --i)
			swap(first[i], first[rand() % (i + 1)]);
	}

	template <class RandomIter, class URNG>
	void shuffle(RandomIter first, RandomIter last, URNG&& gen)
	{
		for (auto i = (last - first) - 1; i > 0; --i)
		{
			std::uniform_int_distribution<decltype(i)> d(0, i);
			swap(first[i], first[d(gen)]);
		}
	}

	template <class InputIter, class UnaryPredicate>
	bool is_partitioned(InputIter first, InputIter last, UnaryPredicate pred)
	{
		while (first != last && pred(*first))
			++first;
		while (first != last)
		{
			if (pred(*first)) return false;
			++first;
		}
		return true;
	}

	template <class ForwardIter, class UnaryPredicate>
	ForwardIter _partition(ForwardIter first, ForwardIter last,
						   UnaryPredicate pred, forward_iterator_tag)
	{
		if (first == last)return first;
		while (pred(*first))
		{
			if (++first == last)
				return first;
		}
		ForwardIter next = first;
		while (++next != last)
		{
			if (pred(*next))
			{
				swap(*first, *next);
				++first;
			}
		}
		return first;
	}

	template <class BidirectIter, class UnaryPredicate>
	BidirectIter _partition(BidirectIter first, BidirectIter last,
						    UnaryPredicate pred, bidirectional_iterator_tag)
	{
		while (first != last)
		{
			while (pred(*first))
			{
				++first;
				if (first == last)return first;
			}
			do
			{
				--last;
				if (first == last)return first;
			} while (!pred(*last));
			swap(*first, *last);
			++first;
		}
		return first;
	}

	template <class Iterator, class Predicate>
	Iterator partition(Iterator first, Iterator last, Predicate pred)
	{
		return _partition(first, last, pred, iterator_category(first));
	}

	template <class ForwardIter, class UnaryPredicate, class Distance>
	ForwardIter inplace_stable_partition(ForwardIter first, ForwardIter last,
										 UnaryPredicate pred, Distance len)
	{
		if (len == 1)return pred(*first) ? last : first;
		ForwardIter middle = first;
		advance(middle, len / 2);
		return rotate(inplace_stable_partition(first, middle, pred, len / 2), 
					  middle,
					  inplace_stable_partition(middle, last, pred, len - len / 2));
	}


	template <class ForwardIter, class Pointer, class UnaryPredicate>
	ForwardIter adaptive_stable_partition(ForwardIter first, ForwardIter last,
										  UnaryPredicate pred, Pointer ptr)
	{
		ForwardIter result1 = first;
		ForwardIter result2 = ptr;
		while (first != last)
		{
			if (pred(*first))
			{
				*result1 = *first;
				++result1;
			}
			else
			{
				*result2 = *first;
				++result2;
			}
			++first;
		}
		copy(ptr, result2, result1);
		free(ptr);
		return result1;
	}

	/*
		-get_temporary_buffer() have been deprecated in C++17.
		-though using get_temporary_buffer() creates a chance to makes the funtion go faster,
		 this implementation only calls adaptive_stable_partition() when there's enough memory.
	*/
	template <class ForwardIter, class UnanryPredicate>
	ForwardIter stable_partition(ForwardIter first, ForwardIter last,
								 UnanryPredicate pred)
	{
		if (first == last)return first;
		typename iterator_traits<ForwardIter>::difference_type dis;
		dis = distance(first, last);
		typename iterator_traits<ForwardIter>::pointer p;
		p = malloc(sizeof(iterator_traits<ForwardIter>::value_type) * dis);
		if (0 == p)return inplace_stable_partition(first, last, pred, dis);
		else return adaptive_stable_partition(first, last, pred, p);
	}

	template <class InputIter, class OutputIter1,
			 class OutputIter2, class UnaryPredicate>
	pair<OutputIter1, OutputIter2>
	partition_copy(InputIter first, InputIter last,
				   OutputIter1 result_true, OutputIter2 result_false,
				   UnaryPredicate pred)
	{
		while (first != last)
		{
			if (pred(*first))
			{
				*result_true = *first;
				++result_true;
			}
			else
			{
				*result_false = *first;
				++result_false;
			}
			++first;
		}
		return make_pair(result_true, result_false);
	}

	/* elements in [first,last) shall already be partitioned */
	template <class ForwardIter,class UnaryPredicate>
	ForwardIter partition_point(ForwardIter first, ForwardIter last,
								UnaryPredicate pred)
	{
		auto n = distance(first, last);
		while (n > 0)
		{
			ForwardIter it = first;
			auto step = n / 2;
			advance(it, step);
			if (pred(*it))
			{
				first = ++it;
				n -= step + 1;
			}
			else n = step;
		}
		return first;
	}

	template <class RandomIter,class T,class Compare>
	RandomIter unguarded_partition(RandomIter first, RandomIter last, 
								   T pivot, Compare comp)
	{
		while (true)
		{
			while (comp(*first, pivot))
				++first;
			--last;
			while (comp(pivot, *last))
				--last;
			if (!(first < last))
				return first;
			iter_swap(first, last);
			++first;
		}
	}

	template <class RandomIter, class Size>
	void introsort(RandomIter first, RandomIter last, Size depth_limit)
	{
		introsort(first, last, depth_limit, less<>());
	}

	template <class RandomIter, class Size, class Compare>
	void introsort(RandomIter first, RandomIter last, 
				   Size depth_limit, Compare comp)
	{
		while (last - first > SORT_THRESHOLD)
		{
			if (depth_limit == 0)
			{
				partial_sort(first, last, last, comp);
				return;
			}
			--depth_limit;
			RandomIter cut = unguarded_partition(first, last, 
												 median(*first,
														*(first + (last - first) / 2),
														*(last - 1), comp));
			introsort(cut, last, depth_limit, comp);
			last = cut;
		}
		insertion_sort(first, last, comp);
	}

	template <class RandomIter>
	void sort(RandomIter first, RandomIter last)
	{
		if (first != last)
			introsort(first, last, log(last - first) * 2);
	}

	template <class RandomIter, class Compare>
	void sort(RandomIter first, RandomIter last, Compare comp)
	{
		if (first != last)
			introsort(first, last, log(last - first) * 2, comp);
	}

	template <class RandomIter>
	void inplace_stable_sort(RandomIter first, RandomIter last)
	{
		inplace_stable_sort(first, last, less<>());
	}

	template <class RandomIter, class Compare>
	void inplace_stable_sort(RandomIter first, RandomIter last, Compare comp)
	{
		if (last - first < STABLE_SORT_THRESHOLD)
		{
			insertion_sort(first, last, comp);
			return;
		}
		RandomIter mid = first + (last - first) / 2;
		inplace_stable_sort(first, mid, comp);
		inplace_stable_sort(mid, last, comp);
		inplace_merge(first, mid, last, comp);
	}

	template <class RandomIter, class Pointer>
	void adaptive_stable_sort(RandomIter first, RandomIter last,
							  Pointer ptr)
	{
		adaptive_stable_sort(first, last, ptr, less<>());
	}

	template <class RandomIter, class Pointer, class Compare>
	void adaptive_stable_sort(RandomIter first, RandomIter last, 
							  Pointer ptr, Compare comp)
	{
		if (last - first == 1)return;
		RandomIter mid = first + (last - first) / 2;
		adaptive_stable_sort(first, mid, comp);
		adaptive_stable_sort(mid, last, comp);
		auto ptr_last = merge(first, mid, mid, last, ptr, comp);
		copy(ptr, ptr_last, first);
	}

	template <class RandomIter>
	void stable_sort(RandomIter first, RandomIter last)
	{
		stable_sort(first, last, less<>());
	}

	template <class RandomIter, class Compare>
	void stable_sort(RandomIter first, RandomIter last, Compare comp)
	{
		if (first == last)return;
		auto dis = last - first;
		typename iterator_traits<RandomIter>::pointer p;
		p = malloc(sizeof(iterator_traits<RandomIter>::value_type) * dis);
		if (0 == p)inplace_stable_sort(first, last, comp);
		else adaptive_stable_sort(first, last, p, comp);
		free(p);
	}

	/* comp : must be a comparision between T and val_t*/

	template <class RandomIter, class T, class Compare>
	void unguarded_linear_insert(RandomIter last, T val, Compare comp)
	{
		RandomIter next = last - 1;
		while (comp(val, *next))
		{
			*last = *next;
			last = next;
			--next;
		}
		*last = val;
	}

	template <class RandomIter, class T>
	void linear_insert(RandomIter first, RandomIter last, T*)
	{
		linear_insert(first, last, T * (0), less<>());
	}

	template <class RandomIter, class Compare, class T>
	void linear_insert(RandomIter first, RandomIter last, Compare comp, T*)
	{
		T val = *last;
		if (comp(val, *first)) 
		{
			copy_backward(first, last, last + 1);
			*first = val;
		}
		else unguarded_linear_insert(last, val, comp);
	}

	template <class RandomIter>
	void insertion_sort(RandomIter first, RandomIter last)
	{
		insertion_sort(first, last, less<>());
	}

	template <class RandomIter, class Compare>
	void insertion_sort(RandomIter first, RandomIter last, Compare comp)
	{
		if (first == last)return;
		for (RandomIter i = first + 1; i != last; ++i)
			linear_insert(first, i, comp, value_type(first));
	}

	template <class RandomIter>
	void partial_sort(RandomIter first, RandomIter mid, RandomIter last)
	{
		partial_sort(first, mid, last, less<>());
	}

	template <class RandomIter, class Compare>
	void partial_sort(RandomIter first, RandomIter mid, 
					  RandomIter last, Compare comp)
	{
		heap_select(first, mid, last, comp);
		sort_heap(first, mid, comp);
	}

	template <class InputIter, class RandomIter>
	void partial_sort_copy(InputIter first, InputIter last,
						   RandomIter result_first, RandomIter result_last)
	{
		partial_sort_copy(first, last, result_first, result_last, less<>());
	}

	template <class InputIter, class RandomIter, class Compare>
	void partial_sort_copy(InputIter first, InputIter last, 
						   RandomIter result_first, RandomIter result_last,
						   Compare comp)
	{
		typedef typename iterator_traits<InputIter>::difference_type Distance;
		typedef typename iterator_traits<InputIter>::value_type		 T;
		if (result_first == result_last)
			return result_last;
		RandomIter result_now = result_first;
		while (first != last && result_now != result_last)
		{
			*result_now = *first;
			++result_now;
			++first;
		}
		make_heap(result_first, result_now, comp);
		while (first != last)
		{
			if (comp(first, result_first))
				__adjust_heap(result_first, Distance(0),
							  Distance(result_now - result_first),
							  T(*first));
			++first;
		}
		sort_heap(result_first, result_now);
		return result_now;
	}

	template <class ForwardIter>
	bool is_sorted(ForwardIter first, ForwardIter last)
	{
		return is_sorted(first, last, less<>());
	}

	template <class ForwardIter,class Compare>
	bool is_sorted(ForwardIter first, ForwardIter last, Compare comp)
	{
		if (first == last) return true;
		ForwardIter next = first;
		while (++next != last)
		{
			if (comp(*next, *first))
				return false;
			++first;
		}
		return true;
	}

	template <class ForwardIter>
	ForwardIter is_sorted_until(ForwardIter first, ForwardIter last)
	{
		return is_sorted_until(first, last, less<>());
	}

	template <class ForwardIter, class Compare>
	ForwardIter is_sorted_until(ForwardIter first, ForwardIter last, Compare comp)
	{
		if (first == last) return true;
		ForwardIter next = first;
		while (++next != last)
		{
			if (comp(*next, *first))
				return next;
			++first;
		}
		return true;
	}
	/* when last-first<=3, adopt insertion_sort()
	   when last-first>3 , adopt partition for quicksort (binary)  */
	template <class RandomIter>
	void nth_element(RandomIter first, RandomIter nth, RandomIter last)
	{
		nth_element(first, nth, last, less<>());
	}

	template <class RandomIter, class Compare, class T>
	void nth_element(RandomIter first, RandomIter nth, RandomIter last, Compare comp, T*)
	{
		while (last - first > 3)
		{
			RandomIter cut = unguarded_partition(first, last,
												 median(*first,
														*(first + (last - first) / 2),
														*(last-1), comp));
			if (cut <= nth)first = cut;
			else last = cut;
		}
		insertion_sort(first, last, comp);
	}

	template <class ForwardIter, class T>
	ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& val)
	{
		return lower_bound(first, last, val, less<>());
	}

	template <class ForwardIter, class T, class Compare>
	ForwardIter lower_bound(ForwardIter first, ForwardIter last,
							const T& val, Compare comp)
	{
		ForwardIter it;
		typename iterator_traits<ForwardIter>::difference_type count, step;
		count = distance(first, last);
		while (count > 0) 
		{
			it = first;
			step = count >> 1;
			advance(it, step);
			if (comp(*it, val)) 
			{
				first = it;
				++first;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}

	template <class ForwardIter, class T>
	ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& val)
	{
		return upper_bound(first, last, val, less<>());
	}

	template <class ForwardIter, class T, class Compare>
	ForwardIter upper_bound(ForwardIter first, ForwardIter last, 
							const T& val, Compare comp)
	{
		ForwardIter it;
		typename iterator_traits<ForwardIter>::difference_type count, step;
		count = distance(first, last);
		while (count > 0) 
		{
			it = first;
			step = count / 2;
			advance(it, step);
			if (!comp(val, *it)) 
			{
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}

	template <class ForwardIter, class T>
	pair<ForwardIter, ForwardIter>
	equal_range(ForwardIter first, ForwardIter last, const T& val)
	{
		return equal_range(first, last, val, less<>());
	}

	template <class ForwardIter, class T, class Compare>
	pair<ForwardIter, ForwardIter>
	equal_range(ForwardIter first, ForwardIter last, 
				const T& val, Compare comp)
	{
		ForwardIter it = lower_bound(first, last, val, comp);
		return make_pair(it, upper_bound(it, last, val, comp));
	}

	template <class ForwardIter, class T>
	bool binary_search(ForwardIter first, ForwardIter last, const T& val)
	{
		return binary_search(first, last, val, less<>());
	}

	template <class ForwardIter, class T, class Compare>
	bool binary_search(ForwardIter first, ForwardIter last, 
					   const T& val, Compare comp)
	{
		first = lower_bound(first, last, val, comp);
		return (first != last && !comp(val, *first));
	}

	template <class InputIter1, class InputIter2, class OutputIter>
	OutputIter merge(InputIter1 first1, InputIter1 last1,
					 InputIter2 first2, InputIter2 last2, OutputIter result)
	{
		return merge(first1, last1, first2, last2, result, less<>());
	}

	template <class InputIter1, class InputIter2, class OutputIter, class Compare>
	OutputIter merge(InputIter1 first1, InputIter1 last1,
					 InputIter2 first2, InputIter2 last2, OutputIter result,
					 Compare comp)
	{
		while (true) 
		{
			if (first1 == last1) return copy(first2, last2, result);
			if (first2 == last2) return copy(first1, last1, result);
			*result++ = comp(*first2, *first1) ? *first2++ : *first1++;
		}
	}

	template <class BidirectIter, class Distance>
	void merge_without_buffer(BidirectIter first, BidirectIter mid,
							  BidirectIter last, Distance len1, Distance len2)
	{
		merge_without_buffer(first, mid, last, len1, len2, less<>());
	}

	template <class BidirectIter, class Distance, class Compare>
	void merge_without_buffer(BidirectIter first, BidirectIter mid,
							  BidirectIter last, Distance len1, Distance len2,
							  Compare comp) 
	{
		if (len1 == 0 || len2 == 0)
			return;
		if (len1 + len2 == 2) 
		{
			if (comp(*mid, *first))
				iter_swap(first, mid);
			return;
		}
		BidirectIter first_cut = first;
		BidirectIter second_cut = mid;
		Distance len11 = 0;
		Distance len22 = 0;
		if (len1 > len2) 
		{
			len11 = len1 / 2;
			advance(first_cut, len11);
			second_cut = lower_bound(mid, last, *first_cut, comp);
			distance(mid, second_cut, len22);
		}
		else 
		{
			len22 = len2 / 2;
			advance(second_cut, len22);
			first_cut = upper_bound(first, mid, *second_cut, comp);
			distance(first, first_cut, len11);
		}
		BidirectIter new_mid = rotate(first_cut, mid, second_cut);
		merge_without_buffer(first, first_cut, new_mid, len11, len22, comp);
		merge_without_buffer(new_mid, second_cut, last, 
							 len1 - len11, len2 - len22, 
							 comp);
	}

	template <class BidirectIter>
	void inplace_merge(BidirectIter first, BidirectIter mid, BidirectIter last)
	{
		inplace_merge(first, mid, last, less<>());
	}
	

	template <class BidirectIter, class Compare>
	void inplace_merge(BidirectIter first, BidirectIter mid,
					   BidirectIter last, Compare comp) 
	{
		if (first == mid || mid == last)return;
		typedef typename iterator_traits<BidirectIter>::difference_type  Distance;
		typedef typename iterator_traits<BidirectIter>::pointer			 Pointer;
		typedef typename iterator_traits<BidirectIter>::value_type		 T;
		Distance len1 = distance(first, mid);
		Distance len2 = distance(mid, last);

		Pointer ptr = malloc(sizeof(T) * len1);
		if (ptr == 0)
			merge_without_buffer(first, mid, last, len1, len2, comp);
		else
		{
			Pointer ptr_end = copy(first, mid, ptr);
			merge(ptr, ptr_end, mid, last, first, comp);
		}
		free(ptr);
	}

	template <class InputIter1, class InputIter2>
	bool includes(InputIter1 first1, InputIter1 last1,
				  InputIter2 first2, InputIter2 last2)
	{
		return includes(first1, last1, first2, last2, less<>());
	}

	template <class InputIter1, class InputIter2, class Compare>
	bool includes(InputIter1 first1, InputIter1 last1,
				  InputIter2 first2, InputIter2 last2, Compare comp)
	{
		while (first2 != last2) 
		{
			if ((first1 == last1) || comp(*first2, *first1))
				return false;
			if (!comp(*first1, *first2)) 
				++first2;
			++first1;
		}
		return true;
	}

	template <class InputIter1, class InputIter2, class OutputIter>
	OutputIter set_union(InputIter1 first1, InputIter1 last1,
						 InputIter2 first2, InputIter2 last2, OutputIter result)
	{
		return set_union(first1, last1, first2, last2, result, less<>());
	}

	template <class InputIter1, class InputIter2, class OutputIter, class Compare>
	OutputIter set_union(InputIter1 first1, InputIter1 last1,
						 InputIter2 first2, InputIter2 last2,
						 OutputIter result, Compare comp)
	{
		while (true)
		{
			if (first1 == last1) return copy(first2, last2, result);
			if (first2 == last2) return copy(first1, last1, result);
			if (comp(*first1, *first2)) { *result = *first1; ++first1; }
			else if (comp(*first2, *first1)) { *result = *first2; ++first2; }
			else { *result = *first1; ++first1; ++first2; }
			++result;
		}
	}

	template <class InputIter1, class InputIter2, class OutputIter>
	OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
								InputIter2 first2, InputIter2 last2, OutputIter result)
	{
		return set_intersection(first1, last1, first2, last2, result, less<>());
	}

	template <class InputIter1, class InputIter2, class OutputIter, class Compare>
	OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
								InputIter2 first2, InputIter2 last2,
								OutputIter result, Compare comp)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (comp(*first1, *first2)) ++first1;
			else if (comp(*first2, *first1)) ++first2;
			else 
			{
				*result = *first1;
				++result; ++first1; ++first2;
			}
		}
		return result;
	}

	template <class InputIter1, class InputIter2, class OutputIter>
	OutputIter set_difference(InputIter1 first1, InputIter1 last1,
							  InputIter2 first2, InputIter2 last2, OutputIter result)
	{
		return set_difference(first1, last1, first2, last2, result, less<>());
	}

	template <class InputIter1, class InputIter2, class OutputIter, class Compare>
	OutputIter set_difference(InputIter1 first1, InputIter1 last1,
							  InputIter2 first2, InputIter2 last2,
							  OutputIter result, Compare comp)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (comp(*first1, *first2)) { *result = *first1; ++result; ++first1; }
			else if (comp(*first2, *first1)) ++first2;
			else { ++first1; ++first2; }
		}
		return copy(first1, last1, result);
	}

	template <class InputIter1, class InputIter2, class OutputIter>
	OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
										InputIter2 first2, InputIter2 last2,
										OutputIter result)
	{
		return set_symmetric_difference(first1, last1, first2, last2, result, less<>());
	}

	template <class InputIter1, class InputIter2, class OutputIter, class Compare>
	OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
										InputIter2 first2, InputIter2 last2,
										OutputIter result, Compare comp)
	{
		while (true)
		{
			if (first1 == last1) return copy(first2, last2, result);
			if (first2 == last2) return copy(first1, last1, result);
			if (comp(*first1, *first2)) { *result = *first1; ++result; ++first1; }
			else if (comp(*first2, *first1)) { *result = *first2; ++result; ++first2; }
			else { ++first1; ++first2; }
		}
	}

	template <class T>
	const T& min(const T& a, const T& b)
	{
		return !(b < a) ? a : b;
	}

	template <class T>
	const T& max(const T& a, const T& b)
	{
		return a < b ? b : a;
	}

	template <class T>
	pair <const T&, const T&>
	minmax(const T& a, const T& b) 
	{
		return (b < a) ? make_pair(b, a) : make_pair(a, b);
	}

	template <class ForwardIter>
	ForwardIter min_element(ForwardIter first, ForwardIter last)
	{
		return min_element(first, last, less<>());
	}

	template <class ForwardIter, class Compare>
	ForwardIter min_element(ForwardIter first, ForwardIter last, Compare comp)
	{
		if (first == last) return last;
		ForwardIter smallest = first;
		while (++first != last)
		{
			if (comp(*first, *smallest))
				smallest = first;
		}
		return smallest;
	}

	template <class ForwardIter>
	ForwardIter max_element(ForwardIter first, ForwardIter last)
	{
		return max_element(first, last, less<>());
	}

	template <class ForwardIter, class Compare>
	ForwardIter max_element(ForwardIter first, ForwardIter last, Compare comp)
	{
		if (first == last) return last;
		ForwardIter largest = first;
		while (++first != last)
		{
			if (comp(*largest, *first))
				largest = first;
		}
		return largest;
	}

	template <class ForwardIter>
	pair<ForwardIter, ForwardIter>
	minmax_element(ForwardIter first, ForwardIter last)
	{
		return minmax_element(first, last, less<>());
	}

	template <class ForwardIter, class Compare>
	pair<ForwardIter, ForwardIter>
	minmax_element(ForwardIter first, ForwardIter last, Compare comp)
	{
		if (first == last) return last;
		ForwardIter largest = first;
		ForwardIter smallest = first;
		while (++first != last)
		{
			if (comp(*first, *smallest))
				smallest = first;
			if (*largest < *first)
				largest = first;
		}
		return make_pair(smallest, largest);
	}

	template <class InputIter1, class InputIter2>
	bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
								 InputIter2 first2, InputIter2 last2)
	{
		return lexicographical_compare(first1, last1, first2, last2, less<>());
	}

	template <class InputIter1, class InputIter2, class Compare>
	bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
								 InputIter2 first2, InputIter2 last2, Compare comp)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || comp(*first2, *first1))
				return false;
			else if (comp(*first1, *first2))
				return true;
			++first1; ++first2;
		}
		return (first2 != last2);
	}

	template <class BidirectIter>
	bool next_permutation(BidirectIter first, BidirectIter last)
	{
		return next_permutation(first, last, less<>());
	}

	template <class BidirectIter, class Compare>
	bool next_permutation(BidirectIter first, BidirectIter last, Compare comp)
	{
		if (first == last)return false;
		BidirectIter i = first;
		++i;
		if (i == last)return false;
		i = last;
		--i;
		for (;;)
		{
			BidirectIter ii = i;
			--i;
			if (comp(*i, *ii))// if (*i < *(i+1)) 
			{
				BidirectIter j = last;
				while (!comp(*i, *--j));// find the last j satisfying (*j > *i)
				iter_swap(i, j);
				reverse(ii, last);
				return true;
			}
			if (i == first)// inverted order
			{
				reverse(first, last);
				return false;
			}
		}
	}

	template <class BidirectIter>
	bool prev_permutation(BidirectIter first, BidirectIter last)
	{
		return prev_permutation(first, last, less<>());
	}

	template <class BidirectIter, class Compare>
	bool prev_permutation(BidirectIter first, BidirectIter last, Compare comp)
	{
		if (first == last)return false;
		BidirectIter i = first;
		++i;
		if (i == last)return false;
		i = last;
		--i;
		for (;;)
		{
			BidirectIter ii = i;
			--i;
			if (comp(*ii, *i))
			{
				BidirectIter j = last;
				while (!comp(*--j, *i));
				iter_swap(i, j);
				reverse(ii, last);
				return true;
			}
			if (i == first)
			{
				reverse(first, last);
				return false;
			}
		}
	}

}

#endif /* TINYSTL_ALGORITHM_H_ */