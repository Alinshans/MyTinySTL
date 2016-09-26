#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "hashtable.h"

namespace MyTinySTL {

	// hash_map: 使用方法与 map 相同，使用 hashtable 作为底层机制，元素不会自动排序
	template <class Key, class T, class HashFcn = MyTinySTL::hash<Key>,
		class EqualKey = MyTinySTL::equal_to<Key>, class Alloc = alloc>
		class hash_map {
		private:
			// 使用 hash_table 作为底层机制
			typedef hashtable<pair<const Key, T>, Key, HashFcn, 
				MyTinySTL::selectfirst<pair<const Key, T>>, EqualKey, Alloc>	rep_type;
			rep_type ht;

		public:
			// 使用 hash_table 的型别
			typedef typename rep_type::key_type	key_type;
			typedef T	data_type;
			typedef T	mapped_type;
			typedef typename rep_type::value_type	value_type;
			typedef typename rep_type::hasher	hasher;
			typedef typename rep_type::key_equal	key_equal;

			typedef typename rep_type::size_type	size_type;
			typedef typename rep_type::difference_type	difference_type;
			typedef typename rep_type::pointer	pointer;
			typedef typename rep_type::const_pointer	const_pointer;
			typedef typename rep_type::reference	reference;
			typedef typename rep_type::const_reference	const_reference;

			typedef typename rep_type::iterator	iterator;
			typedef typename rep_type::const_iterator	const_iterator;
			typedef typename rep_type::allocator_type	allocator_type;

			hasher hash_fcn() const { return ht.hash_fcn(); }
			key_equal key_eq() const { return ht.key_eq(); }
			allocator_type get_allocator() const { return allocator_type(); }

		public:
			// 构造函数
			hash_map() :ht(100, hasher(), key_equal()) {}	// 缺省使用大小为100的表格
			explicit hash_map(size_type n) :ht(n, hasher(), key_equal()) {}
			hash_map(size_type n, const hasher& hf) :ht(n, hf, key_equal()) {}
			hash_map(size_type n, const hasher& hf, const key_equal& keq) :ht(n, hf, keq) {}

			// 全部使用 insert_unique，键值不允许重复
			template <class InputIterator>
			hash_map(InputIterator first, InputIterator last)
				: ht(100, hasher(), key_equal()) {
				ht.insert_unique(first, last);
			}
			template <class InputIterator>
			hash_map(InputIterator first, InputIterator last, size_type n)
				: ht(n, hasher(), key_equal()) {
				ht.insert_unique(first, last);
			}
			template <class InputIterator>
			hash_map(InputIterator first, InputIterator last, size_type n,
				const hasher& hf) : ht(n, hf, key_equal()) {
				ht.insert_unique(first, last);
			}
			template <class InputIterator>
			hash_map(InputIterator first, InputIterator last, size_type n,
				const hasher& hf, const key_equal& keq) : ht(n, hf, keq) {
				ht.insert_unique(first, last);
			}

			// 几乎所有操作都是调用 hash_table 的版本
			// 相关接口
			iterator begin() { return ht.begin(); }
			const_iterator begin() const { return ht.begin(); }
			iterator end() { return ht.end(); }
			const_iterator end() const { return ht.end(); }

			bool empty() const { return ht.empty(); }
			size_type size() const { return ht.size(); }
			size_type max_size() const { return ht.max_size(); }

			// 插入删除操作
			// 使用 insert_unique，键值不允许重复 
			pair<iterator, bool> insert(const value_type& value) {
				return ht.insert_unique(value);
			}
			template <class InputIterator>
			void insert(InputIterator first, InputIterator last) {
				ht.insert_unique(first, last);
			}
			pair<iterator, bool> insert_noresize(const value_type& value) {
				return ht.insert_unique_noresize(value);
			}

			size_type erase(const key_type& key) { return ht.erase(key); }
			void erase(iterator it) { ht.erase(it); }
			void erase(iterator first, iterator last) { ht.erase(first, last); }
			void clear() { ht.clear(); }

			data_type& operator[](const key_type& key) {
				return ht.find_or_insert(value_type(key, T()));
			}

			// hash_map 相关操作
			iterator find(const key_type& key) { return ht.find(key); }
			const_iterator find(const key_type& key) const { return ht.find(key); }
			size_type count(const key_type& key) const { return ht.count(key); }
			pair<iterator, iterator> equal_range(const key_type& key) {
				return ht.equal_range(key);
			}
			pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
				return ht.equal_range(key);
			}

			void resize(size_type hint) { ht.resize(hint); }
			size_type bucket_count() const { return ht.bucket_count(); }
			size_type max_bucket_count() const { return ht.max_bucket_count(); }
			size_type elems_in_bucket(size_type n)const { return ht.elems_in_bucket(n); }
			void swap(hash_map& rhs) { ht.swap(rhs.ht); }

