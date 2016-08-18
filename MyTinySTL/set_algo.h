#ifndef SET_ALGO_H
#define SET_ALGO_H

#include "iterator.h"
#include "algobase.h"

//包含set的四种算法：union(并集),intersection(交集),difference(差集),symmetric difference(对称差集)
//接受 set/multiset 容器作为输入区间
namespace MyTinySTL {

	/*********************************************************************************/
	// set_union
	// 计算S1∪S2的结果并保存到result中，返回输出区间的尾端
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				*result = *first1;
				++first1;
			}
			else if (*first2 < *first1) {
				*result = *first2;
				++first2;
			}
			else {
				*result = *first1;
				++first1;
				++first2;
			}
			++result;
		}
		//将剩余元素拷贝到 result
		return copy(first2, last2, copy(first1, last1, result));
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class InputIterator1, class InputIterator2, class OutputIterator,
	class Compared>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, OutputIterator result,Compared comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2)) {
				*result = *first1;
				++first1;
			}
			else if (comp(*first2, *first1)) {
				*result = *first2;
				++first2;
			}
			else {
				*result = *first1;
				++first1;
				++first2;
			}
			++result;
		}
		//将剩余元素拷贝到 result
		return copy(first2, last2, copy(first1, last1, result));
	}

	/*********************************************************************************/
	// set_intersection
	// 计算S1∩S2的结果并保存到result中，返回输出区间的尾端
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2)	++first1;
			else if (*first2 < *first1)	++first2;
			else {
				*result = *first1;
				++first1;
				++first2;
				++result;
			}
			return result;
		}
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class InputIterator1, class InputIterator2, class OutputIterator, 
		class Compared>
	OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, OutputIterator result,
		Compared comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2))	++first1;
			else if (comp(*first2, *first1))	++first2;
			else {
				*result = *first1;
				++first1;
				++first2;
				++result;
			}
			return result;
		}
	}

	/*********************************************************************************/
	// set_difference
	// 计算S1-S2的结果并保存到result中，返回输出区间的尾端
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				*result = *first1;
				++first1;
				++result;
			}
			else if (*first2 < *first1) {
				++first2;
			}
			else {
				++first1;
				++first2;
			}
		}
		return copy(first1, last1, result);
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class InputIterator1, class InputIterator2, class OutputIterator,
		class Compared>
	OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, OutputIterator result,
		Compared comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2)) {
				*result = *first1;
				++first1;
				++result;
			}
			else if (comp(*first2, *first1)) {
				++first2;
			}
			else {
				++first1;
				++first2;
			}
		}
		return copy(first1, last1, result);
	}

	/*********************************************************************************/
	// set_symmetric_difference
	// 计算(S1-S2)∪(S2-S1)的结果并保存到result中，返回输出区间的尾端
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				*result = *first1;
				++first1;
				++result;
			}
			else if (*first2 < *first1) {
				*result = *first2;
				++first2;
				++result;
			}
			else {
				++first1;
				++first2;
			}
		}
		return copy(first2, last2, copy(first1, last1, result));
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class InputIterator1, class InputIterator2, class OutputIterator,
		class Compared>
	OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, OutputIterator result,
		Compared comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2)) {
				*result = *first1;
				++first1;
				++result;
			}
			else if (comp(*first2, *first1)) {
				*result = *first2;
				++first2;
				++result;
			}
			else {
				++first1;
				++first2;
			}
		}
		return copy(first2, last2, copy(first1, last1, result));
	}
}
#endif // !SET_ALGO_H

