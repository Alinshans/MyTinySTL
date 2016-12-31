#ifndef MYTINYSTL_RB_TREE_H_
#define MYTINYSTL_RB_TREE_H_

// 这个头文件包含一个模板类 rb_tree
// rb_tree : 红黑树

#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "pair.h"

namespace mystl {

// rb tree 节点颜色的类型
typedef enum { __rb_tree_red = false, __rb_tree_black = true }__rb_tree_color_type;

// rb tree 的节点设计
struct __rb_tree_node_base {
    typedef __rb_tree_color_type    color_type;
    typedef __rb_tree_node_base*    base_ptr;
    color_type color;   // 节点颜色
    base_ptr   parent;  // 父节点
    base_ptr   left;    // 左子节点
    base_ptr   right;   // 右子节点

    static base_ptr minimum(base_ptr x) {
        while (x->left != nullptr)  x = x->left;    // 一直向左走，找到最小值
        return x;
    }
    static base_ptr maximum(base_ptr x) {
        while (x->right != nullptr)  x = x->right;  // 一直向右走，找到最大值
        return x;
    }
};

template <class T>
struct __rb_tree_node :public __rb_tree_node_base {
    typedef __rb_tree_node<T>*    link_type;
    T value;  // 节点值
};

// rb tree 的迭代器设计
struct __rb_tree_iterator_base {
    typedef __rb_tree_node_base::base_ptr    base_ptr;
    typedef bidirectional_iterator_tag       iterator_category;
    typedef ptrdiff_t                        difference_type;

    base_ptr node;  // 指向节点本身

    // 使迭代器前进
    void incremet() {
        if (node->right) {              // 如果有右子节点
            node = node->right;         // 就向右走
            while (node->left)          // 然后一直向左走
                node = node->left;      // 找到下一个节点
        }
        else {                          // 如果没有右子节点
            base_ptr y = node->parent;  // 找到父节点
            while (y->right == node) {  // 如果该节点为父节点的右子节点
                node = y;               // 一直上溯，直到节点不为右子节点
                y = y->parent;
            }
            if (node->right != y)       // 应对“寻找根节点的下一节点，而根节点没有右子节点”的特殊情况
                node = y;
        }
    }

    // 使迭代器后退
    void decrement() {
        if (node->color == __rb_tree_red && node->parent->parent == node) {
            // 如果 node 为 header
            node = node->right;         // 指向整棵树的 max 节点
        }
        else if (node->left) {          // 如果有左子节点
            base_ptr y = node->left;    // 向左走
            while (y->right)            // 然后一直向右走
                y = y->right;
            node = y;                   // 找到前一个节点
        }
        else {                          // 非 header 节点，也无左子节点
            base_ptr y = node->parent;  // 找到父节点
            while (node == y->left) {   // 如果该节点为父节点的左子节点
                node = y;               // 一直上溯直到不为左子节点
                y = y->parent;
            }
            node = y;                   // 此时父节点为要找的前一个节点
        }
    }

    bool operator==(const __rb_tree_iterator_base& rhs) { return node == rhs.node; }
    bool operator!=(const __rb_tree_iterator_base& rhs) { return node != rhs.node; }
};

template <class T, class Ref, class Ptr>
struct __rb_tree_iterator :public __rb_tree_iterator_base {
    typedef T                                            value_type;
    typedef Ref                                          reference;
    typedef Ptr                                          pointer;
    typedef __rb_tree_iterator<T, T&, T*>                iterator;
    typedef __rb_tree_iterator<T, const T&, const T*>    const_iterator;
    typedef __rb_tree_iterator<T, Ref, Ptr>              self;
    typedef __rb_tree_node<T>*                           link_type;

    // 构造函数
    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type x)         { node = x; }
    __rb_tree_iterator(const iterator& rhs) { node = rhs.node; }

    // 重载操作符
    reference operator*() const { return ((link_type)node)->value; }
    pointer operator->()  const { return &(operator*()); }

