/* untested */
#pragma once
#ifndef TINYSTL_ALLOCATE_H
#define TINYSTL_ALLOCATE_H

#include "alloc.h"

namespace TinySTL
{

	template <typename T>
	class allocator
	{
	public:
		typedef    T           value_type;
		typedef    T*          pointer;
		typedef    T&          reference;
		typedef    size_t      size_type;
		typedef    const T*    const_pointer;
		typedef    ptrdiff_t   difference_type;

		template <typename U>
		struct rebind
		{
			typedef allocator<U> other;
		};
		pointer allocate()
		{
			return static_cast<pointer>(default_alloc_template::allocate(sizeof(T)));
		}
		pointer allocate(size_type n)
		{
			return n != 0 ? static_cast<pointer>(default_alloc_template::allocate(sizeof(T) * n)) : 0;
		}
		void deallocate(pointer ptr)
		{
			default_alloc_template::deallocate(ptr);
		}
		void deallocate(pointer ptr, size_type n)
		{
			if (n != 0)default_alloc_template::deallocate(ptr, n);
		}
		pointer address(reference x)const
		{
			return (pointer)&x;
		}
		const_pointer address(reference x)const
		{
			return (const_pointer)&x;
		}
		size_type max_size()const
		{
			return size_t(-1) / sizeof(T);
		}
		void construct(pointer p, const T& value)
		{
			new(p) T(value);
		}
		void destroy(pointer p)
		{
			p->~T();
		}
		void destroy(pointer first, pointer last)
		{
			for (; first != last; ++first)
				first->~T();
		}

	};

	template <typename T, class Alloc>
	class simple_alloc
	{
	public:
		pointer allocate()
		{
			return static_cast<pointer>(Alloc::allocate(sizeof(T)));
		}
		pointer allocate(size_type n)
		{
			return n != 0 ? static_cast<pointer>(Alloc::allocate(sizeof(T) * n)) : 0;
		}
		void deallocate(pointer ptr)
		{
			Alloc::deallocate(ptr);
		}
		void deallocate(pointer ptr, size_type n)
		{
			if (n != 0)Alloc::deallocate(ptr, n);
		}
	};


}

#endif /* TINYSTL_ALLOCATE_H */