#ifndef MYTINYSTL_RB_TREE_H_
#define MYTINYSTL_RB_TREE_H_

// 这个头文件包含一个模板类 rb_tree
// rb_tree : 红黑树

#include <initializer_list>

#include <cassert>

#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "type_traits.h"
#include "exceptdef.h"

namespace mystl
{

// rb tree 节点颜色的类型

typedef bool rb_tree_color_type;

static constexpr rb_tree_color_type rb_tree_red   = false;
static constexpr rb_tree_color_type rb_tree_black = true;

// forward declaration

template <class T> struct rb_tree_node_base;
template <class T> struct rb_tree_node;

template <class T> struct rb_tree_iterator;
template <class T> struct rb_tree_const_iterator;

// rb tree value traits

template <class T, bool>
struct rb_tree_value_traits_imp
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
struct rb_tree_value_traits_imp<T, true>
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
struct rb_tree_value_traits
{
  static constexpr bool is_map = mystl::is_pair<T>::value;

  typedef rb_tree_value_traits_imp<T, is_map> value_traits_type;

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

// rb tree node traits

template <class T>
struct rb_tree_node_traits
{
  typedef rb_tree_color_type                 color_type;

  typedef rb_tree_value_traits<T>            value_traits;
  typedef typename value_traits::key_type    key_type;
  typedef typename value_traits::mapped_type mapped_type;
  typedef typename value_traits::value_type  value_type;

  typedef rb_tree_node_base<T>*              base_ptr;
  typedef rb_tree_node<T>*                   node_ptr;
};

// rb tree 的节点设计

template <class T>
struct rb_tree_node_base
{
  typedef rb_tree_color_type    color_type;
  typedef rb_tree_node_base<T>* base_ptr;
  typedef rb_tree_node<T>*      node_ptr;

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
struct rb_tree_node :public rb_tree_node_base<T>
{
  typedef rb_tree_node_base<T>* base_ptr;
  typedef rb_tree_node<T>*      node_ptr;

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

// rb tree traits

template <class T>
struct rb_tree_traits
{
  typedef rb_tree_value_traits<T>            value_traits;

  typedef typename value_traits::key_type    key_type;
  typedef typename value_traits::mapped_type mapped_type;
  typedef typename value_traits::value_type  value_type;

  typedef value_type*                        pointer;
  typedef value_type&                        reference;
  typedef const value_type*                  const_pointer;
  typedef const value_type&                  const_reference;

  typedef rb_tree_node_base<T>               base_type;
  typedef rb_tree_node<T>                    node_type;

  typedef base_type*                         base_ptr;
  typedef node_type*                         node_ptr;
};

// rb tree 的迭代器设计

template <class T>
struct rb_tree_iterator_base :public mystl::iterator<mystl::bidirectional_iterator_tag, T>
{
  typedef typename rb_tree_traits<T>::base_ptr  base_ptr;

  base_ptr node;  // 指向节点本身

  rb_tree_iterator_base() :node(nullptr) {}