    self& operator++() {
        incremet();
        return *this;
    }
    self operator++(int) {
        self tmp(*this);
        incremet();
        return tmp;
    }
    self& operator--() {
        decrement();
        return *this;
    }
    self operator--(int) {
        self tmp(*this);
        decrement();
        return tmp;
    }
};

// 以下四个全局函数，用于调整 rb tree 节点的位置与颜色
// 左旋，参数一为左旋点，参数二为根节点
inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    auto y = x->right;                // 设置 y 为旋转点的右子节点
    x->right = y->left;               // x 的右子节点为 y 的左子节点
    if (y->left)    
        y->left->parent = x;          // 设置 y 的左子节点的父节点为 x
    y->parent = x->parent;            // 设置 y 的父节点
    if (x == root) {                  // 如果 x 为根节点
        root = y;
    }
    else if (x == x->parent->left) {  // 如果 x 是它父节点的左子节点
        x->parent->left = y;          // 设置 x 的父节点的左子节点为 y
    }
    else {                            // 如果 x 是它父节点的右子节点
        x->parent->right = y;         // 设置 x 的父节点的右子节点为 y
    }
    y->left = x;                      // 连接 x 与 y 的关系
    x->parent = y;    
}

// 右旋，参数一为右旋点，参数二为根节点
inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    auto y = x->left;                  // 设置 y 为右旋点的左子节点
    x->left = y->right;                // 设置 x 的左子节点为 y 的右子节点
    if (y->right)
        y->right->parent = x;          // 设置 y 右子节点的父节点为 x
    y->parent = x->parent;             // 设置 y 的父节点为 x 的父节点
    if (x == root) {                   // 如果 x 为根节点
        root = y;                      // 根节点调整为 y
    }
    else if (x == x->parent->right) {  // 如果 x 是它父节点的右子节点
        x->parent->right = y;          // 将 x 的父节点的右子节点设置成 y
    }
    else {                             // 如果 x 是它父节点的左子节点
        x->parent->left = y;           // 将 x 的父节点的左子节点设置成 y
    }
    y->right = x;                      // 连接 x 与 y 的关系
    x->parent = y;    
}

