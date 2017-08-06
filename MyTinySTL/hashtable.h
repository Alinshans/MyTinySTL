#ifndef MYTINYSTL_HASHTABLE_H_
#define MYTINYSTL_HASHTABLE_H_

// 这个头文件包含了一个模板类 hashtable
// hashtable : 哈希表，使用开链法处理冲突

#include <initializer_list>

#include "algo.h"
#include "functional.h"
#include "memory.h"
#include "vector.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{

// hashtable 的节点定义
template <class T>
struct hashtable_node
{
  hashtable_node* next;   // 指向下一个节点
  T               value;  // 储存实值

  hashtable_node() = default;
  hashtable_node(const T& n) :next(nullptr), value(n) {}

  hashtable_node(const hashtable_node& node) :next(node.next), value(node.value) {}
  hashtable_node(hashtable_node&& node) :next(node.next), value(mystl::move(node.value))
  {
    node.next = nullptr;
  }
};

// value traits
template <class T, bool>
struct ht_value_traits_imp
{
  typedef T key_type;
  typedef T mapped_type;
  typedef T value_type;

  template <class Ty>
  static const key_type& get_key(const Ty& value)
  {
    return value;
  }

  template <class Ty>
  static const value_type& get_value(const Ty& value)
  {
    return value;
  }
};

template <class T>
struct ht_value_traits_imp<T, true>
{
  typedef typename std::remove_cv<typename T::first_type>::type key_type;
  typedef typename T::second_type                               mapped_type;
  typedef T                                                     value_type;

  template <class Ty>
  static const key_type& get_key(const Ty& value)
  {
    return value.first;
  }

  template <class Ty>
  static const value_type& get_value(const Ty& value)
  {
    return value;
  }
};

template <class T>
struct ht_value_traits
{
  static constexpr bool __is_map = mystl::__is_pair<T>::value;

  typedef ht_value_traits_imp<T, __is_map> value_traits_type;

  typedef typename value_traits_type::key_type    key_type;
  typedef typename value_traits_type::mapped_type mapped_type;
  typedef typename value_traits_type::value_type  value_type;

  template <class Ty>
  static const key_type& get_key(const Ty& value)
  {
    return value_traits_type::get_key(value);
  }

  template <class Ty>
  static const value_type& get_value(const Ty& value)
  {
    return value_traits_type::get_value(value);
  }
};


// forward declaration

template <class T, class HashFun, class KeyEqual>
class hashtable;

template <class T, class HashFun, class KeyEqual>
struct ht_iterator;

template <class T, class HashFun, class KeyEqual>
struct ht_const_iterator;

// ht_iterator

template <class T, class Hash, class KeyEqual>
struct ht_iterator_base :public mystl::iterator<mystl::forward_iterator_tag, T>
{
  typedef mystl::hashtable<T, Hash, KeyEqual>    hashtable;
  typedef ht_iterator_base<T, Hash, KeyEqual> base;
  typedef mystl::ht_iterator<T, Hash, KeyEqual>    iterator;
  typedef mystl::ht_const_iterator<T, Hash, KeyEqual>    const_iterator;
  typedef hashtable_node<T>*                  node_ptr;
  typedef hashtable*                          contain_ptr;
  typedef const node_ptr                     const_node_ptr;
  typedef const contain_ptr                const_contain_ptr;

  typedef size_t                  size_type;
  typedef ptrdiff_t               difference_type;

  node_ptr    node;  // 迭代器当前所指节点
  contain_ptr ht;   // 保持与容器的连结

  ht_iterator_base() = default;

  bool operator==(const base& rhs) const { return node == rhs.node; }
  bool operator!=(const base& rhs) const { return node != rhs.node; }

};

template <class T, class Hash, class KeyEqual>
struct ht_iterator :public ht_iterator_base<T, Hash, KeyEqual>
{
  typedef ht_iterator_base<T, Hash, KeyEqual> base;
  typedef typename base::hashtable  hashtable;
  typedef typename base::iterator  iterator;
  typedef typename base::const_iterator const_iterator;
  typedef typename base::node_ptr node_ptr;
  typedef typename base::contain_ptr contain_ptr;

  typedef ht_value_traits<T> value_traits;

  // ht_iterator 的内嵌型别
  typedef T                     value_type;
  typedef value_type*                    pointer;
  typedef value_type&                    reference;

  using base::node;
  using base::ht;

