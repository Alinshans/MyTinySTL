#ifndef MYTINYSTL_LIST_H_
#define MYTINYSTL_LIST_H_

// 这个头文件包含了一个模板类 list
// list : 双向链表

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"

#include <initializer_list>

namespace mystl
{

template <class T> struct __list_node;
template <class T> struct __list_node_base;

template <class T>
struct __node_traits
{
  typedef __list_node_base<T>* base_type;
  typedef __list_node<T>*      link_type;
};

// list 的节点结构

template <class T>
struct __list_node_base
{
  typedef typename __node_traits<T>::base_type base_type;
  typedef typename __node_traits<T>::link_type link_type;

  base_type prev;  // 前一节点
  base_type next;  // 下一节点

  __list_node_base() { unlink(); }

  link_type as_node()
  {
    return static_cast<link_type>(_self());
  }

  void unlink()
  {
    prev = next = _self();
  }

  base_type _self()
  {
    return static_cast<base_type>(&*this);
  }
};

template <class T>
struct __list_node : public __list_node_base<T>
{
  typedef typename __node_traits<T>::base_type base_type;
  typedef typename __node_traits<T>::link_type link_type;

  T value;  // 数据域

  __list_node() = default;
  __list_node(const T& v)
    :value(v)
  {
  }
  __list_node(T&& v)
    :value(mystl::move(v))
  {
  }

  base_type as_base()
  {
    return static_cast<base_type>(&*this);
  }
  link_type _self()
  {
    return static_cast<link_type>(&*this);
  }
};

// list 的迭代器设计
template <class T>
struct __list_iterator : public bidirectional_iterator<T>
{
  typedef T                                    value_type;
  typedef T*                                   pointer;
  typedef T&                                   reference;
  typedef typename __node_traits<T>::base_type base_type;
  typedef typename __node_traits<T>::link_type link_type;
  typedef __list_iterator<T>                   self;

  base_type node_;  // 指向当前节点

  // 构造函数
  __list_iterator() noexcept
    :node_() {}
  __list_iterator(base_type x) noexcept
    :node_(x) {}
  __list_iterator(link_type x) noexcept
    :node_(x->as_base()) {}
  __list_iterator(const __list_iterator& rhs) noexcept
    :node_(rhs.node_) {}

  // 重载操作符
  reference operator*()  const { return node_->as_node()->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    node_ = node_->next;
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
    node_ = node_->prev;
    return *this;
  }
  self operator--(int)
  {
    self tmp = *this;
    --*this;
    return tmp;
  }

  // 重载比较操作符
  bool operator==(const self& rhs) const { return node_ == rhs.node_; }
  bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
};

template <class T>
struct __list_const_iterator : public bidirectional_iterator<T>
{
  typedef T                                    value_type;
  typedef const T*                             pointer;
  typedef const T&                             reference;
  typedef typename __node_traits<T>::base_type base_type;
  typedef typename __node_traits<T>::link_type link_type;
  typedef __list_const_iterator<T>             self;

  base_type node_;

  __list_const_iterator() noexcept
    :node_(nullptr) {}
  __list_const_iterator(base_type x) noexcept
    :node_(x) {}
  __list_const_iterator(link_type x) noexcept
    :node_(x->as_base()) {}
  __list_const_iterator(const __list_iterator<T>& rhs) noexcept
    :node_(rhs.node_) {}
  __list_const_iterator(const __list_const_iterator& rhs) noexcept
    :node_(rhs.node_) {}

  reference operator*()  const { return node_->as_node()->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    node_ = node_->next;
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
    node_ = node_->prev;
    return *this;
  }
  self operator--(int)
  {
    self tmp = *this;
    --*this;
    return tmp;
  }

  // 重载比较操作符
  bool operator==(const self& rhs) const { return node_ == rhs.node_; }
  bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
};

// 模板类: list
// 模板参数 T 代表数据类型
template <class T>
class list
{
public:
  // list 的嵌套型别定义
  typedef mystl::allocator<T>                      allocator_type;
  typedef mystl::allocator<T>                      data_allocator;
  typedef mystl::allocator<__list_node_base<T>>    base_allocator;
  typedef mystl::allocator<__list_node<T>>         node_allocator;

  typedef typename allocator_type::value_type      value_type;
  typedef typename allocator_type::pointer         pointer;
  typedef typename allocator_type::const_pointer   const_pointer;
  typedef typename allocator_type::reference       reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type       size_type;
  typedef typename allocator_type::difference_type difference_type;