  // 使迭代器前进
  void inc()
  {
    if (node->right != nullptr)
    { 
      node = rb_tree_min(node->right);
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
    if (node->parent->parent == node && rb_tree_is_red(node))
    { // 如果 node 为 header
      node = node->right;  // 指向整棵树的 max 节点
    }
    else if (node->left != nullptr)
    {
      node = rb_tree_max(node->left);
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

  bool operator==(const rb_tree_iterator_base& rhs) { return node == rhs.node; }
  bool operator!=(const rb_tree_iterator_base& rhs) { return node != rhs.node; }
};

template <class T>
struct rb_tree_iterator :public rb_tree_iterator_base<T>
{
  typedef rb_tree_traits<T>                tree_traits;

  typedef typename tree_traits::value_type value_type;
  typedef typename tree_traits::pointer    pointer;
  typedef typename tree_traits::reference  reference;
  typedef typename tree_traits::base_ptr   base_ptr;
  typedef typename tree_traits::node_ptr   node_ptr;

  typedef rb_tree_iterator<T>              iterator;
  typedef rb_tree_const_iterator<T>        const_iterator;
  typedef iterator                         self;

  using rb_tree_iterator_base<T>::node;

  // 构造函数
  rb_tree_iterator() {}
  rb_tree_iterator(base_ptr x) { node = x; }
  rb_tree_iterator(node_ptr x) { node = x; }
  rb_tree_iterator(const iterator& rhs) { node = rhs.node; }
  rb_tree_iterator(const const_iterator& rhs) { node = rhs.node; }

  // 重载操作符
  reference operator*()  const { return node->get_node_ptr()->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    this->inc();
    return *this;
  }
  self operator++(int)
  {
    self tmp(*this);
    this->inc();
    return tmp;
  }
  self& operator--()
  {
    this->dec();
    return *this;
  }
  self operator--(int)
  {
    self tmp(*this);
    this->dec();
    return tmp;
  }
};

template <class T>
struct rb_tree_const_iterator :public rb_tree_iterator_base<T>
{
  typedef rb_tree_traits<T>                     tree_traits;

  typedef typename tree_traits::value_type      value_type;
  typedef typename tree_traits::const_pointer   pointer;
  typedef typename tree_traits::const_reference reference;
  typedef typename tree_traits::base_ptr        base_ptr;
  typedef typename tree_traits::node_ptr        node_ptr;

  typedef rb_tree_iterator<T>                   iterator;
  typedef rb_tree_const_iterator<T>             const_iterator;
  typedef const_iterator                        self;

  using rb_tree_iterator_base<T>::node;

  // 构造函数
  rb_tree_const_iterator() {}
  rb_tree_const_iterator(base_ptr x) { node = x; }
  rb_tree_const_iterator(node_ptr x) { node = x; }
  rb_tree_const_iterator(const iterator& rhs) { node = rhs.node; }
  rb_tree_const_iterator(const const_iterator& rhs) { node = rhs.node; }

  // 重载操作符
  reference operator*()  const { return node->get_node_ptr()->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    this->inc();
    return *this;
  }
  self operator++(int)
  {
    self tmp(*this);
    this->inc();
    return tmp;
  }
  self& operator--()
  {
    this->dec();
    return *this;
  }
  self operator--(int)
  {
    self tmp(*this);
    this->dec();
    return tmp;
  }
};

// tree algorithm

template <class NodePtr>
NodePtr rb_tree_min(NodePtr x) noexcept
{
  while (x->left != nullptr)
    x = x->left;
  return x;
}

template <class NodePtr>
NodePtr rb_tree_max(NodePtr x) noexcept
{
  while (x->right != nullptr)
    x = x->right;
  return x;
}

template <class NodePtr>
bool rb_tree_is_lchild(NodePtr node) noexcept
{
  return node == node->parent->left;
}

template <class NodePtr>
bool rb_tree_is_red(NodePtr node) noexcept
{
  return node->color == rb_tree_red;
}

template <class NodePtr>
void rb_tree_set_black(NodePtr& node) noexcept
{
  node->color = rb_tree_black;
}

template <class NodePtr>
void rb_tree_set_red(NodePtr& node) noexcept
{
  node->color = rb_tree_red;
}

template <class NodePtr>
NodePtr rb_tree_next(NodePtr node) noexcept
{
  if (node->right != nullptr)
    return rb_tree_min(node->right);
  while (!rb_tree_is_lchild(node))
    node = node->parent;
  return node->parent;
}

/*---------------------------------------*\
|       p                         p       |
|      / \                       / \      |
|     x   d    rotate left      y   d     |
|    / \       ===========>    / \        |
|   a   y                     x   c       |
|      / \                   / \          |
|     b   c                 a   b         |
\*---------------------------------------*/
// 左旋，参数一为左旋点，参数二为根节点
template <class NodePtr>
void rb_tree_rotate_left(NodePtr x, NodePtr& root) noexcept
{
  auto y = x->right;  // y 为 x 的右子节点
  x->right = y->left;
  if (y->left != nullptr)
    y->left->parent = x;
  y->parent = x->parent;

  if (x == root)
  { // 如果 x 为根节点，让 y 顶替 x 成为根节点
    root = y;
  }
  else if (rb_tree_is_lchild(x))
  { // 如果 x 是左子节点
    x->parent->left = y;
  }
  else
  { // 如果 x 是右子节点
    x->parent->right = y;
  }
  // 调整 x 与 y 的关系
  y->left = x;  
  x->parent = y;
}

/*----------------------------------------*\
|     p                         p          |
|    / \                       / \         |
|   d   x      rotate right   d   y        |
|      / \     ===========>      / \       |
|     y   a                     b   x      |
|    / \                           / \     |
|   b   c                         c   a    |
\*----------------------------------------*/
// 右旋，参数一为右旋点，参数二为根节点
template <class NodePtr>
void rb_tree_rotate_right(NodePtr x, NodePtr& root) noexcept
{
  auto y = x->left;
  x->left = y->right;
  if (y->right)
    y->right->parent = x;
  y->parent = x->parent;

  if (x == root)
  { // 如果 x 为根节点，让 y 顶替 x 成为根节点
    root = y;
  }
  else if (rb_tree_is_lchild(x))
  { // 如果 x 是右子节点
    x->parent->left = y;
  }
  else
  { // 如果 x 是左子节点
    x->parent->right = y;
  }
  // 调整 x 与 y 的关系
  y->right = x;                      
  x->parent = y;
}

// 插入节点后使 rb tree 重新平衡，参数一为新增节点，参数二为根节点
//
// case 1: 新增节点位于根节点，令新增节点为黑
// case 2: 新增节点的父节点为黑，没有破坏平衡，直接返回
// case 3: 父节点和叔叔节点都为红，令父节点和叔叔节点为黑，祖父节点为红，
//         然后令祖父节点为当前节点，继续处理
// case 4: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为右（左）孩子，
//         让父节点成为当前节点，再以当前节点为支点左（右）旋
// case 5: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为左（右）孩子，
//         让父节点变为黑色，祖父节点变为红色，以祖父节点为支点右（左）旋
//
// 参考博客: http://blog.csdn.net/v_JULY_v/article/details/6105630
//          http://blog.csdn.net/v_JULY_v/article/details/6109153
template <class NodePtr>
void rb_tree_insert_rebalance(NodePtr x, NodePtr& root) noexcept
{
  rb_tree_set_red(x);  // 新增节点为红色
  while (x != root && rb_tree_is_red(x->parent))
  {
    if (rb_tree_is_lchild(x->parent))
    { // 如果父节点是左子节点
      auto uncle = x->parent->parent->right;
      if (uncle != nullptr && rb_tree_is_red(uncle))
      { // case 3: 父节点和叔叔节点都为红
        rb_tree_set_black(x->parent);
        rb_tree_set_black(uncle);
        x = x->parent->parent;
        rb_tree_set_red(x);
      }
      else
      { // 无叔叔节点或叔叔节点为黑
        if (!rb_tree_is_lchild(x))
        { // case 4: 当前节点 x 为右子节点
          x = x->parent;
          rb_tree_rotate_left(x, root);
        }
        // 都转换成 case 5： 当前节点为左子节点
        rb_tree_set_black(x->parent);
        rb_tree_set_red(x->parent->parent);
        rb_tree_rotate_right(x->parent->parent, root);
        break;
      }
    }
    else  // 如果父节点是右子节点，对称处理
    { 
      auto uncle = x->parent->parent->left;
      if (uncle != nullptr && rb_tree_is_red(uncle))
      { // case 3: 父节点和叔叔节点都为红
        rb_tree_set_black(x->parent);
        rb_tree_set_black(uncle);
        x = x->parent->parent;
        rb_tree_set_red(x);
        // 此时祖父节点为红，可能会破坏红黑树的性质，令当前节点为祖父节点，继续处理
      }
      else
      { // 无叔叔节点或叔叔节点为黑
        if (rb_tree_is_lchild(x))
        { // case 4: 当前节点 x 为左子节点
          x = x->parent;
          rb_tree_rotate_right(x, root);
        }
        // 都转换成 case 5： 当前节点为左子节点
        rb_tree_set_black(x->parent);
        rb_tree_set_red(x->parent->parent);
        rb_tree_rotate_left(x->parent->parent, root);
        break;
      }
    }
  }
  rb_tree_set_black(root);  // 根节点永远为黑
}

// 删除节点后使 rb tree 重新平衡，参数一为要删除的节点，参数二为根节点，参数三为最小节点，参数四为最大节点
// 
// 参考博客: http://blog.csdn.net/v_JULY_v/article/details/6105630
//          http://blog.csdn.net/v_JULY_v/article/details/6109153
template <class NodePtr>
NodePtr rb_tree_erase_rebalance(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost)
{
  // y 是可能的替换节点，指向最终要删除的节点
  auto y = (z->left == nullptr || z->right == nullptr) ? z : rb_tree_next(z);
  // x 是 y 的一个独子节点或 NIL 节点
  auto x = y->left != nullptr ? y->left : y->right;
  // xp 为 x 的父节点
  NodePtr xp = nullptr;

  // y != z 说明 z 有两个非空子节点，此时 y 指向 z 右子树的最左节点，x 指向 y 的右子节点。
  // 用 y 顶替 z 的位置，用 x 顶替 y 的位置，最后用 y 指向 z
  if (y != z)
  {
    z->left->parent = y;
    y->left = z->left;

    // 如果 y 不是 z 的右子节点，那么 z 的右子节点一定有左孩子
    if (y != z->right)
    { // x 替换 y 的位置
      xp = y->parent;
      if (x != nullptr)
        x->parent = y->parent;

      y->parent->left = x;
      y->right = z->right;
      z->right->parent = y;
    }
    else
    {
      xp = y;
    }

    // 连接 y 与 z 的父节点 
    if (root == z)
      root = y;
    else if (rb_tree_is_lchild(z))
      z->parent->left = y;
    else
      z->parent->right = y;
    y->parent = z->parent;
    mystl::swap(y->color, z->color);
    y = z;
  }
  // y == z 说明 z 至多只有一个孩子
  else
  { 
    xp = y->parent;
    if (x)  
      x->parent = y->parent;

    // 连接 x 与 z 的父节点
    if (root == z)
      root = x;
    else if (rb_tree_is_lchild(z))
      z->parent->left = x;
    else
      z->parent->right = x;

    // 此时 z 有可能是最左节点或最右节点，更新数据
    if (leftmost == z)
      leftmost = x == nullptr ? xp : rb_tree_min(x);
    if (rightmost == z)
      rightmost = x == nullptr ? xp : rb_tree_max(x);
  }

  // 此时，y 指向要删除的节点，x 为替代节点，从 x 节点开始调整。
  // 如果删除的节点为红色，树的性质没有被破坏，否则按照以下情况调整（x 为左子节点为例）：
  // case 1: 兄弟节点为红色，令父节点为红，兄弟节点为黑，进行左（右）旋，继续处理
  // case 2: 兄弟节点为黑色，且两个子节点都为黑色或 NIL，令兄弟节点为红，父节点成为当前节点，继续处理
  // case 3: 兄弟节点为黑色，左子节点为红色或 NIL，右子节点为黑色或 NIL，
  //         令兄弟节点为红，兄弟节点的左子节点为黑，以兄弟节点为支点右（左）旋，继续处理
  // case 4: 兄弟节点为黑色，右子节点为红色，令兄弟节点为父节点的颜色，父节点为黑色，兄弟节点的右子节点
  //         为黑色，以父节点为支点左（右）旋，树的性质调整完成，算法结束
  if (!rb_tree_is_red(y))
  { // x 为黑色时，调整，否则直接将 x 变为黑色即可
    while (x != root && (x == nullptr || !rb_tree_is_red(x)))
    {
      if (x == xp->left)
      { // 如果 x 为左子节点
        auto brother = xp->right;
        if (rb_tree_is_red(brother))
        { // case 1
          rb_tree_set_black(brother);
          rb_tree_set_red(xp);
          rb_tree_rotate_left(xp, root);
          brother = xp->right;
        }
        // case 1 转为为了 case 2、3、4 中的一种
        if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
            (brother->right == nullptr || !rb_tree_is_red(brother->right)))
        { // case 2
          rb_tree_set_red(brother);
          x = xp;
          xp = xp->parent;
        }
        else
        { 
          if (brother->right == nullptr || !rb_tree_is_red(brother->right))
          { // case 3
            if (brother->left != nullptr)
              rb_tree_set_black(brother->left);
            rb_tree_set_red(brother);
            rb_tree_rotate_right(brother, root);
            brother = xp->right;
          }
          // 转为 case 4
          brother->color = xp->color;
          rb_tree_set_black(xp);
          if (brother->right != nullptr)  
            rb_tree_set_black(brother->right);
          rb_tree_rotate_left(xp, root);
          break;
        }
      }
      else  // x 为右子节点，对称处理
      { 
        auto brother = xp->left;
        if (rb_tree_is_red(brother))
        { // case 1
          rb_tree_set_black(brother);
          rb_tree_set_red(xp);
          rb_tree_rotate_right(xp, root);
          brother = xp->left;
        }
        if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
            (brother->right == nullptr || !rb_tree_is_red(brother->right)))
        { // case 2
          rb_tree_set_red(brother);
          x = xp;
          xp = xp->parent;
        }
        else
        {
          if (brother->left == nullptr || !rb_tree_is_red(brother->left))
          { // case 3
            if (brother->right != nullptr)
              rb_tree_set_black(brother->right);
            rb_tree_set_red(brother);
            rb_tree_rotate_left(brother, root);
            brother = xp->left;
          }
          // 转为 case 4
          brother->color = xp->color;
          rb_tree_set_black(xp);
          if (brother->left != nullptr)  
            rb_tree_set_black(brother->left);
          rb_tree_rotate_right(xp, root);
          break;
        }
      }
    }
    if (x != nullptr)
      rb_tree_set_black(x);
  }
  return y;
}

// 模板类 rb_tree
// 参数一代表数据类型，参数二代表键值比较类型
template <class T, class Compare>
class rb_tree
{
public:
  // rb_tree 的嵌套型别定义 
  
