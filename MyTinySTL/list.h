#ifndef MYTINYSTL_LIST_H_
#define MYTINYSTL_LIST_H_

// 这个头文件包含了一个模板类 list
// list : 双向链表

#include "iterator.h"
#include "memory.h"
#include "functional.h"

#include <initializer_list>

namespace mystl {

// list 的节点结构
template <class T>
struct __list_node {
    T               data;  // 数据域
    __list_node<T>* prev;  // 指向前一个节点
    __list_node<T>* next;  // 指向下一个节点

    __list_node(T value = 0, __list_node<T>* p = nullptr, __list_node<T>* n = nullptr)
        :data(value), prev(p), next(n) {}
};

// list 的迭代器设计
template <class T, class Ref, class Ptr>
struct __list_iterator : public iterator<bidirectional_iterator_tag, T> {
    typedef __list_iterator<T, T&, T*>                iterator;
    typedef __list_iterator<T, const T&, const T*>    const_iterator;
    typedef __list_iterator<T, Ref, Ptr>              self;

    typedef T                                         value_type;
    typedef Ptr                                       pointer;
    typedef Ref                                       reference;
    typedef size_t                                    size_type;
    typedef ptrdiff_t                                 difference_type;

    typedef __list_node<T>*                           link_type;
    link_type node_;  // 指向当前节点

    // 构造函数
    __list_iterator() {}
    __list_iterator(link_type x) :node_(x) {}
    __list_iterator(const iterator& rhs) :node_(rhs.node_) {}

    // 重载操作符
    reference operator*()  const { return (*node_).data; }
    pointer   operator->() const { return &(operator*()); }

    self& operator++() {
        node_ = node_->next;
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--() {
        node_ = node_->prev;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    } 
    
    // 重载比较操作符
    bool operator==(const self& rhs) const { return node_ == rhs.node_; }
    bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
};

// 模板类 list
// 参数一代表数据类型，参数二代表空间配置器类型，缺省使用 mystl 的 alloc
template <class T, class Alloc = mystl::alloc>
class list {
  public:
    // list 的嵌套型别定义
    typedef T                                          value_type;
    typedef Alloc                                      allocator_type;
    typedef value_type*                                pointer;
    typedef const value_type*                          const_pointer;
    typedef value_type&                                reference;
    typedef const value_type&                          const_reference;
    typedef size_t                                     size_type;
    typedef ptrdiff_t                                  difference_type;

    typedef __list_iterator<T, T&, T*>                 iterator;
    typedef __list_iterator<T, const T&, const T*>     const_iterator;
    typedef mystl::reverse_iterator<iterator>          reverse_iterator;
    typedef mystl::reverse_iterator<const_iterator>    const_reverse_iterator;

    typedef __list_node<T>*                            link_type;

    typedef mystl::allocator<__list_node<T>, Alloc>    data_allocate;
    allocator_type get_allocator() { return allocator_type(); }

  private:
    // 只用一个节点来表示整个 list
    link_type node_;  // 指向尾端的一个空节点

  public:
    // 构造、复制、移动、析构函数
    list() { __list_initialize(); }
    explicit list(size_type n);
    list(size_type n, const T& value);
    template <class InputIterator>
    list(InputIterator first, InputIterator last);
    list(std::initializer_list<T> ilist);

    list(const list& rhs);
    list(list&& rhs);

    list& operator=(const list& rhs);
    list& operator=(list&& rhs);
    list& operator=(std::initializer_list<T> list);

    ~list();

