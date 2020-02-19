#pragma once
#ifndef _TINYSTL_VECTOR_H_
#define _TINYSTL_VECTOR_H_

#include <stdexcept>//range_error
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

	public:
		vector() noexcept(noexcept(Alloc()))
			:start(0), finish(0), end_of_storage(0) {}

		explicit vector(const allocator_type& alloc) noexcept
			:start(0), finish(0), end_of_storage(0) {}

		vector(size_type n, const value_type& val,
			   const allocator_type& alloc = allocator_type())
		{
			_alloc_and_init_n(n, val);
		}

		explicit vector(size_type n, const allocator_type& alloc = allocator_type())
		{
			_alloc_and_init_n(n, T());
		}

		/* 
			may get confused with the above case, e.g. vector(int, int, alloc)
			see _is_iterator_v in "type_traits.h"
			(change it to concept if possible)
		*/
		template <class InputIter, class = enable_if_t<_is_iterator_v<InputIter> > >
		vector(InputIter first, InputIter last,
			   const allocator_type& alloc = allocator_type())
		{
			_range_init(first, last);
		}

		vector(const vector& other)
		{
			_alloc_and_copy_n(other.size(), other.start, other.finish);
		}

		vector(const vector& other, const Alloc& alloc)
		{
			_alloc_and_copy_n(other.size(), other.start, other.finish);
		}

		vector(vector&& other) noexcept
			:start(other.start), finish(other.finish),
			 end_of_storage(other.end_of_storage) {}

		vector(vector&& other, const Alloc& alloc)
			:start(other.start), finish(other.finish),
			 end_of_storage(other.end_of_storage) {}

		vector(std::initializer_list<T> ilist, const Alloc& alloc = Alloc())
		{
			_range_init(ilist.begin(), ilist.end());
		}

		/* 
			by using allocator_traits<Alloc>::func instead of alloc.func,
				custom allocator is better supported(less code for SOME functions is needed)
		    see allocator_traits in "xmemory.h" for further detail
		*/
		~vector()
		{
			_destroy_and_dealloc();
		}

		vector& operator=(const vector& other)
		{
			if (this != addressof(other))
			{
				_destroy_and_dealloc();
				_range_init(other.start, other.finish);
			}
			return *this;
		}

		vector& operator=(vector&& other)noexcept;

		vector& operator=(std::initializer_list<T> ilist)
		{
			_destroy_and_dealloc();
			_range_init(ilist.begin(), ilist.end());
			return *this;
		}

		void assign(size_type n, const T& val);

		template <class InputIter, class = enable_if_t<_is_iterator_v<InputIter> > >
		void assign(InputIter first, InputIter last)
		{
			_assign(first, last, iterator_category(first));
		}

		void assign(std::initializer_list<T> ilist)
		{
			assign(ilist.begin(), ilist.end());
		}

		allocator_type get_allocator() const { return allocator_type(); }

		reference at(size_type n)
		{
			_range_check(n);
			return (*this)[n];
		}

		const_reference at(size_type n) const
		{
			_range_check(n);
			return (*this)[n];
		}

		reference operator[](size_type n) { return *(begin() + n); }
		const_reference operator[](size_type n) const { return *(begin() + n); }

		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }

		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }

		constexpr pointer data() noexcept { return start; }
		constexpr const_pointer data() const noexcept { return start; }

		iterator begin() noexcept { return start; }
		const_iterator begin() const noexcept { return start; }
		const_iterator cbegin() const noexcept { return start; }

		iterator end() noexcept { return finish; }
		const_iterator end() const noexcept { return finish; }
		const_iterator cend() const noexcept { return finish; }
		
		reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() noexcept { return const_reverse_iterator(end()); }
		const_reverse_iterator crbegin() noexcept { return const_reverse_iterator(end()); }

		reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator rend() noexcept { return const_reverse_iterator(begin()); }
		const_reverse_iterator crend() noexcept { return const_reverse_iterator(begin()); }

		_NODISCARD constexpr bool empty() const noexcept { return begin() == end(); }
		size_type size() const noexcept { return size_type(end() - begin()); }
		size_type max_size() const { return size_type(-1) / sizeof(T); }
		size_type capacity() const noexcept { return size_type(end_of_storage - begin()); }

		void reserve(size_type new_cap)
		{
			if (new_cap > capacity())
			{
				const size_type old_size = size();
				iterator tmp = _alloc_and_copy_n(new_cap, start, finish);
				_destroy_and_dealloc();
				start          = tmp;
				finish         = tmp + old_size;
				end_of_storage = tmp + new_cap;
			}
		}

		void shrink_to_fit()
		{
			if (end_of_storage != end())
			{
				iterator tmp = Alloc::allocate(size());
				end_of_storage = uninitialized_copy(start, finish, tmp);
				_destroy_and_dealloc();
				start  = tmp;
				finish = end_of_storage;
			}
		}


	protected:
		void _alloc_and_init_n(size_type n, value_type val)
		{
			start = Alloc::allocate(n);
			end_of_storage = start + n;
			finish = uninitialized_fill_n(start, n, val);
		}

		void _alloc_and_copy_n(size_type n, iterator first, iterator last)
		{
			start = Alloc::allocate(n);
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
			/* can't get n through last-first, unable to allocate at once */
			for (; first != last; ++first)
				push_back(*first);
		}

		template <class ForwardIter>
		void _range_initialize(ForwardIter first, ForwardIter last, forward_iterator_tag)
		{
			size_type n = distance(first, last);
			_alloc_and_copy_n(n, first, last);
		}

		void _destroy_and_dealloc()
		{
			allocator_traits<Alloc>::destroy(start, finish);
			Alloc::deallocate(start);
		}

		template <class InputIter>
		void _assign(InputIter first, InputIter last, input_iterator_tag);

		template <class ForwardIter>
		void _assign(ForwardIter first, ForwardIter last, forward_iterator_tag);

		void _range_check(size_type n) const
		{
			if (n >= this->size())
				throw range_error("vector");
		}

	};

	template <class T, class Alloc>
	vector<T, Alloc>&
	vector<T, Alloc>::operator=(vector<T, Alloc>&& other)noexcept
	{
		if (this != addressof(other))
		{
			_destroy_and_dealloc();

			start          = other.start;
			finish         = other.finish;
			end_of_storage = other.end_of_storage;

			other.start    = pointer();
			other.finish   = pointer();
			end_of_storage = pointer();
		}
		return *this;
	}

	template <class T, class Alloc>
	void 
	vector<T, Alloc>::assign(size_type n, const T& val)
	{
		if (n > capacity())
		{
			vector<T, Alloc>tmp(n, val);
			tmp.swap(*this);
		}
		else if (n > size())
		{
			fill(begin(), end(), val);
			finish = uninitialized_fill_n(finish, n - size(), val);
		}
		else
			erase(fill_n(begin(), n, val), end());
	}

	template <class T, class Alloc>
	template <class InputIter>
	void
	vector<T, Alloc>::_assign(InputIter first, InputIter last, input_iterator_tag)
	{
		iterator current = begin();
		for (; first != last && current != end(); ++current, ++first)
			*current = *first;
		if (first == last) // this > other
			erase(current, end());
		else
			insert(end(), first, last);
	}

	template <class T, class Alloc>
	template <class ForwardIter>
	void 
	vector<T, Alloc>::_assign(ForwardIter first, ForwardIter last, forward_iterator_tag)
	{
		size_type n = distance(first, last);
		if (n > capacity())
		{
			iterator tmp = _alloc_and_copy_n(n, first, last);
			_destroy_and_dealloc();
			start = tmp;
			end_of_storage = finish = start + n;
		}
		else if (n > size())
		{
			ForwardIter mid = first;
			advance(mid, size());
			copy(first, mid, start);
			finish = uninitialized_copy(mid, last, finish);
		}
		else
		{
			iterator new_finish = copy(first, last, start);
			destroy(new_finish, finish);
			finish = new_finish;
		}
	}
}

#endif /* _TINYSTL_VECTOR_H_ */