#ifndef MYTINYSTL_MEMORY_H_
#define MYTINYSTL_MEMORY_H_

// 这个头文件负责更高级的动态内存管理
// 包含一些基本函数、空间配置器、未初始化的储存空间管理，以及一个模板类 auto_ptr

#include <cstddef>
#include <cstdlib>
#include <climits>

#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace mystl
{

// 获取对象地址
template <class Tp>
constexpr Tp* address_of(Tp& value) noexcept
{
  return &value;
}

// 获取 / 释放 临时缓冲区

template <class T>
pair<T*, ptrdiff_t> get_buffer_helper(ptrdiff_t len, T*)
{
  if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
    len = INT_MAX / sizeof(T);
  while (len > 0)
  {
    T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
    if (tmp)
      return pair<T*, ptrdiff_t>(tmp, len);
    len /= 2;  // 申请失败时减少 len 的大小
  }
  return pair<T*, ptrdiff_t>(nullptr, 0);
}

template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len)
{
  return get_buffer_helper(len, static_cast<T*>(0));
}

template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*)
{
  return get_buffer_helper(len, static_cast<T*>(0));
}

template <class T>
void release_temporary_buffer(T* ptr)
{
  free(ptr);
}

// --------------------------------------------------------------------------------------
// 类模板 : temporary_buffer
// 进行临时缓冲区的申请与释放
template <class ForwardIterator, class T>
class temporary_buffer
{
private:
  ptrdiff_t original_len;  // 缓冲区申请的大小
  ptrdiff_t len;           // 缓冲区实际的大小
  T*        buffer;        // 指向缓冲区的指针

public:
  // 构造、析构函数
  temporary_buffer(ForwardIterator first, ForwardIterator last);

  ~temporary_buffer()
  {
    mystl::destroy(buffer, buffer + len);
    free(buffer);
  }

public:

  ptrdiff_t size()           const noexcept { return len; }
  ptrdiff_t requested_size() const noexcept { return original_len; }
  T*        begin()                noexcept { return buffer; }
  T*        end()                  noexcept { return buffer + len; }

private:
  void allocate_buffer();
  void initialize_buffer(const T&, std::true_type) {}
  void initialize_buffer(const T& value, std::false_type)
  { mystl::uninitialized_fill_n(buffer, len, value); }

private:
  temporary_buffer(const temporary_buffer&);
  void operator=(const temporary_buffer&);
};

// 构造函数
template <class ForwardIterator, class T>
temporary_buffer<ForwardIterator, T>::
temporary_buffer(ForwardIterator first, ForwardIterator last)
{
  try
  {
    len = mystl::distance(first, last);
    allocate_buffer();
    if (len > 0)
    {
      initialize_buffer(*first, std::is_trivially_default_constructible<T>());
    }
  }
  catch (...)
  {
    free(buffer);
    buffer = nullptr;
    len = 0;
  }
}

// allocate_buffer 函数
template <class ForwardIterator, class T>
void temporary_buffer<ForwardIterator, T>::allocate_buffer()
{
  original_len = len;
  if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
    len = INT_MAX / sizeof(T);
  while (len > 0)
  {
    buffer = static_cast<T*>(malloc(len * sizeof(T)));
    if (buffer)
      break;
    len /= 2;  // 申请失败时减少申请空间大小
  }
}

// --------------------------------------------------------------------------------------
// 模板类: auto_ptr
// 一个具有严格对象所有权的小型智能指针
template <class T>
class auto_ptr
{
public:
  typedef T    elem_type;

private:
  T* m_ptr;  // 实际指针

public:
  // 构造、复制、析构函数
  explicit auto_ptr(T* p = nullptr) :m_ptr(p) {}
  auto_ptr(auto_ptr& rhs) :m_ptr(rhs.release()) {}
  template <class U>
  auto_ptr(auto_ptr<U>& rhs) : m_ptr(rhs.release()) {}

  auto_ptr& operator=(auto_ptr& rhs)
  {
    if (this != &rhs)
    {
      delete m_ptr;
      m_ptr = rhs.release();
    }
    return *this;
  }
  template <class U>
  auto_ptr& operator=(auto_ptr<U>& rhs)
  {
    if (this->get() != rhs.get())
    {
      delete m_ptr;
      m_ptr = rhs.release();
    }
    return *this;
  }

  ~auto_ptr() { delete m_ptr; }

public:
  // 重载 operator* 和 operator->
  T& operator*()  const { return *m_ptr; }
  T* operator->() const { return m_ptr; }

  // 获得指针
  T* get() const { return m_ptr; }

  // 释放指针
  T* release()
  {
    T* tmp = m_ptr;
    m_ptr = nullptr;
    return tmp;
  }

  // 重置指针
  void reset(T* p = nullptr)
  {
    if (m_ptr != p)
    {
      delete m_ptr;
      m_ptr = p;
    }
  }
};

} // namespace mystl
#endif // !MYTINYSTL_MEMORY_H_