  // 构造函数
  ht_iterator() = default;
  ht_iterator(node_ptr n, contain_ptr t)
  {
    node = n;
    ht = t;
  }
  ht_iterator(const iterator& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
  }
  ht_iterator(const const_iterator& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
  }
  ht_iterator(iterator&& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
    rhs.node = nullptr;
    rhs.ht = nullptr;
  }
  ht_iterator(const_iterator&& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
    rhs.node = nullptr;
    rhs.ht = nullptr;
  }

  // 重载操作符
  reference operator*()  const { return node->value; }
  pointer   operator->() const { return &(operator*()); }

  iterator& operator++()
  {
    const node_ptr old = node;  // 原来的节点
    node = node->next;
    if (node == nullptr)
    { // 如果下一个位置为空，跳到下一个 bucket 的起始处
      auto index = ht->__bkt_num_key(value_traits::get_key(old->value));
      while (!node && ++index < ht->buckets_.size())
        node = ht->buckets_[index];
    }
    return *this;
  }
  iterator operator++(int)
  {
    iterator tmp = *this;
    ++*this;
    return tmp;
  }
};

template <class T, class Hash, class KeyEqual>
struct ht_const_iterator :public ht_iterator_base<T, Hash, KeyEqual>
{
  typedef ht_iterator_base<T, Hash, KeyEqual> base;
  typedef typename base::hashtable  hashtable;
  typedef typename base::iterator  iterator;
  typedef typename base::const_iterator const_iterator;
  typedef typename base::const_node_ptr node_ptr;
  typedef typename base::const_contain_ptr contain_ptr;
  typedef ht_value_traits<T> value_traits;
  typedef T                     value_type;
  typedef const value_type*              pointer;
  typedef const value_type&              reference;

  using base::node;
  using base::ht;

  // 构造函数
  ht_const_iterator() = default;
  ht_const_iterator(node_ptr n, contain_ptr t)
  {
    node = n;
    ht = t;
  }
  ht_const_iterator(const iterator& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
  }
  ht_const_iterator(const const_iterator& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
  }
  ht_const_iterator(iterator&& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
    rhs.node = nullptr;
    rhs.ht = nullptr;
  }
  ht_const_iterator(const_iterator&& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
    rhs.node = nullptr;
    rhs.ht = nullptr;
  }

  // 重载操作符
  reference operator*()  const { return node->value; }
  pointer   operator->() const { return &(operator*()); }

  const_iterator& operator++()
  {
    const node_ptr old = node;  // 原来的节点
    node = node->next;
    if (node == nullptr)
    {  // 如果下一个位置为空，跳到下一个 bucket 的起始处
      auto index = ht->__bkt_num_key(value_traits::get_key(old->value));    // 找到下一个 bucket 的位置
      while (!node && ++index < ht->buckets_.size())
      {
        node = ht->buckets_[index];
      }
    }
    return *this;
  }
  const_iterator operator++(int)
  {
    const_iterator tmp = *this;
    ++*this;
    return tmp;
  }
};

// local iterator
template <class T>
struct ht_local_iterator :public mystl::iterator<mystl::forward_iterator_tag, T>
{
  typedef T                  value_type;
  typedef value_type*        pointer;
  typedef value_type&        reference;
  typedef size_t             size_type;
  typedef ptrdiff_t          difference_type;
  typedef hashtable_node<T>* node_ptr;
  typedef ht_local_iterator  self;

  node_ptr node;

  ht_local_iterator(node_ptr n)
    :node(n)
  {
  }

  reference operator*()  const { return node->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    MYSTL_DEBUG(node != nullptr);
    node = node->next;
    return *this;
  }
  
  self operator++(int)
  {
    self tmp(*this);
    ++*this;
    return tmp;
  }

  bool operator==(const self& other) const { return node == other.node; }
  bool operator!=(const self& other) const { return node != other.node; }
};

template <class T>
struct ht_const_local_iterator :public mystl::iterator<mystl::forward_iterator_tag, T>
{
  typedef T                        value_type;
  typedef const value_type*        pointer;
  typedef const value_type&        reference;
  typedef size_t                   size_type;
  typedef ptrdiff_t                difference_type;
  typedef const hashtable_node<T>* node_ptr;
  typedef ht_const_local_iterator  self;

  node_ptr node;

  ht_const_local_iterator(node_ptr n)
    :node(n)
  {
  }

