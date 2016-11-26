#ifndef MYTINYSTL_ALLOCATOR_H_
#define MYTINYSTL_ALLOCATOR_H_

// 这个头文件包含一个模板类 allocator，用于对象内存的分配与释放

#include "alloc.h"    

namespace mystl {

// 模板类：allocator
// 接受两个参数，参数一表示对象的类型，参数二表示空间配置器的类型
// allocate   : 用于对象内存的分配，提供两个版本
// deallocate : 用于对象内存的释放，提供两个版本
template <class T, class Alloc>
class allocator {
public:
    typedef T            value_type;
    typedef T*           pointer;
    typedef const T*     const_pointer;
    typedef T&           reference;
    typedef const T&     const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

public:
    static T*   allocate();
    static T*   allocate(size_type n);
    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);
};

// allocate 第一个版本：没有参数，返回分配内存的起始位置
template <class T, class Alloc>
T* allocator<T, Alloc>::allocate() {
    return static_cast<T*>(Alloc::allocate(sizeof(T)));
}

// allocate 第二个版本：接受一个值，表示要分配的对象个数，返回分配内存的起始位置
template <class T, class Alloc>
T* allocator<T, Alloc>::allocate(size_type n) {
    if (n == 0)  return nullptr;
    return static_cast<T*>(Alloc::allocate(n * sizeof(T)));
}

// deallocate 第一个版本：接受一个指针，释放其内存，无返回值
template <class T, class Alloc>
void allocator<T, Alloc>::deallocate(T* ptr) {
    if (ptr == nullptr)  return;
    Alloc::deallocate(ptr, sizeof(T));
}

// deallocate 第二个版本：接受一个指针和一个数值，释放 n 个对象的内存，无返回值
template <class T, class Alloc>
void allocator<T, Alloc>::deallocate(T* ptr, size_type n) {
    if (ptr == nullptr)  return;
    Alloc::deallocate(ptr, n * sizeof(T));
}

} // namespace mystl
#endif // !MYTINYSTL_ALLOCATOR_H_

