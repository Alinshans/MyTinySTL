#ifndef MYTINYSTL_HASHTABLE_H_
#define MYTINYSTL_HASHTABLE_H_

// 这个头文件包含了一个模板类 hashtable
// hashtable 代表哈希表类型数据结构

#include "algo.h"
#include "functional.h"
#include "hash_functional.h"
#include "memory.h"
#include "vector.h"

namespace mystl {

// hashtable 使用开链法，以下是节点定义
template <class T>
struct __hashtable_node {
    __hashtable_node* next;   // 指向下一个节点
    T                 value;  // 储存实值
    __hashtable_node() :next(nullptr), value(0) {}
};

// 前置声明
template <class Val, class Key, class HashFun, 
    class ExtractKey, class EqualKey, class Alloc>
class hashtable;

template <class Val, class Key, class HashFun, 
    class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_iterator;

template <class Val, class Key, class HashFun, 
    class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_const_iterator;

// __hashtable_iterator 定义
template <class Val, class Key, class HashFcn, 
    class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_iterator {
    typedef mystl::hashtable<
        Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>    hashtable;
    typedef mystl::__hashtable_iterator<
        Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>    iterator;
    typedef mystl::__hashtable_const_iterator<
        Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>    const_iterator;
    typedef __hashtable_node<Val>                          node;

    // __hashtable_iterator 的内嵌型别
    typedef forward_iterator_tag    iterator_category;
    typedef Val                     value_type;
    typedef Val*                    pointer;
    typedef Val&                    reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t               difference_type;

    node*      cur;   // 迭代器当前所指节点
    hashtable* ht;    // 保持与容器的连结

    // 构造函数
    __hashtable_iterator() {}
    __hashtable_iterator(node* n, hashtable* t) :cur(n), ht(t) {}
    __hashtable_iterator(const iterator& rhs) :cur(rhs.cur), ht(rhs.ht) {}

    // 重载操作符
    reference operator*()  const { return cur->value; }
    pointer   operator->() const { return &(operator*()); }

    iterator& operator++();
    iterator operator++(int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    // 重载比较操作符
    bool operator==(const iterator& rhs) const { return cur == rhs.cur; }
    bool operator!=(const iterator& rhs) const { return cur != rhs.cur; }
};

// 定义 __hashtable_iterator 的 operator++ 
template <class Val, class Key, class HashFcn, 
    class ExtractKey, class EqualKey, class Alloc>
__hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
    __hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
    const node* old = cur;  // 原来的节点
    cur = cur->next;
    if (!cur) {  // 如果下一个位置为空，跳到下一个 bucket 的起始处
        auto index = ht->__bkt_num(old->value);  // 找到下一个 bucket 的位置
        while (!cur && ++index < ht->buckets_.size())
            cur = ht->buckets_[index];
    }
    return *this;
}

// __hashtable_const_iterator 定义
template <class Val, class Key, class HashFcn, 
    class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_const_iterator {
    typedef mystl::hashtable<
        Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>    hashtable;
    typedef mystl::__hashtable_iterator<
        Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>    iterator;
    typedef mystl::__hashtable_const_iterator<
        Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>    const_iterator;
    typedef __hashtable_node<Val>                          node;

    // __hashtable_const_iterator 的内嵌型别
    typedef forward_iterator_tag    iterator_category;
    typedef Val                     value_type;
    typedef const Val*              pointer;
    typedef const Val&              reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t               difference_type;

    const node*      cur;   // 迭代器目前所指节点
    const hashtable* ht;    // 保持与容器的连结

    // 构造函数
    __hashtable_const_iterator() {}
    __hashtable_const_iterator(const node* n, const hashtable* t) :cur(n), ht(t) {}
    __hashtable_const_iterator(const iterator& rhs) :cur(rhs.cur), ht(rhs.ht) {}

