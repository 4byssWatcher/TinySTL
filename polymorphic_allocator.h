#pragma once
#ifndef _TINYSTL_POLYMORPHIC_ALLOCATOR_
#define _TINYSTL_POLYMORPHIC_ALLOCATOR_

/* thread-unsafe for now */

#include "allocator.h"
#include "memory.h"

namespace TinySTL
{
	using max_align_t = double;
	using byte = char;

	class memory_resource
	{
		static constexpr size_t max_align = alignof(max_align_t);

		static memory_resource* default_resource;

		friend memory_resource* get_default_resource();
		friend memory_resource* set_default_resource(memory_resource* r);

	public:
		virtual ~memory_resource() noexcept {}

		void* allocate(size_t bytes, size_t align = max_align)
			{ return do_allocate(bytes, align); }
		void deallocate(void* ptr, size_t bytes, size_t align = max_align)
			{ return do_deallocate(ptr, bytes, align); }
		bool is_equal(const memory_resource& other) const noexcept
			{ return do_is_equal(other); }

	protected:
		virtual void* do_allocate(size_t bytes, size_t align)                  = 0;
		virtual void  do_deallocate(void* ptr, size_t bytes, size_t align)     = 0;
		virtual bool  do_is_equal(const memory_resource& other) const noexcept = 0;

	};

	memory_resource* get_default_resource();
	memory_resource* set_default_resource(memory_resource* r);

	inline bool operator==(const memory_resource& x, const memory_resource& y)
	{
		return &x == &y || x.is_equal(y);
	}

	inline bool operator!=(const memory_resource& x, const memory_resource& y)
	{
		return !(x == y);
	}

	template <class Allocator>
	class resource_adaptor_imp :public memory_resource
	{
	public:
		using alloc_traits   = allocator_traits<Allocator>;
		using allocator_type = Allocator;

	protected:
		Allocator m_alloc;

	public:
		resource_adaptor_imp() = default;

		resource_adaptor_imp(const resource_adaptor_imp& other) = default;

		resource_adaptor_imp(resource_adaptor_imp&& other) = default;

		explicit resource_adaptor_imp(const Allocator& a2)
			:m_alloc(a2) {};
		explicit resource_adaptor_imp(Allocator&& a2)
			:m_alloc(move(a2)) {};

	protected:
		/* not sure of align...can't find sign of it in allocator_traits */
		void* do_allocate(size_t bytes, size_t align)
			{ return alloc_traits::allocate(m_alloc, bytes /*, align*/); }
		void do_deallocate(void* ptr, size_t bytes, size_t align)
			{ return alloc_traits::deallocate(m_alloc,
				static_cast<typename alloc_traits::pointer>(ptr), bytes /*, align*/); }

		bool do_is_equal(const memory_resource& other) const noexcept
		{
			const resource_adaptor_imp* other_p =
				dynamic_cast<const resource_adaptor_imp*>(&other);
			if (other_p)
				return m_alloc == other_p->m_alloc;
			else
				return false;
		}

		allocator_type get_allocator() const { return m_alloc; }
	};

	/* sizeof(byte) == 1 */
	template <class Allocator>
	using resource_adaptor = resource_adaptor_imp<
		typename allocator_traits<Allocator>::template rebind_alloc<byte> >;

	/* NOT in standard, just want to use allocator.h */
	using default_allocator_resource = resource_adaptor<allocator<byte> >;

	/* 
		to avoid duplicate definition
		see default_singleton in polymorphic_allocator.cpp
	*/

	extern default_allocator_resource default_singleton;

	template <class T>
	class polymorphic_allocator
	{
	private:
		memory_resource* m_resource;
	public:
		using value_type      = T;
		using size_type       = size_t;
		using difference_type = ptrdiff_t;
		using reference       = T&;
		using const_reference = const T&;
		using pointer         = T*;
		using const_pointer   = const T*;

	public:
		polymorphic_allocator() noexcept
			:m_resource(get_default_resource()) {}
		polymorphic_allocator(memory_resource* r)
			:m_resource(r ? r : get_default_resource()) {}
		polymorphic_allocator(const polymorphic_allocator& other)
			:m_resource(other.resource()) {}
		template <class U>
		polymorphic_allocator(const polymorphic_allocator<U>& other) noexcept
			:m_resource(other.resource()) {}

		T* allocate(size_type n)
		{
			return static_cast<pointer>(m_resource->allocate(n * sizeof(T), alignof(T)));
		}
		void deallocate(pointer ptr, size_type n)
		{
			return m_resource->deallocate(ptr, n * sizeof(T), alignof(T));
		}

		polymorphic_allocator select_on_container_copy_construction() const
			{ return polymorphic_allocator(); }
		memory_resource* resource() const
			{ return m_resource; }
	};

	template <class T1, class T2>
	inline bool operator==(const polymorphic_allocator<T1>& x,
						   const polymorphic_allocator<T2>& y)
	{
		return *x.resource() == *y.resource();
	}

	template <class T1, class T2>
	inline bool operator!=(const polymorphic_allocator<T1>& x,
						   const polymorphic_allocator<T2>& y)
	{
		return !(x == y);
	}

}

#endif /* _TINYSTL_POLYMORPHIC_ALLOCATOR_ */
