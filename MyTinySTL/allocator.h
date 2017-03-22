#ifndef MYTINYSTL_ALLOCATOR_H_
#define MYTINYSTL_ALLOCATOR_H_

// 这个头文件包含一个模板类 allocator，用于管理对象的分配

#include "alloc.h"    
#include "construct.h"

namespace mystl
{

// 模板类：allocator
// 接受两个参数，参数一表示对象的类型，参数二表示空间配置器的类型，默认使用 mystl::alloc
template <class T, class Alloc = mystl::alloc>
class allocator
{
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

  static void construct(T* ptr);
  static void construct(T* ptr, const T& value);
  static void construct(T* ptr, T&& value);

  template <class... Args>
  static void construct(T* ptr, Args ...args);

  static void destroy(T* ptr);
  static void destroy(T* first, T* last);
};

template <class T, class Alloc>
T* allocator<T, Alloc>::allocate()
{
  return static_cast<T*>(Alloc::allocate(sizeof(T)));
}

template <class T, class Alloc>
T* allocator<T, Alloc>::allocate(size_type n)
{
  if (n == 0)  return nullptr;
  return static_cast<T*>(Alloc::allocate(n * sizeof(T)));
}

template <class T, class Alloc>
void allocator<T, Alloc>::deallocate(T* ptr)
{
  if (ptr == nullptr)  return;
  Alloc::deallocate(ptr, sizeof(T));
}

template <class T, class Alloc>
void allocator<T, Alloc>::deallocate(T* ptr, size_type n)
{
  if (ptr == nullptr)  return;
  Alloc::deallocate(ptr, n * sizeof(T));
}

template <class T, class Alloc>
void allocator<T, Alloc>::construct(T* ptr)
{
  mystl::construct(ptr);
}

template <class T, class Alloc>
void allocator<T, Alloc>::construct(T* ptr, const T& value)
{
  mystl::construct(ptr, value);
}

template<class T, class Alloc>
 void allocator<T, Alloc>::construct(T* ptr, T&& value)
{
  mystl::construct(ptr, std::move(value));
}

template<class T, class Alloc>
template<class ...Args>
 void allocator<T, Alloc>::construct(T* ptr, Args ...args)
{
  mystl::construct(ptr, std::forward<Args>(args)...);
}

template <class T, class Alloc>
void allocator<T, Alloc>::destroy(T* ptr)
{
  mystl::destroy(ptr);
}

template <class T, class Alloc>
void allocator<T, Alloc>::destroy(T* first, T* last)
{
  mystl::destroy(first, last);
}

} // namespace mystl
#endif // !MYTINYSTL_ALLOCATOR_H_

