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

}
#endif // !DEQUE_H

