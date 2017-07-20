#ifndef MYTINYSTL_DEQUE_H_
#define MYTINYSTL_DEQUE_H_

// 这个头文件包含了一个模板类 deque
// deque: 双端队列

#include "iterator.h"
#include "memory.h"
#include "util.h"

#include <cassert>

#include <initializer_list>

namespace mystl
{

// deque map 初始化的大小
#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 2
#endif

template <class T>
struct __deque_buf_size
{
  static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
};

// deque 的迭代器设计
template <class T, class Ref, class Ptr>
struct __deque_iterator : public iterator<random_access_iterator_tag, T>
{
  typedef __deque_iterator<T, T&, T*>             iterator;
  typedef __deque_iterator<T, const T&, const T*> const_iterator;
  typedef __deque_iterator                        self;

  typedef T            value_type;
  typedef T*           value_pointer;
  typedef T**          map_pointer;
  typedef Ptr          pointer;
  typedef Ref          reference;
  typedef size_t       size_type;
  typedef ptrdiff_t    difference_type;

  static const size_type buffer_size = __deque_buf_size<T>::value;

  // 迭代器所含成员数据
  value_pointer cur;    // 指向所在缓冲区的当前元素
  value_pointer first;  // 指向所在缓冲区的头部
  value_pointer last;   // 指向所在缓冲区的尾部
  map_pointer   node;   // 缓冲区所在节点

  // 构造、复制、移动函数
  __deque_iterator()
    :cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
  __deque_iterator(pointer v, map_pointer n)
    :cur(v), first(*n), last(*n + buffer_size), node(n) {}

  __deque_iterator(const iterator& rhs)
    :cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
  {
  }
  __deque_iterator(const const_iterator& rhs)
    :cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
  {
  }

  self& operator=(const iterator& rhs)
  {
    if (this != &rhs)
    {
      cur = rhs.cur;
      first = rhs.first;
      last = rhs.last;
      node = rhs.node;
    }
    return *this;
  }

  // 转到另一个缓冲区
  void set_node(map_pointer new_node)
  {
    node = new_node;
    first = *new_node;
    last = first + buffer_size;
  }

  // 重载运算符
  reference operator*()  const { return *cur; }
  pointer   operator->() const { return cur; }

  difference_type operator-(const self& x) const
  {
    return static_cast<difference_type>(buffer_size) * (node - x.node - 1)
      + (cur - first) + (x.last - x.cur);
  }

  self& operator++()
  {
    ++cur;
    if (cur == last)
    { // 如果到达缓冲区的尾
      set_node(node + 1);  // 跳到下一个缓冲区
      cur = first;
    }
    return *this;
  }
  self operator++(int)
  {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--()
  {
    if (cur == first)
    { // 如果到达缓冲区的头
      set_node(node - 1);  // 跳到前一个缓冲区
      cur = last;
    }
    --cur;
    return *this;
  }
  self operator--(int)
  {
    self tmp = *this;
    --*this;
    return tmp;
  }

  self& operator+=(difference_type n)
  {
    const auto offset = n + (cur - first);
    if (offset >= 0 && offset < static_cast<difference_type>(buffer_size))
    {  // 仍在当前缓冲区
      cur += n;
    }
    else
    {  // 要跳到其他的缓冲区
      const auto node_offset = offset > 0
        ? offset / static_cast<difference_type>(buffer_size)
        : -static_cast<difference_type>((-offset - 1) / buffer_size) - 1;
      set_node(node + node_offset);
      cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
    }
    return *this;
  }
  self operator+(difference_type n) const
  {
    self tmp = *this;
    return tmp += n;
  }
  self& operator-=(difference_type n)
  {
    return *this += -n;
  }
  self operator-(difference_type n) const
  {
    self tmp = *this;
    return tmp -= n;
  }

  reference operator[](difference_type n) const { return *(*this + n); }

  // 重载比较操作符
  bool operator==(const self& rhs) const { return cur == rhs.cur; }
  bool operator< (const self& rhs) const
  { return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node); }
  bool operator!=(const self& rhs) const { return !(*this == rhs); }
  bool operator> (const self& rhs) const { return rhs < *this; }
  bool operator<=(const self& rhs) const { return !(rhs < *this); }
  bool operator>=(const self& rhs) const { return !(*this < rhs); }
};

// 模板类 deque
// 参数一代表数据类型
template <class T>
class deque
{
public:
  // deque 的型别定义
  typedef mystl::allocator<T>                      allocator_type;
  typedef mystl::allocator<T>                      data_allocator;
  typedef mystl::allocator<T*>                     map_allocator;

  typedef typename allocator_type::value_type      value_type;
  typedef typename allocator_type::pointer         pointer;
  typedef typename allocator_type::const_pointer   const_pointer;
  typedef typename allocator_type::reference       reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type       size_type;
  typedef typename allocator_type::difference_type difference_type;
  typedef pointer*                                 map_pointer;
  typedef const_pointer*                           const_map_pointer;

