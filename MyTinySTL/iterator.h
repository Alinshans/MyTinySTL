#ifndef MYTINYSTL_ITERATOR_H_
#define MYTINYSTL_ITERATOR_H_

// 这个头文件用于基本的迭代器设计，包含了一些模板结构体与全局函数，

#include <cstddef>

namespace mystl {

// 五种迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// input_iterator
template <class T, class Distance>
struct input_iterator {
    typedef input_iterator_tag          iterator_category;
    typedef T                           value_type;
    typedef Distance                    difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
};

// output_iterator
template <class T, class Distance>
struct output_iterator {
    typedef output_iterator_tag         iterator_category;
    typedef T                           value_type;
    typedef Distance                    difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
};

// forward_iterator
template <class T, class Distance>
struct forward_iterator {
    typedef forward_iterator_tag        iterator_category;
    typedef T                           value_type;
    typedef Distance                    difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
};

// bidirectional_iterator
template <class T, class Distance>
struct bidirectional_iterator {
    typedef bidirectional_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef Distance                    difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
};

// random_access_iterator
template <class T, class Distance>
struct random_access_iterator {
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef Distance                    difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
};

// iterator 泛型模板
template <class Category, class T, class Distance = ptrdiff_t , 
    class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category                    iterator_category;
    typedef T                           value_type;
    typedef Distance                    difference_type;
    typedef Pointer                     pointer;
    typedef Reference                   reference;
};

// 以下函数用于萃取迭代器的特性
template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category  iterator_category;
    typedef typename Iterator::value_type         value_type;
    typedef typename Iterator::difference_type    difference_type;
    typedef typename Iterator::pointer            pointer;
    typedef typename Iterator::reference          reference;
};

// 针对原生指针的偏特化版本
template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag            iterator_category;
    typedef T                                     value_type;
    typedef ptrdiff_t                             difference_type;
    typedef T*                                    pointer;
    typedef T&                                    reference;
};

template <class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag            iterator_category;
    typedef T                                     value_type;
    typedef ptrdiff_t                             difference_type;
    typedef const T*                              pointer;
    typedef const T&                              reference;
};

// 萃取某个迭代器的 category
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();
}

// 萃取某个迭代器的 distance_type
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 萃取某个迭代器的 value_type
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 以下函数用于计算迭代器间的距离
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
    return __distance(first, last, iterator_category(first));
}

// distance 的 input_iterator_tag 的版本
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

// distance 的 random_access_iterator_tag 的版本
template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
    __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
    return last - first;
}

// 以下函数用于让迭代器前进 n 个距离
template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
    __advance(i, n, iterator_category(i));
}

// advance 的 input_iterator_tag 的版本
template <class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
    while (n--)  ++i;
}

// advance 的 bidirectional_iterator_tag 的版本
template <class BidirectionalIterator, class Distance>
inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
    if (n >= 0)
        while (n--)  ++i;
    else
        while (n++)  --i;
}

// advance 的 random_access_iterator_tag 的版本
template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
    i += n;
}

} // namespace mystl

#include "reverse_iterator.h"    // 包含 reverse_iterator

#endif // !MYTINYSTL_ITERATOR_H_

