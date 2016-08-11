#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H

#ifndef USE_CSTRING
#define	USE_CSTRING
#include <cstring>	//for memmove
#endif // !USE_CSTRING

#include "type_traits.h"
#include "construct.h"
#include "iterator.h"
#include "algobase.h"

//三个全局函数uninitialized_copy(), uninitialized_fill(), uninitialized_fill_n()
//分别对应高层函数 copy(), fill(), fill_n()
namespace MyTinySTL {

	/********************* uninitialized_copy *************************/
	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator 
		uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return __uninitialized_copy(first, last, result, value_type(result));
	}

	template <class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator
		__uninitialized_copy(InputIterator first, InputIterator last,
			ForwardIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type POD_type;
		return __uninitialized_copy_aux(first, last, result, POD_type());
	}

	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator
		__uninitialized_copy_aux(InputIterator first, InputIterator last,
			ForwardIterator result, __true_type) {
		return	copy(first, last, result);	//调用高层函数 copy
	}

	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator
		__uninitialized_copy_aux(InputIterator first, InputIterator last,
			ForwardIterator result, __false_type) {
		ForwardIterator cur;
		for (; first != last; ++first, ++cur) {
			construct(&*cur, *first);	//一个一个构造
		}
		return cur;
	}

	//针对 char* 和 wchar_t* 的特化版本
	inline char* uninitialized_copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}

	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, 
		wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}

	/********************* uninitialized_fill *************************/
	template <class ForwardIterator, class T>
	inline void 
		uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
		__uninitialized_fill(first, last, x, value_type(first));
	}

	template <class ForwardIterator, class T, class Ty>
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& x, Ty*) {
		typedef typename __type_traits<Ty>::is_POD_type POD_type;
		__uninitialized_fill_aux(first, last, x, POD_type());
	}

	template <class ForwardIterator, class T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& x, __true_type) {
		fill(first, last, x);	//调用高层函数 fill
	}

	template <class ForwardIterator, class T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& x, __false_type) {
		ForwardIterator cur = first;
		for (; cur != last; ++cur) {
			construct(&*cur, x);	//一个一个构造
		}
	}

	/************************ uninitialized_fill_n ************************/
	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}

	template <class ForwardIterator, class Size, class T, class Ty>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n,
		const T& x, Ty*) {
		typedef typename __type_traits<Ty>::is_POD_type POD_type;
		return __uninitialized_fill_n_aux(first, n, x, POD_type());
	}

	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& x, __true_type) {
		return fill_n(first, n, x);	//调用高层函数 fill_n
	}

	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& x, __false_type) {
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur) {
			construct(&*cur, x);
		}
		return cur;
	}
}
#endif // !UNINITIALIZED_H

