#pragma once
#ifndef _TINYSTL_TYPE_TRAITS_H_
#define _TINYSTL_TYPE_TRAITS_H_

namespace TinySTL
{
    template <class ...>
    using void_t = void;

    template <bool B, class T = void>
    struct enable_if {};

    template <class T>
    struct enable_if<true, T> { using type = T; };

    template <bool B, class T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    template<class T>
    struct remove_reference
    {
        using type = T;
    };

    template<class T>
    struct remove_reference<T&>
    {
        using type = T;
    };

    template<class T>
    struct remove_reference<T&&>
    {
        using type = T;
    };

    template <class T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <class T>
    struct remove_extent
    {
        using type = T;
    };

    template <class T, size_t n>
    struct remove_extent<T[n]>
    {
        using type = T;
    };

    template <class T>
    struct remove_extent<T[]>
    {
        using type = T;
    };

    template <class T>
    using remove_extent_t = typename remove_extent<T>::type;
}

#endif /* _TINYSTL_TYPE_TRAITS_H_ */