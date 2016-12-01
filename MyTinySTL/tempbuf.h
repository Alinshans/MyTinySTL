#ifndef MYTINYSTL_TEMPBUF_H_
#define MYTINYSTL_TEMPBUF_H_

// 这个头文件用于临时缓冲区的申请与释放，包含一个模板类和一些全局函数

#include <cstddef>
#include <cstdlib>
#include <climits>

#include "type_traits.h"
#include "uninitialized.h"

namespace mystl {

// 函数模板 get_temporary_buffer，获取一个临时缓冲区
// 版本1：接受一个数值
// 版本2：接受一个数值和一个指针
// 都转交给 __get_temporary_buffer 函数处理
template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
    return __get_temporary_buffer(len, static_cast<T*>(0));
}

template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*) {
    return __get_temporary_buffer(len, static_cast<T*>(0));
}

// __get_temporary_buffer
template <class T>
pair<T*, ptrdiff_t> __get_temporary_buffer(ptrdiff_t len, T*) {
    if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
        len = INT_MAX / sizeof(T);
    while (len > 0) {
        T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
        if (tmp)
            return pair<T*, ptrdiff_t>(tmp, len);  // 申请成功就返回
        len /= 2;                                  // 申请失败时减少 len 的大小
    }
    return pair<T*, ptrdiff_t>((static_cast<T*>(0), static_cast<ptrdiff_t>(0)));
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
    ptrdiff_t original_len;  // 缓冲区申请的大小
    ptrdiff_t len;           // 缓冲区实际的大小
    T*        buffer;        // 指向缓冲区的指针

  public:
    // 构造、析构函数
    temporary_buffer(ForwardIterator first, ForwardIterator last);

    ~temporary_buffer() {
        mystl::destroy(buffer, buffer + len);
        free(buffer);
    }

  public:
    ptrdiff_t size()           const { return len; }
    ptrdiff_t requested_size() const { return original_len; }
    T*        begin()                { return buffer; }
    T*        end()                  { return buffer + len; }

  private:
    void __allocate_buffer();
    void __initialize_buffer(const T&, __true_type) {}
    void __initialize_buffer(const T& value, __false_type) { mystl::uninitialized_fill_n(buffer, len, value); }

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
        buffer = nullptr;
        len = 0;
    }
}

// __allocate_buffer 函数
template <class ForwardIterator, class T>
void temporary_buffer<ForwardIterator, T>::__allocate_buffer() {    
    original_len = len;
    if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
        len = INT_MAX / sizeof(T);
    while (len > 0) {
        buffer = static_cast<T*>(malloc(len * sizeof(T)));
        if (buffer)  break;
        len /= 2;    // 申请失败时减少申请空间大小
    }
}

} // namespace mystl
#endif // !MYTINYSTL_TEMPBUF_H_