  typedef rb_tree_traits<T>                        tree_traits;
  typedef rb_tree_value_traits<T>                  value_traits;

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

  typedef rb_tree_iterator<T>                      iterator;
  typedef rb_tree_const_iterator<T>                const_iterator;
  typedef mystl::reverse_iterator<iterator>        reverse_iterator;
  typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

  allocator_type get_allocator() const { return node_allocator(); }
  key_compare    key_comp()      const { return key_comp_; }

private:
  // 用以下三个数据表现 rb tree
  base_ptr    header_;      // 特殊节点，与根节点互为对方的父节点
  size_type   node_count_;  // 节点数
  key_compare key_comp_;    // 节点键值比较的准则

private:
  // 以下三个函数用于取得根节点，最小节点和最大节点
  base_ptr& root()      const { return header_->parent; }
  base_ptr& leftmost()  const { return header_->left; }
  base_ptr& rightmost() const { return header_->right; }

public:
  // 构造、复制、析构函数
  rb_tree() { rb_tree_init(); }

  rb_tree(const rb_tree& rhs);
  rb_tree(rb_tree&& rhs) noexcept;

  rb_tree& operator=(const rb_tree& rhs);
  rb_tree& operator=(rb_tree&& rhs);

  ~rb_tree() { clear(); }

public:
  // 迭代器相关操作