  typedef __list_iterator<T>                       iterator;
  typedef __list_const_iterator<T>                 const_iterator;
  typedef mystl::reverse_iterator<iterator>        reverse_iterator;
  typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

  typedef typename __node_traits<T>::base_type     base_type;
  typedef typename __node_traits<T>::link_type     link_type;

  allocator_type get_allocator() { return node_allocator(); }

private:
  base_type node_;  // 指向末尾节点

public:
  // 构造、复制、移动、析构函数
  list() { node_ = __get_base_node(); node_->unlink(); }

  explicit list(size_type n) 
  { __fill_initialize(n, value_type()); }

  list(size_type n, const T& value)
  { __fill_initialize(n, value); }

  template <class IIter>
  list(IIter first, IIter last)
  { __list_initialize(first, last, typename __is_integer<
                      IIter>::is_integer()); }

  list(std::initializer_list<T> ilist)
  { __range_initialize(ilist.begin(), ilist.end()); }

  list(const list& rhs)
  { __range_initialize(rhs.cbegin(), rhs.cend()); }

  list(list&& rhs)
  {
    node_ = rhs.node_;
    rhs.node_ = __get_base_node();
    rhs.node_->unlink();
  }

  list& operator=(const list& rhs)
  {
    if (this != &rhs)
    {
      assign(rhs.begin(), rhs.end());
    }
    return *this;
  }

  list& operator=(list&& rhs)
  {
    if (this != &rhs)
    {
      clear();
      node_ = rhs.node_;
      rhs.node_ = __get_base_node();
      rhs.node_->unlink();
    }
    return *this;
  }

  list& operator=(std::initializer_list<T> ilist)
  {
    list<T> tmp(ilist.begin(), ilist.end());
    *this = mystl::move(tmp);
    return *this;
  }

  ~list()
  {
    if (node_->next != node_)
      clear();
  }

public:
  // 迭代器相关操作
  iterator               begin()         noexcept
  { return node_->next; }
  const_iterator         begin()   const noexcept
  { return node_->next; }
  iterator               end()           noexcept 
  { return node_; }
  const_iterator         end()     const noexcept
  { return node_; }

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
  bool      empty()    const { return node_->next == node_; }
  size_type size()     const { return static_cast<size_type>(distance(begin(), end())); }
  size_type max_size() const { return static_cast<size_type>(-1); }

  // 访问元素相关操作
  reference       front() 
  { assert(!empty()); return *begin(); }

  const_reference front() const 
  { assert(!empty()); return *begin(); }

  reference       back() 
  { assert(!empty()); return *(--end()); }

  const_reference back()  const 
  { assert(!empty()); return *(--end()); }

  // 调整容器相关操作

  // assign
  void     assign(size_type n) 
  { __fill_assign(n, value_type()); }

  void     assign(size_type n, const value_type& value) 
  { __fill_assign(n, value); }

  template <class IIter>
  void     assign(IIter first, IIter last)
  { __assign_dispatch(first, last, typename __is_integer<IIter>::is_integer()); }

  void     assign(std::initializer_list<T> ilist)
  { __assign_dispatch(ilist.begin(), ilist.end(), __false_type()); }

  // insert
  iterator insert(const_iterator pos) 
  { 
    return insert(pos, value_type()); 
  }

  iterator insert(const_iterator pos, const value_type& value)
  {
    auto link_node = __create_node(value);
    return __link(pos, link_node->as_base());
  }

  iterator insert(const_iterator pos, value_type&& value)
  {
    auto link_node = __create_node(mystl::move(value));
    return __link(pos, link_node->as_base());
  }

  iterator insert(const_iterator pos, size_type n, const value_type& value)
  { 
    return __fill_insert(pos, n, value); 
  }

  template <class IIter>
  iterator insert(const_iterator pos, IIter first, IIter last)
  { 
    return __insert_dispatch(pos, first, last, 
                             typename __is_integer<IIter>::is_integer()); 
  }

  iterator insert(const_iterator pos, std::initializer_list<T> ilist)
  { 
    return insert(pos, ilist.begin(), ilist.end());
  }

  // push_front / push_back
  void     push_front(const value_type& value)
  {
    auto link_node = __create_node(value);
    __link_nodes_at_front(link_node->as_base(), link_node->as_base());
  }

  void     push_front(value_type&& value)
  {
    auto link_node = __create_node(mystl::move(value));
    __link_nodes_at_front(link_node->as_base(), link_node->as_base());
  }

