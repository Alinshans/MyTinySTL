#ifndef ALGO_H
#define ALGO_H

#include "iterator.h"
#include "reverse_iterator.h"
#include "algobase.h"

//包含其他算法
namespace MyTinySTL {

	/*********************************************************************************/
	// adjacent_find
	// 找出第一对相邻的重复元素，如果找回返回一个 ForwardIterator，指向这对元素的第一个元素
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
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

	// 重载版本使用仿函数 comp 代替比较操作
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

	/*********************************************************************************/
	// count
	// 利用等于操作符，把 [first, last)区间内的元素与输入的值进行比较，并返回相等元素的个数
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
	// 对[first, last)区间内的每个元素都进行 pred 操作，返回结果为 true 的个数
	/*********************************************************************************/
	template <class InputIterator, class Predicate>
	typename iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, Predicate pred) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first) {
			if (pred(*first))	++n;
		}
		return n;
	}

	/*********************************************************************************/
	// find
	// 在[first, last)区间内找到第一个匹配等同条件的元素，返回一个InputIterator指向该元素
	/*********************************************************************************/
	template <class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& value) {
		while (first != last && *first != value)	++first;
		return first;
	}

	/*********************************************************************************/
	// find_if
	// 在[first, last)区间内找到第一个令 pred 为 true 的元素并返回指向该元素的迭代器
	/*********************************************************************************/
	template <class InputIterator, class Predicate>
	InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
		while (first != last && !pred(*first))	++first;
		return first;
	}

	/*********************************************************************************/
	// find_end
	// 在[first1, last1)区间中查找[first2, last2)最后一次出现的地方，若不存在返回last1
	// 重载版本使用仿函数 comp 代替比较操作
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

	// 重载版本使用仿函数 comp 代替比较操作
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

	/*********************************************************************************/
	// find_first_of
	// 在[first1, last1)中查找[first2, last2)中的某些元素，返回指向第一次出现的元素的迭代器
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
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

	// 重载版本使用仿函数 comp 代替比较操作
	template <class InputIterator, class ForwardIterator, class Compared>
	InputIterator find_first_of(InputIterator first1, InputIterator last1,
		ForwardIterator first2, ForwardIterator last2, Compared comp) {
		for (; first1 != last1; ++first1) {
			for (ForwardIterator iter = first2; iter != last2; ++iter) {
				//如果找到符合比较条件的元素，就返回
				if (comp(*first1, *iter))
					return first1;
			}
		}
		return last1;
	}

	/*********************************************************************************/
	// for_each
	// 对[first, last)区间内的每个元素执行 f() 操作，但不能改变元素内容
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
	// 用仿函数 gen 的运算结果对[first, last)内的每个元素赋值
	/*********************************************************************************/
	template <class ForwardIterator, class Generator>
	void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
		for (; first != last; ++first) {
			*first = gen();
		}
	}

	/*********************************************************************************/
	// generate_n
	// 用仿函数 gen 连续对 n 个元素赋值
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
	// 重载版本使用仿函数 comp 代替比较操作
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

	// 重载版本使用仿函数 comp 代替比较操作
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
	// max_element
	// 返回一个 iterator，指向序列中最大的元素
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class ForwardIterator>
	ForwardIterator max_elememt(ForwardIterator first, ForwardIterator last) {
		if (first == last)	return first;
		ForwardIterator result = first;
		while (++first != last) {
			if (*result < *first)	result = first;
		}
		return result;
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class ForwardIterator, class Compared>
	ForwardIterator max_elememt(ForwardIterator first, ForwardIterator last,
		Compared comp) {
		if (first == last)	return first;
		ForwardIterator result = first;
		while (++first != last) {
			if (comp(*result, *first))	result = first;
		}
		return result;
	}

	/*********************************************************************************/
	// min_element
	// 返回一个 iterator，指向序列中最小的元素
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class ForwardIterator>
	ForwardIterator min_elememt(ForwardIterator first, ForwardIterator last) {
		if (first == last)	return first;
		ForwardIterator result = first;
		while (++first != last) {
			if (*first < *result)	result = first;
		}
		return result;
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class ForwardIterator, class Compared>
	ForwardIterator max_elememt(ForwardIterator first, ForwardIterator last,
		Compared comp) {
		if (first == last)	return first;
		ForwardIterator result = first;
		while (++first != last) {
			if (comp(*first, *result))	result = first;
		}
		return result;
	}

	/*********************************************************************************/
	// merge
	// 将两个经过排序的集合 S1 和 S2 合并起来置于另一段空间，返回一个迭代器指向最后一个元素的下一位置
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result) {
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
		return copy(first2, last2, copy(first1, last1, result));
	}

	// 重载版本使用仿函数 comp 代替比较操作
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
		return copy(first2, last2, copy(first1, last1, result));
	}

	/*********************************************************************************/
	// partition
	// 对区间内的元素重排，被一元条件运算判定为 true 的元素会放到区间的前段
	// 该函数不保证元素的原始相对位置
	/*********************************************************************************/
	template <class BidirectionalIterator, class Predicate>
	BidirectionalIterator partition(BidirectionalIterator first,
		BidirectionalIterator last, Predicate pred) {
		while (true) {
			while (true) {
				if (first == last)	return first;
				else if (pred(*first))	++first;	//头指针所指元素符合 pred 为 true 的情况
				else break;
			}
			--last;		//尾指针回溯1
			while (true) {
				if (first == last)	return first;
				else if (!pred(*last))	--last;		//尾指针所指元素符合 pred 为 false 的情况
				else break;
			}
			iter_swap(first, last);				//交换头尾迭代器
			++first;	//头指针前进1，准备下一轮循环
		}
	}

	/*********************************************************************************/
	// remove
	// 移除所有与指定 value 相等的元素
	// 并不从容器中删除这些元素，所以 remove 和 remove_if 不适用于 array
	/*********************************************************************************/
	template <class ForwardIterator, class T>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
		first = find(first, last, value);	//利用find找出第一个匹配的地方
		ForwardIterator next = first;
		return first == last ? first : remove_copy(++next, last, first, value);
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
	// remove_if
	// 移除区间内所有令仿函数 pred 为 true 的元素
	/*********************************************************************************/
	template <class ForwardIterator, class Predicate>
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
		Predicate pred) {
		first = find_if(first, last, pred);	//利用find_if找出第一个匹配的地方
		ForwardIterator next = first;
		return first == last ? first : remove_copy_if(++next, last, first, pred);
	}

	/*********************************************************************************/
	// remove_copy_if
	// 移除区间内所有令仿函数 pred 为 true 的元素，并将结果复制到以 result 为起始位置的容器上
	/*********************************************************************************/
	template <class InputIterator, class OutputIterator, class Predicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last,
		OutputIterator result, Predicate pred) {
		for (; first != last; ++first) {
			if (!pred(*first)) {
				*result = *first;
				++result;
			}
		}
		return result;
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
	// replace_if
	// 将区间内所有令 pred 为 true 的元素都用 new_value 替代
	/*********************************************************************************/
	template <class ForwardIterator, class Predicate, class T>
	void replace(ForwardIterator first, ForwardIterator last, Predicate pred,
		const T& new_value) {
		for (; first != last; ++first) {
			if (pred(*first))	*first = new_value;
		}
	}

	/*********************************************************************************/
	// replace_copy_if
	// 行为与 replace_if 类型，不同的是将结果复制到 result 所指的容器中，原序列没有改变
	/*********************************************************************************/
	template <class InputIterator, class OutputIterator, class Predicate, class T>
	OutputIterator replace_copy(InputIterator first, InputIterator last,
		OutputIterator result, Predicate pred, const T& new_value) {
		for (; first != last; ++first, ++result) {
			*result = pred(*first) ? new_value : *first;
		}
		return result;
	}

	/*********************************************************************************/
	// reverse
	// 颠倒序列中的元素
	/*********************************************************************************/
	template <class BidirectionalIterator>
	inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
		//根据迭代器的不同，采用不同的版本
		__reverse(first, last, iterator_category(first));
	}

	// __reverse 的 bidirectional iterator 版本
	template <class BidirectionalIterator>
	void __reverse(BidirectionalIterator first, BidirectionalIterator last,
		bidirectional_iterator_tag) {
		while (true) {
			if (first == last || first == -last)
				return;
			else
				iter_swap(first++, last);
		}
	}

	// __reverse 的 random access iterator 版本
	template <class RandomAccessIterator>
	void __reverse(RandomAccessIterator first, RandomAccessIterator last,
		random_access_iterator_tag) {
		while (first < last)	iter_swap(first++, --last);
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
		//根据迭代器的不同，采用不同的版本
		__rotate(first, middle, last, distance_type(first), iterator_category(first));
	}

	// __rotate 的 forward iterator 版本
	template <class ForwardIterator, class Distance>
	void __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last,
		Distance*, forward_iterator_tag) {
		for (ForwardIterator i = middle; ;) {
			iter_swap(first, i);			//前段后段元素互换
			++first;
			++i;
			if (first == middle) {			//前半段结束
				if (i == last)	return;		//后半段也结束，交换完成
				middle = i;			//调整，对新的前后段进行交换
			}
			else if (i == last) {			//后半段结束
				i = middle;			//调整，对新的前后段进行交换
			}
		}
	}
	
	// __rotate 的 bidirectional iterator 版本
	template <class BidirectionalIterator, class Distance>
	void __rotate(BidirectionalIterator first, BidirectionalIterator middle,
		BidirectionalIterator last, Distance*, bidirectional_iterator_tag) {
		reverse(first, middle);		//先反转前半段
		reverse(middle, last);		//再反转后半段
		reverse(first, last);		//最后反转整个区间
	}

	// __rotate 的 random access iterator 版本
	template <class RandomAccessIterator, class Distance>
	void __rotate(RandomAccessIterator first, RandomAccessIterator middle,
		RandomAccessIterator last, Distance*, random_access_iterator_tag) {
		//因为是 random access iterator，我们可以确定每个元素的位置
		//取前半段和全长的最大公因子，即循环次数
		Distance n = __gcd(last - first, middle - first);
		while (n--) {
			// first + n : 起始位置
			// middle - first : 偏移量
			__rotate_cycle(first, last, first + n, middle - first, value_type(first));
		}
	}

	//最大公因子，辗转相除法
	template <class EuclideanRingElement>
	EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
		while (n != 0) {
			EuclideanRingElement t = m % n;
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
		RandomAccessIterator ptr1 = initial;
		RandomAccessIterator ptr2 = ptr1 + shift;
		while (ptr2 != initial) {
			*ptr1 = *ptr2;		//元素移动
			ptr1 = ptr2;
			if (last - ptr2 > shift)	//下次偏移未到达尾端
				ptr2 += shift;
			else				//下次偏移超出尾端
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
		return copy(first, middle, copy(middle, last, result));
	}

	/*********************************************************************************/
	// search
	// 在[first1, last1)中查找[first2, last2)的首次出现点
	// 重载版本使用仿函数 comp 代替比较操作
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
		Distance1 d1 = distance(first1, last1);
		Distance2 d2 = distance(first2, last2);
		if (d1 < d2)	return last1;	//若序列二长度大于序列一，不可能成为其子序列
		ForwardIterator1 current1 = first1;
		ForwardIterator2 current2 = first2;
		while (current2 != last2) {
			if (*current1 == *current2) {	//元素相等，比对下一个元素
				++current1;
				++current2;
			}
			else {		//元素不相等
				if (d1 == d2)	//如果两个序列等长，则序列二不可能成为序列一子序列
					return last1;
				else {
					current1 = ++first1;	//调整序列一的标兵
					current2 = first2;	//重新开始比对
					--d1;			//已经排除了序列一的一个元素，长度要减1
				}
			}
		}
		return first1;
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class ForwardIterator1, class ForwardIterator2, class Compared>
	inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2, Compared comp) {
		return __search(first1, last1, first2, last2, comp, distance_type(first1),
			distance_type(first2));
	}

	template <class ForwardIterator1, class ForwardIterator2, class Compared
		class Distance1, class Distance2>
		ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1,
			ForwardIterator2 first2, ForwardIterator2 last2, Compared comp,
			Distance1*, Distance2*) {
		Distance1 d1 = distance(first1, last1);
		Distance2 d2 = distance(first2, last2);
		if (d1 < d2)	return last1;	//若序列二长度大于序列一，不可能成为其子序列
		ForwardIterator1 current1 = first1;
		ForwardIterator2 current2 = first2;
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
					current2 = first2;	//重新开始比对
					--d1;			//已经排除了序列一的一个元素，长度要减1
				}
			}
		}
		return first1;
	}

	/*********************************************************************************/
	// search_n
	// 在[first, last)中查找连续 n 个 value 所形成的子序列，返回一个迭代器指向该子序列的起始处
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class ForwardIterator, class Count, class T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Count n, const T& value) {
		if (n <= 0)	return first;
		else {
			first = find(first, last, value);	//先找出第一个 value 出现的位置
			while (first != last) {
				Count m = n - 1;		//还需要查找 n - 1 次
				ForwardIterator i = first;	//从上次出现点开始找
				++i;
				while (i != last && n != 0 && *i == value) {
					++i;
					--n;
				}
				if (n == 0)		//value 出现了 n 次
					return first;
				else	//本次查找失败，重新从第一个位置开始找
					first = find(i, last, value);
			}
			return last;	//没找到
		}
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class ForwardIterator, class Count, class T, class Compared>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Count n, const T& value, Compared comp) {
		if (n <= 0)	return first;
		else {
			while (first != last) {	//先找出第一个 value 出现的位置
				if (comp(*first, value))	break;
				++first;
			}	
			while (first != last) {
				Count m = n - 1;		//还需要查找 n - 1 次
				ForwardIterator i = first;	//从上次出现点开始找
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
	// swap_ranges
	// 将[first1, last1)内的元素与从 first2 开始、个数相同的元素交换
	// 交换的区间长度必须相同，若成功返回一个迭代器指向序列二最后一个被交换元素的下一位置
	/*********************************************************************************/
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2) {
		for (; first1 != last1; ++first1, ++first2) {
			iter_swap(first1, first2);
		}
		return first2;
	}

	/*********************************************************************************/
	// transform
	// 第一个版本以仿函数 op 作用于[first, last)中的每个元素并将结果保存至 result 中
	// 第二个版本以仿函数 binary_op 作用于两个序列[first1, last1)、[first2, last2)的相同位置
	/*********************************************************************************/
	//版本一
	template <class InputIterator, class OutputIterator, class UnaryOperation>
	OutputIterator transform(InputIterator first, InputIterator last,
		OutputIterator result, UnaryOperation op) {
		for (; first != last; ++first, ++result) {
			*result = op(*first);
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
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class ForwardIterator>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
		first = adjacent_find(first, last);		//利用 adjacent_find 找到相邻重复元素的起点
		return unique_copy(first, last, first);
	}

	// 重载版本使用仿函数 comp 代替比较操作
	template <class ForwardIterator, class Compared>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last, Compared comp) {
		first = adjacent_find(first, last, comp);	//利用 adjacent_find 找到相邻重复元素的起点
		return unique_copy(first, last, first, comp);
	}

	/*********************************************************************************/
	// unique_copy
	// 从[first, last)中将元素复制到 result 上，序列必须有序，如果有重复的元素，只会复制一次
	// 重载版本使用仿函数 comp 代替比较操作
	/*********************************************************************************/
	template <class InputIterator, class OutputIterator>
	inline OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result) {
		if (first == last)	return result;
		//根据不同的 iterator 采用不同操作
		return __unique_copy(first, last, result, iterator_category(result));
	}
	
	// __unique_copy 的 forward iterator 版本
	template <class InputIterator, class ForwardIterator>
	ForwardIterator __unique_copy(InputIterator first, InputIterator last,
		ForwardIterator result, forward_iterator_tag) {
		*result = *first;	//记录第一个元素
		while (++first != last) {
			if (*result != *first)	*++result = *first;	//元素不同就复制
		}
		return ++result;
	}

	// __unique_copy 的 output iterator 版本
	template <class InputIterator, class OutputIterator>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, output_iterator_tag) {
		//由于 output iterator 的限制，必须先知道其value type
		return __unique_copy(first, last, result, value_type(first));
	}

	//由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
	template <class InputIterator, class OutputIterator, class T>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
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

	// 重载版本使用仿函数 comp 代替比较操作
	template <class InputIterator, class OutputIterator, class Compared>
	inline OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, Compared comp) {
		if (first == last)	return result;
		//根据不同的 iterator 采用不同操作
		return __unique_copy(first, last, result, comp, iterator_category(result));
	}

	// __unique_copy 的 forward iterator 版本
	template <class InputIterator, class ForwardIterator, class Compared>
	ForwardIterator __unique_copy(InputIterator first, InputIterator last,
		ForwardIterator result, Compared comp, forward_iterator_tag) {
		*result = *first;	//记录第一个元素
		while (++first != last) {
			if (!comp(*result, *first))	*++result = *first;	//元素不同就复制
		}
		return ++result;
	}

	// __unique_copy 的 output iterator 版本
	template <class InputIterator, class OutputIterator, class Compared>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, Compared comp, output_iterator_tag) {
		//由于 output iterator 的限制，必须先知道其value type
		return __unique_copy(first, last, result, comp, value_type(first));
	}

	//由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
	template <class InputIterator, class OutputIterator, class Compared, class T>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
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
}
#endif // !ALGO_H
