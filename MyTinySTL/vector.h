#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

// 这个头文件包含一个模板类 vector
// vector : 向量

#include "iterator.h"
#include "memory.h"

#include <cassert>

#include <initializer_list>

namespace mystl
{

#ifdef _MSC_VER
#ifdef max
#undef max
#endif // max
#ifdef min
#undef min
#endif // min
#endif // Undef max, min in MSVC

// 模板类: vector 
// 模板参数 T 代表类型
template <class T>
class vector
{
public:
  // vector 的嵌套型别定义
  typedef T                                        value_type;
  typedef mystl::alloc                             allocator_type;
  typedef value_type*                              pointer;
  typedef const value_type*                        const_pointer;
  typedef value_type&                              reference;
  typedef const value_type&                        const_reference;
  typedef size_t                                   size_type;
  typedef ptrdiff_t                                difference_type;

  typedef value_type*                              iterator;
  typedef const value_type*                        const_iterator;
  typedef mystl::reverse_iterator<iterator>        reverse_iterator;
  typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

  typedef mystl::allocator<T>                      data_allocator;

  allocator_type get_allocator() { return allocator_type(); }

private:
  iterator begin_;  // 表示目前使用空间的头部
  iterator end_;    // 表示目前使用空间的尾部
  iterator cap_;    // 表示目前储存空间的尾部

public:
  // 构造、复制、移动、析构函数
  vector()
  {
    begin_ = data_allocator::allocate(16);
    end_ = begin_;
    cap_ = begin_ + 16;
  }

  explicit vector(size_type n)
  { __fill_initialize(n, value_type()); }
  vector(size_type n, const value_type& value)
  { __fill_initialize(n, value); }

  template <class Iter>
  vector(Iter first, Iter last)
  {
    __vector_initialize(first, last, typename __is_integer<Iter>::is_integer());
  }

  vector(const vector& rhs)
  {
    __range_initialize(rhs.begin_, rhs.end_);
  }
  vector(vector&& rhs) :
    begin_(rhs.begin_),
    end_(rhs.end_),
    cap_(rhs.cap_)
  {
    rhs.begin_ = nullptr;
    rhs.end_ = nullptr;
    rhs.cap_ = nullptr;
  }

  vector(std::initializer_list<T> ilist)
  {
    __range_initialize(ilist.begin(), ilist.end());
  }

  vector& operator=(const vector& rhs);
  vector& operator=(vector&& rhs);
  vector& operator=(std::initializer_list<T> ilist)
  {
    __destroy_and_deallocate(begin_, end_, cap_ - begin_);
    __range_initialize(ilist.begin(), ilist.end());
    return *this;
  }

  ~vector() { __destroy_and_deallocate(begin_, end_, cap_ - begin_); }

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
    assert(n < size());
    return *(begin_ + n);
  }
  const_reference operator[](size_type n) const
  {
    assert(n < size());
    return *(begin_ + n);
  }
  reference at(size_type n)
  {
    return (*this)[n];
  }
  const_reference at(size_type n) const
  {
    return (*this)[n];
  }

  reference front()
  {
    assert(!empty());
    return *begin_;
  }
  const_reference front() const
  {
    assert(!empty());
    return *begin_;
  }
  reference back()
  {
    assert(!empty());
    return *(end_ - 1);
  }
  const_reference back() const
  {
    assert(!empty());
    return *(end_ - 1);
  }

  pointer       data()       noexcept { return begin_; }
  const_pointer data() const noexcept { return begin_; }

  // 修改容器相关操作
  void assign(size_type n)
  { __fill_assign(n, value_type()); }

  void assign(size_type n, const value_type& value)
  { __fill_assign(n, value); }

  template <class Iter>
  void assign(Iter first, Iter last)
  { __assign_dispatch(first, last, typename __is_integer<Iter>::is_integer()); }

  void assign(std::initializer_list<value_type> il)
  { assign(il.begin(), il.end()); }

  template <class... Args>
  void emplace(iterator pos, Args&& ...args);

  template <class... Args>
  void emplace_back(Args&& ...args);

  void push_back(const_reference value);
  void push_back(value_type&& value);

  void pop_back();

