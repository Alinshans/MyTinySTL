#ifndef MYTINYSTL_UNORDERED_MAP_H_
#define MYTINYSTL_UNORDERED_MAP_H_

// 这个头文件包含两个模板类 unordered_map 和 unordered_multimap
// 功能与用法与 map 和 multimap 类似，不同的是使用 hashtable 作为底层机制

#include "hashtable.h"
#include "util.h"

namespace mystl
{

// 模板类 unordered_map
// 参数一代表键值类型，参数二代表实值类型，参数三代表哈希函数，缺省使用 mystl::hash
// 参数四代表键值比较方式，缺省使用 mystl::equal_to
// 使用方法与 map 类似，使用 hashtable 作为底层机制，所以 unordered_map 内的元素不会自动排序
template <class Key, class T, class Hash = mystl::hash<Key>, class EqualKey = mystl::equal_to<Key>>
  class unordered_map
{
private:
  // 使用 hashtable 作为底层机制
  typedef hashtable<mystl::pair<const Key, T>, Hash, EqualKey>  rep_type;
  rep_type ht_;

public:
  // 使用 hashtable 的型别
  typedef typename rep_type::key_type           key_type;
  typedef T                                     data_type;
  typedef T                                     mapped_type;
  typedef typename rep_type::value_type         value_type;
  typedef typename rep_type::hasher             hasher;
  typedef typename rep_type::key_equal          key_equal;

  typedef typename rep_type::size_type          size_type;
  typedef typename rep_type::difference_type    difference_type;
  typedef typename rep_type::pointer            pointer;
  typedef typename rep_type::const_pointer      const_pointer;
  typedef typename rep_type::reference          reference;
  typedef typename rep_type::const_reference    const_reference;

  typedef typename rep_type::iterator           iterator;
  typedef typename rep_type::const_iterator     const_iterator;
  typedef typename rep_type::allocator_type     allocator_type;

  hasher         hash_fcn()      const { return ht_.hash_fcn(); }
  key_equal      key_eq()        const { return ht_.key_eq(); }
  allocator_type get_allocator() const { return ht_.get_allocator(); }

public:
  // 构造、复制、移动函数
  unordered_map() :ht_(100, hasher(), key_equal()) {}  // 缺省使用大小为 100 的表格
  explicit unordered_map(size_type n) :ht_(n, hasher(), key_equal()) {}
  unordered_map(size_type n, const hasher& hf) :ht_(n, hf, key_equal()) {}
  unordered_map(size_type n, const hasher& hf, const key_equal& keq) :ht_(n, hf, keq) {}

  template <class InputIterator>
  unordered_map(InputIterator first, InputIterator last,
                const hasher& hf = hasher(),
                const key_equal& keq = key_equal())
    : ht_(distance(first, last), hf, keq)
  {
    ht_.insert_unique(first, last);
  }
  unordered_map(std::initializer_list<value_type> ilist,
                const hasher& hf = hasher(),
                const key_equal& keq = key_equal())
    :ht_(ilist.size(), hf, keq)
  {
    ht_.insert_unique(ilist.begin(), ilist.end());
  }

  unordered_map(const unordered_map& rhs) :ht_(rhs.ht_) {}
  unordered_map(unordered_map&& rhs) :ht_(mystl::move(rhs.ht_)) {}

  unordered_map& operator=(const unordered_map& rhs) { ht_ = rhs.ht_; return *this; }
  unordered_map& operator=(unordered_map&& rhs) { ht_ = mystl::move(rhs.ht_); return *this; }
  unordered_map& operator=(std::initializer_list<value_type> ilist)
  {
    ht_.clear();
    ht_.insert_unique(ilist.begin(), ilist.end());
    return *this;
  }

  // 相关接口
  iterator       begin() { return ht_.begin(); }
  const_iterator begin()    const { return ht_.begin(); }
  iterator       end() { return ht_.end(); }
  const_iterator end()      const { return ht_.end(); }

  bool           empty()    const { return ht_.empty(); }
  size_type      size()     const { return ht_.size(); }
  size_type      max_size() const { return ht_.max_size(); }

  pair<iterator, bool> insert(const value_type& value)
  {
    return ht_.insert_unique(value);
  }
  template <class InputIterator>
  void insert(InputIterator first, InputIterator last)
  {
    ht_.insert_unique(first, last);
  }
  pair<iterator, bool> insert_noresize(const value_type& value)
  {
    return ht_.insert_unique_noresize(value);
  }

