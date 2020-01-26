#pragma once
#ifndef _TINYSTL_ALGORITHM_H_
#define _TINYSTL_ALGORITHM_H_

#include "iterator.h"

namespace TinySTL
{
    template<class InputIter,class UnaryPredicate>
    bool all_of(InputIter first,InputIter last,UnaryPredicate pred)
    {
        for(;first!=last;++first)
        {
            if(!pred(*first))
                return false;
        }
        return true;
    }

    template<class InputIter,class UnaryPredicate>
    bool any_of(InputIter first,InputIter last,UnaryPredicate pred)
    {
        for(;first!=last;++first)
        {
            if(pred(*first))
                return true;
        }
        return false;
    }

    template<class InputIter,class UnaryPredicate>
    bool none_of(InputIter first,InputIter last,UnaryPredicate pred)
    {
        for(;first!=last;++first)
        {
            if(pred(*first))
                return false;
        }
        return true;
    }

    /* return func:for purpose like getting the sum/total execution count? */
    template<class InputIter,class Function>
    Function for_each(InputIter first,InputIter last,Function func)
    {
        for(;first!=last;++first)
            func(first);
        return std::move(func);
    }

    template<class InputIter,class T>
    InputIter find(InputIter first,InputIter last,const T& val)
    {
        for(;first!=last;++first)
        {
            if(*first==val)return first;
        }
        return last;
    }

    template<class InputIter,class UnaryPredicate>
    InputIter find_if(InputIter first,InputIter last,UnaryPredicate pred)
    {
        for(;first!=last;++first)
        {
            if(pred(*first))return first;
        }
        return last;
    }

    template<class InputIter,class UnaryPredicate>
    InputIter find_if_not(InputIter first,InputIter last,UnaryPredicate pred)
    {
        for(;first!=last;++first)
        {
            if(!pred(*first))return first;
        }
        return last;
    }

    template<class ForwardIter1,class ForwardIter2>
    ForwardIter1 find_end(ForwardIter1 first1,ForwardIter1 last1,
                                ForwardIter2 first2,ForwardIter2 last2)
    {
        if(first2==last2)return last1;
        ForwardIter1 ret=last1;
        while(first1!=last1)
        {
            ForwardIter1 it1=first1;
            ForwardIter2 it2=first2;
            while(*it1==*it2)
            {
                ++it1;
                ++it2;
                if(it2==last2){ret=first1;break;}
                if(it1==last1)return ret;
            }
        }
        return ret;
    }

    template<class ForwardIter1,class ForwardIter2,class BinaryPredicate>
    ForwardIter1 find_end(ForwardIter1 first1,ForwardIter1 last1,
                            ForwardIter2 first2,ForwardIter2 last2,
                                BinaryPredicate pred)
    {
        if(first2==last2)return last1;
        ForwardIter1 ret=last1;
        while(first1!=last1)
        {
            ForwardIter1 it1=first1;
            ForwardIter2 it2=first2;
            while(pred(*it1,*it2))
            {
                ++it1;
                ++it2;
                if(it2==last2){ret=first1;break;}
                if(it1==last1)return ret;
            }
            ++first1;
        }
        return ret;
    }

    /*  not sure
    template<class BidirectIter1,class BidirectIter2>
    BidirectIter find_end(BidirectIter1 first1,BidirectIter1 last1,
                           BidirectIter2 first2,BidirectIter2 last2)
    {
        if(first2==last2)return last1;
        while(first1!=last1)
        {
            InputIter1 it1=last1;it1--;
            InputIter2 it2=last2;it2--;
            while(*it1==*it2)
            {
                if(it2==first2)return it1;
                if(it1==first1)return last1;
                --it1;
                --it2;
            }
            --last1;
        }
    }
    */

    template<class InputIter1,class ForwardIter2>
    InputIter1 find_first_of(InputIter1 first1,InputIter1 last1,
                                ForwardIter2 first2,ForwardIter2 last2)
    {
        if(first2==last2)return last1;
        while(first1!=last1)
        {
            for(ForwardIter2 it2=first2;it2!=last2;++it2)
            {
                if(*it2==*first1)
                    return first1;
            }
            ++first1;
        }
        return last1;
    }

    template<class InputIter1,class ForwardIter2,class BinaryPredicate>
    InputIter1 find_first_of(InputIter1 first1,InputIter1 last1,
                                ForwardIter2 first2,ForwardIter2 last2,
                                    BinaryPredicate pred)
    {
        if(first2==last2)return last1;
        while(first1!=last1)
        {
            for(ForwardIter2 it2=first2;it2!=last2;++it2)
            {
                if(pred(*it2,*first1))
                    return first1;
            }
            ++first1;
        }
        return last1;
    }

    template<class ForwardIter>
    ForwardIter adjacent_find(ForwardIter first,ForwardIter last)
    {
        ForwardIter it=first++;
        while(first!=last)
        {
            if(*first==*it)return it;
            it=first++;
        }
        return last;
    }

    template<class ForwardIter>
    ForwardIter adjacent_find(ForwardIter first,ForwardIter last,BinaryPredicate pred)
    {
        ForwardIter it=first++;
        while(first!=last)
        {
            if(pred(*first,*it))return it;
            it=first++;
        }
        return last;
    }

    template<class InputIter,class T>
    typename iterator_traits<InputIter>::difference_type
    count(InputIter first,InputIter last,const T& val)
    {
        typename iterator_traits<InputIter>::difference_type ret=0;
        while(first!=last)
        {
            if(*first==val)++ret;
            ++first;
        }
        return ret;
    }

