#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "algo.h"
#include "vector.h"
#include "memory.h"
#include "functional.h"
#include "hash_functional.h"

namespace MyTinySTL {

	// hashtable 使用开链法表现，以下是节点定义
	template <class T>
	struct __hashtable_node {
		__hashtable_node* next;	//指向下一个节点
		T value;	//储存实值
	};

	template <class Val, class Key, class HashFun,
		class ExtractKey, class EqualKey, class Alloc = alloc>
		class hashtable;

	template <class Val, class Key, class HashFun,
		class ExtractKey, class EqualKey, class Alloc = alloc>
		struct __hashtable_iterator;

	template <class Val, class Key, class HashFun,
		class ExtractKey, class EqualKey, class Alloc = alloc>
		struct __hashtable_const_iterator;

	// __hashtable_iterator 定义
	template <class Val, class Key, class HashFcn,
		class ExtractKey, class EqualKey, class Alloc = alloc>
		struct __hashtable_iterator {
		typedef hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			hashtable;
		typedef __hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			iterator;
		typedef __hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			const_iterator;
		typedef __hashtable_node<Val>	node;

		// iterator 的内嵌型别
		typedef forward_iterator_tag	iterator_category;
		typedef Val	value_type;
		typedef Val*	pointer;
		typedef Val&	reference;
		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;

		node* cur;	//迭代器目前所指节点
		hashtable* ht;	//保持与容器的连结

		// 构造函数
		__hashtable_iterator(node* n, hashtable* tab) :cur(n), ht(tab) {}
		__hashtable_iterator() {}

		// 重载操作符
		reference operator*() const { return cur->value; }
		pointer operator->() const { return &(operator*()); }
		iterator& operator++();
		iterator operator++(int) {
			iterator tmp = *this;
			++*this;
			return tmp;
		}
		bool operator==(const iterator& rhs) const { return cur == rhs.cur; }
		bool operator!=(const iterator& rhs) const { return cur != rhs.cur; }
	};