    // 重载操作符
    reference operator*()  const { return cur->value; }
    pointer   operator->() const { return &(operator*()); }
    const_iterator& operator++();
    const_iterator operator++(int) {
        const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

    // 重载比较操作符
    bool operator==(const const_iterator& rhs) const { return cur == rhs.cur; }
    bool operator!=(const const_iterator& rhs) const { return cur != rhs.cur; }
};

// 定义 __hashtable_const_iterator 的 operator++ 
template <class Val, class Key, class HashFcn, 
    class ExtractKey, class EqualKey, class Alloc>
__hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
    __hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
    const node* old = cur;  // 原来的节点
    cur = cur->next;
    if (!cur) {  // 如果下一个位置为空，跳到下一个 bucket 的起始处
        auto index = ht->__bkt_num(old->value);    // 找到下一个 bucket 的位置
        while (!cur && ++index < ht->buckets_.size())
            cur = ht->buckets_[index];
    }
    return *this;
}

// 用 28 个质数来设计表格大小
constexpr int kPrimeNum = 28;

static const unsigned long __prime_list[kPrimeNum] = {
    53ul,            97ul,            193ul,          389ul,          769ul, 
    1543ul,          3079ul,          6151ul,         12289ul,        24593ul,
    49157ul,         98317ul,         196613ul,       393241ul,       786433ul, 
    1572869ul,       3145739ul,       6291469ul,      12582917ul,     25165843ul, 
    50331653ul,      100663319ul,     201326611ul,    402653189ul,    805306457ul,
    1610612741ul,    3221225473ul,    4294967291ul
};

// 以下函数用于找出最接近并大于等于 n 的那个质数
inline unsigned long __next_prime(unsigned long n) {
    const unsigned long* first = __prime_list;
    const unsigned long* last = __prime_list + kPrimeNum;
    const unsigned long* pos = mystl::lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
}

// 模板类 hashtable
// 参数一代表实值类型，参数二代表键值类型，参数三代表哈希函数
// 参数四代表取出键值的方式，缺省使用 mystl 的 identity
// 参数五代表键值比较方式，缺省使用 mystl 的 equal_to，参数六代表空间配置器类型，缺省使用 mystl 的 alloc
// hashtable 内的元素不会自动排序
template <class Val, class Key, class HashFcn, class ExtractKey = mystl::identity<Key>, 
    class EqualKey = mystl::equal_to<Key>, class Alloc = mystl::alloc>
class hashtable {
  public:
    // hashtable 的型别定义
    typedef Val                            value_type;
    typedef Key                            key_type;
    typedef HashFcn                        hasher;
    typedef EqualKey                       key_equal;
    typedef Alloc                          allocator_type;

    typedef size_t                         size_type;
    typedef ptrdiff_t                      difference_type;
    typedef Val*                           pointer;
    typedef const Val*                     const_pointer;
    typedef Val&                           reference;
    typedef const Val&                     const_reference;
    typedef __hashtable_node<Val>          node;
    typedef mystl::vector<node*, Alloc>    bucket_type;

    friend struct mystl::__hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
    friend struct mystl::__hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
    typedef mystl::__hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
        iterator;
    typedef mystl::__hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
        const_iterator;
 
    typedef allocator<node, Alloc>    node_allocator;
    allocator_type get_allocator() const { return allocator_type(); }

    hasher    hash_fcn() const { return hash_; }   // 获取 hash function 的型别
    key_equal key_eq()   const { return equal_; }  // 获取从节点中取出键值的方法

private:
    // 用以下五个参数来表现 hashtable
    hasher      hash_;
    key_equal   equal_;
    ExtractKey  get_key_;
    size_type   element_nums_;
    bucket_type buckets_;

public:
    // 构造、复制、移动、析构函数
    explicit hashtable(size_type n, 
        const HashFcn& hf = HashFcn(), 
        const EqualKey& eqk = EqualKey(),
        const ExtractKey& exk = ExtractKey())
        :hash_(hf), equal_(eqk), get_key_(exk), element_nums_(0) {
        __hashtable_initialize(n);
    }

