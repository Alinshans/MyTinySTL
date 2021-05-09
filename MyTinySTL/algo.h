#ifndef MYTINYSTL_ALGO_H_
#define MYTINYSTL_ALGO_H_

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

// 这个头文件包含了 mystl 的一系列算法

#include <cstddef>
#include <ctime>

#include "algobase.h"
#include "memory.h"
#include "heap_algo.h"
#include "functional.h"

namespace mystl
{

/*****************************************************************************************/
// all_of
// 检查[first, last)内是否全部元素都满足一元操作 unary_pred 为 true 的情况，满足则返回 true
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  for (; first != last; ++first)
  {
    if (!unary_pred(*first))
      return false;
  }
  return true;
}

/*****************************************************************************************/
// any_of
// 检查[first, last)内是否存在某个元素满足一元操作 unary_pred 为 true 的情况，满足则返回 true
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  for (; first != last; ++first)
  {
    if (unary_pred(*first))
      return true;
  }
  return false;
}

/*****************************************************************************************/
// none_of
// 检查[first, last)内是否全部元素都不满足一元操作 unary_pred 为 true 的情况，满足则返回 true
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  for (; first != last; ++first)
  {
    if (unary_pred(*first))
      return false;
  }
  return true;
}

/*****************************************************************************************/
// count
// 对[first, last)区间内的元素与给定值进行比较，缺省使用 operator==，返回元素相等的个数
/*****************************************************************************************/
template <class InputIter, class T>
size_t count(InputIter first, InputIter last, const T& value)
{
  size_t n = 0;
  for (; first != last; ++first)
  {
    if (*first == value)
      ++n;
  }
  return n;
}

/*****************************************************************************************/
// count_if
// 对[first, last)区间内的每个元素都进行一元 unary_pred 操作，返回结果为 true 的个数
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
size_t count_if(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  size_t n = 0;
  for (; first != last; ++first)
  {
    if (unary_pred(*first))
      ++n;
  }
  return n;
}

/*****************************************************************************************/
// find
// 在[first, last)区间内找到等于 value 的元素，返回指向该元素的迭代器
/*****************************************************************************************/
template <class InputIter, class T>
InputIter
find(InputIter first, InputIter last, const T& value)
{
  while (first != last && *first != value)
    ++first;
  return first;
}

/*****************************************************************************************/
// find_if
// 在[first, last)区间内找到第一个令一元操作 unary_pred 为 true 的元素并返回指向该元素的迭代器
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
InputIter
find_if(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  while (first != last && !unary_pred(*first))
    ++first;
  return first;
}

/*****************************************************************************************/
// find_if_not
// 在[first, last)区间内找到第一个令一元操作 unary_pred 为 false 的元素并返回指向该元素的迭代器
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
InputIter
find_if_not(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  while (first != last && unary_pred(*first))
    ++first;
  return first;
}

/*****************************************************************************************/
// search
// 在[first1, last1)中查找[first2, last2)的首次出现点
/*****************************************************************************************/
template <class ForwardIter1, class ForwardIter2>
ForwardIter1
search(ForwardIter1 first1, ForwardIter1 last1,
       ForwardIter2 first2, ForwardIter2 last2)
{
  auto d1 = mystl::distance(first1, last1);
  auto d2 = mystl::distance(first2, last2);
  if (d1 < d2)
    return last1;
  auto current1 = first1;
  auto current2 = first2;
  while (current2 != last2)
  {
    if (*current1 == *current2)
    {
      ++current1;
      ++current2;
    }
    else
    {
      if (d1 == d2)
      {
        return last1;
      }
      else
      {
        current1 = ++first1;
        current2 = first2;
        --d1;
      }
    }
  }
  return first1;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter1, class ForwardIter2, class Compared>
ForwardIter1
search(ForwardIter1 first1, ForwardIter1 last1,
       ForwardIter2 first2, ForwardIter2 last2, Compared comp)
{
  auto d1 = mystl::distance(first1, last1);
  auto d2 = mystl::distance(first2, last2);
  if (d1 < d2)
    return last1;
  auto current1 = first1;
  auto current2 = first2;
  while (current2 != last2)
  {
    if (comp(*current1, *current2))
    {
      ++current1;
      ++current2;
    }
    else
    {
      if (d1 == d2)
      {
        return last1;
      }
      else
      {
        current1 = ++first1;
        current2 = first2;
        --d1;
      }
    }
  }
  return first1;
}

/*****************************************************************************************/
// search_n
// 在[first, last)中查找连续 n 个 value 所形成的子序列，返回一个迭代器指向该子序列的起始处
/*****************************************************************************************/
template <class ForwardIter, class Size, class T>
ForwardIter
search_n(ForwardIter first, ForwardIter last, Size n, const T& value)
{
  if (n <= 0)
  {
    return first;
  }
  else
  {
    first = mystl::find(first, last, value);
    while (first != last)
    {
      auto m = n - 1;
      auto i = first;
      ++i;
      while (i != last && m != 0 && *i == value)
      {
        ++i;
        --m;
      }
      if (m == 0)
      {
        return first;
      }
      else
      {
        first = mystl::find(i, last, value);
      }
    }
    return last;
  }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Size, class T, class Compared>
ForwardIter
search_n(ForwardIter first, ForwardIter last,
         Size n, const T& value, Compared comp)
{
  if (n <= 0)
  {
    return first;
  }
  else
  {
    while (first != last)
    {
      if (comp(*first, value))
        break;
      ++first;
    }
    while (first != last)
    {
      auto m = n - 1;
      auto i = first;
      ++i;
      while (i != last && m != 0 && comp(*i, value))
      {
        ++i;
        --m;
      }
      if (m == 0)
      {
        return first;
      }
      else
      {
        while (i != last)
        {
          if (comp(*i, value))
            break;
          ++i;
        }
        first = i;
      }
    }
    return last;
  }
}

/*****************************************************************************************/
// find_end
// 在[first1, last1)区间中查找[first2, last2)最后一次出现的地方，若不存在返回 last1
/*****************************************************************************************/
// find_end_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter1, class ForwardIter2>
ForwardIter1
find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                  ForwardIter2 first2, ForwardIter2 last2,
                  forward_iterator_tag, forward_iterator_tag)
{
  if (first2 == last2)
  {
    return last1;
  }
  else
  {
    auto result = last1;
    while (true)
    {
      // 利用 search 查找某个子序列的首次出现点，找不到则返回 last1
      auto new_result = mystl::search(first1, last1, first2, last2);
      if (new_result == last1)
      {
        return result;
      }
      else
      {
        result = new_result;
        first1 = new_result;
        ++first1;
      }
    }
  }
}

// find_end_dispatch 的 bidirectional_iterator_tag 版本
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter1
find_end_dispatch(BidirectionalIter1 first1, BidirectionalIter1 last1,
                  BidirectionalIter2 first2, BidirectionalIter2 last2,
                  bidirectional_iterator_tag, bidirectional_iterator_tag)
{
  typedef reverse_iterator<BidirectionalIter1> reviter1;
  typedef reverse_iterator<BidirectionalIter2> reviter2;
  reviter1 rlast1(first1);
  reviter2 rlast2(first2);
  reviter1 rresult = mystl::search(reviter1(last1), rlast1, reviter2(last2), rlast2);
  if (rresult == rlast1)
  {
    return last1;
  }
  else
  {
    auto result = rresult.base();
    mystl::advance(result, -mystl::distance(first2, last2));
    return result;
  }
}

template <class ForwardIter1, class ForwardIter2>
ForwardIter1
find_end(ForwardIter1 first1, ForwardIter1 last1,
         ForwardIter2 first2, ForwardIter2 last2)
{
  typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
  typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
  return mystl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2());
}

// 重载版本使用函数对象 comp 代替比较操作
// find_end_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter1, class ForwardIter2, class Compared>
ForwardIter1
find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                  ForwardIter2 first2, ForwardIter2 last2,
                  forward_iterator_tag, forward_iterator_tag, Compared comp)
{
  if (first2 == last2)
  {
    return last1;
  }
  else
  {
    auto result = last1;
    while (true)
    {
      // 利用 search 查找某个子序列的首次出现点，找不到则返回 last1
      auto new_result = mystl::search(first1, last1, first2, last2, comp);
      if (new_result == last1)
      {
        return result;
      }
      else
      {
        result = new_result;
        first1 = new_result;
        ++first1;
      }
    }
  }
}

