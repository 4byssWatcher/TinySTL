#pragma once
#ifndef _TINYSTL_TYPE_TRAITS_H_
#define _TINYSTL_TYPE_TRAITS_H_

#include "iterator.h"

namespace TinySTL
{
    template <class ...>
    using void_t = void;

    template <bool B, class T = void>
    class enable_if {};

    template <class T>
    class enable_if<true, T> { typedef T type; };

    template <bool B, class T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    template<class T>
    class remove_reference
    {
    public:
        typedef T type;
    };

    template<class T>
    class remove_reference<T&>
    {
    public:
        typedef T type;
    };

    template<class T>
    class remove_reference<T&&>
    {
    public:
        typedef T type;
    };

    template <class T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <class T, class = void>
    bool _is_iterator_v = false;

    template <class T>
    bool _is_iterator_v<T, void_t<typename iterator_traits<T>::iterator_category>> = true;
}

#endif /* _TINYSTL_TYPE_TRAITS_H_ */