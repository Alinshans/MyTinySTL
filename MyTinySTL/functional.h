#ifndef MYTINYSTL_FUNCTIONAL_H_
#define MYTINYSTL_FUNCTIONAL_H_

// 这个头文件包含了 mystl 的函数对象与哈希函数

#include <cstddef>

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
T identity_element(plus<T>) { return T(0); }

// 乘法的证同元素
template <class T>
T identity_element(multiplies<T>) { return T(1); }

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

/*****************************************************************************************/
// 哈希函数对象

// 对于大部分类型，hash function 什么都不做
template <class Key>
struct hash {};

// 针对指针的偏特化版本
template <class T>
struct hash<T*>
{
  size_t operator()(T* p) const noexcept
  { return reinterpret_cast<size_t>(p); }
};

// 对于整型类型，只是返回原值
#define MYSTL_TRIVIAL_HASH_FCN(Type)         \
template <> struct hash<Type>                \
{                                            \
  size_t operator()(Type val) const noexcept \
  { return static_cast<size_t>(val); }       \
};

MYSTL_TRIVIAL_HASH_FCN(bool)

MYSTL_TRIVIAL_HASH_FCN(char)

MYSTL_TRIVIAL_HASH_FCN(signed char)

MYSTL_TRIVIAL_HASH_FCN(unsigned char)

MYSTL_TRIVIAL_HASH_FCN(wchar_t)

MYSTL_TRIVIAL_HASH_FCN(char16_t)

MYSTL_TRIVIAL_HASH_FCN(char32_t)

MYSTL_TRIVIAL_HASH_FCN(short)

MYSTL_TRIVIAL_HASH_FCN(unsigned short)

MYSTL_TRIVIAL_HASH_FCN(int)

MYSTL_TRIVIAL_HASH_FCN(unsigned int)

MYSTL_TRIVIAL_HASH_FCN(long)

MYSTL_TRIVIAL_HASH_FCN(unsigned long)

MYSTL_TRIVIAL_HASH_FCN(long long)

MYSTL_TRIVIAL_HASH_FCN(unsigned long long)

#undef MYSTL_TRIVIAL_HASH_FCN

// 对于浮点数，逐位哈希
inline size_t bitwise_hash(const unsigned char* first, size_t count)
{
#if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) &&__SIZEOF_POINTER__ == 8)
  const size_t fnv_offset = 14695981039346656037ull;
  const size_t fnv_prime = 1099511628211ull;
#else
  const size_t fnv_offset = 2166136261u;
  const size_t fnv_prime = 16777619u;
#endif
  size_t result = fnv_offset;
  for (size_t i = 0; i < count; ++i)
  {
    result ^= (size_t)first[i];
    result *= fnv_prime;
  }
  return result;
}

template <>
struct hash<float>
{
  size_t operator()(const float& val)
  { 
    return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(float));
  }
};

template <>
struct hash<double>
{
  size_t operator()(const double& val)
  {
    return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(double));
  }
};

template <>
struct hash<long double>
{
  size_t operator()(const long double& val)
  {
    return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(long double));
  }
};

} // namespace mystl
#endif // !MYTINYSTL_FUNCTIONAL_H_

