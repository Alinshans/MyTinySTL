#ifndef LIST_H
#define LIST_H

#include "iterator.h"
#include "reverse_iterator.h"
#include "memory.h"

namespace MyTinySTL {

	// list的节点结构
	template <class T>
	struct __list_node {
		T data;	//数据域
		__list_node<T>* prev;	// 指向前一个节点
		__list_node<T>* next;	// 指向下一个节点
		__list_node() {
			data = 0;
			prev = NULL;
			next = NULL;
		}
		__list_node(T value = 0, __list_node<T>* p = NULL, __list_node<T>* n = NULL)
			:data(value), prev(p), next(n) {}
	};

	// list的迭代器
	template <class T, class Ref, class Ptr>
	struct __list_iterator : public iterator<bidirectional_iterator_tag, T> {
		typedef __list_iterator<T, T&, T*>	iterator;
		typedef __list_iterator<T, const T&, const T*>	const_iterator;
		typedef __list_iterator<T, Ref, Ptr>	self;

		typedef T	value_type;
		typedef Ptr	pointer;
		typedef Ref	reference;
		typedef size_t size_type;

		typedef __list_node<T>*	link_type;
		link_type	node;

		// 构造函数
		__list_iterator() {}
		__list_iterator(link_type x) :node(x) {}
		__list_iterator(const iterator& x) :node(x.node) {}