  public:
    // 迭代器相关操作
    iterator               begin()        { return node_->next; }
    const_iterator         begin()  const { return node_->next; }
    iterator               end()          { return node_; }
    const_iterator         end()    const { return node_; }
    reverse_iterator       rbegin()       { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator       rend()         { return reverse_iterator(begin()); }
    const_reverse_iterator rend()   const { return reverse_iterator(begin()); }

    // 容量相关操作
    bool      empty()    const { return node_->next == node_; }
    size_type size()     const { return static_cast<size_type>(distance(begin(), end())); }
    size_type max_size() const { return static_cast<size_type>(-1); }

    // 访问元素相关操作
    reference       front()       { return *begin(); }
    const_reference front() const { return *begin(); }
    reference       back()        { return *(--end()); }
    const_reference back()  const { return *(--end()); }

    // 调整容器相关操作
    void     assign(size_type n)                          { __fill_assign(n, value_type()); }
    void     assign(size_type n, const value_type& value) { __fill_assign(n, value); }
    template <class InputIterator>
    void     assign(InputIterator first, InputIterator last);
    void     assign(std::initializer_list<T> ilist);
    iterator insert(iterator position)                    { return insert(position, value_type()); }
    iterator insert(iterator position, const value_type& value);
    void     insert(iterator position, size_type n, const value_type& value);
    template <class InputIterator>
    void     insert(iterator position, InputIterator first, InputIterator last);
    void     insert(iterator position, std::initializer_list<T> ilist);
    void     push_front(const value_type& value);
    void     push_back(const value_type& value);
    void     pop_front()                                  { erase(begin()); }
    void     pop_back()                                   { iterator tmp = end(); erase(--tmp); }
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void     clear();
    
    void     resize(size_type new_size)                   { resize(new_size, value_type()); }
    void     resize(size_type new_size, const value_type& value);
    void     swap(list& rhs)                              { mystl::swap(node_, rhs.node_); }

    // list 相关操作
    void splice(iterator position, list& x);
    void splice(iterator position, list&, iterator i);
    void splice(iterator position, list& x, iterator first, iterator last);
    void remove(const value_type& value);
    template <class Predicate>
    void remove_if(Predicate pred);
    void unique();
    void merge(list& x);
    void sort();
    template <class Compared>
    void sort(Compared comp);
    void reverse();

  private:
    // list 的私有成员函数
    link_type __get_node();
    void      __put_node(link_type p);
    link_type __create_node(const value_type& value);
    void      __destroy_node(link_type p);
    void      __list_initialize();
    void      __link_nodes(link_type p, link_type first, link_type last);
    void      __link_nodes_at_front(link_type first, link_type last);
    void      __link_nodes_at_back(link_type first, link_type last);
    void      __unlink_nodes(link_type f, link_type l);
    void      __fill_assign(size_type n, const value_type& value);
    template <class Integer>
    void      __assign_dispatch(Integer n, Integer value, __true_type);
    template <class InputIterator>
    void      __assign_dispatch(InputIterator first, InputIterator last, __false_type);
    void      __fill_insert(iterator position, size_type n, const value_type& value);
    template <class Integer>
    void      __insert_dispatch(iterator position, Integer n, Integer value, __true_type);
    template <class InputIterator>
    void      __insert_dispatch(iterator position, InputIterator first, InputIterator last, __false_type);
    template <class Compared>
    iterator  __sort(iterator first, iterator last, size_type n, Compared comp);
    void      __splice(iterator position, list& l, iterator first, iterator last);
};

/*****************************************************************************************/

// 构造函数
template <class T, class Alloc>
inline list<T, Alloc>::
list(size_type n) {
    __list_initialize(); 
    insert(end(), n, value_type());
}

template <class T, class Alloc>
inline list<T, Alloc>::
list(size_type n, const T& value) {
    __list_initialize(); 
    insert(end(), n, value);
}

template <class T, class Alloc>
template <class InputIterator>
inline list<T, Alloc>::
list(InputIterator first, InputIterator last) {
    __list_initialize();
    insert(end(), first, last);
}

template<class T, class Alloc>
inline list<T, Alloc>::
list(std::initializer_list<T> ilist) {
    __list_initialize();
    insert(end(), ilist.begin(), ilist.end());
}

// 复制构造函数
template <class T, class Alloc>
inline list<T, Alloc>::
list(const list& rhs) { 
    __list_initialize();
    insert(begin(), rhs.begin(), rhs.end());
}

// 移动构造函数
template <class T, class Alloc>
inline list<T, Alloc>::
list(list&& rhs) {
    node_ = rhs.node_;
    rhs.node_ = nullptr;
}

// 复制赋值运算符
template <class T, class Alloc>
inline list<T, Alloc>& list<T, Alloc>::
operator=(const list& rhs) {
    if (this != &rhs) {
        auto first1 = begin();
        auto last1 = end();
        auto first2 = rhs.begin();
        auto last2 = rhs.end();
        while (first1 != last1 && first2 != last2)
            *first1++ = *first2++;
        if (first2 == last2)
            erase(first1, last1);
        else
            insert(last1, first2, last2);
    }
    return *this;
}

// 移动赋值运算符
template <class T, class Alloc>
inline list<T, Alloc>& list<T, Alloc>::
operator=(list&& rhs) {
    if (this != &rhs) {
        clear();
        node_ = rhs.node_;
        rhs.node_ = nullptr;
    }
    return *this;
}

// 使用初值列表赋值
template<class T, class Alloc>
inline list<T, Alloc>& list<T, Alloc>::
operator=(std::initializer_list<T> ilist) {
    list tmp(ilist.begin(), ilist.end());
    *this = std::move(tmp);
    return *this;
}

// 析构函数
template <class T, class Alloc>
inline list<T, Alloc>::~list() {
    if (node_) {
        clear();
        __put_node(node_);
        node_ = nullptr;
    }
}

// 初始化容器
template <class T, class Alloc>
template <class InputIterator>
inline void list<T, Alloc>::
assign(InputIterator first, InputIterator last) {
    typedef typename __is_integer<InputIterator>::is_integer Integer;
    __assign_dispatch(first, last, Integer());
}

// 使用初值列表赋值给容器
template<class T, class Alloc>
inline void list<T, Alloc>::
assign(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
}

// 在 position 处插入元素
template <class T, class Alloc>
inline typename list<T, Alloc>::iterator list<T, Alloc>::
insert(iterator position, const value_type& value) {
    auto tmp = __create_node(value);
    if (position == begin()) {
        __link_nodes_at_front(tmp, tmp);
        return tmp;
    }
    else if (position == end()) {
        __link_nodes_at_back(tmp, tmp);
        return tmp;
    }
    else {
        __link_nodes(position.node_, tmp, tmp);
    }
    return tmp;
}

// 在 position 处插入 n 个元素
template <class T, class Alloc>
inline void list<T, Alloc>::
insert(iterator position, size_type n, const value_type& value) {
    __fill_insert(position, n, value);
}

// 在 position 处插入 [first, last)区间内的元素
template <class T, class Alloc>
template <class InputIterator>
inline void list<T, Alloc>::
insert(iterator position, InputIterator first, InputIterator last) {
    typedef typename __is_integer<InputIterator>::is_integer Integer;
    __insert_dispatch(position, first, last, Integer());
}

// 在 position 处插入初值列表
template<class T, class Alloc>
inline void list<T, Alloc>::
insert(iterator position, std::initializer_list<T> ilist) {
    insert(position, ilist.begin(), ilist.end());
}

// 在头部插入元素
template<class T, class Alloc>
inline void list<T, Alloc>::
push_front(const value_type& value) {
    auto tmp = __create_node(value);
    __link_nodes_at_front(tmp, tmp);
}

// 在尾部插入元素
template<class T, class Alloc>
inline void list<T, Alloc>::
push_back(const value_type& value) {
    auto tmp = __create_node(value);
    __link_nodes_at_back(tmp, tmp);
}

// 删除 position 处的元素
template <class T, class Alloc>
inline typename list<T, Alloc>::iterator list<T, Alloc>::
erase(iterator position) {
    link_type prev_node = position.node_->prev;
    link_type next_node = position.node_->next;
    link_type this_node = position.node_;
    prev_node->next = next_node;
    next_node->prev = prev_node;
    __destroy_node(this_node);
    return iterator(next_node);
}

// 删除[first, last)内的元素
template <class T, class Alloc>
inline typename list<T, Alloc>::iterator list<T, Alloc>::
erase(iterator first, iterator last) {
    while (first != last) {
        auto cur = first;
        ++first;
        erase(cur);
    }
    return last;
}

// 重置容器大小
template <class T, class Alloc>
inline void list<T, Alloc>::
resize(size_type new_size, const value_type& value) {
    auto i = begin();
    size_type len = 0;
    while (i != end() && len < new_size) {
        ++i;
        ++len;
    }
    if (len == new_size)    
        erase(i, end());
    else    
        insert(end(), new_size - len, value);
}

// 清空 list
template <class T, class Alloc>
inline void list<T, Alloc>::
clear() {
    if (!empty()) {
        link_type cur = node_->next;
        node_->prev = node_;
        node_->next = node_;
        for (link_type next; cur != node_; cur = next) {
            next = cur->next;
            __destroy_node(cur);
        }
    }
}

// 将 list x 接合于 position 之前
template <class T, class Alloc>
inline void list<T, Alloc>::
splice(iterator position, list& x) {
    if (this != &x && !x.empty()) {
        __splice(position, x, x.begin(), x.end());
    }
}

// 将 first 所指的元素接合于 position 之前
template <class T, class Alloc>
inline void list<T, Alloc>::
splice(iterator position, list& x, iterator first) {
    if (first != x.end()) {
        iterator last = first;
        ++last;
        if (this != &x || (position != first && position != last)) {
            __splice(position, x, first, last);
        }
    }
}

// 将 [first, last) 内的所有元素接合于 position 之前
template <class T, class Alloc>
inline void list<T, Alloc>::
splice(iterator position, list& x, iterator first, iterator last) {
    if (first != last && (this != &x || position != last)) {
        __splice(position, x, first, last);
    }
}

// 将数值为 value 的所有元素移除
template <class T, class Alloc>
inline void list<T, Alloc>::
remove(const value_type& value) {
    auto first = begin();
    auto last = end();
    while (first != last) {
        auto next = first;
        ++next;
        if (*first == value)  
            erase(first);
        first = next;
    }
}

// 将另一元操作 unary_pred 为 true 的所有元素移除
template <class T, class Alloc>
template <class Predicate>
inline void list<T, Alloc>::
remove_if(Predicate unary_pred) {
    auto first = begin();
    auto last = end();
    while (first != last) {
        auto next = first;
        ++next;
        if (unary_pred(*first)) 
            erase(first);
        first = next;
    }
}

// 移除数值连续相同的元素
template <class T, class Alloc>
inline void list<T, Alloc>::
unique() {
    auto first = begin();
    auto last = end();
    if (first == last) 
        return;
    auto next = first;
    while (++next != last) {
        if (*first == *next)
            erase(next);
        else
            first = next;
        next = first;
    }
}

// 与另一个 list 合并，两个 list 应保证有序
template <class T, class Alloc>
inline void list<T, Alloc>::
merge(list<T, Alloc>& x) {
    auto first1 = begin();
    auto last1 = end();
    auto first2 = x.begin();
    auto last2 = x.end();
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            auto next = first2;
            __splice(first1, x, first2, ++next);
            first2 = next;
        }
        else {
            ++first1;
        }
    }
    if (first2 != last2)
        __splice(last1, x, first2, last2);
}

// 将 list 按升序排列
template <class T, class Alloc>
inline void list<T, Alloc>::
sort() {
    __sort(begin(), end(), size(), mystl::less<T>());
}

// 将 list 按二元谓词 comp 排序
template <class T, class Alloc>
template <class Compared>
inline void list<T, Alloc>::
sort(Compared comp) {
    __sort(begin(), end(), size(), comp);
}

// 将 list 反转
template <class T, class Alloc>
inline void list<T, Alloc>::
reverse() {
    if (node_->next == node_ || node_->next->next == node_) 
        return;
    auto first = begin();
    ++first;
    while (first != end()) {
        auto old = first;
        ++first;
        __splice(begin(), *this, old, first);
    }
}

// 获取结点空间
template <class T, class Alloc>
inline typename list<T, Alloc>::link_type list<T, Alloc>::
__get_node() {
    return data_allocate::allocate(1);
}

// 释放结点空间
template <class T, class Alloc>
inline void list<T, Alloc>::
__put_node(link_type p) {
    data_allocate::deallocate(p);
}

// 创建结点
template <class T, class Alloc>
inline typename list<T, Alloc>::link_type list<T, Alloc>::
__create_node(const value_type& value) {
    auto p = __get_node();
    try {
        mystl::construct(p, __list_node<T>(value));
    }
    catch (...) {
        data_allocate::deallocate(p);
    }
    return p;
}

// 销毁结点
template <class T, class Alloc>
inline void list<T, Alloc>::__destroy_node(link_type p) {
    mystl::destroy(&p->data);
    __put_node(p);
}

// 初始化 list
template <class T, class Alloc>
inline void list<T, Alloc>::__list_initialize() {
    node_ = __get_node();
    node_->prev = node_->next = node_;
}

// 在 pos 处连接 [first, last] 结点
template<class T, class Alloc>
inline void list<T, Alloc>::
__link_nodes(link_type pos, link_type first, link_type last) {
    pos->prev->next = first;
    first->prev = pos->prev;
    pos->prev = last;
    last->next = pos;
}

// 在头部连接 [first, last] 结点
template<class T, class Alloc>
inline void list<T, Alloc>::
__link_nodes_at_front(link_type first, link_type last) {
    first->prev = node_;
    last->next = node_->next;
    last->next->prev = last;
    node_->next = first;
}

// 在尾部连接 [first, last] 结点
template<class T, class Alloc>
inline void list<T, Alloc>::
__link_nodes_at_back(link_type first, link_type last) {
    last->next = node_;
    first->prev = node_->prev;
    first->prev->next = first;
    node_->prev = last;
}

// 与 [first, last] 的结点断开连接
template<class T, class Alloc>
inline void list<T, Alloc>::
__unlink_nodes(link_type first, link_type last) {
    first->prev->next = last->next;
    last->next->prev = first->prev;
}

// 填充元素
template <class T, class Alloc>
void list<T, Alloc>::
__fill_assign(size_type n, const value_type& value) {
    auto i = begin();
    auto e = end();
    for (; n > 0 && i != e; --n, ++i) {
        *i = value;
    }
    if (n > 0)
        insert(e, n, value);
    else
        erase(i, e);
}

// 赋值元素
template <class T, class Alloc>
template <class Integer>
inline void list<T, Alloc>::
__assign_dispatch(Integer n, Integer value, __true_type) {
    __fill_assign(static_cast<size_type>(n), static_cast<T>(value));
}

template <class T, class Alloc>
template <class InputIterator>
void list<T, Alloc>::
__assign_dispatch(InputIterator first2, InputIterator last2,
    __false_type) {
    auto first1 = begin();
    auto last1 = end();
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        *first1 = *first2;
    }
    if (first2 == last2)
        erase(first1, last1);
    else
        insert(last1, first2, last2);
}