		public:
			friend bool operator==(const hash_map& lhs, const hash_map& rhs) {
				return lhs.ht == rhs.ht;
			}
	};

	// 重载操作符
	template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
	inline bool operator==(const hash_map<Key, T, HashFcn, EqualKey, Alloc>& lhs,
		const hash_map<Key, T, HashFcn, EqualKey, Alloc>& rhs) {
		return lhs == rhs;
	}

	template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
	inline bool operator!=(const hash_map<Key, T, HashFcn, EqualKey, Alloc>& lhs,
		const hash_map<Key, T, HashFcn, EqualKey, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	// 重载 MyTinySTL 的 swap
	template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
	void swap(hash_map<Key, T, HashFcn, EqualKey, Alloc>& lhs,
		hash_map<Key, T, HashFcn, EqualKey, Alloc>& rhs) {
		lhs.swap(rhs);
	}

	/***********************************************************************************/

	// hash_multimap: 键值允许重复，其它与 hash_map 相同
	template <class Key, class T, class HashFcn = MyTinySTL::hash<Key>,
		class EqualKey = MyTinySTL::equal_to<Key>, class Alloc = alloc>
		class hash_multimap {
		private:
			// 使用 hash_table 作为底层机制
			typedef hashtable<pair<const Key, T>, Key, HashFcn,
				MyTinySTL::selectfirst<pair<const Key, T>>, EqualKey, Alloc>	rep_type;
			rep_type ht;

		public:
			// 使用 hash_table 的型别
			typedef typename rep_type::key_type	key_type;
			typedef T	data_type;
			typedef T	mapped_type;
			typedef typename rep_type::value_type	value_type;
			typedef typename rep_type::hasher	hasher;
			typedef typename rep_type::key_equal	key_equal;

			typedef typename rep_type::size_type	size_type;
			typedef typename rep_type::difference_type	difference_type;
			typedef typename rep_type::pointer	pointer;
			typedef typename rep_type::const_pointer	const_pointer;
			typedef typename rep_type::reference	reference;
			typedef typename rep_type::const_reference	const_reference;

			typedef typename rep_type::iterator	iterator;
			typedef typename rep_type::const_iterator	const_iterator;
			typedef typename rep_type::allocator_type	allocator_type;

			hasher hash_fcn() const { return ht.hash_fcn(); }
			key_equal key_eq() const { return ht.key_eq(); }
			allocator_type get_allocator() const { return allocator_type(); }

		public:
			// 构造函数
			hash_multimap() :ht(100, hasher(), key_equal()) {}	// 缺省使用大小为100的表格
			explicit hash_multimap(size_type n) :ht(n, hasher(), key_equal()) {}
			hash_multimap(size_type n, const hasher& hf) :ht(n, hf, key_equal()) {}
			hash_multimap(size_type n, const hasher& hf, const key_equal& keq) :ht(n, hf, keq) {}

			// 全部使用 insert_equal，键值不允许重复
			template <class InputIterator>
			hash_multimap(InputIterator first, InputIterator last)
				: ht(100, hasher(), key_equal()) {
				ht.insert_equal(first, last);
			}
			template <class InputIterator>
			hash_multimap(InputIterator first, InputIterator last, size_type n)
				: ht(n, hasher(), key_equal()) {
				ht.insert_equal(first, last);
			}
			template <class InputIterator>
			hash_multimap(InputIterator first, InputIterator last, size_type n,
				const hasher& hf) : ht(n, hf, key_equal()) {
				ht.insert_equal(first, last);
			}
			template <class InputIterator>
			hash_multimap(InputIterator first, InputIterator last, size_type n,
				const hasher& hf, const key_equal& keq) : ht(n, hf, keq) {
				ht.insert_equal(first, last);
			}

			// 几乎所有操作都是调用 hash_table 的版本
			// 相关接口
			iterator begin() { return ht.begin(); }
			const_iterator begin() const { return ht.begin(); }
			iterator end() { return ht.end(); }
			const_iterator end() const { return ht.end(); }

			bool empty() const { return ht.empty(); }
			size_type size() const { return ht.size(); }
			size_type max_size() const { return ht.max_size(); }

			// 插入删除操作
			// 使用 insert_equal，键值不允许重复 
			iterator insert(const value_type& value) {
				return ht.insert_equal(value);
			}
			template <class InputIterator>
			void insert(InputIterator first, InputIterator last) {
				ht.insert_equal(first, last);
			}
			iterator insert_noresize(const value_type& value) {
				return ht.insert_equal_noresize(value);
			}

			size_type erase(const key_type& key) { return ht.erase(key); }
			void erase(iterator it) { ht.erase(it); }
			void erase(iterator first, iterator last) { ht.erase(first, last); }
			void clear() { ht.clear(); }

			data_type& operator[](const key_type& key) {
				return ht.find_or_insert(value_type(key, T())).second;
			}

			// hash_multimap 相关操作
			iterator find(const key_type& key) { return ht.find(key); }
			const_iterator find(const key_type& key) const { return ht.find(key); }
			size_type count(const key_type& key) const { return ht.count(key); }
			pair<iterator, iterator> equal_range(const key_type& key) {
				return ht.equal_range(key);
			}
			pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
				return ht.equal_range(key);
			}

			void resize(size_type hint) { ht.resize(hint); }
			size_type bucket_count() const { return ht.bucket_count(); }
			size_type max_bucket_count() const { return ht.max_bucket_count(); }
			size_type elems_in_bucket(size_type n)const { return ht.elems_in_bucket(n); }
			void swap(hash_multimap& rhs) { ht.swap(rhs.ht); }

		public:
			friend bool operator==(const hash_multimap& lhs, const hash_multimap& rhs) {
				return lhs.ht == rhs.ht;
			}
	};

	// 重载操作符
	template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
	inline bool operator==(const hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
		const hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& rhs) {
		return lhs == rhs;
	}

	template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
	inline bool operator!=(const hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
		const hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	// 重载 MyTinySTL 的 swap
	template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
	void swap(hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
		hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}
#endif // !HASH_MAP_H

