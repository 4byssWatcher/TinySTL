#pragma once
#ifndef _TINYSTL_FUNCTIONAL_H_
#define _TINYSTL_FUNCTIONAL_H_

namespace TinySTL
{
    template <class Arg, class Result>
    class unary_function
    {
        typedef  Arg     argument_type;
        typedef  Result  result_type;
    };

    template <class Arg1, class Arg2, class Result>
    class binary_function
    {
        typedef  Arg1    first_argument_type;
        typedef  Arg2    second_argument_type;
        typedef  Result  result_type;
    };

    template<class Arg1, class Arg2, class Result>
    class equal_to:public binary_function<Arg1, Arg2, Result>
    {
        bool operator ()(const Arg1& x, const Arg2& y) { return x == y; }
    };

    template<class Arg1,class Arg2,class Result>
    class less :public binary_function<Arg1, Arg2, Result>
    {
        bool operator()(const Arg1& x, const Arg2& y) { return x < y; }
    };

    /* ... */
}

#endif /* _TINYSTL_FUNCTIONAL_H_ */