// 重新使 rb tree 平衡，参数一为新增节点，参数二为根节点
inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    x->color = __rb_tree_red;                                 // 新增节点为红色
    while (x != root && x->parent->color == __rb_tree_red) {  // 当父节点为红
        if (x->parent == x->parent->parent->left) {           // 如果父节点是左子节点
            auto y = x->parent->parent->right;    
            if (y && y->color == __rb_tree_red) {             // 伯父节点存在且为红
                x->parent->color = __rb_tree_black;           // 更改父节点为黑
                y->color = __rb_tree_black;                   // 更改伯父节点为黑
                x->parent->parent->color = __rb_tree_red;     // 更改祖父节点为红
                x = x->parent->parent;
            }
            else {                                            // 无伯父节点或伯父节点为黑
                if (x == x->parent->right) {                  // 如果 x 为父节点的右子节点
                    x = x->parent;
                    __rb_tree_rotate_left(x, root);
                }
                x->parent->color = __rb_tree_black;           // 调整颜色
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_right(x->parent->parent, root);
            }
        }
        else {                                                // 如果父节点是右子节点，对称处理
            auto y = x->parent->parent->left;    
            if (y && y->color == __rb_tree_red) {             // 伯父节点存在且为红
                x->parent->color = __rb_tree_black;           // 更改父节点为黑
                y->color = __rb_tree_black;                   // 更改伯父节点为黑
                x->parent->parent->color = __rb_tree_red;     // 更改祖父节点为红
                x = x->parent->parent;
            }
            else {                                            // 无伯父节点或伯父节点为黑
                if (x == x->parent->left) {                   // 如果x为父节点的左子节点
                    x = x->parent;
                    __rb_tree_rotate_right(x, root);
                }
                x->parent->color = __rb_tree_black;           // 调整颜色
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }
    root->color = __rb_tree_black;                            // 根节点永远为黑
}

// 因删除节点而要使 rb tree 重新平衡，参数一为要删除的节点，参数二为根节点，参数三为最小节点，参数四为最大节点
inline __rb_tree_node_base* __rb_tree_rebalance_for_erase(__rb_tree_node_base* z,
    __rb_tree_node_base*& root, __rb_tree_node_base*& leftmost, __rb_tree_node_base*& rightmost) {
    auto y = z;
    __rb_tree_node_base* x = nullptr;
    __rb_tree_node_base* x_parent = nullptr;
    // 寻找可以代替的节点
    if (y->left == nullptr) {                 // 如果被删节点的左子节点为空
        x = y->right;
    }
    else {
        if (y->right == nullptr) {            // 如果被删节点的右子节点为空
            x = y->left;
        }
        else {                                // 如果被删节点的左右节点都不为空
            y = y->right;
            while (y->left != nullptr)        // 从被删节点的右子节点一直向左走，找到要替代的节点
                y = y->left;
            x = y->right;
        }
    }
    // 调整代替节点的节点关系
    if (y != z) {                             // y 变换了
        z->left->parent = y;                  // 用 y 替代被删节点
        y->left = z->left;                    // 设置新的左子节点
        if (y != z->right) {
            x_parent = y->parent;
            if (x)  x->parent = y->parent;    // 调整替代节点的右子节点的父子关系
            y->parent->left = x;
            y->right = z->right;              // 设置新的右子节点
            z->right->parent = y;
        }
        else {
            x_parent = y;
        }
        if (root == z) {                      // 如果被删节点为根节点
            root = y;                         // 另替代节点为根节点
        }
        else if (z->parent->left == z) {      // 调整替代节点的父子关系
            z->parent->left = y;
        }
        else {
            z->parent->right = y;    
        }
        y->parent = z->parent;
        mystl::swap(y->color, z->color);
        y = z;                                // 另 y 指向要删除的节点
    }
    else {                                    // y == z，x 为替代节点
        x_parent = y->parent;    
        if (x)  x->parent = y->parent;
        if (root == z) {                      // 如果被删节点为根节点
            root = x;
        }
        else {                                // 调整替代节点的父子关系
            if (z->parent->left == z)
                z->parent->left = x;
            else
                z->parent->right = x;
        }
        if (leftmost == z) {                  // 被删节点为最小节点，调整最小值
            if (z->right == nullptr)
                leftmost = z->parent;
            else
                leftmost = __rb_tree_node_base::minimum(x);
        }
        if (rightmost == z) {                 // 被删节点为最大节点，调整最大值
            if (z->left == nullptr)
                rightmost = z->parent;
            else
                rightmost = __rb_tree_node_base::maximum(x);
        }
    }
    // 调整节点颜色
    if (y->color != __rb_tree_red) {    // 如果被删节点颜色为黑
        while (x != root && (x == nullptr || x->color == __rb_tree_black)) {
            if (x == x_parent->left) {  // 如果替代节点是左子节点
                auto w = x_parent->right;
                if (w->color == __rb_tree_red) {
                    w->color = __rb_tree_black;
                    x_parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(x_parent, root);
                    w = x_parent->right;
                }
                if ((w->left == nullptr || w->left->color == __rb_tree_black) && 
                    (w->right == nullptr || w->right->color == __rb_tree_black)) {
                    w->color = __rb_tree_red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else {
                    if (w->right == nullptr || w->right->color == __rb_tree_black) {
                        if (w->left)  w->left->color = __rb_tree_black;
                        w->color = __rb_tree_red;
                        __rb_tree_rotate_right(w, root);
                        w = x_parent->right;
                    }
                    w->color = x_parent->color;
                    x_parent->color = __rb_tree_black;
                    if (w->right)  w->right->color = __rb_tree_black;
                    __rb_tree_rotate_left(x_parent, root);
                    break;
                }
            }
            else {                      // 如果替代节点为右子节点，对称处理
                auto w = x_parent->left;
                if (w->color == __rb_tree_red) {
                    w->color = __rb_tree_black;
                    x_parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(x_parent, root);
                    w = x_parent->left;
                }
                if ((w->left == nullptr || w->left->color == __rb_tree_black) &&
                    (w->right == nullptr || w->right->color == __rb_tree_black)) {
                    w->color = __rb_tree_red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else {
                    if (w->left == nullptr || w->left->color == __rb_tree_black) {
                        if (w->right)  w->right->color = __rb_tree_black;
                        w->color = __rb_tree_red;
                        __rb_tree_rotate_left(w, root);
                        w = x_parent->left;
                    }
                    w->color = x_parent->color;
                    x_parent->color = __rb_tree_black;
                    if (w->left)  w->left->color = __rb_tree_black;
                    __rb_tree_rotate_right(x_parent, root);
                    break;
                }
            }
        }   
        if (x)  x->color = __rb_tree_black;
    }
    return y;
}

// 模板类 rb_tree
// 参数一代表键值类型，参数二代表实值类型，参数三代表实值转化成键值的方式，缺省使用 mystl 的 identity
// 参数四代表键值比较的方法，缺省使用 mystl 的 less，参数代表为空间配置器的类型，缺省使用 mystl 的 alloc
template <class Key, class Value, class KeyOfValue = mystl::identity<Key>,
    class Compare = mystl::less<Key>, class Alloc = mystl::alloc>
class rb_tree {
  private:
    typedef __rb_tree_node_base*     base_ptr;
    typedef __rb_tree_node<Value>    rb_tree_node;
    typedef __rb_tree_color_type     color_type;

  public:
    // rb tree 的嵌套型别定义
    typedef Alloc                                                    allocator_type;
    typedef Key                                                      key_type;
    typedef Value                                                    value_type;
    typedef value_type*                                              pointer;
    typedef const value_type*                                        const_pointer;
    typedef value_type&                                              reference;
    typedef const value_type&                                        const_reference;
    typedef rb_tree_node*                                            link_type;
    typedef size_t                                                   size_type;
    typedef ptrdiff_t                                                difference_type;

    typedef __rb_tree_iterator<Value, Value&, Value*>                iterator;
    typedef __rb_tree_iterator<Value, const Value&, const Value*>    const_iterator;
    typedef mystl::reverse_iterator<iterator>                        reverse_iterator;
    typedef mystl::reverse_iterator<const_iterator>                  const_reverse_iterator;

    typedef mystl::allocator<rb_tree_node, Alloc>                    rb_tree_node_allocator;
    allocator_type get_allocator() const { return allocator_type(); }

  private:
    // 用以下三个数据表现 rb tree
    size_type node_count_;  // rb tree 的节点数目
    link_type header_;      // 特殊节点，与根节点互为对方的父节点
    Compare   key_compare;  // 节点键值比较的准则

  private:
    // 节点相关操作
    link_type __get_node()            { return rb_tree_node_allocator::allocate(1); }
    void      __put_node(link_type p) { rb_tree_node_allocator::deallocate(p, 1); }
    link_type __create_node(const value_type& x);
    link_type __clone_node(link_type x);
    void      __destroy_node(link_type p);

  private:
    // 以下三个函数用于取得根节点，最小节点和最大节点
    link_type&             root()        const { return reinterpret_cast<link_type&>(header_->parent); }
    link_type&             leftmost()    const { return reinterpret_cast<link_type&>(header_->left); }
    link_type&             rightmost()   const { return reinterpret_cast<link_type&>(header_->right); }

    // 以下六个函数用来取得节点的成员
    static link_type&      left(link_type x)   { return reinterpret_cast<link_type&>(x->left); }
    static link_type&      right(link_type x)  { return reinterpret_cast<link_type&>(x->right); }
    static link_type&      parent(link_type x) { return reinterpret_cast<link_type&>(x->parent); }
    static reference       value(link_type x)  { return x->value; }
    static const key_type& key(link_type x)    { return KeyOfValue()(value(x)); }
    static color_type&     color(link_type x)  { return x->color; }

    // 以下六个函数用来取得基层节点的成员
    static link_type&      left(base_ptr x)    { return reinterpret_cast<link_type&>(x->left); }
    static link_type&      right(base_ptr x)   { return reinterpret_cast<link_type&>(x->right); }
    static link_type&      parent(base_ptr x)  { return reinterpret_cast<link_type&>(x->parent); }
    static reference       value(base_ptr x)   { return reinterpret_cast<link_type>(x)->value; }
    static const key_type& key(base_ptr x)     { return KeyOfValue()(value(reinterpret_cast<link_type>(x))); }
    static color_type&     color(base_ptr x)   { return reinterpret_cast<link_type&>(x->color); }

    // 求最小值和最大值
    static link_type minimum(link_type x) { 
        return reinterpret_cast<link_type>(__rb_tree_node_base::minimum(x)); 
    }
    static link_type maximum(link_type x) { 
        return reinterpret_cast<link_type>(__rb_tree_node_base::maximum(x)); 
    }

  public:
    // 构造、复制、析构函数
    rb_tree() { __rb_tree_initialize(); }

    rb_tree(const rb_tree& rhs);
    rb_tree(rb_tree&& rhs);

    rb_tree& operator=(const rb_tree& rhs);
    rb_tree& operator=(rb_tree&& rhs);

    ~rb_tree() { clear(); }

  public:
    // 迭代器相关操作
    iterator               begin()        { return leftmost(); }
    const_iterator         begin()  const { return leftmost(); }
    iterator               end()          { return header_; }
    const_iterator         end()    const { return header_; }
    reverse_iterator       rbegin()       { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator       rend()         { return reverse_iterator(begin()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }

    // 容量相关操作
    bool      empty()    const { return node_count_ == 0; }
    size_type size()     const { return node_count_; }
    size_type max_size() const { return static_cast<size_type>(-1); }

    // 插入删除相关操作
    iterator             insert_equal(const value_type& value);
    iterator             insert_equal(iterator position, const value_type& value);
    template <class InputIterator>
    void                 insert_equal(InputIterator first, InputIterator last);
    pair<iterator, bool> insert_unique(const value_type& value);
    iterator             insert_unique(iterator position, const value_type& value);
    template <class InputIterator>
    void                 insert_unique(InputIterator first, InputIterator last);
    void                 erase(iterator position);
    size_type            erase(const key_type& k);
    void                 erase(iterator first, iterator last);
    void                 clear();
    
    // rb_tree 相关操作
    Compare                              key_comp()                     const { return key_compare; }
    iterator                             find(const key_type& k);
    const_iterator                       find(const key_type& k)        const;
    size_type                            count(const key_type& k)       const;
    iterator                             lower_bound(const key_type& k);
    const_iterator                       lower_bound(const key_type& k) const;
    iterator                             upper_bound(const key_type& k);
    const_iterator                       upper_bound(const key_type& k) const;
    pair<iterator, iterator>             equal_range(const key_type& k);
    pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
    void                                 swap(rb_tree& rhs);

  private:
    // rb_tree 的私有成员函数
    void      __rb_tree_initialize();
    link_type __copy(link_type x, link_type p);
    iterator  __insert(base_ptr x_, base_ptr y_, const value_type& value);
    void      __erase(link_type x);
};

/*****************************************************************************************/

// 创建一个结点
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::link_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__create_node(const value_type& x) {
    auto tmp = __get_node();
    try {
        mystl::construct(&tmp->value, x);
    }
    catch (...) {
        __put_node(tmp);
    }
    return tmp;
}

// 复制一个结点
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__clone_node(link_type x) {
    auto tmp = __create_node(x->value);
    tmp->color = x->color;
    tmp->left = nullptr;
    tmp->right = nullptr;
    return tmp;
}

// 销毁一个结点
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__destroy_node(link_type p) {
    mystl::destroy(&p->value);
    __put_node(p);
}

// 复制构造函数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const rb_tree& rhs) {
    __rb_tree_initialize();
    if (rhs.root() != nullptr) {
        color(header_) = __rb_tree_red;
        root() = __copy(rhs.root(), header_);
        leftmost() = minimum(root());
        rightmost() = maximum(root());
    }
    node_count_ = rhs.node_count_;
    key_compare = rhs.key_compare;
}

// 移动构造函数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(rb_tree&& rhs) {
    __rb_tree_initialize();
    if (rhs.root() != nullptr) {
        color(header_) = __rb_tree_red;
        root() = __copy(rhs.root(), header_);
        leftmost() = minimum(root());
        rightmost() = maximum(root());
        rhs.root() = nullptr;
        rhs.leftmost() = rhs.header_;
        rhs.rightmost() = rhs.header_;
        rhs.node_count_ = 0;
    }
    node_count_ = rhs.node_count_;
    key_compare = rhs.key_compare;
}

// 复制赋值操作符
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& 
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const rb_tree& rhs) {
    if (this != &rhs) {
        if (root() == nullptr)  __rb_tree_initialize();
        clear();
        if (rhs.root() != nullptr) {
            root() = __copy(rhs.root(), header_);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
        }
        node_count_ = rhs.node_count_;
        key_compare = rhs.key_compare;
    }
    return *this;
}

// 移动赋值操作符
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(rb_tree&& rhs) {
    if (this != &rhs) {
        if (root() == nullptr)  __rb_tree_initialize();
        clear();
        if (rhs.root() != nullptr) {
            root() = __copy(rhs.root(), header_);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
            node_count_ = rhs.node_count_;
            rhs.root() = nullptr;
            rhs.leftmost() = rhs.header_;
            rhs.rightmost() = rhs.header_;
            rhs.node_count_ = 0;
        }
        node_count_ = rhs.node_count_;
        key_compare = rhs.key_compare;
    }
    return *this;
}

// 插入新值，节点键值允许重复
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
    insert_equal(const value_type& value) {
    auto y = header_;
    auto x = root();
    while (x != nullptr) {  //从根节点开始往下搜索合适的插入点
        y = x;
        x = key_compare(KeyOfValue()(value), key(x)) ? left(x) : right(x);  // 小于向左走
    }
    return __insert(x, y, value);
}

// 从 position 位置开始插入新值，节点键值允许重复
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
    insert_equal(iterator position, const value_type& value) {
    if (position.node == header_->left) {  // 位于 begin 处
        if (size() > 0 && !key_compare(key(position.node), KeyOfValue()(value)))
            // position 节点键值大于等于新值键值
            return __insert(position.node, position.node, value);
        else
            return insert_equal(value);    // 寻找合适的插入点
    }
    else if (position.node == header_) {   // 位于 end 处
        if (!key_compare(KeyOfValue()(value), key(rightmost())))
            // 新值键值大于等于最大节点键值
            return __insert(static_cast<base_ptr>(nullptr), rightmost(), value);
        else
            return insert_equal(value);    // 寻找合适的插入点
    }
    else {
        auto before = position;
        --before;
        if (!key_compare(KeyOfValue()(value), key(before.node)) && 
            !key_compare(key(position.node), KeyOfValue()(value))) {
            // before <= value <= position
            if (right(before.node) == nullptr)
                return __insert(static_cast<base_ptr>(nullptr), before.node, value);
            else
                return __insert(position.node, position.node, value);
        }
        else {
            return insert_equal(value);    // 寻找合适的插入点
        }
    }
}

// 插入[first, last)区间内的值，键值允许重复
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
template <class InputIterator>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(InputIterator first, InputIterator last) {
    for (; first != last; ++first)
        insert_equal(*first);
}

// 插入新值，节点键值不允许重复，返回一个 pair，若插入成功，pair 的第二参数为 true，否则为 false
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& value) {
    auto y = header_;
    auto x = root();
    bool comp = true;
    while (x != nullptr) {
        y = x;
        comp = key_compare(KeyOfValue()(value), key(x));  // value 键值小于当前节点键值
        x = comp ? left(x) : right(x);                    // 小于向左走，否则向右走
    }
    iterator j = iterator(y);                             // 此时 y 为插入点的父节点
    if (comp) {                                           // 离开循坏时 comp 为 true，插在左边
        if (j == begin())                                 // 父节点为最左节点
            return pair<iterator, bool>(__insert(x, y, value), true);
        else
            --j;
    }
    if (key_compare(key(j.node), KeyOfValue()(value)))    // 新节点没有重复
        return pair<iterator, bool>(__insert(x, y, value), true);
    // 进行至此，表示新节点与现有节点键值重复
    return pair<iterator, bool>(j, false);
}

// 从 position 开始插入新值，节点键值不允许重复
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position,
    const value_type& value) {
    if (position.node == header_->left) {                                    // 位于 begin 处
        if (size() && key_compare(KeyOfValue()(value), key(position.node)))  // 新值键值小于 position 节点的键值
            return __insert(position.node, position.node, value);
        else
            return insert_unique(value).first;                               // 寻找合适的插入点
    }
    else if (position.node == header_) {
        if (key_compare(key(rightmost()), KeyOfValue()(value)))              // 最大节点键值小于新值键值
            return __insert(static_cast<base_ptr>(nullptr), rightmost(), value);
        else
            return insert_unique(value).first;                               // 寻找合适的插入点
    }
    else {
        auto before = position;
        --before;
        if (key_compare(key(before.node), KeyOfValue()(value)) && 
            key_compare(KeyOfValue()(value), key(position.node))) {
            // before < value < position
            if (right(before.node) == nullptr)
                return __insert(static_cast<base_ptr>(nullptr), before.node, value);
            else
                return __insert(position.node, position.node, value);
        }
        else {
            return insert_unique(value).first;                               // 寻找合适的插入点
        }
    }
}

