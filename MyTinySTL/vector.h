#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

// 这个头文件包含一个模板类 vector
// vector : 向量

// notes:
//
// 异常保证：
// mystl::vecotr<T> 满足基本异常保证，部分函数无异常保证，并对以下函数做强异常安全保证：
//   * emplace
//   * emplace_back
//   * push_back
// 当 std::is_nothrow_move_assignable<T>::value == true 时，以下函数也满足强异常保证：
//   * reserve
//   * resize
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"
#include "algo.h"

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

// 模板类: vector 
// 模板参数 T 代表类型
template <class T>
class vector
{
  static_assert(!std::is_same<bool, T>::value, "vector<bool> is abandoned in mystl");
public:
  // vector 的嵌套型别定义
  typedef mystl::allocator<T>                      allocator_type;
  typedef mystl::allocator<T>                      data_allocator;

  typedef typename allocator_type::value_type      value_type;
  typedef typename allocator_type::pointer         pointer;
  typedef typename allocator_type::const_pointer   const_pointer;
  typedef typename allocator_type::reference       reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type       size_type;
  typedef typename allocator_type::difference_type difference_type;

  typedef value_type*                              iterator;
  typedef const value_type*                        const_iterator;
  typedef mystl::reverse_iterator<iterator>        reverse_iterator;
  typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

  allocator_type get_allocator() { return data_allocator(); }

private:
  iterator begin_;  // 表示目前使用空间的头部
  iterator end_;    // 表示目前使用空间的尾部
  iterator cap_;    // 表示目前储存空间的尾部

public:
  // 构造、复制、移动、析构函数
  vector() noexcept
  { try_init(); }

  explicit vector(size_type n)
  { fill_init(n, value_type()); }

  vector(size_type n, const value_type& value)
  { fill_init(n, value); }

  template <class Iter, typename std::enable_if<
    mystl::is_input_iterator<Iter>::value, int>::type = 0>
  vector(Iter first, Iter last)
  {
    MYSTL_DEBUG(!(last < first));
    range_init(first, last);
  }

  vector(const vector& rhs)
  {
    range_init(rhs.begin_, rhs.end_);
  }

  vector(vector&& rhs) noexcept
    :begin_(rhs.begin_),
    end_(rhs.end_),
    cap_(rhs.cap_)
  {
    rhs.begin_ = nullptr;
    rhs.end_ = nullptr;
    rhs.cap_ = nullptr;
  }

  vector(std::initializer_list<value_type> ilist)
  {
    range_init(ilist.begin(), ilist.end());
  }

  vector& operator=(const vector& rhs);
  vector& operator=(vector&& rhs) noexcept;

  vector& operator=(std::initializer_list<value_type> ilist)
  {
    vector tmp(ilist.begin(), ilist.end());
    swap(tmp);
    return *this;
  }

  ~vector()
  { 
    destroy_and_recover(begin_, end_, cap_ - begin_);
    begin_ = end_ = cap_ = nullptr;
  }

public:

  // 迭代器相关操作
  iterator               begin()         noexcept 
  { return begin_; }
  const_iterator         begin()   const noexcept
  { return begin_; }
  iterator               end()           noexcept
  { return end_; }
  const_iterator         end()     const noexcept 
  { return end_; }

  reverse_iterator       rbegin()        noexcept 
  { return reverse_iterator(end()); }
  const_reverse_iterator rbegin()  const noexcept
  { return const_reverse_iterator(end()); }
  reverse_iterator       rend()          noexcept
  { return reverse_iterator(begin()); }
  const_reverse_iterator rend()    const noexcept 
  { return const_reverse_iterator(begin()); }

  const_iterator         cbegin()  const noexcept 
  { return begin(); }
  const_iterator         cend()    const noexcept
  { return end(); }
  const_reverse_iterator crbegin() const noexcept
  { return rbegin(); }
  const_reverse_iterator crend()   const noexcept
  { return rend(); }

