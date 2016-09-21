#ifndef RB_TREE_H
#define RB_TREE_H


#include "iterator.h"
#include "reverse_iterator.h"
#include "memory.h"
#include "pair.h"

namespace MyTinySTL {

	// RB-tree 节点颜色的类型，红色为0，黑色为1
	typedef bool	__rb_tree_color_type;
	const __rb_tree_color_type	__rb_tree_red = false;
	const __rb_tree_color_type	__rb_tree_black = true;

	// RB-tree 节点的双层设计
	struct __rb_tree_node_base {
		typedef __rb_tree_color_type	color_type;
		typedef __rb_tree_node_base*	base_ptr;
		color_type color;	//节点颜色
		base_ptr parent;	//父节点
		base_ptr left;	//左节点
		base_ptr right;	//右节点

		static base_ptr minimum(base_ptr x) {
			while (x->left != 0)	x = x->left;	//一直向左走，找到最小值
			return x;
		}

		static base_ptr maximum(base_ptr x) {
			while (x->right != 0)	x = x->right;	//一直向右走，找到最大值
			return x;
		}
	};

	template <class T>
	struct __rb_tree_node :public __rb_tree_node_base {
		typedef __rb_tree_node<T>*	link_type;
		T value;	//节点值
	};

	// RB-tree 迭代器的双层设计
	struct __rb_tree_iterator_base {
		typedef __rb_tree_node_base::base_ptr	base_ptr;
		typedef bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t	difference_type;

		base_ptr node;	//指向节点本身

		// 用于 operator++
		void incremet() {
			if (node->right != 0) {	//如果有右子节点
				node = node->right;	//就向右走
				while (node->left != 0)	//然后一直向左走
					node = node->left;	//找到下一个节点
			}
			else {	//没有右子节点了
				base_ptr y = node->parent;	//找到父节点
				while (y->right == node) { //如果该节点为父节点的右子节点
					node = y;	//一直上溯，直到节点不为右子节点
					y = y->parent;
				}
				if (node->right != y)	//应对“寻找根节点的下一节点，而根节点没有右子节点”的特殊情况
					node = y;
			}
		}

		// 用于 operator--
		void decrement() {
			if (node->color == __rb_tree_red && node->parent->parent == node) {
				//如果是红节点，而且父节点的父节点等于自己，此时 node 为 header
				node = node->right;	//指向整棵树的 max 节点
			}
			else if (node->left != 0) {	//如果有左子节点
				base_ptr y = node->left;	//向左走
				while (y->right != 0)	//然后一直向右走
					y = y->right;
				node = y;	// 找到前一个节点
			}
			else {	//非根节点，也无左子节点
				base_ptr y = node->parent;	//找到父节点
				while (node == y->left) {	//如果该节点为父节点的左子节点
					node = y;	//一直上溯直到不为左子节点
					y = y->parent;
				}
				node = y;	//此时父节点为要找的前一个节点
			}
		}
	};

	template <class T, class Ref, class Ptr>
	struct __rb_tree_iterator :public __rb_tree_iterator_base {
		typedef T	value_type;
		typedef Ref	reference;
		typedef Ptr	pointer;
		typedef __rb_tree_iterator<T, T&, T*>	iterator;
		typedef __rb_tree_iterator<T, const T&, const T*>	const_iterator;
		typedef __rb_tree_iterator<T, Ref, Ptr>	self;
		typedef __rb_tree_node<T>*	link_type;

		// 构造函数
		__rb_tree_iterator() {}
		__rb_tree_iterator(link_type x) { node = x; }
		__rb_tree_iterator(const iterator& x) { node = x.node; }

		// 重载操作符
		reference operator*() const { return ((link_type)node)->value; }
		pointer operator->() const { return &(operator*()); }
		self& operator++() {
			incremet();
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			incremet();
			return tmp;
		}
		self& operator--() {
			decrement();
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			decrement();
			return tmp;
		}
	};

	inline bool operator==(const __rb_tree_iterator_base& x, __rb_tree_iterator_base& y) {
		return x.node == y.node;
	}

	inline bool operator!=(const __rb_tree_iterator_base& x, __rb_tree_iterator_base& y) {
		return !(x == y);
	}

	// 调整 RB-tree，旋转与改变颜色