  void     push_back(const value_type& value)
  {
    auto link_node = __create_node(value);
    __link_nodes_at_back(link_node->as_base(), link_node->as_base());
  }

  void     push_back(value_type&& value)
  {
    auto link_node = __create_node(mystl::move(value));
    __link_nodes_at_back(link_node->as_base(), link_node->as_base());
  }

  // emplace_front / emplace_back / emplace
  template <class ...Args>
  void     emplace_front(Args&& ...args)
  {
    auto link_node = __create_node(mystl::forward<Args>(args)...);
    __link_nodes_at_front(link_node->as_base(), link_node->as_base());
  }

  template <class ...Args>
  void     emplace_back(Args&& ...args)
  {
    auto link_node = __create_node(mystl::forward<Args>(args)...);
    __link_nodes_at_back(link_node->as_base(), link_node->as_base());
  }

  template <class ...Args>
  iterator emplace(const_iterator pos, Args&& ...args)
  {
    auto link_node = __create_node(mystl::forward<Args>(args)...);
    __link_nodes(pos.node_, link_node->as_base(), link_node->as_base());
    return iterator(link_node);
  }

  // pop_front / pop_back
  void     pop_front() 
  {
    auto n = node_->next;
    __unlink_nodes(n, n);
    __destroy_node(n->as_node());
  }

  void     pop_back() 
  { 
    auto n = node_->prev;
    __unlink_nodes(n, n);
    __destroy_node(n->as_node());
  }

  // erase / clear
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  void     clear();

  // resize
  void     resize(size_type new_size) { resize(new_size, value_type()); }
  void     resize(size_type new_size, const value_type& value);

  void     swap(list& rhs) { mystl::swap(node_, rhs.node_); }

  // list 相关操作

  void splice(const_iterator pos, list& other);
  void splice(const_iterator pos, list& other, const_iterator it);
  void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

  void remove(const value_type& value)
  { remove_if([&](const value_type& v) {return v == value; }); }
  template <class UnaryPredicate>
  void remove_if(UnaryPredicate pred);

  void unique()
  { unique(mystl::equal_to<T>()); }
  template <class BinaryPredicate>
  void unique(BinaryPredicate pred);

  void merge(list& x)
  { merge(x, mystl::less<T>()); }
  template <class Compare>
  void merge(list& x, Compare comp);

  void sort()
  { __sort(begin(), end(), size(), mystl::less<T>()); }
  template <class Compared>
  void sort(Compared comp)
  { __sort(begin(), end(), size(), comp); }

  void reverse();

private:
  // helper functions

  // create / destroy node
  base_type __get_base_node();
  link_type __get_node();
  template <class ...Args>
  link_type __create_node(Args&& ...agrs);
  void      __destroy_node(link_type p);

  // initialize
  template <class Integer>
  void      __list_initialize(Integer n, const value_type& x, __true_type);
  template <class Iter>
  void      __list_initialize(Iter first, Iter last, __false_type);
  void      __fill_initialize(size_type n, const value_type& x);
  template <class IIter>
  void      __range_initialize(IIter first, IIter last);

  // link / unlink
  iterator  __link(const_iterator pos, base_type node);
  void      __link_nodes(base_type p, base_type first, base_type last);
  void      __link_nodes_at_front(base_type first, base_type last);
  void      __link_nodes_at_back(base_type first, base_type last);
  void      __unlink_nodes(base_type f, base_type l);

  // assign
  void      __fill_assign(size_type n, const value_type& value);
  template <class Integer>
  void      __assign_dispatch(Integer n, const value_type& value, __true_type);
  template <class IIter>
  void      __assign_dispatch(IIter first, IIter last, __false_type);

  // insert
  iterator  __fill_insert(const_iterator pos, size_type n, const value_type& value);
  template <class Integer>
  iterator  __insert_dispatch(const_iterator pos, Integer n, const value_type& value, __true_type);
  template <class IIter>
  iterator  __insert_dispatch(const_iterator pos, IIter first, IIter last, __false_type);