// 插入[first, last)内的值，键值不允许重复
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
template <class InputIterator>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last) {
    for (; first != last; ++first)
        insert_unique(*first);
}

// 删除 position 位置的节点
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position) {
    auto y = reinterpret_cast<link_type>(__rb_tree_rebalance_for_erase(position.node,
        header_->parent, header_->left, header_->right));
    __destroy_node(y);
    --node_count_;
}

// 删除与 x 键值相等的元素，返回删除的个数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type& x) {
    auto p = equal_range(x);  // 找出与 x 键值相等的区间
    auto n = static_cast<size_type>(distance(p.first, p.second));
    erase(p.first, p.second);
    return n;
}

// 删除[first, last)区间内的元素
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
    if (first == begin() && last == end()) {
        clear();
    }
    else {
        while (first != last)  erase(first++);
    }
}

// 清空 rb tree
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
    if (node_count_ != 0) {
        __erase(root());
        leftmost() = header_;
        root() = nullptr;
        rightmost() = header_;
        node_count_ = 0;
    }
}

// 查找键值为 k 的节点，返回指向它的迭代器
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) {
    auto y = header_;                   // 最后一个不小于 k 的节点
    auto x = root();
    while (x != nullptr) {
        if (!key_compare(key(x), k)) {  // k 小于等于 x 键值，向左走
            y = x, x = left(x);
        }
        else {                          // k 大于 x 键值，向右走
            x = right(x);
        }
    }
    iterator j = iterator(y);
    return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) const {
    auto y = header_;                   // 最后一个不小于 k 的节点
    auto x = root();
    while (x != nullptr) {
        if (!key_compare(key(x), k)) {  // k 小于等于 x 键值，向左走
            y = x, x = left(x);
        }
        else {                           // k 大于 x 键值，向右走
            x = right(x);
        }
    }
    const_iterator j = const_iterator(y);
    return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

// 查找键值为 k 的节点出现的次数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) const {
    auto p = equal_range(k);
    auto n = static_cast<size_type>(distance(p.first, p.second));
    return n;
}

