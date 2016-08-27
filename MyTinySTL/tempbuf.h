#ifndef TEMPBUF_H
#define TEMPBUF_H

namespace MyTinySTL {

	template <class T>
	pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
		return __get_temporary_buffer(len, (T*)0);
	}

	template <class T>
	pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*) {
		return __get_temporary_buffer(len, (T*)0);
	}

	template <class T>
	pair<T*, ptrdiff_t> __get_temporary_buffer(ptrdiff_t len, T*) {
		if (len > ptrdiff_t(INT_MAX / sizeof(T)))
			len = INT_MAX / sizeof(T);
		while (len > 0) {	//申请大小为 len 的缓冲区
			T* tmp = (T*)malloc((size_t)len * sizeof(T));
			if (tmp != 0)
				return pair<T*, ptrdiff_t>(tmp, len);
			len /= 2;	//申请失败时减少 len 的大小
		}
		return pair<T*, ptrdiff_t>((T*)0, 0);
	}

	template <class T>
	void return_temporary_buffer(T* ptr) {
		free(ptr);
	}

	// temporary_buffer 临时缓冲区模板类
	template <class ForwardIterator, class T>
	class temporary_buffer {
	private:
		ptrdiff_t original_len;
		ptrdiff_t len;
		T*	buffer;

		void allocate_buffer(){	//配置缓冲区空间
			original_len = len;
			if (len > (ptrdiff_t)INT_MAX / sizeof(T))
				len = INT_MAX / sizeof(T);
			while (len > 0) {	//申请空间
				buffer = (T*)malloc(len * sizeof(T));
				if (buffer != 0)
					break;
				len /= 2;	//申请失败时减少空间大小
			}
		}

		void initialize_buffer(const T&, __true_type) {}
		void initialize_buffer(const T& value, __false_type) {
			uninitialized_fill_n(buffer, len, value);
		}

	public:
		// 构造函数
		temporary_buffer(ForwardIterator first, ForwardIterator last) {
			typedef typename __type_traits<T>::has_trivial_default_constructor trivial;
			try {
				len = distance(first, last);
				allocate_buffer();
				if (len > 0) {
					initialize_buffer(*first, trivial());
				}
			}
			catch (...) {
				free(buffer);
				buffer = 0;
				len = 0;
			}
		}

		// 析构函数
		~temporary_buffer() {
			destroy(buffer, buffer + len);
			free(buffer);
		}

		ptrdiff_t size() const { return len; }
		ptrdiff_t requested_size() const { return original_len; }
		T* begin() { return buffer; }
		T* end() { return buffer + len; }

	private:
		// 禁用复制构造函数和赋值操作符
		temporary_buffer(const temporary_buffer&) {}
		void operator=(const temporary_buffer&) {}
	};
}
#endif // !TEMPBUF_H
