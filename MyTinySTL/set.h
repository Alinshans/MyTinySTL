#ifndef MYTINYSTL_SET_H_
#define MYTINYSTL_SET_H_

// 这个头文件包含两个模板类 set 和 multiset
// set      : 集合，键值即实值，键值不允许重复，以 rb_tree 作为底层机制，集合内元素会自动排序
// multiset : 键值允许重复的 set

#include "rb_tree.h"

namespace mystl {

// 模板类 set
// 参数一代表键值类型，参数二代表键值比较方式，缺省使用 mystl 的 less 
// 参数三代表空间配置器类型，缺省使用 mystl 的 alloc
// 键值与实值相同，键值不允许重复，以 rb_tree 作为底层机制，元素会根据键值大小自动排序
template <class Key, class Compare = mystl::less<Key>, class Alloc = mystl::alloc>
class set {
  public:
    typedef Key        key_type;
    typedef Key        value_type;
    typedef Compare    key_compare;
    typedef Compare    value_compare;

  private:
    // 以 rb_tree 作为底层机制
    typedef rb_tree<key_type, value_type, mystl::identity<value_type>, key_compare, Alloc>  rep_type;
    rep_type t_;  // 以 rb_tree 表现 set

  public:
    // 使用 rb_tree 定义的型别
    typedef typename rep_type::const_pointer             pointer;
    typedef typename rep_type::const_pointer             const_pointer;
    typedef typename rep_type::const_reference           reference;
    typedef typename rep_type::const_reference           const_reference;
    typedef typename rep_type::const_iterator            iterator;
    // iterator 定义为 rb_tree 的 const_iterator，表示 set 不允许进行写入操作
    typedef typename rep_type::const_iterator            const_iterator;
    typedef typename rep_type::const_reverse_iterator    reverse_iterator;
    typedef typename rep_type::const_reverse_iterator    const_reverse_iterator;
    typedef typename rep_type::size_type                 size_type;
    typedef typename rep_type::difference_type           difference_type;
    typedef typename rep_type::allocator_type            allocator_type;

  public:
    // 构造、复制、移动函数
    set() :t_() {}
    template <class InputIterator>
    set(InputIterator first, InputIterator last) : t_() { t_.insert_unique(first, last); }
        
    set(const set& rhs) :t_(rhs.t_) {}
    set(set&& rhs) :t_(std::move(rhs.t_)) {}

    set& operator=(const set& rhs) { t_ = rhs.t_; return *this; }
    set& operator=(set&& rhs)      { t_ = std::move(rhs.t_); return *this; }

    // 相关接口操作
    key_compare      key_comp()      const { return t_.key_comp(); }
    value_compare    value_comp()    const { return t_.key_comp(); }
    allocator_type   get_allocator() const { return t_.get_allocator(); }

    iterator         begin()         const { return t_.begin(); }
    iterator         end()           const { return t_.end(); }
    reverse_iterator rbegin()        const { return t_.rbegin(); }
    reverse_iterator rend()          const { return t_.rend(); }

    bool             empty()         const { return t_.empty(); }
    size_type        size()          const { return t_.size(); }
    size_type        max_size()      const { return t_.max_size(); }
        
    // 插入删除操作，全部使用 rb_tree 的 insert_unique
    pair<iterator, bool> insert(const value_type& value) {
        return t_.insert_unique(value);
    }
    iterator insert(iterator position, const value_type& value) {
        typedef typename rep_type::iterator rep_iterator;
        return t_.insert_unique(reinterpret_cast<rep_iterator&>(position), value);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        t_.insert_unique(first, last);
    }

    void erase(iterator position) {
        typedef typename rep_type::iterator rep_iterator;
        t_.erase(reinterpret_cast<rep_iterator&>(position));
    }
    size_type erase(const key_type& k) {
        return t_.erase(k);
    }
    void erase(iterator first, iterator last) {
        typedef typename rep_type::iterator rep_iterator;
        t_.erase(reinterpret_cast<rep_iterator&>(first), reinterpret_cast<rep_iterator&>(last));
    }
    void clear() { t_.clear(); }

    // set 相关操作
    iterator                 find(const key_type& k)        const { return t_.find(k); }
    size_type                count(const key_type& k)       const { return t_.count(k); }
    iterator                 lower_bound(const key_type& k) const { return t_.lower_bound(k); }
    iterator                 upper_bound(const key_type& k) const { return t_.upper_bound(k); }
    pair<iterator, iterator> equal_range(const key_type& k) const { return t_.equal_range(k); }
    void                     swap(set& rhs)                       { t_.swap(rhs.t_); }

  public:
    friend bool operator==(const set& lhs, const set& rhs) { return lhs.t_ == rhs.t_; }
    friend bool operator< (const set& lhs, const set& rhs) { return lhs.t_ <  rhs.t_; }
};

// 重载比较操作符
template <class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
    return lhs == rhs;
}

template <class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
    return lhs < rhs;
}

template <class Key, class Compare, class Alloc>
inline bool operator!=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
    return !(lhs == rhs);
}

template <class Key, class Compare, class Alloc>
inline bool operator>(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
    return rhs < lhs;
}

template <class Key, class Compare, class Alloc>
inline bool operator<=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
    return !(rhs < lhs);
}

template <class Key, class Compare, class Alloc>
inline bool operator>=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
    return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class Key, class Compare, class Alloc>
void swap(set<Key, Compare, Alloc>& lhs, set<Key, Compare, Alloc>& rhs) {
    lhs.swap(rhs);
}

/*****************************************************************************************/