  iterator insert(iterator pos);
  iterator insert(iterator pos, const value_type& value);
  void     insert(iterator pos, size_type n, const value_type& value);
  template <class Iter>
  void     insert(iterator pos, Iter first, Iter last);

  iterator erase(iterator pos);
  iterator erase(iterator first, iterator last);
  void     clear() { erase(begin(), end()); }

  void     resize(size_type new_size) { return resize(new_size, value_type()); }
  void     resize(size_type new_size, const value_type& value);
  void     reverse() { mystl::reverse(begin(), end()); }
  void     swap(vector& rhs);

private:
  // helper functions

  // initialize
  template <class Integer>
  void __vector_initialize(Integer n, Integer value, __true_type);

  template <class Iter>
  void __vector_initialize(Iter first, Iter last, __false_type);

  void __fill_initialize(size_type n, const value_type& value);

  template <class Iter>
  void __range_initialize(Iter first, Iter last);

  // destroy and deallocate
  void __destroy_and_deallocate(iterator first, iterator last, size_type n);

  // calculate the growth size
  size_type __growth_size(size_type add_size);

  // reallocate
  template <class... Args>
  void __reallocate_and_emplace(iterator pos, Args&& ...args);

  void __reallocate_and_insert(iterator pos, const value_type& value);

  // assign
  template <class Integer>
  void __assign_dispatch(Integer n, Integer value, __true_type);

  template <class Iter>
  void __assign_dispatch(Iter first, Iter last, __false_type);

  void __fill_assign(size_type n, const value_type& value);

  template <class IIter>
  void __range_assign(IIter first, IIter last, input_iterator_tag);

  template <class FIter>
  void __range_assign(FIter first, FIter last, forward_iterator_tag);

  // insert
  template <class Integer>
  void __insert_dispatch(iterator pos, Integer n, Integer x, __true_type);

  template <class Iter>
  void __insert_dispatch(iterator pos, Iter first, Iter last, __false_type);

  void __fill_insert(iterator pos, size_type n, const value_type& value);

  template <class IIter>
  void __range_insert(iterator pos, IIter first, IIter last, input_iterator_tag);

  template <class FIter>
  void __range_insert(iterator pos, FIter first, FIter last, forward_iterator_tag);

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
    {  // 如果要赋值的 vector 大小超过原 vector 容量大小
      __destroy_and_deallocate(begin_, end_, cap_ - begin_);
      __range_initialize(rhs.begin(), rhs.end());
    }
    else if (size() >= len)
    {  // 如果原 vector 大小大于等于要赋值的 vector 大小
      auto i = mystl::copy(rhs.begin(), rhs.end(), begin());
      data_allocator::destroy(i, end_);
      end_ = begin_ + len;
    }
    else
    {  // 如果原 vector 大小小于要赋值的 vector 大小
      mystl::copy(rhs.begin(), rhs.begin() + size(), begin_);
      mystl::uninitialized_copy(rhs.begin() + size(), rhs.end(), end_);
      cap_ = end_ = begin_ + len;
    }
  }
  return *this;
}

// 移动赋值操作符
template <class T>
vector<T>& vector<T>::operator=(vector&& rhs)
{
  if (this != &rhs)
  {
    __destroy_and_deallocate(begin_, end_, cap_ - begin_);
    begin_ = rhs.begin_;
    end_ = rhs.end_;
    cap_ = rhs.cap_;
    rhs.begin_ = nullptr;
    rhs.end_ = nullptr;
    rhs.cap_ = nullptr;
  }
  return *this;
}

// 放弃多余的容量
template <class T>
void vector<T>::shrink_to_fit()
{
  if (end_ < cap_)
  {
    data_allocator::deallocate(end_ + 1, cap_ - end_);
    cap_ = end_;
  }
}

// 预留空间大小，当原容量小于要求大小时，才会重新分配
template <class T>
void vector<T>::reserve(size_type n)
{
  if (capacity() < n)
  {
    const auto old_size = size();
    auto tmp = data_allocator::allocate(n);
    mystl::uninitialized_move(begin_, end_, tmp);
    data_allocator::deallocate(begin_, cap_ - begin_);
    begin_ = tmp;
    end_ = tmp + old_size;
    cap_ = begin_ + n;
  }
}

