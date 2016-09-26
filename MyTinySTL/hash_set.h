#ifndef HASH_SET_H
#define HASH_SET_H

#include "hashtable.h"

namespace MyTinySTL {
	
	// hash_set:使用方法与 set 相同，以 hashtable 作为底层机制，hash_set 中的元素没有自动排序
	template <class Value, class HashFcn = MyTinySTL::hash<Value>, 
		class EqualKey = MyTinySTL::equal_to<Value>, class Alloc = alloc>
		class hash_set {
		private:
			// 使用 hash_table 作为底层机制
			typedef hashtable<Value, Value, HashFcn, MyTinySTL::identity<Value>,
				EqualKey, Alloc>	rep_type;
			rep_type ht;

		public:
			// 使用 hash_table 的型别
			typedef typename rep_type::key_type	key_type;
			typedef typename rep_type::value_type	value_type;
			typedef typename rep_type::hasher	hasher;
			typedef typename rep_type::key_equal	key_equal;

			typedef typename rep_type::size_type	size_type;
			typedef typename rep_type::difference_type	difference_type;
			typedef typename rep_type::const_pointer	pointer;
			typedef typename rep_type::const_pointer	const_pointer;
			typedef typename rep_type::const_reference	reference;
			typedef typename rep_type::const_reference	const_reference;

			typedef typename rep_type::const_iterator	iterator;
			typedef typename rep_type::const_iterator	const_iterator;
			typedef typename rep_type::allocator_type	allocator_type;

			hasher hash_fcn() const { return ht.hash_fcn(); }
			key_equal key_eq() const { return ht.key_eq(); }
			allocator_type get_allocator() const { return allocator_type(); }

		public:
			// 构造函数
			hash_set() :ht(100, hasher(), key_equal()) {}	// 缺省使用大小为100的表格
			explicit hash_set(size_type n) :ht(n, hasher(), key_equal()) {}
			hash_set(size_type n, const hasher& hf) :ht(n, hf, key_equal()) {}
			hash_set(size_type n, const hasher& hf, const key_equal& keq) :ht(n, hf, keq) {}

			// 全部使用 insert_unique，键值不允许重复
			template <class InputIterator>
			hash_set(InputIterator first, InputIterator last)
				: ht(100, hasher(), key_equal()) {
				ht.insert_unique(first, last);
			}
			template <class InputIterator>
			hash_set(InputIterator first, InputIterator last, size_type n)
				: ht(n, hasher(), key_equal()) {
				ht.insert_unique(first, last);
			}
			template <class InputIterator>
			hash_set(InputIterator first, InputIterator last, size_type n,
				const hasher& hf) : ht(n, hf, key_equal()) {
				ht.insert_unique(first, last);
			}
			template <class InputIterator>
			hash_set(InputIterator first, InputIterator last, size_type n,
				const hasher& hf, const key_equal& keq) : ht(n, hf, keq) {
				ht.insert_unique(first, last);
			}

			// 几乎所有操作都是调用 hash_table 的版本
			// 相关接口
			iterator begin() const { return ht.begin(); }
			iterator end() const { return ht.end(); }

			bool empty() const { return ht.empty(); }
			size_type size() const { return ht.size(); }
			size_type max_size() const { return ht.max_size(); }
			
			// 插入删除操作
			// 使用 insert_unique，键值不允许重复 
			pair<iterator, bool> insert(const value_type& value) {
				pair<typename rep_type::iterator, bool> p = ht.insert_unique(value);
				return pair<iterator, bool>(p.first, p.second);
			}
			template <class InputIterator>
			void insert(InputIterator first, InputIterator last) { 
				ht.insert_unique(first, last); 
			}
			pair<iterator, bool> insert_noresize(const value_type& value) {
				pair<typename rep_type::iterator, bool> p = ht.insert_unique_noresize(value);
				return pair<iterator, bool>(p.first, p.second);
			}

			size_type erase(const key_type& key) { return ht.erase(key); }
			void erase(iterator it) { ht.erase(it); }
			void erase(iterator first, iterator last) { ht.erase(first, last); }
			void clear() { ht.clear(); }

			// hash_set 相关操作
			iterator find(const key_type& key) const { return ht.find(key); }
			size_type count(const key_type& key) const { return ht.count(key); }
			pair<iterator, iterator> equal_range(const key_type& key) const {
				return ht.equal_range(key);
			}

			void resize(size_type hint) { ht.resize(hint); }
			size_type bucket_count() const { return ht.bucket_count(); }
			size_type max_bucket_count() const { return ht.max_bucket_count(); }
			size_type elems_in_bucket(size_type n)const { return ht.elems_in_bucket(n); }
			void swap(hash_set& rhs) { ht.swap(rhs.ht); }

		public:
			friend bool operator==(const hash_set& lhs, const hash_set& rhs) {
				return lhs.ht == rhs.ht;
			}
	};

