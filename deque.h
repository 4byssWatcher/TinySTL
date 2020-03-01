#pragma once
#ifndef _TINYSTL_DEQUE_H_
#define _TINYSTL_DEQUE_H_

#include "iterator.h"
#include "memory.h"
#include "polymorphic_allocator.h"
#include "type_traits.h"

#include <initializer_list> // std::initializer_list

namespace TinySTL
{
	enum { DEQUE_INITIAL_MAP_SIZE = 8 };

	inline size_t _deque_buf_size(size_t size)
	{
		return size < 512 ? size_t(512 / size) : size_t(1);
	}

	template <class T, class Alloc = polymorphic_allocator<T> >
	class deque;

	template <class T, class Ref, class Ptr>
	class deque_iterator
	{
	protected:
		template <class T, class Alloc>
		friend class deque;
	public:
		using iterator			= deque_iterator<T, T&, T*>;
		using const_iterator	= deque_iterator<T, const T&, const T*>;
		using iterator_category = random_access_iterator_tag;
		using value_type        = T;
		using difference_type   = ptrdiff_t;
		using pointer			= Ptr;
		using reference			= Ref;
		using map_pointer		= T**;
		using self              = deque_iterator;

	public:
		pointer     cur;
		pointer     first; // first_iter of the current block
		pointer     last;
		map_pointer node;

		static size_t buffer_size() { return _deque_buf_size(sizeof(T)); }

	public:
		deque_iterator()
			:cur(0), first(0), last(0), node(0) {}
		deque_iterator(pointer x, map_pointer y)
			:cur(x), first(*y), last(*y + buffer_size()), node(y) {}
		deque_iterator(const iterator& x)
			:cur(x.cur), first(x.first), last(x.last), node(x.node) {}

		reference operator*()  const
			{ return *cur; }
		pointer   operator->() const
			{ return  cur; }

		difference_type operator-(const self& x) const
		{
			return difference_type(buffer_size() * (node - x.node - 1) +
								   (cur - first) + (x.last - x.cur));
		}

		self& operator++()
		{
			++cur;
			if (cur == last)
			{
				_set_node(node + 1);
				cur = first;
			}
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return tmp;
		}

		self& operator--()
		{
			if (cur == first)
			{
				_set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			--(*this);
			return tmp;
		}

		self& operator+=(difference_type n)
		{
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				cur += n;
			else
			{
				difference_type node_offset = //floor
					offset > 0 ? offset / difference_type(buffer_size())
						:difference_type((-offset - 1) / buffer_size() - 1);
				_set_node(node + node_offset);
				cur = first + (offset - node_offset * difference_type(buffer_size()));
			}
			return *this;
		}

		self operator+(difference_type n) const
		{
			self tmp = *this;
			return tmp += n;
		}

		self* operator-=(difference_type n)
		{
			return *this += -n;
		}

		self operator-(difference_type n) const
		{
			self tmp = *this;
			return tmp -= n;
		}

		reference operator[](difference_type n) const
			{ return *(*this + n); }
		
		bool operator==(const self& x) const
			{ return cur == x.cur; }
		bool operator!=(const self& x) const
			{ return !(*this == x); }
		bool operator <(const self& x) const
		{
			return (node == x.node) ?
				(cur < x.cur) : (node < x.node);
		}
		bool operator >(const self& x) const
			{ return x < *this; }
		bool operator<=(const self& x) const
			{ return !(x < *this); }
		bool operator>=(const self& x) const
			{ return !(*this < x); }

	protected:
		void _set_node(map_pointer new_node)
		{
			node  = new_node;
			first = *new_node;
			last  = first + difference_type(buffer_size());
		}
	};



