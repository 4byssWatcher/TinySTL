#pragma once
#ifndef _TINYSTL_ITERATOR_H_
#define _TINYSTL_ITERATOR_H_

#include "type_traits.h"

namespace TinySTL
{
    class input_iterator_tag{};
	class output_iterator_tag{};
	class forward_iterator_tag :public input_iterator_tag {};
	class bidirectional_iterator_tag :public forward_iterator_tag {};
	class random_access_iterator_tag :public bidirectional_iterator_tag {};

    template <class T, class Distance>
    struct input_iterator
    {
        using iterator_category = input_iterator_tag;
        using value_type        = T;
        using difference_type   = Distance;
        using pointer           = T*;
        using reference         = T&;
    };

    template <class T, class Distance>
    struct output_iterator
    {
        using iterator_category = output_iterator_tag;
        using value_type        = void;
        using difference_type   = void;
        using pointer           = void;
        using reference         = void;
    };

    template <class T, class Distance>
    struct forward_iterator
    {
        using iterator_category = forward_iterator_tag;
        using value_type        = T;
        using difference_type   = Distance;
        using pointer           = T*;
        using reference         = T&;
    };

    template <class T, class Distance>
    struct bidirectional_iterator
    {
        using iterator_category = bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = Distance;
        using pointer           = T*;
        using reference         = T&;
    };

    template <class T, class Distance>
    struct random_access_iterator
    {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = Distance;
        using pointer           = T*;
        using reference         = T&;
    };

    template <class Category, class T, 
              class Distance = ptrdiff_t, 
              class Pointer = T*, 
              class Reference = T&>
    struct iterator
    {
        using iterator_category = Category;
        using value_type        = T;
        using difference_type   = Distance;
        using pointer           = Pointer;
        using reference         = Reference;
    };

    template <class Non_Iter, class = void>
    struct iterator_traits_base {};

    template <class Iterator>
    struct iterator_traits_base<Iterator, void_t<typename Iterator::iterator_category> >
    {
        using iterator_category = typename Iterator::iterator_category;
        using value_type        = typename Iterator::value_type;
        using difference_type   = typename Iterator::difference_type;
        using pointer           = typename Iterator::pointer;
        using reference         = typename Iterator::reference;
    };

    template <class Iterator>
    struct iterator_traits :iterator_traits_base<Iterator> {};

    template <class T>
    struct iterator_traits<T*>
    {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    };

    template <class T>
    struct iterator_traits<const T*>
    {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T;
    };

    template <class Iterator>
    typename iterator_traits<Iterator>::iterator_category 
    iterator_category(const Iterator&)
    {
        using category = typename iterator_traits<Iterator>::iterator_category;
        return category();
    }

    template <class Iterator>
    typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    template <class Iterator>
    typename iterator_traits<Iterator>::difference_type*
    difference_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    template <class Iterator>
    class reverse_iterator
    {
    protected:
        Iterator current;
    public:
        using iterator_category =
            typename iterator_traits<Iterator>::iterator_category;
        using value_type        =
            typename iterator_traits<Iterator>::value_type;
        using difference_type   =
            typename iterator_traits<Iterator>::difference_type;
        using pointer           =
            typename iterator_traits<Iterator>::pointer;
        using reference         =
            typename iterator_traits<Iterator>::reference;

        using iterator_type = Iterator;
        using Self          = reverse_iterator<Iterator>;

    public:
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type x) :current(x) {}
        reverse_iterator(const Self& x) :current(x.current) {}
        
        template <class Iter>
        reverse_iterator(const reverse_iterator<Iter>& x) : current(x.base()) {}

        iterator_type base() const { return current; }

        /* reverse(first) -> end-1  ,  reverse(end-1) -> first */
        reference operator*()const
        {
            Iterator tmp = current;
            return *--tmp;
        }

        pointer operator->()const { return &(operator*()); }

        Self& operator++() // prefix
        {
            --current;
            return *this;
        }
        Self& operator++(int) // suffix
        {
            Self tmp = *this;
            --current;
            return tmp;
        }
        Self& operator--()
        {
            ++current;
            return *this;
        }
        Self& operator--(int)
        {
            Self tmp = *this;
            ++current;
            return tmp;
        }
        Self operator+(difference_type n) const 
        {
            return Self(current - n);
        }
        Self& operator+=(difference_type n)
        {
            current -= n;
            return *this;
        }
        Self operator-(difference_type n) const
        {
            return Self(current + n);
        }
        Self& operator-=(difference_type n)
        {
            current += n;
            return *this;
        }
        reference operator[](difference_type n) const { return *(*this + n); }
    };

    template <class Iterator>
    inline bool operator==(const reverse_iterator<Iterator>& x,
                           const reverse_iterator<Iterator>& y)
    {
        return x.base() == y.base();
    }

    template <class Iterator>
    inline bool operator<(const reverse_iterator<Iterator>& x,
                          const reverse_iterator<Iterator>& y)
    {
        return y.base() < x.base();
    }

    template <class Iterator>
    inline bool operator!=(const reverse_iterator<Iterator>& x,
                           const reverse_iterator<Iterator>& y) 
    {
        return !(x == y);
    }

    template <class Iterator>
    inline bool operator>(const reverse_iterator<Iterator>& x,
                          const reverse_iterator<Iterator>& y)
    {
        return y < x;
    }

    template <class Iterator>
    inline bool operator<=(const reverse_iterator<Iterator>& x,
                           const reverse_iterator<Iterator>& y)
    {
        return !(y < x);
    }

    template <class Iterator>
    inline bool operator>=(const reverse_iterator<Iterator>& x,
                           const reverse_iterator<Iterator>& y)
    {
        return !(x < y);
    }

    template <class Iterator>
    inline typename reverse_iterator<Iterator>::difference_type
    operator-(const reverse_iterator<Iterator>& x,
              const reverse_iterator<Iterator>& y)
    {
        return y.base() - x.base();
    }

    /* just for test, NOT completed, pending modification */
    template <class InputIter>
    typename iterator_traits<InputIter>::difference_type
    distance(InputIter first, InputIter last)
    {
        typename iterator_traits<InputIter>::difference_type dis = 0;
        while (first != last)
        {
            ++dis;
            ++first;
        }
        return dis;
    }

    template <class InputIter, class Distance>
    void advance(InputIter &first, Distance dis)
    {
        if (dis > 0)
        {
            while (dis--)
                ++first;
        }
        else
        {
            while (dis++)
                ++first;
        }
    }

    template <class T, class U = void>
    constexpr bool _is_iterator_v = false;

    template <class T>
    constexpr bool _is_iterator_v<T, void_t<typename iterator_traits<T>::iterator_category>> = true;
}

#endif /* _TINYSTL_ITERATOR_H_ */