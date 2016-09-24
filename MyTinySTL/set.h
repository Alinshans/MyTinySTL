#ifndef SET_H
#define SET_H

#include "rb_tree.h"

namespace MyTinySTL {

	// set:键值与实值相同，键值不允许重复，以 RB-tree 作为底层机制
	template <class Key, class Compare = MyTinySTL::less<Key>, class Alloc = alloc>
	class set {
	public:
		typedef Key	key_type;
		typedef Key	value_type;
		typedef Compare	key_compare;
		typedef Compare	value_compare;
		typedef Alloc	allocate_type;

	private:
		// 用 RB-tree 来表现 set
		typedef rb_tree<key_type, value_type, MyTinySTL::identity<value_type>,
			key_compare, Alloc>	rep_type;
		rep_type t;

	public:
		// 使用 RB-tree 定义的型别
		typedef typename rep_type::const_pointer	pointer;
		typedef typename rep_type::const_pointer	const_pointer;
		typedef typename rep_type::const_reference	reference;
		typedef typename rep_type::const_reference	const_reference;
		typedef typename rep_type::const_iterator	iterator;
		// iterator 定义为 RB-tree 的 const_iterator，表示 set 不允许进行写入操作
		typedef typename rep_type::const_iterator	const_iterator;
		typedef typename rep_type::const_reverse_iterator	reverse_iterator;
		typedef typename rep_type::const_reverse_iterator	const_reverse_iterator;
		typedef typename rep_type::size_type	size_type;
		typedef typename rep_type::difference_type	difference_type;

	public:
		// 构造、复制函数
		set() :t() {}
		template <class InputIterator>
		set(InputIterator first, InputIterator last) : t() { t.insert_unique(first, last); }
		set(const set& x) :t(x.t) {}
		set& operator=(const set& x) { t = x.t; return *this; }

		// 相关接口操作
		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return t.key_comp(); }
		allocate_type get_allocator() const { return t.get_allocator(); }

		iterator begin() const { return t.begin(); }
		iterator end() const { return t.end(); }
		reverse_iterator rbegin() const { return t.rbegin(); }
		reverse_iterator rend() const { return t.rend(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }
		
		// 插入删除操作
		// 插入操作使用的是 RB-tree 的 insert_unique
		pair<iterator, bool> insert(const value_type& x) {
			pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
			return pair<iterator, bool>(p.first, p.second);
		}
		iterator insert(iterator position, const value_type& x) {
			typedef typename rep_type::iterator rep_iterator;
			return t.insert_unique((rep_iterator&)position, x);
		}
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			t.insert_unique(first, last);
		}
		void erase(iterator position) {
			typedef typename rep_type::iterator rep_iterator;
			t.erase((rep_iterator&)position);
		}
		size_type erase(const key_type& x) {
			return t.erase(x);
		}
		void erase(iterator first, iterator last) {
			typedef typename rep_type::iterator rep_iterator;
			t.erase((rep_iterator&)first, (rep_iterator&)last);
		}
		void clear() { t.clear(); }

