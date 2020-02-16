#pragma once
#ifndef _TINYSTL_XMEMORY_H_
#define _TINYSTL_XMEMORY_H_

#include "allocator.h"
#include "type_traits.h"
#include "utility.h"
#include <xtr1common>
/* allocator_traits */

namespace TinySTL
{
	template <class T, class = void>
	class _get_pointer_type
	{
		using type = typename T::value_type*;
	};

	template <class T>
	class _get_pointer_type<T, void_t<typename T::pointer>>
	{
		using type = typename T::pointer;
	};

	template <class T, class = void>
	class _get_const_pointer_type
	{
		using ptr_t = typename _get_pointer_type<T>::type;
		using val_t = typename T::value_type;
		using type = typename pointer_traits<ptr_t>::template rebind<const val_t>;
	};

	template <class T>
	class _get_const_pointer_type<T, void_t<typename T::const_pointer>>
	{
		using type = typename T::const_pointer;
	};

	template <class T, class = void>
	struct _get_void_pointer_type
	{
		using ptr_t = typename _get_pointer_type<T>::type;
		using type  = typename pointer_traits<ptr_t>::template rebind<void>;
	};

	template <class T>
	struct _get_void_pointer_type<T, void_t<typename T::void_pointer> >
	{
		using type  = typename T::void_pointer;
	};

	template <class T, class = void>
	struct _get_const_void_pointer_type
	{
		using ptr_t = typename _get_pointer_type<T>::type;
		using type  = typename pointer_traits<ptr_t>::template rebind<const void>;
	};

	template <class T>
	struct _get_const_void_pointer_type<T, void_t<typename T::const_void_pointer> >
	{
		using type = typename T::const_void_pointer;
	};

	template <class T, class = void>
	struct _get_difference_type
	{
		using ptr_t = typename _get_pointer_type<T>::type;
		using type = typename pointer_traits<ptr_t>::difference_type;
	};

	template <class T>
	struct _get_difference_type<T, void_t<typename T::difference_type> >
	{
		using type = typename T::difference_type;
	};

	/*
		shall use make_unsigned_t
		pending modification
	*/
	template <class T, class = void>
	struct _get_size_type
	{
		using type = typename _get_difference_type<T>::type;
	};

	template <class T>
	struct _get_size_type<T, void_t<typename T::size_type> >
	{
		using type = typename T::size_type;
	};

	template <class T, class Other, class = void>
	struct _get_rebind_type
	{
		using type = typename _replace_first_parameter<Other, T>::type;
	};

	template <class T, class Other>
	struct _get_rebind_type<T, Other, void_t<typename T::template rebind<Other>::other> > 
	{
		using type = typename T::template rebind<Other>::other;
	};

	template <class Alloc, class = void>
	class normal_allocator_traits
	{
		using allocator_type     = Alloc;
		using value_type         = typename Alloc::value_type;

		using pointer            = typename _get_pointer_type<Alloc>::type;
		using const_pointer      = typename _get_const_pointer_type<Alloc>::type;
		using void_pointer       = typename _get_void_pointer_type<Alloc>::type;
		using const_void_pointer = typename _get_const_void_pointer_type<Alloc>::type;

		using size_type          = typename _get_size_type<Alloc>::type;
		using difference_type    = typename _get_difference_type<Alloc>::type;

		/*for simplification
		using propagate_on_container_copy_assignment = typename _get_propagate_on_container_copy<Alloc>::type;
		using propagate_on_container_move_assignment = typename _get_propagate_on_container_move<Alloc>::type;
		using propagate_on_container_swap            = typename _get_propagate_on_container_swap<Alloc>::type;
		using is_always_equal                        = typename _get_is_always_equal<Alloc>::type;
		*/

		template <class Other>
		using rebind_alloc  = typename _get_rebind_type<Alloc, Other>::type;

		template <class Other>
		using rebind_traits = normal_allocator_traits<rebind_alloc<Other> >;

	};

	/* default */
	template <class Alloc>
	class default_allocator_traits
	{
		using allocator_type     = Alloc;
		using value_type         = typename Alloc::value_type;

		using pointer            = value_type*;
		using const_pointer      = const value_type*;
		using void_pointer       = void*;
		using const_void_pointer = const void*;

		using size_type          = size_t;
		using difference_type    = ptrdiff_t;

