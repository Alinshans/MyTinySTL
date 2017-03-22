#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

// 这个头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include <new>
#include <utility>

#include "type_traits.h"
#include "iterator.h"

namespace mystl
{

// construct 构造对象的内容

template <class Ty>
void construct(Ty* ptr)
{
  ::new ((void*)ptr) Ty();
}

template <class Ty1, class Ty2>
void construct(Ty1* ptr, const Ty2& value)
{
  ::new ((void*)ptr) Ty1(value);
}

template <class Ty, class... Args>
void construct(Ty* ptr, Args&&... args)
{
  ::new ((void*)ptr) Ty(std::forward<Args>(args)...);
}

// destroy 将对象析构

template <class FIter>
void __destroy_aux(FIter first, FIter last, __false_type)
{
  for (; first != last; ++first)
    destroy(&*first);
}

template <class FIter>
void __destroy_aux(FIter first, FIter last, __true_type) {}

template <class FIter, class Ty>
void __destroy(FIter first, FIter last, Ty*)
{
  return __destroy_aux(first, last,
                       __type_traits<Ty>::has_trivial_destructor());
}

template <class Ty>
void destroy(Ty* pointer)
{
  pointer->~Ty();
}

template <class FIter>
void destroy(FIter first, FIter last)
{
  __destroy(first, last, value_type(first));
}

} // namespace mystl
#endif // !MYTINYSTL_CONSTRUCT_H_

