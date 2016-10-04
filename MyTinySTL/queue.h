#ifndef MYTINYSTL_QUEUE_H_
#define MYTINYSTL_QUEUE_H_

// 这个头文件包含了两个模板类 queue 和 priority_queue
// queue: 队列，一种先进先出的数据结构
// priority_queue: 优先队列，具有权值的队列，权值大的优先出队

#include "deque.h"
#include "heap_algo.h"
#include "vector.h"

namespace mystl {

	// 模板类 queue
	// 参数一代表数据类型，参数二代表容器类型，缺省使用 deque 作为底层容器
	// 用法与 STL queue 类似
	template <class T, class Sequence = deque<T>>
	class queue {
	public:
		// 使用底层容器的型别
		typedef typename Sequence::value_type	value_type;
		typedef typename Sequence::size_type	size_type;
		typedef typename Sequence::reference	reference;
		typedef typename Sequence::const_reference	const_reference;

	private:
		Sequence c_;	//用底层容器来表现 queue

	public:
		queue() :c_() {}
		queue(const Sequence& s) :c_(s) {}
		queue(size_type n, const T& value) :c_(n, value) {}
		explicit queue(size_type n) :c_(n) {}
		template <class InputIterator>
		queue(InputIterator first, InputIterator last) : c_(first, last) {}

		// 以下操作使用底层容器的操作
		bool empty() const { return c_.empty(); }
		size_type size() const { return c_.size(); }

		reference front() { return c_.front(); }
		const_reference front() const { return c_.front(); }
		reference back() { return c_.back(); }
		const_reference back() const { return c_.back(); }

		void push(const T& x) { c_.push_back(x); }
		void pop() { c_.pop_front(); }

		void clear() { while (!empty()) pop(); }
		void swap(queue& rhs) { mystl::swap(c_, rhs.c_); }

	public:
		friend bool operator==(const queue& lhs, const queue& rhs) {
			return lhs.c_ == rhs.c_;
		}
		friend bool operator<(const queue& lhs, const queue& rhs) {
			return lhs.c_ < rhs.c_;
		}
	};

	// 重载比较操作符
	template <class T, class Sequence>
	bool operator!=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
		return !(lhs == rhs);
	}

	template <class T, class Sequence>
	bool operator>(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
		return rhs < lhs;
	}

	template <class T, class Sequence>
	bool operator<=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
		return !(rhs < lhs);
	}

	template <class T, class Sequence>
	bool operator>=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
		return !(lhs < rhs);
	}

	// 重载 mystl 的 swap
	template <class T, class Sequence>
	void swap(queue<T, Sequence>& lhs, queue<T, Sequence>& rhs) {
		lhs.swap(rhs);
	}

	// 模板类 priority_queue
	// 参数一代表数据类型，参数二代表容器类型，缺省使用 vector 作为底层容器
	// 参数三代表比较权值的方式，缺省使用 mystl 的 less 作为比较方式
	// 用法与 STL priority_queue 类似
	template <class T, class Sequence = vector<T>,
		class Compare = less<T>>
	class priority_queue {
	public:
		// 使用底层容器的型别
		typedef typename Sequence::value_type	value_type;
		typedef typename Sequence::size_type	size_type;
		typedef typename Sequence::reference	reference;
		typedef typename Sequence::const_reference	const_reference;

	private:
		Sequence c_;	//底层容器
		Compare comp_;	//权值比较大小的标准

	public:
		// 构造函数
		priority_queue() :c_() {}
		priority_queue(const Sequence& s) :c_(s) { make_heap(c_.begin(), c_.end(), comp_); }
		priority_queue(size_type n, const T& value) 
			:c_(n, value) { make_heap(c_.begin(), c_.end(), comp_); }
		explicit priority_queue(size_type n) 
			:c_(n) { make_heap(c_.begin(), c_.end(), comp_); }
		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last)
			: c_(first, last) { make_heap(c_.begin(), c_.end(), comp_); }

	public:
		// 以下操作使用底层容器的操作
		bool empty() const { return c_.empty(); }
		size_type size() const { return c_.size(); }

		const_reference top() const { return c_.front(); }
		void push(const T& x);
		void pop();

		void clear() { while (!empty())	pop(); }
	};

	// 插入一个元素
	template <class T, class Sequence, class Compare>
	void priority_queue<T, Sequence, Compare>::push(const T& x) {
		try {
			c_.push_back(x);
			push_heap(c_.begin(), c_.end(), comp_);
		}
		catch (...) {
			c_.clear();
		}
	}

	// 弹出队首元素
	template <class T, class Sequence, class Compare>
	void priority_queue<T, Sequence, Compare>::pop() {
		try {
			pop_heap(c_.begin(), c_.end(), comp_);
			c_.pop_back();
		}
		catch (...) {
			c_.clear();
		}
	}
}
#endif // !MYTINYSTL_QUEUE_H_

