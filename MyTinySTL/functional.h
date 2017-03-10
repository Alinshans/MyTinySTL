#ifndef MYTINYSTL_FUNCTIONAL_H_
#define MYTINYSTL_FUNCTIONAL_H_

// 这个头文件包含了 mystl 的函数对象

#include <cstddef>

#include "string.h"

namespace mystl
{

// 定义一元函数的参数型别和返回值型别
template <class Arg, class Result>
struct unarg_function
{
  typedef Arg       argument_type;
  typedef Result    result_type;
};

// 定义二元函数的参数型别的返回值型别
template <class Arg1, class Arg2, class Result>
struct binary_function
{
  typedef Arg1      first_argument_type;
  typedef Arg2      second_argument_type;
  typedef Result    result_type;
};

// 函数对象：加法
template <class T>
struct plus :public binary_function<T, T, T>
{
  T operator()(const T& x, const T& y) const { return x + y; }
};

// 函数对象：减法
template <class T>
struct minus :public binary_function<T, T, T>
{
  T operator()(const T& x, const T& y) const { return x - y; }
};

// 函数对象：乘法
template <class T>
struct multiplies :public binary_function<T, T, T>
{
  T operator()(const T& x, const T& y) const { return x * y; }
};

// 函数对象：除法
template <class T>
struct divides :public binary_function<T, T, T>
{
  T operator()(const T& x, const T& y) const { return x / y; }
};

// 函数对象：模取
template <class T>
struct modulus :public binary_function<T, T, T>
{
  T operator()(const T& x, const T& y) const { return x % y; }
};

// 函数对象：否定
template <class T>
struct negate :public unarg_function<T, T>
{
  T operator()(const T& x) const { return -x; }
};

// 加法的证同元素
template <class T>
inline T identity_element(plus<T>) { return T(0); }

// 乘法的证同元素
template <class T>
inline T identity_element(multiplies<T>) { return T(1); }

// 函数对象：等于
template <class T>
struct equal_to :public binary_function<T, T, bool>
{
  bool operator()(const T& x, const T& y) const { return x == y; }
};

// 函数对象：不等于
template <class T>
struct not_equal_to :public binary_function<T, T, bool>
{
  bool operator()(const T& x, const T& y) const { return x != y; }
};

// 函数对象：大于
template <class T>
struct greater :public binary_function<T, T, bool>
{
  bool operator()(const T& x, const T& y) const { return x > y; }
};

// 函数对象：小于
template <class T>
struct less :public binary_function<T, T, bool>
{
  bool operator()(const T& x, const T& y) const { return x < y; }
};

// 函数对象：大于等于
template <class T>
struct greater_equal :public binary_function<T, T, bool>
{
  bool operator()(const T& x, const T& y) const { return x >= y; }
};

// 函数对象：小于等于
template <class T>
struct less_equal :public binary_function<T, T, bool>
{
  bool operator()(const T& x, const T& y) const { return x <= y; }
};

// 函数对象：逻辑与
template <class T>
struct logical_and :public binary_function<T, T, bool>
{
  bool operator()(const T& x, const T& y) const { return x && y; }
};

// 函数对象：逻辑或
template <class T>
struct logical_or :public binary_function<T, T, bool>
{
  bool operator()(const T& x, const T& y) const { return x || y; }
};

// 函数对象：逻辑非
template <class T>
struct logical_not :public unarg_function<T, bool>
{
  bool operator()(const T& x) const { return !x; }
};

// 证同函数：不会改变元素，返回本身
template <class T>
struct identity :public unarg_function<T, bool>
{
  const T& operator()(const T& x) const { return x; }
};

// 选择函数：接受一个 pair，返回第一个元素
template <class Pair>
struct selectfirst :public unarg_function<Pair, typename Pair::first_type>
{
  const typename Pair::first_type& operator()(const Pair& x) const
  {
    return x.first;
  }
};

// 选择函数：接受一个 pair，返回第二个元素
template <class Pair>
struct selectsecond :public unarg_function<Pair, typename Pair::second_type>
{
  const typename Pair::second_type& operator()(const Pair& x) const
  {
    return x.second;
  }
};

// 投射函数：返回第一参数
template <class Arg1, class Arg2>
struct projectfirst :public binary_function<Arg1, Arg2, Arg1>
{
  Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};

// 投射函数：返回第二参数
template <class Arg1, class Arg2>
struct projectsecond :public binary_function<Arg1, Arg2, Arg1>
{
  Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
};

// 哈希函数对象

// 对于大部分类型，hash function 什么都不做
template <class Key>
struct hash {};

// 针对 char* 和 const char* 的特化版本
inline size_t __hash_string(const char* s)
{
  size_t h = 0;
  for (; *s; ++s)
    h = 3 * h + static_cast<size_t>(*s);
  return h;
}

template <> struct hash<char*>
{
  size_t operator()(const char* s) const { return __hash_string(s); }
};

template <> struct hash<const char*>
{
  size_t operator()(const char* s) const { return __hash_string(s); }
};

// 针对 mystl::string 的特化版本
template <> struct hash<mystl::string>
{
  size_t operator()(const mystl::string& str) const { return __hash_string(str.data()); }
};

// 针对 char，int，long 等整数型别，只是返回原值

template <> struct hash<char>
{
  size_t operator()(char x) const { return x; }
};

template <> struct hash<unsigned char>
{
  size_t operator()(unsigned char x) const { return x; }
};

template <> struct hash<signed char>
{
  size_t operator()(unsigned char x) const { return x; }
};

template <> struct hash<short>
{
  size_t operator()(short x) const { return x; }
};

template <> struct hash<unsigned short>
{
  size_t operator()(unsigned short x) const { return x; }
};

template <> struct hash<int>
{
  size_t operator()(int x) const { return x; }
};

template <> struct hash<unsigned int>
{
  size_t operator()(unsigned int x) const { return x; }
};

template <> struct hash<long>
{
  size_t operator()(long x) const { return x; }
};

template <> struct hash<unsigned long>
{
  size_t operator()(unsigned long x) const { return x; }
};

} // namespace mystl
#endif // !MYTINYSTL_FUNCTIONAL_H_

