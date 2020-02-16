#pragma once
#ifndef _TINYSTL_ITERATOR_H_
#define _TINYSTL_ITERATOR_H_

namespace TinySTL
{
    class input_iterator_tag{};
	class output_iterator_tag{};
	class forward_iterator_tag :public input_iterator_tag {};
	class bidirectional_iterator_tag :public forward_iterator_tag {};
	class random_access_iterator_tag :public bidirectional_iterator_tag {};

    template <class T,class Distance>
    class input_iterator
    {
    public:
        typedef input_iterator_tag  iterator_category;
        typedef T                   value_type;
        typedef Distance            difference_type;
        typedef T*                  pointer;
        typedef T&                  reference;
    };

    template <class T,class Distance>
    class output_iterator
    {
    public:
        typedef output_iterator_tag  iterator_category;
        typedef void                 value_type;
        typedef void                 difference_type;
        typedef void                 pointer;
        typedef void                 reference;
    };

    template <class T,class Distance>
    class forward_iterator
    {
    public:
        typedef forward_iterator_tag  iterator_category;
        typedef T                     value_type;
        typedef Distance              difference_type;
        typedef T*                    pointer;
        typedef T&                    reference;
    };

    template <class T,class Distance>
    class bidirectional_iterator
    {
    public:
        typedef bidirectional_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Distance                    difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    template <class T,class Distance>
    class random_access_iterator
    {
    public:
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Distance                    difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    template <class Category, class T, 
              class Distance = ptrdiff_t, 
              class Pointer = T*, 
              class Reference = T&>
    class iterator
    {
    public:
        typedef Category  iterator_category;
        typedef T         value_type;
        typedef Distance  difference_type;
        typedef Pointer   pointer;
        typedef Reference reference;
    };

    template <class Iterator>
    class iterator_traits
    {
    public:
        typedef typename Iterator::iterator_category  iterator_category;
        typedef typename Iterator::value_type         value_type;
        typedef typename Iterator::difference_type    difference_type;
        typedef typename Iterator::pointer            pointer;
        typedef typename Iterator::reference          reference;
    };

    template <class T>
    class iterator_traits<T*>
    {
    public:
        typedef random_access_iterator_tag    iterator_category;
        typedef T                             value_type;
        typedef ptrdiff_t                     difference_type;
        typedef T*                            pointer;
        typedef T&                            reference;
    };

    template <class T>
    class iterator_traits<const T*>
    {
    public:
        typedef random_access_iterator_tag    iterator_category;
        typedef T                             value_type;
        typedef ptrdiff_t                     difference_type;
        typedef const T*                      pointer;
        typedef const T                       reference;
    };

    template <class Iterator>
    typename iterator_traits<Iterator>::iterator_category 
    iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category category;
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
        typedef typename iterator_traits<Iterator>::iterator_category 
                iterator_category;
        typedef typename iterator_traits<Iterator>::value_type
                value_type;
        typedef typename iterator_traits<Iterator>::difference_type
                difference_type;
        typedef typename iterator_traits<Iterator>::pointer 
                pointer;
        typedef typename iterator_traits<Iterator>::reference
                reference;

        typedef Iterator                    iterator_type;
        typedef reverse_iterator<Iterator>  Self;

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
}

#endif /* _TINYSTL_ITERATOR_H_ */