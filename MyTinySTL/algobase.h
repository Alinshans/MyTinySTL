#ifndef MYTINYSTL_ALGOBASE_H_
#define MYTINYSTL_ALGOBASE_H_

// 这个头文件包含了 mystl 的基本算法

#include <cstring>
#include <utility>

#include "iterator.h"
#include "pair.h"
#include "type_traits.h"
#include "util.h"

namespace mystl
{

#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

/*****************************************************************************************/
// max
// 取二者中的较大值
/*****************************************************************************************/
template <class T>
const T& max(const T& lhs, const T& rhs)
{
  return lhs < rhs ? rhs : lhs;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class T, class Compare>
const T& max(const T& lhs, const T& rhs, Compare comp)
{
  return comp(lhs, rhs) ? rhs : lhs;
}

/*****************************************************************************************/
// min 
// 取二者中的较小值
/*****************************************************************************************/
template <class T>
const T& min(const T& lhs, const T& rhs)
{
  return rhs < lhs ? rhs : lhs;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class T, class Compare>
const T& min(const T& lhs, const T& rhs, Compare comp)
{
  return comp(rhs, lhs) ? rhs : lhs;
}

/*****************************************************************************************/
// swap
// 交换两个对象的值
/*****************************************************************************************/
template <class T>
void swap(T& lhs, T& rhs)
{
  auto tmp(mystl::move(lhs));
  lhs = mystl::move(rhs);
  rhs = mystl::move(tmp);
}

/*****************************************************************************************/
// iter_swap
// 将两个迭代器所指对象对调
/*****************************************************************************************/
template <class FIter1, class FIter2>
void iter_swap(FIter1 lhs, FIter2 rhs)
{
  mystl::swap(*lhs, *rhs);
}

/*****************************************************************************************/
// copy
// 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
/*****************************************************************************************/
// __copy 的 input_iterator_tag 版本
template <class IIter, class OIter>
OIter
__copy(IIter first, IIter last, OIter result, input_iterator_tag)
{
  for (; first != last; ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

// __copy 的 random_access_iterator_tag 版本
template <class RAIter, class OIter>
OIter
__copy(RAIter first, RAIter last, OIter result, random_access_iterator_tag)
{
  for (auto n = last - first; n > 0; --n, ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

// __copy_t : 指针所指对象具备 trivial assignment operator
template <class T>
T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
  memmove(result, first, sizeof(T) * (last - first));
  return result + (last - first);
}

// __copy_t : 指针所指对象具备 non-trivial assignment operator
template <class T>
T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
  return __copy(first, last, result, random_access_iterator_tag());
}

// 根据类型性质分派不同函数
// __copy_dispatch 的泛化版本
template <class IIter, class OIter>
struct __copy_dispatch
{
  OIter operator()(IIter first, IIter last, OIter result)
  {
    return __copy(first, last, result, iterator_category(first));
  }
};

// __copy_dispatch 的偏特化版本，两个参数都是 T* 形式
template <class T>
struct __copy_dispatch<T*, T*>
{
  T* operator()(T* first, T* last, T* result)
  {
    typedef typename __type_traits<T>::has_trivial_assignment_operator Trivial;
    return __copy_t(first, last, result, Trivial());
  }
};

// __copy_dispatch的偏特化版本，第一个参数是 const T* 形式，第二个参数是 T* 形式
template <class T>
struct __copy_dispatch<const T*, T*>
{
  T* operator()(const T* first, const T* last, T* result)
  {
    typedef typename __type_traits<T>::has_trivial_assignment_operator Trivial;
    return __copy_t(first, last, result, Trivial());
  }
};

template <class IIter, class OIter>
OIter copy(IIter first, IIter last, OIter result)
{
  return __copy_dispatch<IIter, OIter>()(first, last, result);
}

// char* 的特化版本
char* copy(const char* first, const char* last, char* result)
{
  memmove(result, first, last - first);
  return result + (last - first);
}

// wchar_t* 的特化版本
wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}

/*****************************************************************************************/
// copy_backward
// 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
/*****************************************************************************************/
// __copy_backward 的 bidirectional_iterator_tag 版本
template <class BIter1, class BIter2>
BIter2 __copy_backward(BIter1 first, BIter1 last, BIter2 result,
                       bidirectional_iterator_tag)
{
  while (first != last)
  {
    *--result = *--last;
  }
  return result;
}

// __copy_backward 的 random_access_iterator_tag 版本
template <class BIter1, class BIter2>
BIter2 __copy_backward(BIter1 first, BIter1 last, BIter2 result,
                       random_access_iterator_tag)
{
  for (auto n = last - first; n > 0; --n)
  {
    *--result = *--last;
  }
  return result;
}

template <class BIter1, class BIter2>
BIter2 copy_backward(BIter1 first, BIter1 last, BIter2 result)
{
  return __copy_backward(first, last, result, iterator_category(first));
}

/*****************************************************************************************/
// copy_if
// 把[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上
/*****************************************************************************************/
template <class IIter, class OIter, class UnaryPredicate>
OIter copy_if(IIter first, IIter last, OIter result, UnaryPredicate unary_pred)
{
  for (; first != last; ++first)
  {
    if (unary_pred(*first))
      *result++ = *first;
  }
  return result;
}

/*****************************************************************************************/
// copy_n
// 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
// 返回一个 pair 分别指向拷贝结束的尾部
/*****************************************************************************************/
// __copy_n 的 input_iterator_tag 版本
template <class IIter, class Size, class OIter>
mystl::pair<IIter, OIter>
__copy_n(IIter first, Size n, OIter result, input_iterator_tag)
{
  for (; n > 0; --n, ++first, ++result)
  {
    *result = *first;
  }
  return mystl::pair<IIter, OIter>(first, result);
}

// __copy_n 的 random_access_iterator_tag 版本
template <class IIter, class Size, class OIter>
mystl::pair<IIter, OIter>
__copy_n(IIter first, Size n, OIter result, random_access_iterator_tag)
{
  auto last = first + n;
  return mystl::pair<IIter, OIter>(last, mystl::copy(first, last, result));
}

template <class IIter, class Size, class OIter>
mystl::pair<IIter, OIter> 
copy_n(IIter first, Size n, OIter result)
{
  return __copy_n(first, n, result, iterator_category(first));
}

// char* 的特化版本
mystl::pair<const char*, char*>
copy_n(const char* first, size_t n, char* result)
{
  memmove(result, first, n);
  return mystl::make_pair(first + n, result + n);
}

// wchar_t* 的特化版本
mystl::pair<const wchar_t*, wchar_t*>
copy_n(const wchar_t* first, size_t n, wchar_t* result)
{
  memmove(result, first, sizeof(wchar_t) * n);
  return mystl::make_pair(first + n, result + n);
}

/*****************************************************************************************/
// equal
// 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
/*****************************************************************************************/
template <class IIter1, class IIter2>
bool equal(IIter1 first1, IIter1 last1, IIter2 first2)
{
  for (; first1 != last1; ++first1, ++first2)
  {
    if (*first1 != *first2)  
      return false;
  }
  return true;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class IIter1, class IIter2, class Compared>
bool equal(IIter1 first1, IIter1 last1, IIter2 first2, Compared comp)
{
  for (; first1 != last1; ++first1, ++first2)
  {
    if (!comp(*first1, *first2))  
      return false;
  }
  return true;
}

/*****************************************************************************************/
// fill
// 为 [first, last)区间内的所有元素填充新值
/*****************************************************************************************/
template <class FIter, class T>
void fill(FIter first, FIter last, const T& value)
{
  for (; first != last; ++first)
  {
    *first = value;
  }
}

// 为 one-byte 类型提供特化版本
void fill(unsigned char* first, unsigned char* last, const unsigned char& c)
{
  unsigned char tmp = c;
  memset(first, tmp, last - first);
}

void fill(signed char* first, signed char* last, const signed char& c)
{
  signed char tmp = c;
  memset(first, static_cast<unsigned char>(tmp), last - first);
}

void fill(char* first, char* last, const char& c)
{
  char tmp = c;
  memset(first, static_cast<unsigned char>(tmp), last - first);
}

/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充 n 个新值
/*****************************************************************************************/
template <class OIter, class Size, class T>
OIter fill_n(OIter first, Size n, const T& value)
{
  for (; n > 0; --n, ++first)
  {
    *first = value;
  }
  return first;
}

/*****************************************************************************************/
// lexicographical_compare
// 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
// (1)如果第一序列的元素较小，返回 true ，否则返回 false
// (2)如果到达 last1 而尚未到达 last2 返回 true
// (3)如果到达 last2 而尚未到达 last1 返回 false
// (4)如果同时到达 last1 和 last2 返回 false
/*****************************************************************************************/
template <class IIter1, class IIter2>
bool lexicographical_compare(IIter1 first1, IIter1 last1,
                             IIter2 first2, IIter2 last2)
{
  for (; first1 != last1 && first2 != last2; ++first1, ++first2)
  {
    if (*first1 < *first2)
      return true;
    if (*first2 < *first1)
      return false;
  }
  return first1 == last1 && first2 != last2;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class IIter1, class IIter2, class Compred>
bool lexicographical_compare(IIter1 first1, IIter1 last1,
                             IIter2 first2, IIter2 last2, Compred comp)
{
  for (; first1 != last1 && first2 != last2; ++first1, ++first2)
  {
    if (comp(*first1, *first2))
      return true;
    if (comp(*first2, *first1))
      return false;
  }
  return first1 == last1 && first2 != last2;
}

// 针对 const unsigned char* 的特化版本
bool lexicographical_compare(const unsigned char* first1,
                             const unsigned char* last1,
                             const unsigned char* first2,
                             const unsigned char* last2)
{
  const auto len1 = last1 - first1;
  const auto len2 = last2 - first2;
  // 先比较相同长度的部分
  const auto result = memcmp(first1, first2, mystl::min(len1, len2));
  // 若相等，长度较长的比较大
  return result != 0 ? result < 0 : len1 < len2;
}

/*****************************************************************************************/
// mismatch
// 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
/*****************************************************************************************/
template <class IIter1, class IIter2>
mystl::pair<IIter1, IIter2> 
mismatch(IIter1 first1, IIter1 last1, IIter2 first2)
{
  while (first1 != last1 && *first1 == *first2)
  {
    ++first1;
    ++first2;
  }
  return mystl::pair<IIter1, IIter2>(first1, first2);
}

// 重载版本使用函数对象 comp 代替比较操作
template <class IIter1, class IIter2, class Compred>
mystl::pair<IIter1, IIter2> 
mismatch(IIter1 first1, IIter1 last1, IIter2 first2, Compred comp)
{
  while (first1 != last1 && comp(*first1, *first2))
  {
    ++first1;
    ++first2;
  }
  return mystl::pair<IIter1, IIter2>(first1, first2);
}

} // namespace mystl
#endif // !MYTINYSTL_ALGOBASE_H_