  iterator               begin()         noexcept 
  { return leftmost(); }
  const_iterator         begin()   const noexcept
  { return leftmost(); }
  iterator               end()           noexcept
  { return header_; }
  const_iterator         end()     const noexcept
  { return header_; }

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

  // 容量相关操作

  bool      empty()    const noexcept { return node_count_ == 0; }
  size_type size()     const noexcept { return node_count_; }
  size_type max_size() const noexcept { return static_cast<size_type>(-1); }

  // 插入删除相关操作

  // emplace

  template <class ...Args>
  iterator  emplace_multi(Args&& ...args);

  template <class ...Args>
  mystl::pair<iterator, bool> emplace_unique(Args&& ...args);

  template <class ...Args>
  iterator  emplace_multi_use_hint(iterator hint, Args&& ...args);

  template <class ...Args>
  iterator  emplace_unique_use_hint(iterator hint, Args&& ...args);

  // insert

  iterator  insert_multi(const value_type& value);
  iterator  insert_multi(value_type&& value)
  {
    return emplace_multi(mystl::move(value));
  }

  iterator  insert_multi(iterator hint, const value_type& value)
  {
    return emplace_multi_use_hint(hint, value);
  }
  iterator  insert_multi(iterator hint, value_type&& value)
  {
    return emplace_multi_use_hint(hint, mystl::move(value));
  }