// 在 position 处插入 n 个元素
template <class T, class Alloc>
inline void list<T, Alloc>::
__fill_insert(iterator position, size_type n, const value_type& value) {
    for (; n > 0; --n) {
        insert(position, value);
    }
}

// 插入元素
template <class T, class Alloc>
template <class Integer>
inline void list<T, Alloc>::
__insert_dispatch(iterator position, Integer n, Integer value, __true_type) {
    __fill_insert(position, static_cast<size_type>(n), static_cast<T>(value));
}

// 在 position 处插入 [first, last) 的元素
template <class T, class Alloc>
template <class InputIterator>
inline void list<T, Alloc>::
__insert_dispatch(iterator position, InputIterator first, InputIterator last, __false_type) {
    for (; first != last; ++first) {
        insert(position, *first);
    }
}

// 对 list 排序
template<class T, class Alloc>
template<class Compared>
typename list<T, Alloc>::iterator list<T, Alloc>::
__sort(iterator first1, iterator last2, size_type n, Compared comp) {
    if (n < 2)
        return first1;

    if (n == 2) {
        if (comp(*--last2, *first1)) {
            link_type first = last2.node_;
            __unlink_nodes(first, first);
            __link_nodes(first1.node_, first, first);
            return last2;
        }
        return first1;
    }

    auto n2 = n / 2;
    auto last1 = first1;
    advance(last1, n2);
    auto result = first1 = __sort(first1, last1, n2, comp);
    auto first2 = last1 = __sort(last1, last2, n - n2, comp);
    if (comp(*first2, *first1)) {
        auto mid = first2;
        ++mid;
        for (; mid != last2 && comp(*mid, *first1); ++mid) {}
        link_type first = first2.node_;
        link_type last = mid.node_->prev;
        result = first2;
        last1 = first2 = mid;
        __unlink_nodes(first, last);
        mid = first1;
        ++mid;
        __link_nodes(first1.node_, first, last);
        first1 = mid;
    }
    else {
        ++first1;
    }

    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            auto mid = first2;
            ++mid;
            for (; mid != last2 && comp(*mid, *first1); ++mid) {}
            link_type first = first2.node_;
            link_type last = mid.node_->prev;
            if (last1 == first2)
                last1 = mid;
            first2 = mid;
            __unlink_nodes(first, last);
            mid = first1;
            ++mid;
            __link_nodes(first1.node_, first, last);
            first1 = mid;
        }
        else {
            ++first1;
        }
    }
    return result;
}