  reference operator*()  const { return node->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    MYSTL_DEBUG(node != nullptr);
    node = node->next;
    return *this;
  }

  self operator++(int)
  {
    self tmp(*this);
    ++*this;
    return tmp;
  }

  bool operator==(const self& other) const { return node == other.node; }
  bool operator!=(const self& other) const { return node != other.node; }
};

// 用 28 个质数来设计表格大小
#define PRIME_NUM 28

static constexpr uint32_t ht_prime_list[PRIME_NUM] = {
  53u,            97u,            193u,          389u,          769u,
  1543u,          3079u,          6151u,         12289u,        24593u,
  49157u,         98317u,         196613u,       393241u,       786433u,
  1572869u,       3145739u,       6291469u,      12582917u,     25165843u,
  50331653u,      100663319u,     201326611u,    402653189u,    805306457u,
  1610612741u,    3221225473u,    4294967291u
};

// 以下函数用于找出最接近并大于等于 n 的那个质数
uint32_t __next_prime(uint32_t n)
{
  const uint32_t* first = ht_prime_list;
  const uint32_t* last = ht_prime_list + PRIME_NUM;
  const uint32_t* pos = mystl::lower_bound(first, last, n);
  return pos == last ? *(last - 1) : *pos;
}

// 模板类 hashtable
// 参数一代表实值类型，参数二代表键值类型，参数三代表哈希函数
// 参数四代表取出键值的方式，缺省使用 mystl 的 identity
// 参数五代表键值比较方式，缺省使用 mystl 的 equal_to
// hashtable 内的元素不会自动排序
template <class T, class Hash, class KeyEqual>
class hashtable
{
public:
  // hashtable 的型别定义
  typedef ht_value_traits<T>                       value_traits;
  typedef typename value_traits::key_type          key_type;
  typedef typename value_traits::mapped_type       mapped_type;
  typedef typename value_traits::value_type        value_type;
  typedef Hash                                     hasher;
  typedef KeyEqual                                 key_equal;

  typedef hashtable_node<T>                        node_type;
  typedef node_type*                               node_ptr;
  typedef mystl::vector<node_ptr>                  bucket_type;

  typedef mystl::allocator<T>                      allocator_type;
  typedef mystl::allocator<T>                      data_allocator;
  typedef mystl::allocator<node_type>              node_allocator;

  typedef typename allocator_type::pointer         pointer;
  typedef typename allocator_type::const_pointer   const_pointer;
  typedef typename allocator_type::reference       reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type       size_type;
  typedef typename allocator_type::difference_type difference_type;


  friend struct mystl::ht_iterator<T, Hash, KeyEqual>;
  friend struct mystl::ht_const_iterator<T, Hash, KeyEqual>;

  typedef mystl::ht_iterator<T, Hash, KeyEqual>       iterator;
  typedef mystl::ht_const_iterator<T, Hash, KeyEqual> const_iterator;
  typedef mystl::ht_local_iterator<T>                 local_iterator;
  typedef mystl::ht_const_local_iterator<T>           const_local_iterator;

  allocator_type get_allocator() const { return allocator_type(); }

  hasher    hash_fcn() const { return hash_; }
  key_equal key_eq()   const { return equal_; }

private:
  // 用以下四个参数来表现 hashtable
  bucket_type buckets_;
  size_type   size_;
  hasher      hash_;
  key_equal   equal_;
  

private:
  bool is_equal(const key_type& key1, const key_type& key2)
  {
    return equal_(key1, key2);
  }

  bool is_equal(const key_type& key1, const key_type& key2) const
  {
    return equal_(key1, key2);
  }

  const_iterator M_cit(node_ptr node) const noexcept
  {
    return const_iterator(node, const_cast<hashtable*>(this));
  }

  iterator M_begin() noexcept
  {
    for (size_type n = 0; n < buckets_.size(); ++n)
    {
      if (buckets_[n])  // 找到第一个有节点的位置就返回
        return iterator(buckets_[n], this);
    }
    return iterator(nullptr, this);
  }

  const_iterator M_begin() const noexcept
  {
    for (size_type n = 0; n < buckets_.size(); ++n)
    {
      if (buckets_[n])  // 找到第一个有节点的位置就返回
        return M_cit(buckets_[n]);
    }
    return M_cit(nullptr);
  }

public:
  // 构造、复制、移动、析构函数
  explicit hashtable(size_type bucket_count,
                     const Hash& hash = Hash(),
                     const KeyEqual& equal = KeyEqual())
    :size_(0), hash_(hash), equal_(equal)
  {
    __hashtable_initialize(bucket_count);
  }