// find_end_dispatch 的 bidirectional_iterator_tag 版本
template <class BidirectionalIter1, class BidirectionalIter2, class Compared>
BidirectionalIter1
find_end_dispatch(BidirectionalIter1 first1, BidirectionalIter1 last1,
                  BidirectionalIter2 first2, BidirectionalIter2 last2,
                  bidirectional_iterator_tag, bidirectional_iterator_tag, Compared comp)
{
  typedef reverse_iterator<BidirectionalIter1> reviter1;
  typedef reverse_iterator<BidirectionalIter2> reviter2;
  reviter1 rlast1(first1);
  reviter2 rlast2(first2);
  reviter1 rresult = mystl::search(reviter1(last1), rlast1, reviter2(last2), rlast2, comp);
  if (rresult == rlast1)
  {
    return last1;
  }
  else
  {
    auto result = rresult.base();
    mystl::advance(result, -mystl::distance(first2, last2));
    return result;
  }
}

template <class ForwardIter1, class ForwardIter2, class Compared>
ForwardIter1
find_end(ForwardIter1 first1, ForwardIter1 last1,
         ForwardIter2 first2, ForwardIter2 last2, Compared comp)
{
  typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
  typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
  return mystl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2(), comp);
}

/*****************************************************************************************/
// find_first_of
// 在[first1, last1)中查找[first2, last2)中的某些元素，返回指向第一次出现的元素的迭代器
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
InputIter
find_first_of(InputIter first1, InputIter last1,
              ForwardIter first2, ForwardIter last2)
{
  for (; first1 != last1; ++first1)
  {
    for (auto iter = first2; iter != last2; ++iter)
    {
      if (*first1 == *iter)
        return first1;
    }
  }
  return last1;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter, class ForwardIter, class Compared>
InputIter
find_first_of(InputIter first1, InputIter last1,
              ForwardIter first2, ForwardIter last2, Compared comp)
{
  for (; first1 != last1; ++first1)
  {
    for (auto iter = first2; iter != last2; ++iter)
    {
      if (comp(*first1, *iter))
        return first1;
    }
  }
  return last1;
}

/*****************************************************************************************/
// for_each
// 使用一个函数对象 f 对[first, last)区间内的每个元素执行一个 operator() 操作，但不能改变元素内容
// f() 可返回一个值，但该值会被忽略
/*****************************************************************************************/
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function f)
{
  for (; first != last; ++first)
  {
    f(*first);
  }
  return f;
}

/*****************************************************************************************/
// adjacent_find
// 找出第一对匹配的相邻元素，缺省使用 operator== 比较，如果找到返回一个迭代器，指向这对元素的第一个元素
/*****************************************************************************************/
template <class ForwardIter>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last)
{
  if (first == last)  return last;
  auto next = first;
  while (++next != last)
  {
    if (*first == *next)  return first;
    first = next;
  }
  return last;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last, Compared comp)
{
  if (first == last)  return last;
  auto next = first;
  while (++next != last)
  {
    if (comp(*first, *next))  return first;
    first = next;
  }
  return last;
}

/*****************************************************************************************/
// lower_bound
// 在[first, last)中查找第一个不小于 value 的元素，并返回指向它的迭代器，若没有则返回 last
/*****************************************************************************************/
// lbound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T>
ForwardIter
lbound_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (*middle < value)
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else
    {
      len = half;
    }
  }
  return first;
}

// lbound_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T>
RandomIter
lbound_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (*middle < value)
    {
      first = middle + 1;
      len = len - half - 1;
    }
    else
    {
      len = half;
    }
  }
  return first;
}

template <class ForwardIter, class T>
ForwardIter
lower_bound(ForwardIter first, ForwardIter last, const T& value)
{
  return mystl::lbound_dispatch(first, last, value, iterator_category(first));
}

// 重载版本使用函数对象 comp 代替比较操作
// lbound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T, class Compared>
ForwardIter
lbound_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag, Compared comp)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (comp(*middle, value))
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else
    {
      len = half;
    }
  }
  return first;
}

// lbound_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T, class Compared>
RandomIter
lbound_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag, Compared comp)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (comp(*middle, value))
    {
      first = middle + 1;
      len = len - half - 1;
    }
    else
    {
      len = half;
    }
  }
  return first;
}

template <class ForwardIter, class T, class Compared>
ForwardIter
lower_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp)
{
  return mystl::lbound_dispatch(first, last, value, iterator_category(first), comp);
}

/*****************************************************************************************/
// upper_bound
// 在[first, last)中查找第一个大于value 的元素，并返回指向它的迭代器，若没有则返回 last
/*****************************************************************************************/
// ubound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T>
ForwardIter
ubound_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (value < *middle)
    {
      len = half;
    }
    else
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

// ubound_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T>
RandomIter
ubound_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (value < *middle)
    {
      len = half;
    }
    else
    {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

template <class ForwardIter, class T>
ForwardIter
upper_bound(ForwardIter first, ForwardIter last, const T& value)
{
  return mystl::ubound_dispatch(first, last, value, iterator_category(first));
}

// 重载版本使用函数对象 comp 代替比较操作
// ubound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T, class Compared>
ForwardIter
ubound_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag, Compared comp)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (comp(value, *middle))
    {
      len = half;
    }
    else
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

// ubound_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T, class Compared>
RandomIter
ubound_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag, Compared comp)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (comp(value, *middle))
    {
      len = half;
    }
    else
    {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

template <class ForwardIter, class T, class Compared>
ForwardIter
upper_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp)
{
  return mystl::ubound_dispatch(first, last, value, iterator_category(first), comp);
}

/*****************************************************************************************/
// binary_search
// 二分查找，如果在[first, last)内有等同于 value 的元素，返回 true，否则返回 false
/*****************************************************************************************/
template <class ForwardIter, class T>
bool binary_search(ForwardIter first, ForwardIter last, const T& value)
{
  auto i = mystl::lower_bound(first, last, value);
  return i != last && !(value < *i);
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class T, class Compared>
bool binary_search(ForwardIter first, ForwardIter last, const T& value, Compared comp)
{
  auto i = mystl::lower_bound(first, last, value);
  return i != last && !comp(value, *i);
}

/*****************************************************************************************/
// equal_range
// 查找[first,last)区间中与 value 相等的元素所形成的区间，返回一对迭代器指向区间首尾
// 第一个迭代器指向第一个不小于 value 的元素，第二个迭代器指向第一个大于 value 的元素
/*****************************************************************************************/
// erange_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T>
mystl::pair<ForwardIter, ForwardIter>
erange_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle, left, right;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (*middle < value)
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else if (value < *middle)
    {
      len = half;
    }
    else
    {
      left = mystl::lower_bound(first, last, value);
      mystl::advance(first, len);
      right = mystl::upper_bound(++middle, first, value);
      return mystl::pair<ForwardIter, ForwardIter>(left, right);
    }
  }
  return mystl::pair<ForwardIter, ForwardIter>(last, last);
}

// erange_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T>
mystl::pair<RandomIter, RandomIter>
erange_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle, left, right;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (*middle < value)
    {
      first = middle + 1;
      len = len - half - 1;
    }
    else if (value < *middle)
    {
      len = half;
    }
    else
    {
      left = mystl::lower_bound(first, middle, value);
      right = mystl::upper_bound(++middle, first + len, value);
      return mystl::pair<RandomIter, RandomIter>(left, right);
    }
  }
  return mystl::pair<RandomIter, RandomIter>(last, last);
}

template <class ForwardIter, class T>
mystl::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& value)
{
  return mystl::erange_dispatch(first, last, value, iterator_category(first));
}

// 重载版本使用函数对象 comp 代替比较操作
// erange_dispatch 的 forward iterator 版本
template <class ForwardIter, class T, class Compared>
mystl::pair<ForwardIter, ForwardIter>
erange_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag, Compared comp)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle, left, right;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (comp(*middle, value))
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else if (comp(value, *middle))
    {
      len = half;
    }
    else
    {
      left = mystl::lower_bound(first, last, value, comp);
      mystl::advance(first, len);
      right = mystl::upper_bound(++middle, first, value, comp);
      return mystl::pair<ForwardIter, ForwardIter>(left, right);
    }
  }
  return mystl::pair<ForwardIter, ForwardIter>(last, last);
}