    hashtable(const hashtable& rhs) :hash_(rhs.hash_), equal_(rhs.equal_),
        get_key_(rhs.get_key_), element_nums_(0) {
        __copy_from(rhs);
    }
    hashtable(hashtable&& rhs) :hash_(rhs.hash_), equal_(rhs.equal_),
        get_key_(rhs.get_key_), element_nums_(rhs.element_nums_) {
        buckets_ = std::move(rhs.buckets_);
        rhs.element_nums_ = 0;
    }

    hashtable& operator=(const hashtable& rhs);
    hashtable& operator=(hashtable&& rhs);

    ~hashtable() { clear(); }

    // 迭代器相关操作
    iterator       begin();
    const_iterator begin() const;
    iterator       end()         { return iterator(static_cast<node*>(0), this); }
    const_iterator end()   const { return const_iterator(static_cast<node*>(0), this); }

    // 容量相关操作
    bool      empty()                      const { return element_nums_ == 0; }
    size_type size()                       const { return element_nums_; }
    size_type max_size()                   const { return static_cast<size_type>(-1); }
    size_type bucket_count()               const { return buckets_.size(); }
    size_type max_bucket_count()           const { return __prime_list[kPrimeNum - 1]; }
    size_type elems_in_bucket(size_type n) const;

    // 插入删除相关操作
    pair<iterator, bool> insert_unique(const value_type& value) {
        reserve(element_nums_ + 1);
        return insert_unique_noresize(value);
    }
    iterator insert_equal(const value_type& value) {
        reserve(element_nums_ + 1);
        return insert_equal_noresize(value);
    }

    pair<iterator, bool> insert_unique_noresize(const value_type& value);
    iterator insert_equal_noresize(const value_type& value);