  template <class Iter, typename std::enable_if<
    mystl::__is_input_iterator<Iter>::value, int>::type = 0>
    hashtable(Iter first, Iter last,
              size_type bucket_count,
              const Hash& hash = Hash(),
              const KeyEqual& equal = KeyEqual())
    :size_(mystl::distance(first, last)), hash_(hash), equal_(equal)
  {
    __hashtable_initialize(mystl::max(bucket_count,
                           static_cast<size_type>(mystl::distance(first, last))));
  }

  hashtable(const hashtable& rhs)
    :hash_(rhs.hash_), equal_(rhs.equal_)
  {
    __copy_from(rhs);
  }
  hashtable(hashtable&& rhs) noexcept
    :size_(rhs.size_), hash_(rhs.hash_), equal_(rhs.equal_)
  {
    buckets_ = mystl::move(rhs.buckets_);
    rhs.size_ = 0;
  }

  hashtable& operator=(const hashtable& rhs);
  hashtable& operator=(hashtable&& rhs) noexcept;

  ~hashtable() { clear(); }

  // 迭代器相关操作
  iterator       begin()        noexcept
  { return M_begin(); }
  const_iterator begin()  const noexcept
  { return M_begin(); }
  iterator       end()          noexcept
  { return iterator(nullptr, this); }
  const_iterator end()    const noexcept
  { return M_cit(nullptr); }
  
  const_iterator cbegin() const noexcept
  { return begin(); }
  const_iterator cend()   const noexcept
  { return end(); }

  // 容量相关操作
  bool      empty()    const noexcept { return size_ == 0; }
  size_type size()     const noexcept { return size_; }
  size_type max_size() const noexcept { return static_cast<size_type>(-1); }

  // 插入删除相关操作
  pair<iterator, bool> insert_unique(const value_type& value)
  {
    reserve(size_ + 1);
    return insert_unique_noresize(value);
  }
  iterator insert_equal(const value_type& value)
  {
    reserve(size_ + 1);
    return insert_equal_noresize(value);
  }

  pair<iterator, bool> insert_unique_noresize(const value_type& value);
  iterator insert_equal_noresize(const value_type& value);

  template <class InputIterator>
  void insert_unique(InputIterator first, InputIterator last)
  {
    insert_unique(first, last, iterator_category(first));
  }
  template <class InputIterator>
  void insert_equal(InputIterator first, InputIterator last)
  {
    insert_equal(first, last, iterator_category(first));
  }

  template <class InputIterator>
  void insert_unique(InputIterator first, InputIterator last, input_iterator_tag);
  template <class InputIterator>
  void insert_equal(InputIterator first, InputIterator last, input_iterator_tag);
  template <class ForwardIterator>
  void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
  template <class ForwardIterator>
  void insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

  size_type erase(const key_type& k);
  void      erase(const iterator& it);
  void      erase(iterator first, iterator last);
  void      erase(const const_iterator& it);
  void      erase(const_iterator first, const_iterator last);
  void      clear();
  void      reserve(size_type bucket_count);

  // hashtable 相关操作
  reference                            find_or_insert(const value_type& value);
  iterator                             find(const key_type& k);
  const_iterator                       find(const key_type& k) const;
  size_type                            count(const key_type& k) const;
  pair<iterator, iterator>             equal_range(const key_type& k);
  pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
  void                                 swap(hashtable& rhs);

  // bucket interface

  local_iterator begin(size_type n)              noexcept
  { return buckets_[n]; }
  const_local_iterator begin(size_type n)  const noexcept
  { return buckets_[n]; }
  const_local_iterator cbegin(size_type n) const noexcept
  { return buckets_[n]; }

  local_iterator end(size_type n) noexcept
  { return nullptr; }
  const_local_iterator end(size_type n)    const noexcept
  { return nullptr; }
  const_local_iterator cend(size_type n)   const noexcept
  { return nullptr; }

  size_type bucket_count()                 const noexcept
  { return buckets_.size(); }
  size_type max_bucket_count()             const noexcept
  { return ht_prime_list[PRIME_NUM - 1]; }

  size_type bucket_size(size_type n)       const noexcept;
  size_type bucket(const key_type& key)    const;

private:
  // hashtable 成员函数

  void      __hashtable_initialize(size_type n);

