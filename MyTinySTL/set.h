#ifndef MYTINYSTL_SET_H_
#define MYTINYSTL_SET_H_

// 这个头文件包含两个模板类 set 和 multiset
// set      : 集合，键值即实值，集合内元素会自动排序，键值不允许重复
// multiset : 集合，键值即实值，集合内元素会自动排序，键值允许重复

// notes:
//
// 异常保证：
// mystl::set<Key> / mystl::multiset<Key> 满足基本异常保证，对以下等函数做强异常安全保证：
//   * emplace
//   * emplace_hint
//   * insert

#include "rb_tree.h"

namespace mystl
{

// 模板类 set，键值不允许重复
// 参数一代表键值类型，参数二代表键值比较方式，缺省使用 mystl::less 
template <class Key, class Compare = mystl::less<Key>>
class set
{
public:
  typedef Key        key_type;
  typedef Key        value_type;
  typedef Compare    key_compare;
  typedef Compare    value_compare;

private:
  // 以 mystl::rb_tree 作为底层机制
  typedef mystl::rb_tree<value_type, key_compare>  base_type;
  base_type tree_;

public:
  // 使用 rb_tree 定义的型别
  typedef typename base_type::node_type              node_type;
  typedef typename base_type::const_pointer          pointer;
  typedef typename base_type::const_pointer          const_pointer;
  typedef typename base_type::const_reference        reference;
  typedef typename base_type::const_reference        const_reference;
  typedef typename base_type::const_iterator         iterator;
  typedef typename base_type::const_iterator         const_iterator;
  typedef typename base_type::const_reverse_iterator reverse_iterator;
  typedef typename base_type::const_reverse_iterator const_reverse_iterator;
  typedef typename base_type::size_type              size_type;
  typedef typename base_type::difference_type        difference_type;
  typedef typename base_type::allocator_type         allocator_type;

public:
  // 构造、复制、移动函数
  set() = default;

  template <class InputIterator>
  set(InputIterator first, InputIterator last) 
    :tree_() 
  { tree_.insert_unique(first, last); }
  set(std::initializer_list<value_type> ilist)
    :tree_()
  { tree_.insert_unique(ilist.begin(), ilist.end()); }

  set(const set& rhs) 
    :tree_(rhs.tree_)
  {
  }
  set(set&& rhs) noexcept
    :tree_(mystl::move(rhs.tree_))
  {
  }

  set& operator=(const set& rhs)
  {
    tree_ = rhs.tree_;
    return *this;
  }
  set& operator=(set&& rhs)
  { 
    tree_ = mystl::move(rhs.tree_); 
    return *this; 
  }
  set& operator=(std::initializer_list<value_type> ilist)
  {
    tree_.clear();
    tree_.insert_unique(ilist.begin(), ilist.end());
    return *this;
  }

  // 相关接口

  key_compare      key_comp()      const { return tree_.key_comp(); }
  value_compare    value_comp()    const { return tree_.key_comp(); }
  allocator_type   get_allocator() const { return tree_.get_allocator(); }

  // 迭代器相关

  iterator               begin()         noexcept
  { return tree_.begin(); }
  const_iterator         begin()   const noexcept
  { return tree_.begin(); }
  iterator               end()           noexcept
  { return tree_.end(); }
  const_iterator         end()     const noexcept
  { return tree_.end(); }

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

  // 容量相关
  bool                   empty()    const noexcept { return tree_.empty(); }
  size_type              size()     const noexcept { return tree_.size(); }
  size_type              max_size() const noexcept { return tree_.max_size(); }

  // 插入删除操作

  template <class ...Args>
  pair<iterator, bool> emplace(Args&& ...args)
  {
    return tree_.emplace_unique(mystl::forward<Args>(args)...);
  }

  template <class ...Args>
  iterator emplace_hint(iterator hint, Args&& ...args)
  {
    return tree_.emplace_unique_use_hint(hint, mystl::forward<Args>(args)...);
  }

  pair<iterator, bool> insert(const value_type& value)
  {
    return tree_.insert_unique(value);
  }
  pair<iterator, bool> insert(value_type&& value)
  {
    return tree_.insert_unique(mystl::move(value));
  }

