#pragma once
#ifndef _TINYSTL_TYPE_TRAITS_H_
#define _TINYSTL_TYPE_TRAITS_H_

namespace TinySTL
{
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

    /* ... */
}

#endif /* _TINYSTL_TYPE_TRAITS_H_ */