#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

// 这个头文件包含两个全局函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include <new>

#include "type_traits.h"
#include "iterator.h"

namespace mystl {
    
// construct
// 构造对象的内容
// 版本1：接受一个指针
// 版本2：接受一个指针和初值
template <class T1>
inline void construct(T1* p) {
    new (static_cast<void*>(p)) T1();
}

template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
    new (static_cast<void*>(p)) T1(value);
}

// destroy
// 将对象析构
// 版本1：接受一个指针
// 版本2：接受两个迭代器，指向析构区间首尾
template <class T>
inline void destroy(T* pointer) {
    pointer->~T();
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
    __destroy(first, last, value_type(first));
}

template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
    typedef typename __type_traits<T>::has_trivial_destructor TrivialDestructor;
    return __destroy_aux(first, last, TrivialDestructor());
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
    for (; first != last; ++first)
        destroy(&*first);
}

// 如果是对象有 trivial destructor，则什么也不做
template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

// 如果销毁的对象是指针，则什么也不做
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}
inline void destroy(int*, int*) {}
inline void destroy(long*, long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}

} // namespace mystl
#endif // !MYTINYSTL_CONSTRUCT_H_

