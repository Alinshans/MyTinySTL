#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <new>

#include "type_traits.h"

namespace MyTinySTL {
	
	template <class T1>
	inline void construct(T1 *p) {
		new ((void*)p) T1();
	}

	template <class T1, class T2>
	inline void construct(T1 *p, const T2 &value) {
		new ((void*)p) T1(value);
	}

	template <class T>
	inline void destroy(T * pointer) {
		pointer->~T()
	}

	template <class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		_destroy(first, last, value_type(first));
	}

	template <class ForwardIterator, class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*) {
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	template <class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
		for (; first != last; ++first)
			destroy(&*first);
	}

	template <class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

	//针对char* 和 wchar_t* 的特化版本
	template <>
	inline void destroy(char*, char*) {}

	template <>
	inline void destroy(wchar_t*, wchar_t*) {}
}
#endif // !CONSTRUCT_H

