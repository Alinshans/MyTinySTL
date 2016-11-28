#ifndef MYTINYSTL_PAIR_H_
#define MYTINYSTL_PAIR_H_

// 这个头文件包含一个结构体模板 pair，其中储存了一对数据类型

namespace mystl {

// 结构体模板 : pair
// 有两个模板参数，参数一代表第一种数据类型，参数二代表第二种数据类型
// 用 first 和 second 来分别取出第一个数据和第二个数据
template <class T1, class T2>
struct pair {
    typedef T1    first_type;
    typedef T2    second_type;

    T1 first;   // 保存第一个数据
    T2 second;  // 保存第二个数据

    // 构造函数
    pair() :first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b) :first(a), second(b) {}

    template <class U1, class U2>
    pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
};

// 重载比较操作符 
template <class T1, class T2>
inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T1, class T2>
inline bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return !(x == y);
}

template <class T1, class T2>
inline bool operator>(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return y < x;
}

template <class T1, class T2>
inline bool operator<=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return !(y < x);
}

template <class T1, class T2>
inline bool operator>=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return !(x < y);
}

// 一个全局函数，让两个数据成为一个 pair
template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& x, const T2& y) {
    return pair<T1, T2>(x, y);
}

} // namespace mystl
#endif // !MYTINYSTL_PAIR_H_

