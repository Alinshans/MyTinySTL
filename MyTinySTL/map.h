#ifndef MAP_H
#define MAP_H

#include "rb_tree.h"

namespace MyTinySTL {

	// map:每个元素都是 pair，拥有键值与实值，map 不允许有重复的键值，使用 RB-tree 作为底层机制
	template <class Key, class T, class Compare = MyTinySTL::less<Key>, class Alloc = alloc>
	class map {
	public:
		// map 的型别定义
		typedef Key	key_type;
		typedef T	data_type;
		typedef T	mapped_type;
		typedef pair<const Key, T>	value_type;	// 元素型别是一对 pair，含键值/实值
		typedef Compare key_compare;

		// 定义一个 functor，用来进行元素比较
		class value_compare : public binary_function <value_type, value_type, bool> {
			friend class map<Key, T, Compare, Alloc>;
		protected:
			Compare comp;
			value_compare(Compare c) : comp(c) {}
		public:
			bool operator()(const value_type& x, const value_type& y) const {
				return comp(x.first, y.first);
			}
		};

	private:
		// 用 map 元素型别的第一型别作为 RB-tree 节点的键值型别
		typedef rb_tree<key_type, value_type,
			MyTinySTL::selectfirst<value_type>, key_compare, Alloc>	rep_type;
		rep_type t;	//用 RB-tree 表现 map

	public:
		// 使用 RB-tree 的型别
		typedef typename rep_type::pointer	pointer;
		typedef typename rep_type::const_pointer	const_pointer;
		typedef typename rep_type::reference	reference;
		typedef typename rep_type::const_reference	const_reference;
		typedef typename rep_type::iterator	iterator;
		// iterator 定义为 RB-tree 的 iterator，因为 map 允许修改元素的实值
		typedef typename rep_type::const_iterator	const_iterator;
		typedef typename rep_type::reverse_iterator	reverse_iterator;
		typedef typename rep_type::const_reverse_iterator	const_reverse_iterator;
		typedef typename rep_type::size_type	size_type;
		typedef typename rep_type::difference_type	difference_type;

	public:
		// 构造、复制函数
		map() :t() {}
		template <class InputIterator>
		map(InputIterator first, InputIterator last) : t() { t.insert_unique(first, last); }
		map(const map& x) :t(x.t) {}
		map& operator=(const map& x) { t = x.t; return *this; }

		// 相关接口操作
		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return value_compare(t.key_comp()); }
		iterator begin() { return t.begin(); }
		const_iterator begin() const { return t.begin(); }
		iterator end() { return t.end(); }
		const_iterator end() const { return t.end(); }
		reverse_iterator rbegin() { return t.rbegin(); }
		const_reverse_iterator rbegin() const { return t.rbegin(); }
		reverse_iterator rend() { return t.rend(); }
		const_reverse_iterator rend() const { return t.rend(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }
		T& operator[](const key_type& k) {
			iterator i = lower_bound(k);	// 找到第一个键值大于等于 k 的
			if (i == end() || key_comp()(k, (*i).first))
				i = insert(i, value_type(k, T()));
			return (*i).second;
		}

		// 插入删除操作
		// 插入操作使用的是 RB-tree 的 insert_unique
		pair<iterator, bool> insert(const value_type& x) { return t.insert_unique(x); }
		iterator insert(iterator position, const value_type& x) {
			return t.insert_unique(position, x);
		}
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			t.insert_unique(first, last);
		}
		void erase(iterator position) { t.erase(position); }
		size_type erase(const key_type& x) { return t.erase(x); }
		void erase(iterator first, iterator last) { t.erase(first, last); }
		void clear() { t.clear(); }

