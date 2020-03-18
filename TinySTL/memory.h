#pragma once
#ifndef _TINYSTL_MEMORY_H_
#define _TINYSTL_MEMORY_H_

#include <cstddef>//std::true_type
#include <functional>//std::function
#include <type_traits>//std::is_trivial
#include "algorithm.h"
#include "iterator.h"
#include "type_traits.h"
#include "utility.h"
#include "xmemory.h"

namespace TinySTL
{
	template <class T>
	class default_deleter
	{
	public:
		void operator ()(T* ptr) { if (ptr)delete ptr; }
	};
	/* for array */
	template <class T>
	class default_deleter<T[]>
	{
	public:
		void operator ()(T* ptr) { if (ptr)delete[] ptr; }
	};

	/* never mind EBO... */
	template <class T,
			  class D = std::conditional_t<std::is_array_v<T>,
										   default_deleter<remove_extent_t<T>[]>,
										   default_deleter<remove_extent_t<T> > > >
	class unique_ptr
	{
	public:
		using element_type = remove_extent_t<T>;
		using deleter_type = D;
		using pointer      = element_type*;
		using size_type    = size_t;
	private:
		pointer       data;
		deleter_type  deleter;

	public:
		template <class U>
		explicit unique_ptr(U* _data = nullptr) :data(_data) {}

		template <class U>
		unique_ptr(U* _data, D _del) :data(_data), deleter(_del) {}

		unique_ptr(unique_ptr&& _up)
		{
			data = _up.data;
			_up.data = nullptr;
		}
		unique_ptr& operator = (unique_ptr&& _up)
		{
			this->swap(_up);
			return *this;
		}

		/* disable copy/assign from lvalue */
		unique_ptr(const unique_ptr& _up) = delete;
		unique_ptr& operator = (const unique_ptr& _up) = delete;

		~unique_ptr() { clean(); }

		pointer get() { return data; }
		const pointer get()const { return data; }

		deleter_type get_deleter() { return deleter; }
		const deleter_type get_deleter()const { return deleter; }

		operator bool() { return data != nullptr; }

		element_type& operator *() { return *data; }
		const element_type& operator *()const { return *data; }

		pointer operator ->() { return data; }
		const pointer operator ->()const { return data; }

		element_type& operator [](size_type n) { return *(data + n); }
		const element_type& operator [](size_type n)const { return *(data + n); }

		void reset(pointer _data = nullptr)
		{
			clean();
			data = _data;
		}

		void release()
		{
			pointer ret = data;
			data = nullptr;
			return ret;
		}

		void swap(unique_ptr& _up)
		{
			pointer tmp = data;
			data = _up.data;
			_up.data = tmp;
		}

		template <class T, class D>
		void swap(unique_ptr<T, D>& _x, unique_ptr<T, D>& _y)
		{
			_x.swap(_y);
		}

	private:
		void clean()
		{
			deleter(data);
			data = nullptr;
		}
	};

	template <class T1, class D1, class T2, class D2>
	bool operator== (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.get() == rhs.get();
	}

	template <class T, class D>
	bool operator== (const unique_ptr<T, D>& lhs, nullptr_t _p)
	{
		return lhs.get() == _p;
	}

	template <class T, class D>
	bool operator== (nullptr_t _p, const unique_ptr<T, D>& rhs)
	{
		return _p == rhs.get();
	}



	template <class T1, class D1, class T2, class D2>
	bool operator!= (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.get() != rhs.get();
	}

	template <class T, class D>
	bool operator!= (const unique_ptr<T, D>& lhs, nullptr_t _p)
	{
		return lhs.get() != _p;
	}

	template <class T, class D>
	bool operator!= (nullptr_t _p, const unique_ptr<T, D>& rhs)
	{
		return _p != rhs.get();
	}



	template <class T1, class D1, class T2, class D2>
	bool operator< (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.get() < rhs.get();
	}

	template <class T1, class D1, class T2, class D2>
	bool operator<= (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.get() <= rhs.get();
	}

	template <class T1, class D1, class T2, class D2>
	bool operator> (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.get() > rhs.get();
	}

	template <class T1, class D1, class T2, class D2>
	bool operator>= (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.get() >= rhs.get();
	}

	template <class T, class... Targs>
	unique_ptr<T> make_unique(Targs&&... args)
	{
		return unique_ptr<T>(new T(forward<Targs>(args)...));
	}

	template <class T>
	void _default_deleter(T* _data)
	{
		if (_data)delete _data;
	}

	/*
		just a stupid implementation
		this works but there should be a better one...
		not able to understand how it's implemented in STL for now
		pending modification
	*/

	template <class T>
	class shared_ptr;

	class ref_type_base
	{
	public:
		template <class T>
		friend class shared_ptr;

	protected:
		size_t refcount;

	protected:
		ref_type_base() :refcount(0) {}

		void inc_ref()
			{ ++refcount; }