	// iterator 的 operator++ 操作
	template <class Val, class Key, class HashFcn,
		class ExtractKey, class EqualKey, class Alloc = alloc>
		__hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
		__hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
		const node* old = cur;	//原来的节点
		cur = cur->next;
		if (!cur) {	//如果 cur 为 NULL，跳到下一个 bucket 的起始处
			size_type bucket = ht->get_index(old->value);	//找到下一个 bucket 的位置
			while (!cur && ++bucket < ht->get_buckets().size())
				cur = ht->get_buckets()[bucket];
		}
		return *this;
	}

	// const_iterator 定义
	template <class Val, class Key, class HashFcn,
		class ExtractKey, class EqualKey, class Alloc = alloc>
		struct __hashtable_const_iterator {
		typedef hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			hashtable;
		typedef __hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			iterator;
		typedef __hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			const_iterator;
		typedef __hashtable_node<Val>	node;

		// const_iterator 的内嵌型别
		typedef forward_iterator_tag	iterator_category;
		typedef Val	value_type;
		typedef const Val*	pointer;
		typedef const Val&	reference;
		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;

		const node* cur;	//迭代器目前所指节点
		const hashtable* ht;	//保持与容器的连结

		// 构造函数
		__hashtable_const_iterator(const node* n, const hashtable* tab)
			:cur(n), ht(tab) {}
		__hashtable_const_iterator() {}
		__hashtable_const_iterator(const iterator& rhs)
			:cur(rhs.cur), ht(rhs.ht) {}

		// 重载操作符
		reference operator*() const { return cur->value; }
		pointer operator->() const { return &(operator*()); }
		const_iterator& operator++();
		const_iterator operator++(int) {
			const_iterator tmp = *this;
			++*this;
			return tmp;
		}
		bool operator==(const const_iterator& rhs) const { return cur == rhs.cur; }
		bool operator!=(const const_iterator& rhs) const { return cur != rhs.cur; }
	};

	// const_iterator 的 operator++ 操作
	template <class Val, class Key, class HashFcn,
		class ExtractKey, class EqualKey, class Alloc = alloc>
	__hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
		__hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
		const node* old = cur;	//原来的节点
		cur = cur->next;
		if (!cur) {	//如果 cur 为 NULL，跳到下一个 bucket 的起始处
			size_type bucket = ht->get_index(old->value);	//找到下一个 bucket 的位置
			while (!cur && ++bucket < ht->get_buckets().size())
				cur = ht->get_buckets()[bucket];
		}
		return *this;
	}

	// 开链法用28个质数来设计表格大小
	static const int __prime_nums = 28;
	static const unsigned long __prime_list[__prime_nums] = {
		53ul,			97ul,			193ul,			389ul,			769ul, 
		1543ul,			3079ul,			6151ul,			12289ul,		24593ul,
		49157ul,		98317ul,		196613ul,		393241ul,		786433ul, 
		1572869ul,		3145739ul,		6291469ul,		12582917ul,		25165843ul, 
		50331653ul,		100663319ul,	201326611ul,	402653189ul,	805306457ul,
		1610612741ul,	3221225473ul,	4294967291ul
	};

	// 以下函数用于找出最接近并大于等于 n 的那个质数
	inline unsigned long __next_prime(unsigned long n) {
		const unsigned long* first = __prime_list;
		const unsigned long* last = __prime_list + __prime_nums;
		const unsigned long* pos = MyTinySTL::lower_bound(first, last, n);
		return pos == last ? *(last - 1) : *pos;
	}

	// hashtable 模板类定义
	template <class Val, class Key, class HashFcn,
		class ExtractKey = MyTinySTL::identity<Key>, 
		class EqualKey = MyTinySTL::equal_to<Key>, class Alloc = alloc>
	class hashtable {
	public:
		// hashtable 型别定义
		typedef Val	value_type;
		typedef Key	key_type;
		typedef HashFcn	hasher;
		typedef EqualKey	key_equal;

		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;
		typedef Val*	pointer;
		typedef const Val*	const_pointer;
		typedef Val&	reference;
		typedef	const Val&	const_reference;

		hasher hash_fcn() const { return hash; }	//获取 hash function 的型别
		key_equal key_eq() const { return equals; }	//获取从节点中取出键值的方法

	private:
		typedef __hashtable_node<Val>	node;	// hashtable 的节点
		typedef vector<node*, Alloc>	bucket_type;
	public:
		// hashtable 的迭代器型别
		typedef __hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			iterator;
		typedef __hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			const_iterator;

		// hashtable 的空间配置器型别
		typedef Alloc	allocator_type;
		allocator_type get_allocator() const { return allocator_type(); }

	private:
		// 配置节点相关
		typedef allocator<node, Alloc>	node_allocator;
		node* __get_node() { return node_allocator::allocate(1); }
		void __put_node(node* p) { node_allocator::deallocate(p, 1); }

	private:
		// 以下参数表现 hashtable
		hasher hash;
		key_equal equals;
		ExtractKey get_key;
		size_type element_nums;
		bucket_type buckets;	//以 vector 完成

	public:
		bucket_type& get_buckets() { return buckets; }
		const bucket_type& get_buckets() const { return buckets; }

	public:
		// 构造，复制，析构函数
		hashtable(size_type n, const HashFcn& hf = HashFcn(), 
			const EqualKey& eql = EqualKey(),
			const ExtractKey ext = ExtractKey())
			:hash(hf), equals(eql), get_key(ext), element_nums(0) {
			__initialize_buckets(n);
		}

		hashtable(const hashtable& ht)
			:hash(ht.hash), equals(ht.equals), get_key(ht.get_key), element_nums(0) {
			__copy_from(ht);
		}
		
		hashtable& operator=(const hashtable& ht);

		~hashtable() { clear(); }

		// 迭代器相关操作
		iterator begin();
		const_iterator begin() const;
		iterator end() { return iterator((node*)0, this); }
		const_iterator end() const { return const_iterator((node*)0, this); }

		// 容量相关操作
		bool empty() const { return size() == 0; }
		size_type size() const { return element_nums; }
		size_type max_size() const { return size_type(-1); }
		size_type bucket_count() const { return buckets.size(); }
		size_type max_bucket_count() const { return __prime_list[__prime_nums - 1]; }
		size_type elems_in_bucket(size_type n) const;

		// 调整容器相关操作
		pair<iterator, bool> insert_unique(const value_type& value) {
			resize(element_nums + 1);
			return insert_unique_noresize(value);
		}
		iterator insert_equal(const value_type& value) {
			resize(element_nums + 1);
			return insert_equal_noresize(value);
		}
		pair<iterator, bool> insert_unique_noresize(const value_type& value);
		iterator insert_equal_noresize(const value_type& value);

		template <class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			insert_unique(first, last, iterator_category(first));
		}
		template <class InputIterator>
		void insert_equal(InputIterator first, InputIterator last) {
			insert_equal(first, last, iterator_category(first));
		}
		template <class InputIterator>
		void insert_unique(InputIterator first, InputIterator last, input_iterator_tag);
		template <class InputIterator>
		void insert_equal(InputIterator first, InputIterator last, input_iterator_tag);
		template <class ForwardIterator>
		void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
		template <class ForwardIterator>
		void insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

		size_type erase(const key_type& k);
		void erase(const iterator& it);
		void erase(iterator first, iterator last);
		void erase(const const_iterator& it);
		void erase(const_iterator first, const_iterator last);
		void clear();

		void resize(size_type num_elements_hint);

		// hashtable 相关操作
		reference find_or_insert(const value_type& value);
		iterator find(const key_type& k);
		const_iterator find(const key_type& k) const;
		size_type count(const key_type& k) const;
		size_type get_index(const value_type& value) const { return __bkt_num(value); }

		pair<iterator, iterator> equal_range(const key_type& k);
		pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
		void swap(hashtable& rhs);

	private:
		// 内部成员函数
		size_type __next_size(size_type n) const;
		void __initialize_buckets(size_type n);
		size_type __bkt_num(const value_type& value, size_type n) const;
		size_type __bkt_num(const value_type& value) const;
		size_type __bkt_num_key(const key_type& key, size_type n) const;
		size_type __bkt_num_key(const key_type& key) const;
		node* __new_node(const value_type& value);
		void __delete_node(node* n);
		void __erase_bucket(const size_type n, node* first, node* last);
		void __erase_bucket(const size_type n, node* last);
		void __copy_from(const hashtable& ht);
	};

	/********************************************************************************/

	// 赋值操作符 operator=
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator=(const hashtable & ht) {
		if (this != &ht) {
			clear();
			hash = ht.hash;
			equals = ht.equals;
			get_key = ht.get_key;
			__copy_from(ht);
		}
		return *this;
	}

	// begin() 返回指向第一个节点的迭代器
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() {
		for (size_type n = 0; n < buckets.size(); ++n) {
			if (buckets[n])	//找到第一个有节点的位置就返回
				return iterator(buckets[n], this);
		}
		return end();	// 没有找到返回 end()
	}

	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() const {
		for (size_type n = 0; n < buckets.size(); ++n) {
			if (buckets[n])	//找到第一个有节点的位置就返回
				return const_iterator(buckets[n], this);
		}
		return end();	// 没有找到返回 end()
	}

	// 在某个位置节点的个数
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::elems_in_bucket(size_type n) const {
		size_type result = 0;
		for (node* cur = buckets[n]; cur; cur = cur->next)
			result++;
		return result;
	}

	// 在不需要重建表格的情况下插入新节点，键值不允许重复
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool> 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		insert_unique_noresize(const value_type& value) {
		const size_type n = __bkt_num(value);	// value 位于第 n 个 bucket
		node* first = buckets[n];	// first 指向 第 n 个 bucket 的头部
		// 如果 bucket[n] 被占用，first不为0，进入循环，走访链表
		for (node* cur = first; cur; cur = cur->next)
			if (equals(get_key(cur->value), get_key(value)))	//如果链表中存在相同键值的节点就返回
				return pair<iterator, bool>(iterator(cur, this), false);
		node* tmp = __new_node(value);	//产生新节点
		tmp->next = first;
		buckets[n] = tmp;	//让新节点成为链表的第一个节点
		++element_nums;
		return pair<iterator, bool>(iterator(tmp, this), true);
	}

	// 在不需要重建表格的情况下插入新节点，键值允许重复
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		insert_equal_noresize(const value_type& value) {
		const size_type n = __bkt_num(value);	// value 位于第 n 个 bucket
		node* first = buckets[n];	// first 指向 第 n 个 bucket 的头部
		// 如果 bucket[n] 被占用，first不为0，进入循环，走访链表
		for (node* cur = first; cur; cur = cur->next)
			if (equals(get_key(cur->value), get_key(value))) {
				// 如果链表中存在相同键值的节点就马上插入，然后返回
				node* tmp = __new_node(value);	//产生新节点
				tmp->next = cur->next;	//将新节点插入到目标节点后
				cur->next = tmp;
				++element_nums;
				return iterator(tmp, this);
			}
		node* tmp = __new_node(value);	//产生新节点
		tmp->next = first;
		buckets[n] = tmp;	//让新节点成为链表的第一个节点
		++element_nums;
		return iterator(tmp, this);	
	}

	// insert_unique 的 input_iterator_tag 版本
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	template<class InputIterator>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
		for (; first != last; ++first)
			insert_unique(*first);	//一个一个插入
	}

	// insert_equal 的 input_iterator_tag 版本
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	template<class InputIterator>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		insert_equal(InputIterator first, InputIterator last, input_iterator_tag) {
		for (; first != last; ++first)
			insert_equal(*first);	//一个一个插入
	}

	// insert_unique 的 forward_iterator_tag 版本
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	template<class ForwardIterator>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
		size_type n = distance(first, last);	
		resize(element_nums + n);	//调整大小
		for (; n > 0; --n, ++first)
			insert_unique_noresize(*first);
	}

	// insert_equal 的 forward_iterator_tag 版本
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	template<class ForwardIterator>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
		size_type n = distance(first, last);
		resize(element_nums + n);	//调整大小
		for (; n > 0; --n, ++first)
			insert_equal_noresize(*first);
	}

	// 删除键值为 key 的节点
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		erase(const key_type & k) {
		const size_type n = __bkt_num_key(k);	// 键值为 k 的节点位于第 n 个 bucket
		node* first = buckets[n];	// first 指向第 n 个 bucket 的头部
		size_type erased = 0;	//已删除的元素个数
		if (first) {
			node* cur = first;
			node* next = cur->next;
			while (next) {	//如果 next 不为 NULL
				if (equals(get_key(next->value), k)) {	//如果找到键值为 k 的节点
					cur->next = next->next;	
					__delete_node(next);
					next = cur->next;
					++erased;
					--element_nums;
				}
				else {
					cur = next;
					next = cur->next;
				}
			}
			if (equals(get_key(first->value), k)) {	//如果第一个节点键值为 k
				buckets[n] = first->next;
				__delete_node(first);
				++erased;
				--element_nums;
			}
		}
		return erased;
	}

	// 删除迭代器所指的节点
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const iterator& it) {
		node* p = it.cur;
		if (p) {
			const size_type	n = __bkt_num(p->value);
			node* cur = buckets[n];
			if (cur == p) {	// p 位于链表头部
				buckets[n] = cur->next;
				__delete_node(cur);
				--element_nums;
			}
			else {
				node* next = cur->next;
				while (next) {	//当没到链表尾部
					if (next == p) {	//如果找到 p
						cur->next = next->next;
						__delete_node(next);
						--element_nums;
						break;
					}
					else {
						cur = next;
						next = cur->next;
					}
				}
			}
		}
	}

	// 删除[first, last)内的节点
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		erase(iterator first, iterator last) {
		size_type first_bucket = first.cur ? __bkt_num(first.cur->value) : buckets.size();
		size_type last_bucket = last.cur ? __bkt_num(last.cur->value) : buckets.size();
		if (first.cur == last.cur)
			return;
		else if (first_bucket == last_bucket)	//如果在 bucket 在同一个位置
			__erase_bucket(first_bucket, first.cur, last.cur);
		else {
			__erase_bucket(first_bucket, first.cur, 0);	//删除 first_bucket
			for (size_type n = first_bucket + 1; n < last_bucket; ++n)
				__erase_bucket(n, 0);	//删除 first 和 last 中间的 bucket
			if (last_bucket != buckets.size())
				__erase_bucket(last_bucket, last.cur);	//删除剩余部分
		}
	}

	// 删除迭代器所指的节点
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	inline void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		erase(const const_iterator& it) {
		// 用 const_cast 作常量性消除
		erase(iterator(const_cast<node*>(it.cur), const_cast<hashtable*>(it.ht)));
	}

	// 删除[first, last)内的节点
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	inline void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		erase(const_iterator first, const_iterator last) {
		// 用 const_cast 作常量性消除
		erase(iterator(const_cast<node*>(first.cur), const_cast<hashtable*>(first.ht)),
			iterator(const_cast<node*>(last.cur), const_cast<hashtable*>(last.ht)));
	}

	// 清空 hashtable
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear() {
		for (size_type i = 0; i < buckets.size(); ++i) {
			node* cur = buckets[i];
			while (cur != 0) {
				node* next = cur->next;
				__delete_node(cur);	// 删除所有节点
				cur = next;
			}
			buckets[i] = 0;
		}
		element_nums = 0;
	}

	// 重新配置 hashtable 大小
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		resize(size_type num_elements_hint) {
		// 若新增元素后，元素个数大于 bucket vector 的大小，就重建表格
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n) {	//如果需要重建
			const size_type n = __next_size(num_elements_hint);	//找到下一个质数
			if (n > old_n) {
				vector<node*, Alloc> tmp(n, (node*)0);	//建立新的 buckets
				try {
					for (size_type bucket = 0; bucket < old_n; ++bucket) {
						node* first = buckets[bucket];	
						while (first) {
							size_type new_bucket = __bkt_num(first->value, n);
							buckets[bucket] = first->next;	//旧的 buckets 指向下一个节点
							first->next = tmp[new_bucket];	//当前节点插入到新的 bucket
							tmp[new_bucket] = first;	//使当前节点成为新 bucket 的第一个节点
							first = buckets[bucket];
						}
					}
					buckets.swap(tmp);	//和新的 bucket 交换
				}
				catch (...) {
					for (size_type bucket = 0; bucket < tmp.size(); ++bucket) {
						while (tmp[bucket]) {
							node* next = tmp[bucket]->next;
							__delete_node(tmp[bucket]);
							tmp[bucket] = next;
						}
					}
					throw;
				}
			}
		}
	}

	// 如果找到实值为 value 的元素，就返回它，否则就插入新元素后再返回
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::reference 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		find_or_insert(const value_type & value) {
		resize(element_nums + 1);
		size_type n = __bkt_num(value);
		node* first = buckets[n];
		for (node* cur = first; cur; cur = cur->next)
			if (equals(get_key(cur->value), get_key(value)))
				return cur->value;
		node* tmp = __new_node(value);	//新建节点
		tmp->next = first;
		buckets[n] = tmp;	//让新节点成为链表的第一个节点
		++element_nums;
		return tmp->value;
	}

	// 查找键值为 k 的节点，返回其迭代器
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		find(const key_type& k) {
		size_type n = __bkt_num_key(k);
		node* first = buckets[n];
		for (; first && !equals(get_key(first->value), k); first = first->next) {}
		return iterator(first, this);
	}

	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		find(const key_type& k) const {
		size_type n = __bkt_num_key(k);
		node* first = buckets[n];
		for (; first && !equals(get_key(first->value), k); first = first->next) {}
		return const_iterator(first, this);
	}

	// 查找键值为 k 出现的次数
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		count(const key_type& k) const {
		const size_type n = __bkt_num_key(k);
		size_type result = 0;
		for (const node* cur = buckets[n]; cur; cur = cur->next) {
			if (equals(get_key(cur->value), k))
				++result;
		}
		return result;
	}

	// 查找与键值 k 相等的区间，返回一个 pair
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator,
		typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator>
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& k) {
		typedef pair<iterator, iterator>	pii;
		const size_type n = __bkt_num_key(k);
		for (node* first = buckets[n]; first; first = first->next) {
			if (equals(get_key(first->value), k)) {	//如果出现相等的键值
				for (node* cur = first->next; cur; cur = cur->next)
					if (!equals(get_key(cur->value), k))	//如果出现不相等的键值
						return pii(iterator(first, this), iterator(cur, this));
				for (size_type m = n + 1; m < buckets.size(); ++m)	//整个链表都相等，查找下一个链表出现的位置
					if (buckets[m])
						return pii(iterator(first, this), iterator(buckets[m], this));
				return pii(iterator(first, this), end());
			}
		}
		// 没有找到
		return pii(end(), end());
	}

	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator,
		typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator>
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& k) const {
		typedef pair<const_iterator, const_iterator>	pii;
		const size_type n = __bkt_num_key(k);
		for (const node* first = buckets[n]; first; first = first->next) {
			if (equals(get_key(first->value), k)) {	//如果出现相等的键值
				for (const node* cur = first->next; cur; cur = cur->next)
					if (!equals(get_key(cur->value), k))	//如果出现不相等的键值
						return pii(const_iterator(first, this), const_iterator(cur, this));
				for (size_type m = n + 1; m < buckets.size(); ++m)	//整个链表都相等，查找下一个链表出现的位置
					if (buckets[m])
						return pii(const_iterator(first, this), const_iterator(buckets[m], this));
				return pii(const_iterator(first, this), end());
			}
		}
		// 没有找到
		return pii(end(), end());
	}

	// 交换 hashtable
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::swap(hashtable& rhs) {
		MyTinySTL::swap(hash, rhs.hash);
		MyTinySTL::swap(equals, rhs.equals);
		MyTinySTL::swap(get_key, rhs.get_key);
		buckets.swap(rhs.buckets);
		MyTinySTL::swap(element_nums, rhs.element_nums);
	}

	// __next_size 函数
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::__next_size(size_type n) const {
		return __next_prime(n);
	}

	// __initialize_buckets 
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		__initialize_buckets(size_type n) {
		const size_type n_buckets = __next_size(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);
		element_nums = 0;
	}

	// __bkt_num 函数
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		__bkt_num(const value_type & value, size_type n) const {
		return __bkt_num_key(get_key(value), n);	
	}

	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		__bkt_num(const value_type & value) const {
		return __bkt_num_key(get_key(value));
	}

	// __bkt_num_key 函数
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		__bkt_num_key(const key_type & key, size_type n) const {
		return hash(key) % n;
	}

	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		__bkt_num_key(const key_type & key) const {
		return __bkt_num_key(key, buckets.size());
	}

	// __new_node 函数
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node* 
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::__new_node(const value_type & value) {
		node* n = __get_node();
		n->next = 0;
		try {
			MyTinySTL::construct(&n->value, value);
			return n;
		}
		catch (...) {
			__put_node(n);
		}
	}

	// __delete_node 函数
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::__delete_node(node* n) {
		MyTinySTL::destroy(&n->value);
		__put_node(n);
	}

	// __erase_bucket 函数
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		__erase_bucket(const size_type n, node * first, node * last) {
		node* cur = buckets[n];
		if (cur == first)
			__erase_bucket(n, last);
		else {
			node* next;
			for (next = cur->next; next != first; cur = next, next = cur->next);	//找到first
			while (next != last) {
				cur->next = next->next;
				__delete_node(cur);
				next = cur->next;
				--element_nums;
			}
		}
	}

	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		__erase_bucket(const size_type n, node * last) {
		node* cur = buckets[n];
		while (cur != last) {
			node* next = cur->next;
			__delete_node(cur);
			cur = next;
			buckets[n] = cur;
			--element_nums;
		}
	}

	// __copy_from 函数
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		__copy_from(const hashtable & ht) {
		buckets.clear();
		buckets.resize(ht.buckets.size());
		buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);
		try {
			for (size_type i = 0; i < ht.buckets.size(); ++i) {
				const node* cur = ht.buckets[i];
				if (cur) {	//如果某位置存在链表
					node* copy = __new_node(cur->value);
					buckets[i] = copy;
					for(node* next =cur->next;next;cur=next,next=cur->next){	//复制链表
						copy->next = __new_node(next->value);
						copy = copy->next;
					}
				}
			}
			element_nums = ht.element_nums;
		}
		catch (...) {
			clear();
		}
	}

	// 重载 operator==
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	bool operator==(const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
			const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs) {
		typedef typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node node;
		if (lhs.buckets.size() != rhs.buckets.size())
			return false;
		for (int n = 0; n < lhs.buckets.size(); ++n) {
			node* cur1 = lhs.buckets[n];
			node* cur2 = rhs.buckets[n];
			for (; cur1 && cur2 && cur1->value == cur2->value;
				cur1 = cur1->next, cur2 = cur2->next) {
			}
			if (cur1 || cur2)	// cur1 或 cur2 还有元素
				return false;
		}
		return true;
	}

	// 重载 MyTinySTL 的 swap
	template<class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
	void swap(hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
		hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}
#endif // !HASHTABLE_H

