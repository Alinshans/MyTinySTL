#ifndef MYTINYSTL_PAIR_H_
#define MYTINYSTL_PAIR_H_

// 这个头文件包含一个结构体模板 pair，其中储存了一对数据类型

#include "util.h"

namespace mystl
{

// 结构体模板 : pair
// 有两个模板参数，参数一代表第一种数据类型，参数二代表第二种数据类型
// 用 first 和 second 来分别取出第一个数据和第二个数据
template <class T1, class T2>
struct pair
{
  typedef T1    first_type;
  typedef T2    second_type;

  T1 first;   // 保存第一个数据
  T2 second;  // 保存第二个数据

  // 构造、复制、移动、赋值函数
  pair() = default;

  pair(const T1& a, const T2& b) 
    : first(a), second(b) {}

  pair(const pair& rhs)
    : first(rhs.first), second(rhs.second) {}
  pair(pair&& rhs)
    : first(mystl::move(rhs.first)), second(mystl::move(rhs.second)) {}

  template <class U1, class U2>
  pair(U1&& a, U2&& b)
    : first(a), second(b) {}

  template <class U1, class U2>
  pair(const pair<U1, U2>& p) 
    : first(p.first), second(p.second) {}

  template <class U1, class U2>
  pair(pair<U1, U2>&& p)
    : first(mystl::move(p.first)), second(mystl::move(p.second)) {}

  pair& operator=(const pair& rhs)
  {
    if (this != &rhs)
    {
      first = rhs.first;
      second = rhs.second;
    }
    return *this;
  }

  pair& operator=(pair&& rhs)
  {
    if (this != &rhs)
    {
      first = mystl::move(rhs.first);
      second = mystl::move(rhs.second);
    }
    return *this;
  }

  template <class U1, class U2>
  pair& operator=(const pair<U1, U2>& other)
  {
    first = other.first;
    second = other.second;
    return *this;
  }

  template <class U1, class U2>
  pair& operator=(pair<U1, U2>&& other)
  {
    first = mystl::move(other.first);
    second = mystl::move(other.second);
    return *this;
  }

  ~pair() = default;

  void swap(pair& other)
  {
    mystl::swap(first, other.first);
    mystl::swap(second, other.second);
  }

};

// 重载比较操作符 
template <class T1, class T2>
bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class T1, class T2>
bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
  return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
}

template <class T1, class T2>
bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
  return !(lhs == rhs);
}

template <class T1, class T2>
bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
  return rhs < lhs;
}

template <class T1, class T2>
bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
  return !(rhs < lhs);
}

template <class T1, class T2>
bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T1, class T2>
void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs)
{
  lhs.swap(rhs);
}

// 全局函数，让两个数据成为一个 pair
template <class T1, class T2>
pair<T1, T2> make_pair(T1&& first, T2&& second)
{
  return pair<T1, T2>(mystl::forward<T1>(first), mystl::forward<T2>(second));
}

} // namespace mystl
#endif // !MYTINYSTL_PAIR_H_

