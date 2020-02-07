#pragma once
#ifndef _TINYSTL_TYPE_TRAITS_H_
#define _TINYSTL_TYPE_TRAITS_H_

namespace TinySTL
{
    template<class T>
    class remove_reference
    {
        typedef T type;
    };

    template<class T>
    class remove_reference<T&>
    {
        typedef T type;
    };

    template<class T>
    class remove_reference<T&&>
    {
        typedef T type;
    };

    /* ... */
}

#endif /* _TINYSTL_TYPE_TRAITS_H_ */