// 键值不小于 k 的第一个位置
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) {
    auto y = header_;
    auto x = root();
    while (x != nullptr) {
        if (!key_compare(key(x), k)) {  // k <= x
            y = x, x = left(x);
        }
        else {
            x = right(x);
        }
    }
    return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) const {
    auto y = header_;
    auto x = root();
    while (x != nullptr) {
        if (!key_compare(key(x), k)) {  // k <= x
            y = x, x = left(x);
        }
        else {
            x = right(x);
        }
    }
    return const_iterator(y);
}

// 键值不小于 k 的最后一个位置
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) {
    auto y = header_;
    auto x = root();
    while (x != nullptr) {
        if (key_compare(k, key(x))) {  // k < x
            y = x, x = left(x);
        }
        else {
            x = right(x);
        }
    }
    return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) const {
    auto y = header_;
    auto x = root();
    while (x != nullptr) {
        if (key_compare(k, key(x))) {  // k < x
            y = x, x = left(x);
        }
        else {
            x = right(x);
        }
    }
    return const_iterator(y);
}

// 查找与键值 k 相等的区间，返回一个 pair 分别指向相等区间的首尾
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) {
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) const {
    return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
}

// 交换 rb tree
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::swap(rb_tree& rhs) {
    mystl::swap(header_, rhs.header_);
    mystl::swap(node_count_, rhs.node_count_);
    mystl::swap(key_compare, rhs.key_compare);
}

