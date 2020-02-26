#pragma once
#ifndef _TINYSTL_UTILITY_H_
#define _TINYSTL_UTILITY_H_

#include "type_traits.h"
#include "xutility.h"

namespace TinySTL
{
	template<class T>
	remove_reference_t<T>&&
	move(T&& x)
	{
		return static_cast<remove_reference_t<T>&&>(x);
	}

	template<class T>
	void swap(T& x, T& y)
	{
		T z(move(x));
		x = std::move(y);
		y = std::move(z);
	}

	template<class T, size_t N>
	void swap(T (&x)[N], T (&y)[N])
	{
		for (size_t i = 0; i < N; ++i)
			swap(x[i], y[i]);
	}
	
	template <class T>
	T&& forward(typename remove_reference<T>::type& t)
	{
		return static_cast<T&&>(t);
	}

	template <class T>
	T&& forward(typename remove_reference<T>::type&& t)
	{
		return static_cast<T&&>(t);
	}

	template<class T1, class T2>
	class pair
	{
	public:
		using first_type  = T1;
		using second_type = T2;
	public:
		T1 first;
		T2 second;
	public:
		pair() :first(), second() {}

		template<class U, class V>
		pair(const pair<U, V>& x) :first(x.first), second(x.second) {}

		template<class U, class V>
		pair(pair<U, V>&& x) :first(forward(x.first)), second(forward(x.second)) {}

		pair(const pair& x) = default;

		pair(pair&& x) = default;

		pair(const T1& x, const T2& y) :first(x), second(y) {}

		template<class U, class V>
		pair(U&& x, V&& y) : first(forward(x)), second(forward(y)) {}

		pair& operator =(const pair& x)
		{
			first = x.first;
			second = x.second;
			return *this;
		}

		pair& operator =(pair&& x)
		{
			first = forward(x.first);
			second = forward(x.second);
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
			first = forward(x.first);
			second = forward(x.second);
			return *this;
		}

	public:
		void swap(pair& x)
		{
			swap(first, x.first);
			swap(second, x.second);
		}
		friend bool operator== (const pair& lhs, const pair& rhs);
		friend bool operator!= (const pair& lhs, const pair& rhs);
		friend bool operator<  (const pair& lhs, const pair& rhs);
		friend bool operator<= (const pair& lhs, const pair& rhs);
		friend bool operator>  (const pair& lhs, const pair& rhs);
		friend bool operator>= (const pair& lhs, const pair& rhs);
	};
	template<class T1, class T2>
	bool operator== (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}
	template<class T1, class T2>
	bool operator!= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs == rhs);
	}
	template<class T1, class T2>
	bool operator<  (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
	}
	template<class T1, class T2>
	bool operator<= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(rhs < lhs);
	}
	template<class T1, class T2>
	bool operator>  (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return rhs < lhs;
	}
	template<class T1, class T2>
	bool operator>= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs < rhs);
	}

	template<class T1, class T2>
	void swap(pair<T1, T2>& x, pair<T1, T2>& y)
	{
		x.swap(y);
	}

	template<class T1, class T2>
	pair<T1, T2> make_pair(T1&& x, T2&& y)
	{
		return (pair<T1, T2>(forward<T1>(x), forward<T2>(y)));
	}
}

#endif /* _TINYSTL_UTILITY_H_ */