  size_type erase(const key_type& key) { return ht_.erase(key); }
  void      erase(iterator it) { ht_.erase(it); }
  void      erase(iterator first, iterator last) { ht_.erase(first, last); }
  void      clear() { ht_.clear(); }

  data_type& operator[](const key_type& key)
  {
    return ht_.find_or_insert(value_type(key, T())).second;
  }

  // unordered_map 相关操作
  iterator       find(const key_type& key) { return ht_.find(key); }
  const_iterator find(const key_type& key)        const { return ht_.find(key); }
  size_type      count(const key_type& key)       const { return ht_.count(key); }
  pair<iterator, iterator>
    equal_range(const key_type& key) { return ht_.equal_range(key); }
  pair<const_iterator, const_iterator>
    equal_range(const key_type& key) const { return ht_.equal_range(key); }
  void           reserve(size_type hint) { ht_.reserve(hint); }
  size_type      bucket_count()                   const { return ht_.bucket_count(); }
  size_type      max_bucket_count()               const { return ht_.max_bucket_count(); }
  size_type      bucket_size(size_type n)     const { return ht_.bucket_size(n); }
  void           swap(unordered_map& rhs) { ht_.swap(rhs.ht_); }

public:
  friend bool operator==(const unordered_map& lhs, const unordered_map& rhs)
  {
    return lhs.ht_ == rhs.ht_;
  }
  friend bool operator!=(const unordered_map& lhs, const unordered_map& rhs)
  {
    return lhs.ht_ != rhs.ht_;
  }
};

// 重载比较操作符
template <class Key, class T, class Hash, class EqualKey, class Alloc>
inline bool
operator==(const unordered_map<Key, T, Hash, EqualKey>& lhs,
           const unordered_map<Key, T, Hash, EqualKey>& rhs)
{
  return lhs == rhs;
}

template <class Key, class T, class Hash, class EqualKey, class Alloc>
inline bool
operator!=(const unordered_map<Key, T, Hash, EqualKey>& lhs,
           const unordered_map<Key, T, Hash, EqualKey>& rhs)
{
  return lhs != rhs;
}

// 重载 mystl 的 swap
template <class Key, class T, class Hash, class EqualKey, class Alloc>
inline void
swap(unordered_map<Key, T, Hash, EqualKey>& lhs,
     unordered_map<Key, T, Hash, EqualKey>& rhs)
{
  lhs.swap(rhs);
}

/*****************************************************************************************/

// 模板类 unordered_multimap
// 键值允许重复，其它与 unordered_map 相同
template <class Key, class T, class Hash = mystl::hash<Key>, class EqualKey = mystl::equal_to<Key>>
  class unordered_multimap
{
private:
  // 使用 hashtable 作为底层机制
  typedef hashtable<pair<const Key, T>, Hash, EqualKey>  rep_type;
  rep_type ht_;

public:
  // 使用 hashtable 的型别
  typedef typename rep_type::key_type           key_type;
  typedef T                                     data_type;
  typedef T                                     mapped_type;
  typedef typename rep_type::value_type         value_type;
  typedef typename rep_type::hasher             hasher;
  typedef typename rep_type::key_equal          key_equal;

  typedef typename rep_type::size_type          size_type;
  typedef typename rep_type::difference_type    difference_type;
  typedef typename rep_type::pointer            pointer;
  typedef typename rep_type::const_pointer      const_pointer;
  typedef typename rep_type::reference          reference;
  typedef typename rep_type::const_reference    const_reference;

  typedef typename rep_type::iterator           iterator;
  typedef typename rep_type::const_iterator     const_iterator;
  typedef typename rep_type::allocator_type     allocator_type;

  hasher         hash_fcn()      const { return ht_.hash_fcn(); }
  key_equal      key_eq()        const { return ht_.key_eq(); }
  allocator_type get_allocator() const { return ht_.get_allocator(); }

public:
  // 构造、复制、移动函数
  unordered_multimap() :ht_(100, hasher(), key_equal()) {}  // 缺省使用大小为 100 的表格
  explicit unordered_multimap(size_type n) :ht_(n, hasher(), key_equal()) {}
  unordered_multimap(size_type n, const hasher& hf) :ht_(n, hf, key_equal()) {}
  unordered_multimap(size_type n, const hasher& hf, const key_equal& keq) :ht_(n, hf, keq) {}

