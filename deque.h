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
		using value_type			 = T;
		using iterator				 = deque_iterator<T,T&,T*>;
		using const_iterator		 = deque_iterator<T, const T&, const T*>;
		using pointer				 = T*;
		using const_pointer			 = const T*;
		using reference				 = T&;
		using const_reference		 = const T&;
		using size_type				 = size_t;
		using difference_type		 = ptrdiff_t;
		using map_pointer			 = T**;
		using const_reverse_iterator = reverse_iterator<const_iterator>;
		using reverse_iterator		 = reverse_iterator<iterator>;
		using allocator_type		 = Alloc;
		using map_allocator_type	 =
			typename allocator_traits<Alloc>::template rebind_alloc<pointer>;

	protected:
		using alloc_traits	   = allocator_traits<allocator_type>;
		using map_alloc_traits = allocator_traits<map_allocator_type>;

	protected:
		iterator		   start;
		iterator		   finish;
		map_pointer		   map;
		size_type		   map_size;
		allocator_type	   data_allocator;
		map_allocator_type map_allocator;

		static size_t buffer_size() { return _deque_buf_size(sizeof(T)); }

	public:
		explicit deque(const Alloc& alloc = Alloc())
			:start(0), finish(0), map(0), data_allocator(alloc), map_allocator(alloc) {}

		explicit deque(size_type n, const T& val = T(), const Alloc& alloc = Alloc())
			:data_allocator(alloc), map_allocator(alloc)
		{
			_initialize_map(n);
			uninitialized_fill_n(start, n, val, data_allocator);
		}

		template <class InputIter,
				  class = enable_if_t<_is_iterator_v<InputIter> > >
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
			 map_allocator(other.get_allocator())
		{
			other._pointer_clear();
		}

		deque(deque&& other, const Alloc& alloc)
			:start(other.start), finish(other.finish),
			 map(other.map), map_size(other.map_size),
			 data_allocator(alloc),
			 map_allocator(alloc)
		{
			other._pointer_clear();
		}

		deque(std::initializer_list<T> ilist, const Alloc& alloc = Alloc())
			:data_allocator(alloc), map_allocator(alloc)
		{
			_range_init(ilist.begin(), ilist.end());
		}

		deque& operator=(const deque& other)
		{
			if (this != addressof(other))
				assign(other.start, other.finish);
			return *this;
		}

		deque& operator=(deque&& other)
		{
			if (this != addressof(other))
			{
				start    = other.start;
				finish   = other.finish;
				map		 = other.map;
				map_size = other.map_size;
			}
			other._pointer_clear();
			return *this;
		}

		deque& operator=(std::initializer_list<T> ilist)
		{
			assign(ilist.begin(), ilist.end());
			return *this;
		}

		void assign(size_type n, value_type val);

		template <class Iterator,
				  class = enable_if_t<_is_iterator_v<Iterator> > >
		void assign(Iterator first, Iterator last)
		{
			_assign(first, last, iterator_category(first));
		}

		void assign(std::initializer_list<T> ilist)
		{
			assign(ilist.begin(), ilist.end());
		}

		allocator_type get_allocator()
			{ return data_allocator; }

		reference at(size_type n)
		{
			if (n < size()) return (*this)[n];
			return *start;
		}

		const_reference at(size_type n) const
		{
			if (n < size()) return (*this)[n];
			return *start;
		}

		reference operator[](size_type n) { return *(begin() + n); }
		const_reference operator[](size_type n) const { return *(begin() + n); }

		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }

		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }

		iterator begin() noexcept { return start; }
		const_iterator begin() const noexcept { return start; }
		const_iterator cbegin() const noexcept { return start; }

		iterator end() noexcept { return finish; }
		const_iterator end() const noexcept { return finish; }
		const_iterator cend() const noexcept { return finish; }

		reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

		reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

		bool empty() const { return start == finish; }
		size_type size() const { return finish - start; }
		size_type max_size() const { return size_type(-1); }

		void shrink_to_fit()
		{
			size_type new_size = size() / buffer_size() + 1;
			new_size = max(DEQUE_INITIAL_MAP_SIZE, new_size + 2);

			if (new_size < map_size)
			{
				deque tmp(begin(), end());
				this.swap(tmp);
			}
		}

		void clear() noexcept { erase(begin(), end()); }

		iterator insert(iterator pos, const T& val)
			{ return emplace(pos, val); }

		iterator insert(iterator pos, T&& val)
			{ return emplace(pos, move(val)); }

		iterator insert(iterator pos, size_type n, const T& val)
		{
			if (n != 0)
			{
				auto now = _insert_spare_n(pos, n);
				iterator tmp_s = std::get<1>(now);
				iterator mid   = std::get<2>(now);
				iterator tmp_t = tmp_s + n;

				if (std::get<0>(now))//unini, ini
				{
					uninitialized_fill(tmp_s, mid, val, data_allocator);
					fill(mid, tmp_t, val);
				}
				else
				{
					fill(tmp_s, mid, val);
					uninitialized_fill(mid, tmp_t, val, data_allocator);
				}
				return tmp_s;
			}
			return pos;
		}

		template <class Iterator, class = enable_if_t<_is_iterator_v<Iterator> > >
		iterator insert(iterator pos, Iterator first, Iterator last)
		{
			return _insert(pos, first, last, iterator_category(first));
		}

		iterator insert(iterator pos, std::initializer_list<T> ilist)
		{
			return _insert(pos, ilist.begin(), ilist.end(), iterator_category(ilist.begin()));
		}

		template <class... Args>
		iterator emplace(iterator pos, Args&&... args)
		{
			auto now = _insert_spare_n(pos, 1);
			bool t1 = std::get<0>(now);
			bool t2 = (std::get<1>(now) == std::get<2>(now));

			iterator aim = std::get<1>(now);

			if (t1 ^ t2) //unini
				alloc_traits::construct(data_allocator, aim, forward<Args>(args)...);
			else //ini
				*aim = value_type(forward<Args>(args)...);
			return aim;
		}

		iterator erase(iterator pos)
		{
			if (pos + 1 != end())
				move(pos + 1, finish, pos);
			--finish;
			alloc_traits::destroy(finish);
			return pos;
		}

		iterator erase(iterator first, iterator last)
		{
			iterator tmp = move(last, finish, first);
			alloc_traits::destroy(data_allocator, tmp, finish);
			finish = finish - (last - first);
			return first;
		}

		void push_back(const T& val)
		{
			if (finish.cur != finish.last - 1)
			{
				alloc_traits::construct(data_allocator, finish.cur, val);
				++finish.cur;
			}
			else emplace(end(), val);
		}

		void push_back(T&& val)
		{
			if (finish.cur != finish.last - 1)
			{
				alloc_traits::construct(data_allocator, finish.cur, move(val));
				++finish.cur;
			}
			else emplace(end(), move(val));
		}

		template <class... Args>
		reference emplace_back(Args&&... args)
		{
			iterator now = emplace(end(), forward<Args>(args)...);
			return *now;
		}

		void pop_back()
		{
			--finish;
			alloc_traits::destroy(data_allocator, finish);
		}

		void resize(size_type new_size, T val = T())
		{
			if (new_size < size())
				erase(begin() + new_size, end());
			else insert(end(), new_size - size(), val);
		}

		void swap(deque& other)
		{
			swap(start, other.start);
			swap(finish, other.finish);
			swap(map, other.map);
			swap(map_size, other.map_size);
		}

	protected:
		template <class InputIter>
		void _assign(InputIter first, InputIter last, input_iterator_tag);

		template <class ForwardIter>
		void _assign(ForwardIter first, ForwardIter last, forward_iterator_tag);

		void _initialize_map(size_type n);

		template <class InputIter>
		iterator _insert(iterator pos, InputIter first,
						 InputIter last, input_iterator_tag)
		{
			for (; first != last; ++first)
			{
				pos = emplace(pos, *first);
				++pos;
			}
			return pos;
		}

		template <class ForwardIter>
		iterator _insert(iterator pos, ForwardIter first,
						 ForwardIter last, forward_iterator_tag)
		{
			if (first != last)
			{
				size_type n = distance(first, last);
				iterator now = _insert_spare_n(pos, n);

				iterator mid = first;
				iterator ret = std::get<1>(now), aim = ret;
				advance(mid, std::get<2>(now) - std::get<1>(now));

				if (std::get<0>(now))//unini, ini
				{
					aim = uninitialized_copy(first, mid, aim, data_allocator);
					copy(mid, last, aim);
				}
				else
				{
					aim = copy(first, mid, aim, data_allocator);
					uninitialized_copy(mid, last, aim, data_allocator);
				}
				return ret;
			}
			return pos;
		}

		std::tuple<bool, typename deque<T, Alloc>::iterator,
						 typename deque<T, Alloc>::iterator>
		_insert_spare_n(iterator pos, size_type n);

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

		void _pointer_clear()
		{
			start    = pointer();
			finish   = pointer();
			map      = map_pointer();
			map_size = 0;
		}

	};

	template <class T, class Alloc>
	void
	deque<T, Alloc>::assign(size_type n, value_type val)
	{
		if (n > size())
		{
			fill(begin(), end(), val);
			insert(end(), n - size(), val);
		}
		else
			erase(fill(begin(), end(), val), end());
	}

	template <class T, class Alloc>
	template <class InputIter>
	void
	deque<T, Alloc>::_assign(InputIter first, InputIter last,
							 input_iterator_tag)
	{
		iterator cur = begin();
		for (; first != last && cur != end(); ++cur, ++first)
			*cur = *first;
		if (first == last)
			erase(cur, end());
		else
			insert(end(), first, last);
	}

	template <class T, class Alloc>
	template <class ForwardIter>
	void
	deque<T, Alloc>::_assign(ForwardIter first, ForwardIter last,
							 forward_iterator_tag)
	{
		size_type n = distance(first, last);
		if (n > size())
		{
			ForwardIter mid = first;
			advance(mid, size());
			copy(first, mid, begin());
			insert(end(), mid, last);
		}
		else erase(copy(first, last, begin(), end()));
	}

	template <class T, class Alloc>
	void
	deque<T, Alloc>::_initialize_map(size_type n)
	{
		size_type num_nodes = n / buffer_size() + 1;

		map_size = max(DEQUE_INITIAL_MAP_SIZE, num_nodes + 2);
		map = alloc_traits::allocate(map_allocator, map_size);

		map_pointer tmp_s = map + (map_size - num_nodes) / 2;
		map_pointer tmp_t = tmp_s + num_nodes;

		for (; tmp_s != tmp_t; ++tmp_s)
			*tmp_s = map_alloc_traits::allocate(data_allocator,
												buffer_size());

		start.cur  = start.first;
		finish.cur = finish.first + n % buffer_size();
	}

	/*
		tuple(bool, iter, iter)
		true  : uninitialized block / initialized block
		false : initialized block / uninitialized block
	*/
	template <class T, class Alloc>
	std::tuple<bool, typename deque<T, Alloc>::iterator,
					 typename deque<T, Alloc>::iterator>
	deque<T, Alloc>::_insert_spare_n(iterator pos, size_type n)
	{
		const difference_type elems_before = pos - start;
		size_type len = size();
		if (elems_before < difference_type(len / 2))
		{
			iterator new_s = reserve_elements_at_front(n);
			iterator old_s = start;
			if (elems_before > difference_type(n))
			{
				iterator start_n = start + difference_type(n);
				uninitialized_move(start, start_n, new_s, data_allocator);
				copy(start_n, pos, start);
				start = new_s;
				return make_tuple(true, pos - difference_type(n),
										pos - difference_type(n));
			}
			else
			{
				uninitialized_move(start, pos, new_s, data_allocator);
				start = new_s;
				return make_tuple(true, pos - difference_type(n), old_s);
			}
		}
		else
		{
			iterator new_t = reserve_elements_at_back(n);
			iterator old_t = finish;
			const difference_type elems_after = difference_type(len);
			if (elems_after > difference_type(n))
			{
				iterator finish_n = finish - difference_type(n);
				uninitialized_move(finish_n, finish, finish, data_allocator);
				finish = new_t;
				copy_backward(pos, finish_n, old_t);
				return make_tuple(true, pos, pos);
			}
			else
			{
				uninitialized_move(pos, finish, pos + difference_type(n), data_allocator);
				finish = new_t;
				return make_pair(false, pos, old_t);
			}
		}
	}
}

#endif /* _TINYSTL_DEQUE_H_ */
