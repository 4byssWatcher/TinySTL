#pragma once
#ifndef _TINYSTL_VECTOR_H_
#define _TINYSTL_VECTOR_H_

#include "algorithm.h"
#include "iterator.h"
#include "memory.h"
#include "polymorphic_allocator.h"
#include "type_traits.h"
#include "utility.h"

#include <initializer_list> // std::initializer_list
#include <xtr1common>

namespace TinySTL
{
	template <class T, class Alloc =  polymorphic_allocator<T> >
	class vector
	{
	protected:
		using alloc_traits           = allocator_traits<Alloc>;
	public:
		using value_type             = T;
		using allocator_type         = Alloc;
		using size_type              = size_t;
		using difference_type        = ptrdiff_t;
		using reference              = T&;
		using const_reference        = const T&;
		using pointer                = typename alloc_traits::pointer;
		using const_pointer          = typename alloc_traits::const_pointer;
		using iterator               = T*;
		using const_iterator         = const T*;
		using const_reverse_iterator = reverse_iterator<const_iterator>;
		using reverse_iterator       = reverse_iterator<iterator>;
	
	protected:
		iterator        start;
		iterator	    finish;
		iterator	    end_of_storage;
		allocator_type  data_allocator;

	public:
		explicit vector(const allocator_type& alloc = Alloc()) noexcept
			:start(0), finish(0), end_of_storage(0), data_allocator(alloc) {}

		explicit vector(size_type n, const value_type& val = T(),
						const allocator_type& alloc = allocator_type())
			:data_allocator(alloc)
		{
			_alloc_n_and_init(n, val);
		}

		/* 
			may get confused with the above case, e.g. vector(int, int, alloc)
			see _is_iterator_v in "type_traits.h"
			(change it to concept if possible)
		*/
		template <class InputIter, class = enable_if_t<_is_iterator_v<InputIter> > >
		vector(InputIter first, InputIter last,
			   const allocator_type& alloc = allocator_type())
			:data_allocator(alloc)
		{
			_range_init(first, last);
		}

		vector(const vector& other)
			:data_allocator(other.get_allocator())
		{
			_range_init(other.begin(), other.end());
		}

		vector(const vector& other, const Alloc& alloc)
			:data_allocator(alloc)
		{
			_range_init(other.begin(), other.end());
		}

		vector(vector&& other) noexcept
			:start(other.start), finish(other.finish),
			 end_of_storage(other.end_of_storage),
			 data_allocator(other.get_allocator())
		{
			other._pointer_clear();
		}

		vector(vector&& other, const Alloc& alloc)
			:start(other.start), finish(other.finish),
			 end_of_storage(other.end_of_storage),
			 data_allocator(alloc)
		{
			other._pointer_clear();
		}

		vector(std::initializer_list<T> ilist, const Alloc& alloc = Alloc())
			:data_allocator(alloc)
		{
			_range_init(ilist.begin(), ilist.end());
		}

		/* 
			by using alloc_traits::func instead of alloc.func,
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
				assign(other.begin(), other.end());
			return *this;
		}

		vector& operator=(vector&& other);

		vector& operator=(std::initializer_list<T> ilist)
		{
			assign(ilist.begin(), ilist.end());
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

		allocator_type get_allocator() const { return data_allocator; }

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

		constexpr pointer data() noexcept { return start; }
		constexpr const_pointer data() const noexcept { return start; }

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

		_NODISCARD constexpr bool empty() const noexcept { return begin() == end(); }
		size_type size() const noexcept { return size_type(end() - begin()); }
		size_type max_size() const { return size_type(-1) / sizeof(T); }
		size_type capacity() const noexcept { return size_type(end_of_storage - begin()); }

		void reserve(size_type new_cap)
		{
			if (new_cap > capacity())
				_realloc_n_and_move(new_cap, begin(), end());
		}

		void shrink_to_fit()
		{
			if (end_of_storage != end())
				_realloc_n_and_move(size(), begin(), end());
		}

		void clear() noexcept { erase(begin(), end()); }

		/*
			it's temporarily beyond my comprehension why use const_iterator here,
			and it's complicated to use it, so I use iterator for now
			pending modification
		*/
		iterator insert(iterator pos, const T& val)
			{ return emplace(pos, val); }