// 模板类 multiset
// 允许键值重复的 set
template <class Key, class Compare = mystl::less<Key>, class Alloc = mystl::alloc>
class multiset {
  public:
    typedef Key        key_type;
    typedef Key        value_type;
    typedef Compare    key_compare;
    typedef Compare    value_compare;

  private:
    // 以 rb_tree 作为底层机制
    typedef rb_tree<key_type, value_type, mystl::identity<value_type>, key_compare, Alloc>  rep_type;
    rep_type t_;  // 以 rb_tree 表现 multiset

  public:
    // 使用 rb_tree 定义的型别
    typedef typename rep_type::const_pointer             pointer;
    typedef typename rep_type::const_pointer             const_pointer;
    typedef typename rep_type::const_reference           reference;
    typedef typename rep_type::const_reference           const_reference;
    typedef typename rep_type::const_iterator            iterator;
    // iterator 定义为 rb_tree 的 const_iterator，表示 multiset 不允许进行写入操作
    typedef typename rep_type::const_iterator            const_iterator;
    typedef typename rep_type::const_reverse_iterator    reverse_iterator;
    typedef typename rep_type::const_reverse_iterator    const_reverse_iterator;
    typedef typename rep_type::size_type                 size_type;
    typedef typename rep_type::difference_type           difference_type;
    typedef typename rep_type::allocator_type            allocator_type;

  public:
    // 构造、复制、移动函数
    multiset() :t_() {}
    template <class InputIterator>
    multiset(InputIterator first, InputIterator last) : t_() { t_.insert_equal(first, last); }
        
    multiset(const multiset& rhs) :t_(rhs.t_) {}
    multiset(multiset&& rhs) :t_(std::move(rhs.t_)) {}

    multiset& operator=(const multiset& rhs) { t_ = rhs.t_; return *this; }
    multiset& operator=(multiset&& rhs)      { t_ = std::move(rhs.t_); return *this; }

    // 相关接口
    key_compare      key_comp()      const { return t_.key_comp(); }
    value_compare    value_comp()    const { return t_.key_comp(); }
    allocator_type   get_allocator() const { return t_.get_allocator(); }

    iterator         begin()         const { return t_.begin(); }
    iterator         end()           const { return t_.end(); }
    reverse_iterator rbegin()        const { return t_.rbegin(); }
    reverse_iterator rend()          const { return t_.rend(); }

    bool             empty()         const { return t_.empty(); }
    size_type        size()          const { return t_.size(); }
    size_type        max_size()      const { return t_.max_size(); }

    // 插入删除，全部使用 rb_tree 的 insert_equal
    iterator insert(const value_type& value) {
        return t_.insert_equal(value);
    }
    iterator insert(iterator position, const value_type& value) {
        typedef typename rep_type::iterator rep_iterator;
        return t_.insert_equal(reinterpret_cast<rep_iterator&>(position), value);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        t_.insert_equal(first, last);
    }

    void erase(iterator position) {
        typedef typename rep_type::iterator rep_iterator;
        t_.erase(reinterpret_cast<rep_iterator&>(position));
    }
    size_type erase(const key_type& k) {
        return t_.erase(k);
    }
    void erase(iterator first, iterator last) {
        typedef typename rep_type::iterator rep_iterator;
        t_.erase(reinterpret_cast<rep_iterator&>(first), reinterpret_cast<rep_iterator&>(last));
    }
    void clear() { t_.clear(); }

    // multiset 相关操作
    iterator                 find(const key_type& k)        const { return t_.find(k); }
    size_type                count(const key_type& k)       const { return t_.count(k); }
    iterator                 lower_bound(const key_type& k) const { return t_.lower_bound(k); }
    iterator                 upper_bound(const key_type& k) const { return t_.upper_bound(k); }
    pair<iterator, iterator> equal_range(const key_type& k) const { return t_.equal_range(k); }
    void                     swap(multiset& rhs)                  { t_.swap(rhs.t_); }

  public:
    friend bool operator==(const multiset& lhs, const multiset& rhs) { return lhs.t_ == rhs.t_; }
    friend bool operator< (const multiset& lhs, const multiset& rhs) { return lhs.t_ <  rhs.t_; }
};

// 重载比较操作符
template <class Key, class Compare, class Alloc>
inline bool operator==(const multiset<Key, Compare, Alloc>& lhs,
                       const multiset<Key, Compare, Alloc>& rhs) {
    return lhs == rhs;
}

template <class Key, class Compare, class Alloc>
inline bool operator<(const multiset<Key, Compare, Alloc>& lhs,
                       const multiset<Key, Compare, Alloc>& rhs) {
    return lhs < rhs;
}

template <class Key, class Compare, class Alloc>
inline bool operator!=(const multiset<Key, Compare, Alloc>& lhs, 
    const multiset<Key, Compare, Alloc>& rhs) {
    return !(lhs == rhs);
}

template <class Key, class Compare, class Alloc>
inline bool operator>(const multiset<Key, Compare, Alloc>& lhs, 
    const multiset<Key, Compare, Alloc>& rhs) {
    return rhs < lhs;
}

template <class Key, class Compare, class Alloc>
inline bool operator<=(const multiset<Key, Compare, Alloc>& lhs, 
    const multiset<Key, Compare, Alloc>& rhs) {
    return !(rhs < lhs);
}

template <class Key, class Compare, class Alloc>
inline bool operator>=(const multiset<Key, Compare, Alloc>& lhs,
    const multiset<Key, Compare, Alloc>& rhs) {
    return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class Key, class Compare, class Alloc>
void swap(multiset<Key, Compare, Alloc>& lhs, multiset<Key, Compare, Alloc>& rhs) {
    lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_SET_H_