// 将 [first, last) 内的所有元素移到 position 之前
template <class T, class Alloc>
inline void list<T, Alloc>::
__splice(iterator position, list& l, iterator first, iterator last) {
    // 将[first, last)从原来的位置移出
    first.node_->prev->next = last.node_;
    last.node_->prev->next = position.node_;
    position.node_->prev->next = first.node_;
    // 将[first, last)接合到新的位置
    link_type tmp = position.node_->prev;
    position.node_->prev = last.node_->prev;
    last.node_->prev = first.node_->prev;
    first.node_->prev = tmp;
}

// 重载比较操作符
template <class T, class Alloc>
inline bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    typedef typename list<T, Alloc>::const_iterator const_iterator;
    const_iterator first1 = lhs.begin();
    const_iterator first2 = rhs.begin();
    const_iterator last1 = lhs.end();
    const_iterator last2 = rhs.end();
    while (first1 != last1 && first2 != last2 && *first1 == *first2) {
        ++first1;
        ++first2;
    }
    return first1 == last1 && first2 == last2;
}

template <class T, class Alloc>
inline bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Alloc>
inline bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Alloc>
inline bool operator>(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return rhs < lhs;
}

template <class T, class Alloc>
inline bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Alloc>
inline bool operator>=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T, class Alloc>
inline void swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs) {
    lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_LIST_H_

