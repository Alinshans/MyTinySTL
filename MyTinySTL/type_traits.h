#ifndef MYTINYSTL_TYPE_TRAITS_H_
#define MYTINYSTL_TYPE_TRAITS_H_

// 这个头文件用于提取类型信息

// use standard header for type_traits
#include <type_traits>

namespace mystl
{

// helper struct

template <class T, T v>
struct m_integral_constant
{
  static constexpr T value = v;
};

template <bool b>
using m_bool_constant = m_integral_constant<bool, b>;

typedef m_bool_constant<true>  m_true_type;
typedef m_bool_constant<false> m_false_type;

/*****************************************************************************************/

// type_tarits
// 型别萃取
template <class type>
struct type_traits
{
  typedef m_true_type     this_dummy_member_must_be_first;
  // 表示这个 type_traits 是 mystl 中专用的结构

  typedef m_false_type    has_trivial_default_constructor;
  typedef m_false_type    has_trivial_copy_constructor;
  typedef m_false_type    has_trivial_assignment_operator;
  typedef m_false_type    has_trivial_destructor;
  typedef m_false_type    is_POD_type;
};

// 萃取类型特性，针对内置类型提供特化版本
template<>    struct type_traits<bool>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<char>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<signed char>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<unsigned char>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<short>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<unsigned short>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<int>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<unsigned int>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<long>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<unsigned long>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<long long>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<unsigned long long>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<float>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<double>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<long double>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<char*>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<signed char*>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<unsigned char*>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<const char*>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<const signed char*>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

template<>    struct type_traits<const unsigned char*>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

//针对原生指针设计 type_traits 偏特化版本
template <class T>
struct type_traits<T*>
{
  typedef m_true_type    has_trivial_default_constructor;
  typedef m_true_type    has_trivial_copy_constructor;
  typedef m_true_type    has_trivial_assignment_operator;
  typedef m_true_type    has_trivial_destructor;
  typedef m_true_type    is_POD_type;
};

/*****************************************************************************************/
// char_type
// 字符型型别萃取

struct char_type {};
struct wchar_type {};

template<class Type>
struct char_traits
{
  typedef m_false_type    value_type;
};

template<> struct char_traits<char>
{
  typedef char_type       value_type;
};

template<> struct char_traits<wchar_t>
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
struct is_pair : mystl::m_false_type {};

template <class T1, class T2>
struct is_pair<mystl::pair<T1, T2>> : mystl::m_true_type {};

} // namespace mystl

#endif // !MYTINYSTL_TYPE_TRAITS_H_