		// 重载操作符
		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
		reference operator*() const { return (*node).data; }
		pointer operator->() const { return &(operator*()); }
		self& operator++() {
			node = node->next;
			return *this;
		}
		self& operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}
		self& operator--() {
			node = node->prev;
			return *this;
		}
		self& operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}
	};

	// list:链表
	template <class T, class Alloc = allocator<__list_node<T>>>
	class list {
	public:
		// list 的嵌套型别定义
		typedef T		value_type;
		typedef Alloc	allocate_type;
		typedef value_type*	pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&	reference;
		typedef const value_type&	const_reference;
		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;
		typedef __list_iterator<T, T&, T*>	iterator;
		typedef __list_iterator<T, const T&, const T*>	const_iterator;
		typedef reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef reverse_iterator<iterator>	reverse_iterator;

	public:
		typedef Alloc	data_allocate;	// list 的空间配置器

	public:
		typedef	__list_node<T>*	link_type;	// list 的节点类型

	protected:
		link_type node;	// 指向尾端的一个空白节点

	public:
		// 构造函数
		list() { __initialize(); }
		explicit list(size_type n);
		explicit list(size_type n, const T& value);
		template <class InputIterator>
		list(InputIterator first, InputIterator last);

		// 复制构造函数
		list(const list& x);

		// 赋值操作符 operator=
		list& operator=(const list& x);

		// 析构函数
		~list();

	public:
		// 迭代器相关操作
		iterator begin() { return (link_type)node->next; }
		const_iterator begin() const { return (link_type)node->next; }
		iterator end() { return node; }
		const_iterator end() const { return node; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return reverse_iterator(begin()); }

		// 容量相关操作
		bool empty() const { return node->next == node; }
		size_type size() const { return (size_type)distance(begin(), end()); }
		size_type max_size() const { return size_type(-1); }

		// 访问元素相关操作
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(--end()); }
		const_reference back() const { return *(--end()); }

		// 调整容器相关操作
		void assign(size_type n, const T& value) { __fill_assign(n, value); }
		void assign(size_type n) { __fill_assign(n, T()); }
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		iterator insert(iterator position, const T& x);
		iterator insert(iterator position) { return insert(position, T()); }
		void insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void clear();
		void push_front(const T& x) { insert(begin(), x); }
		void push_front() { insert(begin()); }
		void push_back(const T& x) { insert(end(), x); }
		void push_back() { insert(end()); }
		void pop_front() { erase(begin()); }
		void pop_back() { iterator tmp = end(); erase(--tmp); }
		void resize(size_type new_size, const T& x);
		void resize(size_type new_size) { resize(new_size, T()); }
		void swap(list& x) { MyTinySTL::swap(node, x.node); }

		// 容器相关操作
		void splice(iterator position, list& x);
		void splice(iterator position, list&, iterator i);
		void splice(iterator position, list& x, iterator first, iterator last);
		void remove(const T& value);
		template <class Predicate>
		void remove_if(Predicate pred);
		void unique();
		void merge(list& x);
		void sort();
		void reverse();

		// 配置器相关操作
		allocate_type get_allocate() { return allocate_type(); }

	protected:
		// 内部成员函数
		link_type __create_node(const T& x = T());
		void __destroy_node(link_type p);
		void __initialize();
		void __fill_assign(size_type n, const T& value);
		template <class Integer>
		void __assign_dispatch(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __assign_dispatch(InputIterator first, InputIterator last, __false_type);
		void __fill_insert(iterator position, size_type n, const T& value);
		template <class Integer>
		void __insert_dispatch(iterator position, Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __insert_dispatch(iterator position, InputIterator first, InputIterator last,
			__false_type);
		void __transfer(iterator position, iterator first, iterator last);
	};

	/**********************************************************************************/

	// 构造函数
	template <class T, class Alloc>
	list<T, Alloc>::list(size_type n) {
		__initialize(); 
		insert(begin(), n, T());
	}

	template <class T, class Alloc>
	list<T, Alloc>::list(size_type n, const T& value) {
		__initialize(); 
		insert(begin(), n, value);
	}

	template <class T, class Alloc>
	template <class InputIterator>
	list<T, Alloc>::list(InputIterator first, InputIterator last) {
		__initialize();
		insert(begin(), first, last);
	}

	// 复制构造函数
	template <class T, class Alloc>
	list<T, Alloc>::list(const list& x) { 
		__initialize();
		insert(begin(), x.begin(), x.end());
	}

	// 赋值操作符
	template <class T, class Alloc>
	list<T, Alloc>& list<T, Alloc>::operator=(const list<T, Alloc>& x) {
		if (*this != x) {
			iterator first1 = begin();
			iterator last1 = end();
			const_iterator first2 = x.begin();
			const_iterator last2 = x.end();
			while (first1 != las1 && first2 != last2)
				*first1++ = *first2++;
			if (first2 == last2)
				erase(first1, last1);
			else
				insert(last1, first2, last2);
		}
		return *this;
	}

	// 析构函数
	template <class T, class Alloc>
	list<T, Alloc>::~list() {
		link_type first = (link_type)node->next;
		link_type last = (link_type)node;
		while (first != last) {
			link_type cur = first;
			first = first->next;
			__destroy_node(cur);
		}
	}

	// 初始化容器
	template <class T, class Alloc>
	template <class InputIterator>
	void list<T, Alloc>::assign(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer integer;
		__assign_dispatch(first, last, integer());
	}

	// 在 position 处插入元素
	template <class T, class Alloc>
	typename list<T, Alloc>::iterator 
		list<T, Alloc>::insert(iterator position, const T& x) {
		link_type tmp = __create_node(x);
		tmp->next = (link_type)position.node;
		tmp->prev = (link_type)position.node->prev;
		position.node->prev->next = tmp;
		position.node->prev = tmp;
		return tmp;
	}

	// 在 position 处插入 n 个元素
	template <class T, class Alloc>
	void list<T, Alloc>::insert(iterator position, size_type n, const T& x) {
		__fill_insert(position, n, x);
	}

	// 在 position 处插入 [first, last)区间内的元素
	template <class T, class Alloc>
	template <class InputIterator>
	void list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer integer;
		__insert_dispatch(position, first, last, integer());
	}

	// 删除 position 处的元素
	template <class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(iterator position) {
		link_type prev_node = (link_type)position.node->prev;
		link_type next_node = (link_type)position.node->next;
		link_type this_node = (link_type)position.node;
		prev_node->next = next_node;
		next_node->prev = prev_node;
		__destroy_node(this_node);
		return (iterator)next_node;
	}

	// 删除[first, last)内的元素
	template <class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(iterator first, iterator last) {
		while (first != last) {
			erase(first++);
		}
		return last;
	}

	// 重置容器大小
	template <class T, class Alloc>
	void list<T, Alloc>::resize(size_type new_size, const T& x) {
		iterator i = begin();
		size_type len = 0;
		while (i != end() && len < new_size) {
			++i;
			++len;
		}
		if (len == new_size)	
			erase(i, end());
		else	
			insert(end(), new_size - len, x);
	}

	// 清空 list
	template <class T, class Alloc>
	void list<T, Alloc>::clear() {
		link_type first = (link_type)node->next;
		link_type last = (link_type)node;
		while (first != last) {
			link_type cur = first;
			first = first->next;
			__destroy_node(cur);
		}
		node->prev = node->next = node;
	}

	// 将 list x 接合于 position 之前
	template <class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list& x) {
		if (!x.empty()) {
			__transfer(position, x.begin(), x.end());
		}
	}

	// 将 i 所指的元素接合于 position 之前
	template <class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list&, iterator i) {
		iterator j = i;
		++j;
		if (position == i || position == j)	return;
		__transfer(position, i, j);
	}

	// 将 [first, last)内的所有元素接合于 position 之前
	template <class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list& x, iterator first, iterator last) {
		if (first != last)
			__transfer(position, first, last);
	}

	// 将数值为 value 的所有元素移除
	template <class T, class Alloc>
	void list<T, Alloc>::remove(const T& value) {
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator next = first;
			++next;
			if (*first == value)	erase(first);
			first = next;
		}
	}

	// 将使一元操作 pred 为真的所有元素移除
	template <class T, class Alloc>
	template <class Predicate>
	void list<T, Alloc>::remove_if(Predicate pred) {
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator next = first;
			++next;
			if (pred(*first))	erase(first);
			first = next;
		}
	}

	// 移除数值相同的连续元素
	template <class T, class Alloc>
	void list<T, Alloc>::unique() {
		iterator first = begin();
		iterator last = end();
		if (first == last)	return;
		iterator next = first;
		while (++next != last) {
			if (*first == *next)
				erase(next);
			else
				first = next;
			next = first;
		}
	}

	// 与 x 合并
	template <class T, class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x) {
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1) {
				iterator next = first2;
				__transfer(first1, first2, ++next);
				first2 = next;
			}
			else
				++first1;
		}
		if (first2 != last2)	//还有剩余元素
			__transfer(last1, first2, last2);
	}

	// 将 list 按升序排列
	template <class T, class Alloc>
	void list<T, Alloc>::sort() {
		if (node->next == node || (link_type)node->next->next == node)	return;
		list<T, Alloc> carry;
		list<T, Alloc> counter[64];
		int fill = 0;
		while (!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()) {
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)	++fill;
		}
		for (int i = 1; i < fill; ++i) {
			counter[i].merge(counter[i - 1]);
		}
		swap(counter[fill - 1]);
	}

	// 将 list 逆转
	template <class T, class Alloc>
	void list<T, Alloc>::reverse() {
		if (node->next == node || (link_type)node->next->next == node)	return;
		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			__transfer(begin(), old, first);
		}
	}

	// __create_node 函数
	template <class T, class Alloc>
	typename list<T, Alloc>::link_type 
		list<T, Alloc>::__create_node(const T& x = T()) {
		link_type p = data_allocate::allocate();
		try {
			data_allocate::construct(p, __list_node<T>(x));
		}
		catch (...) {
			data_allocate::deallocate(p);
		}
		return p;
	}

	// __destroy_node 函数
	template <class T, class Alloc>
	void list<T, Alloc>::__destroy_node(link_type p) {
		MyTinySTL::destroy(&p->data);
		data_allocate::deallocate(p);
	}

	// __initialize 函数
	template <class T, class Alloc>
	void list<T, Alloc>::__initialize() {
		node = __create_node();
		node->prev = node->next = node;
	}

	// __fill_assign 函数
	template <class T, class Alloc>
	void list<T, Alloc>::__fill_assign(size_type n, const T& value) {
		iterator i = begin();
		for (; i != end() && n > 0; ++i, --n) {
			*i = value;
		}
		if (n > 0)	//如果还没有初始化完成
			insert(end(), n, value);
		else
			erase(i, end());
	}

	// __assign_dispatch 函数
	template <class T, class Alloc>
	template <class Integer>
	void list<T, Alloc>::__assign_dispatch(Integer n, Integer value, __true_type) {
		__fill_assign((size_type)n, (T)value);
	}

	template <class T, class Alloc>
	template <class InputIterator>
	void list<T, Alloc>::__assign_dispatch(InputIterator first2, InputIterator last2,
		__false_type) {
		iterator first1 = begin();
		iterator last1 = end();
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			*first1 = *first2;
		}
		if (first2 == last2)	//如果有多余元素
			erase(first1, last1);
		else
			insert(last1, first2, last2);
	}

	// __fill_insert 函数
	template <class T, class Alloc>
	void list<T, Alloc>::__fill_insert(iterator position, size_type n, const T& value) {
		for (; n > 0; --n) {
			insert(position, value);
		}
	}

	// __insert_dispatch 函数
	template <class T, class Alloc>
	template <class Integer>
	void list<T, Alloc>::__insert_dispatch(iterator position, Integer n, Integer value,
		__true_type) {
		__fill_insert(position, (size_type)n, (T)value);
	}

	template <class T, class Alloc>
	template <class InputIterator>
	void list<T, Alloc>::__insert_dispatch(iterator position, InputIterator first,
		InputIterator last, __false_type) {
		for (; first != last; ++first) {
			insert(position, *first);
		}
	}

	// __transfer 函数
	template <class T, class Alloc>
	void list<T, Alloc>::__transfer(iterator position, iterator first, iterator last) {
		// 将[first, last)内的所有元素移到 position 之前
		if (position != last) {
			(link_type)last.node->prev->next = (link_type)position.node;
			(link_type)first.node->prev->next = (link_type)last.node;
			(link_type)position.node->prev->next = (link_type)first.node;
			link_type tmp = (link_type)position.node->prev;
			(link_type)position.node->prev = (link_type)last.node->prev;
			(link_type)last.node->prev = (link_type)first.node->prev;
			(link_type)first.node->prev = tmp;
		}
	}
}
#endif // !LIST_H