  template <class InputIterator>
  void      insert_multi(InputIterator first, InputIterator last)
  {
    size_type n = mystl::distance(first, last);
    THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Comp>'s size too big");
    for (; n > 0; --n, ++first)
      insert_multi(end(), *first);
  }

  mystl::pair<iterator, bool> insert_unique(const value_type& value);
  mystl::pair<iterator, bool> insert_unique(value_type&& value)
  {
    return emplace_unique(mystl::move(value));
  }

  iterator  insert_unique(iterator hint, const value_type& value)
  {
    return emplace_unique_use_hint(hint, value);
  }
  iterator  insert_unique(iterator hint, value_type&& value)
  {
    return emplace_unique_use_hint(hint, mystl::move(value));
  }

  template <class InputIterator>
  void      insert_unique(InputIterator first, InputIterator last)
  {
    size_type n = mystl::distance(first, last);
    THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Comp>'s size too big");
    for (; n > 0; --n, ++first)
      insert_unique(end(), *first);
  }

  // erase

  iterator  erase(iterator hint);

  size_type erase_multi(const key_type& key);
  size_type erase_unique(const key_type& key);

  void      erase(iterator first, iterator last);

  void      clear();

  // rb_tree 相关操作

  iterator       find(const key_type& key);
  const_iterator find(const key_type& key) const;

  size_type      count_multi(const key_type& key) const
  {
    auto p = equal_range_multi(key);
    return static_cast<size_type>(mystl::distance(p.first, p.second));
  }
  size_type      count_unique(const key_type& key) const
  {
    return find(key) != end() ? 1 : 0;
  }

  iterator       lower_bound(const key_type& key);
  const_iterator lower_bound(const key_type& key) const;

  iterator       upper_bound(const key_type& key);
  const_iterator upper_bound(const key_type& key) const;

  mystl::pair<iterator, iterator>             
  equal_range_multi(const key_type& key)
  {
    return mystl::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
  }
  mystl::pair<const_iterator, const_iterator>
  equal_range_multi(const key_type& key) const
  {
    return mystl::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
  }

  mystl::pair<iterator, iterator>             
  equal_range_unique(const key_type& key)
  {
    iterator it = find(key);
    auto next = it;
    return it == end() ? mystl::make_pair(it, it) : mystl::make_pair(it, ++next);
  }
  mystl::pair<const_iterator, const_iterator> 
  equal_range_unique(const key_type& key) const
  {
    const_iterator it = find(key);
    auto next = it;
    return it == end() ? mystl::make_pair(it, it) : mystl::make_pair(it, ++next);
  }

  void swap(rb_tree& rhs) noexcept;

private:

  // node related
  template <class ...Args>
  node_ptr create_node(Args&&... args);
  node_ptr clone_node(base_ptr x);
  void     destroy_node(node_ptr p);

  // init / reset
  void     rb_tree_init();
  void     reset();

  // get insert pos
  mystl::pair<base_ptr, bool> 
           get_insert_multi_pos(const key_type& key);
  mystl::pair<mystl::pair<base_ptr, bool>, bool> 
           get_insert_unique_pos(const key_type& key);

  // insert value / insert node
  iterator insert_value_at(base_ptr x, const value_type& value, bool add_to_left);
  iterator insert_node_at(base_ptr x, node_ptr node, bool add_to_left);

  // insert use hint
  iterator insert_multi_use_hint(iterator hint, key_type key, node_ptr node);
  iterator insert_unique_use_hint(iterator hint, key_type key, node_ptr node);