    template <class InputIterator>
    void insert_unique(InputIterator first, InputIterator last) {
        insert_unique(first, last, iterator_category(first));
    }
    template <class InputIterator>
    void insert_equal(InputIterator first, InputIterator last) {
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
    void      reserve(size_type num_elements_hint);

    // hashtable 相关操作
    reference                            find_or_insert(const value_type& value);
    iterator                             find(const key_type& k);
    const_iterator                       find(const key_type& k) const;
    size_type                            count(const key_type& k) const;
    pair<iterator, iterator>             equal_range(const key_type& k);
    pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
    void                                 swap(hashtable& rhs);

private:
    // hashtable 成员函数
    node*     __get_node() { return node_allocator::allocate(1); }
    void      __put_node(node* p) { node_allocator::deallocate(p, 1); }
    size_type __next_size(size_type n) const;
    void      __hashtable_initialize(size_type n);
    size_type __bkt_num(const value_type& value, size_type n) const;
    size_type __bkt_num(const value_type& value) const;
    size_type __bkt_num_key(const key_type& key, size_type n) const;
    size_type __bkt_num_key(const key_type& key) const;
    node*     __create_node(const value_type& value);
    void      __delete_node(node* n);
    void      __erase_bucket(const size_type n, node* first, node* last);
    void      __erase_bucket(const size_type n, node* last);
    void      __copy_from(const hashtable& ht);
};

/*****************************************************************************************/

// 复制赋值运算符
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator=(const hashtable & rhs) {
    if (this != &rhs) {
        clear();
        hash_ = rhs.hash_;
        equal_ = rhs.equal_;
        get_key_ = rhs.get_key_;
        buckets_ = rhs.buckets_;
        element_nums_ = rhs.element_nums_;
    }
    return *this;
}

// 移动赋值运算符
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator=(hashtable&& rhs) {
    if (this != &rhs) {
        clear();
        hash_ = rhs.hash_;
        equal_ = rhs.equal_;
        get_key_ = rhs.get_key_;
        buckets_ = std::move(rhs.buckets_);
        element_nums_ = rhs.element_nums_;
        rhs.element_nums_ = 0;
    }
    return *this;
}

// begin 返回指向第一个节点的迭代器
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() {
    for (size_type n = 0; n < buckets_.size(); ++n) {
        if (buckets_[n])  // 找到第一个有节点的位置就返回
            return iterator(buckets_[n], this);
    }
    return end();         // 没有找到返回 end
}

template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() const {
    for (size_type n = 0; n < buckets_.size(); ++n) {
        if (buckets_[n])  // 找到第一个有节点的位置就返回
            return const_iterator(buckets_[n], this);
    }
    return end();         // 没有找到返回 end
}

// 在某个 bucket 节点的个数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::elems_in_bucket(size_type n) const {
    size_type result = 0;
    for (auto cur = buckets_[n]; cur; cur = cur->next)
        ++result;
    return result;
}

// 在不需要重建表格的情况下插入新节点，键值不允许重复
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool> 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    insert_unique_noresize(const value_type& value) {
    const auto n = __bkt_num(value);                        // value 位于第 n 个 bucket
    auto first = buckets_[n];                               // first 指向 第 n 个 bucket 的头部
    // 如果 bucket[n] 被占用，first不为空，进入循环，走访链表
    for (auto cur = first; cur; cur = cur->next) {
        if (equal_(get_key_(cur->value), get_key_(value)))  // 如果链表中存在相同键值的节点就返回
            return pair<iterator, bool>(iterator(cur, this), false);
    }
    auto tmp = __create_node(value);                        // 产生新节点
    tmp->next = first;
    buckets_[n] = tmp;                                      // 让新节点成为链表的第一个节点
    ++element_nums_;
    return pair<iterator, bool>(iterator(tmp, this), true);
}

// 在不需要重建表格的情况下插入新节点，键值允许重复
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    insert_equal_noresize(const value_type& value) {
    const auto n = __bkt_num(value);          // value 位于第 n 个 bucket
    auto first = buckets_[n];                 // first 指向 第 n 个 bucket 的头部
    // 如果 bucket[n] 被占用，first不为空，进入循环，走访链表
    for (auto cur = first; cur; cur = cur->next) {
        if (equal_(get_key_(cur->value), get_key_(value))) {
            // 如果链表中存在相同键值的节点就马上插入，然后返回
            auto tmp = __create_node(value);  // 产生新节点
            tmp->next = cur->next;            // 将新节点插入到目标节点后
            cur->next = tmp;
            ++element_nums_;
            return iterator(tmp, this);
        }
    }
    auto tmp = __create_node(value);          // 产生新节点
    tmp->next = first;
    buckets_[n] = tmp;                        // 让新节点成为链表的第一个节点
    ++element_nums_;
    return iterator(tmp, this);    
}

// insert_unique 的 input_iterator_tag 版本
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class InputIterator>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
    for (; first != last; ++first)
        insert_unique(*first);
}

// insert_equal 的 input_iterator_tag 版本
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class InputIterator>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    insert_equal(InputIterator first, InputIterator last, input_iterator_tag) {
    for (; first != last; ++first)
        insert_equal(*first);
}

// insert_unique 的 forward_iterator_tag 版本
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class ForwardIterator>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
    auto n = static_cast<size_type>(distance(first, last));
    reserve(element_nums_ + n);  // 调整大小
    for (; n > 0; --n, ++first)
        insert_unique_noresize(*first);
}

// insert_equal 的 forward_iterator_tag 版本
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
template<class ForwardIterator>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
    auto n = static_cast<size_type>(distance(first, last));
    reserve(element_nums_ + n);  // 调整大小
    for (; n > 0; --n, ++first)
        insert_equal_noresize(*first);
}