  typedef __deque_iterator<T, T&, T*>              iterator;
  typedef __deque_iterator<T, const T&, const T*>  const_iterator;
  typedef mystl::reverse_iterator<iterator>        reverse_iterator;
  typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

  allocator_type get_allocator() { return allocator_type(); }

  static const size_type buffer_size = __deque_buf_size<T>::value;

private:
  // 用以下四个数据来表现一个 deque
  iterator       start_;       // 指向第一个节点
  iterator       finish_;      // 指向最后一个结点
  map_pointer    map_;         // 指向一块 map，map 中的每个元素都是一个指针，指向一个缓冲区
  size_type      map_size_;    // map 内指针的数目

public:
  // 构造、复制、移动、析构函数
  deque()
  { __fill_initialize(0, value_type()); }

  explicit deque(size_type n)
  { __fill_initialize(n, value_type()); }

  deque(size_type n, const value_type& value)
  { __fill_initialize(n, value); }

  template <class IIter>
  deque(IIter first, IIter last)
  {
    typedef typename __is_integer<IIter>::is_integer Integer;
    __initialize_dispatch(first, last, Integer());
  }

  deque(std::initializer_list<T> ilist)
  {
    __range_initialize(ilist.begin(), ilist.end(), mystl::forward_iterator_tag());
  }

  deque(const deque& rhs)
  {
    __map_initialize(rhs.size());
    mystl::uninitialized_copy(rhs.start_, rhs.finish_, start_);
  }
  deque(deque&& rhs) 
    :start_(mystl::move(rhs.start_)), finish_(mystl::move(rhs.finish_)),
    map_(rhs.map_), map_size_(rhs.map_size_)
  {
    rhs.map_ = nullptr;
    rhs.map_size_ = 0;
  }

  deque& operator=(const deque& rhs);
  deque& operator=(deque&& rhs);

  deque& operator=(std::initializer_list<T> ilist)
  {
    clear();
    __range_initialize(ilist.begin(), ilist.end(), mystl::forward_iterator_tag());
    return *this;
  }

  ~deque()
  {
    if (map_)
    {
      clear();
      data_allocator::deallocate(*start_.node, buffer_size);
      map_allocator::deallocate(map_, map_size_);
      map_ = nullptr;
    }
  }

public:
  // 迭代器相关操作
  iterator               begin()         noexcept
  { return start_; }
  const_iterator         begin()   const noexcept
  { return start_; }
  iterator               end()           noexcept
  { return finish_; }
  const_iterator         end()     const noexcept
  { return finish_; }

  reverse_iterator       rbegin()        noexcept
  { return reverse_iterator(end()); }
  const_reverse_iterator rbegin()  const noexcept
  { return reverse_iterator(end()); }
  reverse_iterator       rend()          noexcept
  { return reverse_iterator(begin()); }
  const_reverse_iterator rend()    const noexcept
  { return reverse_iterator(begin()); }

  const_iterator         cbegin()  const noexcept
  { return begin(); }
  const_iterator         cend()    const noexcept
  { return end(); }
  const_reverse_iterator crbegin() const noexcept
  { return rbegin(); }
  const_reverse_iterator crend()   const noexcept
  { return rend(); }

  // 容量相关操作
  bool      empty()    const noexcept  { return begin() == end(); }
  size_type size()     const noexcept  { return finish_ - start_; }
  size_type max_size() const noexcept  { return static_cast<size_type>(-1); }
  void      resize(size_type new_size) { resize(new_size, value_type()); }
  void      resize(size_type new_size, const value_type& value);
  void      shrink_to_fit() noexcept;

  // 访问元素相关操作 
  reference       operator[](size_type n)
  {
    assert(n < size());
    return start_[n];
  }
  const_reference operator[](size_type n) const
  {
    assert(n < size());
    return start_[n];
  }
  reference       at(size_type n) { return (*this)[n]; }
  const_reference at(size_type n) const { return (*this)[n]; }

  reference       front()
  {
    assert(!empty());
    return *begin();
  }
  const_reference front() const
  {
    assert(!empty());
    return *begin();
  }
  reference       back()
  {
    assert(!empty());
    return *(end() - 1);
  }
  const_reference back() const
  {
    assert(!empty());
    return *(end() - 1);
  }

  // 修改容器相关操作

  // assign
  void     assign(size_type n)
  { __fill_assign(n, value_type()); }
  void     assign(size_type n, const value_type& value)
  { __fill_assign(n, value); }
  template <class IIter>
  void     assign(IIter first, IIter last)
  {
    typedef typename __is_integer<IIter>::is_integer Integer;
    __assign_dispatch(first, last, Integer());
  }

