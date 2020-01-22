#pragma once
#include <cstddef>
#ifndef _TINYSTL_MEMORY_H_
#define _TINYSTL_MEMORY_H_

namespace TinySTL
{
	template<class T>
	class default_deleter
	{
		void operator ()(T* ptr) { if (ptr)delete ptr; }
	};
	/* for array */
	template<class T>
	class default_deleter<T[]>
	{
		void operator ()(T* ptr) { if (ptr)delete[] ptr; }
	};

	template<class T, class D = default_deleter<T> >
	class unique_ptr
	{
	public:
		typedef  T    element_type;
		typedef  D    deleter_type;
		typedef  T* pointer;
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
			if (&_up != this)
			{
				clean();
				data = _up.data;
				_up.data = nullptr;
			}
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
		const poiner operator ->()const { return data; }



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
		return lhs.data == rhs.data;
	}

	template <class T, class D>
	bool operator== (const unique_ptr<T, D>& lhs, nullptr_t _p)
	{
		return lhs.data == _p;
	}

	template <class T, class D>
	bool operator== (nullptr_t _p, const unique_ptr<T, D>& rhs)
	{
		return _p == rhs.data;
	}



	template <class T1, class D1, class T2, class D2>
	bool operator!= (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.data != rhs.data;
	}

	template <class T, class D>
	bool operator!= (const unique_ptr<T, D>& lhs, nullptr_t _p)
	{
		return lhs.data != _p;
	}

	template <class T, class D>
	bool operator!= (nullptr_t _p, const unique_ptr<T, D>& rhs)
	{
		return _p != rhs.data;
	}



	template <class T1, class D1, class T2, class D2>
	bool operator< (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.data < rhs.data;
	}

	template <class T1, class D1, class T2, class D2>
	bool operator<= (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.data <= rhs.data;
	}

	template <class T1, class D1, class T2, class D2>
	bool operator> (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.data > rhs.data;
	}

	template <class T1, class D1, class T2, class D2>
	bool operator>= (const unique_ptr<T1, D1>& lhs, const unique_ptr<T2, D2>& rhs)
	{
		return lhs.data >= rhs.data;
	}

	template <class T, class... Targs>
	unique_ptr<T> make_unique(Targs&&... args)
	{
		return unique_ptr<T>(new T(std::forward<Targs>(args)...));
	}
}

#endif /* _TINYSTL_MEMORY_H_ */