  template <class InputIterator>
  unordered_multimap(InputIterator first, InputIterator last,
                     const hasher& hf = hasher(),
                     const key_equal& keq = key_equal())
    : ht_(distance(first, last), hf, keq)
  {
    ht_.insert_equal(first, last);
  }
  unordered_multimap(std::initializer_list<value_type> ilist,
                     const hasher& hf = hasher(),
                     const key_equal& keq = key_equal())
    :ht_(ilist.size(), hf, keq)
  {
    ht_.insert_equal(ilist.begin(), ilist.end());
  }

  unordered_multimap(const unordered_multimap& rhs) :ht_(rhs.ht_) {}
  unordered_multimap(unordered_multimap&& rhs) :ht_(mystl::move(rhs.ht_)) {}

  unordered_multimap& operator=(const unordered_multimap& rhs) { ht_ = rhs.ht_; return *this; }
  unordered_multimap& operator=(unordered_multimap&& rhs) { ht_ = mystl::move(rhs.ht_); return *this; }
  unordered_multimap& operator=(std::initializer_list<value_type> ilist)
  {
    ht_.clear();
    ht_.insert_equal(ilist.begin(), ilist.end());
    return *this;
  }

  // 相关接口
  iterator       begin() { return ht_.begin(); }
  const_iterator begin()    const { return ht_.begin(); }
  iterator       end() { return ht_.end(); }
  const_iterator end()      const { return ht_.end(); }

  bool           empty()    const { return ht_.empty(); }
  size_type      size()     const { return ht_.size(); }
  size_type      max_size() const { return ht_.max_size(); }

  iterator insert(const value_type& value) { return ht_.insert_equal(value); }
  template <class InputIterator>
  void     insert(InputIterator first, InputIterator last) { ht_.insert_equal(first, last); }
  iterator insert_noresize(const value_type& value) { return ht_.insert_equal_noresize(value); }

  size_type erase(const key_type& key) { return ht_.erase(key); }
  void      erase(iterator it) { ht_.erase(it); }
  void      erase(iterator first, iterator last) { ht_.erase(first, last); }
  void      clear() { ht_.clear(); }

  data_type& operator[](const key_type& key)
  {
    return ht_.find_or_insert(value_type(key, T())).second;
  }

  // unordered_multimap 相关操作
  iterator       find(const key_type& key) { return ht_.find(key); }
  const_iterator find(const key_type& key)        const { return ht_.find(key); }
  size_type      count(const key_type& key)       const { return ht_.count(key); }
  pair<iterator, iterator>
    equal_range(const key_type& key) { return ht_.equal_range(key); }
  pair<const_iterator, const_iterator>
    equal_range(const key_type& key) const { return ht_.equal_range(key); }
  void           reserve(size_type hint) { ht_.reserve(hint); }
  size_type      bucket_count()                   const { return ht_.bucket_count(); }
  size_type      max_bucket_count()               const { return ht_.max_bucket_count(); }
  size_type      bucket_size(size_type n)     const { return ht_.bucket_size(n); }
  void           swap(unordered_multimap& rhs) { ht_.swap(rhs.ht_); }

public:
  friend bool operator==(const unordered_multimap& lhs, const unordered_multimap& rhs)
  {
    return lhs.ht_ == rhs.ht_;
  }
  friend bool operator!=(const unordered_multimap& lhs, const unordered_multimap& rhs)
  {
    return lhs.ht_ != rhs.ht_;
  }
};

// 重载比较操作符
template <class Key, class T, class Hash, class EqualKey, class Alloc>
inline bool
operator==(const unordered_multimap<Key, T, Hash, EqualKey>& lhs,
           const unordered_multimap<Key, T, Hash, EqualKey>& rhs)
{
  return lhs == rhs;
}

template <class Key, class T, class Hash, class EqualKey, class Alloc>
inline bool
operator!=(const unordered_multimap<Key, T, Hash, EqualKey>& lhs,
           const unordered_multimap<Key, T, Hash, EqualKey>& rhs)
{
  return lhs != rhs;
}

// 重载 mystl 的 swap
template <class Key, class T, class Hash, class EqualKey, class Alloc>
inline void
swap(unordered_multimap<Key, T, Hash, EqualKey>& lhs,
     unordered_multimap<Key, T, Hash, EqualKey>& rhs)
{
  lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_UNORDERED_MAP_H_