// erange_dispatch 的 random access iterator 版本
template <class RandomIter, class T, class Compared>
mystl::pair<RandomIter, RandomIter>
erange_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag, Compared comp)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle, left, right;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (comp(*middle, value))
    {
      first = middle + 1;
      len = len - half - 1;
    }
    else if (comp(value, *middle))
    {
      len = half;
    }
    else
    {
      left = mystl::lower_bound(first, middle, value, comp);
      right = mystl::upper_bound(++middle, first + len, value, comp);
      return mystl::pair<RandomIter, RandomIter>(left, right);
    }
  }
  return mystl::pair<RandomIter, RandomIter>(last, last);
}

template <class ForwardIter, class T, class Compared>
mystl::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& value, Compared comp)
{
  return mystl::erange_dispatch(first, last, value, iterator_category(first), comp);
}

/*****************************************************************************************/
// generate
// 将函数对象 gen 的运算结果对[first, last)内的每个元素赋值
/*****************************************************************************************/
template <class ForwardIter, class Generator>
void generate(ForwardIter first, ForwardIter last, Generator gen)
{
  for (; first != last; ++first)
  {
    *first = gen();
  }
}

/*****************************************************************************************/
// generate_n
// 用函数对象 gen 连续对 n 个元素赋值
/*****************************************************************************************/
template <class ForwardIter, class Size, class Generator>
void generate_n(ForwardIter first, Size n, Generator gen)
{
  for (; n > 0; --n, ++first)
  {
    *first = gen();
  }
}