  // push_back / push_front
  void     push_back(const value_type& value);
  void     push_front(const value_type& value);
  void     push_back(value_type&& value)  { emplace_back(mystl::move(value)); }
  void     push_front(value_type&& value) { emplace_front(mystl::move(value)); }

  // emplace_front / emplace_back / emplace
  template <class ...Args>
  void     emplace_front(Args&& ...args);
  template <class ...Args>
  void     emplace_back(Args&& ...args);
  template <class ...Args>
  iterator emplace(iterator pos, Args&& ...args);

  // pop_back / pop_front
  void     pop_back();
  void     pop_front();

  // insert
  iterator insert(iterator position) { return insert(position, value_type()); }
  iterator insert(iterator position, const value_type& value);
  void     insert(iterator position, size_type n, const value_type& value)
  { __fill_insert(position, n, value); }
  template <class IIter>
  void     insert(iterator position, IIter first, IIter last)
  {
    typedef typename __is_integer<IIter>::is_integer Integer;
    __insert_dispatch(position, first, last, Integer());
  }

  // erase /clear
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  void     clear();

  // swap
  void     swap(deque& rhs);

private:
  // helper functions

  // create node / destroy node
  void     __create_node(map_pointer nstart, map_pointer nfinish);
  void     __destroy_node(map_pointer nstart, map_pointer nfinish);

  // initialize
  void     __map_initialize(size_type nelem);
  void     __fill_initialize(size_type n, const value_type& value);
  template <class IIter>
  void     __range_initialize(IIter, IIter, input_iterator_tag);
  template <class FIter>
  void     __range_initialize(FIter, FIter, forward_iterator_tag);
  template <class Integer>
  void     __initialize_dispatch(Integer n, Integer value, __true_type);
  template <class IIter>
  void     __initialize_dispatch(IIter first, IIter last, __false_type);

  // assign
  void     __fill_assign(size_type n, const value_type& value);
  template <class IIter>
  void     __range_assign(IIter first, IIter last, input_iterator_tag);
  template <class FIter>
  void     __range_assign(FIter first, FIter last, forward_iterator_tag);
  template <class Integer>
  void     __assign_dispatch(Integer n, Integer value, __true_type);
  template <class IIter>
  void     __assign_dispatch(IIter first, IIter last, __false_type);
  

  // push_back / push_front
  template <class... Args>
  void     __push_back_aux(Args&& ...args);
  template <class... Args>
  void     __push_front_aux(Args&& ...args);

  // pop_back / pop_front
  void     __pop_back_aux();
  void     __pop_front_aux();
  // insert
  template <class... Args>
  iterator __insert_aux(iterator position, Args&& ...args);
  void     __fill_insert(iterator position, size_type n, const value_type& x);
  void     __fill_insert_aux(iterator position, size_type n, const value_type& x);
  template <class FIter>
  void     __fill_insert_aux(iterator, FIter, FIter, size_type);
  template <class Integer>
  void     __insert_dispatch(iterator, Integer, Integer, __true_type);
  template <class IIter>
  void     __insert_dispatch(iterator, IIter, IIter, __false_type);
  template <class IIter>
  void     __insert_dispatch_aux(iterator, IIter, IIter, input_iterator_tag);
  template <class FIter>
  void     __insert_dispatch_aux(iterator, FIter, FIter, forward_iterator_tag);

