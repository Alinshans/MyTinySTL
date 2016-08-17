#ifndef REVERSE_ITERATOR_H
#define REVERSE_ITERATOR_H

#include "iterator.h"

namespace MyTinySTL {

	//反向迭代器：使前进为后退，后退为前进
	template <class Iterator>
	class reverse_iterator {
		
	protected:
		Iterator current;	//记录对应的正向迭代器

	public:
		//反向迭代器的五种相应性别
		typedef typename iterator_traits<Iterator>::iterator_category
			iterator_category;
		typedef typename iterator_traits<Iterator>::value_type
			value_type;
		typedef typename iterator_traits<Iterator>::difference_type
			difference_type;
		typedef typename iterator_traits<Iterator>::pointer
			pointer;
		typedef typename iterator_traits<Iterator>::reference
			reference;

		typedef Iterator iterator_type;				//代表正向迭代器
		typedef reverse_iterator<Iterator> self;	//代表反向迭代器

	public:
		//构造函数
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type x) :current(x) {}
		reverse_iterator(const self& x) :current(x.current) {}

		iterator_type base() const {	//取出对应的正向迭代器
			return current;
		}
		reference operator*() const {	//实际对应正向迭代器的前一个位置
			Iterator tmp = current;
			return *--tmp;
		}
		pointer operator->() const {
			return &(operator*());
		}

		//前进(++)变为后退(--)
		self& operator++() {
			--current;
			return *this;
		}
		self& operator++(int) {
			self tmp = *this;
			--current;
			return tmp;
		}
		//后退(--)变为前进(++)
		self& operator--() {
			++current;
			return *this;
		}
		self& operator--(int) {
			self tmp = *this;
			++current;
			return tmp;
		}

		self operator+(difference_type n) const {
			return self(current - n);
		}
		self operator+=(difference_type n) const {
			current -= n;
			return *this;
		}
		self operator-(difference_type n) const {
			return self(current + n);
		}
		self operator-=(difference_type n) const {
			current += n;
			return *this;
		}

		//下面的第一个* 和 + 操作符都会调用本类的operator* 和 operator+
		reference operator[](difference_type n) const {
			return *(*this + n);
		}
	};
}
#endif // !REVERSE_ITERATOR_H
