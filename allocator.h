/* pending modification */
#pragma once
#ifndef _TINYSTL_ALLOCATE_H_
#define _TINYSTL_ALLOCATE_H_

#include "alloc.h"

namespace TinySTL
{

	template <typename T>
	class allocator
	{
	public:
		using value_type      = T;
		using pointer         = T*;
		using reference       = T&;
		using size_type       = size_t;
		using const_pointer   = const T*;
		using const_reference = const T&;
		using difference_type = ptrdiff_t;

		/*
			*NOT NECESSARY
			*to judge whether Alloc is a default allocator
		*/
		using _From_primary   = allocator;

		template <typename U>
		struct rebind
		{
			using other = allocator<U>;
		};
		static pointer allocate()
		{
			return static_cast<pointer>(default_alloc_template::allocate(sizeof(T)));
		}
		static pointer allocate(size_type n)
		{
			return n != 0 ? static_cast<pointer>(default_alloc_template::allocate(sizeof(T) * n)) : 0;
		}
		static void deallocate(pointer ptr)
		{
			default_alloc_template::deallocate(ptr);
		}
		static void deallocate(pointer ptr, size_type n)
		{
			if (n != 0)default_alloc_template::deallocate(ptr, n);
		}
		pointer address(reference x)const
		{
			return (pointer)&x;
		}
		const_pointer address(const_reference x)const
		{
			return (const_pointer)&x;
		}
		size_type max_size()const
		{
			return size_type(UINT_MAX/sizeof(T));
		}
		static void destroy(pointer p)
		{
			p->~T();
		}
		static void destroy(pointer first, pointer last)
		{
			for (; first != last; ++first)
				first->~T();
		}

	};

	template <class T>
	bool operator ==(const allocator<T>& x, const allocator<T>& y)
	{
		return true;
	}

	template <class T>
	bool operator !=(const allocator<T>& x, const allocator<T>& y)
	{
		return false;
	}

	template <typename T, class Alloc>
	class simple_alloc
	{
	public:
		static T* allocate()
		{
			return static_cast<T*>(Alloc::allocate(sizeof(T)));
		}
		static T* allocate(size_t n)
		{
			return n != 0 ? static_cast<T*>(Alloc::allocate(sizeof(T) * n)) : 0;
		}
		static void deallocate(T* ptr)
		{
			Alloc::deallocate(ptr);
		}
		static void deallocate(T* ptr, size_t n)
		{
			if (n != 0)Alloc::deallocate(ptr, n);
		}
	};


}

#endif /* _TINYSTL_ALLOCATE_H_ */