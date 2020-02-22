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
        typedef  Arg     argument_type;
        typedef  Result  result_type;
    };

    template <class Arg1, class Arg2, class Result>
    class binary_function
    {
    public:
        typedef  Arg1    first_argument_type;
        typedef  Arg2    second_argument_type;
        typedef  Result  result_type;
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