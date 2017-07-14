#ifndef MYTINYSTL_RB_TREE_H_
#define MYTINYSTL_RB_TREE_H_

// 这个头文件包含一个模板类 rb_tree
// rb_tree : 红黑树

#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "type_traits.h"

#include <initializer_list>

#include <cassert>

namespace mystl
{

// rb tree 节点颜色的类型

typedef bool __rb_tree_color_type;

static constexpr __rb_tree_color_type __rb_tree_red = false;
static constexpr __rb_tree_color_type __rb_tree_black = true;

// forward declaration

template <class T> struct __rb_tree_node_base;
template <class T> struct __rb_tree_node;

template <class T> struct __rb_tree_iterator;
template <class T> struct __rb_tree_const_iterator;

// rb tree value traits

template <class T, bool>
struct __rb_tree_value_traits_imp
{
  typedef T key_type;
  typedef T mapped_type;
  typedef T value_type;

  static constexpr bool __is_map = false;

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
struct __rb_tree_value_traits_imp<T, true>
{
  typedef typename mystl::remove_cv<typename T::first_type>::type key_type;
  typedef typename T::second_type                                 mapped_type;
  typedef T                                                       value_type;

  static constexpr bool __is_map = true;

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
struct __rb_tree_value_traits
{
  static constexpr bool __is_map = mystl::is_pair<T>::value;

  typedef typename __rb_tree_value_traits_imp<T, __is_map> value_traits_type;

  typedef typename value_traits_type::key_type             key_type;
  typedef typename value_traits_type::mapped_type          mapped_type;
  typedef typename value_traits_type::value_type           value_type;

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

// rb tree node traits

template <class T>
struct __rb_tree_node_traits
{
  typedef __rb_tree_color_type                    color_type;

  typedef __rb_tree_node_base<T>*                 base_ptr;
  typedef __rb_tree_node<T>*                      node_ptr;

  typedef __rb_tree_value_traits<T>               value_traits_type;
  typedef typename value_traits_type::key_type    key_type;
  typedef typename value_traits_type::mapped_type mapped_type;
  typedef typename value_traits_type::value_type  value_type;

  template <class NodePtr>
  static node_ptr get_node_ptr(NodePtr& v)
  {
    return v->get_node_ptr();
  }

  template <class Ty>
  static const key_type& get_key(const Ty& v)
  {
    return value_traits_type::get_key(get_node_ptr(v)->value);
  }

  template <class Ty>
  static const mapped_type& get_value(const Ty& v)
  {
    return value_traits_type::get_value(get_node_ptr(v)->value);
  }
};

// rb tree traits

template <class T>
struct __rb_tree_traits
{
  typedef __rb_tree_value_traits<T>               value_traits_type;

  typedef typename value_traits_type::key_type    key_type;
  typedef typename value_traits_type::mapped_type mapped_type;
  typedef typename value_traits_type::value_type  value_type;

  typedef value_type*                             pointer;
  typedef value_type&                             reference;
  typedef const value_type*                       const_pointer;
  typedef const value_type&                       const_reference;

  typedef __rb_tree_node_base<T>                  base_type;
  typedef __rb_tree_node<T>                       node_type;

  typedef base_type*                              base_ptr;
  typedef node_type*                              node_ptr;

  template <class NodePtr>
  static base_ptr get_base_ptr(NodePtr& ptr)
  {
    return __rb_tree_node_traits<value_type>::get_base_ptr(ptr);
  }

  template <class NodePtr>
  static node_ptr get_node_ptr(NodePtr& ptr)
  {
    return __rb_tree_node_traits<value_type>::get_node_ptr(ptr);
  }

  template <class V>
  static const key_type& get_key(const V& value)
  {
    return __get_key(value, mystl::is_pointer<V>{});
  }

  template <class V>
  static const value_type& get_value(const V& value)
  {
    return __get_value(value, mystl::is_pointer<V>{});
  }

  template <class V>
  static const key_type& __get_key(const V& value, mystl::__true_type)
  {
    return __rb_tree_node_traits<value_type>::get_key(value);
  }

  template <class V>
  static const key_type& __get_key(const V& value, mystl::__false_type)
  {
    return __rb_tree_value_traits<value_type>::get_key(value);
  }

  template <class V>
  static const value_type& __get_value(const V& value, mystl::__true_type)
  {
    return __rb_tree_node_traits<value_type>::get_value(value);
  }

  template <class V>
  static const value_type& __get_value(const V& value, mystl::__false_type)
  {
    return __rb_tree_value_traits<value_type>::get_value(value);
  }

};

// rb tree 的节点设计

template <class T>
struct __rb_tree_node_base
{
  typedef __rb_tree_node_traits<T>         node_traits;

  typedef typename node_traits::color_type color_type;
  typedef typename node_traits::base_ptr   base_ptr;
  typedef typename node_traits::node_ptr   node_ptr;

