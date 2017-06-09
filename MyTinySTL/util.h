#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_

// 这个文件包含一些通用工具

#include "type_traits.h"

namespace mystl
{

template <class T>
constexpr mystl::remove_reference_t<T>&& move(T&& arg) noexcept
{
  return static_cast<mystl::remove_reference_t<T>&&>(arg);
}

template <class T>
constexpr T&& forward(mystl::remove_reference_t<T>& arg) noexcept
{
  return static_cast<T&&>(arg);
}

template <class T>
constexpr T&& forward(mystl::remove_reference_t<T>&& arg) noexcept
{
  static_assert(!mystl::is_lvalue_reference_v<T>, "bad forward");
  return static_cast<T&&>(arg);
}

}

#endif // !MYTINYSTL_UTIL_H_