		// map 操作
		iterator find(const key_type& x) { return t.find(x); }
		const_iterator find(const key_type& x) const { return t.find(x); }
		size_type count(const key_type& x) const { return t.count(x); }
		iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
		const_iterator lower_bound(const key_type& x) const {
			return t.lower_bound(x);
		}
		iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
		const_iterator upper_bound(const key_type& x) const {
			return t.upper_bound(x);
		}
		pair<iterator, iterator> equal_range(const key_type& x) { return t.equal_range(x); }
		pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
			return t.equal_range(x);
		}
		void swap(map& x) { t.swap(x.t); }

	public:
		friend bool operator==(const map& x, const map& y) { return x.t == y.t; }
		friend bool operator<(const map& x, const map& y) { return x.t < y.t; }
	};

	// 重载比较操作符
	template <class Key, class T, class Compare, class Alloc>
	inline bool operator==(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
		return x == y;
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator<(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
		return x < y;
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator!=(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
		return !(x == y);
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator>(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
		return y < x;
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator<=(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
		return !(y < x);
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator>=(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
		return !(x < y);
	}

	// 重载 MyTinySTL 的 swap
	template <class Key, class T, class Compare, class Alloc>
	void swap(map<Key, T, Compare, Alloc>& x, map<Key, T, Compare, Alloc>& y) {
		x.swap(y);
	}

	/***********************************************************************************/

	// multimap:允许键值重复的 map
	template <class Key, class T, class Compare = MyTinySTL::less<Key>, class Alloc = alloc>
	class multimap {
	public:
		// multimap 的型别定义
		typedef Key	key_type;
		typedef T	data_type;
		typedef T	mapped_type;
		typedef pair<const Key, T>	value_type;	// 元素型别是一对 pair，含键值/实值
		typedef Compare key_compare;

		// 定义一个 functor，用来进行元素比较
		class value_compare : public binary_function <value_type, value_type, bool> {
			friend class multimap<Key, T, Compare, Alloc>;
		protected:
			Compare comp;
			value_compare(Compare c) : comp(c) {}
		public:
			bool operator()(const value_type& x, const value_type& y) const {
				return comp(x.first, y.first);
			}
		};

	private:
		// 用 multimap 元素型别的第一型别作为 RB-tree 节点的键值型别
		typedef rb_tree<key_type, value_type,
			MyTinySTL::selectfirst<value_type>, key_compare, Alloc>	rep_type;
		rep_type t;	//用 RB-tree 表现 map

	public:
		// 使用 RB-tree 的型别
		typedef typename rep_type::pointer	pointer;
		typedef typename rep_type::const_pointer	const_pointer;
		typedef typename rep_type::reference	reference;
		typedef typename rep_type::const_reference	const_reference;
		typedef typename rep_type::iterator	iterator;
		// iterator 定义为 RB-tree 的 iterator，因为 multimap 允许修改元素的实值
		typedef typename rep_type::const_iterator	const_iterator;
		typedef typename rep_type::reverse_iterator	reverse_iterator;
		typedef typename rep_type::const_reverse_iterator	const_reverse_iterator;
		typedef typename rep_type::size_type	size_type;
		typedef typename rep_type::difference_type	difference_type;

	public:
		// 构造、复制函数
		multimap() :t() {}
		template <class InputIterator>
		multimap(InputIterator first, InputIterator last) : t() { t.insert_equal(first, last); }
		multimap(const multimap& x) :t(x.t) {}
		multimap& operator=(const multimap& x) { t = x.t; return *this; }

		// 相关接口操作
		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return value_compare(t.key_comp()); }
		iterator begin() { return t.begin(); }
		const_iterator begin() const { return t.begin(); }
		iterator end() { return t.end(); }
		const_iterator end() const { return t.end(); }
		reverse_iterator rbegin() { return t.rbegin(); }
		const_reverse_iterator rbegin() const { return t.rbegin(); }
		reverse_iterator rend() { return t.rend(); }
		const_reverse_iterator rend() const { return t.rend(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }
		T& operator[](const key_type& k) {
			iterator i = lower_bound(k);	// 找到第一个键值大于等于 k 的
			if (i == end() || key_comp()(k, (*i).first))
				i = insert(i, value_type(k, T()));
			return (*i).second;
		}

		// 插入删除操作
		// 插入操作使用的是 RB-tree 的 insert_equal
		pair<iterator, bool> insert(const value_type& x) { return t.insert_equal(x); }
		iterator insert(iterator position, const value_type& x) {
			return t.insert_equal(position, x);
		}
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			t.insert_equal(first, last);
		}
		void erase(iterator position) { t.erase(position); }
		size_type erase(const key_type& x) { return t.erase(x); }
		void erase(iterator first, iterator last) { t.erase(first, last); }
		void clear() { t.clear(); }

		// multimap 操作
		iterator find(const key_type& x) { return t.find(x); }
		const_iterator find(const key_type& x) const { return t.find(x); }
		size_type count(const key_type& x) const { return t.count(x); }
		iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
		const_iterator lower_bound(const key_type& x) const {
			return t.lower_bound(x);
		}
		iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
		const_iterator upper_bound(const key_type& x) const {
			return t.upper_bound(x);
		}
		pair<iterator, iterator> equal_range(const key_type& x) { return t.equal_range(x); }
		pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
			return t.equal_range(x);
		}
		void swap(multimap& x) { t.swap(x.t); }

	public:
		friend bool operator==(const multimap& x, const multimap& y) { return x.t == y.t; }
		friend bool operator<(const multimap& x, const multimap& y) { return x.t < y.t; }
	};

	// 重载比较操作符
	template <class Key, class T, class Compare, class Alloc>
	inline bool operator==(const multimap<Key, T, Compare, Alloc>& x,
		const multimap<Key, T, Compare, Alloc>& y) {
		return x == y;
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator<(const multimap<Key, T, Compare, Alloc>& x, 
		multimap<Key, T, Compare, Alloc>& y) {
		return x < y;
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator!=(const multimap<Key, T, Compare, Alloc>& x,
		const multimap<Key, T, Compare, Alloc>& y) {
		return !(x == y);
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator>(const multimap<Key, T, Compare, Alloc>& x, 
		const multimap<Key, T, Compare, Alloc>& y) {
		return y < x;
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator<=(const multimap<Key, T, Compare, Alloc>& x, 
		const multimap<Key, T, Compare, Alloc>& y) {
		return !(y < x);
	}

	template <class Key, class T, class Compare, class Alloc>
	inline bool operator>=(const multimap<Key, T, Compare, Alloc>& x, 
		const multimap<Key, T, Compare, Alloc>& y) {
		return !(x < y);
	}

	// 重载 MyTinySTL 的 swap
	template <class Key, class T, class Compare, class Alloc>
	void swap(multimap<Key, T, Compare, Alloc>& x, multimap<Key, T, Compare, Alloc>& y) {
		x.swap(y);
	}
}
#endif // !MAP_H

