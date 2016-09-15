#ifndef STACK_H
#define STACK_H

#include "deque.h"	

namespace MyTinySTL {

	// stack:是一个后进先出的数据结构
	// 默认使用 deque 作为底层容器
	template <class T, class Sequence = deque<T>>
	class stack {
	public:
		// 使用底层容器的型别
		typedef typename Sequence::value_type	value_type;
		typedef typename Sequence::size_type	size_type;
		typedef typename Sequence::reference	reference;
		typedef typename Sequence::const_reference	const_reference;
		
	protected:
		Sequence c;	//底层容器

	public:
		stack() :c() {}
		explicit stack(const Sequence& s) :c(s) {}
		explicit stack(size_type n, const T& value) :c(n, value) {}
		explicit stack(size_type n) :c(n) {}
		template <class InputIterator>
		stack(InputIterator first, InputIterator last) : c(first, last) {}

		// 以下操作使用底层容器的操作
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference top() { return c.back(); }
		const_reference top() const { return c.back(); }
		void push(const T& x) { c.push_back(x); }
		void pop() { c.pop_back(); } 

		void clear() { while (!empty())	pop(); }
		void swap(const stack& x) { MyTinySTL::swap(c, x.c); }

	public:
		friend bool operator==(const stack& x, const stack& y) {
			return x.c == y.c;
		}
		friend bool operator<(const stack& x, const stack& y) {
			return x.c < y.c;
		}
	};

	// 重载比较操作符
	template <class T, class Sequence>
	bool operator!=(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
		return !(x == y);
	}

	template <class T, class Sequence>
	bool operator>(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
		return y < x;
	}

	template <class T, class Sequence>
	bool operator<=(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
		return !(y < x);
	}

	template <class T, class Sequence>
	bool operator>=(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
		return !(x < y);
	}
	
	// 重载 MyTinySTL 的 swap
	template <class T, class Sequence>
	void swap(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
		x.swap(y);
	}
}
#endif // !STACK_H

