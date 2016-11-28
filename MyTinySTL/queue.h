#ifndef MYTINYSTL_QUEUE_H_
#define MYTINYSTL_QUEUE_H_

// 这个头文件包含了两个模板类 queue 和 priority_queue
// queue          : 队列
// priority_queue : 优先队列

#include "deque.h"
#include "functional.h"
#include "heap_algo.h"
#include "vector.h"

namespace mystl {

// 模板类 queue
// 参数一代表数据类型，参数二代表底层容器类型，缺省使用 deque 作为底层容器
template <class T, class Sequence = mystl::deque<T>>
class queue {
  public:
    // 使用底层容器的型别
    typedef typename Sequence::value_type         value_type;
    typedef typename Sequence::size_type          size_type;
    typedef typename Sequence::reference          reference;
    typedef typename Sequence::const_reference    const_reference;
    typedef Sequence                              container_type;

  private:
    Sequence s_;  // 用底层容器来表现 queue

  public:
    // 构造、复制、移动函数
    queue() :s_() {}
    explicit queue(size_type n) :s_(n) {}
    queue(size_type n, const value_type& value) :s_(n, value) {}
    template <class InputIterator>
    queue(InputIterator first, InputIterator last) : s_(first, last) {}

    queue(const Sequence& s) :s_(s) {}
    queue(Sequence&& s) :s_(std::move(s)) {}

    queue(const queue& rhs) :s_(rhs.s_) {}
    queue(queue&& rhs) :s_(std::move(rhs.s_)) {}

    queue& operator=(const queue& rhs) { s_ = rhs.s_; return *this; }
    queue& operator=(queue&& rhs) { s_ = std::move(rhs.s_); return *this; }

    // 以下操作使用底层容器的操作
    bool            empty()                       const { return s_.empty(); }
    size_type       size()                        const { return s_.size(); }

    reference       front()                             { return s_.front(); }
    const_reference front()                       const { return s_.front(); }
    reference       back()                              { return s_.back(); }
    const_reference back()                        const { return s_.back(); }

    void            push(const value_type& value)       { s_.push_back(value); }
    void            pop()                               { s_.pop_front(); }
    void            clear()                             { while (!empty()) pop(); }
    void            swap(queue& rhs)                    { mystl::swap(s_, rhs.s_); }

  public:
    friend bool operator==(const queue& lhs, const queue& rhs) { return lhs.s_ == rhs.s_; }
    friend bool operator< (const queue& lhs, const queue& rhs) { return lhs.s_ <  rhs.s_; }
};

// 重载比较操作符
template <class T, class Sequence>
bool operator==(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
    return lhs == rhs;
}

template <class T, class Sequence>
bool operator!=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Sequence>
bool operator<(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
    return lhs < rhs;
}

template <class T, class Sequence>
bool operator>(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
    return rhs < lhs;
}

template <class T, class Sequence>
bool operator<=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Sequence>
bool operator>=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
    return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T, class Sequence>
void swap(queue<T, Sequence>& lhs, queue<T, Sequence>& rhs) {
    lhs.swap(rhs);
}

/*****************************************************************************************/

// 模板类 priority_queue
// 参数一代表数据类型，参数二代表容器类型，缺省使用 vector 作为底层容器
// 参数三代表比较权值的方式，缺省使用 mystl 的 less 作为比较方式
template <class T, class Sequence = mystl::vector<T>, class Compare = mystl::less<T>>
class priority_queue {
  public:
    // 使用底层容器的型别
    typedef typename Sequence::value_type         value_type;
    typedef typename Sequence::size_type          size_type;
    typedef typename Sequence::reference          reference;
    typedef typename Sequence::const_reference    const_reference;
    typedef Sequence                              container_type;

  private:
    Sequence s_;     // 用底层容器来表现 priority_queue
    Compare  comp_;  // 权值比较大小的标准

  public:
    // 构造、复制、移动函数
    priority_queue() :s_() {}
    explicit priority_queue(const Compare& c) :s_(), comp_(c) {}
    explicit priority_queue(size_type n) :s_(n) {
        mystl::make_heap(s_.begin(), s_.end(), comp_);
    }
    priority_queue(size_type n, const value_type& value) :s_(n, value) {
        mystl::make_heap(s_.begin(), s_.end(), comp_);
    }
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last): s_(first, last) {
        mystl::make_heap(s_.begin(), s_.end(), comp_); 
    }

    priority_queue(const Sequence& s) :s_(s) {
        mystl::make_heap(s_.begin(), s_.end(), comp_);
    }
    priority_queue(Sequence&& s) :s_(std::move(s)) { 
        mystl::make_heap(s_.begin(), s_.end(), comp_); 
    }

    priority_queue(const priority_queue& rhs) :s_(rhs.s_), comp_(rhs.comp_) { 
        mystl::make_heap(s_.begin(), s_.end(), comp_);
    }
    priority_queue(priority_queue&& rhs) :s_(std::move(rhs.s_)), comp_(rhs.comp_) {
        mystl::make_heap(s_.begin(), s_.end(), comp_);
    }

    priority_queue& operator=(const priority_queue& rhs) {
        s_ = rhs.s_;
        comp_ = rhs.comp_;
        mystl::make_heap(s_.begin(), s_.end(), comp_);
        return *this;
    }
    priority_queue& operator=(priority_queue&& rhs) {
        s_ = std::move(rhs.s_);
        comp_ = rhs.comp_;
        mystl::make_heap(s_.begin(), s_.end(), comp_);
        return *this;
    }

  public:
    // 以下操作使用底层容器的操作
    bool            empty() const { return s_.empty(); }
    size_type       size()  const { return s_.size(); }

    const_reference top()   const { return s_.front(); }

    void push(const value_type& value) {
        try {
            s_.push_back(value);
            mystl::push_heap(s_.begin(), s_.end(), comp_);
        }
        catch (...) { s_.clear(); }
    }
    void pop() {
        try {
            mystl::pop_heap(s_.begin(), s_.end(), comp_);
            s_.pop_back();
        }
        catch (...) { s_.clear(); }
    }

    void clear()                   { while (!empty())    pop(); }
    void swap(priority_queue& rhs) { mystl::swap(s_, rhs.s_); mystl::swap(comp_, rhs.comp_); }
};

// 重载 mystl 的 swap
template <class T, class Sequence, class Compare>
void swap(priority_queue<T, Sequence, Compare>& lhs, priority_queue<T, Sequence, Compare>& rhs) {
    lhs.swap(rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_QUEUE_H_