  node_ptr  create_node(const value_type& value);
  void      destroy_node(node_ptr n);

  size_type __next_size(size_type n) const;
  size_type __bkt_num(const value_type& value, size_type n) const;
  size_type __bkt_num(const value_type& value) const;
  size_type __bkt_num_key(const key_type& key, size_type n) const;
  size_type __bkt_num_key(const key_type& key) const;
  void      __erase_bucket(const size_type n, node_ptr first, node_ptr last);
  void      __erase_bucket(const size_type n, node_ptr last);
  void      __copy_from(const hashtable& ht);
};

/*****************************************************************************************/

// 复制赋值运算符
template <class T, class Hash, class KeyEqual>
hashtable<T, Hash, KeyEqual>&
hashtable<T, Hash, KeyEqual>::
operator=(const hashtable& rhs)
{
  if (this != &rhs)
  {
    clear();
    hash_ = rhs.hash_;
    equal_ = rhs.equal_;
    buckets_ = rhs.buckets_;
    size_ = rhs.size_;
  }
  return *this;
}

// 移动赋值运算符
template <class T, class Hash, class KeyEqual>
hashtable<T, Hash, KeyEqual>&
hashtable<T, Hash, KeyEqual>::
operator=(hashtable&& rhs) noexcept
{
  if (this != &rhs)
  {
    clear();

    buckets_ = mystl::move(rhs.buckets_);
    size_ = rhs.size_;
    hash_ = rhs.hash_;
    equal_ = rhs.equal_;

    rhs.size_ = 0;
  }
  return *this;
}

// 在某个 bucket 节点的个数
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
bucket_size(size_type n) const noexcept
{
  size_type result = 0;
  for (auto cur = buckets_[n]; cur; cur = cur->next)
  {
    ++result;
  }
  return result;
}

// 返回某个 key 所在的 bucket index
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
bucket(const key_type& key) const
{
  return __bkt_num_key(key);
}

// 在不需要重建表格的情况下插入新节点，键值不允许重复
template <class T, class Hash, class KeyEqual>
pair<typename hashtable<T, Hash, KeyEqual>::iterator, bool>
hashtable<T, Hash, KeyEqual>::
insert_unique_noresize(const value_type& value)
{
  const auto n = __bkt_num_key(value_traits::get_key(value));
  auto first = buckets_[n];
  for (auto cur = first; cur; cur = cur->next)
  {
    if (is_equal(value_traits::get_key(cur->value), value_traits::get_key(value)))
      return mystl::make_pair(iterator(cur, this), false);
  }
  // 让新节点成为链表的第一个节点
  auto tmp = create_node(value);  
  tmp->next = first;
  buckets_[n] = tmp;
  ++size_;
  return pair<iterator, bool>(iterator(tmp, this), true);
}

// 在不需要重建表格的情况下插入新节点，键值允许重复
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::iterator
hashtable<T, Hash, KeyEqual>::
insert_equal_noresize(const value_type& value)
{
  const auto n = __bkt_num_key(value_traits::get_key(value));
  auto first = buckets_[n];
  for (auto cur = first; cur; cur = cur->next)
  {
    if (is_equal(value_traits::get_key(cur->value), value_traits::get_key(value)))
    { // 如果链表中存在相同键值的节点就马上插入，然后返回
      auto tmp = create_node(value);
      tmp->next = cur->next;
      cur->next = tmp;
      ++size_;
      return iterator(tmp, this);
    }
  }
  // 否则插入在链表头部
  auto tmp = create_node(value);
  tmp->next = first;
  buckets_[n] = tmp;
  ++size_;
  return iterator(tmp, this);
}

// insert_unique 的 input_iterator_tag 版本
template <class T, class Hash, class KeyEqual>
template <class InputIterator>
void hashtable<T, Hash, KeyEqual>::
insert_unique(InputIterator first, InputIterator last, input_iterator_tag)
{
  for (; first != last; ++first)
  {
    insert_unique(*first);
  }
}

// insert_equal 的 input_iterator_tag 版本
template <class T, class Hash, class KeyEqual>
template <class InputIterator>
void hashtable<T, Hash, KeyEqual>::
insert_equal(InputIterator first, InputIterator last, input_iterator_tag)
{
  for (; first != last; ++first)
  {
    insert_equal(*first);
  }
}