		// set 操作
		iterator find(const key_type& x) const { return t.find(x); }
		size_type count(const key_type& x) const { return t.count(x); }
		iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
		iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
		pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x); }
		void swap(set& x) { t.swap(x.t); }

	public:
		friend bool operator==(const set& x, const set& y) { return x.t == y.t; }
		friend bool operator<(const set& x, const set& y) { return x.t < y.t; }
	};

	// 重载操作符
	template <class Key, class Compare, class Alloc>
	inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return x == y;
	}

	template <class Key, class Compare, class Alloc>
	inline bool operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return x < y;
	}

	template <class Key, class Compare, class Alloc>
	inline bool operator!=(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return !(x == y);
	}

	template <class Key, class Compare, class Alloc>
	inline bool operator>(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return y < x;
	}

	template <class Key, class Compare, class Alloc>
	inline bool operator<=(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return !(y < x);
	}

	template <class Key, class Compare, class Alloc>
	inline bool operator>=(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return !(x < y);
	}

	// 重载 MyTinySTL 的 swap
	template <class Key, class Compare, class Alloc>
	void swap(set<Key, Compare, Alloc>& x, set<Key, Compare, Alloc>& y) {
		x.swap(y);
	}

	/***********************************************************************************/

	// multiset:允许键值重复的 set
	template <class Key, class Compare = MyTinySTL::less<Key>, class Alloc = alloc>
	class multiset {
	public:
		typedef Key	key_type;
		typedef Key	value_type;
		typedef Compare	key_compare;
		typedef Compare	value_compare;
		typedef Alloc	allocate_type;

	private:
		// 用 RB-tree 来表现 set
		typedef rb_tree<key_type, value_type, MyTinySTL::identity<value_type>,
			key_compare, Alloc>	rep_type;
		rep_type t;

	public:
		// 使用 RB-tree 定义的型别
		typedef typename rep_type::const_pointer	pointer;
		typedef typename rep_type::const_pointer	const_pointer;
		typedef typename rep_type::const_reference	reference;
		typedef typename rep_type::const_reference	const_reference;
		typedef typename rep_type::const_iterator	iterator;
		// iterator 定义为 RB-tree 的 const_iterator，表示 set 不允许进行写入操作
		typedef typename rep_type::const_iterator	const_iterator;
		typedef typename rep_type::const_reverse_iterator	reverse_iterator;
		typedef typename rep_type::const_reverse_iterator	const_reverse_iterator;
		typedef typename rep_type::size_type	size_type;
		typedef typename rep_type::difference_type	difference_type;

	public:
		// 构造、复制、析构函数
		multiset() :t() {}
		template <class InputIterator>
		multiset(InputIterator first, InputIterator last) : t() { t.insert_equal(first, last); }
		multiset(const multiset& x) :t(x.t) {}
		multiset& operator=(const multiset& x) { t = x.t; return *this; }

		// 相关接口
		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return t.key_comp(); }
		allocate_type get_allocator() const { return t.get_allocator(); }

		iterator begin() const { return t.begin(); }
		iterator end() const { return t.end(); }
		reverse_iterator rbegin() const { return t.rbegin(); }
		reverse_iterator rend() const { return t.rend(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }

		// 插入删除
		// 插入操作使用的是 RB-tree 的 insert_equal
		pair<iterator, bool> insert(const value_type& x) {
			pair<typename rep_type::iterator, bool> p = t.insert_equal(x);
			return pair<iterator, bool>(p.first, p.second);
		}
		iterator insert(iterator position, const value_type& x) {
			typedef typename rep_type::iterator rep_iterator;
			return t.insert_equal((rep_iterator&)position, x);
		}
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			t.insert_equal(first, last);
		}
		void erase(iterator position) {
			typedef typename rep_type::iterator rep_iterator;
			t.erase((rep_iterator&)position);
		}
		size_type erase(const key_type& x) {
			return t.erase(x);
		}
		void erase(iterator first, iterator last) {
			typedef typename rep_type::iterator rep_iterator;
			t.erase((rep_iterator&)first, (rep_iterator&)last);
		}
		void clear() { t.clear(); }

		// multiset 操作
		iterator find(const key_type& x) const { return t.find(x); }
		size_type count(const key_type& x) const { return t.count(x); }
		iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
		iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
		pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x); }
		void swap(multiset& x) { t.swap(x.t); }

	public:
		friend bool operator==(const multiset& x, const multiset& y) { return x.t == y.t; }
		friend bool operator<(const multiset& x, const multiset& y) { return x.t < y.t; }
	};

	// 重载操作符
	template <class Key, class Compare, class Alloc>
	inline bool operator!=(const multiset<Key, Compare, Alloc>& x, 
		const multiset<Key, Compare, Alloc>& y) {
		return !(x == y);
	}

	template <class Key, class Compare, class Alloc>
	inline bool operator>(const multiset<Key, Compare, Alloc>& x, 
		const multiset<Key, Compare, Alloc>& y) {
		return y < x;
	}

	template <class Key, class Compare, class Alloc>
	inline bool operator<=(const multiset<Key, Compare, Alloc>& x, 
		const multiset<Key, Compare, Alloc>& y) {
		return !(y < x);
	}

	template <class Key, class Compare, class Alloc>
	inline bool operator>=(const multiset<Key, Compare, Alloc>& x,
		const multiset<Key, Compare, Alloc>& y) {
		return !(x < y);
	}

	// 重载 MyTinySTL 的 swap
	template <class Key, class Compare, class Alloc>
	void swap(multiset<Key, Compare, Alloc>& x, multiset<Key, Compare, Alloc>& y) {
		x.swap(y);
	}
}
#endif // !SET_H