// 在 pos 位置就地构造元素，避免额外的复制或移动开销
template <class T>
template <class ...Args>
void vector<T>::emplace(iterator pos, Args&& ...args)
{
  assert(pos >= begin_ && pos <= end_);
  if (end_ != cap_ && pos == end_)
  {
    data_allocator::construct(end_, std::forward<Args>(args)...);
    ++end_;
  }
  else if (end_ != cap_)
  {
    data_allocator::construct(end_, std::move(*(end_ - 1)));
    ++end_;
    mystl::copy_backward(pos, end_ - 2, end_ - 1);
    data_allocator::construct(pos, std::forward<Args>(args)...);
  }
  else
  {
    __reallocate_and_emplace(pos, std::forward<Args>(args)...);
  }
}

// 在尾部就地构造元素，避免额外的复制或移动开销
template <class T>
template <class ...Args>
void vector<T>::emplace_back(Args&& ...args)
{
  if (end_ < cap_)
  {
    data_allocator::construct(end_, std::forward<Args>(args)...);
    ++end_;
  }
  else
  {
    __reallocate_and_emplace(end_, std::forward<Args>(args)...);
  }
}

// 在尾部插入元素
template <class T>
void vector<T>::push_back(const_reference value)
{
  if (end_ != cap_)
  {
    data_allocator::construct(end_, value);
    ++end_;
  }
  else
  {
    __reallocate_and_insert(end_, value);
  }
}

template <class T>
void vector<T>::push_back(value_type&& value)
{
  emplace_back(std::move(value));
}

// 弹出尾部元素
template <class T>
void vector<T>::pop_back()
{
  assert(!empty());
  --end_;
  data_allocator::destroy(end_);
}

// 在 pos 位置插入元素                                     
template <class T>
typename vector<T>::iterator vector<T>::
insert(iterator pos)
{
  return insert(pos, value_type());
}

template <class T>
typename vector<T>::iterator vector<T>::
insert(iterator pos, const value_type& value)
{
  assert(pos >= begin_ && pos <= end_);
  const auto n = pos - begin();
  if (end_ != cap_ && pos == end())
  {
    data_allocator::construct(end_, value);
    ++end_;
  }
  else if (end_ != cap_)
  {
    data_allocator::construct(end_, *(end_ - 1));
    ++end_;
    auto value_copy = value;
    mystl::copy_backward(pos, end_ - 2, end_ - 1);
    *pos = std::move(value_copy);
  }
  else
  {
    __reallocate_and_insert(pos, value);
  }
  return begin() + n;
}

// 在 pos 位置开始插入 n 个元素
template <class T>
void vector<T>::insert(iterator pos, size_type n, const value_type& value)
{
  __fill_insert(pos, n, value);
}

// 在 pos 位置插入[first, last)内的元素
template <class T>
template <class Iter>
void vector<T>::insert(iterator pos, Iter first, Iter last)
{
  __insert_dispatch(pos, first, last, typename __is_integer<Iter>::is_integer());
}

// 删除 pos 位置上的元素
template <class T>
typename vector<T>::iterator
vector<T>::erase(iterator pos)
{
  assert(pos >= begin_ && pos <= end_);
  if (pos + 1 != end_)
  {
    mystl::copy(pos + 1, end_, pos);
  }
  --end_;
  data_allocator::destroy(end_);
  return pos;
}

