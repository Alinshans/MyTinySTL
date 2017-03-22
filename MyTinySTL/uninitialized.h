#ifndef MYTINYSTL_UNINITIALIZED_H_
#define MYTINYSTL_UNINITIALIZED_H_

// 这个头文件用于对未初始化空间构造元素

#include <cstring>

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"

namespace mystl
{

/*****************************************************************************************/
// uninitialized_copy
// 把[first, last)上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/
template <class IIter, class FIter>
FIter __uninitialized_copy(IIter first, IIter last, FIter result, __true_type)
{
  return mystl::copy(first, last, result);
}

template <class IIter, class FIter>
FIter __uninitialized_copy(IIter first, IIter last, FIter result, __false_type)
{
  auto cur = result;
  for (; first != last; ++first, ++cur)
  {
    mystl::construct(&*cur, *first);
  }
  return cur;
}

template <class IIter, class FIter>
FIter uninitialized_copy(IIter first, IIter last, FIter result)
{
  return mystl::__uninitialized_copy(first, last, result, 
                                     typename __type_traits<typename
                                     iterator_traits<IIter>::value_type
                                     >::is_POD_type());
}

/*****************************************************************************************/
// uninitialized_copy_n
// 把[first, first + n)上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/
template <class FIter, class Size, class T>
FIter __uninitialized_copy_n(FIter first, Size n, const T& x, __true_type)
{
  return mystl::copy_n(first, n, x);
}

template <class FIter, class Size, class T>
FIter __uninitialized_copy_n(FIter first, Size n, const T& x, __false_type)
{
  auto cur = first;
  for (; n > 0; --n, ++cur)
  {
    mystl::construct(&*cur, x);
  }
  return cur;
}

template <class FIter, class Size, class T>
FIter uninitialized_copy_n(FIter first, Size n, const T& x)
{
  return mystl::__uninitialized_copy_n(first, n, x, typename __type_traits<
                                       typename iterator_traits<FIter>::
                                       value_type>::is_POD_type());
}

/*****************************************************************************************/
// uninitialized_fill
// 在[first, last)区间内填充元素值
/*****************************************************************************************/
template <class FIter, class T>
void __uninitialized_fill(FIter first, FIter last, const T& x, __true_type)
{
  mystl::fill(first, last, x);
}

template <class FIter, class T>
void __uninitialized_fill(FIter first, FIter last, const T& x, __false_type)
{
  auto cur = first;
  for (; cur != last; ++cur)
  {
    mystl::construct(&*cur, x);
  }
}

template <class FIter, class T>
void  uninitialized_fill(FIter first, FIter last, const T& x)
{
  mystl::__uninitialized_fill(first, last, x, typename __type_traits<
                              typename iterator_traits<FIter>::
                              value_type>::is_POD_type());
}

/*****************************************************************************************/
// uninitialized_fill_n
// 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
/*****************************************************************************************/
template <class FIter, class Size, class T>
FIter __uninitialized_fill_n(FIter first, Size n, const T& x, __true_type)
{
  return mystl::fill_n(first, n, x);
}

template <class FIter, class Size, class T>
FIter __uninitialized_fill_n(FIter first, Size n, const T& x, __false_type)
{
  auto cur = first;
  for (; n > 0; --n, ++cur)
  {
    mystl::construct(&*cur, x);
  }
  return cur;
}

template <class FIter, class Size, class T>
FIter uninitialized_fill_n(FIter first, Size n, const T& x)
{
  return mystl::__uninitialized_fill_n(first, n, x, typename __type_traits<
                                       typename iterator_traits<FIter>::
                                       value_type>::is_POD_type());
}

/*****************************************************************************************/
// uninitialized_move
// 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
template <class IIter, class FIter>
FIter __uninitialized_move(IIter first, IIter last, FIter result, __true_type)
{
  return mystl::copy(first, last, result);
}

template <class IIter, class FIter>
FIter __uninitialized_move(IIter first, IIter last, FIter result, __false_type)
{
  FIter cur = result;
  for (; first != last; ++first, ++cur)
  {
    mystl::construct(&*cur, std::move(*first));
  }
  return cur;
}

template <class IIter, class FIter>
FIter uninitialized_move(IIter first, IIter last, FIter result)
{
  return mystl::__uninitialized_move(first, last, result,
                                     typename __type_traits<typename
                                     iterator_traits<IIter>::value_type
                                     >::is_POD_type());
}

/*****************************************************************************************/
// uninitialized_move_n
// 把[first, first + n)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
template <class FIter, class Size, class T>
FIter __uninitialized_move_n(FIter first, Size n, T&& x, __true_type)
{
  return mystl::copy_n(first, n, std::forward<T>(x));
}

template <class FIter, class Size, class T>
FIter __uninitialized_move_n(FIter first, Size n, T&& x, __false_type)
{
  auto cur = first;
  for (; n > 0; --n, ++cur)
  {
    mystl::construct(&*cur, std::forward<T>(x));
  }
  return cur;
}

template <class FIter, class Size, class T>
FIter uninitialized_move_n(FIter first, Size n, T&& x)
{
  return mystl::__uninitialized_move_n(first, n, std::forward<T>(x), 
                                       typename __type_traits<typename
                                       iterator_traits<FIter>::
                                       value_type>::is_POD_type());
}

} // namespace mystl
#endif // !MYTINYSTL_UNINITIALIZED_H_