  // 容量相关操作
  bool      empty()    const noexcept
  { return begin_ == end_; }
  size_type size()     const noexcept
  { return static_cast<size_type>(end_ - begin_); }
  size_type max_size() const noexcept
  { return static_cast<size_type>(-1) / sizeof(T); }
  size_type capacity() const noexcept
  { return static_cast<size_type>(cap_ - begin_); }
  void      reserve(size_type n);
  void      shrink_to_fit();

  // 访问元素相关操作
  reference operator[](size_type n)
  {
    MYSTL_DEBUG(n < size());
    return *(begin_ + n);
  }
  const_reference operator[](size_type n) const
  {
    MYSTL_DEBUG(n < size());
    return *(begin_ + n);
  }
  reference at(size_type n)
  {
    THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
    return (*this)[n];
  }
  const_reference at(size_type n) const
  {
    THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
    return (*this)[n];
  }

  reference front()
  {
    MYSTL_DEBUG(!empty());
    return *begin_;
  }
  const_reference front() const
  {
    MYSTL_DEBUG(!empty());
    return *begin_;
  }
  reference back()
  {
    MYSTL_DEBUG(!empty());
    return *(end_ - 1);
  }
  const_reference back() const
  {
    MYSTL_DEBUG(!empty());
    return *(end_ - 1);
  }

  pointer       data()       noexcept { return begin_; }
  const_pointer data() const noexcept { return begin_; }

  // 修改容器相关操作

  // assign

  void assign(size_type n, const value_type& value)
  { fill_assign(n, value); }

  template <class Iter, typename std::enable_if<
    mystl::is_input_iterator<Iter>::value, int>::type = 0>
  void assign(Iter first, Iter last)
  {
    MYSTL_DEBUG(!(last < first));
    copy_assign(first, last, iterator_category(first));
  }

  void assign(std::initializer_list<value_type> il)
  { copy_assign(il.begin(), il.end(), mystl::forward_iterator_tag{}); }

  // emplace / emplace_back

  template <class... Args>
  iterator emplace(const_iterator pos, Args&& ...args);

  template <class... Args>
  void emplace_back(Args&& ...args);

  // push_back / pop_back

  void push_back(const value_type& value);
  void push_back(value_type&& value)
  { emplace_back(mystl::move(value)); }

  void pop_back();

  // insert

  iterator insert(const_iterator pos, const value_type& value);
  iterator insert(const_iterator pos, value_type&& value)
  { return emplace(pos, mystl::move(value)); }

  iterator insert(const_iterator pos, size_type n, const value_type& value)
  {
    MYSTL_DEBUG(pos >= begin() && pos <= end());
    return fill_insert(const_cast<iterator>(pos), n, value);
  }

  template <class Iter, typename std::enable_if<
    mystl::is_input_iterator<Iter>::value, int>::type = 0>
  void     insert(const_iterator pos, Iter first, Iter last)
  {
    MYSTL_DEBUG(pos >= begin() && pos <= end() && !(last < first));
    copy_insert(const_cast<iterator>(pos), first, last);
  }

  // erase / clear
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  void     clear() { erase(begin(), end()); }

  // resize / reverse
  void     resize(size_type new_size) { return resize(new_size, value_type()); }
  void     resize(size_type new_size, const value_type& value);

  void     reverse() { mystl::reverse(begin(), end()); }

  // swap
  void     swap(vector& rhs) noexcept;

private:
  // helper functions

  // initialize / destroy
  void      try_init() noexcept;

  void      init_space(size_type size, size_type cap);

  void      fill_init(size_type n, const value_type& value);
  template <class Iter>
  void      range_init(Iter first, Iter last);

  void      destroy_and_recover(iterator first, iterator last, size_type n);

  // calculate the growth size
  size_type get_new_cap(size_type add_size);

  // assign

  void      fill_assign(size_type n, const value_type& value);

  template <class IIter>
  void      copy_assign(IIter first, IIter last, input_iterator_tag);

  template <class FIter>
  void      copy_assign(FIter first, FIter last, forward_iterator_tag);

  // reallocate

  template <class... Args>
  void      reallocate_emplace(iterator pos, Args&& ...args);
  void      reallocate_insert(iterator pos, const value_type& value);

  // insert