	template <class T, class Alloc = polymorphic_allocator<T> >
	class deque
	{
	public:
		using value_type		 = T;
		using iterator			 = deque_iterator<T,T&,T*>;
		using const_iterator	 = deque_iterator<T, const T&, const T*>;
		using pointer			 = T*;
		using const_pointer		 = const T*;
		using reference			 = T&;
		using const_reference	 = const T&;
		using size_type			 = size_t;
		using difference_type	 = ptrdiff_t;
		using map_pointer	     = T**;
		using allocator_type     = Alloc;
		using map_allocator_type =
			typename allocator_traits<Alloc>::template rebind_alloc<pointer>;

	protected:
		iterator		   start;
		iterator		   finish;
		map_pointer		   map;
		size_type		   map_size;
		allocator_type	   data_allocator;
		map_allocator_type map_allocator;

	public:
		explicit deque(const Alloc& alloc = Alloc())
			:start(0), finish(0), map(0), data_allocator(alloc), map_allocator(alloc) {}

		explicit deque(size_type n, const T& val = T(), const Alloc& alloc = Alloc())
			:data_allocator(alloc), map_allocator(alloc)
		{
			_initialize_map(n);
			uninitialized_fill_n(start, n, val, alloc);
		}

		template <class InputIter, class = enable_if_t<_is_iterator_v<InputIter> > >
		deque(InputIter first, InputIter last, const Alloc& alloc = Alloc())
			:data_allocator(alloc), map_allocator(alloc)
		{
			_range_init(first, last);
		}

		deque(const deque& other)
			:data_allocator(other.get_allocator()),
			 map_allocator(other.get_allocator())
		{
			_range_init(other.start, other.finish);
		}

		deque(const deque& other, const Alloc& alloc)
			:data_allocator(alloc), map_allocator(alloc)
		{
			_range_init(other.start, other.finish);
		}

		deque(deque&& other)
			:start(other.start), finish(other.finish),
			 map(other.map), map_size(other.map_size),
			 data_allocator(other.get_allocator()),
			 map_allocator(other.get_allocator()) {}

		deque(deque&& other, const Alloc& alloc)
			:start(other.start), finish(other.finish),
			 map(other.map), map_size(other.map_size),
			 data_allocator(alloc),
			 map_allocator(alloc) {}

		deque(std::initializer_list<T> ilist, const Alloc& alloc = Alloc())
			:data_allocator(alloc), map_allocator(alloc)
		{
			_range_init(ilist.begin(), ilist.end());
		}

		iterator begin() { return start; }
		iterator end() { return finish; }
		reference front() { return *start; }
		reference back()
		{
			iterator tmp = finish;
			--tmp;
			return *tmp;
		}

		allocator_type get_allocator()
			{ return data_allocator; }

	protected:
		void _initialize_map(size_type n)
		{
			size_type num_nodes =
				n / _deque_buf_size(sizeof(T)) + 1;

			map_size = max(DEQUE_INITIAL_MAP_SIZE, num_nodes + 2);
			map = allocator_traits<map_allocator>::allocate(map_allocator, map_size);

			map_pointer tmp_s = map + (map_size - num_nodes) / 2;
			map_pointer tmp_t = tmp_s + num_nodes;

			start._set_node(tmp_s);
			finish._set_node(tmp_t);
			start.cur  = start.first;
			finish.cur = finish.first + n % _deque_buf_size(sizeof(T));
		}

		template <class Iterator>
		void _range_init(Iterator first, Iterator last)
		{
			_range_initialize(first, last, iterator_category(first));
		}

		template <class InputIter>
		void _range_initialize(InputIter first, InputIter last,
							   input_iterator_tag)
		{
			_initialize_map(0);
			for (; first != last; ++first)
				push_back(*first);
		}

		template <class ForwardIter>
		void _range_initialize(ForwardIter first, ForwardIter last,
							   forward_iterator_tag)
		{
			size_type n = distance(first, last);
			_initialize_map(n);
			uninitialized_copy(first, last, start, data_allocator);
		}

	};
}

#endif /* _TINYSTL_DEQUE_H_ */