// insert_unique 的 forward_iterator_tag 版本
template <class T, class Hash, class KeyEqual>
template <class ForwardIterator>
void hashtable<T, Hash, KeyEqual>::
insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
{
  auto n = distance(first, last);
  reserve(size_ + n);  // 调整大小
  for (; n > 0; --n, ++first)
  {
    insert_unique_noresize(*first);
  }
}

// insert_equal 的 forward_iterator_tag 版本
template <class T, class Hash, class KeyEqual>
template <class ForwardIterator>
void hashtable<T, Hash, KeyEqual>::
insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
{
  auto n = distance(first, last);
  reserve(size_ + n);  // 调整大小
  for (; n > 0; --n, ++first)
  {
    insert_equal_noresize(*first);
  }
}

// 删除键值为 key 的节点
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
erase(const key_type & k)
{
  const auto n = __bkt_num_key(k);
  auto first = buckets_[n];
  size_type erased = 0;
  if (first)
  {
    auto cur = first;
    auto next = cur->next;
    while (next)
    {
      if (is_equal(value_traits::get_key(next->value), k))
      {  // 如果找到键值为 k 的节点
        cur->next = next->next;
        destroy_node(next);
        next = cur->next;
        ++erased;
        --size_;
      }
      else
      {
        cur = next;
        next = cur->next;
      }
    }
    if (is_equal(value_traits::get_key(first->value), k))
    {     // 如果第一个节点键值为 k
      buckets_[n] = first->next;
      destroy_node(first);
      ++erased;
      --size_;
    }
  }
  return erased;
}

// 删除迭代器所指的节点
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
erase(const iterator& it)
{
  auto p = it.node;
  if (p)
  {
    const auto n = __bkt_num(p->value);
    auto node = buckets_[n];
    if (node == p)
    {           // p 位于链表头部
      buckets_[n] = node->next;
      destroy_node(node);
      --size_;
    }
    else
    {
      auto next = node->next;
      while (next)
      {        // 当没到链表尾部
        if (next == p)
        {  // 如果找到 p
          node->next = next->next;
          destroy_node(next);
          --size_;
          break;
        }
        else
        {
          node = next;
          next = node->next;
        }
      }
    }
  }
}

// 删除[first, last)内的节点
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
erase(iterator first, iterator last)
{
  auto first_bucket = first.node ? __bkt_num(first.node->value) : buckets_.size();
  auto last_bucket = last.node ? __bkt_num(last.node->value) : buckets_.size();
  if (first.node == last.node)
  {
    return;
  }
  else if (first_bucket == last_bucket)
  {   // 如果在 bucket 在同一个位置
    __erase_bucket(first_bucket, first.node, last.node);
  }
  else
  {
    __erase_bucket(first_bucket, first.node, static_cast<node_ptr>(nullptr));
    for (auto n = first_bucket + 1; n < last_bucket; ++n)
    {
      __erase_bucket(n, static_cast<node_ptr>(0));
    }
    if (last_bucket != buckets_.size())
    {
      __erase_bucket(last_bucket, last.node);
    }
  }
}

// 删除迭代器所指的节点
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
erase(const const_iterator& it)
{
  erase(iterator(const_cast<node_ptr>(it.node), const_cast<hashtable*>(it.ht)));
}

// 删除[first, last)内的节点
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
erase(const_iterator first, const_iterator last)
{
  erase(iterator(const_cast<node_ptr>(first.node), const_cast<hashtable*>(first.ht)),
        iterator(const_cast<node_ptr>(last.node), const_cast<hashtable*>(last.ht)));
}

// 清空 hashtable
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
clear()
{
  if (size_ != 0)
  {
    for (size_type i = 0; i < buckets_.size(); ++i)
    {
      node_ptr cur = buckets_[i];
      while (cur != nullptr)
      {
        node_ptr next = cur->next;
        destroy_node(cur);
        cur = next;
      }
      buckets_[i] = nullptr;
    }
    size_ = 0;
  }
}

// 重建 bucket 大小
// 强异常保证
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
reserve(size_type bucket_count)
{
  // 若新的 bucket 大小大于原来的的大小，就重建表格
  const auto old_size = buckets_.size();
  if (bucket_count > old_size)
  {
    const size_type n = __next_size(bucket_count);
    bucket_type tmp(n);
    for (size_type index = 0; index < old_size; ++index)
    {
      auto first = buckets_[index];
      while (first != nullptr)
      {
        auto new_index = __bkt_num_key(value_traits::get_key(first->value), n);
        buckets_[index] = first->next;
        first->next = tmp[new_index];
        tmp[new_index] = first;
        first = buckets_[index];
      }
    }
    buckets_.swap(tmp);
  }
}