  iterator insert(iterator hint, const value_type& value)
  {
    return tree_.insert_unique(hint, value);
  }
  iterator insert(iterator hint, value_type&& value)
  {
    return tree_.insert_unique(hint, mystl::move(value));
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last)
  {
    tree_.insert_unique(first, last);
  }

  void      erase(iterator position)             { tree_.erase(position); }
  size_type erase(const key_type& key)           { return tree_.erase_unique(key); }
  void      erase(iterator first, iterator last) { tree_.erase(first, last); }

  void      clear() { tree_.clear(); }

  // set 相关操作

  iterator       find(const key_type& key)              { return tree_.find(key); }
  const_iterator find(const key_type& key)        const { return tree_.find(key); }

  size_type      count(const key_type& key)       const { return tree_.count_unique(key); }

  iterator       lower_bound(const key_type& key)       { return tree_.lower_bound(key); }
  const_iterator lower_bound(const key_type& key) const { return tree_.lower_bound(key); }

  iterator       upper_bound(const key_type& key)       { return tree_.upper_bound(key); }
  const_iterator upper_bound(const key_type& key) const { return tree_.upper_bound(key); }

  pair<iterator, iterator>
    equal_range(const key_type& key)
  { return tree_.equal_range_unique(key); }

  pair<const_iterator, const_iterator>
    equal_range(const key_type& key) const
  { return tree_.equal_range_unique(key); }