  // reallocate
  iterator __reserve_elements_at_back(size_type n);
  iterator __reserve_elements_at_front(size_type n);
  void     __reserve_map_at_back(size_type n);
  void     __reserve_map_at_front(size_type n);
  void     __reallocate_map(size_type node_to_add, bool add_at_front);

};

/*****************************************************************************************/

// 复制赋值运算符
template <class T>
deque<T>& deque<T>::operator=(const deque& rhs)
{
  if (this != &rhs)
  {
    const auto len = size();
    if (len >= rhs.size())
    {
      erase(mystl::copy(rhs.start_, rhs.finish_, start_), finish_);
    }
    else
    {
      iterator mid = rhs.begin() + static_cast<difference_type>(len);
      mystl::copy(rhs.start_, mid, start_);
      insert(finish_, mid, rhs.finish_);
    }
  }
  return *this;
}

// 移动赋值运算符
template <class T>
deque<T>& deque<T>::operator=(deque&& rhs)
{
  if (this != &rhs)
  {
    clear();

    start_ = mystl::move(rhs.start_);
    finish_ = mystl::move(rhs.finish_);
    map_ = rhs.map_;
    map_size_ = rhs.map_size_;

    rhs.map_ = nullptr;
    rhs.map_size_ = 0;
  }
  return *this;
}

// 重置容器大小
template <class T>
void deque<T>::resize(size_type new_size, const value_type& value)
{
  const auto len = size();
  if (new_size < len)
  {
    erase(start_ + new_size, finish_);
  }
  else
  {
    insert(finish_, new_size - len, value);
  }
}

// 减小容器容量
template <class T>
void deque<T>::shrink_to_fit() noexcept
{
  auto old_cap = map_size_ * buffer_size;
  auto new_cap = old_cap / 2;
  if (new_cap < buffer_size * DEQUE_MAP_INIT_SIZE)
    new_cap = buffer_size * DEQUE_MAP_INIT_SIZE;
  if ((empty() && new_cap <= old_cap) ||
    (size() <= new_cap && new_cap <= old_cap))
  {
    deque tmp(begin(), end());
    swap(tmp);
  }
}

// 在尾部插入元素
template <class T>
void deque<T>::push_back(const value_type& value)
{
  if (finish_.cur != finish_.last - 1)
  {
    data_allocator::construct(finish_.cur, value);
    ++finish_.cur;
  }
  else
  {
    __push_back_aux(value);
  }
}

// 在头部插入元素
template <class T>
void deque<T>::push_front(const value_type& value)
{
  if (start_.cur != start_.first)
  {
    data_allocator::construct(start_.cur - 1, value);
    --start_.cur;
  }
  else
  {
    __push_front_aux(value);
  }
}

// 在尾部就地构建元素
template <class T>
template <class ...Args>
void deque<T>::emplace_back(Args&& ...args)
{
  if (finish_.cur != finish_.last - 1)
  {
    data_allocator::construct(finish_.cur, mystl::forward<Args>(args)...);
    ++finish_.cur;
  }
  else
  {
    __push_back_aux(mystl::forward<Args>(args)...);
  }
}

// 在头部就地构建元素
template <class T>
template <class ...Args>
void deque<T>::emplace_front(Args&& ...args)
{
  if (start_.cur != start_.first)
  {
    data_allocator::construct(start_.cur - 1, mystl::forward<Args>(args)...);
    --start_.cur;
  }
  else
  {
    __push_front_aux(mystl::forward<Args>(args)...);
  }
}

// 在 pos 位置就地构建元素
template <class T>
template <class ...Args>
typename deque<T>::iterator deque<T>::emplace(iterator pos, Args&& ...args)
{
  if (pos.cur == start_.cur)
  {
    emplace_front(mystl::forward<Args>(args)...);
    return start_;
  }
  else if (pos.cur == finish_.cur)
  {
    emplace_back(mystl::forward<Args>(args)...);
    return finish_;
  }
  return __insert_aux(pos, mystl::forward<Args>(args)...);
}

// 弹出尾部元素
template <class T>
void deque<T>::pop_back()
{
  if (finish_.cur != finish_.first)
  {
    --finish_.cur;
    mystl::destroy(finish_.cur);
  }
  else
  {
    __pop_back_aux();
  }
}

// 弹出头部元素
template <class T>
void deque<T>::pop_front()
{
  if (start_.cur != start_.last - 1)
  {
    mystl::destroy(start_.cur);
    ++start_.cur;
  }
  else
  {
    __pop_front_aux();
  }
}

// 在 position 处插入元素
template <class T>
typename deque<T>::iterator
deque<T>::insert(iterator position, const value_type& value)
{
  if (position.cur == start_.cur)
  {
    push_front(value);
    return start_;
  }
  else if (position.cur == finish_.cur)
  {
    push_back(value);
    auto tmp = finish_;
    --tmp;
    return tmp;
  }
  else
  {
    return __insert_aux(position, value);
  }
}

// 删除 position 处的元素
template <class T>
typename deque<T>::iterator
deque<T>::erase(iterator position)
{
  auto next = position;
  ++next;
  const size_type elems_before = position - start_;
  if (elems_before < (size() >> 1))
  {
    mystl::copy_backward(start_, position, next);
    pop_front();
  }
  else
  {
    mystl::copy(next, finish_, position);
    pop_back();
  }
  return start_ + elems_before;
}

// 删除[first, last)上的元素
template <class T>
typename deque<T>::iterator
deque<T>::erase(iterator first, iterator last)
{
  if (first == start_ && last == finish_)
  {
    clear();
    return finish_;
  }
  else
  {
    auto len = last - first;
    const size_type elems_before = first - start_;
    if (elems_before < ((size() - len) >> 1))
    {
      mystl::copy_backward(start_, first, last);
      auto new_start = start_ + len;
      mystl::destroy(start_, new_start);
      __destroy_node(start_.node, new_start.node);
      start_ = new_start;
    }
    else
    {
      mystl::copy(last, finish_, first);
      auto new_finish = finish_ - len;
      mystl::destroy(new_finish, finish_);
      __destroy_node(new_finish.node + 1, finish_.node + 1);
      finish_ = new_finish;
    }
    return start_ + elems_before;
  }
}

// 清空 deque
template <class T>
void deque<T>::clear()
{
  // clear 会保留至少一个缓冲区
  for (map_pointer cur = start_.node + 1; cur < finish_.node; ++cur)
  {
    mystl::destroy(*cur, *cur + buffer_size);
    data_allocator::deallocate(*cur, buffer_size);
  }
  if (start_.node != finish_.node)
  {  // 有两个以上的缓冲区
    mystl::destroy(start_.cur, start_.last);
    mystl::destroy(finish_.first, finish_.cur);
    // 释放尾部的缓冲区，保留头部
    data_allocator::deallocate(finish_.first, buffer_size);
  }
  else
  {
    mystl::destroy(start_.cur, finish_.cur);
    // 保留唯一的缓冲区
  }
  finish_ = start_;
}

// 交换两个 deque 容器
template <class T>
void deque<T>::swap(deque& rhs)
{
  if (this != &rhs)
  {
    mystl::swap(start_, rhs.start_);
    mystl::swap(finish_, rhs.finish_);
    mystl::swap(map_, rhs.map_);
    mystl::swap(map_size_, rhs.map_size_);
  }
}

/*****************************************************************************************/
// helper function

// __create_node 函数
template <class T>
void deque<T>::
__create_node(map_pointer nstart, map_pointer nfinish)
{
  map_pointer cur;
  try
  {
    for (cur = nstart; cur <= nfinish; ++cur)
    {
      *cur = data_allocator::allocate(buffer_size);
    }
  }
  catch (...)
  {
    __destroy_node(nstart, nfinish);
  }
}

// __destroy_node 函数
template <class T>
void deque<T>::
__destroy_node(map_pointer nstart, map_pointer nfinish)
{
  for (map_pointer n = nstart; n < nfinish; ++n)
  {
    data_allocator::deallocate(*n, buffer_size);
  }
}

// __map_initialize 函数
template <class T>
void deque<T>::
__map_initialize(size_type nElem)
{
  const auto nNode = nElem / buffer_size + 1;  // 需要分配的缓冲区个数
  map_size_ = mystl::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE), nNode + 2);
  map_ = map_allocator::allocate(map_size_);
  // 让 nstart 和 nfinish 都指向 map_ 最中央的区域，方便向头尾扩充
  map_pointer nstart = map_ + (map_size_ - nNode) / 2;
  map_pointer nfinish = nstart + nNode - 1;
  try
  {
    __create_node(nstart, nfinish);
  }
  catch (...)
  {
    map_allocator::deallocate(map_, map_size_);
    map_ = nullptr;
    map_size_ = 0;
  }
  start_.set_node(nstart);
  finish_.set_node(nfinish);
  start_.cur = start_.first;
  finish_.cur = finish_.first + (nElem % buffer_size);
}