// 如果找到实值为 value 的元素，就返回它，否则就插入新元素后再返回
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::reference
hashtable<T, Hash, KeyEqual>::
find_or_insert(const value_type& value)
{
  reserve(size_ + 1);
  const auto n = __bkt_num(value);
  auto first = buckets_[n];
  for (auto cur = first; cur; cur = cur->next)
  {
    if (is_equal(value_traits::get_key(cur->value), value_traits::get_key(value)))
      return cur->value;
  }
  auto tmp = create_node(value);
  tmp->next = first;
  buckets_[n] = tmp;
  ++size_;
  return tmp->value;
}

// 查找键值为 k 的节点，返回其迭代器
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::iterator
hashtable<T, Hash, KeyEqual>::
find(const key_type& k)
{
  const auto n = __bkt_num_key(k);
  node_ptr first = buckets_[n];
  for (; first && !is_equal(value_traits::get_key(first->value), k); first = first->next) {}
  return iterator(first, this);
}

template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::const_iterator
hashtable<T, Hash, KeyEqual>::
find(const key_type& k) const
{
  const auto n = __bkt_num_key(k);
  node_ptr first = buckets_[n];
  for (; first && !is_equal(value_traits::get_key(first->value), k); first = first->next) {}
  return M_cit(first);
}

// 查找键值为 k 出现的次数
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
count(const key_type& k) const
{
  const auto n = __bkt_num_key(k);
  size_type result = 0;
  for (node_ptr cur = buckets_[n]; cur; cur = cur->next)
  {
    if (is_equal(value_traits::get_key(cur->value), k))
      ++result;
  }
  return result;
}

// 查找与键值 k 相等的区间，返回一个 pair，指向相等区间的首尾
template <class T, class Hash, class KeyEqual>
pair<typename hashtable<T, Hash, KeyEqual>::iterator,
  typename hashtable<T, Hash, KeyEqual>::iterator>
  hashtable<T, Hash, KeyEqual>::
  equal_range(const key_type& k)
{
  typedef pair<iterator, iterator>  pii;
  const auto n = __bkt_num_key(k);
  for (auto first = buckets_[n]; first; first = first->next)
  {
    if (is_equal(value_traits::get_key(first->value), k))
    {             // 如果出现相等的键值
      for (auto cur = first->next; cur; cur = cur->next)
      {
        if (!is_equal(value_traits::get_key(cur->value), k))        // 如果出现不相等的键值
          return pii(iterator(first, this), iterator(cur, this));
      }
      for (auto m = n + 1; m < buckets_.size(); ++m)
      { // 整个链表都相等，查找下一个链表出现的位置
        if (buckets_[m])
          return pii(iterator(first, this), iterator(buckets_[m], this));
      }
      return pii(iterator(first, this), end());
    }
  }
  return pii(end(), end());
}

template <class T, class Hash, class KeyEqual>
pair<typename hashtable<T, Hash, KeyEqual>::const_iterator,
  typename hashtable<T, Hash, KeyEqual>::const_iterator>
  hashtable<T, Hash, KeyEqual>::
  equal_range(const key_type& k) const
{
  typedef pair<const_iterator, const_iterator>  pii;
  const auto n = __bkt_num_key(k);
  for (node_ptr first = buckets_[n]; first; first = first->next)
  {
    if (is_equal(value_traits::get_key(first->value), k))
    {              // 如果出现相等的键值
      for (node_ptr cur = first->next; cur; cur = cur->next)
      {
        if (!is_equal(value_traits::get_key(cur->value), k))         // 如果出现不相等的键值
          return pii(M_cit(first), M_cit(cur));
      }
      for (auto m = n + 1; m < buckets_.size(); ++m)
      {  // 整个链表都相等，查找下一个链表出现的位置
        if (buckets_[m])
          return pii(M_cit(first), M_cit(buckets_[m]));
      }
      return pii(M_cit(first), cend());
    }
  }
  return pii(cend(), cend());
}

// 交换 hashtable
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
swap(hashtable& rhs)
{
  if (this != &rhs)
  {
    buckets_.swap(rhs.buckets_);
    mystl::swap(size_, rhs.size_);
    mystl::swap(hash_, rhs.hash_);
    mystl::swap(equal_, rhs.equal_);
  }
}

