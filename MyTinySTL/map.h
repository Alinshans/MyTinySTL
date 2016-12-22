#ifndef MYTINYSTL_MAP_H_
#define MYTINYSTL_MAP_H_

// 这个头文件包含了两个模板类 map 和 multimap
// map      : 映射，元素具有键值和实值，会根据键值大小自动排序，键值不允许重复
// multimap : 键值允许重复的 map

#include "rb_tree.h"

namespace mystl {

// 模板类 set
// 参数一代表键值类型，参数二代表实值类型，参数三代表键值的比较方式，缺省使用 mystl 的 less
// 参数四代表空间配置器的类型，缺省使用 mystl 的 alloc
// 每个元素都是 pair，拥有键值与实值，不允许有重复的键值，使用 rb_tree 作为底层机制
template <class Key, class T, class Compare = mystl::less<Key>, class Alloc = mystl::alloc>
class map {
  public:
    // map 的嵌套型别定义
    typedef Key                   key_type;
    typedef T                     data_type;
    typedef T                     mapped_type;
    typedef pair<const Key, T>    value_type;  // 元素型别是一对 pair，含键值/实值
    typedef Compare               key_compare;

    // 定义一个 functor，用来进行元素比较
    class value_compare : public binary_function <value_type, value_type, bool> {
        friend class map<Key, T, Compare, Alloc>;
      private:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
      public:
        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return comp(lhs.first, rhs.first);  // 比较键值的大小
        }
    };

  private:
    // 以 rb_tree 作为底层机制
    typedef rb_tree<key_type, value_type, mystl::selectfirst<value_type>, key_compare, Alloc>  rep_type;
    rep_type t_;    // 用 rb_tree 表现 map

  public:
    // 使用 rb_tree 的型别
    typedef typename rep_type::pointer                   pointer;
    typedef typename rep_type::const_pointer             const_pointer;
    typedef typename rep_type::reference                 reference;
    typedef typename rep_type::const_reference           const_reference;
    typedef typename rep_type::iterator                  iterator;
    // iterator 定义为 rb_tree 的 iterator，因为 map 允许修改元素的实值
    typedef typename rep_type::const_iterator            const_iterator;
    typedef typename rep_type::reverse_iterator          reverse_iterator;
    typedef typename rep_type::const_reverse_iterator    const_reverse_iterator;
    typedef typename rep_type::size_type                 size_type;
    typedef typename rep_type::difference_type           difference_type;
    typedef typename rep_type::allocator_type            allocator_type;

  public:
    // 构造、复制、移动函数
    map() :t_() {}    
    template <class InputIterator>
    map(InputIterator first, InputIterator last) : t_() { t_.insert_unique(first, last); }
    
    map(const map& rhs) :t_(rhs.t_) {}
    map(map&& rhs) :t_(std::move(rhs.t_)) {}

    map& operator=(const map& rhs) { t_ = rhs.t_; return *this; }
    map& operator=(map&& rhs)      { t_ = std::move(rhs.t_); return *this; }

    // 相关接口操作
    key_compare            key_comp()      const { return t_.key_comp(); }
    value_compare          value_comp()    const { return value_compare(t_.key_comp()); }
    allocator_type         get_allocator() const { return t_.get_allocator(); }

    iterator               begin()               { return t_.begin(); }
    const_iterator         begin()         const { return t_.begin(); }
    iterator               end()                 { return t_.end(); }
    const_iterator         end()           const { return t_.end(); }
    reverse_iterator       rbegin()              { return t_.rbegin(); }
    const_reverse_iterator rbegin()        const { return t_.rbegin(); }
    reverse_iterator       rend()                { return t_.rend(); }
    const_reverse_iterator rend()          const { return t_.rend(); }

    bool                   empty()         const { return t_.empty(); }
    size_type              size()          const { return t_.size(); }
    size_type              max_size()      const { return t_.max_size(); }

    T& operator[](const key_type& k) {    
        auto i = lower_bound(k);  // 找到第一个键值大于等于 k 的
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }

    // 插入删除操作，全部使用 rb_tree 的 insert_unique
    pair<iterator, bool> insert(const value_type& x) {
        return t_.insert_unique(x);
    }
    iterator insert(iterator position, const value_type& x) {
        return t_.insert_unique(position, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        t_.insert_unique(first, last);
    }

    void      erase(iterator position)             { t_.erase(position); }
    size_type erase(const key_type& k)             { return t_.erase(k); }
    void      erase(iterator first, iterator last) { t_.erase(first, last); }
    void      clear()                              { t_.clear(); }

    // map 相关操作
    iterator                             find(const key_type& k)              { return t_.find(k); }
    const_iterator                       find(const key_type& k)        const { return t_.find(k); }
    size_type                            count(const key_type& k)       const { return t_.count(k); }
    iterator                             lower_bound(const key_type& k)       { return t_.lower_bound(k); }
    const_iterator                       lower_bound(const key_type& k) const { return t_.lower_bound(k); }
    iterator                             upper_bound(const key_type& k)       { return t_.upper_bound(k); }
    const_iterator                       upper_bound(const key_type& k) const { return t_.upper_bound(k); }
    pair<iterator, iterator>             equal_range(const key_type& k)       { return t_.equal_range(k); }
    pair<const_iterator, const_iterator> equal_range(const key_type& k) const { return t_.equal_range(k); }
    void                                 swap(map& rhs)                       { t_.swap(rhs.t_); }

  public:
    friend bool operator==(const map& lhs, const map& rhs) { return lhs.t_ == rhs.t_; }
    friend bool operator< (const map& lhs, const map& rhs) { return lhs.t_ <  rhs.t_; }
};

// 重载比较操作符
template <class Key, class T, class Compare, class Alloc>
inline bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
    return lhs == rhs;
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator<(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
    return lhs < rhs;
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator!=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
    return !(lhs == rhs);
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator>(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
    return rhs < lhs;
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator<=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
    return !(rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator>=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
    return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class Key, class T, class Compare, class Alloc>
void swap(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) {
    lhs.swap(rhs);
}

/*****************************************************************************************/

// 模板类 multimap
// 键值允许重复的 map
template <class Key, class T, class Compare = mystl::less<Key>, class Alloc = mystl::alloc>
class multimap {
  public:
    // multimap 的型别定义
    typedef Key                   key_type;
    typedef T                     data_type;
    typedef T                     mapped_type;
    typedef pair<const Key, T>    value_type;  // 元素型别是一对 pair，含键值/实值
    typedef Compare               key_compare;

    // 定义一个 functor，用来进行元素比较
    class value_compare : public binary_function <value_type, value_type, bool> {
        friend class multimap<Key, T, Compare, Alloc>;
      private:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
      public:
        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return comp(lhs.first, rhs.first);
        }
    };

  private:
    // 用 rb_tree 作为底层机制
    typedef rb_tree<key_type, value_type, mystl::selectfirst<value_type>, key_compare, Alloc>  rep_type;
    rep_type t_;  // 用 rb_tree 表现 multimap

  public:
    // 使用 rb_tree 的型别
    typedef typename rep_type::pointer                   pointer;
    typedef typename rep_type::const_pointer             const_pointer;
    typedef typename rep_type::reference                 reference;
    typedef typename rep_type::const_reference           const_reference;
    typedef typename rep_type::iterator                  iterator;
    // iterator 定义为 RB-tree 的 iterator，因为 multimap 允许修改元素的实值
    typedef typename rep_type::const_iterator            const_iterator;
    typedef typename rep_type::reverse_iterator          reverse_iterator;
    typedef typename rep_type::const_reverse_iterator    const_reverse_iterator;
    typedef typename rep_type::size_type                 size_type;
    typedef typename rep_type::difference_type           difference_type;
    typedef typename rep_type::allocator_type            allocator_type;

  public:
    // 构造、复制、移动函数
    multimap() :t_() {}
    template <class InputIterator>
    multimap(InputIterator first, InputIterator last) : t_() { t_.insert_equal(first, last); }
        
    multimap(const multimap& rhs) :t_(rhs.t_) {}
    multimap(multimap&& rhs) :t_(std::move(rhs.t_)) {}

    multimap& operator=(const multimap& rhs) { t_ = rhs.t_; return *this; }
    multimap& operator=(multimap&& rhs)      { t_ = std::move(rhs.t_); return *this; }

    // 相关接口操作
    key_compare            key_comp()      const { return t_.key_comp(); }
    value_compare          value_comp()    const { return value_compare(t_.key_comp()); }
    allocator_type         get_allocator() const { return t_.get_allocator(); }

    iterator               begin()               { return t_.begin(); }
    const_iterator         begin()         const { return t_.begin(); }
    iterator               end()                 { return t_.end(); }
    const_iterator         end()           const { return t_.end(); }
    reverse_iterator       rbegin()              { return t_.rbegin(); }
    const_reverse_iterator rbegin()        const { return t_.rbegin(); }
    reverse_iterator       rend()                { return t_.rend(); }
    const_reverse_iterator rend()          const { return t_.rend(); }

    bool                   empty()         const { return t_.empty(); }
    size_type              size()          const { return t_.size(); }
    size_type              max_size()      const { return t_.max_size(); }

    T& operator[](const key_type& k) {
        auto i = lower_bound(k);  // 找到第一个键值大于等于 k 的
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }

    // 插入删除操作，全部使用 rb_tree 的 insert_equal
    iterator insert(const value_type& x) { return t_.insert_equal(x); }
    iterator insert(iterator position, const value_type& x) {
        return t_.insert_equal(position, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        t_.insert_equal(first, last);
    }

    void erase(iterator position)             { t_.erase(position); }
    size_type erase(const key_type& k)        { return t_.erase(k); }
    void erase(iterator first, iterator last) { t_.erase(first, last); }
    void clear() { t_.clear(); }

    // multimap 相关操作
    iterator                             find(const key_type& k)              { return t_.find(k); }
    const_iterator                       find(const key_type& k)        const { return t_.find(k); }
    size_type                            count(const key_type& k)       const { return t_.count(k); }
    iterator                             lower_bound(const key_type& k)       { return t_.lower_bound(k); }
    const_iterator                       lower_bound(const key_type& k) const { return t_.lower_bound(k); }
    iterator                             upper_bound(const key_type& k)       { return t_.upper_bound(k); }
    const_iterator                       upper_bound(const key_type& k) const { return t_.upper_bound(k); }
    pair<iterator, iterator>             equal_range(const key_type& k)       { return t_.equal_range(k); }
    pair<const_iterator, const_iterator> equal_range(const key_type& k) const { return t_.equal_range(k); }
    void                                 swap(multimap& rhs)                  { t_.swap(rhs.t_); }

  public:
    friend bool operator==(const multimap& lhs, const multimap& rhs) { return lhs.t_ == rhs.t_; }
    friend bool operator< (const multimap& lhs, const multimap& rhs) { return lhs.t_ <  rhs.t_; }
};

// 重载比较操作符
template <class Key, class T, class Compare, class Alloc>
inline bool operator==(const multimap<Key, T, Compare, Alloc>& lhs,
    const multimap<Key, T, Compare, Alloc>& rhs) {
    return lhs == rhs;
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator<(const multimap<Key, T, Compare, Alloc>& lhs, 
    multimap<Key, T, Compare, Alloc>& rhs) {
    return lhs < rhs;
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator!=(const multimap<Key, T, Compare, Alloc>& lhs,
    const multimap<Key, T, Compare, Alloc>& rhs) {
    return !(lhs == rhs);
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator>(const multimap<Key, T, Compare, Alloc>& lhs, 
    const multimap<Key, T, Compare, Alloc>& rhs) {
    return rhs < lhs;
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator<=(const multimap<Key, T, Compare, Alloc>& lhs, 
    const multimap<Key, T, Compare, Alloc>& rhs) {
    return !(rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator>=(const multimap<Key, T, Compare, Alloc>& lhs, 
    const multimap<Key, T, Compare, Alloc>& rhs) {
    return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class Key, class T, class Compare, class Alloc>
void swap(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) {
    lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_MAP_H_