		iterator insert(iterator pos, T&& val)
			{ return emplace(pos, move(val)); }

		iterator insert(iterator pos, size_type n, const T& val)
		{
			if (n != 0)
			{
				auto now = _insert_spare_n(pos, n);
				fill(now.first, now.second, val);
				n -= now.second - now.first;
				uninitialized_fill_n(now.second, n, val, data_allocator);
				return now.first;
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
			if (now.first != now.second)
				*(now.second) = value_type(forward<Args>(args)...);
			else
				alloc_traits::construct(data_allocator, now.second, forward<Args>(args)...);
			return now.first;
		}

		iterator erase(iterator pos)
		{
			if (pos + 1 != end())
				move(pos + 1, finish, pos);
			--finish;
			alloc_traits::destroy(data_allocator, finish);
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
			if (finish != end_of_storage)
			{
				alloc_traits::construct(data_allocator, finish, val);
				++finish;
			}
			else emplace(end(), val);
		}

		void push_back(T&& val)
		{
			if (finish != end_of_storage)
			{
				alloc_traits::construct(data_allocator, finish, move(val));
				++finish;
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

		void swap(vector& other)
		{
			TinySTL::swap(start, other.start);
			TinySTL::swap(finish, other.finish);
			TinySTL::swap(end_of_storage, other.end_of_storage);
		}


	protected:
		template <class Iterator>
		void _alloc_n_and_copy(size_type n, Iterator first, Iterator last)
		{
			start          = alloc_traits::allocate(data_allocator, n);
			finish         = uninitialized_copy(first, last, start, data_allocator);
			end_of_storage = start + n;
		}

		void _alloc_n_and_init(size_type n, value_type val)
		{
			start          = alloc_traits::allocate(data_allocator, n);
			finish         = uninitialized_fill_n(start, n, val, data_allocator);
			end_of_storage = start + n;
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
			/* can't get n through last-first, unable to allocate at once */
			for (; first != last; ++first)
				push_back(*first);
		}

		template <class ForwardIter>
		void _range_initialize(ForwardIter first, ForwardIter last,
							   forward_iterator_tag)
		{
			size_type n = distance(first, last);

			start          = alloc_traits::allocate(data_allocator, n);
			finish         = uninitialized_copy(first, last, start, data_allocator);
			end_of_storage = start + n;
		}

		void _destroy_and_dealloc()
		{
			alloc_traits::destroy(data_allocator, start, finish);
			alloc_traits::deallocate(data_allocator, start, end_of_storage - start);
		}

		template <class InputIter>
		iterator _insert(iterator pos, InputIter first, InputIter last,
						 input_iterator_tag)
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
				auto now = _insert_spare_n(pos, n);
				iterator mid = first;
				advance(mid, now.second - now.first);
				copy(first, mid, now.first);
				return uninitialized_copy(mid, last, now.second, data_allocator);
			}
			return pos;
		}

		pair<typename vector<T, Alloc>::iterator,
			 typename vector<T, Alloc>::iterator>
		_insert_spare_n(iterator pos, size_type n);

		template <class Iterator>
		void _realloc_n_and_copy(size_type n, Iterator first, Iterator last)
		{
			_destroy_and_dealloc();
			start          = alloc_traits::allocate(data_allocator, n);
			finish         = uninitialized_copy(first, last, start, data_allocator);
			end_of_storage = start + n;
		}

		template <class Iterator>
		void _realloc_n_and_move(size_type n, Iterator first, Iterator last)
		{
			iterator tmp_s = alloc_traits::allocate(data_allocator, n);
			iterator tmp_t = uninitialized_move(first, last, tmp_s, data_allocator);
			_destroy_and_dealloc();
			start		   = tmp_s;
			finish         = tmp_t;
			end_of_storage = start + n;
		}

		template <class InputIter>
		void _assign(InputIter first, InputIter last, input_iterator_tag);

		template <class ForwardIter>
		void _assign(ForwardIter first, ForwardIter last, forward_iterator_tag);

		void _pointer_clear()
		{
			start		   = pointer();
			finish		   = pointer();
			end_of_storage = pointer();
		}

	};

