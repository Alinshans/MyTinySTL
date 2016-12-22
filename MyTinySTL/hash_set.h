#ifndef MYTINYSTL_HASH_SET_H_
#define MYTINYSTL_HASH_SET_H_

// 这个头文件包含两个模板类 hash_set 和 hash_multiset
// 功能与用法与 set 和 multiset 类似，不同的是使用 hashtable 作为底层机制

#include "hashtable.h"

namespace mystl {
    
// 模板类 hash_set
// 参数一代表实值类型，参数二代表哈希函数，缺省使用 mystl 的 hash
// 参数三代表键值比较方式，缺省使用 mystl 的 equal_to，参数四代表空间配置器类型，缺省使用 mystl 的 alloc
// 使用方法与 set 类似，以 hashtable 作为底层机制，所以 hash_set 中的元素不会自动排序
template <class Value, class HashFcn = mystl::hash<Value>, 
    class EqualKey = mystl::equal_to<Value>, class Alloc = mystl::alloc>
class hash_set {
  private:
    // 使用 hash_table 作为底层机制
    typedef hashtable<Value, Value, HashFcn, 
        mystl::identity<Value>, EqualKey, Alloc>  rep_type;
    rep_type ht_;

public:
    // 使用 hash_table 的型别
    typedef typename rep_type::key_type           key_type;
    typedef typename rep_type::value_type         value_type;
    typedef typename rep_type::hasher             hasher;
    typedef typename rep_type::key_equal          key_equal;

    typedef typename rep_type::size_type          size_type;
    typedef typename rep_type::difference_type    difference_type;
    typedef typename rep_type::const_pointer      pointer;
    typedef typename rep_type::const_pointer      const_pointer;
    typedef typename rep_type::const_reference    reference;
    typedef typename rep_type::const_reference    const_reference;

    typedef typename rep_type::const_iterator     iterator;
    typedef typename rep_type::const_iterator     const_iterator;
    typedef typename rep_type::allocator_type     allocator_type;

    hasher         hash_fcn()      const { return ht_.hash_fcn(); }
    key_equal      key_eq()        const { return ht_.key_eq(); }
    allocator_type get_allocator() const { return ht_.get_allocator(); }

public:
    // 构造、复制、移动函数
    hash_set() :ht_(100, hasher(), key_equal()) {}  // 缺省使用大小为 100 的表格
    explicit hash_set(size_type n) :ht_(n, hasher(), key_equal()) {}
    hash_set(size_type n, const hasher& hf) :ht_(n, hf, key_equal()) {}
    hash_set(size_type n, const hasher& hf, const key_equal& keq) :ht_(n, hf, keq) {}

    hash_set(const hash_set& rhs) :ht_(rhs.ht_) {}
    hash_set(hash_set&& rhs) :ht_(std::move(rhs.ht_)) {}

    hash_set& operator=(const hash_set& rhs) { ht_ = rhs.ht_; return *this; }
    hash_set& operator=(hash_set&& rhs)      { ht_ = std::move(rhs.ht_); return *this; }

    // 全部使用 hashtable 的 insert_unique，键值不允许重复
    template <class InputIterator>
    hash_set(InputIterator first, InputIterator last)
        : ht_(100, hasher(), key_equal()) {
        ht_.insert_unique(first, last);
    }
    template <class InputIterator>
    hash_set(InputIterator first, InputIterator last, size_type n)
        : ht_(n, hasher(), key_equal()) {
        ht_.insert_unique(first, last);
    }
    template <class InputIterator>
    hash_set(InputIterator first, InputIterator last, size_type n,
        const hasher& hf) : ht_(n, hf, key_equal()) {
        ht_.insert_unique(first, last);
    }
    template <class InputIterator>
    hash_set(InputIterator first, InputIterator last, size_type n,
        const hasher& hf, const key_equal& keq) : ht_(n, hf, keq) {
        ht_.insert_unique(first, last);
    }

    // 相关接口
    iterator  begin()    const { return ht_.begin(); }
    iterator  end()      const { return ht_.end(); }

    bool      empty()    const { return ht_.empty(); }
    size_type size()     const { return ht_.size(); }
    size_type max_size() const { return ht_.max_size(); }
            
    pair<iterator, bool> insert(const value_type& value) {
        auto p = ht_.insert_unique(value);
        return pair<iterator, bool>(p.first, p.second);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) { 
        ht_.insert_unique(first, last); 
    }
    pair<iterator, bool> insert_noresize(const value_type& value) {
        auto p = ht_.insert_unique_noresize(value);
        return pair<iterator, bool>(p.first, p.second);
    }

    size_type erase(const key_type& key)           { return ht_.erase(key); }
    void      erase(iterator it)                   { ht_.erase(it); }
    void      erase(iterator first, iterator last) { ht_.erase(first, last); }
    void      clear()                              { ht_.clear(); }

    // hash_set 相关操作
    iterator  find(const key_type& key)  const { return ht_.find(key); }
    size_type count(const key_type& key) const { return ht_.count(key); }
    pair<iterator, iterator> equal_range(const key_type& key) const {
        return ht_.equal_range(key);
    }
    void      reserve(size_type hint)            { ht_.reserve(hint); }
    size_type bucket_count()               const { return ht_.bucket_count(); }
    size_type max_bucket_count()           const { return ht_.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return ht_.elems_in_bucket(n); }
    void      swap(hash_set& rhs)                { ht_.swap(rhs.ht_); }

  public:
    // 重载 operator==, operator!=
    friend bool operator==(const hash_set& lhs, const hash_set& rhs) { return lhs.ht_ == rhs.ht_; }
    friend bool operator!=(const hash_set& lhs, const hash_set& rhs) { return lhs.ht_ != rhs.ht_; }
};

// 重载比较操作符
template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc>& lhs,
    const hash_set<Value, HashFcn, EqualKey, Alloc>& rhs) {
    return lhs == rhs;
}