  iterator  fill_insert(iterator pos, size_type n, const value_type& value);
  template <class IIter>
  void      copy_insert(iterator pos, IIter first, IIter last);

  // shrink_to_fit

  void      reinsert(size_type size);
};

/*****************************************************************************************/

// 复制赋值操作符
template <class T>
vector<T>& vector<T>::operator=(const vector& rhs)
{
  if (this != &rhs)
  {
    const auto len = rhs.size();
    if (len > capacity())
    { 
      vector tmp(rhs.begin(), rhs.end());
      swap(tmp);
    }
    else if (size() >= len)
    {
      auto i = mystl::copy(rhs.begin(), rhs.end(), begin());
      data_allocator::destroy(i, end_);
      end_ = begin_ + len;
    }
    else
    { 
      mystl::copy(rhs.begin(), rhs.begin() + size(), begin_);
      mystl::uninitialized_copy(rhs.begin() + size(), rhs.end(), end_);
      cap_ = end_ = begin_ + len;
    }
  }
  return *this;
}

// 移动赋值操作符
template <class T>
vector<T>& vector<T>::operator=(vector&& rhs) noexcept
{
  destroy_and_recover(begin_, end_, cap_ - begin_);
  begin_ = rhs.begin_;
  end_ = rhs.end_;
  cap_ = rhs.cap_;
  rhs.begin_ = nullptr;
  rhs.end_ = nullptr;
  rhs.cap_ = nullptr;
  return *this;
}

// 预留空间大小，当原容量小于要求大小时，才会重新分配
template <class T>
void vector<T>::reserve(size_type n)
{
  if (capacity() < n)
  {
    THROW_LENGTH_ERROR_IF(n > max_size(),
                          "n can not larger than max_size() in vector<T>::reserve(n)");
    const auto old_size = size();
    auto tmp = data_allocator::allocate(n);
    mystl::uninitialized_move(begin_, end_, tmp);
    data_allocator::deallocate(begin_, cap_ - begin_);
    begin_ = tmp;
    end_ = tmp + old_size;
    cap_ = begin_ + n;
  }
}

// 放弃多余的容量
template <class T>
void vector<T>::shrink_to_fit()
{
  if (end_ < cap_)
  {
    reinsert(size());
  }
}

// 在 pos 位置就地构造元素，避免额外的复制或移动开销
template <class T>
template <class ...Args>
typename vector<T>::iterator
vector<T>::emplace(const_iterator pos, Args&& ...args)
{
  MYSTL_DEBUG(pos >= begin() && pos <= end());
  iterator xpos = const_cast<iterator>(pos);
  const size_type n = xpos - begin_;
  if (end_ != cap_ && xpos == end_)
  {
    data_allocator::construct(mystl::address_of(*end_), mystl::forward<Args>(args)...);
    ++end_;
  }
  else if (end_ != cap_)
  {
    auto new_end = end_;
    data_allocator::construct(mystl::address_of(*end_), *(end_ - 1));
    ++new_end;
    mystl::copy_backward(xpos, end_ - 1, end_);
    *xpos = value_type(mystl::forward<Args>(args)...);
    end_ = new_end;
  }
  else
  {
    reallocate_emplace(xpos, mystl::forward<Args>(args)...);
  }
  return begin() + n;
}

// 在尾部就地构造元素，避免额外的复制或移动开销
template <class T>
template <class ...Args>
void vector<T>::emplace_back(Args&& ...args)
{
  if (end_ < cap_)
  {
    data_allocator::construct(mystl::address_of(*end_), mystl::forward<Args>(args)...);
    ++end_;
  }
  else
  {
    reallocate_emplace(end_, mystl::forward<Args>(args)...);
  }
}

// 在尾部插入元素
template <class T>
void vector<T>::push_back(const value_type& value)
{
  if (end_ != cap_)
  {
    data_allocator::construct(mystl::address_of(*end_), value);
    ++end_;
  }
  else
  {
    reallocate_insert(end_, value);
  }
}

// 弹出尾部元素
template <class T>
void vector<T>::pop_back()
{
  MYSTL_DEBUG(!empty());
  data_allocator::destroy(end_ - 1);
  --end_;
}

