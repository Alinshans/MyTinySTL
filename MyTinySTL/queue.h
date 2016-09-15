#ifndef QUEUE_H
#define QUEUE_H

#include "deque.h"
#include "vector.h"
#include "heap_algo.h"

namespace MyTinySTL {

	// queue: 是一种先进先出的数据结构
	// 默认使用 deque 作为底层容器
	template <class T, class Sequence = deque<T>>
	class queue {
	public:
		// 使用底层容器的型别
		typedef typename Sequence::value_type	value_type;
		typedef typename Sequence::size_type	size_type;
		typedef typename Sequence::reference	reference;
		typedef typename Sequence::const_reference	const_reference;

	protected:
		Sequence c;	//底层容器

	public:
		queue() :c() {}
		explicit queue(const Sequence& x) :c(x) {}
		explicit queue(size_type n, const T& value) :c(n, value) {}
		explicit queue(size_type n) :c(n) {}
		template <class InputIterator>
		queue(InputIterator first, InputIterator last) : c(first, last) {}

		// 以下操作使用底层容器的操作
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference front() { return c.front(); }
		const_reference front() const { return c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }
		void push(const T& x) { c.push_back(x); }
		void pop() { c.pop_front(); }

		void clear() { while (!empty()) pop(); }
		void swap(const queue& x) { MyTinySTL::swap(c, x.c); }

	public:
		friend bool operator==(const queue& x, const queue& y) {
			return x.c == y.c;
		}
		friend bool operator<(const queue& x, const queue& y) {
			return x.c < y.c;
		}
	};

	// 重载比较操作符
	template <class T, class Sequence>
	bool operator!=(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		return !(x == y);
	}

	template <class T, class Sequence>
	bool operator>(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		return y < x;
	}

	template <class T, class Sequence>
	bool operator<=(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		return !(y < x);
	}

	template <class T, class Sequence>
	bool operator>=(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		return !(x < y);
	}

	// 重载 MyTinySTL 的 swap
	template <class T, class Sequence>
	void swap(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		x.swap(y);
	}

	// priority_queue: 带有权值的 queue
	// 默认使用 vector 作为底层容器，使用 max-heap 排序
	template <class T, class Sequence = vector<T>,
		class Compare = less<typename Sequence::value_type>>
	class priority_queue {
	public:
		// 使用底层容器的型别
		typedef typename Sequence::value_type	value_type;
		typedef typename Sequence::size_type	size_type;
		typedef typename Sequence::reference	reference;
		typedef typename Sequence::const_reference	const_reference;

	protected:
		Sequence c;	//底层容器
		Compare comp;	//元素比较大小的标准

	public:
		// 构造函数
		priority_queue() :c() {}
		explicit priority_queue(const Sequence& s) :c(s) { make_heap(c.begin(), c.end(), comp); }
		explicit priority_queue(size_type n) :c(n) { make_heap(c.begin(), c.end(), comp); }
		explicit priority_queue(size_type n, const T& value) 
			:c(n, value) { make_heap(c.begin(), c.end(), comp); }
		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last)
			: c(first, last) { make_heap(c.begin(), c.end(), comp); }

	public:
		// 以下操作使用底层容器的操作
		bool empty()const { return c.empty(); }
		size_type size() const { return c.size(); }
		const_reference top() const { return c.front(); }
		void push(const T& x);
		void pop();

		void clear() { while (!empty())	pop(); }
	};

	// 插入函数
	template <class T, class Sequence, class Compare>
	void priority_queue<T, Sequence, Compare>::push(const T& x) {
		try {
			c.push_back(x);
			push_heap(c.begin(), c.end(), comp);
		}
		catch (...) {
			c.clear();
		}
	}

	template <class T, class Sequence, class Compare>
	void priority_queue<T, Sequence, Compare>::pop() {
		try {
			pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}
		catch (...) {
			c.clear();
		}
	}
}
#endif // !QUEUE_H