template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator!=(const hash_set<Value, HashFcn, EqualKey, Alloc>& lhs,
    const hash_set<Value, HashFcn, EqualKey, Alloc>& rhs) {
    return lhs != rhs;
}

// 重载 mystl 的 swap
template <class Value, class HashFcn, class EqualKey, class Alloc>
void swap(hash_set<Value, HashFcn, EqualKey, Alloc>& lhs,
    hash_set<Value, HashFcn, EqualKey, Alloc>& rhs) {
    lhs.swap(rhs);
}

/*****************************************************************************************/
    
// 模板类 hash_multiset
// 键值允许重复，其它与 hash_set 相同
template <class Value, class HashFcn = mystl::hash<Value>,
    class EqualKey = mystl::equal_to<Value>, class Alloc = mystl::alloc>
class hash_multiset {
  private:
    // 使用 hash_table 作为底层机制
    typedef hashtable<Value, Value, HashFcn, 
        mystl::identity<Value>, EqualKey, Alloc>  rep_type;
    rep_type ht_;

  public:
    // 使用 hash_table 的型别
    typedef typename rep_type::key_type           key_type;
    typedef typename rep_type::value_type         value_type;
    typedef typename rep_type::hasher             hasher;
    typedef typename rep_type::key_equal          key_equal;

    typedef typename rep_type::size_type          size_type;
    typedef typename rep_type::difference_type    difference_type;
    typedef typename rep_type::const_pointer      pointer;
    typedef typename rep_type::const_pointer      const_pointer;
    typedef typename rep_type::const_reference    reference;
    typedef typename rep_type::const_reference    const_reference;

    typedef typename rep_type::const_iterator     iterator;
    typedef typename rep_type::const_iterator     const_iterator;
    typedef typename rep_type::allocator_type     allocator_type;

    hasher         hash_fcn()      const { return ht_.hash_fcn(); }
    key_equal      key_eq()        const { return ht_.key_eq(); }
    allocator_type get_allocator() const { return ht_.get_allocator(); }

  public:
    // 构造、复制、移动函数
    hash_multiset() :ht_(100, hasher(), key_equal()) {}  // 缺省使用大小为 100 的表格
    explicit hash_multiset(size_type n) :ht_(n, hasher(), key_equal()) {}
    hash_multiset(size_type n, const hasher& hf) :ht_(n, hf, key_equal()) {}
    hash_multiset(size_type n, const hasher& hf, const key_equal& keq) :ht_(n, hf, keq) {}
            
    hash_multiset(const hash_multiset& rhs) :ht_(rhs.ht_) {}
    hash_multiset(hash_multiset&& rhs) :ht_(std::move(rhs.ht_)) {}

    hash_multiset& operator=(const hash_multiset& rhs) { ht_ = rhs.ht_; return *this; }
    hash_multiset& operator=(hash_multiset&& rhs) { ht_ = std::move(rhs.ht_); return *this; }

    // 全部使用 insert_equal，键值允许重复
    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last)
        : ht_(100, hasher(), key_equal()) {
        ht_.insert_equal(first, last);
    }
    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last, size_type n)
        : ht_(n, hasher(), key_equal()) {
        ht_.insert_equal(first, last);
    }
    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last, size_type n,
        const hasher& hf) : ht_(n, hf, key_equal()) {
        ht_.insert_equal(first, last);
    }
    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last, size_type n,
        const hasher& hf, const key_equal& keq) : ht_(n, hf, keq) {
        ht_.insert_equal(first, last);
    }

    // 相关接口
    iterator  begin()    const { return ht_.begin(); }
    iterator  end()      const { return ht_.end(); }

    bool      empty()    const { return ht_.empty(); }
    size_type size()     const { return ht_.size(); }
    size_type max_size() const { return ht_.max_size(); }

    iterator insert(const value_type& value) {
        return ht_.insert_equal(value);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        ht_.insert_equal(first, last);
    }
    iterator insert_noresize(const value_type& value) {
        return ht_.insert_equal_noresize(value);
    }

    size_type erase(const key_type& key)           { return ht_.erase(key); }
    void      erase(iterator it)                   { ht_.erase(it); }
    void      erase(iterator first, iterator last) { ht_.erase(first, last); }
    void      clear()                              { ht_.clear(); }

    // hash_multiset 相关操作
    iterator  find(const key_type& key)  const { return ht_.find(key); }
    size_type count(const key_type& key) const { return ht_.count(key); }
    pair<iterator, iterator> equal_range(const key_type& key) const {
        return ht_.equal_range(key);
    }
    void      reserve(size_type hint)            { ht_.reserve(hint); }
    size_type bucket_count()               const { return ht_.bucket_count(); }
    size_type max_bucket_count()           const { return ht_.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return ht_.elems_in_bucket(n); }
    void      swap(hash_multiset& rhs)           { ht_.swap(rhs.ht_); }

  public:
    // 重载 operator==, operator!=
    friend bool operator==(const hash_multiset& lhs, const hash_multiset& rhs) {
        return lhs.ht_ == rhs.ht_;
    }
    friend bool operator!=(const hash_multiset& lhs, const hash_multiset& rhs) {
        return lhs.ht_ != rhs.ht_;
    }
};

// 重载比较操作符
template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs,
    const hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
    return lhs == rhs;
}

template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator!=(const hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs,
    const hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
    return lhs != rhs;
}

// 重载 mystl 的 swap
template <class Value, class HashFcn, class EqualKey, class Alloc>
void swap(hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs,
    hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
    lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_HASH_SET_H_