// 在 pos 处插入元素
template <class T>
typename vector<T>::iterator
vector<T>::insert(const_iterator pos, const value_type& value)
{
  MYSTL_DEBUG(pos >= begin() && pos <= end());
  iterator xpos = const_cast<iterator>(pos);
  const size_type n = pos - begin_;
  if (end_ != cap_ && xpos == end_)
  {
    data_allocator::construct(mystl::address_of(*end_), value);
    ++end_;
  }
  else if (end_ != cap_)
  {
    auto new_end = end_;
    data_allocator::construct(mystl::address_of(*end_), *(end_ - 1));
    ++new_end;
    auto value_copy = value;  // 避免元素因以下复制操作而被改变
    mystl::copy_backward(xpos, end_ - 1, end_);
    *xpos = mystl::move(value_copy);
    end_ = new_end;
  }
  else
  {
    reallocate_insert(xpos, value);
  }
  return begin_ + n;
}

// 删除 pos 位置上的元素
template <class T>
typename vector<T>::iterator
vector<T>::erase(const_iterator pos)
{
  MYSTL_DEBUG(pos >= begin() && pos < end());
  iterator xpos = begin_ + (pos - begin());
  mystl::move(xpos + 1, end_, xpos);
  data_allocator::destroy(end_ - 1);
  --end_;
  return xpos;
}

// 删除[first, last)上的元素
template <class T>
typename vector<T>::iterator
vector<T>::erase(const_iterator first, const_iterator last)
{
  MYSTL_DEBUG(first >= begin() && last <= end() && !(last < first));
  const auto n = first - begin();
  iterator r = begin_ + (first - begin());
  data_allocator::destroy(mystl::move(r + (last - first), end_, r), end_);
  end_ = end_ - (last - first);
  return begin_ + n;
}

// 重置容器大小
template <class T>
void vector<T>::resize(size_type new_size, const value_type& value)
{
  if (new_size < size())
  {
    erase(begin() + new_size, end());
  }
  else
  {
    insert(end(), new_size - size(), value);
  }
}

// 与另一个 vector 交换
template <class T>
void vector<T>::swap(vector<T>& rhs) noexcept
{
  if (this != &rhs)
  {
    mystl::swap(begin_, rhs.begin_);
    mystl::swap(end_, rhs.end_);
    mystl::swap(cap_, rhs.cap_);
  }
}

/*****************************************************************************************/
// helper function

// try_init 函数，若分配失败则忽略，不抛出异常
template <class T>
void vector<T>::try_init() noexcept
{
  try
  {
    begin_ = data_allocator::allocate(16);
    end_ = begin_;
    cap_ = begin_ + 16;
  }
  catch (...)
  {
    begin_ = nullptr;
    end_ = nullptr;
    cap_ = nullptr;
  }
}

// init_space 函数
template <class T>
void vector<T>::init_space(size_type size, size_type cap)
{
  try
  {
    begin_ = data_allocator::allocate(cap);
    end_ = begin_ + size;
    cap_ = begin_ + cap;
  }
  catch (...)
  {
    begin_ = nullptr;
    end_ = nullptr;
    cap_ = nullptr;
    throw;
  }
}

// fill_init 函数
template <class T>
void vector<T>::
fill_init(size_type n, const value_type& value)
{
  const size_type init_size = mystl::max(static_cast<size_type>(16), n);
  init_space(n, init_size);
  mystl::uninitialized_fill_n(begin_, n, value);
}

// range_init 函数
template <class T>
template <class Iter>
void vector<T>::
range_init(Iter first, Iter last)
{
  const size_type init_size = mystl::max(static_cast<size_type>(last - first),
                                         static_cast<size_type>(16));
  init_space(static_cast<size_type>(last - first), init_size);
  mystl::uninitialized_copy(first, last, begin_);
}

// destroy_and_recover 函数
template <class T>
void vector<T>::
destroy_and_recover(iterator first, iterator last, size_type n)
{
  data_allocator::destroy(first, last);
  data_allocator::deallocate(first, n);
}

