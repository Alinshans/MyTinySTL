#ifndef MYTINYSTL_TYPE_TRAITS_H_
#define MYTINYSTL_TYPE_TRAITS_H_

// 这个头文件用于提取类型信息

namespace mystl
{

// helper struct

template <class T, T v>
struct __integral_constant
{
  static constexpr T value = v;
};

template <bool b>
using __bool_constant = __integral_constant<bool, b>;

typedef __bool_constant<true>  __true_type;
typedef __bool_constant<false> __false_type;

/*****************************************************************************************/

// __type_tarits
// 型别萃取
template <class type>
struct __type_traits
{
  typedef __true_type        this_dummy_member_must_be_first;
  // 表示这个 __type_traits 是 mystl 中专用的结构

  typedef __false_type    has_trivial_default_constructor;
  typedef __false_type    has_trivial_copy_constructor;
  typedef __false_type    has_trivial_assignment_operator;
  typedef __false_type    has_trivial_destructor;
  typedef __false_type    is_POD_type;
};

// 萃取类型特性，针对内置类型提供特化版本
template<>    struct __type_traits<bool>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<char>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<signed char>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<unsigned char>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<short>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<unsigned short>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<int>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<unsigned int>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<long>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<unsigned long>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<long long>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<unsigned long long>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<float>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<double>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<long double>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<signed char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<unsigned char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<const char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<const signed char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template<>    struct __type_traits<const unsigned char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

//针对原生指针设计 __type_traits 偏特化版本
template <class T>
struct __type_traits<T*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

/*****************************************************************************************/
// __is_integer
// 整型型别萃取
template <class Type>
struct __is_integer
{
  typedef __false_type    is_integer;
};

template<> struct __is_integer<bool>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<char>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<signed char>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<unsigned char>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<short>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<unsigned short>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<int>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<unsigned int>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<long>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<unsigned long>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<long long>
{
  typedef __true_type        is_integer;
};

template<> struct __is_integer<unsigned long long>
{
  typedef __true_type        is_integer;
};

/*****************************************************************************************/
// __char_type
// 字符型型别萃取

struct char_type {};
struct wchar_type {};

template<class Type>
struct __char_type
{
  typedef __false_type    value_type;
};

template<> struct __char_type<char>
{
  typedef char_type       value_type;
};

template<> struct __char_type<wchar_t>
{
  typedef wchar_type      value_type;
};

/*****************************************************************************************/
// type traits

// is_pair

// --- forward declaration begin
template <class T1, class T2>
struct pair;
// --- forward declaration end

template <class T>
struct is_pair : mystl::__false_type {};

template <class T1, class T2>
struct is_pair<mystl::pair<T1, T2>> : mystl::__true_type {};

} // namespace mystl

// use standard header for type_traits
#include <type_traits>

#endif // !MYTINYSTL_TYPE_TRAITS_H_

