#ifndef MYTINYSTL_STACK_H_
#define MYTINYSTL_STACK_H_

// 这个头文件包含了一个模板类 stack
// stack : 栈

#include "deque.h"    

namespace mystl {

// 模板类 stack
// 参数一代表数据类型，参数二代表底层容器类型，缺省使用 deque 作为底层容器
template <class T, class Sequence = mystl::deque<T>>
class stack {
  public:
    // 使用底层容器的型别
    typedef typename Sequence::value_type         value_type;
    typedef typename Sequence::size_type          size_type;
    typedef typename Sequence::reference          reference;
    typedef typename Sequence::const_reference    const_reference;
        
  private:
    Sequence s_;  // 用底层容器表现 stack

  public:
    // 构造、复制、移动函数
    stack() :s_() {}
    explicit stack(size_type n) :s_(n) {}
    stack(size_type n, const value_type& value) :s_(n, value) {}
    template <class InputIterator>
    stack(InputIterator first, InputIterator last) : s_(first, last) {}

    explicit stack(const Sequence& s) :s_(s) {}
    explicit stack(Sequence&& s) :s_(std::move(s)) {}

    stack(const stack& rhs) :s_(rhs.s_) {}
    stack(stack&& rhs) :s_(std::move(rhs.s_)) {}

    stack& operator=(const stack& rhs) { s_ = rhs.s_; return *this; }
    stack& operator=(stack&& rhs)      { s_ = std::move(rhs.s_); return *this; }

    // 以下操作使用底层容器的操作
    bool            empty()                        const { return s_.empty(); }
    size_type       size()                         const { return s_.size(); }

    reference       top()                                { return s_.back(); }
    const_reference top()                          const { return s_.back(); }
    void            push(const value_type& value)        { s_.push_back(value); }
    void            pop()                                { s_.pop_back(); } 

    void clear()                                         { while (!empty())  pop(); }
    void swap(stack& rhs)                                { mystl::swap(s_, rhs.s_); }

  public:
    friend bool operator==(const stack& lhs, const stack& rhs) { return lhs.s_ == rhs.s_; }
    friend bool operator< (const stack& lhs, const stack& rhs) { return lhs.s_ <  rhs.s_; }
};

// 重载比较操作符
template <class T, class Sequence>
bool operator==(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
    return lhs == rhs;
}

template <class T, class Sequence>
bool operator<(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
    return lhs < rhs;
}

template <class T, class Sequence>
bool operator!=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Sequence>
bool operator>(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
    return rhs < lhs;
}

template <class T, class Sequence>
bool operator<=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Sequence>
bool operator>=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
    return !(lhs < rhs);
}
    
// 重载 mystl 的 swap
template <class T, class Sequence>
void swap(stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) {
    lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_STACK_H_

