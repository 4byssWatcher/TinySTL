#pragma once
#ifndef _TINYSTL_FUNCTIONAL_H_
#define _TINYSTL_FUNCTIONAL_H_

#include "utility.h"

namespace TinySTL
{
    template <class Arg, class Result>
    class unary_function
    {
    public:
        using argument_type = Arg;
        using result_type   = Result;
    };

    template <class Arg1, class Arg2, class Result>
    class binary_function
    {
    public:
        using first_argument_type  = Arg1;
        using second_argument_type = Arg2;
        using result_type          = Result;
    };

    template<class Arg>
    class equal_to
    {
    public:
        bool operator ()(const Arg& x, const Arg& y) { return x == y; }
    };

    template<>
    class equal_to<void>
    {
    public:
        template <class T, class U>
        auto operator()(T&& x, U&& y)
        ->decltype(forward<T>(x) == forward<U>(y))
        {
            return forward<T>(x) == forward<U>(y);
        }
    };

    template <class Arg = void>
    class less
    {
    public:
        bool operator()(const Arg& x, const Arg& y) { return x < y; }
    };

    template <>
    class less<void>
    {
    public:
        template <class T, class U>
        auto operator()(T&& x, U&& y)
        ->decltype(forward<T>(x) < forward<U>(y))
        {
            return forward<T>(x) < forward<U>(y);
        }
    };

    /* ... */
}

#endif /* _TINYSTL_FUNCTIONAL_H_ */