  void swap(set& rhs) noexcept
  { tree_.swap(rhs.tree_); }

public:
  friend bool operator==(const set& lhs, const set& rhs) { return lhs.tree_ == rhs.tree_; }
  friend bool operator< (const set& lhs, const set& rhs) { return lhs.tree_ <  rhs.tree_; }
};

// 重载比较操作符
template <class Key, class Compare>
bool operator==(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs)
{
  return lhs == rhs;
}

template <class Key, class Compare>
bool operator<(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs)
{
  return lhs < rhs;
}

template <class Key, class Compare>
bool operator!=(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs)
{
  return !(lhs == rhs);
}

template <class Key, class Compare>
bool operator>(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs)
{
  return rhs < lhs;
}

template <class Key, class Compare>
bool operator<=(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs)
{
  return !(rhs < lhs);
}

template <class Key, class Compare>
bool operator>=(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class Key, class Compare>
void swap(set<Key, Compare>& lhs, set<Key, Compare>& rhs) noexcept
{
  lhs.swap(rhs);
}

/*****************************************************************************************/

// 模板类 multiset，键值允许重复
// 参数一代表键值类型，参数二代表键值比较方式，缺省使用 mystl::less 
template <class Key, class Compare = mystl::less<Key>>
class multiset
{
public:
  typedef Key        key_type;
  typedef Key        value_type;
  typedef Compare    key_compare;
  typedef Compare    value_compare;

private:
  // 以 mystl::rb_tree 作为底层机制
  typedef mystl::rb_tree<value_type, key_compare>  base_type;
  base_type tree_;  // 以 rb_tree 表现 multiset

public:
  // 使用 rb_tree 定义的型别
  typedef typename base_type::node_type              node_type;
  typedef typename base_type::const_pointer          pointer;
  typedef typename base_type::const_pointer          const_pointer;
  typedef typename base_type::const_reference        reference;
  typedef typename base_type::const_reference        const_reference;
  typedef typename base_type::const_iterator         iterator;
  typedef typename base_type::const_iterator         const_iterator;
  typedef typename base_type::const_reverse_iterator reverse_iterator;
  typedef typename base_type::const_reverse_iterator const_reverse_iterator;
  typedef typename base_type::size_type              size_type;
  typedef typename base_type::difference_type        difference_type;
  typedef typename base_type::allocator_type         allocator_type;

public:
  // 构造、复制、移动函数
  multiset() = default;

  template <class InputIterator>
  multiset(InputIterator first, InputIterator last) 
    :tree_() 
  { tree_.insert_multi(first, last); }
  multiset(std::initializer_list<value_type> ilist)
    :tree_() 
  { tree_.insert_multi(ilist.begin(), ilist.end()); }

  multiset(const multiset& rhs)
    :tree_(rhs.tree_)
  {
  }
  multiset(multiset&& rhs) noexcept
    :tree_(mystl::move(rhs.tree_))
  {
  }

  multiset& operator=(const multiset& rhs) 
  { 
    tree_ = rhs.tree_;
    return *this; 
  }
  multiset& operator=(multiset&& rhs)
  {
    tree_ = mystl::move(rhs.tree_);
    return *this; 
  }
  multiset& operator=(std::initializer_list<value_type> ilist)
  {
    tree_.clear();
    tree_.insert_multi(ilist.begin(), ilist.end());
    return *this;
  }

  // 相关接口

  key_compare      key_comp()      const { return tree_.key_comp(); }
  value_compare    value_comp()    const { return tree_.key_comp(); }
  allocator_type   get_allocator() const { return tree_.get_allocator(); }

  // 迭代器相关

  iterator               begin()         noexcept
  { return tree_.begin(); }
  const_iterator         begin()   const noexcept
  { return tree_.begin(); }
  iterator               end()           noexcept
  { return tree_.end(); }
  const_iterator         end()     const noexcept
  { return tree_.end(); }

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

  // 容量相关
  bool                   empty()    const noexcept { return tree_.empty(); }
  size_type              size()     const noexcept { return tree_.size(); }
  size_type              max_size() const noexcept { return tree_.max_size(); }

  // 插入删除操作

  template <class ...Args>
  iterator emplace(Args&& ...args)
  {
    return tree_.emplace_multi(mystl::forward<Args>(args)...);
  }

  template <class ...Args>
  iterator emplace_hint(iterator hint, Args&& ...args)
  {
    return tree_.emplace_multi_use_hint(hint, mystl::forward<Args>(args)...);
  }

  iterator insert(const value_type& value)
  {
    return tree_.insert_multi(value);
  }
  iterator insert(value_type&& value)
  {
    return tree_.insert_multi(mystl::move(value));
  }

  iterator insert(iterator hint, const value_type& value)
  {
    return tree_.insert_multi(hint, value);
  }
  iterator insert(iterator hint, value_type&& value)
  {
    return tree_.insert_multi(hint, mystl::move(value));
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last)
  {
    tree_.insert_multi(first, last);
  }

  void           erase(iterator position)             { tree_.erase(position); }
  size_type      erase(const key_type& key)           { return tree_.erase_multi(key); }
  void           erase(iterator first, iterator last) { tree_.erase(first, last); }

  void           clear() { tree_.clear(); }

  // multiset 相关操作

  iterator       find(const key_type& key)              { return tree_.find(key); }
  const_iterator find(const key_type& key)        const { return tree_.find(key); }

  size_type      count(const key_type& key)       const { return tree_.count_multi(key); }

  iterator       lower_bound(const key_type& key)       { return tree_.lower_bound(key); }
  const_iterator lower_bound(const key_type& key) const { return tree_.lower_bound(key); }

  iterator       upper_bound(const key_type& key)       { return tree_.upper_bound(key); }
  const_iterator upper_bound(const key_type& key) const { return tree_.upper_bound(key); }

  pair<iterator, iterator>
    equal_range(const key_type& key)
  { return tree_.equal_range_multi(key); }

  pair<const_iterator, const_iterator>
    equal_range(const key_type& key) const
  { return tree_.equal_range_multi(key); }

  void swap(multiset& rhs) noexcept
  { tree_.swap(rhs.tree_); }

public:
  friend bool operator==(const multiset& lhs, const multiset& rhs) { return lhs.tree_ == rhs.tree_; }
  friend bool operator< (const multiset& lhs, const multiset& rhs) { return lhs.tree_ <  rhs.tree_; }
};

// 重载比较操作符
template <class Key, class Compare>
bool operator==(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs)
{
  return lhs == rhs;
}

template <class Key, class Compare>
bool operator<(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs)
{
  return lhs < rhs;
}

template <class Key, class Compare>
bool operator!=(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs)
{
  return !(lhs == rhs);
}

template <class Key, class Compare>
bool operator>(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs)
{
  return rhs < lhs;
}

template <class Key, class Compare>
bool operator<=(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs)
{
  return !(rhs < lhs);
}

template <class Key, class Compare>
bool operator>=(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class Key, class Compare>
void swap(multiset<Key, Compare>& lhs, multiset<Key, Compare>& rhs) noexcept
{
  lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_SET_H_

