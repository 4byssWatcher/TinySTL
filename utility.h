#pragma once
#ifndef _TINYSTL_UTILITY_H_
#define _TINYSTL_UTILITY_H_

#include "type_traits.h"

namespace TinySTL
{
	template<class T>
	void swap(T& x, T& y)
	{
		T z(move(x));
		x = move(y);
		y = move(z);
	}

	template<class T, size_t N>
	void swap(T (&x)[N], T (&y)[N])
	{
		for (size_t i = 0; i < N; ++i)
			swap(x[i], y[i]);
	}

	template<class T>
	typename remove_reference<T>::type&&
	move(T&& x)
	{
		return static_cast<remove_reference<decltype(x)>::type>(x);
	}

	template<class T1, class T2>
	pair<T1, T2> make_pair(T1 x, T2 y)
	{
		return pair<T1, T2>(forward<T1>(x), forward<T2>(y));
	}

	template <class T>
	inline T&& forward(typename remove_reference<T>::type& t)
	{
		return static_cast<T&&>(t);
	}

	template <class T>
	inline T&& forward(typename remove_reference<T>::type&& t)
	{
		return static_cast<T&&>(t);
	}

	template<class T1, class T2>
	class pair
	{
	public:
		typedef T1 first_type;
		typedef T2 second_type;
	public:
		T1 first;
		T2 second;
	public:
		pair() {}

		template<class U, class V>
		pair(const pair<U, V>& x) :first(x.first), second(x.second) {}

		pair(pair<U, V>&& x) :first(forward<T1>(x.first)), second(forward<T2>(x.second)) {}

		pair(const pair& x) :first(x.first), second(x.second) {}

		pair(pair&& x) :first(forward<T1>(x.first)), second(forward<T2>(x.second)) {}

		pair(const T1& x, const T2& y) :first(x), second(y) {}

		template<class U, class V>
		pair(U&& x, V&& y) : first(forward<T1>(x)), second(forward<T2>(y)) {}

		pair& operator =(const pair& x)
		{
			first = x.first;
			second = x.second;
			return *this;
		}

		pair& operator =(pair&& x)
		{
			first = forward<T1>(x.first);
			second = forward<T2>(x.second);
			return *this;
		}

		template<class U,class V>
		pair& operator =(const pair<U,V>& x)
		{
			first = x.first;
			second = x.second;
			return *this;
		}

		template<class U,class V>
		pair& operator =(pair<U,V>&& x)
		{
			first = forward<T1>(x.first);
			second = forward<T2>(x.second);
			return *this;
		}

	public:
		void swap(pair& x)
		{
			swap(first, x.first);
			swap(second, x.second);
		}

		bool operator== (const pair& lhs, const pair& rhs)
		{
			return lhs.first == rhs.first && lhs.second == rhs.second;
		}
		bool operator!= (const pair& lhs, const pair& rhs)
		{
			return !(lhs == rhs);
		}
		bool operator<  (const pair& lhs, const pair& rhs)
		{
			return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
		}
		bool operator<= (const pair& lhs, const pair& rhs)
		{
			return !(rhs < lhs);
		}
		bool operator>(const pair& lhs, const pair& rhs)
		{
			return rhs < lhs;
		}
		bool operator>= (const pair& lhs, const pair& rhs)
		{
			return !(lhs < rhs);
		}
		void swap(pair& x, pair& y)
		{
			x.swap(y);
		}
	};
}

#endif /* _TINYSTL_UTILITY_H_ */