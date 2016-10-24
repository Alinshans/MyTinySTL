#ifndef MYTINYSTL_STACK_H_
#define MYTINYSTL_STACK_H_

// 这个头文件包含了一个模板类 stack
// stack 是一个后进先出的数据结构 

#include "deque.h"	

namespace mystl {

	// 模板类 stack
	// 参数一代表数据类型，参数局代表容器类型，缺省使用 deque 作为底层容器
	template <class T, class Sequence = mystl::deque<T>>
	class stack {
	public:
		// 使用底层容器的型别
		typedef typename Sequence::value_type	value_type;
		typedef typename Sequence::size_type	size_type;
		typedef typename Sequence::reference	reference;
		typedef typename Sequence::const_reference	const_reference;
		
	private:
		Sequence c_;	//底层容器

	public:
		// 构造、复制、移动函数
		stack() :c_() {}
		stack(size_type n, const T& value) :c_(n, value) {}
		explicit stack(size_type n) :c_(n) {}
		template <class InputIterator>
		stack(InputIterator first, InputIterator last) : c_(first, last) {}

		explicit stack(const Sequence& s) :c_(s) {}
		explicit stack(Sequence&& s) :c_(std::move(s)) {}

		stack(const stack& rhs) :c_(rhs.c_) {}
		stack(stack&& rhs) :c_(std::move(rhs.c_)) {}

		stack& operator=(const stack& rhs) { c_ = rhs.c_; return *this; }
		stack& operator=(stack&& rhs) { c = std::move(rhs.c_); return *this; }

		// 以下操作使用底层容器的操作
		bool empty() const { return c_.empty(); }
		size_type size() const { return c_.size(); }

		reference top() { return c_.back(); }
		const_reference top() const { return c_.back(); }
		void push(const T& x) { c_.push_back(x); }
		void pop() { c_.pop_back(); } 

		void clear() { while (!empty())	pop(); }
		void swap(stack& rhs) { mystl::swap(c_, rhs.c_); }

	public:
		friend bool operator==(const stack& lhs, const stack& rhs) {
			return lhs.c_ == rhs.c_;
		}
		friend bool operator<(const stack& lhs, const stack& rhs) {
			return lhs.c_ < rhs.c_;
		}
	};

	// 重载比较操作符
	template <class T, class Sequence>
	bool operator!=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
		return !(lhs == rhs);
	}

	template <class T, class Sequence>
	bool operator>(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
		return rhs < lhs;
	}

	template <class T, class Sequence>
	bool operator<=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
		return !(rhs < lhs);
	}

	template <class T, class Sequence>
	bool operator>=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
		return !(lhs < rhs);
	}
	
	// 重载 mystl 的 swap
	template <class T, class Sequence>
	void swap(stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) {
		lhs.swap(rhs);
	}
}
#endif // !MYTINYSTL_STACK_H_