  base_ptr   parent;  // 父节点
  base_ptr   left;    // 左子节点
  base_ptr   right;   // 右子节点
  color_type color;   // 节点颜色

  base_ptr get_base_ptr()
  {
    return &*this;
  }

  node_ptr get_node_ptr()
  {
    return reinterpret_cast<node_ptr>(&*this);
  }

  node_ptr& get_node_ref()
  {
    return reinterpret_cast<node_ptr&>(*this);
  }
};

template <class T>
struct __rb_tree_node :public __rb_tree_node_base<T>
{
  typedef __rb_tree_node_base<T> base;

  using base::base_ptr;
  using base::node_ptr;

  T value;  // 节点值

  base_ptr get_base_ptr()
  {
    return static_cast<base_ptr>(&*this);
  }

  node_ptr get_node_ptr()
  {
    return &*this;
  }
};

// rb tree 的迭代器设计

template <class T>
struct __rb_tree_iterator_base :public iterator<bidirectional_iterator_tag, T>
{
  typedef typename __rb_tree_node_base<T>::base_ptr  base_ptr;

  base_ptr node;  // 指向节点本身

  __rb_tree_iterator_base() :node(nullptr) {}

  // 使迭代器前进
  void inc()
  {
    if (node->right)
    {  // 如果有右子节点
      node = node->right;
      while (node->left)
        node = node->left;
    }
    else
    {  // 如果没有右子节点
      auto y = node->parent;
      while (y->right == node)
      {
        node = y;
        y = y->parent;
      }
      if (node->right != y)  // 应对“寻找根节点的下一节点，而根节点没有右子节点”的特殊情况
        node = y;
    }
  }

  // 使迭代器后退
  void dec()
  {
    if (node->color == __rb_tree_red && node->parent->parent == node)
    {  // 如果 node 为 header
      node = node->right;  // 指向整棵树的 max 节点
    }
    else if (node->left)
    {  // 如果有左子节点
      auto y = node->left;
      while (y->right)
        y = y->right;
      node = y;
    }
    else
    {  // 非 header 节点，也无左子节点
      auto y = node->parent;
      while (node == y->left)
      {
        node = y;
        y = y->parent;
      }
      node = y;
    }
  }

  bool operator==(const __rb_tree_iterator_base& rhs) { return node == rhs.node; }
  bool operator!=(const __rb_tree_iterator_base& rhs) { return node != rhs.node; }
};

template <class T>
struct __rb_tree_iterator :public __rb_tree_iterator_base<T>
{
  typedef __rb_tree_traits<T>              tree_traits;

  typedef typename tree_traits::value_type value_type;
  typedef typename tree_traits::pointer    pointer;
  typedef typename tree_traits::reference  reference;
  typedef typename tree_traits::base_ptr   base_ptr;
  typedef typename tree_traits::node_ptr   node_ptr;

  typedef __rb_tree_iterator<T>            iterator;
  typedef __rb_tree_const_iterator<T>      const_iterator;
  typedef iterator                         self;

  using __rb_tree_iterator_base<T>::node;

  // 构造函数
  __rb_tree_iterator() {}
  __rb_tree_iterator(base_ptr x) { node = x; }
  __rb_tree_iterator(node_ptr x) { node = x; }
  __rb_tree_iterator(const iterator& rhs) { node = rhs.node; }
  __rb_tree_iterator(const const_iterator& rhs) { node = rhs.node; }

  // 重载操作符
  reference operator*()  const { return node->get_node_ptr()->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    inc();
    return *this;
  }
  self operator++(int)
  {
    self tmp(*this);
    inc();
    return tmp;
  }
  self& operator--()
  {
    dec();
    return *this;
  }
  self operator--(int)
  {
    self tmp(*this);
    dec();
    return tmp;
  }
};

template <class T>
struct __rb_tree_const_iterator :public __rb_tree_iterator_base<T>
{
  typedef __rb_tree_traits<T>                   tree_traits;

  typedef typename tree_traits::value_type      value_type;
  typedef typename tree_traits::const_pointer   pointer;
  typedef typename tree_traits::const_reference reference;
  typedef typename tree_traits::base_ptr        base_ptr;
  typedef typename tree_traits::node_ptr        node_ptr;

  typedef __rb_tree_iterator<T>                 iterator;
  typedef __rb_tree_const_iterator<T>           const_iterator;
  typedef const_iterator                        self;

  using __rb_tree_iterator_base<T>::node;

  // 构造函数
  __rb_tree_const_iterator() {}
  __rb_tree_const_iterator(base_ptr x) { node = x; }
  __rb_tree_const_iterator(node_ptr x) { node = x; }
  __rb_tree_const_iterator(const iterator& rhs) { node = rhs.node; }
  __rb_tree_const_iterator(const const_iterator& rhs) { node = rhs.node; }