// 删除[first, last)上的元素
template <class T>
typename vector<T>::iterator
vector<T>::erase(iterator first, iterator last)
{
  assert(first >= begin_ && last <= end_ && first <= last);
  auto i = mystl::copy(last, end_, first);
  data_allocator::destroy(i, end_);
  end_ = end_ - (last - first);
  return first;
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
void vector<T>::swap(vector<T>& rhs)
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

// __vector_initialize 函数
template <class T>
template <class Integer>
void vector<T>::
__vector_initialize(Integer n, Integer value, __true_type)
{
  __fill_initialize(static_cast<size_type>(n), static_cast<T>(value));
}

template <class T>
template <class Iter>
void vector<T>::
__vector_initialize(Iter first, Iter last, __false_type)
{
  __range_initialize(first, last);
}

// __fill_initialize 函数
template <class T>
void vector<T>::
__fill_initialize(size_type n, const value_type& value)
{
  const auto init_size = mystl::max(static_cast<size_type>(16), n);
  begin_ = data_allocator::allocate(init_size);
  end_ = mystl::uninitialized_fill_n(begin_, n, value);
  cap_ = begin_ + init_size;
}

// __range_initialize 函数
template <class T>
template <class Iter>
void vector<T>::
__range_initialize(Iter first, Iter last)
{
  const auto init_size = mystl::max(static_cast<size_type>(last - first),
                                    static_cast<size_type>(16));
  begin_ = data_allocator::allocate(static_cast<size_type>(init_size));
  end_ = mystl::uninitialized_copy(first, last, begin_);
  cap_ = begin_ + init_size;
}

// __destroy_and_deallocate 函数
template <class T>
void vector<T>::
__destroy_and_deallocate(iterator first, iterator last, size_type n)
{
  data_allocator::destroy(first, last);
  data_allocator::deallocate(first, n);
}

// __growth_size 函数
template <class T>
typename vector<T>::size_type vector<T>::
__growth_size(size_type add_size)
{
  const auto old_size = capacity();
  assert(old_size <= max_size() - add_size);
  if (old_size > max_size() - old_size / 2)
  {
    return old_size + add_size;
  }
  const auto new_size = old_size == 0
    ? mystl::max(add_size, static_cast<size_type>(16))
    : mystl::max(old_size + old_size / 2, old_size + add_size);
  return new_size;
}

// __assign_dispatch 函数
template <class T>
template <class Integer>
void vector<T>::
__assign_dispatch(Integer n, Integer value, __true_type)
{
  __fill_assign(static_cast<size_type>(n), static_cast<T>(value));
}

template <class T>
template <class Iter>
void vector<T>::
__assign_dispatch(Iter first, Iter last, __false_type)
{
  __range_assign(first, last, iterator_category(first));
}

// __fill_assign 函数
template <class T>
void vector<T>::
__fill_assign(size_type n, const value_type& value)
{
  if (n > capacity())
  {
    __destroy_and_deallocate(begin_, end_, cap_ - begin_);
    __fill_initialize(n, value);
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

// __range_assign 函数
template <class T>
template <class IIter>
void vector<T>::
__range_assign(IIter first, IIter last, input_iterator_tag)
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

template <class T>
template <class FIter>
void vector<T>::
__range_assign(FIter first, FIter last, forward_iterator_tag)
{
  auto len = static_cast<size_type>(distance(first, last));
  if (len > capacity())
  {  // 如果区间长度大于容器容量
    __destroy_and_deallocate(begin_, end_, cap_ - begin_);
    __range_initialize(first, last);
  }
  else if (size() >= len)
  {  // 如果区间长度小于等于容器大小
    auto new_end = mystl::copy(first, last, begin_);
    data_allocator::destroy(new_end, end_);
    end_ = new_end;
  }
  else
  {  // 如果区间长度大于容器大小并且小于容器容量
    auto mid = first;
    advance(mid, size());
    mystl::copy(first, mid, begin_);
    end_ = mystl::uninitialized_copy(mid, last, end_);
  }
}

// __reallocate_and_emplace 函数
template <class T>
template <class ...Args>
void vector<T>::
__reallocate_and_emplace(iterator pos, Args&& ...args)
{
  const auto new_size = __growth_size(1);
  auto new_begin = data_allocator::allocate(new_size);
  auto new_end = new_begin;
  try
  {
    new_end = mystl::uninitialized_move(begin_, pos, new_begin);
    data_allocator::construct(new_end, std::forward<Args>(args)...);
    ++new_end;
    if (pos != end_)
    {
      new_end = mystl::uninitialized_move(pos, end_, new_end);
    }
  }
  catch (...)
  {
    __destroy_and_deallocate(new_begin, new_end, new_size);
    throw;
  }
  data_allocator::deallocate(begin_, cap_ - begin_);
  begin_ = new_begin;
  end_ = new_end;
  cap_ = new_begin + new_size;
}

// __reallocate_and_insert 函数
template <class T>
void vector<T>::
__reallocate_and_insert(iterator pos, const value_type& value)
{
  const auto old_size = size();
  const auto new_size = __growth_size(1);
  auto new_begin = data_allocator::allocate(new_size);
  auto new_end = new_begin;
  try
  {
    new_end = mystl::uninitialized_move(begin_, pos, new_begin);
    data_allocator::construct(new_end, value);
    ++new_end;
    new_end = mystl::uninitialized_move(pos, end_, new_end);
  }
  catch (...)
  {
    __destroy_and_deallocate(new_begin, new_end, new_size);
    throw;
  }
  data_allocator::deallocate(begin_, cap_ - begin_);
  begin_ = new_begin;
  end_ = new_end;
}

// __insert_dispatch 函数
template <class T>
template <class Integer>
void vector<T>::
__insert_dispatch(iterator pos, Integer n, Integer x, __true_type)
{
  __fill_insert(pos, static_cast<size_type>(n), static_cast<T>(x));
}

template <class T>
template <class Iter>
void vector<T>::
__insert_dispatch(iterator pos, Iter first, Iter last, __false_type)
{
  __range_insert(pos, first, last, iterator_category(first));
}

// __fill_insert 函数
template <class T>
void vector<T>::
__fill_insert(iterator pos, size_type n, const value_type& value)
{
  if (n == 0)
    return;
  if (static_cast<size_type>(cap_ - end_) >= n)
  {  // 如果备用空间大于等于增加的空间
    auto value_copy = value;
    const auto after_elems = end_ - pos;
    auto old_end = end_;
    if (static_cast<size_type>(after_elems) > n)
    {
      mystl::uninitialized_copy(end_ - n, end_, end_);
      end_ += n;
      mystl::copy_backward(pos, old_end - n, old_end);
      mystl::fill_n(pos, n, value_copy);
    }
    else
    {
      mystl::uninitialized_fill_n(end_, n - after_elems, value_copy);
      end_ += n - after_elems;
      mystl::uninitialized_copy(pos, old_end, end_);
      end_ += after_elems;
      mystl::fill(pos, old_end, value_copy);
    }
  }
  else
  {  // 如果备用空间不足
    const auto new_size = __growth_size(n);
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    try
    {
      new_end = mystl::uninitialized_copy(begin_, pos, new_begin);
      new_end = mystl::uninitialized_fill_n(new_end, n, value);
      new_end = mystl::uninitialized_copy(pos, end_, new_end);
    }
    catch (...)
    {
      __destroy_and_deallocate(new_begin, new_end, new_size);
      throw;
    }
    __destroy_and_deallocate(begin_, end_, cap_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    cap_ = begin_ + new_size;
  }
}

// __range_insert 函数
template <class T>
template <class IIter>
void vector<T>::
__range_insert(iterator pos, IIter first, IIter last, input_iterator_tag)
{
  for (; first != last; ++first)
  {
    pos = insert(pos, *first);
    ++pos;
  }
}

template <class T>
template <class FIter>
void vector<T>::
__range_insert(iterator pos, FIter first, FIter last, forward_iterator_tag)
{
  if (first == last)
    return;
  const auto n = distance(first, last);
  if ((cap_ - end_) >= n)
  {  // 如果备用空间大小足够
    const auto after_elems = end_ - pos;
    auto old_end = end_;
    if (after_elems > n)
    {  // pos 后面的元素较多
      mystl::uninitialized_copy(end_ - n, end_, end_);
      end_ += n;
      mystl::copy_backward(pos, old_end - n, old_end);
      mystl::copy(first, last, pos);
    }
    else
    {  // pos 后面的元素较少
      auto mid = first;
      advance(mid, after_elems);
      mystl::uninitialized_copy(mid, last, end_);
      end_ += n - after_elems;
      mystl::uninitialized_copy(pos, old_end, end_);
      end_ += after_elems;
      mystl::copy(first, mid, pos);
    }
  }
  else
  {  // 备用空间不足
    const auto new_size = __growth_size(n);
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    try
    {
      new_end = mystl::uninitialized_copy(begin_, pos, new_begin);
      new_end = mystl::uninitialized_copy(first, last, new_end);
      new_end = mystl::uninitialized_copy(pos, end_, new_end);
    }
    catch (...)
    {
      __destroy_and_deallocate(new_begin, new_end, new_size);
      throw;
    }
    __destroy_and_deallocate(begin_, end_, cap_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    cap_ = begin_ + new_size;
  }
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

