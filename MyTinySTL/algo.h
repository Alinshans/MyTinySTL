#ifndef MYTINYSTL_ALGO_H_
#define MYTINYSTL_ALGO_H_

// 这个头文件包含了 mystl 的一系列算法

#include <cstdlib>
#include <ctime>

#include "algobase.h"
#include "heap_algo.h"
#include "iterator.h"

namespace mystl {

	/*********************************************************************************/
	// adjacent_find
	// 找出第一对相邻的重复元素，如果找回返回一个迭代器，指向这对元素的第一个元素
	/*********************************************************************************/
	template <class ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
		if (first == last)	return last;	//区间为空或未找到都返回last
		auto next = first;
		while (++next != last) {
			if (*next == *first)	return first;
			first = next;
		}
		return last;	//未找到
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator, class Compared>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
		Compared comp) {
		if (first == last)	return last;	//区间为空或未找到都返回last
		auto next = first;
		while (++next != last) {
			if (comp(*next, *first))	return first;
			first = next;
		}
		return last;	//未找到
	}

	/*********************************************************************************/
	// all_of
	// 检查[first, last)内是否全部元素都满足一元操作 unary_pred 为 true 的情况，满足则返回 true
	/*********************************************************************************/
	template <class InputIterator, class UnaryPredicate>
	bool all_of(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (!unary_pred(*first))	return false;	//只要有不满足的就返回 false
		}
		return true;
	}

	/*********************************************************************************/
	// any_of
	// 检查[first, last)内是否存在某个元素满足一元操作 unary_pred 为 true 的情况，满足则返回 true
	/*********************************************************************************/
	template <class InputIterator, class UnaryPredicate>
	bool any_of(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (unary_pred(*first))	return true;	//只要存在满足的元素就返回 true
		}
		return false;
	}

	/*********************************************************************************/
	// binary_search
	// 二分查找，如果在[first, last)内有等同于 value 的元素，返回 true，否则返回 false
	/*********************************************************************************/
	template <class ForwardIterator, class T>
	bool binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
		auto i = lower_bound(first, last, value);
		return i != last && !(value < *i);
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator, class T, class Compared>
	bool binary_search(ForwardIterator first, ForwardIterator last, const T& value,
		Compared comp) {
		auto i = lower_bound(first, last, value);
		return i != last && !comp(value, *i);
	}

	/*********************************************************************************/
	// count
	// 利用 operator==，把 [first, last)区间内的元素与输入的值进行比较，并返回元素相等的个数
	/*********************************************************************************/
	template <class InputIterator, class T>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T& value) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first) {
			if (*first == value)	++n;
		}
		return n;
	}

	/*********************************************************************************/
	// count_if
	// 对[first, last)区间内的每个元素都进行一元 unary_pred 操作，返回结果为 true 的个数
	/*********************************************************************************/
	template <class InputIterator, class UnaryPredicate>
	typename iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first) {
			if (unary_pred(*first))	++n;
		}
		return n;
	}

	/*********************************************************************************/
	// equal_range
	// 查找[first,last)区间中与 value 相等的元素所形成的区间，返回一对迭代器指向区间首尾
	/*********************************************************************************/
	template <class ForwardIterator, class T>
	inline pair<ForwardIterator, ForwardIterator>
		equal_range(ForwardIterator first, ForwardIterator last, const T& value) {
		//根据迭代器不同采用不同的方法
		return __equal_range(first, last, value, distance_type(first), iterator_category(first));
	}
	
	// __equal_range 的 forward_iterator_tag 版本
	template <class ForwardIterator, class T, class Distance>
	pair<ForwardIterator, ForwardIterator>
		__equal_range(ForwardIterator first, ForwardIterator last, const T& value,
			Distance*, forward_iterator_tag) {
		Distance len = distance(first, last);
		auto half;
		auto middle, left, right;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first;
			advance(middle, half);	//令 middle 指向中间位置
			if (*middle < value) {
				first = middle;
				++first;
				len = len - half - 1;	//修正 len，查找后半段
			}
			else if (value < *middle)
				len = half;	//修正 len，查找前半段
			else {	// value == *middle
				left = lower_bound(first, last, value);
				advance(first, len);
				right = upper_bound(++middle, first, value);
				return pair<ForwardIterator, ForwardIterator>(left, right);
			}
		}
		return pair<ForwardIterator, ForwardIterator>(first, first);
	}

	// __equal_range 的 random_access_iterator_tag 版本
	template <class RandomAccessIterator, class T, class Distance>
	pair<RandomAccessIterator, RandomAccessIterator>
		__equal_range(RandomAccessIterator first, RandomAccessIterator last,
			const T& value, Distance*, random_access_iterator_tag) {
		Distance len = last - first;
		auto half;
		auto middle, left, right;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first + half;
			if (*middle < value) {
				first = middle + 1;
				len = len - half - 1;	//修正 len，查找后半段
			}
			else if (value < *middle)
				len = half;	//修正 len，查找前半段
			else {	// *middle == value
				left = lower_bound(first, middle, value);
				right = upper_bound(++middle, first + len, value);
				return pair<RandomAccessIterator, RandomAccessIterator>(left, right);
			}
		}
		return pair<RandomAccessIterator, RandomAccessIterator>(first, first);
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator, class T, class Compared>
	inline pair<ForwardIterator, ForwardIterator>
		equal_range(ForwardIterator first, ForwardIterator last, const T& value,
			Compared comp) {
		//根据迭代器不同采用不同的方法
		return __equal_range(first, last, value, comp, distance_type(first),
			iterator_category(first));
	}

	// __equal_range 的 forward iterator 版本
	template <class ForwardIterator, class T, class Distance, class Compared>
	pair<ForwardIterator, ForwardIterator>
		__equal_range(ForwardIterator first, ForwardIterator last, const T& value,
			Compared comp, Distance*, forward_iterator_tag) {
		Distance len = distance(first, last);
		auto half;
		auto middle, left, right;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first;
			advance(middle, half);	//令 middle 指向中间位置
			if (comp(*middle, value)) {
				first = middle;
				++first;
				len = len - half - 1;	//修正 len，查找后半段
			}
			else if (comp(value, *middle))
				len = half;	//修正 len，查找前半段
			else {	// value == *middle
				left = lower_bound(first, last, value, comp);
				advance(first, len);
				right = upper_bound(++middle, first, value, comp);
				return pair<ForwardIterator, ForwardIterator>(left, right);
			}
		}
		return pair<ForwardIterator, ForwardIterator>(first, first);
	}

	// __equal_range 的 random access iterator 版本
	template <class RandomAccessIterator, class T, class Distance, class Compared>
	pair<RandomAccessIterator, RandomAccessIterator>
		__equal_range(RandomAccessIterator first, RandomAccessIterator last,
			const T& value, Compared comp, Distance*, random_access_iterator_tag) {
		Distance len = last - first;
		auto half;
		auto middle, left, right;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first + half;
			if (comp(*middle, value)) {
				first = middle + 1;
				len = len - half - 1;	//修正 len，查找后半段
			}
			else if (comp(value, *middle))
				len = half;	//修正 len，查找前半段
			else {	// *middle == value
				left = lower_bound(first, middle, value, comp);
				right = upper_bound(++middle, first + len, value, comp);
				return pair<RandomAccessIterator, RandomAccessIterator>(left, right);
			}
		}
		return pair<RandomAccessIterator, RandomAccessIterator>(first, first);
	}

	/*********************************************************************************/
	// find
	// 在[first, last)区间内找到第一个匹配等同条件的元素，返回一个迭代器指向该元素
	/*********************************************************************************/
	template <class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& value) {
		while (first != last && *first != value)	++first;
		return first;
	}

	/*********************************************************************************/
	// find_end
	// 在[first1, last1)区间中查找[first2, last2)最后一次出现的地方，若不存在返回last1
	/*********************************************************************************/
	template <class ForwardIterator1, class ForwardIterator2>
	inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2) {
		typedef typename iterator_traits<ForwardIterator1>::iterator_category
			category1;
		typedef typename iterator_traits<ForwardIterator2>::iterator_category
			category2;
		return __find_end(first1, last1, first2, last2, category1(), category2());
	}

	// __find_end 的 forward_iterator_tag 版本
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		forward_iterator_tag, forward_iterator_tag) {
		if (first2 == last2)	return last1;	//查找目标为空，返回 last1
		else {
			auto result = last1;
			while (true) {
				//利用 search 查找某个子序列的首次出现点，找不到则返回 last1
				auto new_result = mystl::search(first1, last1, first2, last2);
				if (new_result == last1)	return result;	//没找到
				else {	//寻找下一个目标点
					result = new_result;
					first1 = new_result;
					++first1;
				}
			}
		}
	}

	// __find_end 的 bidirectional_iterator_tag 版本
	template <class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator1
		__find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
			BidirectionalIterator2 first2, BidirectionalIterator2 last2,
			bidirectional_iterator_tag, bidirectional_iterator_tag) {
		// BidirectionalIterator 可以进行逆向操作
		typedef reverse_iterator<BidirectionalIterator1> reviter1;
		typedef reverse_iterator<BidirectionalIterator2> reviter2;

		reviter1 rlast1(first1);
		reviter2 rlast2(first2);
		reviter1 rresult = mystl::search(reviter1(last1), rlast1, reviter2(last2), rlast2);
		if (rresult == rlast1)	return last1;	//没找到
		else {
			auto result = rresult.base();	//变回正向迭代器
			advance(result, -distance(first2, last2));	//调整回子序列的开头
			return result;
		}
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator1, class ForwardIterator2, class Compared>
	inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2, Compared comp) {
		typedef typename iterator_traits<ForwardIterator1>::iterator_category
			category1;
		typedef typename iterator_traits<ForwardIterator2>::iterator_category
			category2;
		return __find_end(first1, last1, first2, last2, category1(), category2(), comp);
	}

	// __find_end 的 forward_iterator_tag 版本
	template <class ForwardIterator1, class ForwardIterator2, class Compared>
	ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		forward_iterator_tag, forward_iterator_tag, Compared comp) {
		if (first2 == last2)	return last1;	//查找目标为空，返回 last1
		else {
			auto result = last1;
			while (true) {
				//利用 search() 查找某个子序列的首次出现点，找不到则返回 last1
				auto new_result = mystl::search(first1, last1, first2, last2, comp);
				if (new_result == last1)	return result;	//没找到
				else {	//寻找下一个目标点
					result = new_result;
					first1 = new_result;
					++first1;
				}
			}
		}
	}

	// __find_end 的 bidirectional_iterator_tag 版本
	template <class BidirectionalIterator1, class BidirectionalIterator2, class Compared>
	BidirectionalIterator1
		__find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
			BidirectionalIterator2 first2, BidirectionalIterator2 last2,
			bidirectional_iterator_tag, bidirectional_iterator_tag, Compared comp) {
		// BidirectionalIterator 可以进行逆向操作
		typedef reverse_iterator<BidirectionalIterator1> reviter1;
		typedef reverse_iterator<BidirectionalIterator2> reviter2;

		reviter1 rlast1(first1);
		reviter2 rlast2(first2);
		reviter1 rresult = mystl::search(reviter1(last1), rlast1, reviter2(last2), rlast2, comp);
		if (rresult == rlast1)	return last1;	//没找到
		else {
			auto result = rresult.base();	//变回正向迭代器
			advance(result, -distance(first2, last2));	//调整回子序列的开头
			return result;
		}
	}

	/*********************************************************************************/
	// find_first_of
	// 在[first1, last1)中查找[first2, last2)中的某些元素，返回指向第一次出现的元素的迭代器
	/*********************************************************************************/
	template <class InputIterator, class ForwardIterator>
	InputIterator find_first_of(InputIterator first1, InputIterator last1,
		ForwardIterator first2, ForwardIterator last2) {
		for (; first1 != last1; ++first1) {
			for (auto iter = first2; iter != last2; ++iter) {
				//如果序列一的元素在序列二中找到，就返回
				if (*first1 == *iter)
					return first1;
			}
		}
		return last1;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class InputIterator, class ForwardIterator, class Compared>
	InputIterator find_first_of(InputIterator first1, InputIterator last1,
		ForwardIterator first2, ForwardIterator last2, Compared comp) {
		for (; first1 != last1; ++first1) {
			for (auto iter = first2; iter != last2; ++iter) {
				//如果找到符合比较条件的元素，就返回
				if (comp(*first1, *iter))
					return first1;
			}
		}
		return last1;
	}

	/*********************************************************************************/
	// find_if
	// 在[first, last)区间内找到第一个令一元操作 unary_pred 为 true 的元素并返回指向该元素的迭代器
	/*********************************************************************************/
	template <class InputIterator, class UnaryPredicate>
	InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
		while (first != last && !unary_pred(*first))	++first;
		return first;
	}

	/*********************************************************************************/
	// find_if_not
	// 在[first, last)区间内找到第一个令一元操作 unary_pred 为 false 的元素并返回指向该元素的迭代器
	/*********************************************************************************/
	template <class InputIterator, class UnaryPredicate>
	InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
		while (first != last && unary_pred(*first))	++first;
		return first;
	}

	/*********************************************************************************/
	// for_each
	// 对[first, last)区间内的每个元素执行一个函数 f() 操作，但不能改变元素内容
	// f() 可返回一个值，但该值会被忽略
	/*********************************************************************************/
	template <class InputIterator, class Function>
	Function for_each(InputIterator first, InputIterator last, Function f) {
		for (; first != last; ++first) {
			f(*first);
		}
		return f;
	}

	/*********************************************************************************/
	// generate
	// 将函数对象 gen 的运算结果对[first, last)内的每个元素赋值
	/*********************************************************************************/
	template <class ForwardIterator, class Generator>
	void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
		for (; first != last; ++first) {
			*first = gen();
		}
	}

	/*********************************************************************************/
	// generate_n
	// 用函数对象 gen 连续对 n 个元素赋值
	/*********************************************************************************/
	template <class ForwardIterator, class Size, class Generator>
	void generate_n(ForwardIterator first, Size n, Generator gen) {
		for (; n > 0; --n, ++first) {
			*first = gen();
		}
	}

	/*********************************************************************************/
	// includes
	// 判断序列二 S2 是否被包含(<=)于序列一 S1，S1 和 S2 都必须是有序集合
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2>
	bool includes(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2) {
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1)	//序列二元素大于序列一
				return false;		//被包含条件必然不成立
			else if (*first1 < *first2)
				++first1;
			else
				++first1, ++first2;
		}
		return first2 == last2;		//判断序列二是否遍历完
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class InputIterator1, class InputIterator2, class Compared>
	bool includes(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, Compared comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first2, *first1))	//comp(S2,S1)的包含条件为真
				return false;			//被包含条件必然不成立
			else if (comp(*first1, *first2))
				++first1;
			else
				++first1, ++first2;
		}
		return first2 == last2;		//判断序列二是否遍历完
	}

	/*********************************************************************************/
	// lower_bound
	// 在[first, last)中查找第一个不小于 value 的元素的位置
	// 返回一个迭代器，指向在范围内的有序序列中可以插入指定值而不破坏容器顺序的第一个位置。
	/*********************************************************************************/
	template <class ForwardIterator, class T>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value) {
		return __lower_bound(first, last, value,
			distance_type(first), iterator_category(first));
	}

	// __lower_bound 的 forward_iterator_tag 版本
	template <class ForwardIterator, class T, class Distance>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Distance*, forward_iterator_tag) {
		Distance len = distance(first, last);	//求区间长度
		Distance half;
		ForwardIterator middle;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first;
			advance(middle, half);	//令 middle 指向中间位置
			if (*middle < value) {	//如果中间位置元素值小于 value
				first = middle;
				++first;	//令 first 指向 middle 的下一个位置
				len = len - half - 1;	//修正 len，查找后半段
			}
			else
				len = half;	//修正 len，查找前半段
		}
		return first;
	}

	// __lower_bound 的 random_access_iterator_tag 版本
	template <class RandomAccessIterator, class T, class Distance>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
		const T& value, Distance*, random_access_iterator_tag) {
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;
		while (len > 0) {	
			half = len >> 1;	//除以2
			middle = first + half;	//令 middle 指向中间位置
			if (*middle < value) {	//如果中间位置元素值小于 value
				first = middle + 1;	//令 first 指向 middle 的下一个位置
				len = len - half - 1;	//修正 len，查找后半段
			}
			else
				len = half;	//修正 len，查找前半段
		}
		return first;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator, class T, class Compared>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Compared comp) {
		return __lower_bound(first, last, value, comp,
			distance_type(first), iterator_category(fitst));
	}

	// __lower_bound 的 forward_iterator_tag 版本
	template <class ForwardIterator, class T, class Distance, class Compared>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Compared comp, Distance*, forward_iterator_tag) {
		Distance len = distance(first, last);	//求区间长度
		auto half;
		auto middle;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first;
			advance(middle, half);	//令 middle 指向中间位置
			if (comp(*middle, value)) {	
				first = middle;
				++first;	//令 first 指向 middle 的下一个位置
				len = len - half - 1;	//修正 len，查找后半段
			}
			else
				len = half;	//修正 len，查找前半段
		}
		return first;
	}

	// __lower_bound 的 random_access_iterator_tag 版本
	template <class RandomAccessIterator, class T, class Distance, class Compared>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
		const T& value, Compared comp, Distance*, random_access_iterator_tag) {
		Distance len = last - first;
		auto half;
		auto middle;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first + half;	//令 middle 指向中间位置
			if (comp(*middle, value)) {
				first = middle + 1;	//令 first 指向 middle 的下一个位置
				len = len - half - 1;	//修正 len，查找后半段
			}
			else
				len = half;	//修正 len，查找前半段
		}
		return first;
	}

	/*********************************************************************************/
	// max_element
	// 返回一个迭代器，指向序列中最大的元素
	/*********************************************************************************/
	template <class ForwardIterator>
	ForwardIterator max_elememt(ForwardIterator first, ForwardIterator last) {
		if (first == last)	return first;
		auto result = first;
		while (++first != last) {
			if (*result < *first)	result = first;
		}
		return result;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator, class Compared>
	ForwardIterator max_elememt(ForwardIterator first, ForwardIterator last,
		Compared comp) {
		if (first == last)	return first;
		auto result = first;
		while (++first != last) {
			if (comp(*result, *first))	result = first;
		}
		return result;
	}

	/*********************************************************************************/
	// median
	// 找出三个值的中间值
	/*********************************************************************************/
	template <class T>
	inline const T& median(const T& left, const T& mid, const T& right) {
		if (left < mid)
			if (mid < right)	// left < mid < right
				return mid;
			else if (left < right)	// left < right <= mid
				return right;
			else	// right <= left < mid
				return left;
		else if (left < right)	// mid <= left < right
			return left;
		else if (mid < right)	// mid < right <= left
			return right;
		else	// right <= mid <= left
			return mid;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class T, class Compared>
	inline const T& median(const T& left, const T& mid, const T& right, Compared comp) {
		if (comp(left, mid))
			if (comp(mid, right))
				return mid;
			else if (comp(left, right))
				return right;
			else
				return left;
		else if (comp(left, right))
			return left;
		else if (comp(mid, right))
			return right;
		else
			return mid;
	}

	/*********************************************************************************/
	// merge
	// 将两个经过排序的集合 S1 和 S2 合并起来置于另一段空间，返回一个迭代器指向最后一个元素的下一位置
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1) {
				*result = *first2;
				++first2
			}
			else {
				*result = *first1;
				++first1;
			}
			++result;
		}
		return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class InputIterator1, class InputIterator2, class OutputIterator,
		class Compared>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compared comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first2, *first1)) {
				*result = *first2;
				++first2
			}
			else {
				*result = *first1;
				++first1;
			}
			++result;
		}
		return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
	}

	/*********************************************************************************/
	// min_element
	// 返回一个迭代器，指向序列中最小的元素
	/*********************************************************************************/
	template <class ForwardIterator>
	ForwardIterator min_elememt(ForwardIterator first, ForwardIterator last) {
		if (first == last)	return first;
		auto result = first;
		while (++first != last) {
			if (*first < *result)	result = first;
		}
		return result;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator, class Compared>
	ForwardIterator min_elememt(ForwardIterator first, ForwardIterator last,
		Compared comp) {
		if (first == last)	return first;
		auto result = first;
		while (++first != last) {
			if (comp(*first, *result))	result = first;
		}
		return result;
	}
	
	/*********************************************************************************/
	// next_permutation
	// 取得[first, last)所标示序列的下一个排列组合，如果没有下一个排序组合，返回 false，否则返回 true
	/*********************************************************************************/
	template <class BidirectionalIterator>
	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
		if (first == last)	return false;
		auto i = first;
		++i;
		if (i = last)	return false;
		i = last;	//i 指向尾端
		--i;
		for (;;) {
			auto ii = i;
			--i;
			if (*i < *ii) {	//如果前一个元素小于后一个元素
				auto j = last;	//令 j 指向尾端
				while (!(*i < *--j));	//从尾端开始查找第一个比 *i 大的元素
				mystl::iter_swap(i, j);	//交换i，j
				mystl::reverse(ii, last);	//将 ii 之后的所有元素反转
				return true;
			}
			if (i == first) {	//进行至最前面
				mystl::reverse(first, last);	//全部反转
				return false;
			}
		}
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class BidirectionalIterator, class Compared>
	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last,
		Compared comp) {
		if (first == last)	return false;
		auto i = first;
		++i;
		if (i = last)	return false;
		i = last;	//i 指向尾端
		--i;
		for (;;) {
			auto ii = i;
			--i;
			if (comp(*i, *ii)) {	//如果令 comp(*i, *ii)为 true
				auto j = last;	//令 j 指向尾端
				while (!comp(*i, *--j));	//从尾端开始查找第一个令 comp(*i, *j)为 true 的元素
				mystl::iter_swap(i, j);	//交换i，j
				mystl::reverse(ii, last);	//将 ii 之后的所有元素反转
				return true;
			}
			if (i == first) {	//进行至最前面
				mystl::reverse(first, last);	//全部反转
				return false;
			}
		}
	}

	/*********************************************************************************/
	// none_of
	// 检查[first, last)内是否全部元素都不满足一元操作 unary_pred 为 true 的情况，满足则返回 true
	/*********************************************************************************/
	template <class InputIterator, class UnaryPredicate>
	bool none_of(InputIterator first, InputIterator last, UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (unary_pred(*first))	return false;	//只要有满足的就返回 false
		}
		return true;
	}

	/*********************************************************************************/
	// nth_element
	// 对序列重排，使得所有小于第 n 个元素的元素出现在它的前面，大于它的出现在它的后面
	/*********************************************************************************/
	template <class RandomAccessIterator>
	inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last) {
		__nth_element(first, nth, last, value_type(first));
	}

	template <class RandomAccessIterator, class T>
	void __nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last, T*) {
		while (last - first > 3) {
			auto cut = mystl::__unguarded_partition(first, last,
				static_cast<T>(mystl::median(*first, *(first + (last - first) / 2), *(last - 1))));
			if (cut <= nth)	//如果 nth 位于右段
				first = cut;	//对右段进行分割
			else
				last = cut; //对左段进行分割
		}
		mystl::__insertion_sort(first, last);
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class RandomAccessIterator, class Compared>
	inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last, Compared comp) {
		__nth_element(first, nth, last, value_type(first), comp);
	}

	template <class RandomAccessIterator, class T, class Compared>
	void __nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last, T*, Compared comp) {
		while (last - first > 3) {
			auto cut = mystl::__unguarded_partition(first, last,
				static_cast<T>(mystl::median(*first, *(first + (last - first) / 2), *(last - 1))), comp);
			if (cut <= nth)	//如果 nth 位于右段
				first = cut;	//对右段进行分割
			else
				last = cut; //对左段进行分割
		}
		mystl::__insertion_sort(first, last, comp);
	}

	/*********************************************************************************/
	// partial_sort
	// 对整个序列做部分排序，保证较小的 N 个元素以递增顺序置于[first, first + N)中
	/*********************************************************************************/
	template <class RandomAccessIterator>
	inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
		RandomAccessIterator last) {
		__partial_sort(first, middle, last, value_type(first));
	}

	template <class RandomAccessIterator, class T>
	void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
		RandomAccessIterator last, T*) {
		mystl::make_heap(first, middle);
		for (auto i = middle; i < last; ++i) {
			if (*i < *first)	// max-heap 第一个元素最大
				mystl::__pop_heap_aux(first, middle, i, static_cast<T>(*i), distance_type(first));
		}
		mystl::sort_heap(first, middle);	//排序
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class RandomAccessIterator, class Compared>
	inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
		RandomAccessIterator last, Compared comp) {
		__partial_sort(first, middle, last, comp, value_type(first));
	}

	template <class RandomAccessIterator, class Compared, class T>
	void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
		RandomAccessIterator last, Compared comp, T*) {
		mystl::make_heap(first, middle, comp);
		for (auto i = middle; i < last; ++i) {
			if (comp(*i, *first))	
				mystl::__pop_heap_aux(first, middle, i, static_cast<T>(*i), distance_type(first), comp);
		}
		mystl::sort_heap(first, middle, comp);	//排序
	}

	/*********************************************************************************/
	// partial_sort_copy
	// 行为与 partial_sort 类似，不同的是把排序结果复制到 result 容器中
	/*********************************************************************************/
	template <class InputIterator, class RandomAccessIterator>
	inline RandomAccessIterator partial_sort_copy(InputIterator first,
		InputIterator last, RandomAccessIterator result_first,
		RandomAccessIterator result_last) {
		return __partial_sort_copy(first, last, result_first, result_last,
			distance_type(result_first), value_type(first));
	}

	template <class InputIterator, class RandomAccessIterator, class Distance, class T>
	RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last,
		RandomAccessIterator result_first, RandomAccessIterator result_last,
		Distance*, T*) {
		if (result_first == result_last)	return result_last;
		auto result_iter = result_first;
		while (first != last && result_iter != result_last) {	//复制区间
			*result_iter = *first;
			++result_iter;
			++first;
		}
		mystl::make_heap(result_first, result_iter);
		while (first != last) {
			if (*first < *result_first)
				mystl::__adjust_heap(result_first, static_cast<Distance>(0),
					static_cast<Distance>(result_iter - result_first), static_cast<T>(*first));
			++first;
		}
		mystl::sort_heap(result_first, result_iter);
		return result_iter;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class InputIterator, class RandomAccessIterator, class Compared>
	inline RandomAccessIterator partial_sort_copy(InputIterator first,
		InputIterator last, RandomAccessIterator result_first,
		RandomAccessIterator result_last, Compared comp) {
		return __partial_sort_copy(first, last, result_first, result_last, comp,
			distance_type(result_first), value_type(first));
	}

	template <class InputIterator, class RandomAccessIterator, 
		class Compared, class Distance, class T>
	RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last,
		RandomAccessIterator result_first, RandomAccessIterator result_last,
		Compared comp, Distance*, T*) {
		if (result_first == result_last)	return result_last;
		auto result_iter = result_first;
		while (first != last && result_iter != result_last) {	//复制区间
			*result_iter = *first;
			++result_iter;
			++first;
		}
		mystl::make_heap(result_first, result_iter, comp);
		while (first != last) {
			if (comp(*first, *result_first))
				mystl::__adjust_heap(result_first, static_cast<Distance>(0),
					static_cast<Distance>(result_iter - result_first), static_cast<T>(*first), comp);
			++first;
		}
		mystl::sort_heap(result_first, result_iter, comp);
		return result_iter;
	}

	/*********************************************************************************/
	// partition
	// 对区间内的元素重排，被一元条件运算判定为 true 的元素会放到区间的前段
	// 该函数不保证元素的原始相对位置
	/*********************************************************************************/
	template <class BidirectionalIterator, class UnaryPredicate>
	BidirectionalIterator partition(BidirectionalIterator first,
		BidirectionalIterator last, UnaryPredicate unary_pred) {
		while (true) {
			while (true) {
				if (first == last)	return first;
				else if (unary_pred(*first))	++first;//头指针所指元素符合 unary_pred 为 true 的情况
				else break;
			}
			--last;		//尾指针回溯1
			while (true) {
				if (first == last)	return first;
				else if (!unary_pred(*last))	--last;	//尾指针所指元素符合 unary_pred 为 false 的情况
				else break;
			}
			mystl::iter_swap(first, last);	//交换头尾迭代器
			++first;	//头指针前进1，准备下一轮循环
		}
	}

	/*********************************************************************************/
	// prev_permutation
	// 取得[first, last)所标示序列的上一个排列组合，如果没有上一个排序组合，返回 false，否则返回 true
	/*********************************************************************************/
	template <class BidirectionalIterator>
	bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last) {
		if (first == last)	return false;
		auto i = first;
		++i;
		if (i = last)	return false;
		i = last;	//i 指向尾端
		--i;
		for (;;) {
			auto ii = i;
			--i;
			if (*ii < *i) {	//如果前一个元素大于后一个元素
				auto j = last;	//令 j 指向尾端
				while (!(*--j < *i));	//从尾端开始查找第一个比 *i 小的元素
				mystl::iter_swap(i, j);	//交换i，j
				mystl::reverse(ii, last);	//将 ii 之后的所有元素反转
				return true;
			}
			if (i == first) {	//进行至最前面
				mystl::reverse(first, last);	//全部反转
				return false;
			}
		}
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class BidirectionalIterator, class Compared>
	bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last,
		Compared comp) {
		if (first == last)	return false;
		auto i = first;
		++i;
		if (i = last)	return false;
		i = last;	//i 指向尾端
		--i;
		for (;;) {
			auto ii = i;
			--i;
			if (comp(*ii, *i)) {	//如果令 comp(*ii, *i)为 true
				auto j = last;	//令 j 指向尾端
				while (!comp(*--j, *i));	//从尾端开始查找第一个令 comp(*j, *i)为 true 的元素
				mystl::iter_swap(i, j);	//交换i，j
				mystl::reverse(ii, last);	//将 ii 之后的所有元素反转
				return true;
			}
			if (i == first) {	//进行至最前面
				mystl::reverse(first, last);	//全部反转
				return false;
			}
		}
	}

	/*********************************************************************************/
	// random_shuffle
	// 将[first, last)内的元素次序随机重排
	// 重载版本使用一个产生随机数的函数对象 rand
	/*********************************************************************************/
	template <class RandomAccessIterator>
	inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
		__random_shuffle(first, last, distance_type(first));
	}

	template <class RandomAccessIterator, class Distance>
	void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
		Distance*) {
		if (first == last)	return;
		srand(static_cast<unsigned>(time)(0));
		for (auto i = first + 1; i != last; ++i) {
			mystl::iter_swap(i, first + static_cast<Distance>(rand() % ((i - first) + 1)));
		}
	}

	// 重载版本使用一个产生随机数的函数对象 rand
	template <class RandomAccessIterator, class RandomNumberGenerator>
	void random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
		RandomNumberGenerator& rand) {
		if (first == last)	return;
		for (auto i = first + 1; i != last; ++i) {
			iter_swap(i, first + rand((i - first) + 1));
		}
	}

	/*********************************************************************************/
	// remove
	// 移除所有与指定 value 相等的元素
	// 并不从容器中删除这些元素，所以 remove 和 remove_if 不适用于 array
	/*********************************************************************************/
	template <class ForwardIterator, class T>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
		first = mystl::find(first, last, value);	//利用find找出第一个匹配的地方
		auto next = first;
		return first == last ? first : mystl::remove_copy(++next, last, first, value);
	}

	/*********************************************************************************/
	// remove_copy
	// 移除区间内与指定 value 相等的元素，并将结果复制到以 result 标示起始位置的容器上
	/*********************************************************************************/
	template <class InputIterator, class OutputIterator, class T>
	OutputIterator remove_copy(InputIterator first, InputIterator last,
		OutputIterator result, const T& value) {
		for (; first != last; ++first) {
			if (*first != value) {
				*result = *first;
				++result;
			}
		}
		return result;
	}

	/*********************************************************************************/
	// remove_copy_if
	// 移除区间内所有令一元操作 unary_pred 为 true 的元素，并将结果复制到以 result 为起始位置的容器上
	/*********************************************************************************/
	template <class InputIterator, class OutputIterator, class UnaryPredicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last,
		OutputIterator result, UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (!unary_pred(*first)) {
				*result = *first;
				++result;
			}
		}
		return result;
	}

	/*********************************************************************************/
	// remove_if
	// 移除区间内所有令一元操作 unary_pred 为 true 的元素
	/*********************************************************************************/
	template <class ForwardIterator, class UnaryPredicate>
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
		UnaryPredicate unary_pred) {
		first = mystl::find_if(first, last, unary_pred);	//利用find_if找出第一个匹配的地方
		auto next = first;
		return first == last ? first : mystl::remove_copy_if(++next, last, first, unary_pred);
	}

	/*********************************************************************************/
	// replace
	// 将区间内所有的 old_value 都以 new_value 替代
	/*********************************************************************************/
	template <class ForwardIterator, class T>
	void replace(ForwardIterator first, ForwardIterator last,
		const T& old_value, const T& new_value) {
		for (; first != last; ++first) {
			if (*first == old_value)	*first = new_value;
		}
	}

	/*********************************************************************************/
	// replace_copy
	// 行为与 replace 类型，不同的是将结果复制到 result 所指的容器中，原序列没有改变
	/*********************************************************************************/
	template <class InputIterator, class OutputIterator, class T>
	OutputIterator replace_copy(InputIterator first, InputIterator last,
		OutputIterator result, const T& old_value, const T& new_value) {
		for (; first != last; ++first, ++result) {
			*result = *first == old_value ? new_value : *first;
		}
		return result;
	}

	/*********************************************************************************/
	// replace_copy_if
	// 行为与 replace_if 类型，不同的是将结果复制到 result 所指的容器中，原序列没有改变
	/*********************************************************************************/
	template <class InputIterator, class OutputIterator, class UnaryPredicate, class T>
	OutputIterator replace_copy_if(InputIterator first, InputIterator last,
		OutputIterator result, UnaryPredicate unary_pred, const T& new_value) {
		for (; first != last; ++first, ++result) {
			*result = unary_pred(*first) ? new_value : *first;
		}
		return result;
	}

	/*********************************************************************************/
	// replace_if
	// 将区间内所有令一元操作 unary_pred 为 true 的元素都用 new_value 替代
	/*********************************************************************************/
	template <class ForwardIterator, class UnaryPredicate, class T>
	void replace_if(ForwardIterator first, ForwardIterator last, UnaryPredicate unary_pred,
		const T& new_value) {
		for (; first != last; ++first) {
			if (unary_pred(*first))	*first = new_value;
		}
	}

	/*********************************************************************************/
	// reverse
	// 颠倒序列中的元素
	/*********************************************************************************/
	template <class BidirectionalIterator>
	inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
		__reverse(first, last, iterator_category(first));
	}

	// __reverse 的 bidirectional_iterator_tag 版本
	template <class BidirectionalIterator>
	void __reverse(BidirectionalIterator first, BidirectionalIterator last,
		bidirectional_iterator_tag) {
		while (true) {
			if (first == last || first == -last)	// 0或1个元素
				return;
			else
				mystl::iter_swap(first++, last);
		}
	}

	// __reverse 的 random_access_iterator_tag 版本
	template <class RandomAccessIterator>
	void __reverse(RandomAccessIterator first, RandomAccessIterator last,
		random_access_iterator_tag) {
		while (first < last)	mystl::iter_swap(first++, --last);
	}

	/*********************************************************************************/
	// reverse_copy
	// 行为与 reverse 类型，不同的是将结果复制到 result 所指容器中
	/*********************************************************************************/
	template <class BidirectionalIterator, class OutputIterator>
	OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last,
		OutputIterator result) {
		while (first != last) {
			--last;
			*result = *last;
			++result;
		}
		return result;
	}

	/*********************************************************************************/
	// rotate
	// 将[first, middle)内的元素和 [middle, last)内的元素互换，可以交换两个长度不同的区间
	/*********************************************************************************/
	template <class ForwardIterator>
	inline void rotate(ForwardIterator first, ForwardIterator middle,
		ForwardIterator last) {
		if (first == middle || middle == last)	return;
		__rotate(first, middle, last, distance_type(first), iterator_category(first));
	}

	// __rotate 的 forward_iterator_tag 版本
	template <class ForwardIterator, class Distance>
	void __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last,
		Distance*, forward_iterator_tag) {
		for (auto i = middle; ;) {
			mystl::iter_swap(first, i);	//前段后段元素互换
			++first;
			++i;
			if (first == middle) {			//前半段结束
				if (i == last)	return;		//后半段也结束，交换完成
				middle = i;					//调整，对新的前后段进行交换
			}
			else if (i == last) {			//后半段结束
				i = middle;					//调整，对新的前后段进行交换
			}
		}
	}
	
	// __rotate 的 bidirectional_iterator_tag 版本
	template <class BidirectionalIterator, class Distance>
	void __rotate(BidirectionalIterator first, BidirectionalIterator middle,
		BidirectionalIterator last, Distance*, bidirectional_iterator_tag) {
		mystl::reverse(first, middle);		//先反转前半段
		mystl::reverse(middle, last);		//再反转后半段
		mystl::reverse(first, last);		//最后反转整个区间
	}

	// __rotate 的 random_access_iterator_tag 版本
	template <class RandomAccessIterator, class Distance>
	void __rotate(RandomAccessIterator first, RandomAccessIterator middle,
		RandomAccessIterator last, Distance*, random_access_iterator_tag) {
		//因为是 random access iterator，我们可以确定每个元素的位置
		//取前半段和全长的最大公因子，即循环次数
		auto n = __gcd(last - first, middle - first);
		while (n--) {
			// first + n : 起始位置
			// middle - first : 偏移量
			__rotate_cycle(first, last, first + n, middle - first, value_type(first));
		}
	}

	// 求最大公因子，辗转相除法
	template <class EuclideanRingElement>
	EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
		while (n != 0) {
			auto t = m % n;
			m = n;
			n = t;
		}
		return m;
	}

	template <class RandomAccessIterator, class Distance, class T>
	void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator initial, Distance shift, T*) {
		// initial:起始位置 ， shift:偏移量
		T value = *initial;
		auto ptr1 = initial;
		auto ptr2 = ptr1 + shift;
		while (ptr2 != initial) {
			*ptr1 = *ptr2;		//元素移动
			ptr1 = ptr2;
			if (last - ptr2 > shift)	//下次偏移未到达尾端
				ptr2 += shift;
			else						//下次偏移超出尾端
				ptr2 = first + (shift - (last - ptr2));
		}
		*ptr1 = value;	//把 value 移到最后一个元素移到的位置
	}

	/*********************************************************************************/
	// rotate_copy
	// 行为与 rotate 类型，不同的是将结果复制到 result 所指的容器中
	/*********************************************************************************/
	template <class ForwardIterator, class OutputIterator>
	ForwardIterator rotate_copy(ForwardIterator first, ForwardIterator middle, 
		ForwardIterator last, OutputIterator result) {
		//直接调用 copy，先把后半段复制到 result，再把前半段继续复制到 result
		return mystl::copy(first, middle, mystl::copy(middle, last, result));
	}

	/*********************************************************************************/
	// search
	// 在[first1, last1)中查找[first2, last2)的首次出现点
	/*********************************************************************************/
	template <class ForwardIterator1, class ForwardIterator2>
	inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2) {
		return __search(first1, last1, first2, last2, distance_type(first1),
			distance_type(first2));
	}

	template <class ForwardIterator1, class ForwardIterator2,
		class Distance1, class Distance2>
	ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2, Distance1*, Distance2*) {
		auto d1 = distance(first1, last1);
		auto d2 = distance(first2, last2);
		if (d1 < d2)	return last1;	//若序列二长度大于序列一，不可能成为其子序列
		auto current1 = first1;
		auto current2 = first2;
		while (current2 != last2) {
			if (*current1 == *current2) {	//元素相等，比对下一个元素
				++current1;
				++current2;
			}
			else {				//元素不相等
				if (d1 == d2)	//如果两个序列等长，则序列二不可能成为序列一子序列
					return last1;
				else {
					current1 = ++first1;	//调整序列一的标兵
					current2 = first2;		//重新开始比对
					--d1;					//已经排除了序列一的一个元素，长度要减1
				}
			}
		}
		return first1;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator1, class ForwardIterator2, class Compared>
	inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2, Compared comp) {
		return __search(first1, last1, first2, last2, distance_type(first1),
			distance_type(first2), comp);
	}

	template <class ForwardIterator1, class ForwardIterator2, class Compared,
		class Distance1, class Distance2>
		ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1,
			ForwardIterator2 first2, ForwardIterator2 last2,
			Distance1*, Distance2*, Compared comp) {
		auto d1 = distance(first1, last1);
		auto d2 = distance(first2, last2);
		if (d1 < d2)	return last1;	//若序列二长度大于序列一，不可能成为其子序列
		auto current1 = first1;
		auto current2 = first2;
		while (current2 != last2) {
			if (comp(*current1, *current2)) {//comp 结果为 true，比对下一个元素
				++current1;
				++current2;
			}
			else {				//comp 结果为 false
				if (d1 == d2)	//如果两个序列等长，则序列二不可能成为序列一子序列
					return last1;
				else {
					current1 = ++first1;	//调整序列一的标兵
					current2 = first2;		//重新开始比对
					--d1;					//已经排除了序列一的一个元素，长度要减1
				}
			}
		}
		return first1;
	}

	/*********************************************************************************/
	// search_n
	// 在[first, last)中查找连续 n 个 value 所形成的子序列，返回一个迭代器指向该子序列的起始处
	/*********************************************************************************/
	template <class ForwardIterator, class Size, class T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size n, const T& value) {
		if (n <= 0)	return first;
		else {
			first = mystl::find(first, last, value);	//先找出第一个 value 出现的位置
			while (first != last) {
				Size m = n - 1;	//还需要查找 n - 1 次
				auto i = first;		//从上次出现点开始找
				++i;
				while (i != last && n != 0 && *i == value) {
					++i;
					--n;
				}
				if (n == 0)		//value 出现了 n 次
					return first;
				else	//本次查找失败，重新从第一个位置开始找
					first = mystl::find(i, last, value);
			}
			return last;	//没找到
		}
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator, class Size, class T, class Compared>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size n, const T& value, Compared comp) {
		if (n <= 0)	return first;
		else {
			while (first != last) {	//先找出第一个 value 出现的位置
				if (comp(*first, value))	break;
				++first;
			}	
			while (first != last) {
				Size m = n - 1;	//还需要查找 n - 1 次
				auto i = first;		//从上次出现点开始找
				++i;
				while (i != last && n != 0 && comp(*i, value)) {
					++i;
					--n;
				}
				if (n == 0)		//value 出现了 n 次
					return first;
				else {	//本次查找失败，重新从第一个位置开始找
					while (i != last) {
						if (comp(*i, value))	break;
						++i;
					}
					first = i;
				}
			}
			return last;	//没找到
		}
	}

	/*********************************************************************************/
	// sort
	// 将[first, last)内的元素以递增的方式排序
	/*********************************************************************************/
	template <class RandomAccessIterator>
	inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
		if (first != last) {
			__introsort_loop(first, last, value_type(first), __lg(last - first) * 2);
			__final_insertion_sort(first, last);
		}
	}

	// 用于控制分割恶化的情况
	template <class Size>
	inline Size __lg(Size n) {	//找出使得 2^k <= n 时，k 的最大值
		auto k;
		for (k = 0; n > 1; n >>= 1)	++k;
		return k;
	}

	const int __SECTIONSIZE = 16;	// 小型区间的大小，在这个大小内采用插入排序

	// 内省式排序，先进行 quick sort，当分割行为有恶化倾向时，改用 heap sort
	template <class RandomAccessIterator, class T, class Size>
	void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last,
		T*, Size depth_limit) {
		while (last - first > __SECTIONSIZE) {	//区间长度大于 __SECTIONSIZE 就分割
			if (depth_limit == 0) {	//到达最大分割深度限制
				partial_sort(first, last, last);	//改用 heap_sort
				return;
			}
			--depth_limit;
			//cut : 选择一个枢纽作为分割点
			auto cut = __unguarded_partition(first, last,
				static_cast<T>(median(*(first), *(first + (last - first) / 2), *(last - 1))));
			//对右半段进行递归
			__introsort_loop(cut, last, value_type(first), depth_limit);
			last = cut;	//回到while循环，准备对左半段进行递归
		}
	}

	// 分割函数 __unguarded_partition
	template <class RandomAccessIterator, class T>
	RandomAccessIterator __unguarded_partition(RandomAccessIterator first,
		RandomAccessIterator last, T pivot) {
		while (true) {
			while (*first < pivot)	++first;	//从左边找第一个大于 pivot 的元素
			--last;
			while (pivot < *last)	--last;	//从右边找第一个小于 pivot 的元素
			if (!(first < last))	return first;	//交错，循环结束
			mystl::iter_swap(first, last);	//交换两个位置
			++first;
		}
	}

	// 最终插入排序函数 __final_insertion_sort
	template <class RandomAccessIterator>
	void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
		if (last - first > __SECTIONSIZE) {
			// 如果区间长度大于16，对前16个元素进行插入排序，剩余元素调用 __unguarded_insertion_sort
			__insertion_sort(first, first + __SECTIONSIZE);
			__unguarded_insertion_sort(first + __SECTIONSIZE, last);
		}
		else	// 区间长度小于等于16，直接调用插入排序
			__insertion_sort(first, last);
	}

	// 插入排序函数 __insertion_sort
	template <class RandomAccessIterator>
	void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
		if (first == last)	return;
		for (auto i = first + 1; i != last; ++i) {	//外循环
			//对[first, i)形成的子区间插入元素
			__linear_insert(first, i, value_type(first));
		}
	}

	// 辅助函数 __linear_insert
	template <class RandomAccessIterator, class T>
	inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last,
		T*) {
		T value = *last;	//要插入的元素值
		if (value < *first) {	//若要插入的值比头还小 （头部必定为最小元素）
			mystl::copy_backward(first, last, last + 1);	//直接把整个区间后移一位
			*first = value;	//插入到头部
		}
		else
			__unguarded_linear_insert(last, value);
	}

	// 辅助函数 __unguarded_linear_insert
	template <class RandomAccessIterator, class T>
	void __unguarded_linear_insert(RandomAccessIterator last, T value) {
		auto next = last;
		--next;
		while (value < *next) {	//从尾部开始寻找第一个可插入位置
			*last = *next;
			last = next;
			--next;
		}
		*last = value;	//插入元素
	}

	// 插入排序函数 __unguarded_insertion_sort
	template <class RandomAccessIterator>
	inline void __unguarded_insertion_sort(RandomAccessIterator first,
		RandomAccessIterator last) {
		__unguarded_insertion_sort_aux(first, last, value_type(first));
	}

	// 辅助函数 __unguarded_insertion_sort_aux
	template <class RandomAccessIterator, class T>
	void __unguarded_insertion_sort_aux(RandomAccessIterator first,
		RandomAccessIterator last, T*) {
		for (auto i = first; i != last; ++i) {	//外循环
			__unguarded_linear_insert(i, static_cast<T>(*i));
		}
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class RandomAccessIterator, class Compared>
	inline void sort(RandomAccessIterator first, RandomAccessIterator last, 
		Compared comp) {
		if (first != last) {
			__introsort_loop(first, last, value_type(first),
				__lg(last - first) * 2, comp);
			__final_insertion_sort(first, last, comp);
		}
	}

	// 内省式排序，先进行 quick sort，当分割行为有恶化倾向时，改用 heap sort
	template <class RandomAccessIterator, class T, class Size, class Compared>
	void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last,
		T*, Size depth_limit, Compared comp) {
		while (last - first > __SECTIONSIZE) {	//区间长度大于 __SECTIONSIZE 就分割
			if (depth_limit == 0) {	//到达最大分割深度限制
				partial_sort(first, last, last, comp);	//改用 heap_sort
				return;
			}
			--depth_limit;
			//cut : 选择一个枢纽作为分割点
			auto cut = __unguarded_partition(first, last,
				static_cast<T>(median(*(first), *(first + (last - first) / 2), *(last - 1))), comp);
			//对右半段进行递归
			__introsort_loop(cut, last, value_type(first), depth_limit, comp);
			last = cut;	//回到while循环，准备对左半段进行递归
		}
	}

	// 分割函数 __unguarded_partition
	template <class RandomAccessIterator, class T, class Compared>
	RandomAccessIterator __unguarded_partition(RandomAccessIterator first,
		RandomAccessIterator last, T pivot, Compared comp) {
		while (true) {
			while (comp(*first, pivot))	++first;	
			--last;
			while (comp(pivot, *last))	--last;	
			if (!(first < last))	return first;	//交错，循环结束
			mystl::iter_swap(first, last);	//交换两个位置
			++first;
		}
	}

	// 最终插入排序函数 __final_insertion_sort
	template <class RandomAccessIterator, class Compared>
	void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last,
		Compared comp) {
		if (last - first > __SECTIONSIZE) {
			// 如果区间长度大于16，对前16个元素进行插入排序，剩余元素调用 __unguarded_insertion_sort
			__insertion_sort(first, first + __SECTIONSIZE, comp);
			__unguarded_insertion_sort(first + __SECTIONSIZE, last, comp);
		}
		else	// 区间长度小于等于16，直接调用插入排序
			__insertion_sort(first, last, comp);
	}

	// 插入排序函数 __insertion_sort
	template <class RandomAccessIterator, class Compared>
	void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last,
		Compared comp) {
		if (first == last)	return;
		for (auto i = first + 1; i != last; ++i) {	//外循环
			//对[first, i)形成的子区间插入元素
			__linear_insert(first, i, value_type(first), comp);
		}
	}

	// 辅助函数 __linear_insert
	template <class RandomAccessIterator, class T, class Compared>
	inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last,
		T*, Compared comp) {
		T value = *last;	//要插入的元素值
		if (comp(value, *first)) {
			mystl::copy_backward(first, last, last + 1);	//直接把整个区间后移一位
			*first = value;	//插入到头部
		}
		else
			__unguarded_linear_insert(last, value, comp);
	}

	// 辅助函数 __unguarded_linear_insert
	template <class RandomAccessIterator, class T, class Compared>
	void __unguarded_linear_insert(RandomAccessIterator last, T vlaue, Compared comp) {
		auto next = last;
		--next;
		while (comp(value, *next)) {	//从尾部开始寻找第一个可插入位置
			*last = *next;
			last = next;
			--next;
		}
		*last = value;	//插入元素
	}

	// 插入排序函数 __unguarded_insertion_sort
	template <class RandomAccessIterator, class Compared>
	inline void __unguarded_insertion_sort(RandomAccessIterator first,
		RandomAccessIterator last, Compared comp) {
		__unguarded_insertion_sort_aux(first, last, value_type(first), comp);
	}

	// 辅助函数 __unguarded_insertion_sort_aux
	template <class RandomAccessIterator, class T, class Compared>
	void __unguarded_insertion_sort_aux(RandomAccessIterator first,
		RandomAccessIterator last, T*, Compared comp) {
		for (auto i = first; i != last; ++i) {	//外循环
			__unguarded_linear_insert(i, static_cast<T>(*i), comp);
		}
	}

	/*********************************************************************************/
	// swap_ranges
	// 将[first1, last1)内的元素与从 first2 开始、个数相同的元素交换
	// 交换的区间长度必须相同，若成功返回一个迭代器指向序列二最后一个被交换元素的下一位置
	/*********************************************************************************/
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2) {
		for (; first1 != last1; ++first1, ++first2) {
			mystl::iter_swap(first1, first2);
		}
		return first2;
	}

	/*********************************************************************************/
	// transform
	// 第一个版本以函数对象 unary_op 作用于[first, last)中的每个元素并将结果保存至 result 中
	// 第二个版本以函数对象 binary_op 作用于两个序列[first1, last1)、[first2, last2)的相同位置
	/*********************************************************************************/
	//版本一
	template <class InputIterator, class OutputIterator, class UnaryOperation>
	OutputIterator transform(InputIterator first, InputIterator last,
		OutputIterator result, UnaryOperation unary_op) {
		for (; first != last; ++first, ++result) {
			*result = unary_op(*first);
		}
		return result;
	}

	//版本二
	template <class InputIterator1, class InputIterator2, class OutputIterator,
		class BinaryOperation>
		OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, OutputIterator result, BinaryOperation binary_op) {
		for (; first1 != last1; ++first1, ++first2, ++result) {
			*result = binary_op(*first1, *first2);
		}
		return result;
	}

	/*********************************************************************************/
	// unique
	// 移除[first, last)内重复的元素，序列必须有序，和 remove 类似，它也不能真正的删除重复元素
	/*********************************************************************************/
	template <class ForwardIterator>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
		first = mystl::adjacent_find(first, last);		//利用 adjacent_find 找到相邻重复元素的起点
		return mystl::unique_copy(first, last, first);
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator, class Compared>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last, Compared comp) {
		first = mystl::adjacent_find(first, last, comp);		//利用 adjacent_find 找到相邻重复元素的起点
		return mystl::unique_copy(first, last, first, comp);
	}

	/*********************************************************************************/
	// unique_copy
	// 从[first, last)中将元素复制到 result 上，序列必须有序，如果有重复的元素，只会复制一次
	/*********************************************************************************/
	template <class InputIterator, class OutputIterator>
	inline OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result) {
		if (first == last)	return result;
		return __unique_copy(first, last, result, iterator_category(result));
	}
	
	// __unique_copy 的 forward_iterator_tag 版本
	template <class InputIterator, class ForwardIterator>
	ForwardIterator __unique_copy(InputIterator first, InputIterator last,
		ForwardIterator result, forward_iterator_tag) {
		*result = *first;	//记录第一个元素
		while (++first != last) {
			if (*result != *first)	*++result = *first;	//元素不同就复制
		}
		return ++result;
	}

	// __unique_copy 的 output_iterator_tag 版本
	template <class InputIterator, class OutputIterator>
	OutputIterator __unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, output_iterator_tag) {
		//由于 output iterator 的限制，必须先知道其 value type
		return __unique_copy(first, last, result, value_type(first));
	}

	// 由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
	template <class InputIterator, class OutputIterator, class T>
	OutputIterator __unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, T*) {
		T value = *first;
		*result = value;
		while (++first != last) {
			if (value != *first) {
				value = *first;
				*++result = value;
			}
		}
		return ++result;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class InputIterator, class OutputIterator, class Compared>
	inline OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, Compared comp) {
		if (first == last)	return result;
		return __unique_copy(first, last, result, comp, iterator_category(result));
	}

	// __unique_copy 的 forward_iterator_tag 版本
	template <class InputIterator, class ForwardIterator, class Compared>
	ForwardIterator __unique_copy(InputIterator first, InputIterator last,
		ForwardIterator result, Compared comp, forward_iterator_tag) {
		*result = *first;	//记录第一个元素
		while (++first != last) {
			if (!comp(*result, *first))	*++result = *first;	//元素不同就复制
		}
		return ++result;
	}

	// __unique_copy 的 output_iterator_tag 版本
	template <class InputIterator, class OutputIterator, class Compared>
	OutputIterator __unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, Compared comp, output_iterator_tag) {
		// 由于 output iterator 的限制，必须先知道其value type
		return __unique_copy(first, last, result, comp, value_type(first));
	}

	// 由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
	template <class InputIterator, class OutputIterator, class Compared, class T>
	OutputIterator __unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, Compared comp, T*) {
		T value = *first;
		*result = value;
		while (++first != last) {
			if (!comp(value, *first)) {
				value = *first;
				*++result = value;
			}
		}
		return ++result;
	}

	/*********************************************************************************/
	// upper_bound
	// 在[first, last)中查找最后一个不小于 value 的元素的位置
	// 返回一个迭代器，它指向在范围内的有序序列中可以插入指定值而不破坏容器顺序的最后一个位置。
	/*********************************************************************************/
	template <class ForwardIterator, class T>
	inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
		const T& value) {
		//根据迭代器的不同采用不同操作
		return __upper_bound(first, last, value,
			distance_type(first), iterator_category(fitst));
	}

	// __upper_bound 的 forward_iterator_tag 版本
	template <class ForwardIterator, class T, class Distance>
	ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Distance*, forward_iterator_tag) {
		Distance len = distance(first, last);	//求区间长度
		auto half;
		auto middle;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first;
			advance(middle, half);	//令 middle 指向中间位置
			if (value < *middle)	//如果中间位置元素值大于指定值
				len = half;	//修正 len，查找前半段
			else {
				first = middle;
				++first;	//令 first 指向 middle 的下一个位置
				len = len - half - 1;	//修正 len，查找后半段
			}
		}
		return first;
	}

	// __upper_bound 的 random_access_iterator_tag 版本
	template <class RandomAccessIterator, class T, class Distance>
	RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last,
		const T& value, Distance*, random_access_iterator_tag) {
		Distance len = last - first;
		auto half;
		auto middle;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first + half;	//令 middle 指向中间位置
			if (value < *middle)	//如果中间位置元素值大于指定值
				len = half;	//修正 len，查找前半段
			else {
				first = middle + 1;	//令 first 指向 middle 的下一个位置
				len = len - half - 1;	//修正 len，查找后半段
			}
		}
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class ForwardIterator, class T, class Compared>
	inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Compared comp) {
		//根据迭代器的不同采用不同操作
		return __upper_bound(first, last, value,
			distance_type(first), iterator_category(fitst), comp);
	}

	// __upper_bound 的 forward_iterator_tag 版本
	template <class ForwardIterator, class T, class Distance, class Compared>
	ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Distance*, forward_iterator_tag, Compared comp) {
		Distance len = distance(first, last);	//求区间长度
		auto half;
		auto middle;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first;
			advance(middle, half);	//令 middle 指向中间位置
			if (comp(value, *middle))	
				len = half;	//修正 len，查找前半段
			else {
				first = middle;
				++first;	//令 first 指向 middle 的下一个位置
				len = len - half - 1;	//修正 len，查找后半段
			}
		}
		return first;
	}

	// __upper_bound 的 random_access_iterator_tag 版本
	template <class RandomAccessIterator, class T, class Distance, class Compared>
	RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last,
		const T& value, Distance*, random_access_iterator_tag, Compared comp) {
		Distance len = last - first;
		auto half;
		auto middle;
		while (len > 0) {
			half = len >> 1;	//除以2
			middle = first + half;	//令 middle 指向中间位置
			if (comp(value, *middle))
				len = half;	//修正 len，查找前半段
			else {
				first = middle + 1;	//令 first 指向 middle 的下一个位置
				len = len - half - 1;	//修正 len，查找后半段
			}
		}
	}
}
#endif // !MYTINYSTL_ALGO_H_
