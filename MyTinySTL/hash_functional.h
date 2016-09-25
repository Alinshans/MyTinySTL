#ifndef HASH_FUNCTIONAL_H
#define HASH_FUNCTIONAL_H

#include <cstddef>

namespace MyTinySTL {
	// hash function : 计算元素位置的函数

	// 对于大部分，hash function 什么都不做
	template <class Key>
	struct hash {};

	// 针对 const char * 设计的函数
	inline size_t __hash_string(const char* s) {
		unsigned long h = 0;
		for (; *s; ++s)
			h = 5 * h + *s;
		return size_t(h);
	}

	template<> struct hash<char*> {
		size_t operator()(const char* s) const { return __hash_string(s); }
	};

	template<> struct hash<const char*> {
		size_t operator()(const char* s) const { return __hash_string(s); }
	};

	// 针对 char，int，long等整数型别，只是返回原值

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
}
#endif // !HASH_FUNCTIONAL_H