// get_new_cap 函数
template <class T>
typename vector<T>::size_type 
vector<T>::
get_new_cap(size_type add_size)
{
  const auto old_size = capacity();
  THROW_LENGTH_ERROR_IF(old_size > max_size() - add_size,
                        "vector<T>'s size too big");
  if (old_size > max_size() - old_size / 2)
  {
    return old_size + add_size > max_size() - 16
      ? old_size + add_size : old_size + add_size + 16;
  }
  const size_type new_size = old_size == 0
    ? mystl::max(add_size, static_cast<size_type>(16))
    : mystl::max(old_size + old_size / 2, old_size + add_size);
  return new_size;
}

// fill_assign 函数
template <class T>
void vector<T>::
fill_assign(size_type n, const value_type& value)
{
  if (n > capacity())
  {
    vector tmp(n, value);
    swap(tmp);
  }
  else if (n > size())
  {
    mystl::fill(begin(), end(), value);
    end_ = mystl::uninitialized_fill_n(end_, n - size(), value);
  }
  else
  {
    erase(mystl::fill_n(begin_, n, value), end_);
  }
}

// copy_assign 函数
template <class T>
template <class IIter>
void vector<T>::
copy_assign(IIter first, IIter last, input_iterator_tag)
{
  auto cur = begin_;
  for (; first != last && cur != end_; ++first, ++cur)
  {
    *cur = *first;
  }
  if (first == last)
  {
    erase(cur, end_);
  }
  else
  {
    insert(end_, first, last);
  }
}

// 用 [first, last) 为容器赋值
template <class T>
template <class FIter>
void vector<T>::
copy_assign(FIter first, FIter last, forward_iterator_tag)
{
  const size_type len = mystl::distance(first, last);
  if (len > capacity())
  {
    vector tmp(first, last);
    swap(tmp);
  }
  else if (size() >= len)
  {
    auto new_end = mystl::copy(first, last, begin_);
    data_allocator::destroy(new_end, end_);
    end_ = new_end;
  }
  else
  {
    auto mid = first;
    mystl::advance(mid, size());
    mystl::copy(first, mid, begin_);
    auto new_end = mystl::uninitialized_copy(mid, last, end_);
    end_ = new_end;
  }
}

// 重新分配空间并在 pos 处就地构造元素
template <class T>
template <class ...Args>
void vector<T>::
reallocate_emplace(iterator pos, Args&& ...args)
{
  const auto new_size = get_new_cap(1);
  auto new_begin = data_allocator::allocate(new_size);
  auto new_end = new_begin;
  try
  {
    new_end = mystl::uninitialized_move(begin_, pos, new_begin);
    data_allocator::construct(mystl::address_of(*new_end), mystl::forward<Args>(args)...);
    ++new_end;
    new_end = mystl::uninitialized_move(pos, end_, new_end);
  }
  catch (...)
  {
    data_allocator::deallocate(new_begin, new_size);
    throw;
  }
  destroy_and_recover(begin_, end_, cap_ - begin_);
  begin_ = new_begin;
  end_ = new_end;
  cap_ = new_begin + new_size;
}

// 重新分配空间并在 pos 处插入元素
template <class T>
void vector<T>::reallocate_insert(iterator pos, const value_type& value)
{
  const auto new_size = get_new_cap(1);
  auto new_begin = data_allocator::allocate(new_size);
  auto new_end = new_begin;
  const value_type& value_copy = value;
  try
  {
    new_end = mystl::uninitialized_move(begin_, pos, new_begin);
    data_allocator::construct(mystl::address_of(*new_end), value_copy);
    ++new_end;
    new_end = mystl::uninitialized_move(pos, end_, new_end);
  }
  catch (...)
  {
    data_allocator::deallocate(new_begin, new_size);
    throw;
  }
  destroy_and_recover(begin_, end_, cap_ - begin_);
  begin_ = new_begin;
  end_ = new_end;
  cap_ = new_begin + new_size;
}