// __fill_initialize 函数
template <class T>
void deque<T>::
__fill_initialize(size_type n, const value_type& value)
{
  __map_initialize(n);
  map_pointer cur;
  try
  {
    for (cur = start_.node; cur < finish_.node; ++cur)
    {
      mystl::uninitialized_fill(*cur, *cur + buffer_size, value);
    }
    mystl::uninitialized_fill(finish_.first, finish_.cur, value);
  }
  catch (...)
  {
    mystl::destroy(start_, iterator(*cur, cur));
  }
}

// __range_initialize 函数
template <class T>
template <class IIter>
void deque<T>::
__range_initialize(IIter first, IIter last, input_iterator_tag)
{
  auto n = distance(first, last);
  __map_initialize(n);
  try
  {
    for (; first != last; ++first)
      push_back(*first);
  }
  catch (...)
  {
    clear();
  }
}

template <class T>
template <class FIter>
void deque<T>::
__range_initialize(FIter first, FIter last, forward_iterator_tag)
{
  auto n = distance(first, last);
  __map_initialize(n);
  map_pointer cur;
  try
  {
    for (cur = start_.node; cur < finish_.node; ++cur)
    {
      auto mid = first;
      advance(mid, buffer_size);
      mystl::uninitialized_copy(first, mid, *cur);
      first = mid;
    }
    mystl::uninitialized_copy(first, last, finish_.first);
  }
  catch (...)
  {
    mystl::destroy(start_, iterator(*cur, cur));
  }
}

// __initialize_dispatch 函数
template <class T>
template <class Integer>
void deque<T>::
__initialize_dispatch(Integer n, Integer value, __true_type)
{
  __fill_initialize(n, value);
}

template <class T>
template <class IIter>
void deque<T>::
__initialize_dispatch(IIter first, IIter last, __false_type)
{
  __range_initialize(first, last, iterator_category(first));
}

// __fill_assign 函数
template <class T>
void deque<T>::
__fill_assign(size_type n, const value_type& value)
{
  if (n > size())
  {
    mystl::fill(begin(), end(), value);
    insert(end(), n - size(), value);
  }
  else
  {
    erase(begin() + n, end());
    mystl::fill(begin(), end(), value);
  }
}

