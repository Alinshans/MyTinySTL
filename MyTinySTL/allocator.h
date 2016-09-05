#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "alloc.h"	// for allocate/deallocate
#include "construct.h"	// for construct/destroy

namespace MyTinySTL {

	//空间配置器
	template <class T, class Alloc>
	class allocator {

	public:
		typedef T		value_type;
		typedef T*		pointer;
		typedef const T*	const_pointer;
		typedef T&		reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

	public:
		static T* allocate();
		static T* allocate(size_t n);
		static void deallocate(T* ptr);
		static void deallocate(T* ptr, size_t n);

		static void construct(T* ptr);
		static void construct(T* ptr, const T& x);
		static void destroy(T* ptr);
		static void destroy(T* first, T* last);
	};

	template <class T, class Alloc>
	T* allocator<T, Alloc>::allocate() {
		return static_cast<T*>(Alloc::allocate(sizeof(T)));
	}

	template <class T, class Alloc>
	T* allocator<T, Alloc>::allocate(size_t n) {
		if (n == 0)	return 0;
		return static_cast<T*>(Alloc::allocate(n * sizeof(T)));
	}

	template <class T, class Alloc>
	void allocator<T, Alloc>::deallocate(T* ptr) {
		if (ptr == 0)	return;
		Alloc::deallocate(ptr, sizeof(T));
	}

	template <class T, class Alloc>
	void allocator<T, Alloc>::deallocate(T* ptr, size_t n) {
		if (ptr == 0)	return;
		Alloc::deallocate(ptr, n * sizeof(T));
	}

	template <class T, class Alloc>
	void allocator<T, Alloc>::construct(T* ptr) {
		MyTinySTL::construct(ptr);
	}

	template <class T, class Alloc>
	void allocator<T, Alloc>::construct(T* ptr, const T& x) {
		MyTinySTL::construct(ptr, x);
	}

	template <class T, class Alloc>
	void allocator<T, Alloc>::destroy(T* ptr) {
		MyTinySTL::destroy(ptr);
	}

	template <class T, class Alloc>
	void allocator<T, Alloc>::destroy(T* first, T* last) {
		MyTinySTL::destroy(first, last);
	}
}
#endif // !ALLOCATOR_H