// fill_insert 函数
template <class T>
typename vector<T>::iterator 
vector<T>::
fill_insert(iterator pos, size_type n, const value_type& value)
{
  if (n == 0)
    return pos;
  const size_type xpos = pos - begin_;
  const value_type value_copy = value;  // 避免被覆盖
  if (static_cast<size_type>(cap_ - end_) >= n)
  { // 如果备用空间大于等于增加的空间
    const size_type after_elems = end_ - pos;
    auto old_end = end_;
    if (after_elems > n)
    {
      mystl::uninitialized_copy(end_ - n, end_, end_);
      end_ += n;
      mystl::move_backward(pos, old_end - n, old_end);
      mystl::uninitialized_fill_n(pos, n, value_copy);
    }
    else
    {
      end_ = mystl::uninitialized_fill_n(end_, n - after_elems, value_copy);
      end_ = mystl::uninitialized_move(pos, old_end, end_);
      mystl::uninitialized_fill_n(pos, after_elems, value_copy);
    }
  }
  else
  { // 如果备用空间不足
    const auto new_size = get_new_cap(n);
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    try
    {
      new_end = mystl::uninitialized_move(begin_, pos, new_begin);
      new_end = mystl::uninitialized_fill_n(new_end, n, value);
      new_end = mystl::uninitialized_move(pos, end_, new_end);
    }
    catch (...)
    {
      destroy_and_recover(new_begin, new_end, new_size);
      throw;
    }
    data_allocator::deallocate(begin_, cap_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    cap_ = begin_ + new_size;
  }
  return begin_ + xpos;
}

// copy_insert 函数
template <class T>
template <class IIter>
void vector<T>::
copy_insert(iterator pos, IIter first, IIter last)
{
  if (first == last)
    return;
  const auto n = mystl::distance(first, last);
  if ((cap_ - end_) >= n)
  { // 如果备用空间大小足够
    const auto after_elems = end_ - pos;
    auto old_end = end_;
    if (after_elems > n)
    {
      end_ = mystl::uninitialized_copy(end_ - n, end_, end_);
      mystl::move_backward(pos, old_end - n, old_end);
      mystl::uninitialized_copy(first, last, pos);
    }
    else
    {
      auto mid = first;
      mystl::advance(mid, after_elems);
      end_ = mystl::uninitialized_copy(mid, last, end_);
      end_ = mystl::uninitialized_move(pos, old_end, end_);
      mystl::uninitialized_copy(first, mid, pos);
    }
  }
  else
  { // 备用空间不足
    const auto new_size = get_new_cap(n);
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    try
    {
      new_end = mystl::uninitialized_move(begin_, pos, new_begin);
      new_end = mystl::uninitialized_copy(first, last, new_end);
      new_end = mystl::uninitialized_move(pos, end_, new_end);
    }
    catch (...)
    {
      destroy_and_recover(new_begin, new_end, new_size);
      throw;
    }
    data_allocator::deallocate(begin_, cap_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    cap_ = begin_ + new_size;
  }
}

// reinsert 函数
template <class T>
void vector<T>::reinsert(size_type size)
{
  auto new_begin = data_allocator::allocate(size);
  try
  {
    mystl::uninitialized_move(begin_, end_, new_begin);
  }
  catch (...)
  {
    data_allocator::deallocate(new_begin, size);
    throw;
  }
  data_allocator::deallocate(begin_, cap_ - begin_);
  begin_ = new_begin;
  end_ = begin_ + size;
  cap_ = begin_ + size;
}

/*****************************************************************************************/
// 重载比较操作符

template <class T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs)
{
  return lhs.size() == rhs.size() &&
    mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T>
bool operator<(const vector<T>& lhs, const vector<T>& rhs)
{
  return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
}

template <class T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs)
{
  return !(lhs == rhs);
}

template <class T>
bool operator>(const vector<T>& lhs, const vector<T>& rhs)
{
  return rhs < lhs;
}

template <class T>
bool operator<=(const vector<T>& lhs, const vector<T>& rhs)
{
  return !(rhs < lhs);
}

template <class T>
bool operator>=(const vector<T>& lhs, const vector<T>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T>
void swap(vector<T>& lhs, vector<T>& rhs)
{
  lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_VECTOR_H_