  // sort
  template <class Compared>
  iterator  __sort(iterator first, iterator last, size_type n, Compared comp);

};

/*****************************************************************************************/

// 删除 pos 处的元素
template <class T>
typename list<T>::iterator 
list<T>::erase(const_iterator pos)
{
  assert(pos != cend());
  auto n = pos.node_;
  auto next = n->next;
  __unlink_nodes(n, n);
  __destroy_node(n->as_node());
  return iterator(next);
}

// 删除 [first, last) 内的元素
template <class T>
typename list<T>::iterator 
list<T>::erase(const_iterator first, const_iterator last)
{
  if (first != last)
  {
    __unlink_nodes(first.node_, last.node_->prev);
    while (first != last)
    {
      auto cur = first.node_;
      ++first;
      __destroy_node(cur->as_node());
    }

  }
  return iterator(last.node_);
}

// 清空 list
template <class T>
void list<T>::clear()
{
  if (node_->next != node_)
  {
    auto cur = node_->next;
    node_->unlink();
    for (base_type next; cur != node_; cur = next)
    {
      next = cur->next;
      __destroy_node(cur->as_node());
    }
  }
}

// 重置容器大小
template <class T>
void list<T>::resize(size_type new_size, const value_type& value)
{
  auto i = begin();
  size_type len = 0;
  while (i != end() && len < new_size)
  {
    ++i;
    ++len;
  }
  if (len == new_size)
  {
    erase(i, node_);
  }
  else
  {
    insert(node_, new_size - len, value);
  }
}

// 将 list x 接合于 pos 之前
template <class T>
void list<T>::splice(const_iterator pos, list<T>& x)
{
  assert(this != &x);
  if (!x.empty())
  {
    auto f = x.node_->next;
    auto l = x.node_->prev;
    x.__unlink_nodes(f, l);
    __link_nodes(pos.node_, f, l);
    x.node_->unlink();
  }
}

// 将 it 所指的节点接合于 pos 之前
template <class T>
void list<T>::splice(const_iterator pos, list& x, const_iterator it)
{
  if (pos.node_ != it.node_ && pos.node_ != it.node_->next)
  {
    auto f = it.node_;
    x.__unlink_nodes(f, f);
    __link_nodes(pos.node_, f, f);
  }
}

// 将 list x 的 [first, last) 内的节点接合于 pos 之前
template <class T>
void list<T>::splice(const_iterator pos, list& x, 
                     const_iterator first, const_iterator last)
{
  if (first != last && this != &x)
  {
    auto f = first.node_;
    auto l = last.node_->prev;
    x.__unlink_nodes(f, l);
    __link_nodes(pos.node_, f, l);
  }
}

// 将另一元操作 pred 为 true 的所有元素移除
template <class T>
template <class UnaryPredicate>
void list<T>::remove_if(UnaryPredicate pred)
{
  auto f = begin();
  auto l = end();
  for (auto next = f; f != l; f = next)
  {
    ++next;
    if (pred(*f))
    {
      erase(f);
    }
  }
}

// 移除 list 中满足 pred 为 true 重复元素
template <class T>
template <class BinaryPredicate>
void list<T>::unique(BinaryPredicate pred)
{
  auto i = begin();
  auto e = end();
  auto j = i;
  ++j;
  while (j != e)
  {
    if (pred(*i, *j))
    {
      erase(j);
    }
    else
    {
      i = j;
    }
    j = i;
    ++j;
  }
}

// 与另一个 list 合并，按照 comp 为 true 的顺序
template <class T>
template <class Compare>
void list<T>::merge(list& x, Compare comp)
{
  if (this != &x)
  {
    auto f1 = begin();
    auto l1 = end();
    auto f2 = x.begin();
    auto l2 = x.end();
    while (f1 != l1 && f2 != l2)
    {
      if (comp(*f2, *f1))
      {
        auto n = f2;
        ++n;
        for (; n != l2 && comp(*n, *f1); ++n)
          ;
        auto f = f2.node_;
        auto l = n.node_->prev;
        f2 = n;
        x.__unlink_nodes(f, l);
        __link_nodes(f1.node_, f, l);
        ++f1;
      }
      else
      {
        ++f1;
      }
    }
    splice(l1, x);
  }
}

// 将 list 反转
template <class T>
void list<T>::reverse()
{
  if (node_->next == node_ || node_->next->next == node_)
  {  // size() <= 1
    return;
  }
  auto i = begin();
  auto e = end();
  while (i.node_ != e.node_)
  {
    mystl::swap(i.node_->prev, i.node_->next);
    i.node_ = i.node_->prev;
  }
  mystl::swap(e.node_->prev, e.node_->next);
}

// 获取基层节点
template <class T>
typename list<T>::base_type 
list<T>::__get_base_node()
{
  return base_allocator::allocate(1);
}

// 获取节点
template <class T>
typename list<T>::link_type 
list<T>::__get_node()
{
  return node_allocator::allocate(1);
}

// 创建结点
template <class T>
template <class ...Args>
typename list<T>::link_type 
list<T>::__create_node(Args&& ...args)
{
  auto p = __get_node();
  try
  {
    data_allocator::construct(&p->value, mystl::forward<Args>(args)...);
  }
  catch (...)
  {
    data_allocator::destroy(&p->value);
    node_allocator::deallocate(p);
    throw;
  }
  return p;
}

// 销毁结点
template <class T>
void list<T>::__destroy_node(link_type p)
{
  data_allocator::destroy(&p->value);
  node_allocator::deallocate(p);
}

// 初始化 list
template <class T>
template <class Integer>
void list<T>::__list_initialize(Integer n, const value_type& x, __true_type)
{
  __fill_initialize(static_cast<size_type>(n), x);
}

template <class T>
template <class Iter>
void list<T>::__list_initialize(Iter first, Iter last, __false_type)
{
  __range_initialize(first, last);
}

template <class T>
void list<T>::__fill_initialize(size_type n, const value_type& x)
{
  node_ = __get_base_node();
  node_->unlink();
  for (; n > 0; --n)
  {
    push_back(x);
  }
}

template <class T>
template <class IIter>
void list<T>::__range_initialize(IIter first, IIter last)
{
  node_ = __get_base_node();
  node_->unlink();
  for (; first != last; ++first)
  {
    push_back(*first);
  }
}

// 在 pos 处连接一个节点
template<class T>
typename list<T>::iterator 
list<T>::__link(const_iterator pos, base_type link_node)
{
  if (pos == node_->next)
  {
    __link_nodes_at_front(link_node, link_node);
  }
  else if (pos == node_)
  {
    __link_nodes_at_back(link_node, link_node);
  }
  else
  {
    __link_nodes(pos.node_, link_node, link_node);
  }
  return iterator(link_node);
}

// 在 pos 处连接 [first, last] 的结点
template <class T>
void list<T>::__link_nodes(base_type pos, base_type first, base_type last)
{
  pos->prev->next = first;
  first->prev = pos->prev;
  pos->prev = last;
  last->next = pos;
}

// 在头部连接 [first, last] 结点
template <class T>
void list<T>::__link_nodes_at_front(base_type first, base_type last)
{
  first->prev = node_;
  last->next = node_->next;
  last->next->prev = last;
  node_->next = first;
}

// 在尾部连接 [first, last] 结点
template <class T>
void list<T>::__link_nodes_at_back(base_type first, base_type last)
{
  last->next = node_;
  first->prev = node_->prev;
  first->prev->next = first;
  node_->prev = last;
}

// 与 [first, last] 的结点断开连接
template <class T>
void list<T>::__unlink_nodes(base_type first, base_type last)
{
  first->prev->next = last->next;
  last->next->prev = first->prev;
}

// 填充元素
template <class T>
void list<T>::__fill_assign(size_type n, const value_type& value)
{
  auto i = begin();
  auto e = end();
  for (; n > 0 && i != e; --n, ++i)
  {
    *i = value;
  }
  if (n > 0)
  {
    insert(e, n, value);
  }
  else
  {
    erase(i, e);
  }
}

// 赋值元素
template <class T>
template <class Integer>
void list<T>::__assign_dispatch(Integer n, const value_type& value, __true_type)
{
  __fill_assign(static_cast<size_type>(n), value);
}

template <class T>
template <class IIter>
void list<T>::__assign_dispatch(IIter f2, IIter l2, __false_type)
{
  auto f1 = begin();
  auto l1 = end();
  for (; f1 != l1 && f2 != l2; ++f1, ++f2)
  {
    *f1 = *f2;
  }
  if (f2 == l2)
  {
    erase(f1, l1);
  }
  else
  {
    insert(l1, f2, l2);
  }
}

// 插入 n 个元素
template <class T>
typename list<T>::iterator 
list<T>::__fill_insert(const_iterator pos, size_type n, const value_type& value)
{
  iterator r(pos.node_);
  if (n != 0)
  {
    auto node = __create_node(value);
    node->prev = nullptr;
    r = iterator(node);
    auto end = r;
    try
    {
      for (--n; n != 0; --n, ++end)
      {
        auto next = __create_node(value);
        end.node_->next = next->as_base();  // link node
        next->prev = end.node_;
      }
    }
    catch (...)
    {
      while (true)
      {
        auto prev = end.node_->prev;
        __destroy_node(end.node_->as_node());
        if (!prev)
          break;
        end = iterator(prev);
      }
      throw;
    }
    __link_nodes(pos.node_, r.node_, end.node_);
  }
  return r;
}

// 插入元素
template <class T>
template <class Integer>
typename list<T>::iterator 
list<T>::__insert_dispatch(const_iterator pos, Integer n, const value_type& value, __true_type)
{
  return __fill_insert(pos, static_cast<size_type>(n), value);
}

// 在 pos 处插入 [first, last) 的元素
template <class T>
template <class IIter>
typename list<T>::iterator 
list<T>::__insert_dispatch(const_iterator pos, IIter first, IIter last, __false_type)
{
  iterator r(pos.node_);
  if (first != last)
  {
    auto node = __create_node(*first);
    node->prev = nullptr;
    r = iterator(node);
    auto end = r;
    try
    {
      for (++first; first != last; ++first, ++end)
      {
        auto next = __create_node(*first);
        end.node_->next = next->as_base();  // link node
        next->prev = end.node_;
      }
    }
    catch (...)
    {
      while (true)
      {
        auto prev = end.node_->prev;
        __destroy_node(end.node_->as_node());
        if (!prev)
          break;
        end = iterator(prev);
      }
      throw;
    }
    __link_nodes(pos.node_, r.node_, end.node_);
  }
  return r;
}

// 对 list 进行归并排序，返回一个迭代器指向区间最小元素的位置
template <class T>
template<class Compared>
typename list<T>::iterator 
list<T>::__sort(iterator f1, iterator l2, size_type n, Compared comp)
{
  if (n < 2)
    return f1;

  if (n == 2)
  {
    if (comp(*--l2, *f1))
    {
      auto ln = l2.node_;
      __unlink_nodes(ln, ln);
      __link_nodes(f1.node_, ln, ln);
      return l2;
    }
    return f1;
  }

  auto n2 = n / 2;
  auto l1 = f1;
  mystl::advance(l1, n2);
  auto result = f1 = __sort(f1, l1, n2, comp);  // 前半段的最小位置
  auto f2 = l1 = __sort(l1, l2, n - n2, comp);  // 后半段的最小位置

  // 把较小的一段区间移到前面
  if (comp(*f2, *f1))
  {
    auto m = f2;
    ++m;
    for (; m != l2 && comp(*m, *f1); ++m)
      ;
    auto f = f2.node_;
    auto l = m.node_->prev;
    result = f2;
    l1 = f2 = m;
    __unlink_nodes(f, l);
    m = f1;
    ++m;
    __link_nodes(f1.node_, f, l);
    f1 = m;
  }
  else
  {
    ++f1;
  }

  // 合并两段有序区间
  while (f1 != l1 && f2 != l2)
  {
    if (comp(*f2, *f1))
    {
      auto m = f2;
      ++m;
      for (; m != l2 && comp(*m, *f1); ++m)
        ;
      auto f = f2.node_;
      auto l = m.node_->prev;
      if (l1 == f2)
        l1 = m;
      f2 = m;
      __unlink_nodes(f, l);
      m = f1;
      ++m;
      __link_nodes(f1.node_, f, l);
      f1 = m;
    }
    else
    {
      ++f1;
    }
  }
  return result;
}

// 重载比较操作符
template <class T>
bool operator==(const list<T>& lhs, const list<T>& rhs)
{
  auto f1 = lhs.cbegin();
  auto f2 = rhs.cbegin();
  auto l1 = lhs.cend();
  auto l2 = rhs.cend();
  for (; f1 != l1 && f2 != l2 && *f1 == *f2; ++f1, ++f2)
    ;
  return f1 == l1 && f2 == l2;
}

template <class T>
bool operator<(const list<T>& lhs, const list<T>& rhs)
{
  return mystl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <class T>
bool operator!=(const list<T>& lhs, const list<T>& rhs)
{
  return !(lhs == rhs);
}

template <class T>
bool operator>(const list<T>& lhs, const list<T>& rhs)
{
  return rhs < lhs;
}

template <class T>
bool operator<=(const list<T>& lhs, const list<T>& rhs)
{
  return !(rhs < lhs);
}

template <class T>
bool operator>=(const list<T>& lhs, const list<T>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T>
void swap(list<T>& lhs, list<T>& rhs)
{
  lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_LIST_H_

