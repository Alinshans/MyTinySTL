#ifndef MYTINYSTL_TEMPBUF_H_
#define MYTINYSTL_TEMPBUF_H_

// 这个头文件包含一些全局函数和一个模板类，用于临时缓冲区的申请与释放

#include <cstdlib>
#include <cstddef>	

#include "type_traits.h"
#include "uninitialized.h"

namespace mystl {

	// 函数模板 get_temporary_buffer，获取一个临时缓冲区
	// 版本1：接受一个大小
	// 版本2：接受一个大小与一个指针
	// 都调用转交函数 __get_temporary_buffer 处理
	template <class T>
	pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
		return __get_temporary_buffer(len, static_cast<T*>(0));
	}

	template <class T>
	pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*) {
		return __get_temporary_buffer(len, static_cast<T*>(0));
	}

	// 转交函数 __get_temporary_buffer
	// 
	template <class T>
	pair<T*, ptrdiff_t> __get_temporary_buffer(ptrdiff_t len, T*) {
		if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
			len = INT_MAX / sizeof(T);
		while (len > 0) {	//申请大小为 len 的缓冲区
			T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
			if (tmp != 0)
				return pair<T*, ptrdiff_t>(tmp, len);	//申请成功就返回
			len /= 2;	//申请失败时减少 len 的大小
		}
		return pair<T*, ptrdiff_t>((T*)0, 0);
	}

	// 释放指针指向的空间
	template <class T>
	void return_temporary_buffer(T* ptr) {
		free(ptr);
	}

	// 模板类：temporary_buffer
	// 可以进行临时缓冲区的申请与释放
	template <class ForwardIterator, class T>
	class temporary_buffer {
	private:
		ptrdiff_t original_len;	//缓冲区申请的大小
		ptrdiff_t len;	//缓冲区实际的大小
		T*	buffer;	//指向缓冲区

	public:
		// 构造、析构函数
		temporary_buffer(ForwardIterator first, ForwardIterator last);

		~temporary_buffer() {
			mystl::destroy(buffer, buffer + len);
			free(buffer);
		}

		ptrdiff_t size() const { return len; }
		ptrdiff_t requested_size() const { return original_len; }
		T* begin() { return buffer; }
		T* end() { return buffer + len; }

	private:
		void __allocate_buffer();
		void __initialize_buffer(const T&, __true_type) {}
		void __initialize_buffer(const T& value, __false_type) {
			mystl::uninitialized_fill_n(buffer, len, value);
		}

	private:
		// 禁用复制构造函数和赋值操作符
		temporary_buffer(const temporary_buffer&) {}
		void operator=(const temporary_buffer&) {}
	};

	// 构造函数
	template <class ForwardIterator, class T>
	temporary_buffer<ForwardIterator, T>::
		temporary_buffer(ForwardIterator first, ForwardIterator last) {
		typedef typename __type_traits<T>::has_trivial_default_constructor Trivial;
		try {
			len = distance(first, last);
			__allocate_buffer();
			if (len > 0) {
				__initialize_buffer(*first, Trivial());
			}
		}
		catch (...) {
			free(buffer);
			buffer = 0;
			len = 0;
		}
	}

	// 成员函数 __allocate_buffer，分配换成区空间
	template <class ForwardIterator, class T>
	void temporary_buffer<ForwardIterator, T>::__allocate_buffer() {	
		original_len = len;
		if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
			len = INT_MAX / sizeof(T);
		while (len > 0) {	//申请空间
			buffer = static_cast<T*>(malloc(len * sizeof(T)));
			if (buffer != 0)
				break;
			len /= 2;	//申请失败时减少空间大小
		}
	}
}
#endif // !MYTINYSTL_TEMPBUF_H_
