#ifndef QUEUE_H
#define QUEUE_H

#include "deque.h"

namespace MyTinySTL {

	// queue: 是一种先进先出的数据结构
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

		// 全部操作使用底层容器的操作
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference front() { return c.front(); }
		const_reference front() const { return c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }
		void push(const T& x) { c.push_back(x); }
		void pop() { c.pop_front(); }

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
}
#endif // !QUEUE_H