// __next_size 函数
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::__next_size(size_type n) const
{
  return __next_prime(static_cast<unsigned long>(n));
}

// __hashtable_initialize 函数
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
__hashtable_initialize(size_type n)
{
  const auto bucket_nums = __next_size(n);
  buckets_.reserve(bucket_nums);
  buckets_.assign(bucket_nums, static_cast<node_ptr>(nullptr));
}

// __bkt_num 函数
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
__bkt_num(const value_type & value, size_type n) const
{
  return __bkt_num_key(value_traits::get_key(value), n);
}

template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
__bkt_num(const value_type & value) const
{
  return __bkt_num_key(value_traits::get_key(value));
}

// __bkt_num_key 函数
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
__bkt_num_key(const key_type& key, size_type n) const
{
  return hash_(key) % n;
}

template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
__bkt_num_key(const key_type& key) const
{
  return __bkt_num_key(key, buckets_.size());
}

// create_node 函数
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::node_ptr
hashtable<T, Hash, KeyEqual>::
create_node(const value_type& value)
{
  node_ptr tmp = node_allocator::allocate(1);
  try
  {
    data_allocator::construct(mystl::address_of(tmp->value), value);
    tmp->next = nullptr;
  }
  catch (...)
  {
    node_allocator::deallocate(tmp);
    throw;
  }
  return tmp;
}

// destroy_node 函数
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
destroy_node(node_ptr node)
{
  data_allocator::destroy(mystl::address_of(node->value));
  node_allocator::deallocate(node);
}

// __erase_bucket 函数
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
__erase_bucket(const size_type n, node_ptr first, node_ptr last)
{
  auto cur = buckets_[n];
  if (cur == first)
  {
    __erase_bucket(n, last);
  }
  else
  {
    node_ptr next;
    for (next = cur->next; next != first; cur = next, next = cur->next) {}  // 找到 first
    while (next != last)
    {
      cur->next = next->next;
      destroy_node(cur);
      next = cur->next;
      --size_;
    }
  }
}

template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
__erase_bucket(const size_type n, node_ptr last)
{
  auto cur = buckets_[n];
  while (cur != last)
  {
    auto next = cur->next;
    destroy_node(cur);
    cur = next;
    buckets_[n] = cur;
    --size_;
  }
}

// __copy_from 函数
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
__copy_from(const hashtable & ht)
{
  buckets_.clear();
  buckets_.reserve(ht.buckets_.size());
  buckets_.insert(buckets_.end(), ht.buckets_.size(), static_cast<node_ptr>(nullptr));
  try
  {
    for (size_type i = 0; i < ht.buckets_.size(); ++i)
    {
      node_ptr cur = ht.buckets_[i];
      if (cur)
      {  // 如果某 bucket 存在链表
        auto copy = create_node(cur->value);
        buckets_[i] = copy;
        for (auto next = cur->next; next; cur = next, next = cur->next)
        {  //复制链表
          copy->next = create_node(next->value);
          copy = copy->next;
        }
      }
    }
    size_ = ht.size_;
  }
  catch (...)
  {
    clear();
  }
}

// 重载比较操作符
template <class T, class Hash, class KeyEqual>
bool
operator==(const hashtable<T, Hash, KeyEqual>& lhs,
           const hashtable<T, Hash, KeyEqual>& rhs)
{
  if (lhs.buckets_.size() != rhs.buckets_.size())
    return false;
  for (auto n = 0; n < lhs.buckets_.size(); ++n)
  {
    auto cur1 = lhs.buckets_[n];
    auto cur2 = rhs.buckets_[n];
    for (; cur1 && cur2 && cur1->value == cur2->value;
         cur1 = cur1->next, cur2 = cur2->next)
    {
    }
    if (cur1 || cur2)  // cur1 或 cur2 还有元素
      return false;
  }
  return true;
}

template <class T, class Hash, class KeyEqual>
bool
operator!=(const hashtable<T, Hash, KeyEqual>& lhs,
           const hashtable<T, Hash, KeyEqual>& rhs)
{
  return !(lhs == rhs);
}

// 重载 mystl 的 swap
template <class T, class Hash, class KeyEqual>
void
swap(hashtable<T, Hash, KeyEqual>& lhs,
     hashtable<T, Hash, KeyEqual>& rhs)
{
  lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_HASHTABLE_H_

