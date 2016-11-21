#ifndef MYTINYSTL_MEMORY_H_
#define MYTINYSTL_MEMORY_H_

// 这个头文件包含一些基本算法、内存的构造析构与分配释放、临时缓冲区的头文件，以及一个模板类 auto_ptr

#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace mystl {
    
    // 模板类: auto_ptr
    // 一个小型的智能指针
    template <class T>
    class auto_ptr {
    public:
        typedef T    elem_type;

    private:
        T* m_ptr;    //指针

    public:
        // 构造、复制、析构函数
        explicit auto_ptr(T* p = nullptr) :m_ptr(p) {}
        auto_ptr(auto_ptr& x) :m_ptr(x.release()) {}
        template <class T1>
        auto_ptr(auto_ptr<T1>& x) : m_ptr(x.release()) {}

        auto_ptr& operator=(auto_ptr& x) {
            if (this != &x) {
                delete m_ptr;
                m_ptr = x.release();
            }
            return *this;
        }

        template <class T1>
        auto_ptr& operator=(auto_ptr<T1>& x) {
            if (this->get() != x.get()) {
                delete m_ptr;
                m_ptr = x.release();
            }
            return *this;
        }

        ~auto_ptr() { delete m_ptr; }    

    public:
        // 重载 operator* 和 operator->
        T& operator*() const { return *m_ptr; }
        T* operator->() const { return m_ptr; }

        T* get() const { return m_ptr; }
        T* release() {
            T* tmp = m_ptr;
            m_ptr = nullptr;
            return tmp;
        }
        void reset(T* p = nullptr) {
            if (m_ptr != p) {
                delete m_ptr;
                m_ptr = p;
            }
        }
    };
}
#endif // !MYTINYSTL_MEMORY_H_

