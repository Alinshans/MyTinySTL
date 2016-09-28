#ifndef MYTINYSTL_UNINITIALIZED_H_
#define MYTINYSTL_UNINITIALIZED_H_

// 这个头文件包含三个全局函数 uninitialized_copy, uninitialized_fill, uninitialized_fill_n
// 用于未初始化时的 copy, fill, fill_n
// 分别与高层算法函数中的 copy, fill, fill_n 对应

#include <cstring>	//for memmove

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"

namespace MyTinySTL {

	// uninitialized_copy
	// 把[first, last)上的内容复制到以 result 为起始处的空间，返回一个迭代器指向复制结束的尾部
	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator 
		uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return __uninitialized_copy(first, last, result, value_type(result));
	}

	template <class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator
		__uninitialized_copy(InputIterator first, InputIterator last,
			ForwardIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type PODType;
		return __uninitialized_copy_aux(first, last, result, PODType());
	}

	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator
		__uninitialized_copy_aux(InputIterator first, InputIterator last,
			ForwardIterator result, __true_type) {
		return	MyTinySTL::copy(first, last, result);	//如果是 __true_type，调用高层函数 copy
	}

	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator
		__uninitialized_copy_aux(InputIterator first, InputIterator last,
			ForwardIterator result, __false_type) {
		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur) {
			MyTinySTL::construct(&*cur, *first);
		}
		return cur;
	}

	//针对 char* 和 wchar_t* 的特化版本
	inline char* uninitialized_copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}

	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}

	// uninitialized_fill
	// 在[first, last)区间内填充元素值，无返回值
	template <class ForwardIterator, class T>
	inline void 
		uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
		__uninitialized_fill(first, last, x, value_type(first));
	}

	template <class ForwardIterator, class T, class Ty>
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& x, Ty*) {
		typedef typename __type_traits<Ty>::is_POD_type PODType;
		__uninitialized_fill_aux(first, last, x, PODType());
	}

	template <class ForwardIterator, class T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& x, __true_type) {
		MyTinySTL::fill(first, last, x);	//如果是 __true_type，调用高层函数 fill
	}

	template <class ForwardIterator, class T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& x, __false_type) {
		ForwardIterator cur = first;
		for (; cur != last; ++cur) {
			MyTinySTL::construct(&*cur, x);
		}
	}

	// uninitialized_fill_n
	// 从 first 位置开始，填充 n 个元素值，返回一个迭代器指向填充结束的位置
	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}

	template <class ForwardIterator, class Size, class T, class Ty>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, Ty*) {
		typedef typename __type_traits<Ty>::is_POD_type PODType;
		return __uninitialized_fill_n_aux(first, n, x, PODType());
	}

	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& x, __true_type) {
		return MyTinySTL::fill_n(first, n, x);	//如果是 __true_type，调用高层函数 fill_n
	}

	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& x, __false_type) {
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur) {
			MyTinySTL::construct(&*cur, x);
		}
		return cur;
	}
}
#endif // !MYTINYSTL_UNINITIALIZED_H_
