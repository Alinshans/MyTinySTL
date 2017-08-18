#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

// 这个头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include <new>

#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)  // unused parameter
#endif // _MSC_VER

namespace mystl
{

// construct 构造对象

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
  ::new ((void*)ptr) Ty(mystl::forward<Args>(args)...);
}

// destroy 将对象析构

template <class Ty>
void destroy_one(Ty*, std::true_type) {}

template <class Ty>
void destroy_one(Ty* pointer, std::false_type)
{
  if (pointer != nullptr)
  {
    pointer->~Ty();
  }
}

template <class ForwardIter>
void destroy_cat(ForwardIter , ForwardIter , std::true_type) {}

template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type)
{
  for (; first != last; ++first)
    destroy(&*first);
}

template <class Ty>
void destroy(Ty* pointer)
{
  destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}

template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last)
{
  destroy_cat(first, last, std::is_trivially_destructible<
              typename iterator_traits<ForwardIter>::value_type>{});
}

} // namespace mystl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // !MYTINYSTL_CONSTRUCT_H_