  // copy tree / erase tree
  base_ptr copy_from(base_ptr x, base_ptr p);
  void     erase_since(base_ptr x);
};

/*****************************************************************************************/

// 复制构造函数
template <class T, class Compare>
rb_tree<T, Compare>::
rb_tree(const rb_tree& rhs)
{
  rb_tree_init();
  if (rhs.node_count_ != 0)
  {
    root() = copy_from(rhs.root(), header_);
    leftmost() = rb_tree_min(root());
    rightmost() = rb_tree_max(root());
  }
  node_count_ = rhs.node_count_;
  key_comp_ = rhs.key_comp_;
}

// 移动构造函数
template <class T, class Compare>
rb_tree<T, Compare>::
rb_tree(rb_tree&& rhs) noexcept
  :header_(mystl::move(rhs.header_)),
  node_count_(rhs.node_count_),
  key_comp_(rhs.key_comp_)
{
  rhs.reset();
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
      root() = copy_from(rhs.root(), header_);
      leftmost() = rb_tree_min(root());
      rightmost() = rb_tree_max(root());
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
  clear();
  header_ = mystl::move(rhs.header_);
  node_count_ = rhs.node_count_;
  key_comp_ = rhs.key_comp_;
  rhs.reset();
  return *this;
}

// 就地插入元素，键值允许重复
template <class T, class Compare>
template <class ...Args>
typename rb_tree<T, Compare>::iterator 
rb_tree<T, Compare>::
emplace_multi(Args&& ...args)
{
  THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
  node_ptr np = create_node(mystl::forward<Args>(args)...);
  auto res = get_insert_multi_pos(value_traits::get_key(np->value));
  return insert_node_at(res.first, np, res.second);
}

// 就地插入元素，键值不允许重复
template <class T, class Compare>
template <class ...Args>
mystl::pair<typename rb_tree<T, Compare>::iterator, bool> 
rb_tree<T, Compare>::
emplace_unique(Args&& ...args)
{
  THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
  node_ptr np = create_node(mystl::forward<Args>(args)...);
  auto res = get_insert_unique_pos(value_traits::get_key(np->value));
  if (res.second)
  { // 插入成功
    return mystl::make_pair(insert_node_at(res.first.first, np, res.first.second), true);
  }
  destroy_node(np);
  return mystl::make_pair(iterator(res.first.first), false);
}

// 就地插入元素，键值允许重复，当 hint 位置与插入位置接近时，插入操作的时间复杂度可以降低
template <class T, class Compare>
template <class ...Args>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
emplace_multi_use_hint(iterator hint, Args&& ...args)
{
  THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
  node_ptr np = create_node(mystl::forward<Args>(args)...);
  if (node_count_ == 0)
  {
    return insert_node_at(header_, np, true);
  }
  key_type key = value_traits::get_key(np->value);
  if (hint == begin())
  { // 位于 begin 处
    if (key_comp_(key, value_traits::get_key(*hint)))
    {
      return insert_node_at(hint.node, np, true);
    }
    else
    {
      auto pos = get_insert_multi_pos(key);
      return insert_node_at(pos.first, np, pos.second);
    }
  }
  else if (hint == end())
  { // 位于 end 处
    if (!key_comp_(key, value_traits::get_key(rightmost()->get_node_ptr()->value)))
    {
      return insert_node_at(rightmost(), np, false);
    }
    else
    {
      auto pos = get_insert_multi_pos(key);
      return insert_node_at(pos.first, np, pos.second);
    }
  }
  return insert_multi_use_hint(hint, key, np);
}

// 就地插入元素，键值不允许重复，当 hint 位置与插入位置接近时，插入操作的时间复杂度可以降低
template<class T, class Compare>
template<class ...Args>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
emplace_unique_use_hint(iterator hint, Args&& ...args)
{
  THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
  node_ptr np = create_node(mystl::forward<Args>(args)...);
  if (node_count_ == 0)
  {
    return insert_node_at(header_, np, true);
  }
  key_type key = value_traits::get_key(np->value);
  if (hint == begin())
  { // 位于 begin 处
    if (key_comp_(key, value_traits::get_key(*hint)))
    {
      return insert_node_at(hint.node, np, true);
    }
    else
    {
      auto pos = get_insert_unique_pos(key);
      if (!pos.second)
      {
        destroy_node(np);
        return pos.first.first;
      }
      return insert_node_at(pos.first.first, np, pos.first.second);
    }
  }
  else if (hint == end())
  { // 位于 end 处
    if (key_comp_(value_traits::get_key(rightmost()->get_node_ptr()->value), key))
    {
      return insert_node_at(rightmost(), np, false);
    }
    else
    {
      auto pos = get_insert_unique_pos(key);
      if (!pos.second)
      {
        destroy_node(np);
        return pos.first.first;
      }
      return insert_node_at(pos.first.first, np, pos.first.second);
    }
  }
  return insert_unique_use_hint(hint, key, np);
}

// 插入元素，节点键值允许重复
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
insert_multi(const value_type& value)
{
  THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
  auto res = get_insert_multi_pos(value_traits::get_key(value));
  return insert_value_at(res.first, value, res.second);
}

// 插入新值，节点键值不允许重复，返回一个 pair，若插入成功，pair 的第二参数为 true，否则为 false
template <class T, class Compare>
mystl::pair<typename rb_tree<T, Compare>::iterator, bool>
rb_tree<T, Compare>::
insert_unique(const value_type& value)
{
  THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
  auto res = get_insert_unique_pos(value_traits::get_key(value));
  if (res.second)
  { // 插入成功
    return mystl::make_pair(insert_value_at(res.first.first, value, res.first.second), true);
  }
  return mystl::make_pair(res.first.first, false);
}

// 删除 hint 位置的节点
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
erase(iterator hint)
{
  auto node = hint.node->get_node_ptr();
  iterator next(node);
  ++next;
  
  rb_tree_erase_rebalance(hint.node, root(), leftmost(), rightmost());
  destroy_node(node);
  --node_count_;
  return next;
}

// 删除键值等于 key 的元素，返回删除的个数
template <class T, class Compare>
typename rb_tree<T, Compare>::size_type
rb_tree<T, Compare>::
erase_multi(const key_type& key)
{
  auto p = equal_range_multi(key);
  size_type n = mystl::distance(p.first, p.second);
  erase(p.first, p.second);
  return n;
}

// 删除键值等于 key 的元素，返回删除的个数
template <class T, class Compare>
typename rb_tree<T, Compare>::size_type
rb_tree<T, Compare>::
erase_unique(const key_type& key)
{
  auto it = find(key);
  if (it != end())
  {
    erase(it);
    return 1;
  }
  return 0;
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
    erase_since(root());
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
    if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
    { // key 小于等于 x 键值，向左走
      y = x, x = x->left;
    }
    else
    { // key 大于 x 键值，向右走
      x = x->right;
    }
  }
  iterator j = iterator(y);
  return (j == end() || key_comp_(key, value_traits::get_key(*j))) ? end() : j;
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
    if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
    { // key 小于等于 x 键值，向左走
      y = x, x = x->left;
    }
    else
    { // key 大于 x 键值，向右走
      x = x->right;
    }
  }
  const_iterator j = const_iterator(y);
  return (j == end() || key_comp_(key, value_traits::get_key(*j))) ? end() : j;
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
    if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
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
    if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
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
    if (key_comp_(key, value_traits::get_key(x->get_node_ptr()->value)))
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
    if (key_comp_(key, value_traits::get_key(x->get_node_ptr()->value)))
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

// 交换 rb tree
template <class T, class Compare>
void rb_tree<T, Compare>::
swap(rb_tree& rhs) noexcept
{
  if (this != &rhs)
  {
    mystl::swap(header_, rhs.header_);
    mystl::swap(node_count_, rhs.node_count_);
    mystl::swap(key_comp_, rhs.key_comp_);
  }
}

/*****************************************************************************************/
// helper function

// 创建一个结点
template <class T, class Compare>
template <class ...Args>
typename rb_tree<T, Compare>::node_ptr
rb_tree<T, Compare>::
create_node(Args&&... args)
{
  auto tmp = node_allocator::allocate(1);
  try
  {
    data_allocator::construct(mystl::address_of(tmp->value), mystl::forward<Args>(args)...);
    tmp->left = nullptr;
    tmp->right = nullptr;
    tmp->parent = nullptr;
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
clone_node(base_ptr x)
{
  node_ptr tmp = create_node(x->get_node_ptr()->value);
  tmp->color = x->color;
  tmp->left = nullptr;
  tmp->right = nullptr;
  return tmp;
}

// 销毁一个结点
template <class T, class Compare>
void rb_tree<T, Compare>::
destroy_node(node_ptr p)
{
  data_allocator::destroy(&p->value);
  node_allocator::deallocate(p);
}

// 初始化容器
template <class T, class Compare>
void rb_tree<T, Compare>::
rb_tree_init()
{
  header_ = base_allocator::allocate(1);
  header_->color = rb_tree_red;  // header_ 节点颜色为红，与 root 区分
  root() = nullptr;
  leftmost() = header_;
  rightmost() = header_;
  node_count_ = 0;
}

// reset 函数
template <class T, class Compare>
void rb_tree<T, Compare>::reset()
{
  header_ = nullptr;
  node_count_ = 0;
}

// get_insert_multi_pos 函数
template <class T, class Compare>
mystl::pair<typename rb_tree<T, Compare>::base_ptr, bool>
rb_tree<T, Compare>::get_insert_multi_pos(const key_type& key)
{
  auto x = root();
  auto y = header_;
  bool add_to_left = true;
  while (x != nullptr)
  {
    y = x;
    add_to_left = key_comp_(key, value_traits::get_key(x->get_node_ptr()->value));
    x = add_to_left ? x->left : x->right;
  }
  return mystl::make_pair(y, add_to_left);
}

// get_insert_unique_pos 函数
template <class T, class Compare>
mystl::pair<mystl::pair<typename rb_tree<T, Compare>::base_ptr, bool>, bool>
rb_tree<T, Compare>::get_insert_unique_pos(const key_type& key)
{ // 返回一个 pair，第一个值为一个 pair，包含插入点的父节点和一个 bool 表示是否在左边插入，
  // 第二个值为一个 bool，表示是否插入成功
  auto x = root();
  auto y = header_;
  bool add_to_left = true;  // 树为空时也在 header_ 左边插入
  while (x != nullptr)
  {
    y = x;
    add_to_left = key_comp_(key, value_traits::get_key(x->get_node_ptr()->value));
    x = add_to_left ? x->left : x->right;
  }
  iterator j = iterator(y);  // 此时 y 为插入点的父节点
  if (add_to_left)
  {
    if (y == header_ || j == begin())
    { // 如果树为空树或插入点在最左节点处，肯定可以插入新的节点
      return mystl::make_pair(mystl::make_pair(y, true), true);
    }
    else
    { // 否则，如果存在重复节点，那么 --j 就是重复的值
      --j;
    }
  }
  if (key_comp_(value_traits::get_key(*j), key))  
  { // 表明新节点没有重复
    return mystl::make_pair(mystl::make_pair(y, add_to_left), true);
  }
  // 进行至此，表示新节点与现有节点键值重复
  return mystl::make_pair(mystl::make_pair(y, add_to_left), false);
}

// insert_value_at 函数
// x 为插入点的父节点， value 为要插入的值，add_to_left 表示是否在左边插入
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
insert_value_at(base_ptr x, const value_type& value, bool add_to_left)
{
  node_ptr node = create_node(value);
  node->parent = x;
  auto base_node = node->get_base_ptr();
  if (x == header_)
  {
    root() = base_node;
    leftmost() = base_node;
    rightmost() = base_node;
  }
  else if (add_to_left)
  {
    x->left = base_node;
    if (leftmost() == x)
      leftmost() = base_node;
  }
  else
  {
    x->right = base_node;
    if (rightmost() == x)
      rightmost() = base_node;
  }
  rb_tree_insert_rebalance(base_node, root());
  ++node_count_;
  return iterator(node);
}

// 在 x 节点处插入新的节点
// x 为插入点的父节点， node 为要插入的节点，add_to_left 表示是否在左边插入
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::
insert_node_at(base_ptr x, node_ptr node, bool add_to_left)
{
  node->parent = x;
  auto base_node = node->get_base_ptr();
  if (x == header_)
  {
    root() = base_node;
    leftmost() = base_node;
    rightmost() = base_node;
  }
  else if (add_to_left)
  {
    x->left = base_node;
    if (leftmost() == x)
      leftmost() = base_node;
  }
  else
  {
    x->right = base_node;
    if (rightmost() == x)
      rightmost() = base_node;
  }
  rb_tree_insert_rebalance(base_node, root());
  ++node_count_;
  return iterator(node);
}

// 插入元素，键值允许重复，使用 hint
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator 
rb_tree<T, Compare>::
insert_multi_use_hint(iterator hint, key_type key, node_ptr node)
{
  // 在 hint 附近寻找可插入的位置
  auto np = hint.node;
  auto before = hint;
  --before;
  auto bnp = before.node;
  if (!key_comp_(key, value_traits::get_key(*before)) &&
      !key_comp_(value_traits::get_key(*hint), key))
  { // before <= node <= hint
    if (bnp->right == nullptr)
    {
      return insert_node_at(bnp, node, false);
    }
    else if (np->left == nullptr)
    {
      return insert_node_at(np, node, true);
    }
  }
  auto pos = get_insert_multi_pos(key);
  return insert_node_at(pos.first, node, pos.second);
}

// 插入元素，键值不允许重复，使用 hint
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator 
rb_tree<T, Compare>::
insert_unique_use_hint(iterator hint, key_type key, node_ptr node)
{
  // 在 hint 附近寻找可插入的位置
  auto np = hint.node;
  auto before = hint;
  --before;
  auto bnp = before.node;
  if (key_comp_(value_traits::get_key(*before), key) &&
      key_comp_(key, value_traits::get_key(*hint)))
  { // before < node < hint
    if (bnp->right == nullptr)
    {
      return insert_node_at(bnp, node, false);
    }
    else if (np->left == nullptr)
    {
      return insert_node_at(np, node, true);
    }
  }
  auto pos = get_insert_unique_pos(key);
  if (!pos.second)
  {
    destroy_node(node);
    return pos.first.first;
  }
  return insert_node_at(pos.first.first, node, pos.first.second);
}

// copy_from 函数
// 递归复制一颗树，节点从 x 开始，p 为 x 的父节点
template <class T, class Compare>
typename rb_tree<T, Compare>::base_ptr
rb_tree<T, Compare>::copy_from(base_ptr x, base_ptr p)
{
  auto top = clone_node(x);
  top->parent = p;
  try
  {
    if (x->right)
      top->right = copy_from(x->right, top);
    p = top;
    x = x->left;
    while (x != nullptr)
    {
      auto y = clone_node(x);
      p->left = y;
      y->parent = p;
      if (x->right)
        y->right = copy_from(x->right, y);
      p = y;
      x = x->left;
    }
  }
  catch (...)
  {
    erase_since(top);
    throw;
  }
  return top;
}

// erase_since 函数
// 从 x 节点开始删除该节点及其子树
template <class T, class Compare>
void rb_tree<T, Compare>::
erase_since(base_ptr x)
{
  while (x != nullptr)
  {
    erase_since(x->right);
    auto y = x->left;
    destroy_node(x->get_node_ptr());
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
void swap(rb_tree<T, Compare>& lhs, rb_tree<T, Compare>& rhs) noexcept
{
  lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_RB_TREE_H_