/*****************************************************************************************/
// includes
// 判断序列一S1 是否包含序列二S2
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
bool includes(InputIter1 first1, InputIter1 last1,
              InputIter2 first2, InputIter2 last2)
{
  while (first1 != last1 && first2 != last2)
  {
    if (*first2 < *first1)
    {
      return false;
    }
    else if (*first1 < *first2)
    {
      ++first1;
    }
    else
    {
      ++first1, ++first2;
    }
  }
  return first2 == last2;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter1, class InputIter2, class Compared>
bool includes(InputIter1 first1, InputIter1 last1,
              InputIter2 first2, InputIter2 last2, Compared comp)
{
  while (first1 != last1 && first2 != last2)
  {
    if (comp(*first2, *first1))
    {
      return false;
    }
    else if (comp(*first1, *first2))
    {
      ++first1;
    }
    else
    {
      ++first1, ++first2;
    }
  }
  return first2 == last2;
}

/*****************************************************************************************/
// is_heap
// 检查[first, last)内的元素是否为一个堆，如果是，则返回 true
/*****************************************************************************************/
template <class RandomIter>
bool is_heap(RandomIter first, RandomIter last)
{
  auto n = mystl::distance(first, last);
  auto parent = 0;
  for (auto child = 1; child < n; ++child)
  {
    if (first[parent] < first[child])
      return false;
    if ((child & 1) == 0)
      ++parent;
  }
  return true;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Compared>
bool is_heap(RandomIter first, RandomIter last, Compared comp)
{
  auto n = mystl::distance(first, last);
  auto parent = 0;
  for (auto child = 1; child < n; ++child)
  {
    if (comp(first[parent], first[child]))
      return false;
    if ((child & 1) == 0)
      ++parent;
  }
  return true;
}

/*****************************************************************************************/
// is_sorted
// 检查[first, last)内的元素是否升序，如果是升序，则返回 true
/*****************************************************************************************/
template <class ForwardIter>
bool is_sorted(ForwardIter first, ForwardIter last)
{
  if (first == last)
    return true;
  auto next = first;
  ++next;
  for (; next != last; first = next, ++next)
  {
    if (*next < *first)
      return false;
  }
  return true;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
bool is_sorted(ForwardIter first, ForwardIter last, Compared comp)
{
  if (first == last)
    return true;
  auto next = first;
  ++next;
  for (; next != last; first = next, ++next)
  {
    if (comp(*next, *first))
      return false;
  }
  return true;
}

/*****************************************************************************************/
// median
// 找出三个值的中间值
/*****************************************************************************************/
template <class T>
const T& median(const T& left, const T& mid, const T& right)
{
  if (left < mid)
    if (mid < right)        // left < mid < right
      return mid;
    else if (left < right)  // left < right <= mid
      return right;
    else                    // right <= left < mid
      return left;
  else if (left < right)      // mid <= left < right
    return left;
  else if (mid < right)       // mid < right <= left
    return right;
  else                        // right <= mid <= left
    return mid;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class T, class Compared>
const T& median(const T& left, const T& mid, const T& right, Compared comp)
{
  if (comp(left, mid))
    if (comp(mid, right))
      return mid;
    else if (comp(left, right))
      return right;
    else
      return left;
  else if (comp(left, right))
    return left;
  else if (comp(mid, right))
    return right;
  else
    return mid;
}

/*****************************************************************************************/
// max_element
// 返回一个迭代器，指向序列中最大的元素
/*****************************************************************************************/
template <class ForwardIter>
ForwardIter max_element(ForwardIter first, ForwardIter last)
{
  if (first == last)
    return first;
  auto result = first;
  while (++first != last)
  {
    if (*result < *first)
      result = first;
  }
  return result;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter max_element(ForwardIter first, ForwardIter last, Compared comp)
{
  if (first == last)
    return first;
  auto result = first;
  while (++first != last)
  {
    if (comp(*result, *first))
      result = first;
  }
  return result;
}

/*****************************************************************************************/
// min_element
// 返回一个迭代器，指向序列中最小的元素
/*****************************************************************************************/
template <class ForwardIter>
ForwardIter min_elememt(ForwardIter first, ForwardIter last)
{
  if (first == last)
    return first;
  auto result = first;
  while (++first != last)
  {
    if (*first < *result)
      result = first;
  }
  return result;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter min_elememt(ForwardIter first, ForwardIter last, Compared comp)
{
  if (first == last)
    return first;
  auto result = first;
  while (++first != last)
  {
    if (comp(*first, *result))
      result = first;
  }
  return result;
}

/*****************************************************************************************/
// swap_ranges
// 将[first1, last1)从 first2 开始，交换相同个数元素
// 交换的区间长度必须相同，两个序列不能互相重叠，返回一个迭代器指向序列二最后一个被交换元素的下一位置
/*****************************************************************************************/
template <class ForwardIter1, class ForwardIter2>
ForwardIter2
swap_ranges(ForwardIter1 first1, ForwardIter1 last1,
            ForwardIter2 first2)
{
  for (; first1 != last1; ++first1, ++first2)
  {
    mystl::iter_swap(first1, first2);
  }
  return first2;
}

/*****************************************************************************************/
// transform
// 第一个版本以函数对象 unary_op 作用于[first, last)中的每个元素并将结果保存至 result 中
// 第二个版本以函数对象 binary_op 作用于两个序列[first1, last1)、[first2, last2)的相同位置
/*****************************************************************************************/
template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter
transform(InputIter first, InputIter last,
          OutputIter result, UnaryOperation unary_op)
{
  for (; first != last; ++first, ++result)
  {
    *result = unary_op(*first);
  }
  return result;
}

template <class InputIter1, class InputIter2, class OutputIter, class BinaryOperation>
OutputIter
transform(InputIter1 first1, InputIter1 last1,
          InputIter2 first2, OutputIter result, BinaryOperation binary_op)
{
  for (; first1 != last1; ++first1, ++first2, ++result)
  {
    *result = binary_op(*first1, *first2);
  }
  return result;
}

/*****************************************************************************************/
// remove_copy
// 移除区间内与指定 value 相等的元素，并将结果复制到以 result 标示起始位置的容器上
/*****************************************************************************************/
template <class InputIter, class OutputIter, class T>
OutputIter
remove_copy(InputIter first, InputIter last, OutputIter result, const T& value)
{
  for (; first != last; ++first)
  {
    if (*first != value)
    {
      *result++ = *first;
    }
  }
  return result;
}

/*****************************************************************************************/
// remove
// 移除所有与指定 value 相等的元素
// 并不从容器中删除这些元素，所以 remove 和 remove_if 不适用于 array
/*****************************************************************************************/
template <class ForwardIter, class T>
ForwardIter remove(ForwardIter first, ForwardIter last, const T& value)
{
  first = mystl::find(first, last, value);  // 利用 find 找出第一个匹配的地方
  auto next = first;
  return first == last ? first : mystl::remove_copy(++next, last, first, value);
}

/*****************************************************************************************/
// remove_copy_if
// 移除区间内所有令一元操作 unary_pred 为 true 的元素，并将结果复制到以 result 为起始位置的容器上
/*****************************************************************************************/
template <class InputIter, class OutputIter, class UnaryPredicate>
OutputIter
remove_copy_if(InputIter first, InputIter last,
               OutputIter result, UnaryPredicate unary_pred)
{
  for (; first != last; ++first)
  {
    if (!unary_pred(*first))
    {
      *result = *first;
      ++result;
    }
  }
  return result;
}

/*****************************************************************************************/
// remove_if
// 移除区间内所有令一元操作 unary_pred 为 true 的元素
/*****************************************************************************************/
template <class ForwardIter, class UnaryPredicate>
ForwardIter
remove_if(ForwardIter first, ForwardIter last, UnaryPredicate unary_pred)
{
  first = mystl::find_if(first, last, unary_pred);  // 利用 find_if 找出第一个匹配的地方
  auto next = first;
  return first == last ? first : mystl::remove_copy_if(++next, last, first, unary_pred);
}

/*****************************************************************************************/
// replace
// 将区间内所有的 old_value 都以 new_value 替代
/*****************************************************************************************/
template <class ForwardIter, class T>
void replace(ForwardIter first, ForwardIter last,
             const T& old_value, const T& new_value)
{
  for (; first != last; ++first)
  {
    if (*first == old_value)
      *first = new_value;
  }
}

/*****************************************************************************************/
// replace_copy
// 行为与 replace 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
/*****************************************************************************************/
template <class InputIter, class OutputIter, class T>
OutputIter
replace_copy(InputIter first, InputIter last,
             OutputIter result, const T& old_value, const T& new_value)
{
  for (; first != last; ++first, ++result)
  {
    *result = *first == old_value ? new_value : *first;
  }
  return result;
}

/*****************************************************************************************/
// replace_copy_if
// 行为与 replace_if 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
/*****************************************************************************************/
template <class InputIter, class OutputIter, class UnaryPredicate, class T>
OutputIter
replace_copy_if(InputIter first, InputIter last,
                OutputIter result, UnaryPredicate unary_pred, const T& new_value)
{
  for (; first != last; ++first, ++result)
  {
    *result = unary_pred(*first) ? new_value : *first;
  }
  return result;
}

/*****************************************************************************************/
// replace_if
// 将区间内所有令一元操作 unary_pred 为 true 的元素都用 new_value 替代
/*****************************************************************************************/
template <class ForwardIter, class UnaryPredicate, class T>
void replace_if(ForwardIter first, ForwardIter last,
                UnaryPredicate unary_pred, const T& new_value)
{
  for (; first != last; ++first)
  {
    if (unary_pred(*first))
      *first = new_value;
  }
}

/*****************************************************************************************/
// reverse
// 将[first, last)区间内的元素反转
/*****************************************************************************************/
// reverse_dispatch 的 bidirectional_iterator_tag 版本
template <class BidirectionalIter>
void reverse_dispatch(BidirectionalIter first, BidirectionalIter last,
                      bidirectional_iterator_tag)
{
  while (true)
  {
    if (first == last || first == --last)
      return;
    mystl::iter_swap(first++, last);
  }
}

// reverse_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter>
void reverse_dispatch(RandomIter first, RandomIter last,
                      random_access_iterator_tag)
{
  while (first < last)
    mystl::iter_swap(first++, --last);
}

template <class BidirectionalIter>
void reverse(BidirectionalIter first, BidirectionalIter last)
{
  mystl::reverse_dispatch(first, last, iterator_category(first));
}

/*****************************************************************************************/
// reverse_copy
// 行为与 reverse 类似，不同的是将结果复制到 result 所指容器中
/*****************************************************************************************/
template <class BidirectionalIter, class OutputIter>
OutputIter
reverse_copy(BidirectionalIter first, BidirectionalIter last,
             OutputIter result)
{
  while (first != last)
  {
    --last;
    *result = *last;
    ++result;
  }
  return result;
}

/*****************************************************************************************/
// random_shuffle
// 将[first, last)内的元素次序随机重排
// 重载版本使用一个产生随机数的函数对象 rand
/*****************************************************************************************/
template <class RandomIter>
void random_shuffle(RandomIter first, RandomIter last)
{
  if (first == last)
    return;
  srand((unsigned)time(0));
  for (auto i = first + 1; i != last; ++i)
  {
    mystl::iter_swap(i, first + (rand() % (i - first + 1)));
  }
}

// 重载版本使用一个产生随机数的函数对象 rand
template <class RandomIter, class RandomNumberGenerator>
void random_shuffle(RandomIter first, RandomIter last,
                    RandomNumberGenerator& rand)
{
  if (first == last)
    return;
  auto len = mystl::distance(first, last);
  for (auto i = first + 1; i != last; ++i)
  {
    mystl::iter_swap(i, first + (rand(i - first + 1) % len));
  }
}

/*****************************************************************************************/
// rotate
// 将[first, middle)内的元素和 [middle, last)内的元素互换，可以交换两个长度不同的区间
// 返回交换后 middle 的位置
/*****************************************************************************************/
// rotate_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter>
ForwardIter
rotate_dispatch(ForwardIter first, ForwardIter middle,
                ForwardIter last, forward_iterator_tag)
{
  auto first2 = middle;
  do
  {
    mystl::swap(*first++, *first2++);
    if (first == middle)
      middle = first2;
  } while (first2 != last);  // 后半段移到前面

  auto new_middle = first;   // 迭代器返回的位置
  first2 = middle;
  while (first2 != last)
  {   // 调整剩余元素
    mystl::swap(*first++, *first2++);
    if (first == middle)
    {
      middle = first2;
    }
    else if (first2 == last)
    {
      first2 = middle;
    }
  }
  return new_middle;
}

// rotate_dispatch 的 bidirectional_iterator_tag 版本
template <class BidirectionalIter>
BidirectionalIter
rotate_dispatch(BidirectionalIter first, BidirectionalIter middle,
                BidirectionalIter last, bidirectional_iterator_tag)
{
  mystl::reverse_dispatch(first, middle, bidirectional_iterator_tag());
  mystl::reverse_dispatch(middle, last, bidirectional_iterator_tag());
  while (first != middle && middle != last)
    mystl::swap(*first++, *--last);
  if (first == middle)
  {
    mystl::reverse_dispatch(middle, last, bidirectional_iterator_tag());
    return last;
  }
  else
  {
    mystl::reverse_dispatch(first, middle, bidirectional_iterator_tag());
    return first;
  }
}

// 求最大公因子
template <class EuclideanRingElement>
EuclideanRingElement rgcd(EuclideanRingElement m, EuclideanRingElement n)
{
  while (n != 0)
  {
    auto t = m % n;
    m = n;
    n = t;
  }
  return m;
}

// rotate_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter>
RandomIter
rotate_dispatch(RandomIter first, RandomIter middle,
                RandomIter last, random_access_iterator_tag)
{
  // 因为是 random access iterator，我们可以确定每个元素的位置
  auto n = last - first;
  auto l = middle - first;
  auto r = n - l;
  auto result = first + (last - middle);
  if (l == r)
  {
    mystl::swap_ranges(first, middle, middle);
    return result;
  }
  auto cycle_times = rgcd(n, l);
  for (auto i = 0; i < cycle_times; ++i)
  {
    auto tmp = *first;
    auto p = first;
    if (l < r)
    {
      for (auto j = 0; j < r / cycle_times; ++j)
      {
        if (p > first + r)
        {
          *p = *(p - r);
          p -= r;
        }
        *p = *(p + l);
        p += l;
      }
    }
    else
    {
      for (auto j = 0; j < l / cycle_times - 1; ++j)
      {
        if (p < last - l)
        {
          *p = *(p + l);
          p += l;
        }
        *p = *(p - r);
        p -= r;
      }
    }
    *p = tmp;
    ++first;
  }
  return result;
}

template <class ForwardIter>
ForwardIter
rotate(ForwardIter first, ForwardIter middle, ForwardIter last)
{
  if (first == middle)
    return last;
  if (middle == last)
    return first;
  return mystl::rotate_dispatch(first, middle, last, iterator_category(first));
}

/*****************************************************************************************/
// rotate_copy
// 行为与 rotate 类似，不同的是将结果复制到 result 所指的容器中
/*****************************************************************************************/
template <class ForwardIter, class OutputIter>
ForwardIter
rotate_copy(ForwardIter first, ForwardIter middle,
            ForwardIter last, OutputIter result)
{
  return mystl::copy(first, middle, mystl::copy(middle, last, result));
}

/*****************************************************************************************/
// is_permutation
// 判断[first1,last1)是否为[first2, last2)的排列组合
/*****************************************************************************************/
template <class ForwardIter1, class ForwardIter2, class BinaryPred>
bool is_permutation_aux(ForwardIter1 first1, ForwardIter1 last1,
                        ForwardIter2 first2, ForwardIter2 last2,
                        BinaryPred pred)
{
  constexpr bool is_ra_it = mystl::is_random_access_iterator<ForwardIter1>::value
    && mystl::is_random_access_iterator<ForwardIter2>::value;
  if (is_ra_it)
  {
    auto len1 = last1 - first1;
    auto len2 = last2 - first2;
    if (len1 != len2)
      return false;
  }

  // 先找出相同的前缀段
  for (; first1 != last1 && first2 != last2; ++first1, (void) ++first2)
  {
    if (!pred(*first1, *first2))
      break;
  }
  if (is_ra_it)
  {
    if (first1 == last1)
      return true;
  }
  else
  {
    auto len1 = mystl::distance(first1, last1);
    auto len2 = mystl::distance(first2, last2);
    if (len1 == 0 && len2 == 0)
      return true;
    if (len1 != len2)
      return false;
  }

  // 判断剩余部分
  for (auto i = first1; i != last1; ++i)
  {
    bool is_repeated = false;
    for (auto j = first1; j != i; ++j)
    {
      if (pred(*j, *i))
      {
        is_repeated = true;
        break;
      }
    }

    if (!is_repeated)
    {
      // 计算 *i 在 [first2, last2) 的数目
      auto c2 = 0;
      for (auto j = first2; j != last2; ++j)
      {
        if (pred(*i, *j))
          ++c2;
      }
      if (c2 == 0)
        return false;

      // 计算 *i 在 [first1, last1) 的数目
      auto c1 = 1;
      auto j = i;
      for (++j; j != last1; ++j)
      {
        if (pred(*i, *j))
          ++c1;
      }
      if (c1 != c2)
        return false;
    }
  }
  return true;
}

template <class ForwardIter1, class ForwardIter2, class BinaryPred>
bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2,
                    BinaryPred pred)
{
  return is_permutation_aux(first1, last1, first2, last2, pred);
}

template <class ForwardIter1, class ForwardIter2>
bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2)
{
  typedef typename iterator_traits<ForwardIter1>::value_type v1;
  typedef typename iterator_traits<ForwardIter2>::value_type v2;
  static_assert(std::is_same<v1, v2>::value,
                "the type should be same in mystl::is_permutation");
  return is_permutation_aux(first1, last1, first2, last2,
                            mystl::equal_to<v1>());
}

/*****************************************************************************************/
// next_permutation
// 取得[first, last)所标示序列的下一个排列组合，如果没有下一个排序组合，返回 false，否则返回 true
/*****************************************************************************************/
template <class BidirectionalIter>
bool next_permutation(BidirectionalIter first, BidirectionalIter last)
{
  auto i = last;
  if (first == last || first == --i)
    return false;
  for (;;)
  {
    auto ii = i;
    if (*--i < *ii)
    {                 // 找到第一对小于关系的元素
      auto j = last;
      while (!(*i < *--j)) {}
      mystl::iter_swap(i, j);       // 交换 i，j 所指元素
      mystl::reverse(ii, last);     // 将 ii 之后的所有元素反转
      return true;
    }
    if (i == first)
    {
      mystl::reverse(first, last);
      return false;
    }
  }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class BidirectionalIter, class Compared>
bool next_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp)
{
  auto i = last;
  if (first == last || first == --i)
    return false;
  for (;;)
  {
    auto ii = i;
    if (comp(*--i, *ii))
    {
      auto j = last;
      while (!comp(*i, *--j)) {}
      mystl::iter_swap(i, j);       // 交换 i，j 所指元素
      mystl::reverse(ii, last);     // 将 ii 之后的所有元素反转
      return true;
    }
    if (i == first)
    {
      mystl::reverse(first, last);
      return false;
    }
  }
}

/*****************************************************************************************/
// prev_permutation
// 取得[first, last)所标示序列的上一个排列组合，如果没有上一个排序组合，返回 false，否则返回 true
/*****************************************************************************************/
template <class BidirectionalIter>
bool prev_permutation(BidirectionalIter first, BidirectionalIter last)
{
  auto i = last;
  if (first == last || first == --i)
    return false;
  for (;;)
  {
    auto ii = i;
    if (*ii < *--i)
    {                 // 找到第一对大于关系的元素
      auto j = last;
      while (!(*--j < *i)) {}
      mystl::iter_swap(i, j);       // 交换i，j
      mystl::reverse(ii, last);     // 将 ii 之后的所有元素反转
      return true;
    }
    if (i == first)
    {
      mystl::reverse(first, last);
      return false;
    }
  }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class BidirectionalIter, class Compared>
bool prev_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp)
{
  auto i = last;
  if (first == last || first == --i)
    return false;
  for (;;)
  {
    auto ii = i;
    if (comp(*ii, *--i))
    {
      auto j = last;
      while (!comp(*--j, *i)) {}
      mystl::iter_swap(i, j);       // 交换i，j
      mystl::reverse(ii, last);     // 将 ii 之后的所有元素反转
      return true;
    }
    if (i == first)
    {
      mystl::reverse(first, last);
      return false;
    }
  }
}

/*****************************************************************************************/
// merge
// 将两个经过排序的集合 S1 和 S2 合并起来置于另一段空间，返回一个迭代器指向最后一个元素的下一位置
/*****************************************************************************************/
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter
merge(InputIter1 first1, InputIter1 last1,
      InputIter2 first2, InputIter2 last2,
      OutputIter result)
{
  while (first1 != last1 && first2 != last2)
  {
    if (*first2 < *first1)
    {
      *result = *first2;
      ++first2;
    }
    else
    {
      *result = *first1;
      ++first1;
    }
    ++result;
  }
  return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter1, class InputIter2, class OutputIter, class Compared>
OutputIter
merge(InputIter1 first1, InputIter1 last1,
      InputIter2 first2, InputIter2 last2,
      OutputIter result, Compared comp)
{
  while (first1 != last1 && first2 != last2)
  {
    if (comp(*first2, *first1))
    {
      *result = *first2;
      ++first2;
    }
    else
    {
      *result = *first1;
      ++first1;
    }
    ++result;
  }
  return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
}

/*****************************************************************************************/
// inplace_merge
// 把连接在一起的两个有序序列结合成单一序列并保持有序
/*****************************************************************************************/
// 没有缓冲区的情况下合并
template <class BidirectionalIter, class Distance>
void merge_without_buffer(BidirectionalIter first, BidirectionalIter middle,
                          BidirectionalIter last, Distance len1, Distance len2)
{
  if (len1 == 0 || len2 == 0)
    return;
  if (len1 + len2 == 2)
  {
    if (*middle < *first)
      mystl::iter_swap(first, middle);
    return;
  }
  auto first_cut = first;
  auto second_cut = middle;
  Distance len11 = 0;
  Distance len22 = 0;
  if (len1 > len2)
  {  // 序列一较长，找到序列一的中点
    len11 = len1 >> 1;
    mystl::advance(first_cut, len11);
    second_cut = mystl::lower_bound(middle, last, *first_cut);
    len22 = mystl::distance(middle, second_cut);
  }
  else
  {              // 序列二较长，找到序列二的中点
    len22 = len2 >> 1;
    mystl::advance(second_cut, len22);
    first_cut = mystl::upper_bound(first, middle, *second_cut);
    len11 = mystl::distance(first, first_cut);
  }
  auto new_middle = mystl::rotate(first_cut, middle, second_cut);
  mystl::merge_without_buffer(first, first_cut, new_middle, len11, len22);
  mystl::merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22);
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter1
merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
               BidirectionalIter2 first2, BidirectionalIter2 last2,
               BidirectionalIter1 result)
{
  if (first1 == last1)
    return mystl::copy_backward(first2, last2, result);
  if (first2 == last2)
    return mystl::copy_backward(first1, last1, result);
  --last1;
  --last2;
  while (true)
  {
    if (*last2 < *last1)
    {
      *--result = *last1;
      if (first1 == last1)
        return mystl::copy_backward(first2, ++last2, result);
      --last1;
    }
    else
    {
      *--result = *last2;
      if (first2 == last2)
        return mystl::copy_backward(first1, ++last1, result);
      --last2;
    }
  }
}

template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
BidirectionalIter1
rotate_adaptive(BidirectionalIter1 first, BidirectionalIter1 middle,
                BidirectionalIter1 last, Distance len1, Distance len2,
                BidirectionalIter2 buffer, Distance buffer_size)
{
  BidirectionalIter2 buffer_end;
  if (len1 > len2 && len2 <= buffer_size)
  {
    buffer_end = mystl::copy(middle, last, buffer);
    mystl::copy_backward(first, middle, last);
    return mystl::copy(buffer, buffer_end, first);
  }
  else if (len1 <= buffer_size)
  {
    buffer_end = mystl::copy(first, middle, buffer);
    mystl::copy(middle, last, first);
    return mystl::copy_backward(buffer, buffer_end, last);
  }
  else
  {
    return mystl::rotate(first, middle, last);
  }
}

// 有缓冲区的情况下合并
template <class BidirectionalIter, class Distance, class Pointer>
void merge_adaptive(BidirectionalIter first, BidirectionalIter middle,
                    BidirectionalIter last, Distance len1, Distance len2,
                    Pointer buffer, Distance buffer_size)
{
  // 区间长度足够放进缓冲区
  if (len1 <= len2 && len1 <= buffer_size)
  {
    Pointer buffer_end = mystl::copy(first, middle, buffer);
    mystl::merge(buffer, buffer_end, middle, last, first);
  }
  else if (len2 <= buffer_size)
  {
    Pointer buffer_end = mystl::copy(middle, last, buffer);
    mystl::merge_backward(first, middle, buffer, buffer_end, last);
  }
  else
  {  // 区间长度太长，分割递归处理
    auto first_cut = first;
    auto second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2)
    {
      len11 = len1 >> 1;
      mystl::advance(first_cut, len11);
      second_cut = mystl::lower_bound(middle, last, *first_cut);
      len22 = mystl::distance(middle, second_cut);
    }
    else
    {
      len22 = len2 >> 1;
      mystl::advance(second_cut, len22);
      first_cut = mystl::upper_bound(first, middle, *second_cut);
      len11 = mystl::distance(first, first_cut);
    }
    auto new_middle = mystl::rotate_adaptive(first_cut, middle, second_cut,
                                             len1 - len11, len22, buffer, buffer_size);
    mystl::merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
    mystl::merge_adaptive(new_middle, second_cut, last, len1 - len11,
                          len2 - len22, buffer, buffer_size);
  }
}

template <class BidirectionalIter, class T>
void
inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle,
                  BidirectionalIter last, T*)
{
  auto len1 = mystl::distance(first, middle);
  auto len2 = mystl::distance(middle, last);
  temporary_buffer<BidirectionalIter, T> buf(first, last);
  if (!buf.begin())
  {
    mystl::merge_without_buffer(first, middle, last, len1, len2);
  }
  else
  {
    mystl::merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size());
  }
}

template <class BidirectionalIter>
void
inplace_merge(BidirectionalIter first, BidirectionalIter middle,
              BidirectionalIter last)
{
  if (first == middle || middle == last)
    return;
  mystl::inplace_merge_aux(first, middle, last, value_type(first));
}

// 重载版本使用函数对象 comp 代替比较操作
// 没有缓冲区的情况下合并
template <class BidirectionalIter, class Distance, class Compared>
void merge_without_buffer(BidirectionalIter first, BidirectionalIter middle,
                          BidirectionalIter last, Distance len1, Distance len2,
                          Compared comp)
{
  if (len1 == 0 || len2 == 0)
    return;
  if (len1 + len2 == 2)
  {
    if (comp(*middle, *first))
      mystl::iter_swap(first, middle);
    return;
  }
  auto first_cut = first;
  auto second_cut = middle;
  Distance len11 = 0;
  Distance len22 = 0;
  if (len1 > len2)
  {
    len11 = len1 >> 1;
    mystl::advance(first_cut, len11);
    second_cut = mystl::lower_bound(middle, last, *first_cut, comp);
    len22 = mystl::distance(middle, second_cut);
  }
  else
  {
    len22 = len2 >> 1;
    mystl::advance(second_cut, len22);
    first_cut = mystl::upper_bound(first, middle, *second_cut, comp);
    len11 = mystl::distance(first, first_cut);
  }
  auto new_middle = mystl::rotate(first_cut, middle, second_cut);
  mystl::merge_without_buffer(first, first_cut, new_middle, len11, len22, comp);
  mystl::merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22, comp);
}