	//-------------------------------------------------------------------------------
	//		            x				    y
	//			   / \				   / \
	//			  a    y		=>        x   c
	//			      / \			 / \
	//			     b	 c			a   b
	//-------------------------------------------------------------------------------
	// 左旋，参数一为左旋点，参数二为根节点
	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x->right;	//设置y为旋转点的右子节点
		x->right = y->left;	//x的右子节点为y的左子节点
		if (y->left != 0)	
			y->left->parent = x;	//设置y的左子节点的父节点为x
		y->parent = x->parent;	//设置y的父节点
		if (x == root)	//如果x为根节点
			root = y;
		else if (x == x->parent->left)	//如果x是它父节点的左子节点
			x->parent->left = y;	//设置x的父节点的左子节点为y
		else	//如果x是它父节点的右子节点
			x->parent->right = y;	//设置x的父节点的右子节点为y
		y->left = x;	//连接x与y的关系
		x->parent = y;	
	}

	//-------------------------------------------------------------------------------
	//			    x				    y
	//			   / \				   / \
	//			  y   c		=>		  a   x
	//			 / \				     / \
	//			a   b				    b	c
	//-------------------------------------------------------------------------------
	// 右旋，参数一为右旋点，参数二为根节点
	inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x->left;	//设置y为右旋点的左子节点
		x->left = y->right;		//设置x的左子节点为y的右子节点
		if (y->right != 0)
			y->right->parent = x;	//设置y右子节点的父节点为x
		y->parent = x->parent;	//设置y的父节点为x的父节点
		if (x == root)	//如果x为根节点
			root = y;	//根节点调整为y
		else if (x == x->parent->right)	//如果x是它父节点的右子节点
			x->parent->right = y;	//将x的父节点的右子节点设置成y
		else	//如果x是它父节点的左子节点
			x->parent->left = y;	//将x的父节点的左子节点设置成y
		y->right = x;	//连接x与y的关系
		x->parent = y;	
	}

	// 重新另 RB-tree 平衡，参数一为新增节点，参数二为根节点
	inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		x->color = __rb_tree_red;	//新增节点为红色
		while (x != root && x->parent->color == __rb_tree_red) {	//当父节点为红
			if (x->parent == x->parent->parent->left) {	//如果父节点是左子节点
				__rb_tree_node_base* y = x->parent->parent->right;	
				if (y && y->color == __rb_tree_red) {	//伯父节点存在且为红
					x->parent->color = __rb_tree_black;	//更改父节点为黑
					y->color = __rb_tree_black;	//更改伯父节点为黑
					x->parent->parent->color = __rb_tree_red;	//更改祖父节点为红
					x = x->parent->parent;	//调整x
				}
				else {	//无伯父节点或伯父节点为黑
					if (x == x->parent->right) {	//如果x为父节点的右子节点
						x = x->parent;
						__rb_tree_rotate_left(x, root);
					}
					x->parent->color = __rb_tree_black;	//调整颜色
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			else {	//如果父节点是右子节点
				__rb_tree_node_base* y = x->parent->parent->left;	
				if (y && y->color == __rb_tree_red) {	//伯父节点存在且为红
					x->parent->color = __rb_tree_black;	//更改父节点为黑
					y->color = __rb_tree_black;	//更改伯父节点为黑
					x->parent->parent->color = __rb_tree_red;	//更改祖父节点为红
					x = x->parent->parent;	//调整x
				}
				else {	//无伯父节点或伯父节点为黑
					if (x == x->parent->left) {	//如果x为父节点的左子节点
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}
					x->parent->color = __rb_tree_black;	//调整颜色
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = __rb_tree_black;	//根节点永远为黑
	}

	// 因删除节点而要使 RB-tree 重新平衡，参数一为要删除的节点
	inline __rb_tree_node_base* __rb_tree_rebalance_for_erase(__rb_tree_node_base* z,
		__rb_tree_node_base*& root, __rb_tree_node_base*& leftmost,
		__rb_tree_node_base*& rightmost) {
		__rb_tree_node_base* y = z;
		__rb_tree_node_base* x = 0;
		__rb_tree_node_base* x_parent = 0;

		// 寻找可以代替的节点
		if (y->left == 0)	//被删节点的左子节点为 NULL
			x = y->right;
		else {
			if (y->right == 0)	//被删节点的右子节点为 NULL
				x = y->left;
			else {	//被删节点的左右节点都不为 NULL
				y = y->right;
				while (y->left != 0)	//从被删节点的右子节点一直向左走，找到要替代的节点
					y = y->left;
				x = y->right;
			}
		}

		// 调整代替节点的节点关系
		if (y != z) {	// y 变换了
			z->left->parent = y;	//用y替代被删节点
			y->left = z->left;	//设置新的左子节点
			if (y != z->right) {
				x_parent = y->parent;
				if (x)	x->parent = y->parent;	//调整替代节点的右子节点的父子关系
				y->parent->left = x;
				y->right = z->right;	//设置新的右子节点
				z->right->parent = y;
			}
			else
				x_parent = y;
			if (root == z)	//如果被删节点为根节点
				root = y;	//另替代节点为根节点
			else if (z->parent->left == z)	//调整替代节点的父子关系
				z->parent->left = y;	
			else	
				z->parent->right = y;	
			y->parent = z->parent;
			MyTinySTL::swap(y->color, z->color);
			y = z;	//另 y 指向要删除的节点
		}
		else {	// y == z，x 为替代节点
			x_parent = y->parent;	
			if (x)	x->parent = y->parent;
			if (root == z)	//如果被删节点为根节点
				root = x;
			else	//调整替代节点的父子关系
				if (z->parent->left == z)
					z->parent->left = x;
				else
					z->parent->right = x;
			if (leftmost == z)	//被删节点为 min 节点，调整最小值
				if (z->right == 0)
					leftmost = z->parent;
				else
					leftmost = __rb_tree_node_base::minimum(x);
			if (rightmost == z)	//被删节点为 max 节点，调整最大值
				if (z->left == 0)
					rightmost = z->parent;
				else
					rightmost = __rb_tree_node_base::maximum(x);
		}

		// 调整节点颜色
		if (y->color != __rb_tree_red) {	//如果被删节点颜色为黑
			while(x != root && (x == 0 || x->color == __rb_tree_black))
				if (x == x_parent->left) {	//如果替代节点是左子节点
					__rb_tree_node_base* w = x_parent->right;
					if (w->color == __rb_tree_red) {
						w->color = __rb_tree_black;
						x_parent->color = __rb_tree_red;
						__rb_tree_rotate_left(x_parent, root);
						w = x_parent->right;
					}
					if ((w->left == 0 || w->left->color == __rb_tree_black)
						&& (w->right == 0 || w->right->color == __rb_tree_black)) {
						w->color = __rb_tree_red;
						x = x_parent;
						x_parent = x_parent->parent;
					}
					else {
						if (w->right == 0 || w->right->color == __rb_tree_black) {
							if (w->left)	w->left->color = __rb_tree_black;
							w->color = __rb_tree_red;
							__rb_tree_rotate_right(w, root);
							w = x_parent->right;
						}
						w->color = x_parent->color;
						x_parent->color = __rb_tree_black;
						if (w->right)	w->right->color = __rb_tree_black;
						__rb_tree_rotate_left(x_parent, root);
						break;
					}
				}
				else {	//替代节点为右子节点
					__rb_tree_node_base* w = x_parent->left;
					if (w->color == __rb_tree_red) {
						w->color = __rb_tree_black;
						x_parent->color = __rb_tree_red;
						__rb_tree_rotate_right(x_parent, root);
						w = x_parent->left;
					}
					if ((w->left == 0 || w->left->color == __rb_tree_black)
						&& (w->right == 0 || w->right->color == __rb_tree_black)) {
						w->color = __rb_tree_red;
						x = x_parent;
						x_parent = x_parent->parent;
					}
					else {
						if (w->left == 0 || w->left->color == __rb_tree_black) {
							if (w->right)	w->right->color = __rb_tree_black;
							w->color = __rb_tree_red;
							__rb_tree_rotate_left(w, root);
							w = x_parent->left;
						}
						w->color = x_parent->color;
						x_parent->color = __rb_tree_black;
						if (w->left)	w->left->color = __rb_tree_black;
						__rb_tree_rotate_right(x_parent, root);
						break;
					}
				}
				if (x)	x->color = __rb_tree_black;
		}
		return y;
	}

	// RB-tree 的定义
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
	class rb_tree {
	protected:
		// RB-tree 型别设定
		typedef __rb_tree_node_base*	base_ptr;
		typedef __rb_tree_node<Value>	rb_tree_node;
		typedef __rb_tree_color_type	color_type;
		typedef Alloc	allocator_type;

	public:
		typedef Key	key_type;
		typedef Value	value_type;
		typedef value_type*	pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&	reference;
		typedef const value_type&	const_reference;
		typedef rb_tree_node*	link_type;
		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;

		// 迭代器型别
		typedef __rb_tree_iterator<Value, Value&, Value*>	iterator;
		typedef __rb_tree_iterator<Value, const Value&, const Value*>	const_iterator;
		typedef reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef reverse_iterator<iterator>	reverse_iterator;

		typedef allocator<rb_tree_node, Alloc>	rb_tree_node_allocator;	//空间配置器
		allocator_type get_allocator() const { return allocator_type(); }

	private:
		// RB-tree 用以下三个数据表现
		size_type node_count;	//树的节点数目
		link_type header;	//一个特殊节点(与root节点互为对方的父节点)
		Compare key_compare;	//节点键值比较的准则

	protected:
		// 节点相关操作
		link_type __get_node() { return rb_tree_node_allocator::allocate(); }
		void __put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }
		link_type __create_node(const value_type& x);
		link_type __clone_node(link_type x);
		void __destroy_node(link_type p);

	protected:
		// 以下三个函数用于取得 header 的成员
		link_type& root() const { return (link_type&)header->parent; }
		link_type& leftmost() const { return (link_type&)header->left; }
		link_type& rightmost() const { return (link_type&)header->right; }

		// 以下六个函数用来取得节点x的成员
		static link_type& left(link_type x) { return (link_type&)(x->left); }
		static link_type& right(link_type x) { return (link_type&)(x->right); }
		static link_type& parent(link_type x) { return (link_type&)(x->parent); }
		static reference value(link_type x) { return x->value; }
		static const key_type key(link_type x) { return KeyOfValue()(value(x)); }
		static color_type& color(link_type x) { return (color_type&)(x->color); }

		// 以下六个函数用来取得基层节点x的成员
		static link_type& left(base_ptr x) { return (link_type&)(x->left); }
		static link_type& right(base_ptr x) { return (link_type&)(x->right); }
		static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
		static reference value(base_ptr x) { return ((link_type)x)->value; }
		static const key_type key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
		static color_type& color(base_ptr x) { return (color_type&)(link_type(x)->color); }

		// 求极大值和极小值
		static link_type minimum(link_type x) { 
			return (link_type)__rb_tree_node_base::minimum(x); 
		}
		static link_type maximum(link_type x) {
			return (link_type)__rb_tree_node_base::maximum(x);
		}

	public:
		// 构造、复制、析构函数
		rb_tree() { __initialize(); }
		rb_tree(const rb_tree& x);
		rb_tree& operator=(const rb_tree& x);
		~rb_tree() { clear(); }

	public:
		// 迭代器相关操作
		iterator begin() { return leftmost(); }
		const_iterator begin() const { return leftmost(); }
		iterator end() { return header; }
		const_iterator end() const { return header; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

		// 容量相关操作
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }
		size_type max_size() const { return size_type(-1); }

		// 修改容器相关操作
		iterator insert_equal(const value_type& x);
		iterator insert_equal(iterator position, const value_type& x);
		template <class InputIterator>
		void insert_equal(InputIterator first, InputIterator last);
		pair<iterator, bool> insert_unique(const value_type& x);
		iterator insert_unique(iterator position, const value_type& x);
		template <class InputIterator>
		void insert_unique(InputIterator first, InputIterator last);
		void erase(iterator position);
		size_type erase(const key_type& x);
		void erase(iterator first, iterator last);
		void clear();
		void swap(rb_tree& y);

		// 容器相关操作
		iterator find(const key_type& k);
		size_type count(const key_type& k);
		iterator lower_bound(const key_type& k);
		iterator upper_bound(const key_type& k);
		inline pair<iterator, iterator> equal_range(const key_type& k);
		Compare key_comp() const { return key_compare; }

	protected:
		// 内部成员函数
		void __initialize();
		link_type __copy(link_type x, link_type p);
		iterator __insert(base_ptr _x, base_ptr _y, const value_type& value);
		void __erase(link_type x);
	};

	/**********************************************************************************/

	// 创建一个结点
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::link_type
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__create_node(const value_type& x) {
		link_type tmp = __get_node();
		try {
			MyTinySTL::construct(&tmp->value, x);
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
		link_type tmp = __create_node(x->value);
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}

	// 销毁一个结点
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__destroy_node(link_type p) {
		MyTinySTL::destroy(&p->value);
		__put_node(p);
	}

	// 复制构造函数
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const rb_tree& x) {
		if (x.root() == 0)
			__initialize();
		else {
			color(header) = __rb_tree_red;
			root() = __copy(x.root(), header);
			leftmost() = minimum(root());
			rightmost() = maximum(root());
		}
		node_count = x.node_count;
	}

	// 赋值操作符operator=
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& 
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const rb_tree& x) {
		if (this != &x) {
			clear();
			node_count = 0;
			key_compare = x.key_compare;
			if (x.root() == 0) {
				root() = 0;
				leftmost() = header;
				rightmost() = header;
			}
			else {
				root() = __copy(x.root(), header);
				leftmost() = minimum(root());
				rightmost() = maximum(root());
				node_count = x.node_count;
			}
		}
		return *this;
	}

	// 插入新值，节点允许重复
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		insert_equal(const value_type& value) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {	//从根节点开始往下搜索合适的插入点
			y = x;
			x = key_compare(KeyOfValue()(value), key(x)) ? left(x) : right(x);	//小于向左走
		}
		return __insert(x, y, value);
	}

	// 从 position 位置开始插入新值，节点允许重复
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		insert_equal(iterator position, const value_type& value) {
		if (position.node == header->left) {	// 位于 begin() 处
			if (size() > 0 && !key_compare(key(position.node), KeyOfValue()(value)))
				// position 节点键值大于等于新值键值
				return __insert(position.node, position.node, value);
			else
				return insert_equal(value);	//寻找合适的插入点
		}
		else if (position.node == header) {	//位于 end() 处
			if (!key_compare(KeyOfValue()(value), key(rightmost())))
				// 新值键值大于等于 max 节点键值
				return __insert(0, rightmost(), value);
			else
				return insert_equal(value);	//寻找合适的插入点
		}
		else {
			iterator before = position;
			--before;
			if (!key_compare(KeyOfValue()(value), key(before.node))
				&& !key_compare(key(position.node), KeyOfValue()(value))) {
				// before <= value <= position
				if (right(before.node) == 0)	//如果 before 节点的右子节点为 NULL
					return __insert(0, before.node, value);
				else
					return __insert(position.node, position.node, value);
			}
			else
				return insert_equal(value);	//寻找合适的插入点
		}
	}

	// 插入[first, last)区间内的值，键值允许重复
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	template <class InputIterator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		insert_equal(InputIterator first, InputIterator last) {
		for (; first != last; ++first)
			insert_equal(*first);
	}

	// 插入新值，节点键值不允许重复，返回一个 pair，若插入无效，pair 的第二参数为 false
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		insert_unique(const value_type& value) {
		link_type y = header;
		link_type x = root();
		bool comp = true;	//比较标准
		while (x != 0) {
			y = x;
			comp = key_compare(KeyOfValue()(value), key(x));	// value键值小于当前节点键值
			x = comp ? left(x) : right(x);	//小于向左走，否则向右走
		}
		// 此时y为插入点的父节点
		iterator j = iterator(y);
		if (comp)	// 离开循坏时 comp 为 true，插在左边
			if (j == begin())	//父节点为最左节点
				return pair<iterator, bool>(__insert(x, y, value), true);
			else
				--j;
		if (key_compare(key(j.node), KeyOfValue()(value)))	//新节点没有重复
			return pair<iterator, bool>(__insert(x, y, value), true);
		//进行至此，表示新节点与现有节点键值重复
		return pair<iterator, bool>(j, false);
	}

	// 从 position 开始插入新值，节点键值不允许重复
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		insert_unique(iterator position, const value_type& value) {
		if (position.node == header->left) {	// 位于 begin() 处
			if (size() > 0 && key_compare(KeyOfValue()(value), key(position.node)))	//新值键值小于position节点的键值
				return __insert(position.node, position.node, value);
			else
				return insert_unique(value);	//寻找合适的插入点
		}
		else if (position.node == header) {
			if (key_compare(key(rightmost()), KeyOfValue()(value)))	// max 节点键值小于新值键值
				return __insert(0, rightmost(), value);
			else
				return insert_unique(value);	//寻找合适的插入点
		}
		else {
			iterator before = position;
			--before;
			if (key_compare(key(before.node), KeyOfValue()(value))
				&& key_compare(KeyOfValue()(value), key(position.node))) {
				// before < value < position
				if (right(before.node) == 0)	// before 节点的右子节点为 NULL
					return __insert(0, before.node, value);
				else
					return __insert(position.node, position.node, value);
			}
			else
				return insert_unique(value);	//寻找合适的插入点
		}
	}

	// 插入[first, last)内的值，键值不允许重复
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	template <class InputIterator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		insert_unique(InputIterator first, InputIterator last) {
		for (; first != last; ++first)
			insert_unique(*first);
	}

	// 删除 position 位置的节点
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position) {
		link_type y = (link_type)__rb_tree_rebalance_for_erase(position.node,
			header->parent, header->left, header->right);
		__destroy_node(y);
		--node_count;
	}

	// 删除与 x 键值相等的元素，返回删除的个数
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type& x) {
		pair<iterator, iterator> p = equal_range(x);	//找出与x键值相等的区间
		size_type n = distance(p.first, p.second);	//区间长度
		erase(p.first, p.second);
		return n;
	}

	// 删除[first, last)区间内的元素
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		erase(iterator first, iterator last) {
		if (first == begin() && last == end())
			clear();
		else
			while (first != last)	erase(first++);
	}

	// 清空 RB-tree
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
		if (node_count != 0) {
			__erase(root());
			leftmost() = header;
			root() = 0;
			rightmost() = header;
			node_count = 0;
		}
	}

	// 交换 RB-tree
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::swap(rb_tree& y) {
		MyTinySTL::swap(header, y.header);
		MyTinySTL::swap(node_count, y.node_count);
		MyTinySTL::swap(key_compare, y.key_compare);
	}

	// 查找键值为 k 的节点，返回其迭代器
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) {
		link_type y = header;	// 最后一个不小于 k 的节点
		link_type x = root();
		while (x != 0) {
			if (!key_compare(key(x), k))	// k 小于等于 x键值，向左走
				y = x, x = left(x);
			else	// k 大于 x键值，向右走
				x = right(x);
		}
		iterator j = iterator(y);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}

	// 查找键值为 k 的节点出现的次数
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) {
		pair<iterator, iterator> p = equal_range(k);
		size_type n = distance(p.first, p.second);
		return n;
	}

	// 键值不小于 k 的第一个位置
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			if (!key_compare(key(x), k))	// k <= x
				y = x, x = left(x);
			else
				x = right(x);
		}
		return iterator(y);
	}

	// 键值不小于 k 的最后一个位置
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			if (key_compare(k, key(x)))	// k < x
				y = x, x = left(x);
			else
				x = right(x);
		}
		return iterator(y);
	}

	// 查找与键值 k 相等的区间，返回一个 pair
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
		typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) {
		return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
	}

	// __initialize 函数
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__initialize() {
		header = __get_node();	//产生一个结点，另header指向它
		color(header) = __rb_tree_red;	//另header为红，区分 root
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}

	// __copy 函数
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x, link_type p) {
		link_type top = __clone_node(x);
		top->parent = p;
		try {
			if (x->right)
				top->right = __copy(right(x), top);
			p = top;
			x = left(x);
			while (x != 0) {
				link_type y = __clone_node(x);
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
	}

	// __insert 函数
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		__insert(base_ptr _x, base_ptr _y, const value_type& value) {
		// _x 为新值插入点，_y 为插入点的父节点，value 为新值
		link_type x = (link_type)_x;
		link_type y = (link_type)_y;
		link_type z;
		if (y == header || x != 0 || key_compare(KeyOfValue()(value), key(y))) {
			z = __create_node(value);
			left(y) = z;	//使当 y 为 header 时，leftmost() = z
			if (y == header) {
				root() = z;
				rightmost() = z;
			}
			else if (y == leftmost())	// 如果y为最左节点
				leftmost() = z;	//维护 leftmost(),使其永远指向最左节点
		}
		else {
			z = __create_node(value);
			right(y) = z;	//另新节点成为插入点父节点的右子节点
			if (y == rightmost())
				rightmost() = z;	//维护 rightmost(),使其永远指向最右节点
		}
		parent(z) = y;	//设置新节点的父节点
		left(z) = 0;	//设置新节点的左子节点
		right(z) = 0;	//设置新节点的右子节点
		__rb_tree_rebalance(z, header->parent);
		++node_count;
		return iterator(z);
	}

	// __erase 函数
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x) {
		while (x != 0) {
			__erase(right(x));
			link_type y = left(x);
			__destroy_node(x);
			x = y;
		}
	}

	// 重载比较操作符
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline bool operator==(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {
		return x.size() == y.size() &&
			MyTinySTL::equal(x.begin(), x.end(), y.begin(), y.end());
	}

	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline bool operator<(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {
		return MyTinySTL::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
	}

	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline bool operator!=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {
		return !(x == y);
	}

	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline bool operator>(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {
		return y < x;
	}

	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline bool operator<=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {
		return !(y < x);
	}

	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline bool operator>=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {
		return !(x < y);
	}

	// 重载 MyTinySTL 的 swap
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline void swap(rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x,
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& y) {
		x.swap(y);
	}
}
#endif // !RB_TREE_H