	// 重载操作符
	template <class Value, class HashFcn, class EqualKey, class Alloc>
	inline bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc>& lhs,
		const hash_set<Value, HashFcn, EqualKey, Alloc>& rhs) {
		return lhs == rhs;
	}

	template <class Value, class HashFcn, class EqualKey, class Alloc>
	inline bool operator!=(const hash_set<Value, HashFcn, EqualKey, Alloc>& lhs,
		const hash_set<Value, HashFcn, EqualKey, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	// 重载 MyTinySTL 的 swap
	template <class Value, class HashFcn, class EqualKey, class Alloc>
	void swap(hash_set<Value, HashFcn, EqualKey, Alloc>& lhs,
		hash_set<Value, HashFcn, EqualKey, Alloc>& rhs) {
		lhs.swap(rhs);
	}

	/***********************************************************************************/
	
	// hash_multiset: 键值允许重复，其它与 hash_set 相同
	template <class Value, class HashFcn = MyTinySTL::hash<Value>,
		class EqualKey = MyTinySTL::equal_to<Value>, class Alloc = alloc>
		class hash_multiset {
		private:
			// 使用 hash_table 作为底层机制
			typedef hashtable<Value, Value, HashFcn, MyTinySTL::identity<Value>,
				EqualKey, Alloc>	rep_type;
			rep_type ht;

		public:
			// 使用 hash_table 的型别
			typedef typename rep_type::key_type	key_type;
			typedef typename rep_type::value_type	value_type;
			typedef typename rep_type::hasher	hasher;
			typedef typename rep_type::key_equal	key_equal;

			typedef typename rep_type::size_type	size_type;
			typedef typename rep_type::difference_type	difference_type;
			typedef typename rep_type::const_pointer	pointer;
			typedef typename rep_type::const_pointer	const_pointer;
			typedef typename rep_type::const_reference	reference;
			typedef typename rep_type::const_reference	const_reference;

			typedef typename rep_type::const_iterator	iterator;
			typedef typename rep_type::const_iterator	const_iterator;
			typedef typename rep_type::allocator_type	allocator_type;

			hasher hash_fcn() const { return ht.hash_fcn(); }
			key_equal key_eq() const { return ht.key_eq(); }
			allocator_type get_allocator() const { return allocator_type(); }

		public:
			// 构造函数
			hash_multiset() :ht(100, hasher(), key_equal()) {}	// 缺省使用大小为100的表格
			explicit hash_multiset(size_type n) :ht(n, hasher(), key_equal()) {}
			hash_multiset(size_type n, const hasher& hf) :ht(n, hf, key_equal()) {}
			hash_multiset(size_type n, const hasher& hf, const key_equal& keq) :ht(n, hf, keq) {}

			// 全部使用 insert_equal，键值允许重复
			template <class InputIterator>
			hash_multiset(InputIterator first, InputIterator last)
				: ht(100, hasher(), key_equal()) {
				ht.insert_equal(first, last);
			}
			template <class InputIterator>
			hash_multiset(InputIterator first, InputIterator last, size_type n)
				: ht(n, hasher(), key_equal()) {
				ht.insert_equal(first, last);
			}
			template <class InputIterator>
			hash_multiset(InputIterator first, InputIterator last, size_type n,
				const hasher& hf) : ht(n, hf, key_equal()) {
				ht.insert_equal(first, last);
			}
			template <class InputIterator>
			hash_multiset(InputIterator first, InputIterator last, size_type n,
				const hasher& hf, const key_equal& keq) : ht(n, hf, keq) {
				ht.insert_equal(first, last);
			}

			// 几乎所有操作都是调用 hash_table 的版本
			// 相关接口
			iterator begin() const { return ht.begin(); }
			iterator end() const { return ht.end(); }

			bool empty() const { return ht.empty(); }
			size_type size() const { return ht.size(); }
			size_type max_size() const { return ht.max_size(); }

			// 插入删除操作
			// 使用 insert_equal，键值允许重复 
			iterator insert(const value_type& value) {
				pair<typename rep_type::iterator, bool> p = ht.insert_equal(value);
				return pair<iterator, bool>(p.first, p.second);
			}
			template <class InputIterator>
			void insert(InputIterator first, InputIterator last) {
				ht.insert_equal(first, last);
			}
			iterator insert_noresize(const value_type& value) {
				pair<typename rep_type::iterator, bool> p = ht.insert_equal_noresize(value);
				return pair<iterator, bool>(p.first, p.second);
			}

			size_type erase(const key_type& key) { return ht.erase(key); }
			void erase(iterator it) { ht.erase(it); }
			void erase(iterator first, iterator last) { ht.erase(first, last); }
			void clear() { ht.clear(); }

			// hash_multiset 相关操作
			iterator find(const key_type& key) const { return ht.find(key); }
			size_type count(const key_type& key) const { return ht.count(key); }
			pair<iterator, iterator> equal_range(const key_type& key) const {
				return ht.equal_range(key);
			}

			void resize(size_type hint) { ht.resize(hint); }
			size_type bucket_count() const { return ht.bucket_count(); }
			size_type max_bucket_count() const { return ht.max_bucket_count(); }
			size_type elems_in_bucket(size_type n)const { return ht.elems_in_bucket(n); }
			void swap(hash_multiset& rhs) { ht.swap(rhs.ht); }

		public:
			friend bool operator==(const hash_multiset& lhs, const hash_multiset& rhs) {
				return lhs.ht == rhs.ht;
			}
	};

	// 重载操作符
	template <class Value, class HashFcn, class EqualKey, class Alloc>
	inline bool operator==(const hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs,
		const hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
		return lhs == rhs;
	}

	template <class Value, class HashFcn, class EqualKey, class Alloc>
	inline bool operator!=(const hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs,
		const hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	// 重载 MyTinySTL 的 swap
	template <class Value, class HashFcn, class EqualKey, class Alloc>
	void swap(hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs,
		hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}
#endif // !HASH_SET_H

