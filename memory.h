#pragma once
#ifndef _TINYSTL_MEMORY_H_
#define _TINYSTL_MEMORY_H_

#include <cstddef>//std::true_type
#include <functional>//std::function
#include <type_traits>//std::is_trivial
#include "algorithm.h"
#include "iterator.h"
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

	/* pending modification for EBO */
	template <class T, class D = default_deleter<T> >
	class unique_ptr
	{
	public:
		using element_type = T;
		using deleter_type = D;
		using pointer      = T*;
	private:
		pointer       data;
		deleter_type  deleter;

	public:
		explicit unique_ptr(T* _data = nullptr) :data(_data) {}
		unique_ptr(T* _data, D _del) :data(_data), deleter(_del) {}
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

	template <class T>
	class ref_type
	{
	public:
		using element_type = T;
		using deleter_type = std::function<void(T*)>;
		using pointer = T*;
	public:
		size_t        refcount;
		pointer       data;
		deleter_type  deleter;
	public:
		explicit ref_type(T* _data = nullptr, deleter_type _del = _default_deleter<T>)
			:refcount(0), data(_data), deleter(_del)
		{
			if (data)refcount = 1;
		}
		~ref_type()
		{
			--refcount;
			if (0 == refcount)deleter(data);
			data = nullptr;
		}
	};


	template <class T>
	class shared_ptr
	{
	public:
		using element_type = T;
		using deleter_type = std::function<void(T*)>;
		using pointer      = T*;
	private:
		ref_type<T>*  ref;

	public:
		void dec_ref()
		{
			if (ref->refcount && --ref->refcount==0)
			{
				ref->deleter(ref->data);
				ref->data = nullptr;
			}
		}

		void copy_ref(ref_type<T>* _r)
		{
			ref = _r;
			ref->refcount++;
		}

	public:
		explicit shared_ptr(T* _data = nullptr) :ref(new ref_type<T>(_data)) {}

		template <class D>
		shared_ptr(T* _data, D _del) : ref(new ref_type<T>(_data, _del)) {}

		shared_ptr(const shared_ptr& _sp)
		{
			copy_ref(_sp.ref);
		}

		shared_ptr& operator = (shared_ptr& _sp)
		{
			if (this != &_sp)
			{
				dec_ref();
				copy_ref(_sp.ref);
			}
		}

		~shared_ptr()
		{
			dec_ref();
		}

		pointer get() { return ref->data; }
		const pointer get()const { return ref->data; }

		deleter_type get_deleter() { return ref->deleter; }
		const deleter_type get_deleter()const { return ref->deleter; }

		operator bool() { return ref->data != nullptr; }

		element_type& operator *() { return *(ref->data); }
		const element_type& operator *()const { return *(ref->data); }

		pointer operator ->() { return ref->data; }
		const pointer operator ->()const { return ref->data; }
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

	template <class InputIter, class ForwardIter,
			  class Alloc = allocator_traits<typename iterator_traits<ForwardIter>::value_type>>
	ForwardIter uninitialized_copy(InputIter first, InputIter last,
								   ForwardIter result, Alloc& alloc = Alloc())
	{
		return _uninitialized_copy(first, last, result, std::is_trivial<InputIter>(), alloc);
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
		using Value = typename iterator_traits<ForwardIter>::value_type;
		ForwardIter current = result;
		try 
		{
			for (; first != last; ++first, ++current)
			{
				allocator_traits<Alloc>::construct(alloc,
					static_cast<void*>(addressof(*current)), Value(*first));
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

	template <class ForwardIter, class T,
			  class Alloc = allocator_traits<typename iterator_traits<ForwardIter>::value_type>>
	ForwardIter uninitialized_fill(ForwardIter first, ForwardIter last,
								   const T& val, Alloc& alloc = Alloc())
	{
		_uninitialized_fill(first, last, val, std::is_trivial<ForwardIter>(), alloc);
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
		using Value = typename iterator_traits<ForwardIter>::value_type;
		ForwardIter current = first;
		try
		{
			for (; current != last; ++current)
			{
				allocator_traits<Alloc>::construct(alloc,
					static_cast<void*>(addressof(*current)), Value(val));
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

	template <class ForwardIter, class Size, class T,
			  class Alloc = allocator_traits<typename iterator_traits<ForwardIter>::value_type>>
	ForwardIter uninitialized_fill_n(ForwardIter first, Size n,
									 const T& val, Alloc& alloc = Alloc())
	{
		return _uninitialized_fill_n(first, n, val, std::is_trivial<ForwardIter>(), alloc);
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
		using Value = typename iterator_traits<ForwardIter>::value_type;
		ForwardIter current = first;
		try
		{
			for (; n>0; --n, ++current)
			{
				allocator_traits<Alloc>::construct(alloc,
					static_cast<void*>(addressof(*current)), Value(val));
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

	template <class InputIter, class ForwardIter,
			  class Alloc = allocator_traits<typename iterator_traits<ForwardIter>::value_type>>
	ForwardIter uninitialized_move(InputIter first, InputIter last,
								   ForwardIter result, Alloc& alloc = Alloc())
	{
		return _uninitialized_move(first, last, result, std::is_trivial<InputIter>(), alloc);
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
		using Value = typename iterator_traits<ForwardIter>::value_type;
		ForwardIter current = first;
		try
		{
			for (; first != last; ++first, ++current)
			{
				allocator_traits<Alloc>::construct(alloc,
					static_cast<void*>(addressof(*current)), Value(move(*first)));
			}
			return current;
		}
		catch (...) //commit or rollback
		{
			for (; result != current; ++result)
			{
				allocator_traits<Alloc>::destroy(alloc,result);
			}
			throw;
		}
	}
}

#endif /* _TINYSTL_MEMORY_H_ */