// 删除键值为 key 的节点
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    erase(const key_type & k) {
    const auto n = __bkt_num_key(k);                 // 键值为 k 的节点位于第 n 个 bucket
    auto first = buckets_[n];                        // first 指向第 n 个 bucket 的头部
    size_type erased = 0;                            // 已删除的元素个数
    if (first) {
        auto cur = first;
        auto next = cur->next;
        while (next) {
            if (equal_(get_key_(next->value), k)) {  // 如果找到键值为 k 的节点
                cur->next = next->next;    
                __delete_node(next);
                next = cur->next;
                ++erased;
                --element_nums_;
            }
            else {
                cur = next;
                next = cur->next;
            }
        }
        if (equal_(get_key_(first->value), k)) {     // 如果第一个节点键值为 k
            buckets_[n] = first->next;
            __delete_node(first);
            ++erased;
            --element_nums_;
        }
    }
    return erased;
}

// 删除迭代器所指的节点
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const iterator& it) {
    auto p = it.cur;
    if (p) {
        const auto    n = __bkt_num(p->value);
        auto cur = buckets_[n];
        if (cur == p) {           // p 位于链表头部
            buckets_[n] = cur->next;
            __delete_node(cur);
            --element_nums_;
        }
        else {
            auto next = cur->next;
            while (next) {        // 当没到链表尾部
                if (next == p) {  // 如果找到 p
                    cur->next = next->next;
                    __delete_node(next);
                    --element_nums_;
                    break;
                }
                else {
                    cur = next;
                    next = cur->next;
                }
            }
        }
    }
}

// 删除[first, last)内的节点
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    erase(iterator first, iterator last) {
    auto first_bucket = first.cur ? __bkt_num(first.cur->value) : buckets_.size();
    auto last_bucket = last.cur ? __bkt_num(last.cur->value) : buckets_.size();
    if (first.cur == last.cur) {
        return;
    }
    else if (first_bucket == last_bucket) {   // 如果在 bucket 在同一个位置
        __erase_bucket(first_bucket, first.cur, last.cur);
    }
    else {
        __erase_bucket(first_bucket, first.cur, static_cast<node*>(0));  // 删除 first_bucket
        for (auto n = first_bucket + 1; n < last_bucket; ++n)
            __erase_bucket(n, static_cast<node*>(0));                    // 删除 first 和 last 中间的 bucket
        if (last_bucket != buckets_.size())
            __erase_bucket(last_bucket, last.cur);                       // 删除剩余部分
    }
}

// 删除迭代器所指的节点
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    erase(const const_iterator& it) {
    erase(iterator(const_cast<node*>(it.cur), const_cast<hashtable*>(it.ht)));
}

// 删除[first, last)内的节点
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    erase(const_iterator first, const_iterator last) {
    erase(iterator(const_cast<node*>(first.cur), const_cast<hashtable*>(first.ht)),
        iterator(const_cast<node*>(last.cur), const_cast<hashtable*>(last.ht)));
}

// 清空 hashtable
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear() {
    if (element_nums_ != 0) {
        for (size_type i = 0; i < buckets_.size(); ++i) {
            node* cur = buckets_[i];
            while (cur != nullptr) {
                node* next = cur->next;
                __delete_node(cur);
                cur = next;
            }
            buckets_[i] = nullptr;
        }
        element_nums_ = 0;
    }
}

// 重新配置 hashtable 大小
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    reserve(size_type num_elements_hint) {
    // 若新增元素后，元素个数大于 bucket vector 的大小，就重建表格
    const auto old_num = buckets_.size();
    if (num_elements_hint > old_num) {                  // 如果需要重建
        const auto n = __next_size(num_elements_hint);  // 找到下一个质数
        if (n > old_num) {
            bucket_type tmp(n);    //建立新的 buckets
            try {
                for (size_type index = 0; index < old_num; ++index) {
                    auto first = buckets_[index];    
                    while (first != nullptr) {
                        auto new_bucket = __bkt_num(first->value, n);
                        buckets_[index] = first->next;  // 旧的 buckets_ 指向下一个节点
                        first->next = tmp[new_bucket];  // 当前节点插入到新的 bucket
                        tmp[new_bucket] = first;        // 使当前节点成为新 bucket 的第一个节点
                        first = buckets_[index];
                    }
                }
                buckets_.swap(tmp);                     // 和新的 bucket 交换
            }
            catch (...) {
                for (size_type index = 0; index < tmp.size(); ++index) {
                    while (tmp[index] != nullptr) {
                        auto next = tmp[index]->next;
                        __delete_node(tmp[index]);
                        tmp[index] = next;
                    }
                }
            }
        }
    }
}

