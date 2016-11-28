#ifndef MYTINYSTL_MEMORY_H_
#define MYTINYSTL_MEMORY_H_

// 这个头文件负责更高级的动态内存管理
// 包含一些基本算法、空间配置器、未初始化的储存空间管理，以及一个模板类 auto_ptr

#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "tempbuf.h"
#include "uninitialized.h"

namespace mystl {
    
// 模板类: auto_ptr
// 一个具有严格对象所有权的小型智能指针
template <class T>
class auto_ptr {
  public:
    typedef T    elem_type;

  private:
    T* m_ptr;  // 实际指针

  public:
    // 构造、复制、析构函数
    explicit auto_ptr(T* p = nullptr) :m_ptr(p) {}
    auto_ptr(auto_ptr& rhs) :m_ptr(rhs.release()) {}
    template <class U>
    auto_ptr(auto_ptr<U>& rhs) : m_ptr(rhs.release()) {}

    auto_ptr& operator=(auto_ptr& rhs) {
        if (this != &rhs) {
            delete m_ptr;
            m_ptr = rhs.release();
        }
        return *this;
    }
    template <class U>
    auto_ptr& operator=(auto_ptr<U>& rhs) {
        if (this->get() != rhs.get()) {
            delete m_ptr;
            m_ptr = rhs.release();
        }
        return *this;
    }

    ~auto_ptr() { delete m_ptr; }    

  public:
    // 重载 operator* 和 operator->
    T& operator*()  const { return *m_ptr; }
    T* operator->() const { return m_ptr; }

    // 获得指针
    T* get() const { return m_ptr; }

    // 释放指针
    T* release() {
        T* tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }

    // 重置指针
    void reset(T* p = nullptr) {
        if (m_ptr != p) {
            delete m_ptr;
            m_ptr = p;
        }
    }
};

} // namespace mystl
#endif // !MYTINYSTL_MEMORY_H_