		/* for simplification
		using propagate_on_container_copy_assignment = false_type;
		using propagate_on_container_move_assignment = true_type;
		using propagate_on_container_swap			 = false_type;
		using is_always_equal						 = true_type;
		*/

		template <class Other>
		using rebind_alloc  = allocator<Other>;

		template <class Other>
		using rebind_traits = default_allocator_traits<allocator<Other> >;
	};

	template <class Alloc, class... Args>
	struct _has_allocator_construct :std::false_type {};

	template <class Alloc, class Pointer, class... Args>
	struct _has_allocator_construct<
				void_t<decltype(std::declval<Alloc&>().construct(
														std::declval<Pointer>(),
														std::declval<Args>()...))>,
					   Alloc,
				       Pointer,
				       Args...> :std::true_type {};

	template <class Alloc, class Pointer, class=void>
	struct _has_allocator_destroy :std::false_type {};

	template <class Alloc, class Pointer>
	struct _has_allocator_destroy
				<Alloc, Pointer, void_t<decltype(
											std::declval<Alloc&>().destroy(
												std::declval<Pointer>()))>> :std::true_type {};

	template <class Alloc, class = void>
	struct _has_allocator_max_size :std::false_type {};

	template <class Alloc>
	struct _has_allocator_max_size<Alloc, 
								   void_t<decltype(std::declval<Alloc&>().max_size())>
								  >:std::true_type {};

	template <class Alloc, class = void>
	struct _has_allocator_select_on :std::false_type {};

	template <class Alloc>
	struct _has_allocator_select_on<Alloc,void_t<decltype(
		std::declval<Alloc&>().select_on_container_copy_construction())> > :std::true_type {};
	
	template <class Alloc, class = void>
	struct _is_default_allocator : std::false_type {};

	template <class T>
	struct _is_default_allocator<allocator<T>, void_t<typename allocator<T>::_From_primary> >
		: std::is_same<typename allocator<T>::_From_primary, allocator<T> >::type {};

	template <class Alloc>
	struct allocator_traits:std::conditional_t<_is_default_allocator<Alloc>::value,
											   default_allocator_traits<Alloc>,
											   normal_allocator_traits<Alloc> > {};

	/* 
		allocate/... actually shall have two different versions for default/normal,
			for default_allocator has all these functions and there's no need for judgement
		but this also works!
		(I use this mainly because I didn't write select_on... in my own allocator)
		pending modification
	*/
	template <class Alloc>
	class allocator_traits : std::conditional_t<_is_default_allocator<Alloc>::value,
												default_allocator_traits<Alloc>,
												normal_allocator_traits<Alloc> >
	{
		_NODISCARD static constexpr pointer allocate(Alloc& a, const size_type n)
		{
			return a.allocate(n);
		}

		_NODISCARD static constexpr pointer allocate(Alloc& a, const size_type n,
			const const_void_pointer Hint)
		{
			if (std::_Has_allocate_hint<Alloc, size_type, const_void_pointer>::value)
				return a.allocate(n, Hint);
			else return a.allocate(n);
		}

		static void deallocate(Alloc& a, pointer ptr, size_type n)
		{
			a.deallocate(ptr, n);
		}

		template <class T, class... Args>
		static void construct(allocator_type alloc, T* ptr, Args&&... args)
		{
			if (_has_allocator_construct<alloc, ptr, args...>::value)
				alloc.construct(ptr, forward<Args>(args)...);
			else ::new(static_cast<void*>(ptr)) T(forward<Args>(args)...);
		}

		template <class T>
		static constexpr void destroy(Alloc& alloc, T* ptr)
		{
			if (_has_allocator_destroy<alloc, ptr>::value)
				alloc.destroy(ptr);
			else ptr->~T();
		}

		static constexpr size_type max_size(const Alloc& alloc) noexcept
		{
			if (_has_allocator_max_size<alloc>::value)
				return alloc.max_size();
			else return std::numeric_limits<size_type>::max() / sizeof(value_type);
		}

		static constexpr Alloc select_on_container_copy_construction(const Alloc& alloc)
		{
			if (_has_allocator_select_on<Alloc>::value)
				return alloc.select_on_container_copy_construction();
			else return alloc;
		}
	};
}

#endif /* _TINYSTL_XMEMORY_H_ */