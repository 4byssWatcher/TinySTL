#pragma once
#ifndef _TINYSTL_QUEUE_H_
#define _TINYSTL_QUEUE_H_

#include "deque.h"
#include "utility.h"

namespace TinySTL
{
	template <class T, class Container = deque<T> >
	class queue
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
		queue() :container() {}
		explicit queue(const Container& cont) :container(cont) {}
		explicit queue(Container&& cont) :container(TinySTL::move(cont)) {}
		queue(const queue& other) :container(other.container) {}
		queue(queue&& other) :container(TinySTL::move(other.container)) {}
		
		template <class Alloc>
		explicit queue(const Alloc& alloc)
			:container(alloc) {}

		template <class Alloc>
		explicit queue(const Container& cont, const Alloc& alloc)
			:container(cont, alloc) {}

		template <class Alloc>
		explicit queue(Container&& cont, const Alloc& alloc)
			:container(TinySTL::move(cont), alloc) {}

		template <class Alloc>
		explicit queue(const queue& other, const Alloc& alloc)
			:container(other.container, alloc) {}

		template <class Alloc>
		explicit queue(queue&& other, const Alloc& alloc)
			:container(TinySTL::move(other.container), alloc) {}

		queue& operator=(const queue& other)
		{
			container = other.container;
			return *this;
		}

		queue& operator=(queue&& other)
		{
			container = TinySTL::move(other.container);
			return *this;
		}

	public:
		reference front()
			{ return container.front(); }
		reference back()
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
		void swap(queue& other)
			{ container.swap(other.container); }

	public:
		template <class T, class Container>
		friend bool operator ==(const queue<T, Container>& lhs,
								const queue<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator !=(const queue<T, Container>& lhs,
								const queue<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator < (const queue<T, Container>& lhs,
							    const queue<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator <=(const queue<T, Container>& lhs,
								const queue<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator > (const queue<T, Container>& lhs,
								const queue<T, Container>& rhs);
		template <class T, class Container>
		friend bool operator >=(const queue<T, Container>& lhs,
								const queue<T, Container>& rhs);
	};

	template <class T, class Container>
	bool
	operator ==(const queue<T, Container>& lhs,
				const queue<T, Container>& rhs)
	{
		return lhs.container == rhs.container;
	}

	template <class T, class Container>
	bool
	operator !=(const queue<T, Container>& lhs,
				const queue<T, Container>& rhs)
	{
		return lhs.container != rhs.container;
	}

	template <class T, class Container>
	bool
	operator < (const queue<T, Container>& lhs,
				const queue<T, Container>& rhs)
	{
		return lhs.container < rhs.container;
	}

	template <class T, class Container>
	bool
	operator <=(const queue<T, Container>& lhs,
			    const queue<T, Container>& rhs)
	{
		return lhs.container <= rhs.container;
	}

	template <class T, class Container>
	bool
	operator > (const queue<T, Container>& lhs,
				const queue<T, Container>& rhs)
	{
		return lhs.container > rhs.container;
	}

	template <class T, class Container>
	bool
	operator >=(const queue<T, Container>& lhs,
				const queue<T, Container>& rhs)
	{
		return lhs.container >= rhs.container;
	}

	template <class T, class Container>
	void swap(queue<T, Container>& x, queue<T, Container>& y)
		{ x.swap(y); }
}

#endif /* _TINYSTL_QUEUE_H_ */