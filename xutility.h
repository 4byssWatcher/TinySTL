#pragma once
#ifndef _TINYSTL_XUTILITY_H_
#define _TINYSTL_XUTILITY_H_

#include <cstddef>
#include "type_traits.h"

/* pointer_traits and some type judgement */

namespace TinySTL
{
    template <class T>
    struct _get_first_parameter;

    template <template <class, class...> class T, class First, class... Rest>
    struct _get_first_parameter<T<First, Rest...> >
    {
        using type = First;
    };

    template <class Newfirst, class T>
    struct _replace_first_parameter;

    template <class Newfirst, template <class, class...> class T, class First, class... Rest>
    struct _replace_first_parameter<Newfirst, T<First, Rest...> >
    {
        using type = T<Newfirst, Rest...>;
    };

    template <class T, class = void>
    class _get_element_type 
    {
        using type = typename _get_first_parameter<T>::type;
    };

    template <class T>
    struct _get_element_type<T, void_t<typename T::element_type> > 
    {
        using type = typename T::element_type;
    };

    template <class T, class = void>
    class _get_ptr_difference_type
    {
        using type = ptrdiff_t;
    };

    template <class T>
    struct _get_ptr_difference_type<T, void_t<typename T::difference_type> >
    {
        using type = typename T::difference_type;
    };

    template <class T, class Other, class = void>
    struct _get_rebind_alias 
    {
        using type = typename _replace_first_parameter<Other, T>::type;
    };

    template <class T, class Other>
    struct _get_rebind_alias<T, Other, void_t<typename T::template rebind<Other> > > 
    {
        using type = typename T::template rebind<Other>;
    };

	template <class T>
	class pointer_traits
	{
        using pointer         =  T;
        using element_type    =  typename _get_element_type<T>::type;
        using difference_type =  typename _get_ptr_difference_type<T>::type;

        template <class Other>
        using rebind          =  typename _get_rebind_alias<T, Other>::type;

        static pointer pointer_to(element_type& ref)
        {
            return T::pointer_to(ref);
        }
	};

    template <class T>
    class pointer_traits<T*>
    {
        using pointer         =  T*;
        using element_type    =  T;
        using difference_type =  ptrdiff_t;

        template <class Other>
        using rebind = Other*;

        static pointer pointer_to(element_type& ref)
        {
            return addressof(ref);
        }
    };
}

#endif /* _TINYSTL_XUTILITY_H_ */