// 如果找到实值为 value 的元素，就返回它，否则就插入新元素后再返回
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::reference 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    find_or_insert(const value_type& value) {
    reserve(element_nums_ + 1);
    auto n = __bkt_num(value);
    auto first = buckets_[n];
    for (auto cur = first; cur; cur = cur->next) {
        if (equal_(get_key_(cur->value), get_key_(value)))
            return cur->value;
    }
    auto tmp = __create_node(value);  //新建节点
    tmp->next = first;
    buckets_[n] = tmp;                //让新节点成为链表的第一个节点
    ++element_nums_;
    return tmp->value;
}

// 查找键值为 k 的节点，返回其迭代器
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    find(const key_type& k) {
    auto n = __bkt_num_key(k);
    node* first = buckets_[n];
    for (; first && !equal_(get_key_(first->value), k); first = first->next) {}
    return iterator(first, this);
}

template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    find(const key_type& k) const {
    auto n = __bkt_num_key(k);
    const node* first = buckets_[n];
    for (; first && !equal_(get_key_(first->value), k); first = first->next) {}
    return const_iterator(first, this);
}

// 查找键值为 k 出现的次数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    count(const key_type& k) const {
    const auto n = __bkt_num_key(k);
    size_type result = 0;
    for (const node* cur = buckets_[n]; cur; cur = cur->next) {
        if (equal_(get_key_(cur->value), k))  ++result;
    }
    return result;
}

// 查找与键值 k 相等的区间，返回一个 pair，指向相等区间的首尾
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator,
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator>
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& k) {
    typedef pair<iterator, iterator>    pii;
    const auto n = __bkt_num_key(k);
    for (auto first = buckets_[n]; first; first = first->next) {
        if (equal_(get_key_(first->value), k)) {             // 如果出现相等的键值
            for (auto cur = first->next; cur; cur = cur->next) {
                if (!equal_(get_key_(cur->value), k))        // 如果出现不相等的键值
                    return pii(iterator(first, this), iterator(cur, this));
            }
            for (auto m = n + 1; m < buckets_.size(); ++m) { // 整个链表都相等，查找下一个链表出现的位置
                if (buckets_[m])
                    return pii(iterator(first, this), iterator(buckets_[m], this));
            }
            return pii(iterator(first, this), end());
        }
    }
    return pii(end(), end());  // 没有找到
}

template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator,
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator>
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& k) const {
    typedef pair<const_iterator, const_iterator>    pii;
    const auto n = __bkt_num_key(k);
    for (const node* first = buckets_[n]; first; first = first->next) {
        if (equal_(get_key_(first->value), k)) {              // 如果出现相等的键值
            for (const node* cur = first->next; cur; cur = cur->next) {
                if (!equal_(get_key_(cur->value), k))         // 如果出现不相等的键值
                    return pii(const_iterator(first, this), const_iterator(cur, this));
            }
            for (auto m = n + 1; m < buckets_.size(); ++m) {  // 整个链表都相等，查找下一个链表出现的位置
                if (buckets_[m])
                    return pii(const_iterator(first, this), const_iterator(buckets_[m], this));
            }
            return pii(const_iterator(first, this), end());
        }
    }
    return pii(end(), end());  // 没有找到
}