// __assign_dispatch 函数
template <class T>
template <class Integer>
void deque<T>::
__assign_dispatch(Integer n, Integer value, __true_type)
{
  __fill_assign(static_cast<size_type>(n), static_cast<value_type>(value));
}

template <class T>
template <class IIter>
void deque<T>::
__assign_dispatch(IIter first, IIter last, __false_type)
{
  __range_assign(first, last, iterator_category(first));
}

// __range_assign 函数
template <class T>
template <class IIter>
void deque<T>::
__range_assign(IIter first, IIter last, input_iterator_tag)
{
  auto first1 = begin();
  auto last1 = end();
  for (; first != last && first1 != last1; ++first, ++first1)
  {
    *first1 = *first;
  }
  if (first1 != last1)
  {
    erase(first1, last1);
  }
  else
  {
    insert(end(), first, last);
  }
}

template <class T>
template <class FIter>
void deque<T>::
__range_assign(FIter first, FIter last, forward_iterator_tag)
{
  auto len = distance(first, last);
  if (static_cast<size_type>(len) > size())
  {
    auto mid = first;
    advance(mid, size());
    mystl::copy(first, mid, start_);
    insert(finish_, mid, last);
  }
  else
  {
    erase(mystl::copy(first, last, start_), finish_);
  }
}

// __push_back_aux 函数
template <class T>
template <class... Args>
void deque<T>::
__push_back_aux(Args&& ...args)
{
  __reserve_map_at_back(1);
  *(finish_.node + 1) = data_allocator::allocate(buffer_size);
  try
  {
    data_allocator::construct(finish_.cur, mystl::forward<Args>(args)...);
    finish_.set_node(finish_.node + 1);
    finish_.cur = finish_.first;
  }
  catch (...)
  {
    data_allocator::deallocate(*(finish_.node + 1));
  }
}

// __push_front_aux 函数
template <class T>
template <class... Args>
void deque<T>::
__push_front_aux(Args&&... args)
{
  __reserve_map_at_front(1);
  *(start_.node - 1) = data_allocator::allocate(buffer_size);
  try
  {
    start_.set_node(start_.node - 1);
    start_.cur = start_.last - 1;
    data_allocator::construct(start_.cur, mystl::forward<Args>(args)...);
  }
  catch (...)
  {
    ++start_;
    data_allocator::deallocate(*(start_.node - 1));
  }
}

// __pop_back_aux 函数
template <class T>
void deque<T>::__pop_back_aux()
{
  data_allocator::deallocate(finish_.first);
  finish_.set_node(finish_.node - 1);
  finish_.cur = finish_.last - 1;
  mystl::destroy(finish_.cur);
}

// __pop_front_aux 函数
template <class T>
void deque<T>::__pop_front_aux()
{
  mystl::destroy(start_.cur);
  data_allocator::deallocate(start_.first);
  start_.set_node(start_.node + 1);
  start_.cur = start_.first;
}

// __insert_aux 函数
template <class T>
template <class... Args>
typename deque<T>::iterator
deque<T>::
__insert_aux(iterator position, Args&& ...args)
{
  const size_type elems_before = position - start_;
  value_type value_copy = value_type(mystl::forward<Args>(args)...);
  if (elems_before < (size() >> 1))
  {
    push_front(front());
    auto front1 = start_;
    ++front1;
    auto front2 = front1;
    ++front2;
    position = start_ + elems_before;
    auto pos1 = position;
    ++pos1;
    mystl::copy(front2, pos1, front1);
  }
  else
  {
    push_back(back());
    auto back1 = finish_;
    --back1;
    auto back2 = back1;
    --back2;
    position = start_ + elems_before;
    mystl::copy_backward(position, back2, back1);
  }
  *position = mystl::move(value_copy);
  return position;
}

// __fill_insert 函数
template <class T>
void deque<T>::
__fill_insert(iterator position, size_type n, const value_type& value)
{
  if (position.cur == start_.cur)
  {
    auto new_start = __reserve_elements_at_front(n);
    try
    {
      mystl::uninitialized_fill(new_start, start_, value);
      start_ = new_start;
    }
    catch (...)
    {
      __destroy_node(new_start.node, start_.node);
    }
  }
  else if (position.cur == finish_.cur)
  {
    auto new_finish = __reserve_elements_at_back(n);
    try
    {
      mystl::uninitialized_fill(finish_, new_finish, value);
      finish_ = new_finish;
    }
    catch (...)
    {
      __destroy_node(finish_.node + 1, new_finish.node + 1);
    }
  }
  else
  {
    __fill_insert_aux(position, n, value);
  }
}

