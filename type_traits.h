#pragma once
#ifndef _TINYSTL_TYPE_TRAITS_H_
#define _TINYSTL_TYPE_TRAITS_H_

namespace TinySTL
{
    template<class T>
    class remove_reference
    {
        typedef type T;
    };

    template<class T>
    class remove_reference<T&>
    {
        typedef type T;
    };

    template<class T>
    class remove_reference<T&&>
    {
        typedef type T;
    };

    /* ... */
}

#endif /* _TINYSTL_TYPE_TRAITS_H_ */