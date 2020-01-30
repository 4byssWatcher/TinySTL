#pragma once
#ifndef _TINYSTL_ITERATOR_H_
#define _TINYSTL_ITERATOR_H_

namespace TinySTL
{
    class input_iterator_tag {};
	class output_iterator_tag {};
	class forward_iterator_tag :public input_iterator_tag {};
	class bidirectional_iterator_tag :public forward_iterator_tag {};
	class random_access_iterator_tag :public bidirectional_iterator_tag {};

    template<class T, class Distance>
    class input_iterator
    {
        typedef input_iterator_tag  iterator_category;
        typedef T                   value_type;
        typedef Distance            difference_type;
        typedef T*                  pointer;
        typedef T&                  reference;
    };

    template<class T, class Distance>
    class output_iterator
    {
        typedef output_iterator_tag  iterator_category;
        typedef void                 value_type;
        typedef void                 difference_type;
        typedef void                 pointer;
        typedef void                 reference;
    };

    template<class T, class Distance>
    class forward_iterator
    {
        typedef forward_iterator_tag  iterator_category;
        typedef T                     value_type;
        typedef Distance              difference_type;
        typedef T*                    pointer;
        typedef T&                    reference;
    };

    template<class T, class Distance>
    class bidirectional_iterator
    {
        typedef bidirectional_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Distance                    difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    template<class T, class Distance>
    class random_access_iterator
    {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Distance                    difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    class iterator
    {
        typedef Category  iterator_category;
        typedef T         value_type;
        typedef Distance  difference_type;
        typedef Pointer   pointer;
        typedef Reference reference;
    };

    template<class Iterator>
    class iterator_traits
    {
        typedef typename Iterator::iterator_category  iterator_category;
        typedef typename Iterator::value_type         value_type;
        typedef typename Iterator::difference_type    difference_type;
        typedef typename Iterator::pointer            pointer;
        typedef typename Iterator::reference          reference;
    };

    template<class T>
    class iterator_traits<T*>
    {
        typedef random_access_iterator_tag    iterator_category;
        typedef T                             value_type;
        typedef ptrdiff_t                     difference_type;
        typedef T*                            pointer;
        typedef T&                            reference;
    };

    template<class T>
    class iterator_traits<const T*>
    {
        typedef random_access_iterator_tag    iterator_category;
        typedef T                             value_type;
        typedef ptrdiff_t                     difference_type;
        typedef const T*                      pointer;
        typedef const T                       reference;
    };

    template<class Iterator>
    typename iterator_traits<Iterator>::iterator_category 
    iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template<class Iterator>
    typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    template<class Iterator>
    typename iterator_traits<Iterator>::difference_type*
    difference_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }
}

#endif /* _TINYSTL_ITERATOR_H_ */