// __rb_tree_initialize 函数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_initialize() {
    header_ = __get_node();
    color(header_) = __rb_tree_red;  // header_ 节点颜色为红，与 root 区分
    root() = nullptr;
    leftmost() = header_;
    rightmost() = header_;
    node_count_ = 0;
}

// __copy 函数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x, link_type p) {
    auto top = __clone_node(x);
    top->parent = p;
    try {
        if (x->right)
            top->right = __copy(right(x), top);
        p = top;
        x = left(x);
        while (x != nullptr) {
            auto y = __clone_node(x);
            p->left = y;
            y->parent = p;
            if (x->right)
                y->right = __copy(right(x), y);
            p = y;
            x = left(x);
        }
    }
    catch (...) {
        __erase(top);
    }
    return top;
}

// __insert 函数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
    __insert(base_ptr x_, base_ptr y_, const value_type& value) {
    // x_ 为新值插入点，y_ 为插入点的父节点，value 为新值
    auto x = reinterpret_cast<link_type>(x_);
    auto y = reinterpret_cast<link_type>(y_);
    link_type z;
    if (y == header_ || x != nullptr || key_compare(KeyOfValue()(value), key(y))) {
        z = __create_node(value);
        left(y) = z;                 // 使当 y 为 header_ 时，leftmost() = z
        if (y == header_) {
            root() = z;
            rightmost() = z;
        }
        else if (y == leftmost()) {  // 如果 y 为最左节点
            leftmost() = z;          // 维护 leftmost(),使其永远指向最左节点
        }
    }
    else {
        z = __create_node(value);
        right(y) = z;                // 另新节点成为插入点父节点的右子节点
        if (y == rightmost())
            rightmost() = z;         // 维护 rightmost(),使其永远指向最右节点
    }
    parent(z) = y;                   // 设置新节点的父节点
    left(z) = nullptr;               // 设置新节点的左子节点
    right(z) = nullptr;              // 设置新节点的右子节点
    __rb_tree_rebalance(z, header_->parent);
    ++node_count_;
    return iterator(z);
}

// __erase 函数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x) {
    while (x != nullptr) {
        __erase(right(x));
        auto y = left(x);
        __destroy_node(x);
        x = y;
    }
}

// 重载比较操作符
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator==(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator!=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return !(lhs == rhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return rhs < lhs;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return !(rhs < lhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void swap(rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_RB_TREE_H_

