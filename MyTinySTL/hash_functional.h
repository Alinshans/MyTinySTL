#ifndef MYTINYSTL_HASH_FUNCTIONAL_H_
#define MYTINYSTL_HASH_FUNCTIONAL_H_

// 这个头文件包含了应用于 hash 的函数对象，用于计算元素位置

#include <cstddef>

#include "string.h"

namespace mystl {

// 对于大部分类型，hash function 什么都不做
template <class Key>
struct hash {};

// 针对 char* 和 const char* 的特化版本
inline size_t __hash_string(const char* s) {
    size_t h = 0;
    for (; *s; ++s)
        h = 3 * h + static_cast<size_t>(*s);
    return h;
}

template<> struct hash<char*> {
    size_t operator()(const char* s) const { return __hash_string(s); }
};

template<> struct hash<const char*> {
    size_t operator()(const char* s) const { return __hash_string(s); }
};
    
// 针对 mystl::string 的特化版本
template<> struct hash<mystl::string> {
    size_t operator()(const mystl::string& str) const { return __hash_string(str.data()); }
};

// 针对 char，int，long 等整数型别，只是返回原值

template <> struct hash<char> {
    size_t operator()(char x) const { return x; }
};

template <> struct hash<unsigned char> {
    size_t operator()(unsigned char x) const { return x; }
};

template <> struct hash<signed char> {
    size_t operator()(unsigned char x) const { return x; }
};

template <> struct hash<short> {
    size_t operator()(short x) const { return x; }
};

template <> struct hash<unsigned short> {
    size_t operator()(unsigned short x) const { return x; }
};

template <> struct hash<int> {
    size_t operator()(int x) const { return x; }
};

template <> struct hash<unsigned int> {
    size_t operator()(unsigned int x) const { return x; }
};

template <> struct hash<long> {
    size_t operator()(long x) const { return x; }
};

template <> struct hash<unsigned long> {
    size_t operator()(unsigned long x) const { return x; }
};

} // namespace mystl
#endif // !MYTINYSTL_HASH_FUNCTIONAL_H_

