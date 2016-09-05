#ifndef DEQUE_H
#define DEQUE_H

#include "iterator.h"
#include "reverse_iterator.h"
#include "memory.h"

namespace MyTinySTL {

	// 这是一个全局函数，决定缓冲区的大小
	inline size_t __deque_buf_size(size_t n, size_t sz) {
		return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
	}

	// deque 的迭代器设计
	template <class T, class Ref, class Ptr, size_t BufSiz>
	struct __deque_iterator : public iterator<random_access_iterator_tag, T> {
		typedef __deque_iterator<T, T&, T*>	iterator;
		typedef __deque_iterator<T, const T&, const T*>	const_iterator;
		static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }

		typedef T	value_type;
		typedef Ptr	pointer;
		typedef Ref	reference;
		typedef size_t	size_type;
		typedef T** map_pointer;

		typedef __deque_iterator self;

		// 保持与容器的联结
		T*	cur;	//指向缓冲区当前元素
		T*	first;	//指向缓冲区的头
		T*	last;	//指向缓冲区的尾
		map_pointer node;	

		// 构造函数
		__deque_iterator() :cur(0), first(0), last(0), node(0) {}
		__deque_iterator(T* x, map_pointer y)
			:cur(x), first(*y), last(*y + buffer_size()), node(y) {}
		__deque_iterator(const iterator& x)
			:cur(x.cur), first(x.first), last(x.last), node(x.node) {}

		// 这个函数用于跳出一个缓冲区
		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		// 重载运算符
		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }
		difference_type operator-(const self& x) const {
			return difference_type(buffer_size()) * (node - x.node - 1) +
				(cur - first) + (x.last - x.cur);
		}

		self& operator++() {
			++cur;
			if (cur == last) {	//如果到达缓冲区的尾
				set_node(node + 1);	//跳到下一个缓冲区
				cur = first;
			}
			return *this;
		}

		self& operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}

		self& operator--() {
			if (cur == first) {	//如果到达缓冲区的头
				set_node(node - 1);	//跳到前一个缓冲区
				cur = last;
			}
			--cur;
			return *this;
		}

		self& operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}

		self& operator+=(difference_type n) {
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				// 增进后还在同一个缓冲区
				cur += n;
			else {
				difference_type node_offset = 
					offset > 0 ? offset / difference_type(buffer_size()) 
					: -difference_type((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = first + (offset - node_offset*difference_type(buffer_size()));
			}
			return *this;
		}

		self& operator+(difference_type n) const {
			self tmp = *this;
			return tmp += n;
		}

		self& operator-=(difference_type n) const {
			return *this += -n;
		}

		self& operator-(difference_type n) const {
			self tmp = *this;
			return tmp -= n;
		}

		reference operator[](difference_type n) const {
			return *(*this + n);
		}

		bool operator==(const self& x) const {
			return cur == x.cur;
		}
		 
		bool operator!=(const self& x) const {
			return !(*this == x);
		}

		bool operator<(const self& x) const {
			return node == x.node ? (cur < x.cur) : (node < x.node);
		}

		bool operator>(const self& x) const {
			return x < *this;
		}

		bool operator<=(const self& x) const {
			return !(x < *this);
		}

		bool operator>=(const self& x) const {
			return !(*this < x);
		}
	};

	// deque:双端队列
	template <class T, class Alloc = alloc, size_t BufSiz = 0>
	class deque {
	public:
		// deque 的嵌套型别定义
		typedef T	value_type;
		typedef Alloc	allocate_type;
		typedef	value_type*	pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&	reference;
		typedef	const value_type&	const_reference;
		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;

		typedef __deque_iterator<T, T&, T*, BufSiz>	iterator;
		typedef __deque_iterator<T, const T&, const T*, BufSiz>	const_iterator;
		typedef reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef reverse_iterator<iterator>	reverse_iterator;

	public:
		typedef	allocator<T, Alloc>	data_allocator;	//元素的配置器
		typedef allocator<T*, Alloc>	map_allocator;	//缓冲区的配置器

	protected:
		typedef pointer*	map_pointer;
		iterator start;		//指向第一个节点
		iterator finish;	//指向最后一个结点
		map_pointer map;	//指向一块 map 区域，map 中的每个元素都是一个指针，指向一个缓冲区
		size_type map_size;	//map 内指针的数目

	public:
		// 构造函数
		deque() {}
		deque(size_type n, const T& value) {}
		explicit deque(size_type n) {}
		template <InputIterator>
		deque(InputIterator first, InputIterator last);

		// 复制构造函数
		deque(const deque& x);

		// 赋值操作符
		deque& operator=(const deque& x);

		// 析构函数
		~deque();

	public:
		// 迭代器相关操作
		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		iterator end() { return finish; }
		const_iterator end() const { return finish; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return reverse_iterator(begin()); }

		// 容量相关操作
		bool empty() { return begin() == end(); }
		size_type size() const { return finish - start; }
		size_type max_size() const { return size_type(-1); }

		// 访问元素相关操作
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }
		reference operator[](size_type n) { return start[difference_type(n)]; }
		const_reference operator[](size_type n) const { return start[difference_type(n)]; }
		reference at(size_type n) { return (*this)[n]; }
		const_reference at(size_type n) const { return (*this)[n]; }

		// 调整容器相关操作
		void assign(size_type n, const T& value);
		void assign(size_type n);
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		iterator insert(iterator position, const T& x);
		iterator insert(iterator position) { return insert(position, T()); }
		void insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		iterator clear();
		void push_back(const T& x);
		void push_front(const T& x);
		void pop_back();
		void pop_front();
		void resize(size_type n, const T& x);
		void resize(size_type n) { resize(n, T()); }
		void swap(const deque& x);

		// 配置器相关操作
		allocate_type get_allocate() { return allocate_type(); }
	};
}
#endif // !DEQUE_H