	template <class T, class Alloc>
	vector<T, Alloc>&
	vector<T, Alloc>::operator=(vector<T, Alloc>&& other)
	{
		if (this != addressof(other))
		{
			_destroy_and_dealloc();

			start          = other.start;
			finish         = other.finish;
			end_of_storage = other.end_of_storage;
		}
		other._pointer_clear();
		return *this;
	}

	template <class T, class Alloc>
	void 
	vector<T, Alloc>::assign(size_type n, const T& val)
	{
		if (n > size())
		{
			fill(begin(), end(), val);
			insert(end(), n - size(), val);
		}
		else
			erase(fill_n(begin(), n, val), end());
	}

	template <class T, class Alloc>
	template <class InputIter>
	void
	vector<T, Alloc>::_assign(InputIter first, InputIter last,
							  input_iterator_tag)
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
	vector<T, Alloc>::_assign(ForwardIter first, ForwardIter last,
							  forward_iterator_tag)
	{
		size_type n = distance(first, last);
		if (n > capacity())
		{
			_realloc_n_and_copy(n, first, last);
		}
		else if (n > size())
		{
			ForwardIter mid = first;
			advance(mid, size());
			copy(first, mid, start);
			finish = uninitialized_copy(mid, last, finish, data_allocator);
		}
		else
		{
			iterator tmp_t = copy(first, last, start);
			alloc_traits::destroy(data_allocator, tmp_t, finish);
			finish = tmp_t;
		}
	}

	/*
		initialized block / uninitialized block
	*/
	template <class T, class Alloc>
	pair<typename vector<T, Alloc>::iterator,
		 typename vector<T, Alloc>::iterator>
	vector<T, Alloc>::_insert_spare_n(iterator pos, size_type n)
	{
		if (size_type(end_of_storage - finish) >= n)
		{
			const size_type elems_after = finish - pos;
			iterator old_t = finish;
			if (elems_after > n)
			{
				uninitialized_move(finish - n, finish, finish, data_allocator);
				finish += n;
				move_backward(pos, old_t - n, old_t);
				return make_pair(pos, pos + n);
			}
			else
			{
				finish += n - elems_after;
				if (pos != finish)
				{
					uninitialized_copy(pos, old_t, finish, data_allocator);
					finish += elems_after;
				}
				return make_pair(pos, old_t);
			}
		}
		else
		{
			size_type len = size() + n;
			iterator tmp_s = alloc_traits::allocate(data_allocator, len);
			iterator tmp_t = uninitialized_move(start, pos, tmp_s, data_allocator);
			iterator ret = tmp_t;
			advance(tmp_t, n);
			tmp_t = uninitialized_move(pos, finish, tmp_t, data_allocator);
			_destroy_and_dealloc();
			start          = tmp_s;
			finish         = tmp_t;
			end_of_storage = start + len;
			return make_pair(ret, ret);
		}
	}

	template <class T, class Alloc>
	bool operator ==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return lhs.size() == rhs.size() && 
			equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <class T, class Alloc>
	bool operator !=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class T, class Alloc>
	bool operator <(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return lexicographical_compare(lhs.begin(), lhs.end(),
									   rhs.begin(), rhs.end());
	}

	template <class T, class Alloc>
	bool operator <=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return !(lhs > rhs);
	}

	template <class T, class Alloc>
	bool operator >(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template <class T, class Alloc>
	bool operator >=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template <class T, class Alloc>
	inline void swap(vector<T, Alloc>& x, vector<T, Alloc>& y)
	{
		x.swap(y);
	}

	template <class T, class Alloc, class U>
	constexpr typename vector<T,Alloc>::size_type
	erase(vector<T, Alloc>& v, const U& value)
	{
		auto it = remove(v.begin(), v.end(), value);
		auto ret = distance(it, v.end());
		v.erase(it, v.end());
		return ret;
	}

	template <class T, class Alloc, class Pred>
	constexpr typename vector<T, Alloc>::size_type
	erase_if(vector<T, Alloc>& v, Pred pred)
	{
		auto it = remove_if(v.begin(), v.end(), pred);
		auto ret = distance(it, v.end());
		v.erase(it, v.end());
		return ret;
	}


}

#endif /* _TINYSTL_VECTOR_H_ */