    template<class InputIter,class UnaryPredicate>
    typename iterator_traits<InputIter>::difference_type
    count_if(InputIter first,InputIter last,UnaryPredicate pred)
    {
        typename iterator_traits<InputIter>::difference_type ret=0;
        while(first!=last)
        {
            if(pred(*first))++ret;
            ++first;
        }
        return ret;
    }

    template<class InputIter1,class InputIter2>
    pair<InputIter1,InputIter2>
    mismatch(InputIter1 first1,InputIter1 last1,InputIter2 first2)
    {
        while(first1!=last1 && (*first1==*first2))
        {
            ++first1;
            ++first2;
        }
        return make_pair(first1,first2);
    }

    template<class InputIter1,class InputIter2,,class BinaryPredicate>
    pair<InputIter1,InputIter2>
    mismatch(InputIter1 first1,InputIter1 last1,
                InputIter2 first2,BinaryPredicate pred)
    {
        while(first1!=last1 && pred(*first1,*first2))
        {
            ++first1;
            ++first2;
        }
        return make_pair(first1,first2);
    }

    template<class InputIter1,class InputIter2>
    bool equal(InputIter1 first1,InputIter1 last1,
                InputIter2 first2)
    {
        while(first1!=last1)
        {
            if(*first1!=*first2)return false;
            ++first1;
            ++first2;
        }
        return true;
    }

    template<class InputIter1,class InputIter2,class BinaryPredicate>
    bool equal(InputIter1 first1,InputIter1 last1,
                InputIter2 first2,BinaryPredicate pred)
    {
        while(first1!=last1)
        {
            if(!pred(*first1,*first2))return false;
            ++first1;
            ++first2;
        }
        return true;
    }

    template<class InputIter1,class InputIter2>
    bool is_permutation(InputIter1 first1,InputIter1 last1,
                            InputIter2 first2)
    {
        tie(first1,first2)=mismatch(first1,last1,first2);
        if(first1==last1)return true;
        InputIter2 last2=first2;
        advance(last2,distance(first1,last1));
        for(InputIter1 it1=first1,it1!=last1;++it1)
        {
            if(find(first1,last1,*it1)==it1)
            {
                auto n=count(first2,last2,*it1);
                if(n==0 || count(first1,last1,*it1)!=n)return false;
            }
        }
        return true;
    }

    template<class InputIter1,class InputIter2,class BinaryPredicate>
    bool is_permutation(InputIter1 first1,InputIter1 last1,
                            InputIter2 first2,BinaryPredicate pred)
    {
        tie(first1,first2)=mismatch(first1,last1,first2,pred);
        if(first1==last1)return true;
        InputIter2 last2=first2;
        advance(last2,distance(first1,last1));
        for(InputIter1 it1=first1,it1!=last1;++it1)
        {
            if(find_if(first1,last1,[&](InputIter1 _i1){return pred(*_i1,*it1);})==it1)
            {
                auto n=count_if(first2,last2,[&](InputIter2 _i2){return pred(*_i2,*it1);});
                if(n==0 || count_if(first1,last1,[&](InputIter1 _i1){return pred(*i1,*it1);})!=n)
                    return false;
            }
        }
        return true;
    }

    template<class ForwardIter1,class ForwardIter2>
    ForwardIter1 search(ForwardIter1 first1,ForwardIter1 last1,
                            ForwardIter2 first2,ForwardIter2 last2)
    {
        if(first2==last2)return last1;
        while(first1!=last1)
        {
            ForwardIter1 it1=first1;
            ForwardIter2 it2=first2;
            while(*it1==*it2)
            {
                ++it1;
                ++it2;
                if(it2==last2)return first1;
                if(it1==last1)return last1;
            }
        }
        return last1;
    }

    template<class ForwardIter1,class ForwardIter2>
    ForwardIter1 search(ForwardIter1 first1,ForwardIter1 last1,
                            ForwardIter2 first2,ForwardIter2 last2,
                                BinaryPredicate pred)
    {
        if(first2==last2)return last1;
        while(first1!=last1)
        {
            ForwardIter1 it1=first1;
            ForwardIter2 it2=first2;
            while(pred(*it1,*it2))
            {
                ++it1;
                ++it2;
                if(it2==last2)return first1;
                if(it1==last1)return last1;
            }
        }
        return last1;
    }

    template<class ForwardIter,class Size,class T>
    ForwardIter search_n(ForwardIter first,ForwardIter last,Size n,const T& val)
    {
        Size i;
        ForwardIter limit=first;
        advance(limit,distance(first,last)-n);
        while(first!=limit)
        {
            ForwardIter it=first;
            while(*it==val)
            {
                ++it;
                if(++i==n)return first;
            }
            ++first;
        }
        return last;
    }

    template<class ForwardIter,class Size,class UnaryPredicate>
    ForwardIter search_n(ForwardIter first,ForwardIter last,
                            Size n,UnaryPredicate pred)
    {
        Size i;
        ForwardIter limit=first;
        advance(limit,distance(first,last)-n);
        while(first!=limit)
        {
            ForwardIter it=first;
            while(pred(*it))
            {
                ++it;
                if(++i==n)return first;
            }
            ++first;
        }
        return last;
    }
}

#endif /* TINYSTL_ALGORITHM_H_ */