		virtual void  dec_ref() = 0;
		virtual void* get_data() = 0;

		ref_type_base* copy_ref()
		{
			inc_ref();
			return this;
		}

	};

	template <class T, class D>
	class ref_type_impl : ref_type_base
	{
	public:
		template <class T>
		friend class shared_ptr;

	protected:
		using element_type = T;
		using deleter_type = D;
		using pointer	   = element_type*;
	protected:
		pointer       data;
		deleter_type  deleter;

	protected:
		void dec_ref()
		{
			--refcount;
			if (0 == refcount)
			{
				deleter(data);
				data = nullptr;
			}
		}

		void* get_data()
			{ return static_cast<void*>(data); }

		ref_type_impl(pointer _data, deleter_type _del)
			:data(_data), deleter(_del)
		{
			if (data)inc_ref();
		}

		~ref_type_impl()
		{
			dec_ref();
			if (0 == refcount)deleter(data);
			data = nullptr;
		}
	};


	template <class T>
	class shared_ptr
	{
	public:
		using element_type = remove_extent_t<T>;
		using pointer      = element_type*;
		using size_type	   = size_t;

	private:
		ref_type_base*  ref;

	public:
		shared_ptr() {}
		shared_ptr(nullptr_t) {}

		template <class U>
		explicit shared_ptr(U* data)
		{
			// operator() of default_deleter redefined
			if constexpr (std::is_array_v<T>)
			{
				ref = new ref_type_impl<U,
					default_deleter<U[]> >(data, default_deleter<U[]>{});
			}
			else
			{
				ref = new ref_type_impl<U,
					default_deleter<U> >(data, default_deleter<U>{});
			}
		}

		template <class U, class D>
		shared_ptr(U* data, D del)
			:ref(new ref_type_impl<element_type, D>(data, del)) {}

		shared_ptr(const shared_ptr& sp)
			:ref(sp.ref->copy_ref()) {}

		shared_ptr& operator = (shared_ptr& sp)
		{
			if (this != &sp)
			{
				ref->dec_ref();
				ref = copy_ref(sp.ref);
			}
		}

		~shared_ptr()
		{
			if(ref)
				ref->dec_ref();
		}

		pointer get()
			{ return static_cast<pointer>(ref->get_data()); }
		const pointer get()const
			{ return static_cast<pointer>(ref->get_data()); }

		template <class Deleter>
		Deleter* get_deleter()
		{
			if(ref)
				return static_cast<Deleter*>(ref->deleter);
		}

		operator bool() { return ref->data != nullptr; }

		element_type& operator *()
			{ return *(static_cast<pointer>(ref->get_data())); }
		const element_type& operator *()const
			{ return *(static_cast<pointer>(ref->get_data())); }

		pointer operator ->()
			{ return static_cast<pointer>(ref->get_data()); }
		const pointer operator ->()const
			{ return static_cast<pointer>(ref->get_data()); }

		element_type& operator [](size_type n)
		{
			return *(static_cast<pointer>(ref->get_data()) + n);
		}
		const element_type operator [](size_type n)const
		{
			return *(static_cast<pointer>(ref->get_data()) + n);
		}
	};

	template <class T1, class T2>
	bool operator== (const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs)
	{
		return lhs.get() == rhs.get();
	}

	template <class T>
	bool operator== (const shared_ptr<T>& lhs, nullptr_t _p)
	{
		return lhs.get() == _p;
	}

	template <class T>
	bool operator== (nullptr_t _p, const shared_ptr<T>& rhs)
	{
		return _p == rhs.get();
	}



	template <class T1, class T2>
	bool operator!= (const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs)
	{
		return lhs.get() != rhs.get();
	}

	template <class T>
	bool operator!= (const shared_ptr<T>& lhs, nullptr_t _p)
	{
		return lhs.get() != _p;
	}

	template <class T>
	bool operator!= (nullptr_t _p, const shared_ptr<T>& rhs)
	{
		return _p != rhs.get();
	}



	template <class T1, class T2>
	bool operator< (const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs)
	{
		return lhs.get() < rhs.get();
	}
	template <class T1, class T2>
	bool operator<= (const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs)
	{
		return lhs.get() <= rhs.get();
	}
	template <class T1, class T2>
	bool operator> (const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs)
	{
		return lhs.get() > rhs.get();
	}
	template <class T1, class T2>
	bool operator>= (const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs)
	{
		return lhs.get() >= rhs.get();
	}

	template <class T,class...Targs>
	shared_ptr<T> make_shared(Targs&&...args)
	{
		return shared_ptr<class T>(::new T(forward<Targs>((args)...)));
	}

	/* reinterpret_cast<T&>(x) == reinterpret_cast<T>(&x) */
	template <class T>
	inline T* addressof(T& ref)
	{
		return reinterpret_cast<T*>
					(&const_cast<char&>
							(reinterpret_cast<const volatile char&>(ref)));
	}