// 交换 hashtable
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::swap(hashtable& rhs) {
    mystl::swap(hash_, rhs.hash_);
    mystl::swap(equal_, rhs.equal_);
    mystl::swap(get_key_, rhs.get_key_);
    buckets_.swap(rhs.buckets_);
    mystl::swap(element_nums_, rhs.element_nums_);
}

// __next_size 函数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::__next_size(size_type n) const {
    return __next_prime(static_cast<unsigned long>(n));
}

// __hashtable_initialize 函数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    __hashtable_initialize(size_type n) {
    const auto bucket_nums = __next_size(n);
    buckets_.reserve(bucket_nums);
    buckets_.insert(buckets_.end(), bucket_nums, static_cast<node*>(nullptr));
    element_nums_ = 0;
}

// __bkt_num 函数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    __bkt_num(const value_type & value, size_type n) const {
    return __bkt_num_key(get_key_(value), n);    
}

template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    __bkt_num(const value_type & value) const {
    return __bkt_num_key(get_key_(value));
}

// __bkt_num_key 函数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    __bkt_num_key(const key_type & key, size_type n) const {
    return hash_(key) % n;
}

template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    __bkt_num_key(const key_type & key) const {
    return __bkt_num_key(key, buckets_.size());
}

// __create_node 函数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node* 
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::__create_node(const value_type & value) {
    auto tmp = __get_node();
    tmp->next = nullptr;
    try {
        mystl::construct(&tmp->value, value);
    }
    catch (...) {
        __put_node(tmp);
    }
    return tmp;
}

// __delete_node 函数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::__delete_node(node* n) {
    mystl::destroy(&n->value);
    __put_node(n);
}

// __erase_bucket 函数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    __erase_bucket(const size_type n, node * first, node * last) {
    auto cur = buckets_[n];
    if (cur == first) {
        __erase_bucket(n, last);
    }
    else {
        node* next;
        for (next = cur->next; next != first; cur = next, next = cur->next);  // 找到first
        while (next != last) {
            cur->next = next->next;
            __delete_node(cur);
            next = cur->next;
            --element_nums_;
        }
    }
}

template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    __erase_bucket(const size_type n, node * last) {
    auto cur = buckets_[n];
    while (cur != last) {
        auto next = cur->next;
        __delete_node(cur);
        cur = next;
        buckets_[n] = cur;
        --element_nums_;
    }
}

// __copy_from 函数
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    __copy_from(const hashtable & ht) {
    buckets_.clear();
    buckets_.reserve(ht.buckets_.size());
    buckets_.insert(buckets_.end(), ht.buckets_.size(), static_cast<node*>(0));
    try {
        for (size_type i = 0; i < ht.buckets_.size(); ++i) {
            const node* cur = ht.buckets_[i];
            if (cur) {  // 如果某 bucket 存在链表
                auto copy = __create_node(cur->value);
                buckets_[i] = copy;
                for(auto next =cur->next;next;cur=next,next=cur->next){  //复制链表
                    copy->next = __create_node(next->value);
                    copy = copy->next;
                }
            }
        }
        element_nums_ = ht.element_nums_;
    }
    catch (...) {
        clear();
    }
}

// 重载比较操作符
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
bool operator==(const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
        const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs) {
    if (lhs.buckets_.size() != rhs.buckets_.size())  return false;
    for (auto n = 0; n < lhs.buckets_.size(); ++n) {
        auto cur1 = lhs.buckets_[n];
        auto cur2 = rhs.buckets_[n];
        for (; cur1 && cur2 && cur1->value == cur2->value;
            cur1 = cur1->next, cur2 = cur2->next) {
        }
        if (cur1 || cur2)    // cur1 或 cur2 还有元素
            return false;
    }
    return true;
}

template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline bool operator!=(const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
    const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs) {
    return !(lhs == rhs);
}

// 重载 mystl 的 swap
template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void swap(hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs) {
    lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_HASHTABLE_H_

