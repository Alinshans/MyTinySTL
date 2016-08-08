#ifndef ITERATOR_H
#define ITERATOR_H

#include <cstddef>	//for ptrdiff_t

namespace MyTinySTL {

	//五种迭代器类型
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	//input_iterator
	template <class T, class Distance>
	struct input_iterator {
		typedef input_iterator_tag			iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	//output_iterator
	template <class T, class Distance>
	struct output_iterator {
		typedef output_iterator_tag			iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	//forward_iterator
	template <class T, class Distance>
	struct forward_iterator {
		typedef forward_iterator_tag		iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	//bidirectional_iterator
	template <class T, class Distance>
	struct bidirectional_iterator {
		typedef bidirectional_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	//random_access_iterator
	template <class T, class Distance>
	struct random_access_iterator {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	//iterator
	template <class Category, class T, class Distance = ptrdiff_t , 
		class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	//萃取迭代器特性
	template <class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};

	//针对原生指针的特化版本
	template <class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template <class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef const T*					pointer;
		typedef const T&					reference;
	};

	//这个函数决定某个迭代器的 category
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//这个函数决定某个迭代器的 distance type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		difference_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//这个函数决定某个迭代器的 value type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
}
#endif // !ITERATOR_H