	template <class InputIter, class ForwardIter, class Alloc>
	ForwardIter uninitialized_copy(InputIter first, InputIter last,
								   ForwardIter result, Alloc& alloc)
	{
		using Value = typename iterator_traits<ForwardIter>::value_type;
		return _uninitialized_copy(first, last, result, std::is_trivial<Value>(), alloc);
	}

	template <class InputIter, class ForwardIter, class Alloc>
	ForwardIter _uninitialized_copy(InputIter first, InputIter last, ForwardIter result,
									std::true_type, Alloc& alloc)
	{
		return copy(first, last, result);
	}

	template <class InputIter, class ForwardIter, class Alloc>
	ForwardIter _uninitialized_copy(InputIter first, InputIter last, ForwardIter result,
									std::false_type, Alloc& alloc)
	{
		ForwardIter current = result;
		try 
		{
			for (; first != last; ++first, ++current)
			{
				allocator_traits<Alloc>::construct(alloc,
					addressof(*current), *first);
			}
			return current;
		}
		catch (...) //commit or rollback
		{
			for (; result != current; ++result) 
			{
				allocator_traits<Alloc>::destroy(alloc, result);
			}
			throw;
		}
	}

	template <class ForwardIter, class T, class Alloc>
	ForwardIter uninitialized_fill(ForwardIter first, ForwardIter last,
								   const T& val, Alloc& alloc)
	{
		using Value = typename iterator_traits<ForwardIter>::value_type;
		return _uninitialized_fill(first, last, val, std::is_trivial<Value>(), alloc);
	}

	template <class ForwardIter, class T, class Alloc>
	ForwardIter _uninitialized_fill(ForwardIter first, ForwardIter last,
								    const T& val, std::true_type, Alloc& alloc)
	{
		return fill(first, last, val);
	}

	template <class ForwardIter, class T, class Alloc>
	ForwardIter _uninitialized_fill(ForwardIter first, ForwardIter last,
									const T& val, std::false_type, Alloc& alloc)
	{
		ForwardIter current = first;
		try
		{
			for (; current != last; ++current)
			{
				allocator_traits<Alloc>::construct(alloc,
					addressof(*current), val);
			}
			return current;
		}
		catch (...) //commit or rollback
		{
			for (; first != current; ++first)
			{
				allocator_traits<Alloc>::destroy(alloc, first);
			}
			throw;
		}
	}

	template <class ForwardIter, class Size, class T, class Alloc>
	ForwardIter uninitialized_fill_n(ForwardIter first, Size n,
									 const T& val, Alloc& alloc)
	{
		using Value = typename iterator_traits<ForwardIter>::value_type;
		return _uninitialized_fill_n(first, n, val, std::is_trivial<Value>(), alloc);
	}

	template <class ForwardIter, class Size, class T, class Alloc>
	ForwardIter _uninitialized_fill_n(ForwardIter first, Size n, const T& val,
									  std::true_type, Alloc& alloc)
	{
		return fill_n(first, n, val);
	}

	template <class ForwardIter, class Size, class T, class Alloc>
	ForwardIter _uninitialized_fill_n(ForwardIter first, Size n, const T& val,
									  std::false_type, Alloc& alloc)
	{
		ForwardIter current = first;
		try
		{
			for (; n>0; --n, ++current)
			{
				allocator_traits<Alloc>::construct(alloc,
					addressof(*current), val);
			}
			return current;
		}
		catch (...) //commit or rollback
		{
			for (; first != current; ++first)
			{
				allocator_traits<Alloc>::destroy(alloc, first);
			}
			throw;
		}
	}

	template <class InputIter, class ForwardIter, class Alloc>
	ForwardIter uninitialized_move(InputIter first, InputIter last,
								   ForwardIter result, Alloc& alloc)
	{
		using Value = typename iterator_traits<ForwardIter>::value_type;
		return _uninitialized_move(first, last, result, std::is_trivial<Value>(), alloc);
	}

	template <class InputIter, class ForwardIter, class Alloc>
	ForwardIter _uninitialized_move(InputIter first, InputIter last, ForwardIter result,
									std::true_type, Alloc& alloc)
	{
		return move(first, last, result);
	}

	template <class InputIter, class ForwardIter, class Alloc>
	ForwardIter _uninitialized_move(InputIter first, InputIter last, ForwardIter result,
									std::false_type, Alloc& alloc)
	{
		ForwardIter current = result;
		try
		{
			for (; first != last; ++first, ++current)
			{
				allocator_traits<Alloc>::construct(alloc,
					addressof(*current), move(*first));
			}
			return current;
		}
		catch (...) //commit or rollback
		{
			for (; result != current; ++result)
			{
				allocator_traits<Alloc>::destroy(alloc, result);
			}
			throw;
		}
	}
}

#endif /* _TINYSTL_MEMORY_H_ */