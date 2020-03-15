#pragma once
#ifndef _TINYSTL_PRIORITY_QUEUE_H_
#define _TINYSTL_PRIORITY_QUEUE_H_

#include "functional.h"
#include "heap.h"
#include "utility.h"
#include "vector.h"

namespace TinySTL
{
	template <class T, class Container = vector<T>,
					   class Compare = less<> >
	class priority_queue
	{

	public:
		using container_type  = Container;
		using value_compare   = Compare;
		using value_type	  = typename Container::value_type;
		using size_type		  = typename Container::size_type;
		using reference		  = typename Container::reference;
		using const_reference = typename Container::const_reference;

	protected:
		Container container;
		Compare compare;

	public: // constructor 1-6, 13-14
		priority_queue() :priority_queue(Compare(), Container()) {}

		explicit priority_queue(const Compare& comp)
			:priority_queue(comp, Container()) {}

		priority_queue(const Compare& comp, const Container& cont)
			:compare(comp), container(cont)
			{ make_heap(container.begin(), container.end(), compare); }

		priority_queue(const Compare& comp, Container&& cont)
			:compare(comp), container(TinySTL::move(cont))
			{ make_heap(container.begin(), container.end(), compare); }

		priority_queue(const priority_queue& other)
			:compare(other.compare), container(other.container) {}
		priority_queue(priority_queue&& other)
			:compare(other.compare), container(TinySTL::move(other.container)) {}

		template <class InputIter>
		priority_queue(InputIter first, InputIter last,
					   const Compare& comp, const Container& cont)
			:compare(comp), container(cont)
		{
			container.insert(container.end(), first, last);
			make_heap(container.begin(), container.end(), compare);
		}

		template <class InputIter>
		priority_queue(InputIter first, InputIter last,
					   const Compare& comp = Compare(),
					   Container&& cont = Container())
			: compare(comp), container(TinySTL::move(cont))
		{
			container.insert(container.end(), first, last);
			make_heap(container.begin(), container.end(), compare);
		}

		priority_queue& operator=(const priority_queue& other)
		{
			compare   = other.compare;
			container = other.container;
			return *this;
		}

		priority_queue& operator=(priority_queue&& other)
		{
			compare	  = other.compare;
			container = TinySTL::move(other.container);
			return *this;
		}

	public:
		const_reference top()
			{ return container.front(); }
		bool empty()
			{ return container.empty(); }
		size_type size()
			{ return container.size(); }

		void push(value_type val)
		{
			container.push_back(val);
			push_heap(container.begin(), container.end(), compare);
		}

		void pop()
		{
			pop_heap(container.begin(),container.end(),compare);
			container.pop_back();
		}

		template <class... Args>
		void emplace(Args&&... args)
		{
			container.emplace_back(TinySTL::forward<Args>(args)...);
			push_heap(container.begin(), container.end(), compare);
		}

		void swap(priority_queue& other)
		{
			TinySTL::swap(compare, other.compare);
			container.swap(other.container);
		}
	};

	template <class T, class Container, class Compare>
	void swap(priority_queue<T, Container, Compare>& x,
			  priority_queue<T, Container, Compare>& y)
	{
		x.swap(y);
	}
}

#endif /* _TINYSTL_PRIORITY_QUEUE_H_ */