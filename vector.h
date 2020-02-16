#pragma once
#ifndef _TINYSTL_VECTOR_H_
#define _TINYSTL_VECTOR_H_

#include "allocator.h"
#include "iterator.h"
#include "memory.h"
#include "type_traits.h"

namespace TinySTL
{
	template <class T, class Alloc =  allocator<T>>
	class vector
	{
	private:
		typedef allocator_traits<Alloc>                alloc_traits;
	public:
		typedef T                                      value_type;
		typedef Alloc                                  allocator_type;
		typedef size_t                                 size_type;
		typedef ptrdiff_t                              difference_type;
		typedef T&                                     reference;
		typedef const T&                               const_reference;
		typedef typename alloc_traits::pointer         pointer;
		typedef typename alloc_traits::const_pointer   const_pointer;
		typedef T*                                     iterator;
		typedef const T*                               const_iterator;
		typedef reverse_iterator<const_iterator>	   const_reverse_iterator;
		typedef reverse_iterator<iterator>		  	   reverse_iterator;
	
	protected:
		iterator        start;
		iterator	    finish;
		iterator	    end_of_storage;
		allocator_type  dataAllocator;

	public:
		vector() noexcept(noexcept(Alloc()))
			:start(0), finish(0), end_of_storage(0), dataAllocator(Alloc()) {}

		explicit vector(const allocator_type& alloc) noexcept
			:start(0), finish(0), end_of_storage(0), dataAllocator(alloc) {}

		vector(size_type n, const value_type& val,
			   const allocator_type& alloc = allocator_type())
			:dataAllocator(alloc)
		{
			_alloc_and_init_n(n, val);
		}

		explicit vector(size_type n, const allocator_type& alloc = allocator_type())
			:dataAllocator(alloc)
		{
			_alloc_and_init_n(n, T());
		}

		/* 
			may get confused with the above case, e.g. vector(int, int, alloc)
			see _is_iterator_v in "type_traits.h"
		*/
		template <class InputIter, class = enable_if_t<_is_iterator_v<InputIter> > >
		vector(InputIter first, InputIter last,
			   const allocator_type& alloc = allocator_type())
			:dataAllocator(alloc)
		{
			_range_init(first, last, iterator_category(first));
		}

		vector(const vector& other)
			:dataAllocator(other.get_allocator())
		{
			_alloc_and_copy(other.size(), other.start, other.finish);
		}

		vector(const vector& other, const Alloc& alloc)
			:dataAllocator(alloc)
		{
			_alloc_and_copy(other.size(), other.start, other.finish);
		}

		vector(vector&& other) noexcept
			:start(other.start), finish(other.finish),
			 end_of_storage(other.end_of_storage), dataAllocator(other.dataAllocator) {}

		vector(vector&& other, const Alloc& alloc)
			:start(other.start), finish(other.finish),
			 end_of_storage(other.end_of_storage), dataAllocator(alloc) {}

		vector(std::initializer_list<T> init, const Alloc& alloc = Alloc())
			:dataAllocator(alloc)
		{
			insert(start, init.begin(), init.end());
		}

		/* 
			by using allocator_traits<Alloc>::func instead of alloc.func,
				custom allocator is better supported(less code for SOME functions is needed)
		    see allocator_traits in "xmemory.h" for further detail
		*/
		~vector()
		{
			allocator_traits<Alloc>::destroy(start, finish);
			dataAllocator.deallocate(start);
		}


	private:
		void _alloc_and_init_n(size_type n, value_type val)
		{
			start = allocator_traits<Alloc>::allocate(n);
			end_of_storage = start + n;
			finish = uninitialized_fill_n(start, n, val);
		}

		void _alloc_and_copy(size_type n, iterator first, iterator last)
		{
			start = dataAllocator.allocate(n);
			end_of_storage = start + n;
			finish = uninitialized_copy(first, last, start);
		}

		template <class Iterator>
		void _range_init(Iterator first, Iterator last)
		{
			_range_initialize(first, last, iterator_category(first));
		}

		template <class InputIter>
		void _range_initialize(InputIter first, InputIter last, input_iterator_tag)
		{
			for (; first != last; ++first)
				push_back(*first);
		}

		template <class ForwardIter>
		void _range_initialize(ForwardIter first, ForwardIter last, forward_iterator_tag)
		{
			size_type n = distance(first, last);
			start = dataAllocator.allocate(n);
			end_of_storage = start + n;
			finish = uninitialized_copy(first, last, start);
		}



	};
}

#endif /* _TINYSTL_VECTOR_H_ */