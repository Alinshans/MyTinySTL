#ifndef MEMORY_H
#define MEMORY_H

#include "algobase.h"
#include "allocator.h"
#include "functional.h"
#include "tempbuf.h"
#include "uninitialized.h"

namespace MyTinySTL {
	
	template <class T>
	class auto_ptr {

	private:
		T* m_ptr;

	public:
		typedef T	elem_type;

		// 构造函数
		explicit auto_ptr(T* p = 0) :m_ptr(p) {}
		auto_ptr(auto_ptr& x) :m_ptr(x.release()) {}
		template <class T1>
		auto_ptr(auto_ptr<T1>& x) : m_ptr(x.release()) {}

		// 赋值操作符
		auto_ptr& operator=(auto_ptr& x) {
			if (&x != this) {
				delete m_ptr;
				m_ptr = x.release();
			}
			return *this;
		}

		template <class T1>
		auto_ptr& operator=(auto_ptr<T1>& x) {
			if (x.get() != this->get()) {
				delete m_ptr;
				m_ptr = x.release();
			}
			return *this;
		}

		// 析构函数
		~auto_ptr() { delete m_ptr; }

	public:
		T& operator*() const {
			return *m_ptr;
		}

		T* operator->() const {
			return m_ptr;
		}

		T* get() const {
			return m_ptr;
		}

		T* release() {
			T* tmp = m_ptr;
			m_ptr = 0;
			return tmp;
		}

		void reset(T* p = 0) {
			if (p != m_ptr) {
				delete m_ptr;
				m_ptr = p;
			}
		}
	};
}
#endif // !MEMORY_H

