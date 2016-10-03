#ifndef MYTINYSTL_REVERSE_ITERATOR_H_
#define MYTINYSTL_REVERSE_ITERATOR_H_

// 这个头文件包含了一个模板类 reverse_iterator，代表反向迭代器

#include "iterator.h"

namespace mystl {

	// 模板类 : reverse_iterator
	// 反向迭代器 : 使前进为后退，后退为前进
	template <class Iterator>
	class reverse_iterator {
	private:
		Iterator current;	//记录对应的正向迭代器

	public:
		// 反向迭代器的五种相应性别
		typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename iterator_traits<Iterator>::value_type	value_type;
		typedef typename iterator_traits<Iterator>::difference_type	difference_type;
		typedef typename iterator_traits<Iterator>::pointer	pointer;
		typedef typename iterator_traits<Iterator>::reference	reference;

		typedef Iterator iterator_type;				//代表正向迭代器
		typedef reverse_iterator<Iterator> self;	//代表反向迭代器

	public:
		// 构造函数
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type x) :current(x) {}
		reverse_iterator(const self& x) :current(x.current) {}

		// 迭代器相关操作
		iterator_type base() const {	//取出对应的正向迭代器
			return current;
		}

		// 重载操作符
		reference operator*() const {	//实际对应正向迭代器的前一个位置
			auto tmp = current;
			return *--tmp;
		}
		pointer operator->() const {
			return &(operator*());
		}

		// 前进(++)变为后退(--)
		self& operator++() {
			--current;
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			--current;
			return tmp;
		}
		//后退(--)变为前进(++)
		self& operator--() {
			++current;
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			++current;
			return tmp;
		}

		self operator+(difference_type n) const {
			return static_cast<self>(current - n);
		}
		self& operator+=(difference_type n) {
			current -= n;
			return *this;
		}
		self operator-(difference_type n) const {
			return static_cast<self>(current + n);
		}
		self& operator-=(difference_type n) {
			current += n;
			return *this;
		}

		// 以下第一个operator* 和 operator+ 会调用本类的 operator* 和 operator+
		reference operator[](difference_type n) const {
			return *(*this + n);
		}
	};

	// 重载比较操作符
	template <class Iterator>
	inline bool operator==(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return lhs.base() == rhs.base();
	}

	template <class Iterator>
	inline bool operator<(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return rhs.base() < lhs.base();
	}

	template <class Iterator>
	inline bool operator!=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return !(lhs == rhs);
	}

	template <class Iterator>
	inline bool operator>(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return rhs < lhs;
	}

	template <class Iterator>
	inline bool operator<=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return !(rhs < lhs);
	}

	template <class Iterator>
	inline bool operator>=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return !(lhs < rhs);
	}
}
#endif // !MYTINYSTL_REVERSE_ITERATOR_H_