template <class BidirectionalIter1, class BidirectionalIter2, class Compared>
BidirectionalIter1
merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
               BidirectionalIter2 first2, BidirectionalIter2 last2,
               BidirectionalIter1 result, Compared comp)
{
  if (first1 == last1)
    return mystl::copy_backward(first2, last2, result);
  if (first2 == last2)
    return mystl::copy_backward(first1, last1, result);
  --last1;
  --last2;
  while (true)
  {
    if (comp(*last2, *last1))
    {
      *--result = *last1;
      if (first1 == last1)
        return mystl::copy_backward(first2, ++last2, result);
      --last1;
    }
    else
    {
      *--result = *last2;
      if (first2 == last2)
        return mystl::copy_backward(first1, ++last1, result);
      --last2;
    }
  }
}

// 有缓冲区的情况下合并
template <class BidirectionalIter, class Distance, class Pointer, class Compared>
void merge_adaptive(BidirectionalIter first, BidirectionalIter middle,
                    BidirectionalIter last, Distance len1, Distance len2,
                    Pointer buffer, Distance buffer_size, Compared comp)
{
  // 区间长度足够放进缓冲区
  if (len1 <= len2 && len1 <= buffer_size)
  {
    Pointer buffer_end = mystl::copy(first, middle, buffer);
    mystl::merge(buffer, buffer_end, middle, last, first, comp);
  }
  else if (len2 <= buffer_size)
  {
    Pointer buffer_end = mystl::copy(middle, last, buffer);
    mystl::merge_backward(first, middle, buffer, buffer_end, last, comp);
  }
  else
  {  // 区间长度太长，分割递归处理
    auto first_cut = first;
    auto second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2)
    {
      len11 = len1 >> 1;
      mystl::advance(first_cut, len11);
      second_cut = mystl::lower_bound(middle, last, *first_cut, comp);
      len22 = mystl::distance(middle, second_cut);
    }
    else
    {
      len22 = len2 >> 1;
      mystl::advance(second_cut, len22);
      first_cut = mystl::upper_bound(first, middle, *second_cut, comp);
      len11 = mystl::distance(first, first_cut);
    }
    auto new_middle = mystl::rotate_adaptive(first_cut, middle, second_cut, len1 - len11,
                                             len22, buffer, buffer_size);
    mystl::merge_adaptive(first, first_cut, new_middle, len11,
                          len22, buffer, buffer_size, comp);
    mystl::merge_adaptive(new_middle, second_cut, last, len1 - len11,
                          len2 - len22, buffer, buffer_size, comp);
  }
}

