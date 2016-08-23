#ifndef ALGOBASE_H
#define ALGOBASE_H

#ifndef USE_CSTRING
#define USE_CSTRING
#include <cstring>
#endif // !USE_CSTRING

#include "iterator.h"
#include "type_traits.h"
#include "pair.h"

//基本算法
namespace MyTinySTL {

	/*********************************************************************************/
	// copy
	// 把 [first, last)区间内的元素复制到 [result, result + (last - first))内
	/*********************************************************************************/
	//完全泛化版本
	template <class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last,
		OutputIterator result) {
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}

	//const char*的特化版本
	inline char* copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}

	//const wchar_t*的特化版本
	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}

	// __copy_dispatch的完全泛化版本
	template <class InputIterator, class OutputIterator>
	struct __copy_dispatch {
		OutputIterator operator()(InputIterator first, InputIterator last,
			OutputIterator result) {
			return __copy(first, last, result, iterator_category(first));
		}
	};

	// __copy_dispatch的偏特化版本(1)，两个参数都是 T* 指针形式
	template <class T>
	struct __copy_dispatch<T*, T*> {
		T* operator()(T* first, T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	// __copy_dispatch的偏特化版本(2)，第一个参数是 const T* 指针形式，第二个参数是 T* 指针形式
	template <class T>
	struct __copy_dispatch<const T*, T*> {
		T* operator()(const T* first, const T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	// __copy 的 InputIterator 版本
	template <class InputIterator, class OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last,
		OutputIterator result, input_iterator_tag) {
		// 判断迭代器是否相等决定循环，速度慢
		for (; first != last; ++first, ++result) {
			*result = *first
		}
		return result;
	}

	// __copy 的 RandomAccessIterator 版本
	template <class RandomAccessIterator, class OutputIterator>
	inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, random_access_iterator_tag) {
		return __copy_d(first, last, result, distance_type(first));
	}

	// __copy_d 
	template <class RandomAccessIterator, class OutputIterator, class Distance>
	inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, Distance*) {
		//以 n 决定循环次数，速度快
		for (Distance n = last - first; n > 0; --n, ++first, ++result) {
			*result = *first;
		}
		return result;
	}

	// __copy_t : 指针所指对象具备 trivial assignment operator
	template <class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
		memmove(result, first, sizeof(T) * (last - first));
		return result + (last - first);
	}

	// __copy_t : 指针所指对象具备 non-trivial assignment operator
	template <class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
		// 原生指针也是 RandomAccessIterator，交给 __copy_d 处理
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}

	/*********************************************************************************/
	// copy_backward
	// 将 [first, last)区间内的元素复制到 [result - (last - first), result)内
	/*********************************************************************************/
	template <class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last, BidirectionalIterator2 result) {
		typename __type_traits<typename iterator_traits<BidirectionalIterator2>::value_type>
			::has_trivial_assignment_operator	trivial;
		return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2,
			trivial()>::copy(first, last, result);
	}

	// __copy_backward_dispatch 泛化版本
	template <class BidirectionalIterator1, class BidirectionalIterator2, class Type>
	struct __copy_backward_dispatch {
		typedef typename iterator_traits<BidirectionalIterator1>::iterator_category category;
		typedef typename iterator_traits<BidirectionalIterator1>::difference_type distance;

		static BidirectionalIterator2 copy(BidirectionalIterator1 first,
			BidirectionalIterator1 last, BidirectionalIterator2 result) {
			return __copy_backward(first, last, result, category(), (distance*)0);
		}
	};

	// __copy_backward_dispatch 偏特化版本，两个参数都是 T*
	template <class T>
	struct __copy_backward_dispatch<T*, T*, __true_type> {
		static T* copy(const T* first, const T* last, T* result) {
			const ptrdiff_t n = last - first;
			memmove(result - n, first, sizeof(T) * n);
			return result - n;
		}
	};

	// __copy_backward_dispatch 偏特化版本，第一个参数是 const T*，第二个参数是 T*
	template <class T>
	struct __copy_backward_dispatch<const T*, T*, __true_type> {
		static T* copy(const T* first, const T* last, T* result) {
			return __copy_backward_dispatch<T*, T*, __true_type>
				::copy(first, last, result);
		}
	};

	// __copy_backward 的 bidirectional_iterator_tag 版本
	template <class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
	inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last, BidirectionalIterator2 result,
		bidirectional_iterator_tag, Distance*) {
		while (first != last) {
			*--result = *--first;
		}
		return result;
	}
	
	// __copy_backward 的 random_access_iterator_tag 版本
	template <class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
	inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last, BidirectionalIterator2 result,
		random_access_iterator_tag, Distance*) {
		for (Distance n = last - first; n > 0; --n) {
			*--result = *--first;
		}
		return result;
	}

	/*********************************************************************************/
	// copy_n
	// 把 [first, first + n)区间上的元素复制到 [result, result + n)上
	// 返回一个 pair 分别指向复制结束的尾部
	/*********************************************************************************/
	template <class InputIterator, class Size, class OutputIterator>
	inline pair<InputIterator, OutputIterator> copy_n(InputIterator first, Size n,
		OutputIterator result) {
		return __copy_n(first, n, result, iterator_traits(first));
	}

	// __copy_n 的 input_iterator_tag 版本
	template <class InputIterator, class Size, class OutputIterator>
	inline pair<InputIterator, OutputIterator> __copy_n(InputIterator first, Size n,
		OutputIterator result, input_iterator_tag) {
		for (; n > 0; --n, ++first, ++result) {
			*result = *first;
		}
		return pair<InputIterator, OutputIterator>(first, result);
	}

	// __copy_n 的 random_access_iterator_tag 版本
	template <class InputIterator, class Size, class OutputIterator>
	inline pair<InputIterator, OutputIterator> __copy_n(InputIterator first, Size n,
		OutputIterator result, random_access_iterator_tag) {
		InputIterator last = first + n;
		return pair<InputIterator, OutputIterator>(last, copy(first, last, result));
	}
	
	/*********************************************************************************/
	// equal
	// 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2>
	inline bool equal(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last) {
		for (; first1 != last1; ++first1, ++first2) {
			if (!first2)	return false;	//序列2没有元素了
			if (*first1 != *first2)	return false;
		}
		return true;	//全部相等，返回true
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class InputIterator1, class InputIterator2, class Compared>
	inline bool equal(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last, Compared comp) {
		for (; first1 != last1; ++first1, ++first2) {
			if (!first2)	return false;	//序列2没有元素了
			if (!comp(*first1, *first2))	return false;	
		}
		return true;	//全部相等，返回true
	}

	/*********************************************************************************/
	// fill
	// 为 [first, last)区间内的所有元素填充新值
	/*********************************************************************************/
	template <class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value) {
		for (; first != last; ++first) {
			*first = value;
		}
	}

	//为 char* 类型提供特化版本
	inline void fill(unsigned char* first, unsigned char* last, const unsigned char& c) {
		unsigned char tmp = c;
		memset(first, tmp, last - first);
	}

	inline void fill(signed char* first, signed char* last, const signed char& c) {
		signed char tmp = c;
		memset(first, static_cast<unsigned char>(tmp), last - first);
	}

	inline void fill(char* first, char* last, const char& c) {
		char tmp = c;
		memset(first, static_cast<unsigned char>(tmp), last - first);
	}

	/*********************************************************************************/
	// fill_n
	// 从 first 位置开始填充 n 个新值
	/*********************************************************************************/
	template <class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
		for (; n > 0; --n, ++first) {
			*first = value;
		}
		return first;
	}

	//为 char* 类型提供特化版本
	template <class Size>
	inline unsigned char* fill_n(unsigned char* first, Size n, const unsigned char& c) {
		fill(first, first + n, c);
		return first + n;
	}

	template <class Size>
	inline signed char* fill_n(signed char* first, Size n, const signed char& c) {
		fill(first, first + n, c);
		return first + n;
	}

	template <class Size>
	inline char* fill_n(char* first, Size n, const char& c) {
		fill(first, first + n, c);
		return first + n;
	}

	/*********************************************************************************/
	// iter_swap
	// 将两个 ForwardIterator 所指对象对调
	/*********************************************************************************/
	template <class ForwardIterator1, class ForwardIterator2>
	inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
		typename iterator_traits<ForwardIterator1>::value_type tmp = *a;
		*a = *b;
		*b = tmp;
	}

	/*********************************************************************************/
	// max
	// 取二者中的较大值
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class T>
	inline const T& max(const T& a, const T& b) {
		return a < b ? b : a;
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class T, class Compare>
	inline const T& max(const T& a, const T& b, Compare comp) {
		return comp(a, b) ? b : a;
	}
	
	/*********************************************************************************/
	// min 
	// 取二者中的较小值
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class T>
	inline const T& min(const T& a, const T& b) {
		return b < a ? b : a;
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class T, class Compare>
	inline const T& min(const T& a, const T& b, Compare comp) {
		return comp(b, a) ? b : a;
	}

	/*********************************************************************************/
	// lexicographical_compare
	// 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
	// (1)如果第一序列的元素较小，返回 true ，否则返回 false
	// (2)如果到达 last1 而尚未到达 last2 返回 true
	// (3)如果到达 last2 而尚未到达 last1 返回 false
	// (4)如果同时到达 last1 和 last2 返回 false
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2) {
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (*first1 < *first2)	//情况(1)
				return true;
			if (*first2 < *first1)
				return false;
			//若相等则进行下一组比较
		}
		return first1 == last1 && first2 != last2;	//只有情况(2)才返回 true
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class InputIterator1, class InputIterator2, class Compred>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, Compred comp) {
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (comp(*first1, *first2))
				return true;
			if (comp(*first2, *first1))
				return false;
		}
		return first1 == last1 && first2 != last2;
	}

	//针对 const unsigned char* 的特化版本
	inline bool lexicographical_compare(const unsigned char* first1,
		const unsigned char* last1, const unsigned char* first2,
		const unsigned char* last2) {
		const size_t len1 = last1 - first1;
		const size_t len2 = last2 - first2;
		//先比较相同长度的部分
		const int result = memcmp(first1, first2, min(len1, len2));
		//若相等，则较长的比较大
		return result != 0 ? result < 0 : len1 < len2;
	}
	
	/*********************************************************************************/
	// mismatch
	// 平行比较两个序列，找到第一处失配的点，返回一对迭代器，分别指向两个序列中失配的点
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2>
	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
		InputIterator1 last1, InputIterator2 first2) {
		while (first1 != last1 && *first1 == *first2) {
			++first1;
			++first2;
		}
		return pair<InputIterator1, InputIterator2>(first1, first2);
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class InputIterator1, class InputIterator2, class Compred>
		pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
			InputIterator1 last1, InputIterator2 first2, Compred comp) {
		while (first1 != last1 && comp(*first1, *first2)) {
			++first1;
			++first2;
		}
		return pair<InputIterator1, InputIterator2>(first1, first2);
	}

	/*********************************************************************************/
	// swap
	// 交换两个对象的值
	/*********************************************************************************/
	template <class T>
	inline void swap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;
	}
}
#endif // !ALGOBASE_H
