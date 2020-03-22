#pragma once
#ifndef _TINYSTL_STACK_H_
#define _TINYSTL_STACK_H_

#include "deque.h"
#include "utility.h"

namespace TinySTL
{
	template <class T, class Container = deque<T> >
	class stack
	{

	public:
		using container_type  = Container;
		using value_type	  = typename Container::value_type;
		using size_type		  = typename Container::size_type;
		using reference		  = typename Container::reference;
		using const_reference = typename Container::const_reference;

	protected:
		Container container;

	public:
		stack() :container() {}
		explicit stack(const Container& cont) :container(cont) {}
		explicit stack(Container&& cont) :container(TinySTL::move(cont)) {}
		stack(const stack& other) :container(other.container) {}
		stack(stack&& other) :container(TinySTL::move(other.container)) {}

		template <class Alloc>
		explicit stack(const Alloc& alloc)
			:container(alloc) {}

		template <class Alloc>
		explicit stack(const Container& cont, const Alloc& alloc)
			:container(cont, alloc) {}

		template <class Alloc>
		explicit stack(Container&& cont, const Alloc& alloc)
			:container(TinySTL::move(cont), alloc) {}

		template <class Alloc>
		explicit stack(const stack& other, const Alloc& alloc)
			:container(other.container, alloc) {}

		template <class Alloc>
		explicit stack(stack&& other, const Alloc& alloc)
			:container(TinySTL::move(other.container), alloc) {}

		stack& operator=(const stack& other)
		{
			container = other.container;
			return *this;
		}

		stack& operator=(stack&& other)
		{
			container = TinySTL::move(other.container);
			return *this;
		}

	public:
		reference top()
			{ return container.back(); }
		bool empty()
			{ return container.empty(); }
		size_type size()
			{ return container.size(); }
		void push(value_type val)
			{ container.push_back(val); }

		template <class... Args>
		void emplace(Args&&... args)
		{
			container.emplace_back(TinySTL::forward<Args>(args)...);
		}

		void pop()
			{ container.pop_back(); }
		void swap(stack& other)
			{ container.swap(other.container); }

	public:
		template <class T, class Container>
		friend bool operator ==(const stack<T, Container>& lhs,
								const stack<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator !=(const stack<T, Container>& lhs,
								const stack<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator < (const stack<T, Container>& lhs,
								const stack<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator <=(const stack<T, Container>& lhs,
								const stack<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator > (const stack<T, Container>& lhs,
								const stack<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator >=(const stack<T, Container>& lhs,
								const stack<T, Container>& rhs);
	};

	template <class T, class Container>
	bool
	operator ==(const stack<T, Container>& lhs,
				const stack<T, Container>& rhs)
		{ return lhs.container == rhs.container; }

	template <class T, class Container>
	bool
	operator !=(const stack<T, Container>& lhs,
				const stack<T, Container>& rhs)
		{ return lhs.container != rhs.container; }

	template <class T, class Container>
	bool
	operator < (const stack<T, Container>& lhs,
				const stack<T, Container>& rhs)
		{ return lhs.container < rhs.container; }

	template <class T, class Container>
	bool
	operator <=(const stack<T, Container>& lhs,
				const stack<T, Container>& rhs)
		{ return lhs.container <= rhs.container; }

	template <class T, class Container>
	bool
	operator > (const stack<T, Container>& lhs,
				const stack<T, Container>& rhs)
		{ return lhs.container > rhs.container; }

	template <class T, class Container>
	bool
	operator >=(const stack<T, Container>& lhs,
				const stack<T, Container>& rhs)
		{ return lhs.container >= rhs.container; }

	template <class T, class Container>
	void swap(stack<T, Container>& x, stack<T, Container>& y)
		{ x.swap(y); }
}

#endif /* _TINYSTL_STACK_H_ */