template <class BidirectionalIter, class T, class Compared>
void
inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle,
                  BidirectionalIter last, T*, Compared comp)
{
  auto len1 = mystl::distance(first, middle);
  auto len2 = mystl::distance(middle, last);
  temporary_buffer<BidirectionalIter, T> buf(first, last);
  if (!buf.begin())
  {
    mystl::merge_without_buffer(first, middle, last, len1, len2, comp);
  }
  else
  {
    mystl::merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size(), comp);
  }
}

template <class BidirectionalIter, class Compared>
void
inplace_merge(BidirectionalIter first, BidirectionalIter middle,
              BidirectionalIter last, Compared comp)
{
  if (first == middle || middle == last)
    return;
  mystl::inplace_merge_aux(first, middle, last, value_type(first), comp);
}

/*****************************************************************************************/
// partial_sort
// 对整个序列做部分排序，保证较小的 N 个元素以递增顺序置于[first, first + N)中
/*****************************************************************************************/
template <class RandomIter>
void partial_sort(RandomIter first, RandomIter middle,
                  RandomIter last)
{
  mystl::make_heap(first, middle);
  for (auto i = middle; i < last; ++i)
  {
    if (*i < *first)
    {
      mystl::pop_heap_aux(first, middle, i, *i, distance_type(first));
    }
  }
  mystl::sort_heap(first, middle);
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Compared>
void partial_sort(RandomIter first, RandomIter middle,
                  RandomIter last, Compared comp)
{
  mystl::make_heap(first, middle, comp);
  for (auto i = middle; i < last; ++i)
  {
    if (comp(*i, *first))
    {
      mystl::pop_heap_aux(first, middle, i, *i, distance_type(first), comp);
    }
  }
  mystl::sort_heap(first, middle, comp);
}

/*****************************************************************************************/
// partial_sort_copy
// 行为与 partial_sort 类似，不同的是把排序结果复制到 result 容器中
/*****************************************************************************************/
template <class InputIter, class RandomIter, class Distance>
RandomIter
psort_copy_aux(InputIter first, InputIter last,
               RandomIter result_first, RandomIter result_last,
               Distance*)
{
  if (result_first == result_last)
    return result_last;
  auto result_iter = result_first;
  while (first != last && result_iter != result_last)
  {
    *result_iter = *first;
    ++result_iter;
    ++first;
  }
  mystl::make_heap(result_first, result_iter);
  while (first != last)
  {
    if (*first < *result_first)
    {
      mystl::adjust_heap(result_first, static_cast<Distance>(0),
                           result_iter - result_first, *first);
    }
    ++first;
  }
  mystl::sort_heap(result_first, result_iter);
  return result_iter;
}

template <class InputIter, class RandomIter>
RandomIter
partial_sort_copy(InputIter first, InputIter last,
                  RandomIter result_first, RandomIter result_last)
{
  return mystl::psort_copy_aux(first, last, result_first, result_last,
                               distance_type(result_first));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter, class RandomIter, class Distance, class Compared>
RandomIter
psort_copy_aux(InputIter first, InputIter last,
               RandomIter result_first, RandomIter result_last,
               Distance*, Compared comp)
{
  if (result_first == result_last)
    return result_last;
  auto result_iter = result_first;
  while (first != last && result_iter != result_last)
  {
    *result_iter = *first;
    ++result_iter;
    ++first;
  }
  mystl::make_heap(result_first, result_iter, comp);
  while (first != last)
  {
    if (comp(*first, *result_first))
    {
      mystl::adjust_heap(result_first, static_cast<Distance>(0),
                           result_iter - result_first, *first, comp);
    }
    ++first;
  }
  mystl::sort_heap(result_first, result_iter, comp);
  return result_iter;
}

template <class InputIter, class RandomIter, class Compared>
RandomIter
partial_sort_copy(InputIter first, InputIter last,
                  RandomIter result_first, RandomIter result_last,
                  Compared comp)
{
  return mystl::psort_copy_aux(first, last, result_first, result_last,
                               distance_type(result_first), comp);
}
/*****************************************************************************************/
// partition
// 对区间内的元素重排，被一元条件运算判定为 true 的元素会放到区间的前段
// 该函数不保证元素的原始相对位置
/*****************************************************************************************/
template <class BidirectionalIter, class UnaryPredicate>
BidirectionalIter
partition(BidirectionalIter first, BidirectionalIter last,
          UnaryPredicate unary_pred)
{
  while (true)
  {
    while (first != last && unary_pred(*first))
    {
      ++first;
    }
    if (first == last)
      break;
    --last;
    while (first != last && !unary_pred(*last))
    {
      --last;
    }
    if (first == last)
      break;
    mystl::iter_swap(first, last);
    ++first;
  }
  return first;
}

/*****************************************************************************************/
// partition_copy
// 行为与 partition 类似，不同的是，将被一元操作符判定为 true 的放到 result_true 的输出区间
// 其余放到 result_false 的输出区间，并返回一个 mystl::pair 指向这两个区间的尾部
/*****************************************************************************************/
template <class InputIter, class OutputIter1, class OutputIter2, class UnaryPredicate>
mystl::pair<OutputIter1, OutputIter2>
partition_copy(InputIter first, InputIter last,
               OutputIter1 result_true, OutputIter2 result_false,
               UnaryPredicate unary_pred)
{
  for (; first != last; ++first)
  {
    if (unary_pred(*first))
    {
      *result_true++ = *first;
    }
    else
    {
      *result_false++ = *first;
    }
  }
  return mystl::pair<OutputIter1, OutputIter2>(result_true, result_false);
}

/*****************************************************************************************/
// sort
// 将[first, last)内的元素以递增的方式排序
/*****************************************************************************************/
constexpr static size_t kSmallSectionSize = 128;  // 小型区间的大小，在这个大小内采用插入排序

                                                  // 用于控制分割恶化的情况
template <class Size>
Size slg2(Size n)
{ // 找出 lgk <= n 的 k 的最大值
  Size k = 0;
  for (; n > 1; n >>= 1)
    ++k;
  return k;
}

// 分割函数 unchecked_partition
template <class RandomIter, class T>
RandomIter
unchecked_partition(RandomIter first, RandomIter last, const T& pivot)
{
  while (true)
  {
    while (*first < pivot)
      ++first;
    --last;
    while (pivot < *last)
      --last;
    if (!(first < last))
      return first;
    mystl::iter_swap(first, last);
    ++first;
  }
}

// 内省式排序，先进行 quick sort，当分割行为有恶化倾向时，改用 heap sort
template <class RandomIter, class Size>
void intro_sort(RandomIter first, RandomIter last, Size depth_limit)
{
  while (static_cast<size_t>(last - first) > kSmallSectionSize)
  {
    if (depth_limit == 0)
    {                      // 到达最大分割深度限制
      mystl::partial_sort(first, last, last);  // 改用 heap_sort
      return;
    }
    --depth_limit;
    auto mid = mystl::median(*(first), *(first + (last - first) / 2), *(last - 1));
    auto cut = mystl::unchecked_partition(first, last, mid);
    mystl::intro_sort(cut, last, depth_limit);
    last = cut;
  }
}

// 插入排序辅助函数 unchecked_linear_insert
template <class RandomIter, class T>
void unchecked_linear_insert(RandomIter last, const T& value)
{
  auto next = last;
  --next;
  while (value < *next)
  {
    *last = *next;
    last = next;
    --next;
  }
  *last = value;
}

// 插入排序函数 unchecked_insertion_sort
template <class RandomIter>
void unchecked_insertion_sort(RandomIter first, RandomIter last)
{
  for (auto i = first; i != last; ++i)
  {
    mystl::unchecked_linear_insert(i, *i);
  }
}

// 插入排序函数 insertion_sort
template <class RandomIter>
void insertion_sort(RandomIter first, RandomIter last)
{
  if (first == last)
    return;
  for (auto i = first + 1; i != last; ++i)
  {
    auto value = *i;
    if (value < *first)
    {
      mystl::copy_backward(first, i, i + 1);
      *first = value;
    }
    else
    {
      mystl::unchecked_linear_insert(i, value);
    }
  }
}

// 最终插入排序函数 final_insertion_sort
template <class RandomIter>
void final_insertion_sort(RandomIter first, RandomIter last)
{
  if (static_cast<size_t>(last - first) > kSmallSectionSize)
  {
    mystl::insertion_sort(first, first + kSmallSectionSize);
    mystl::unchecked_insertion_sort(first + kSmallSectionSize, last);
  }
  else
  {
    mystl::insertion_sort(first, last);
  }
}

template <class RandomIter>
void sort(RandomIter first, RandomIter last)
{
  if (first != last)
  {
    // 内省式排序，将区间分为一个个小区间，然后对整体进行插入排序
    mystl::intro_sort(first, last, slg2(last - first) * 2);
    mystl::final_insertion_sort(first, last);
  }
}

// 重载版本使用函数对象 comp 代替比较操作
// 分割函数 unchecked_partition
template <class RandomIter, class T, class Compared>
RandomIter
unchecked_partition(RandomIter first, RandomIter last,
                    const T& pivot, Compared comp)
{
  while (true)
  {
    while (comp(*first, pivot))
      ++first;
    --last;
    while (comp(pivot, *last))
      --last;
    if (!(first < last))
      return first;
    mystl::iter_swap(first, last);
    ++first;
  }
}

// 内省式排序，先进行 quick sort，当分割行为有恶化倾向时，改用 heap sort
template <class RandomIter, class Size, class Compared>
void intro_sort(RandomIter first, RandomIter last,
                Size depth_limit, Compared comp)
{
  while (static_cast<size_t>(last - first) > kSmallSectionSize)
  {
    if (depth_limit == 0)
    {                            // 到达最大分割深度限制
      mystl::partial_sort(first, last, last, comp);  // 改用 heap_sort
      return;
    }
    --depth_limit;
    auto mid = mystl::median(*(first), *(first + (last - first) / 2), *(last - 1));
    auto cut = mystl::unchecked_partition(first, last, mid, comp);
    mystl::intro_sort(cut, last, depth_limit, comp);
    last = cut;
  }
}

// 插入排序辅助函数 unchecked_linear_insert
template <class RandomIter, class T, class Compared>
void unchecked_linear_insert(RandomIter last, const T& value, Compared comp)
{
  auto next = last;
  --next;
  while (comp(value, *next))
  {  // 从尾部开始寻找第一个可插入位置
    *last = *next;
    last = next;
    --next;
  }
  *last = value;
}

// 插入排序函数 unchecked_insertion_sort
template <class RandomIter, class Compared>
void unchecked_insertion_sort(RandomIter first, RandomIter last,
                              Compared comp)
{
  for (auto i = first; i != last; ++i)
  {
    mystl::unchecked_linear_insert(i, *i, comp);
  }
}

// 插入排序函数 insertion_sort
template <class RandomIter, class Compared>
void insertion_sort(RandomIter first, RandomIter last, Compared comp)
{
  if (first == last)
    return;
  for (auto i = first + 1; i != last; ++i)
  {
    auto value = *i;
    if (comp(value, *first))
    {
      mystl::copy_backward(first, i, i + 1);
      *first = value;
    }
    else
    {
      mystl::unchecked_linear_insert(i, value, comp);
    }
  }
}

// 最终插入排序函数 final_insertion_sort
template <class RandomIter, class Compared>
void final_insertion_sort(RandomIter first, RandomIter last, Compared comp)
{
  if (static_cast<size_t>(last - first) > kSmallSectionSize)
  {
    mystl::insertion_sort(first, first + kSmallSectionSize, comp);
    mystl::unchecked_insertion_sort(first + kSmallSectionSize, last, comp);
  }
  else
  {
    mystl::insertion_sort(first, last, comp);
  }
}

template <class RandomIter, class Compared>
void sort(RandomIter first, RandomIter last, Compared comp)
{
  if (first != last)
  {
    // 内省式排序，将区间分为一个个小区间，然后对整体进行插入排序
    mystl::intro_sort(first, last, slg2(last - first) * 2, comp);
    mystl::final_insertion_sort(first, last, comp);
  }
}

/*****************************************************************************************/
// nth_element
// 对序列重排，使得所有小于第 n 个元素的元素出现在它的前面，大于它的出现在它的后面
/*****************************************************************************************/
template <class RandomIter>
void nth_element(RandomIter first, RandomIter nth,
                 RandomIter last)
{
  if (nth == last)
    return;
  while (last - first > 3)
  {
    auto cut = mystl::unchecked_partition(first, last, mystl::median(*first,
										  *(first + (last - first) / 2),
										  *(last - 1)));
    if (cut <= nth)  // 如果 nth 位于右段
      first = cut;   // 对右段进行分割
    else
      last = cut;    // 对左段进行分割
  }
  mystl::insertion_sort(first, last);
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Compared>
void nth_element(RandomIter first, RandomIter nth,
                 RandomIter last, Compared comp)
{
  if (nth == last)
    return;
  while (last - first > 3)
  {
    auto cut = mystl::unchecked_partition(first, last, mystl::median(*first, 
										  *(first + (last - first) / 2),
										  *(last - 1)), comp);
    if (cut <= nth)  // 如果 nth 位于右段
      first = cut;   // 对右段进行分割
    else
      last = cut;    // 对左段进行分割
  }
  mystl::insertion_sort(first, last, comp);
}

/*****************************************************************************************/
// unique_copy
// 从[first, last)中将元素复制到 result 上，序列必须有序，如果有重复的元素，只会复制一次
/*****************************************************************************************/
// unique_copy_dispatch 的 forward_iterator_tag 版本
template <class InputIter, class ForwardIter>
ForwardIter
unique_copy_dispatch(InputIter first, InputIter last,
                     ForwardIter result, forward_iterator_tag)
{
  *result = *first;
  while (++first != last)
  {
    if (*result != *first)
      *++result = *first;
  }
  return ++result;
}

// unique_copy_dispatch 的 output_iterator_tag 版本
// 由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
template <class InputIter, class OutputIter>
OutputIter
unique_copy_dispatch(InputIter first, InputIter last,
                     OutputIter result, output_iterator_tag)
{
  auto value = *first;
  *result = value;
  while (++first != last)
  {
    if (value != *first)
    {
      value = *first;
      *++result = value;
    }
  }
  return ++result;
}

template <class InputIter, class OutputIter>
OutputIter
unique_copy(InputIter first, InputIter last, OutputIter result)
{
  if (first == last)
    return result;
  return mystl::unique_copy_dispatch(first, last, result, iterator_category(result));
}

// 重载版本使用函数对象 comp 代替比较操作
// unique_copy_dispatch 的 forward_iterator_tag 版本
template <class InputIter, class ForwardIter, class Compared>
ForwardIter
unique_copy_dispatch(InputIter first, InputIter last,
                     ForwardIter result, forward_iterator_tag, Compared comp)
{
  *result = *first;
  while (++first != last)
  {
    if (!comp(*result, *first))
      *++result = *first;
  }
  return ++result;
}

// unique_copy_dispatch 的 output_iterator_tag 版本
// 由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
template <class InputIter, class OutputIter, class Compared>
OutputIter
unique_copy_dispatch(InputIter first, InputIter last,
                     OutputIter result, output_iterator_tag, Compared comp)
{
  auto value = *first;
  *result = value;
  while (++first != last)
  {
    if (!comp(value, *first))
    {
      value = *first;
      *++result = value;
    }
  }
  return ++result;
}

template <class InputIter, class OutputIter, class Compared>
OutputIter
unique_copy(InputIter first, InputIter last, OutputIter result, Compared comp)
{
  if (first == last)
    return result;
  return mystl::unique_copy_dispatch(first, last, result, iterator_category(result), comp);
}

/*****************************************************************************************/
// unique
// 移除[first, last)内重复的元素，序列必须有序，和 remove 类似，它也不能真正的删除重复元素
/*****************************************************************************************/
template <class ForwardIter>
ForwardIter unique(ForwardIter first, ForwardIter last)
{
  first = mystl::adjacent_find(first, last);
  return mystl::unique_copy(first, last, first);
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter unique(ForwardIter first, ForwardIter last, Compared comp)
{
  first = mystl::adjacent_find(first, last, comp);
  return mystl::unique_copy(first, last, first, comp);
}

} // namespace mystl

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // !MYTINYSTL_ALGO_H_

