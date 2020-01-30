#pragma once
#ifndef _TINYSTL_ALGORITHM_H_
#define _TINYSTL_ALGORITHM_H_

#include <random>
#include "iterator.h"
#include "utility.h"

namespace TinySTL
{
	template<class InputIter, class UnaryPredicate>
	bool all_of(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (!pred(*first))
				return false;
		}
		return true;
	}

	template<class InputIter, class UnaryPredicate>
	bool any_of(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))
				return true;
		}
		return false;
	}

	template<class InputIter, class UnaryPredicate>
	bool none_of(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))
				return false;
		}
		return true;
	}

	/* return func:for purpose like getting the sum/total execution count? */
	template<class InputIter, class Function>
	Function for_each(InputIter first, InputIter last, Function func)
	{
		for (; first != last; ++first)
			func(first);
		return move(func);
	}

	template<class InputIter, class T>
	InputIter find(InputIter first, InputIter last, const T& val)
	{
		for (; first != last; ++first)
		{
			if (*first == val)return first;
		}
		return last;
	}

	template<class InputIter, class UnaryPredicate>
	InputIter find_if(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))return first;
		}
		return last;
	}

	template<class InputIter, class UnaryPredicate>
	InputIter find_if_not(InputIter first, InputIter last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
		{
			if (!pred(*first))return first;
		}
		return last;
	}

	template<class ForwardIter1, class ForwardIter2>
	ForwardIter1 find_end(ForwardIter1 first1, ForwardIter1 last1,
		                    ForwardIter2 first2, ForwardIter2 last2)
	{
		if (first2 == last2)return last1;
		ForwardIter1 ret = last1;
		while (first1 != last1)
		{
			ForwardIter1 it1 = first1;
			ForwardIter2 it2 = first2;
			while (*it1 == *it2)
			{
				++it1;
				++it2;
				if (it2 == last2) { ret = first1; break; }
				if (it1 == last1)return ret;
			}
		}
		return ret;
	}

	template<class ForwardIter1, class ForwardIter2, class BinaryPredicate>
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
	template<class BidirectIter1,class BidirectIter2>
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

	template<class InputIter1, class ForwardIter2>
	InputIter1 find_first_of(InputIter1 first1, InputIter1 last1,
		                        ForwardIter2 first2, ForwardIter2 last2)
	{
		if (first2 == last2)return last1;
		while (first1 != last1)
		{
			for (ForwardIter2 it2 = first2; it2 != last2; ++it2)
			{
				if (*it2 == *first1)
					return first1;
			}
			++first1;
		}
		return last1;
	}

	template<class InputIter1, class ForwardIter2, class BinaryPredicate>
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

	template<class ForwardIter>
	ForwardIter adjacent_find(ForwardIter first, ForwardIter last)
	{
		ForwardIter it = first++;
		while (first != last)
		{
			if (*first == *it)return it;
			it = first++;
		}
		return last;
	}

	template<class ForwardIter, class BinaryPredicate>
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

	template<class InputIter, class T>
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

	template<class InputIter, class UnaryPredicate>
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

	template<class InputIter1, class InputIter2>
	pair<InputIter1, InputIter2>
	mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		while (first1 != last1 && (*first1 == *first2))
		{
			++first1;
			++first2;
		}
		return make_pair(first1, first2);
	}

	template<class InputIter1, class InputIter2, class BinaryPredicate>
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

	template<class InputIter1, class InputIter2>
	bool equal(InputIter1 first1, InputIter1 last1,
		        InputIter2 first2)
	{
		while (first1 != last1)
		{
			if (*first1 != *first2)return false;
			++first1;
			++first2;
		}
		return true;
	}

	template<class InputIter1, class InputIter2, class BinaryPredicate>
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

	template<class InputIter1, class InputIter2>
	bool is_permutation(InputIter1 first1, InputIter1 last1,
		                InputIter2 first2)
	{
		tie(first1, first2) = mismatch(first1, last1, first2);
		if (first1 == last1)return true;
		InputIter2 last2 = first2;
		advance(last2, distance(first1, last1));
		for (InputIter1 it1 = first1, it1 != last1; ++it1)
		{
			if (find(first1, last1, *it1) == it1)
			{
				auto n = count(first2, last2, *it1);
				if (n == 0 || count(first1, last1, *it1) != n)return false;
			}
		}
		return true;
	}

	template<class InputIter1, class InputIter2, class BinaryPredicate>
	bool is_permutation(InputIter1 first1, InputIter1 last1,
		                InputIter2 first2, BinaryPredicate pred)
	{
		tie(first1, first2) = mismatch(first1, last1, first2, pred);
		if (first1 == last1)return true;
		InputIter2 last2 = first2;
		advance(last2, distance(first1, last1));
		for (InputIter1 it1 = first1, it1 != last1; ++it1)
		{
			if (find_if(first1, last1, [&](InputIter1 _i1) {return pred(*_i1, *it1); }) == it1)
			{
				auto n = count_if(first2, last2, [&](InputIter2 _i2) {return pred(*_i2, *it1); });
				if (n == 0 || count_if(first1, last1, [&](InputIter1 _i1) {return pred(*i1, *it1); }) != n)
					return false;
			}
		}
		return true;
	}

	template<class ForwardIter1, class ForwardIter2>
	ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
		                ForwardIter2 first2, ForwardIter2 last2)
	{
		if (first2 == last2)return last1;
		while (first1 != last1)
		{
			ForwardIter1 it1 = first1;
			ForwardIter2 it2 = first2;
			while (*it1 == *it2)
			{
				++it1;
				++it2;
				if (it2 == last2)return first1;
				if (it1 == last1)return last1;
			}
		}
		return last1;
	}

	template<class ForwardIter1, class ForwardIter2, class BinaryPredicate>
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

	template<class ForwardIter, class Size, class T>
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

	template<class ForwardIter, class Size, class UnaryPredicate>
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

	template<class InputIter, class OutputIter>
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

	template<class InputIter, class Size, class OutputIter>
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

	template<class InputIter, class OutputIter, class UnaryPredicate>
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

	template<class BidirectIter1, class BidirectIter2>
	BidirectIter2 copy_backward(BidirectIter1 first, BidirectIter1 last,
		                        BidirectIter2 result)
	{
		while (first != last)
			*(--result) = *(--last);
		return result;
	}

	template<class InputIter, class Size, class OutputIter>
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

	template<class BidirectIter1, class BidirectIter2>
	BidirectIter2 move_backward(BidirectIter1 first, BidirectIter1 last,
		                            BidirectIter2 result)
	{
		while (first != last)
			*(--result) = move(*(--last));
		return result;
	}

	template<class InputIter1, class InputIter2>
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

	template<class InputIter1, class InputIter2>
	void iter_swap(InputIter1 x, InputIter2 y)
	{
		swap(*x, *y);
	}

	template<class InputIter, class OutputIter, class UnaryOperator>
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

	template<class ForwardIter, class T>
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

	template<class ForwardIter, class T, class UnaryPredicate>
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

	template<class InputIter, class OutputIter, class T>
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

	template<class InputIter, class OutputIter, class T, class UnaryPredicate>
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

	template<class ForwardIter, class T>
	void fill(ForwardIter first, ForwardIter last, const T& val)
	{
		while (first != last)
		{
			*first = val;
			++first;
		}
	}

	template<class ForwardIter, class Size, class T>
	ForwardIter fill_n(ForwardIter first, ForwardIter last, Size n, const T& val)
	{
		while (n > 0)
		{
			*first = val;
			++first; --n;
		}
		return first;
	}

	template<class ForwardIter, class Generator>
	void generate(ForwardIter first, ForwardIter last, Generator gen)
	{
		while (first != last)
		{
			*first = gen();
			++first;
		}
	}

	template<class ForwardIter, class Size, class Generator>
	ForwardIter generate_n(ForwardIter first, Size n, Generator gen)
	{
		while (n > 0)
		{
			*first = gen();
			++first; --n;
		}
		return first;
	}

	template<class ForwardIter, class T>
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

	template<class ForwardIter, class UnaryPredicate>
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

	template<class InputIter, class OutputIter, class T>
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

	template<class InputIter, class OutputIter, class T, class UnaryPredicate>
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

	template<class ForwardIter>
	ForwardIter unique(ForwardIter first, ForwardIter last)
	{
		if (first == last)return last;
		ForwardIter result = first;
		while (++first != last)
		{
			if (!(*result == *first))
				*(++result) = *first;
		}
		return ++result;
	}

	template<class ForwardIter, class BinaryPredicate>
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

	template<class InputIter, class OutputIter>
	OutputIter unique_copy(InputIter first, InputIter last, OutputIter result)
	{
		if (first == last)return result;
		*result = *first;
		while (++first != last)
		{
			if (!(*result == *first))
				*(++result) = *first;
		}
		return ++result;
	}

	template<class InputIter, class OutputIter, class BinaryPredicate>
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

	template<class BidirectIter>
	void reverse(BidirectIter first, BidirectIter last)
	{
		while ((first != last) && (first != --last))
		{
			iter_swap(first, last);
			++first;
		}
	}

	template<class BidirectIter, class OutputIter>
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

	template<class ForwardIter>
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

	template<class ForwardIter, class OutputIter>
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
	template<class RandomIter>
	void random_shuffle(RandomIter first, RandomIter last)
	{
		for (auto i = (last - first) - 1; i > 0; --i)
			swap(first[i], first[rand() % (i + 1)]);
	}

	template<class RandomIter, class URNG>
	void shuffle(RandomIter first, RandomIter last, URNG&& gen)
	{
		for (auto i = (last - first) - 1; i > 0; --i)
		{
			std::uniform_int_distribution<decltype(i)> d(0, i);
			swap(first[i], first[d(gen)]);
		}
	}

	template<class InputIter, class UnaryPredicate>
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

	template<class ForwardIter, class UnaryPredicate>
	ForwardIter partition(ForwardIter first, ForwardIter last,
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

	template<class BidirectIter, class UnaryPredicate>
	BidirectIter partition(BidirectIter first, BidirectIter last,
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

	template<class ForwardIter, class UnaryPredicate, class Distance>
	ForwardIter inplace_stable_partition(ForwardIter first, ForwardIter last,
		                                    UnaryPredicate pred, Distance len)
	{
		if (len == 1)return pred(*first) ? last : first;
		ForwardIter middle = first;
		advance(middle, len / 2);
		return rotate(inplace_stable_partition(first, middle, pred, len / 2), middle,
			inplace_stable_partition(middle, last, pred, len - len / 2));
	}


	template<class ForwardIter, class Pointer, class UnaryPredicate>
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
		return result1;
	}

	/*
		-get_temporary_buffer() have been deprecated in C++17.
		-though using get_temporary_buffer() creates a chance to makes the funtion go faster,
		 this implementation only calls adaptive_stable_partition() when there's enough memory.
	*/
	template<class ForwardIter, class UnanryPredicate>
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
}

#endif /* TINYSTL_ALGORITHM_H_ */