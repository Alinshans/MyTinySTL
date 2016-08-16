#ifndef ALGO_H
#define ALGO_H

#include "iterator.h"
#include "reverse_iterator.h"

//包含其他算法
namespace MyTinySTL {

	/*************************************************************************/
	// adjacent_find
	// 找出第一对相邻的重复元素，如果找回返回一个 ForwardIterator，指向这对元素的第一个元素
	// 重载版本中使用 comp 代替 operator=
	/*************************************************************************/
	template <class ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
		if (first == last)	return last;	//区间为空或未找到都返回last
		ForwardIterator next = first;
		while (++next != last) {
			if (*next == *first)	return first;
			first = next;
		}
		return last;	//未找到
	}

	//用 comp 来取代比较操作
	template <class ForwardIterator, class Compared>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
		Compared comp) {
		if (first == last)	return last;	//区间为空或未找到都返回last
		ForwardIterator next = first;
		while (++next != last) {
			if (comp(*first, *next))	return first;
			first = next;
		}
		return last;	//未找到
	}

	/*************************************************************************/
	// count
	// 利用等于操作符，把 [first, last)区间内的元素与输入的值进行比较，并返回相等元素的个数
	/*************************************************************************/
	template <class InputIterator, class T>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T& value) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first) {
			if (*first == value)	++n;
		}
		return n;
	}

	/*************************************************************************/
	// count_if
	// 对[first, last)区间内的每个元素都进行 comp 操作，返回结果为 true 的个数
	/*************************************************************************/
	template <class InputIterator, class Compared>
	typename iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, Compared comp) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first) {
			if (comp(*first))	++n;
		}
		return n;
	}

	/*************************************************************************/
	// find
	// 在[first, last)区间内找到第一个匹配等同条件的元素，返回一个InputIterator指向该元素
	/*************************************************************************/
	template <class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& value) {
		while (first != last && *first != value)	++first;
		return first;
	}

	/*************************************************************************/
	// find_if
	// 在[first, last)区间内找到第一个令 comp 为 true 的元素并返回指向该元素的迭代器
	/*************************************************************************/
	template <class InputIterator, class Compared>
	InputIterator find_if(InputIterator first, InputIterator last, Compared comp) {
		while (first != last && !comp(*first))	++first;
		return first;
	}

	/*************************************************************************/
	// find_end
	// 在[first1, last1)区间中查找[first2, last2)最后一次出现的地方，若不存在返回last1
	// 重载版本中使用 comp 代替 operator=
	/*************************************************************************/
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
			ForwardIterator1 result = last1;
			while (true) {
				//利用 search() 查找某个子序列的首次出现点，找不到则返回 last1
				ForwardIterator1 new_result = search(first1, last1, first2, last2);
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
		reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);
		if (rresult == rlast1)	return last1;	//没找到
		else {
			BidirectionalIterator1 result = rresult.base();	//变回正向迭代器
			advance(result, -distance(first2, last2));	//调整回子序列的开头
			return result;
		}
	}

	// 重载版本，使用仿函数 comp 代替 operator=
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
			ForwardIterator1 result = last1;
			while (true) {
				//利用 search() 查找某个子序列的首次出现点，找不到则返回 last1
				ForwardIterator1 new_result = search(first1, last1, first2, last2, comp);
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
		reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2, comp);
		if (rresult == rlast1)	return last1;	//没找到
		else {
			BidirectionalIterator1 result = rresult.base();	//变回正向迭代器
			advance(result, -distance(first2, last2));	//调整回子序列的开头
			return result;
		}
	}

	/*************************************************************************/
	// find_first_of
	// 在[first1, last1)中查找[first2, last2)中的某些元素，返回指向第一次出现的元素的迭代器
	// 重载版本中使用 comp 代替 operator=
	/*************************************************************************/ 
	template <class InputIterator, class ForwardIterator>
	InputIterator find_first_of(InputIterator first1, InputIterator last1,
		ForwardIterator first2, ForwardIterator last2) {
		for (; first1 != last1; ++first1) {
			for (ForwardIterator iter = first2; iter != last2; ++iter) {
				//如果序列一的元素在序列二中找到，就返回
				if (*first1 == *iter)
					return first1;
			}
		}
		return last1;
	}

	// 重载版本中使用 comp 代替 operator=
	template <class InputIterator, class ForwardIterator, class Compared>
	InputIterator find_first_of(InputIterator first1, InputIterator last1,
		ForwardIterator first2, ForwardIterator last2, Compared comp) {
		for (; first1 != last1; ++first1) {
			for (ForwardIterator iter = first2; iter != last2; ++iter) {
				//如果序列一的元素在序列二中找到，就返回
				if (comp(*first1, *iter))
					return first1;
			}
		}
		return last1;
	}
}
#endif // !ALGO_H