  // 重载操作符
  reference operator*()  const { return node->get_node_ptr()->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    inc();
    return *this;
  }
  self operator++(int)
  {
    self tmp(*this);
    inc();
    return tmp;
  }
  self& operator--()
  {
    dec();
    return *this;
  }
  self operator--(int)
  {
    self tmp(*this);
    dec();
    return tmp;
  }
};

// tree algorithm

template <class NodePtr>
NodePtr __rb_tree_min(NodePtr x) noexcept
{
  while (x->left != nullptr)
    x = x->left;
  return x;
}

template <class NodePtr>
NodePtr __rb_tree_max(NodePtr x) noexcept
{
  while (x->right != nullptr)
    x = x->right;
  return x;
}

// 左旋，参数一为左旋点，参数二为根节点
template <class NodePtr>
void __rb_tree_rotate_left(NodePtr x, NodePtr& root)
{
  auto y = x->right;
  x->right = y->left;
  if (y->left)
    y->left->parent = x;
  y->parent = x->parent;
  if (x == root)
  { // 如果 x 为根节点
    root = y;
  }
  else if (x == x->parent->left)
  { // 如果 x 是它父节点的左子节点
    x->parent->left = y;
  }
  else
  { // 如果 x 是它父节点的右子节点
    x->parent->right = y;
  }
  // 连接 x 与 y 的关系
  y->left = x;  
  x->parent = y;
}

// 右旋，参数一为右旋点，参数二为根节点
template <class NodePtr>
void __rb_tree_rotate_right(NodePtr x, NodePtr& root)
{
  auto y = x->left;
  x->left = y->right;
  if (y->right)
    y->right->parent = x;
  y->parent = x->parent;
  if (x == root)
  { // 如果 x 为根节点
    root = y;
  }
  else if (x == x->parent->right)
  { // 如果 x 是它父节点的右子节点
    x->parent->right = y;
  }
  else
  { // 如果 x 是它父节点的左子节点
    x->parent->left = y;
  }
  // 连接 x 与 y 的关系
  y->right = x;                      
  x->parent = y;
}

// 插入节点后使 rb tree 重新平衡，参数一为新增节点，参数二为根节点
template <class NodePtr>
void __rb_tree_insert(NodePtr x, NodePtr& root)
{
  x->color = __rb_tree_red;  // 新增节点为红色
  while (x != root && x->parent->color == __rb_tree_red)
  { // 当父节点为红
    if (x->parent == x->parent->parent->left)
    { // 如果父节点是左子节点
      auto y = x->parent->parent->right;
      if (y && y->color == __rb_tree_red)
      { // 如果伯父节点存在且为红
        x->parent->color = __rb_tree_black;
        y->color = __rb_tree_black;
        x->parent->parent->color = __rb_tree_red;
        x = x->parent->parent;
      }
      else
      { // 无伯父节点或伯父节点为黑
        if (x == x->parent->right)
        { // 如果 x 为父节点的右子节点
          x = x->parent;
          __rb_tree_rotate_left(x, root);
        }
        x->parent->color = __rb_tree_black;
        x->parent->parent->color = __rb_tree_red;
        __rb_tree_rotate_right(x->parent->parent, root);
      }
    }
    else
    { // 如果父节点是右子节点，对称处理
      auto y = x->parent->parent->left;
      if (y && y->color == __rb_tree_red)
      { // 如果伯父节点存在且为红
        x->parent->color = __rb_tree_black;
        y->color = __rb_tree_black;
        x->parent->parent->color = __rb_tree_red;
        x = x->parent->parent;
      }
      else
      { // 无伯父节点或伯父节点为黑
        if (x == x->parent->left)
        { // 如果 x 为父节点的左子节点
          x = x->parent;
          __rb_tree_rotate_right(x, root);
        }
        x->parent->color = __rb_tree_black;
        x->parent->parent->color = __rb_tree_red;
        __rb_tree_rotate_left(x->parent->parent, root);
      }
    }
  }
  root->color = __rb_tree_black;  // 根节点永远为黑
}

// 删除节点后使 rb tree 重新平衡，参数一为要删除的节点，参数二为根节点，参数三为最小节点，参数四为最大节点
template <class NodePtr>
NodePtr __rb_tree_erase(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost)
{
  auto y = z;
  NodePtr x = nullptr;
  NodePtr x_parent = nullptr;

  // 寻找可以代替的节点
  if (y->left == nullptr)
  { // 如果被删节点的左子节点为空
    x = y->right;
  }
  else
  {
    if (y->right == nullptr)
    { // 如果被删节点的右子节点为空
      x = y->left;
    }
    else
    { // 如果被删节点的左右节点都不为空
      y = y->right;
      while (y->left != nullptr)
        y = y->left;
      x = y->right;
    }
  }

  // 调整代替节点的节点关系
  if (y != z)
  { // 找到替换点
    z->left->parent = y;
    y->left = z->left;
    if (y != z->right)
    {
      x_parent = y->parent;
      if (x)
        x->parent = y->parent;  // 调整替代节点的右子节点的父子关系
      y->parent->left = x;
      y->right = z->right;
      z->right->parent = y;
    }
    else
    {
      x_parent = y;
    }
    if (root == z)
    { // 如果被删节点为根节点，另替代节点为根节点
      root = y;
    }
    else if (z->parent->left == z)
    { // 调整替代节点的父子关系
      z->parent->left = y;
    }
    else
    {
      z->parent->right = y;
    }
    y->parent = z->parent;
    mystl::swap(y->color, z->color);
    y = z;
  }
  else
  { // y == z，x 为替代节点
    x_parent = y->parent;
    if (x)  
      x->parent = y->parent;
    if (root == z)
    { // 如果被删节点为根节点
      root = x;
    }
    else
    { // 调整替代节点的父子关系
      if (z->parent->left == z)
        z->parent->left = x;
      else
        z->parent->right = x;
    }
    if (leftmost == z)
    { // 被删节点为最小节点，调整最小值
      if (z->right == nullptr)
        leftmost = z->parent;
      else
        leftmost = __rb_tree_min(x);
    }
    if (rightmost == z)
    { // 被删节点为最大节点，调整最大值
      if (z->left == nullptr)
        rightmost = z->parent;
      else
        rightmost = __rb_tree_max(x);
    }
  }

  // 调整节点颜色
  if (y->color != __rb_tree_red)
  {      // 如果被删节点颜色为黑
    while (x != root && (x == nullptr || x->color == __rb_tree_black))
    {
      if (x == x_parent->left)
      {    // 如果替代节点是左子节点
        auto w = x_parent->right;
        if (w->color == __rb_tree_red)
        {
          w->color = __rb_tree_black;
          x_parent->color = __rb_tree_red;
          __rb_tree_rotate_left(x_parent, root);
          w = x_parent->right;
        }
        if ((w->left == nullptr || w->left->color == __rb_tree_black) &&
          (w->right == nullptr || w->right->color == __rb_tree_black))
        {
          w->color = __rb_tree_red;
          x = x_parent;
          x_parent = x_parent->parent;
        }
        else
        {
          if (w->right == nullptr || w->right->color == __rb_tree_black)
          {
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
      else
      { // 如果替代节点为右子节点，对称处理
        auto w = x_parent->left;
        if (w->color == __rb_tree_red)
        {
          w->color = __rb_tree_black;
          x_parent->color = __rb_tree_red;
          __rb_tree_rotate_right(x_parent, root);
          w = x_parent->left;
        }
        if ((w->left == nullptr || w->left->color == __rb_tree_black) &&
          (w->right == nullptr || w->right->color == __rb_tree_black))
        {
          w->color = __rb_tree_red;
          x = x_parent;
          x_parent = x_parent->parent;
        }
        else
        {
          if (w->left == nullptr || w->left->color == __rb_tree_black)
          {
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
    if (x) 
      x->color = __rb_tree_black;
  }
  return y;
}

// 模板类 rb_tree
// 参数一代表数据类型，参数二代表键值比较的方法
template <class T, class Compare>
class rb_tree
{
public:
  // rb_tree 的嵌套型别定义 
  
  typedef __rb_tree_traits<T>                      tree_traits;

  typedef typename tree_traits::base_type          base_type;
  typedef typename tree_traits::base_ptr           base_ptr;
  typedef typename tree_traits::node_type          node_type;
  typedef typename tree_traits::node_ptr           node_ptr;
  typedef typename tree_traits::key_type           key_type;
  typedef typename tree_traits::mapped_type        mapped_type;
  typedef typename tree_traits::value_type         value_type;
  typedef Compare                                  key_compare;

  typedef mystl::allocator<T>                      allocator_type;
  typedef mystl::allocator<T>                      data_allocator;
  typedef mystl::allocator<base_type>              base_allocator;
  typedef mystl::allocator<node_type>              node_allocator;

  typedef typename allocator_type::pointer         pointer;
  typedef typename allocator_type::const_pointer   const_pointer;
  typedef typename allocator_type::reference       reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type       size_type;
  typedef typename allocator_type::difference_type difference_type;

  typedef __rb_tree_iterator<T>                    iterator;
  typedef __rb_tree_const_iterator<T>              const_iterator;
  typedef mystl::reverse_iterator<iterator>        reverse_iterator;
  typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

  allocator_type get_allocator() const { return node_allocator(); }
  key_compare    key_comp()      const { return key_comp_; }

private:
  // 用以下三个数据表现 rb tree
  size_type   node_count_;  // rb tree 的节点数目
  base_ptr    header_;      // 特殊节点，与根节点互为对方的父节点
  key_compare key_comp_;    // 节点键值比较的准则

private:
  // 以下三个函数用于取得根节点，最小节点和最大节点
  base_ptr& root()      const { return header_->parent; }
  base_ptr& leftmost()  const { return header_->left; }
  base_ptr& rightmost() const { return header_->right; }

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

  iterator               begin()        noexcept 
  { return leftmost(); }
  const_iterator         begin()  const noexcept
  { return leftmost(); }
  iterator               end()          noexcept
  { return header_; }
  const_iterator         end()    const noexcept
  { return header_; }

  reverse_iterator       rbegin() noexcept
  { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const noexcept
  { return const_reverse_iterator(end()); }
  reverse_iterator       rend()         noexcept
  { return reverse_iterator(begin()); }
  const_reverse_iterator rend()   const noexcept
  { return const_reverse_iterator(begin()); }

  const_iterator         cbegin()  const noexcept
  { return begin(); }
  const_iterator         cend()    const noexcept
  { return end(); }
  const_reverse_iterator crbegin() const noexcept
  { return rbegin(); }
  const_reverse_iterator crend()   const noexcept
  { return rend(); }

  // 容量相关操作
  bool      empty()    const noexcept { return node_count_ == 0; }
  size_type size()     const noexcept { return node_count_; }
  size_type max_size() const noexcept { return static_cast<size_type>(-1); }

  // 插入删除相关操作
  iterator  insert_equal(const value_type& value);
  iterator  insert_equal(iterator position, const value_type& value);
  template <class InputIterator>
  void      insert_equal(InputIterator first, InputIterator last);

  mystl::pair<iterator, bool> insert_unique(const value_type& value);
  iterator  insert_unique(iterator position, const value_type& value);
  template <class InputIterator>
  void      insert_unique(InputIterator first, InputIterator last);

  void      erase(iterator position);
  size_type erase(const key_type& k);
  void      erase(iterator first, iterator last);

  void      clear();

  // rb_tree 相关操作

  iterator       find(const key_type& key);
  const_iterator find(const key_type& key)        const;
  size_type      count(const key_type& key)       const;

  iterator       lower_bound(const key_type& key);
  const_iterator lower_bound(const key_type& key) const;

  iterator       upper_bound(const key_type& key);
  const_iterator upper_bound(const key_type& key) const;

  mystl::pair<iterator, iterator>             equal_range(const key_type& key);
  mystl::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

  void           swap(rb_tree& rhs);

private:

  // node related
  node_ptr __create_node(const value_type& x);
  node_ptr __clone_node(base_ptr x);
  void     __destroy_node(node_ptr p);

  void     __rb_tree_initialize();
  void     __reset();
  base_ptr __copy_assign(base_ptr x, base_ptr p);
  iterator __insert_aux(base_ptr x_, base_ptr y_, const value_type& value);
  void     __erase_aux(base_ptr x);
};

/*****************************************************************************************/

// 复制构造函数
template <class T, class Compare>
rb_tree<T, Compare>::
rb_tree(const rb_tree& rhs)
{
  __rb_tree_initialize();
  if (rhs.node_count_ != 0)
  {
    root() = __copy_assign(rhs.root(), header_);
    leftmost() = __rb_tree_min(root());
    rightmost() = __rb_tree_max(root());
  }
  node_count_ = rhs.node_count_;
  key_comp_ = rhs.key_comp_;
}

// 移动构造函数
template <class T, class Compare>
rb_tree<T, Compare>::
rb_tree(rb_tree&& rhs)
  :header_(mystl::move(rhs.header_)), 
  key_comp_(rhs.key_comp_),
  node_count_(rhs.node_count_)
{
  rhs.__reset();
}

// 复制赋值操作符
template <class T, class Compare>
rb_tree<T, Compare>& 
rb_tree<T, Compare>::
operator=(const rb_tree& rhs)
{
  if (this != &rhs)
  {
    clear();

    if (rhs.node_count_ != 0)
    {
      root() = __copy_assign(rhs.root(), header_);
      leftmost() = __rb_tree_min(root());
      rightmost() = __rb_tree_max(root());
    }

    node_count_ = rhs.node_count_;
    key_comp_ = rhs.key_comp_;
  }
  return *this;
}

// 移动赋值操作符
template <class T, class Compare>
rb_tree<T, Compare>&
rb_tree<T, Compare>::
operator=(rb_tree&& rhs)
{
  if (this != &rhs)
  {
    clear();

    header_ = mystl::move(rhs.header_);
    node_count_ = rhs.node_count_;
    key_comp_ = rhs.key_comp_;

    rhs.__reset();
  }
  return *this;
}

// 插入新值，节点键值允许重复
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
insert_equal(const value_type& value)
{
  auto y = header_;
  auto x = root();
  while (x != nullptr)
  {  //从根节点开始往下搜索合适的插入点
    y = x;
    x = key_comp_(tree_traits::get_key(value), tree_traits::get_key(x))
      ? x->left : x->right;  // 小于向左走
  }
  return __insert_aux(x, y, value);
}

// 从 position 位置开始插入新值，节点键值允许重复
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
insert_equal(iterator position, const value_type& value)
{
  if (position.node == header_->left)
  { // 位于 begin 处
    if (node_count_ > 0 && !key_comp_(tree_traits::get_key(position.node), tree_traits::get_key(value)))
    { // position 节点键值大于等于新值键值
      return __insert_aux(position.node, position.node, value);
    }
    else
    {
      return insert_equal(value);
    }
  }
  else if (position.node == header_)
  { // 位于 end 处
    if (!key_comp_(tree_traits::get_key(value), tree_traits::get_key(rightmost())))
    { // 新值键值大于等于最大节点键值
      return __insert_aux(static_cast<base_ptr>(nullptr), rightmost(), value);
    }
    else
    {
      return insert_equal(value);
    }
  }
  else
  {
    auto before = position;
    --before;
    if (!key_comp_(tree_traits::get_key(value), tree_traits::get_key(before.node)) &&
        !key_comp_(tree_traits::get_key(position.node), tree_traits::get_key(value)))
    { // before <= value <= position
      if ((before.node)->right == nullptr)
      {
        return __insert_aux(static_cast<base_ptr>(nullptr), before.node, value);
      }
      else
      {
        return __insert_aux(position.node, position.node, value);
      }
    }
    else
    {
      return insert_equal(value);
    }
  }
}

// 插入[first, last)区间内的值，键值允许重复
template <class T, class Compare>
template <class InputIterator>
void rb_tree<T, Compare>::
insert_equal(InputIterator first, InputIterator last)
{
  for (; first != last; ++first)
  {
    insert_equal(*first);
  }
}

// 插入新值，节点键值不允许重复，返回一个 pair，若插入成功，pair 的第二参数为 true，否则为 false
template <class T, class Compare>
mystl::pair<typename rb_tree<T, Compare>::iterator, bool>
rb_tree<T, Compare>::
insert_unique(const value_type& value)
{
  auto y = header_;
  auto x = root();
  bool result = true;
  while (x != nullptr)
  {
    y = x;
    // value 键值小于当前节点键值
    result = key_comp_(tree_traits::get_key(value), tree_traits::get_key(x));  
    x = result ? x->left : x->right;  // 小于向左走，否则向右走
  }
  iterator j = iterator(y);          // 此时 y 为插入点的父节点
  if (result)
  { // 离开循坏时 result 为 true，在左边插入
    if (j == begin())
    {
      return mystl::make_pair(__insert_aux(x, y, value), true);
    }
    else
    {
      --j;
    }
  }
  if (key_comp_(tree_traits::get_key(j.node), tree_traits::get_key(value)))  // 新节点没有重复
  {
    return mystl::make_pair(__insert_aux(x, y, value), true);
  }
  // 进行至此，表示新节点与现有节点键值重复
  return mystl::make_pair(j, false);
}

// 从 position 开始插入新值，节点键值不允许重复
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
insert_unique(iterator position, const value_type& value)
{
  if (position.node == header_->left)
  { // 位于 begin 处
    if (size() && key_comp_(tree_traits::get_key(value), tree_traits::get_key(position.node)))
    { // 新值键值小于 position 节点的键值
      return __insert_aux(position.node, position.node, value);
    }
    else
    {
      return insert_unique(value).first;
    }
  }
  else if (position.node == header_)
  { // 位于 end 处
    if (key_comp_(tree_traits::get_key(rightmost()), tree_traits::get_key(value)))
    { // 最大节点键值小于新值键值
      return __insert_aux(static_cast<base_ptr>(nullptr), rightmost(), value);
    }
    else
    {
      return insert_unique(value).first;
    }
  }
  else
  {
    auto before = position;
    --before;
    if (key_comp_(tree_traits::get_key(before.node), tree_traits::get_key(value)) &&
        key_comp_(tree_traits::get_key(value), tree_traits::get_key(position.node)))
    {
      // before < value < position
      if ((before.node)->right == nullptr)
      {
        return __insert_aux(static_cast<base_ptr>(nullptr), before.node, value);
      }
      else
      {
        return __insert_aux(position.node, position.node, value);
      }
    }
    else
    {
      return insert_unique(value).first;
    }
  }
}

// 插入[first, last)内的值，键值不允许重复
template <class T, class Compare>
template <class InputIterator>
void rb_tree<T, Compare>::
insert_unique(InputIterator first, InputIterator last)
{
  for (; first != last; ++first)
  {
    insert_unique(*first);
  }
}

// 删除 position 位置的节点
template <class T, class Compare>
void rb_tree<T, Compare>::
erase(iterator position)
{
  assert(position != end());
  auto y = __rb_tree_erase(position.node, header_->parent, 
                           header_->left, header_->right);
  __destroy_node(y->get_node_ptr());
  --node_count_;
}

// 删除与 x 键值相等的元素，返回删除的个数
template <class T, class Compare>
typename rb_tree<T, Compare>::size_type
rb_tree<T, Compare>::
erase(const key_type& x)
{
  auto p = equal_range(x);  // 找出与 x 键值相等的区间
  size_type n = mystl::distance(p.first, p.second);
  erase(p.first, p.second);
  return n;
}

// 删除[first, last)区间内的元素
template <class T, class Compare>
void rb_tree<T, Compare>::
erase(iterator first, iterator last)
{
  if (first == begin() && last == end())
  {
    clear();
  }
  else
  {
    while (first != last)
      erase(first++);
  }
}

// 清空 rb tree
template <class T, class Compare>
void rb_tree<T, Compare>::
clear()
{
  if (node_count_ != 0)
  {
    __erase_aux(root());
    leftmost() = header_;
    root() = nullptr;
    rightmost() = header_;
    node_count_ = 0;
  }
}

// 查找键值为 k 的节点，返回指向它的迭代器
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
find(const key_type& key)
{
  auto y = header_;  // 最后一个不小于 key 的节点
  auto x = root();
  while (x != nullptr)
  {
    if (!key_comp_(tree_traits::get_key(x), key))
    { // key 小于等于 x 键值，向左走
      y = x, x = x->left;
    }
    else
    { // key 大于 x 键值，向右走
      x = x->right;
    }
  }
  iterator j = iterator(y);
  return (j == end() || key_comp_(key, tree_traits::get_key(j.node))) ? end() : j;
}

template <class T, class Compare>
typename rb_tree<T, Compare>::const_iterator
rb_tree<T, Compare>::
find(const key_type& key) const
{
  auto y = header_;  // 最后一个不小于 key 的节点
  auto x = root();
  while (x != nullptr)
  {
    if (!key_comp_(tree_traits::get_key(x), key))
    { // key 小于等于 x 键值，向左走
      y = x, x = x->left;
    }
    else
    { // key 大于 x 键值，向右走
      x = x->right;
    }
  }
  const_iterator j = const_iterator(y);
  return (j == end() || key_comp_(key, tree_traits::get_key(j.node))) ? end() : j;
}

// 查找键值为 key 的节点出现的次数
template <class T, class Compare>
typename rb_tree<T, Compare>::size_type
rb_tree<T, Compare>::
count(const key_type& key) const
{
  auto p = equal_range(key);
  size_type n = mystl::distance(p.first, p.second);
  return n;
}

// 键值不小于 key 的第一个位置
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
lower_bound(const key_type& key)
{
  auto y = header_;
  auto x = root();
  while (x != nullptr)
  {
    if (!key_comp_(tree_traits::get_key(x), key))
    { // key <= x
      y = x, x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  return iterator(y);
}

template <class T, class Compare>
typename rb_tree<T, Compare>::const_iterator
rb_tree<T, Compare>::
lower_bound(const key_type& key) const
{
  auto y = header_;
  auto x = root();
  while (x != nullptr)
  {
    if (!key_comp_(tree_traits::get_key(x), key))
    { // key <= x
      y = x, x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  return const_iterator(y);
}

// 键值不小于 key 的最后一个位置
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
upper_bound(const key_type& key)
{
  auto y = header_;
  auto x = root();
  while (x != nullptr)
  {
    if (key_comp_(key, tree_traits::get_key(x)))
    { // key < x
      y = x, x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  return iterator(y);
}

template <class T, class Compare>
typename rb_tree<T, Compare>::const_iterator
rb_tree<T, Compare>::
upper_bound(const key_type& key) const
{
  auto y = header_;
  auto x = root();
  while (x != nullptr)
  {
    if (key_comp_(key, tree_traits::get_key(x)))
    { // key < x
      y = x, x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  return const_iterator(y);
}

// 查找与键值 key 相等的区间，返回一个 pair 分别指向相等区间的首尾
template <class T, class Compare>
mystl::pair<typename rb_tree<T, Compare>::iterator,
  typename rb_tree<T, Compare>::iterator>
rb_tree<T, Compare>::
equal_range(const key_type& key)
{
  return mystl::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
}

template <class T, class Compare>
mystl::pair<typename rb_tree<T, Compare>::const_iterator,
  typename rb_tree<T, Compare>::const_iterator>
  rb_tree<T, Compare>::
  equal_range(const key_type& key) const
{
  return mystl::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
}

// 交换 rb tree
template <class T, class Compare>
void rb_tree<T, Compare>::
swap(rb_tree& rhs)
{
  if (this != &rhs)
  {
    mystl::swap(header_, rhs.header_);
    mystl::swap(node_count_, rhs.node_count_);
    mystl::swap(key_comp_, rhs.key_comp_);
  }
}

// 创建一个结点
template <class T, class Compare>
typename rb_tree<T, Compare>::node_ptr
rb_tree<T, Compare>::
__create_node(const value_type& x)
{
  auto tmp = node_allocator::allocate(1);
  try
  {
    data_allocator::construct(&tmp->value, x);
  }
  catch (...)
  {
    node_allocator::deallocate(tmp);
    throw;
  }
  return tmp;
}

// 复制一个结点
template <class T, class Compare>
typename rb_tree<T, Compare>::node_ptr
rb_tree<T, Compare>::
__clone_node(base_ptr x)
{
  node_ptr tmp = __create_node(x->get_node_ptr()->value);
  tmp->color = x->color;
  tmp->left = nullptr;
  tmp->right = nullptr;
  return tmp;
}

// 销毁一个结点
template <class T, class Compare>
void rb_tree<T, Compare>::
__destroy_node(node_ptr p)
{
  data_allocator::destroy(&p->value);
  node_allocator::deallocate(p);
}

// 初始化容器
template <class T, class Compare>
void rb_tree<T, Compare>::
__rb_tree_initialize()
{
  header_ = base_allocator::allocate(1);
  header_->color = __rb_tree_red;  // header_ 节点颜色为红，与 root 区分
  header_->parent = nullptr;
  header_->left = header_;
  header_->right = header_;
}

// __reset 函数
template<class T, class Compare>
void rb_tree<T, Compare>::__reset()
{
  header_ = nullptr;
  node_count_ = 0;
}

// __copy_assign 函数
template <class T, class Compare>
typename rb_tree<T, Compare>::base_ptr
rb_tree<T, Compare>::__copy_assign(base_ptr x, base_ptr p)
{
  // 递归构造一颗树，节点从 x 开始，p 为 x 的父节点
  auto top = __clone_node(x);
  top->parent = p;
  try
  {
    if (x->right)
      top->right = __copy_assign(x->right, top);
    p = top;
    x = x->left;
    while (x != nullptr)
    {
      auto y = __clone_node(x);
      p->left = y;
      y->parent = p;
      if (x->right)
        y->right = __copy_assign(x->right, y);
      p = y;
      x = x->left;
    }
  }
  catch (...)
  {
    __erase_aux(top);
    throw;
  }
  return top;
}

// __insert_aux 函数
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
__insert_aux(base_ptr x_, base_ptr y_, const value_type& value)
{
  // x_ 为新值插入点，y_ 为插入点的父节点，value 为要插入的值
  auto x = x_->get_node_ptr();
  auto y = y_->get_node_ptr();
  node_ptr z;
  if (y == header_ || x != nullptr
      || key_comp_(tree_traits::get_key(value), tree_traits::get_key(y)))
  {
    z = __create_node(value);
    y->left = z;        // 当 y 为 header_ 时，leftmost() = z
    if (y == header_)
    {
      root() = z;
      rightmost() = z;
    }
    else if (y == leftmost())
    {  // 如果 y 为最左节点
      leftmost() = z;   // 维护 leftmost(),使其永远指向最左节点
    }
  }
  else
  {
    z = __create_node(value);
    y->right = z;
    if (y == rightmost())
      rightmost() = z;  // 维护 rightmost(),使其永远指向最右节点
  }
  z->parent = y;
  z->left = nullptr;
  z->right = nullptr;
  __rb_tree_insert(z->get_base_ptr(), header_->parent);
  ++node_count_;
  return iterator(z);
}

// __erase_aux 函数
template <class T, class Compare>
void rb_tree<T, Compare>::
__erase_aux(base_ptr x)
{
  while (x != nullptr)
  {
    __erase_aux(x->right);
    auto y = x->left;
    __destroy_node(x->get_node_ptr());
    x = y;
  }
}

// 重载比较操作符
template <class T, class Compare>
bool operator==(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
{
  return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, class Compare>
bool operator<(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
{
  return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Compare>
bool operator!=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
{
  return !(lhs == rhs);
}

template <class T, class Compare>
bool operator>(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
{
  return rhs < lhs;
}

template <class T, class Compare>
bool operator<=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
{
  return !(rhs < lhs);
}

template <class T, class Compare>
bool operator>=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T, class Compare>
void swap(rb_tree<T, Compare>& lhs, rb_tree<T, Compare>& rhs)
{
  lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_RB_TREE_H_