// __fill_insert_aux 函数
template <class T>
void deque<T>::
__fill_insert_aux(iterator position, size_type n, const value_type& value)
{
  const auto elems_before = position - start_;
  auto len = size();
  auto value_copy = value;
  if (static_cast<size_type>(elems_before) < (len >> 1))
  {
    auto old_start = start_;
    auto new_start = __reserve_elements_at_front(n);
    position = start_ + elems_before;
    try
    {
      if (static_cast<size_type>(elems_before) >= n)
      {
        auto start_n = start_ + n;
        mystl::uninitialized_copy(start_, start_n, new_start);
        start_ = new_start;
        mystl::copy(start_n, position, old_start);
        mystl::fill(position - n, position, value_copy);
      }
      else
      {
        mystl::uninitialized_fill(mystl::
                                  uninitialized_copy(start_, position, new_start), start_, value_copy);
        start_ = new_start;
        mystl::fill(old_start, position, value_copy);
      }
    }
    catch (...)
    {
      __destroy_node(new_start.node, start_.node);
    }
  }
  else
  {
    auto old_finish = finish_;
    auto new_finish = __reserve_elements_at_back(n);
    const auto elems_after = static_cast<difference_type>(len) - elems_before;
    position = finish_ - elems_after;
    try
    {
      if (static_cast<size_type>(elems_after) > n)
      {
        auto finish_n = finish_ - n;
        mystl::uninitialized_copy(finish_n, finish_, finish_);
        finish_ = new_finish;
        mystl::copy_backward(position, finish_n, old_finish);
        mystl::fill(position, position + n, value_copy);
      }
      else
      {
        mystl::uninitialized_fill(finish_, position + n, value_copy);
        mystl::uninitialized_copy(position, finish_, position + n);
        finish_ = new_finish;
        mystl::fill(position, old_finish, value_copy);
      }
    }
    catch (...)
    {
      __destroy_node(finish_.node + 1, new_finish.node + 1);
    }
  }
}

template <class T>
template <class FIter>
void deque<T>::
__fill_insert_aux(iterator position, FIter first, FIter last, size_type n)
{
  const auto elems_before = position - start_;
  auto len = size();
  if (static_cast<size_type>(elems_before) < (len >> 1))
  {
    auto old_start = start_;
    auto new_start = __reserve_elements_at_front(n);
    position = start_ + elems_before;
    try
    {
      if (static_cast<size_type>(elems_before) >= n)
      {
        auto start_n = start_ + n;
        mystl::uninitialized_copy(start_, start_n, new_start);
        start_ = new_start;
        mystl::copy(start_n, position, old_start);
        mystl::copy(first, last, position - n);
      }
      else
      {
        auto mid = first;
        advance(mid, static_cast<difference_type>(n) - elems_before);
        mystl::uninitialized_copy(first, mid,
                                  mystl::uninitialized_copy(start_, position, new_start));
        start_ = new_start;
        mystl::copy(mid, last, old_start);
      }
    }
    catch (...)
    {
      __destroy_node(new_start.node, start_.node);
    }
  }
  else
  {
    auto old_finish = finish_;
    auto new_finish = __reserve_elements_at_back(n);
    const auto elems_after = static_cast<difference_type>(len) - elems_before;
    position = finish_ - elems_after;
    try
    {
      if (static_cast<size_type>(elems_after) > n)
      {
        auto finish_n = finish_ - n;
        mystl::uninitialized_copy(finish_n, finish_, finish_);
        finish_ = new_finish;
        mystl::copy_backward(position, finish_n, old_finish);
        mystl::copy(first, last, position);
      }
      else
      {
        auto mid = first;
        advance(mid, elems_after);
        mystl::uninitialized_copy(position, finish_,
                                  mystl::uninitialized_copy(mid, last, finish_));
        finish_ = new_finish;
        mystl::copy(first, mid, position);
      }
    }
    catch (...)
    {
      __destroy_node(finish_.node + 1, new_finish.node + 1);
    }
  }
}

// __insert_dispatch 函数
template <class T>
template <class Integer>
void deque<T>::
__insert_dispatch(iterator position, Integer n, Integer x, __true_type)
{
  __fill_insert(position, static_cast<size_type>(n), static_cast<T>(x));
}

template <class T>
template <class IIter>
void deque<T>::
__insert_dispatch(iterator position, IIter first, IIter last, __false_type)
{
  __insert_dispatch_aux(position, first, last, iterator_category(first));
}

// __insert_dispatch_aux 函数
template <class T>
template <class IIter>
void deque<T>::
__insert_dispatch_aux(iterator position, IIter first, IIter last, input_iterator_tag)
{
  mystl::copy(first, last, iterator(*this, position));
}

template <class T>
template <class FIter>
void deque<T>::
__insert_dispatch_aux(iterator position, FIter first, FIter last, forward_iterator_tag)
{
  auto n = distance(first, last);
  if (position.cur == start_.cur)
  {        //在头部插入
    iterator new_start = __reserve_elements_at_front(n);
    try
    {
      mystl::uninitialized_copy(first, last, new_start);
      start_ = new_start;
    }
    catch (...)
    {
      __destroy_node(new_start.node, start_.node);
    }
  }
  else if (position.cur == finish_.cur)
  {  //在尾部插入
    auto new_finish = __reserve_elements_at_back(n);
    try
    {
      mystl::uninitialized_copy(first, last, finish_);
      finish_ = new_finish;
    }
    catch (...)
    {
      __destroy_node(finish_.node + 1, new_finish.node + 1);
    }
  }
  else
  {
    __fill_insert_aux(position, first, last, n);
  }
}

// __reserve_elements_at_back 函数
template <class T>
typename deque<T>::iterator
deque<T>::__reserve_elements_at_back(size_type n)
{
  auto left = static_cast<size_type>(finish_.last - finish_.cur - 1);
  if (n > left)
  {
    auto new_node = (n - left + buffer_size - 1) / buffer_size;
    __reserve_map_at_back(new_node);
    size_type i;
    try
    {
      for (i = 1; i <= new_node; ++i)
        *(finish_.node + i) = data_allocator::allocate(buffer_size);
    }
    catch (...)
    {
      for (size_type j = 1; j < i; ++j)
        map_allocator::deallocate(finish_.node + j);
    }
  }
  return finish_ + n;
}

// __reserve_elements_at_front 函数
template <class T>
typename deque<T>::iterator
deque<T>::__reserve_elements_at_front(size_type n)
{
  auto left = static_cast<size_type>(start_.cur - start_.first);
  if (n > left)
  {
    auto new_node = (n - left + buffer_size - 1) / buffer_size;
    __reserve_map_at_front(new_node);
    size_type i;
    try
    {
      for (i = 1; i <= new_node; ++i)
        *(start_.node - i) = data_allocator::allocate(buffer_size);
    }
    catch (...)
    {
      for (size_type j = 1; j < i; ++j)
        map_allocator::deallocate(start_.node - j);
    }
  }
  return start_ - n;
}

// __reserve_map_at_back 函数
template <class T>
void deque<T>::__reserve_map_at_back(size_type n)
{
  if ((n + 1) > (map_size_ - static_cast<size_type>(finish_.node - map_)))
    __reallocate_map(n, false);
}

// __reserve_map_at_front 函数
template <class T>
void deque<T>::__reserve_map_at_front(size_type n)
{
  if (n > static_cast<size_type>(start_.node - map_))
    __reallocate_map(n, true);
}

// __reallocate_map 函数
template <class T>
void deque<T>::__reallocate_map(size_type node_to_add, bool add_at_front)
{
  size_type old_nodes_num = finish_.node - start_.node + 1;
  size_type new_nodes_num = old_nodes_num + node_to_add;
  map_pointer new_start;
  if (map_size_ > 2 * new_nodes_num)
  {
    new_start = map_ + ((map_size_ - new_nodes_num) / 2)
      + (add_at_front ? node_to_add : 0);
    if (new_start < start_.node)
    {
      mystl::copy(start_.node, finish_.node + 1, new_start);
    }
    else
    {
      mystl::copy_backward(start_.node, finish_.node + 1, new_start + old_nodes_num);
    }
  }
  else
  {
    auto new_map_size = map_size_ + mystl::max(map_size_, node_to_add) + 2;
    map_pointer new_map = map_allocator::allocate(new_map_size);
    new_start = new_map + ((new_map_size - new_nodes_num) / 2)
      + (add_at_front ? node_to_add : 0);
    mystl::copy(start_.node, finish_.node + 1, new_start);
    map_allocator::deallocate(map_, map_size_);
    map_ = new_map;
    map_size_ = new_map_size;
  }
  // 调整新的 start_ 和 finish_
  start_.set_node(new_start);
  finish_.set_node(new_start + old_nodes_num - 1);
}

// 重载比较操作符
template <class T>
bool operator==(const deque<T>& lhs, const deque<T>& rhs)
{
  return lhs.size() == rhs.size() && 
    mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T>
bool operator<(const deque<T>& lhs, const deque<T>& rhs)
{
  return mystl::lexicographical_compare(
    lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator!=(const deque<T>& lhs, const deque<T>& rhs)
{
  return !(lhs == rhs);
}

template <class T>
bool operator>(const deque<T>& lhs, const deque<T>& rhs)
{
  return rhs < lhs;
}

template <class T>
bool operator<=(const deque<T>& lhs, const deque<T>& rhs)
{
  return !(rhs < lhs);
}

template <class T>
bool operator>=(const deque<T>& lhs, const deque<T>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T>
void swap(deque<T>& lhs, deque<T>& rhs)
{
  lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_DEQUE_H_

