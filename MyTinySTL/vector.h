#ifndef VECTOR_H
#define VECTOR_H

#include "iterator.h"
#include "reverse_iterator.h"
#include "memory.h"

namespace MyTinySTL {

	template <class T, class Alloc = allocator<T>>
	class vector {
	public:
		// vector 的嵌套型别定义
		typedef T					value_type;
		typedef Alloc				allocate_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef value_type*			iterator;
		typedef const value_type*	const_iterator;		
		typedef reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef reverse_iterator<iterator>	reverse_iterator;	
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

	public:
		typedef Alloc	data_allocator;	//vector 的空间配置器

	protected:
		iterator start;				//表示目前使用空间的头
		iterator finish;			//表示目前使用空间的尾
		iterator end_of_storage;	//表示目前可用空间的尾

	public:
		// 构造函数
		vector() :start(0), finish(0), end_of_storage(0) {}
		explicit vector(size_type n) { allocate_and_fill(n, T()); }
		vector(size_type n, const T& value) { allocate_and_fill(n, value); }
		template <class InputIterator>
		vector(InputIterator first, InputIterator last);

		// 复制构造函数
		vector(const vector<T, Alloc>& x);
		vector(vector<T, Alloc>&& x);

		// 赋值操作符 operator=
		vector<T, Alloc>& operator=(const vector<T, Alloc>& x);
		vector<T, Alloc>& operator=(vector<T, Alloc>&& x);

		// 析构函数
		~vector() { destroy_and_deallocate(); }

	public:
		// 迭代器相关操作
		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		iterator end() { return finish; }
		const_iterator end() const { return finish; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

		// 容量相关操作
		size_type size() const { return size_type(end() - begin()); }
		size_type max_size() const { return size_type(-1) / sizeof(T); }
		size_type capacity() const { return size_type(end_of_storage - begin()); }
		bool empty() const { return begin() == end(); }
		void resize(size_type new_size, const T& x);
		void resize(size_type new_size) { return resize(new_size, T()); }

		// 访问元素相关操作
		reference operator[](size_type n) { return *(begin() + n); }
		const_reference operator[](size_type n) const { return *(begin() + n); }
		reference at(size_type n) { return (*this)[n]; }
		const_reference at(size_type n)	const { return (*this)[n]; }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }

		// 修改容器相关操作
		void assign(size_type n, const T& value) { fill_assign(n, value); }
		void assign(size_type n) { fill_assign(n, T()); }
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		void push_back(const T& x);
		void pop_back();
		iterator earse(iterator position);
		iterator earse(iterator first, iterator last);
		void clear() { earse(begin(), end()); }
		iterator insert(iterator position, const T& x);
		iterator insert(iterator position);
		void insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		void swap(vector<T, Alloc>& x);

		// 配置器相关操作
		allocate_type get_allocator() { return allocate_type(); }

	protected:
		// 成员函数
		template <class Integer>
		void vector_construct(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void vector_construct(InputIterator first, InputIterator last, __false_type);
		void destroy_and_deallocate();
		void allocate_and_fill(size_type n, const T& value);
		template <class InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last);
		void fill_assign(size_type n, const T& value);
		template <class Integer>
		void assign_dispatch(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void assign_dispatch(InputIterator first, InputIterator last, __false_type);
		template <class InputIterator>
		void assign_aux(InputIterator first, InputIterator last, input_iterator_tag);
		template <class ForwardIterator>
		void assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
		void insert_aux(iterator position, const T& x);
		template <class Integer>
		void insert_dispatch(iterator position, Integer n, Integer x, __true_type);
		template <class InputIterator>
		void insert_dispatch(iterator position, InputIterator first, InputIterator last,
			__false_type);
		void fill_insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void range_insert(iterator position, InputIterator first, InputIterator last,
			input_iterator_tag);
		template <class ForwardIterator>
		void range_insert(iterator position, ForwardIterator first, ForwardIterator last,
			forward_iterator_tag);
	};

	/********************************************************************************/

	// 构造函数
	template <class T, class Alloc>
	template <class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer integer;
		vector_construct(first, last, integer());
	}

	template <class T, class Alloc>
	template <class Integer>
	void vector<T, Alloc>::vector_construct(Integer n, Integer value, __true_type) {
		allocate_and_fill(n, value);
	}

	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::vector_construct(InputIterator first, InputIterator last,
		__false_type) {
		allocate_and_copy(first, last);
	}

	// 复制构造函数
	template <class T, class Alloc>
	vector<T, Alloc>::vector(const vector& x) {
		allocate_and_copy(x.start, x.finish);
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(vector&& x) {
		start = x.start;
		finish = x.finish;
		end_of_storage = x.end_of_storage;
		x.start = x.finish = x.end_of_storage = 0;
	}
	
	// 赋值操作符 operator=
	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& x) {
		if (&x != this) {
			const size_type xlen = x.size();
			if (xlen > capacity()) {	//如果要赋值的 vector 大小超过原 vector 容量大小
				destroy_and_deallocate();
				allocate_and_copy(x.begin(), x.end());
			}
			else if (size() >= xlen) {	//如果原 vector 大小大于等于要赋值的 vector 大小
				iterator i = copy(x.begin(), x.end(), begin());
				destroy(i, finish);
				finish = start + xlen;
			}
			else { //原 vector 大小小于要赋值的 vector 大小
				copy(x.begin(), x.begin() + size(), start);
				uninitialized_copy(x.begin() + size(), x.end(), finish);
				end_of_storage = finish = start + xlen;
			}
		}
		return *this;
	}

	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector<T, Alloc>&& x) {
		if (*this != x) {
			destroy_and_deallocate();
			start = x.start;
			finish = x.finish;
			end_of_storage = x.end_of_storage;
			x.start = x.finish = x.end_of_storage = 0;
		}
		return *this;
	}

	// 重置容器大小
	template <class T, class Alloc>
	void vector<T, Alloc>::resize(size_type new_size, const T& x) {
		if (new_size < size())
			earse(begin() + new_size, end());
		else
			insert(end(), new_size - size(), x);
	}

	// 初始化容器
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer integer;
		assign_dispatch(first, last, integer());
	}

	// 在容器尾端插入元素
	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(const T& x) {
		if (finish != end_of_storage) {
			construct(finish, x);
			++finish;
		}
		else
			insert_aux(end(), x);
	}

	// 删除最后一个元素
	template <class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		--finish;
		destroy(finish);
	}

	// 删除 position 位置上的元素
	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::earse(iterator position) {
		if (position + 1 != end())
			copy(position + 1, finish, position);	//后面的元素往前移
		--finish;
		destroy(finish);
		return position;	//返回被删除的下一个元素
	}

	// 删除[first, last)上的元素
	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::earse(iterator first, iterator last) {
		iterator i = copy(last, finish, first);	//把last之后的元素复制到first为起始的空间
		destroy(i, finish);	//销毁不要的元素
		finish = finish - (last - first);	//调整水位
		return first;
	}

	// 与另一个 vector 交换
	template <class T, class Alloc>
	void vector<T, Alloc>::swap(vector<T, Alloc>& x) {
		if (*this != x) {
			MyTinySTL::swap(start, x.start);
			MyTinySTL::swap(finish, x.finish);
			MyTinySTL::swap(end_of_storage, x.end_of_storage);
		}
	}

	// 在 position 位置插入元素									 
	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(iterator position, const T& x) {
		size_type n = position - begin();
		if (finish != end_of_storage && position == end()) {
			construct(finish, x);
			++finish;
		}
		else
			insert_aux(position, x);
		return begin() + n;
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(iterator position) {
		size_type n = position - begin();
		if (finish != end_of_storage && position == end()) {
			construct(finish);
			++finish;
		}
		else
			insert_aux(position, T());
		return begin() + n;
	}

	// 在 position 位置开始插入 n 个元素
	template <class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
		fill_insert(position, n, x);
	}

	// 在 position 位置插入[first, last)内的元素
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::insert(iterator position,
		InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer integer;
		insert_dispatch(position, first, last, integer());
	}

	// destroy_and_deallocate 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::destroy_and_deallocate() {
		destroy(start, finish);
		data_allocator::deallocate(start, end_of_storage - start);
	}

	// allocate_and_fill 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::allocate_and_fill(size_type n, const T& value) {
		start = data_allocator::allocate(n);
		finish = uninitialized_fill_n(start, n, value);
		end_of_storage = finish;
	}

	// allocate_and_copy 函数
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last) {
		difference_type n = last - first;
		start = data_allocator::allocate(n);
		finish = uninitialized_copy(first, last, start);
		end_of_storage = finish;
	}
	
	// fill_assign 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::fill_assign(size_type n, const T& value) {
		if (n > capacity()) {
			vector<T, Alloc> tmp(n, value);
			tmp.swap(*this);
		}
		else if (n > size()) {
			fill(begin(), end(), value);
			finish = uninitialized_fill_n(finish, n - size(), value);
		}
		else
			earse(fill_n(start, n, value), finish);
	}
	
	// assign_dispatch 函数
	template <class T, class Alloc>
	template <class Integer>
	void vector<T, Alloc>::assign_dispatch(Integer n, Integer value, __true_type) {
		fill_assign((size_type)n, (T)value);
	}

	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::assign_dispatch(InputIterator first, InputIterator last,
		__false_type) {
		assign_aux(first, last, iterator_category(first));
	}

	// assign_aux 函数
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::assign_aux(InputIterator first, InputIterator last,
		input_iterator_tag) {
		iterator cur = begin();
		for (; first != last && cur != end(); ++first, ++cur) {
			*cur = *first;
		}
		if (first == last)
			earse(cur, end());
		else
			insert(end(), first, last);
	}

	template <class T, class Alloc>
	template <class ForwardIterator>
	void vector<T, Alloc>::assign_aux(ForwardIterator first, ForwardIterator last,
		forward_iterator_tag) {
		distance_type len = distance(first, last);
		if (len > capacity()) {
			destroy_and_deallocate();
			allocate_and_copy(first, last);
		}
		else if (size() >= len) {
			iterator new_finish = copy(first, last, start);
			destroy(new_finish, finish);
			finish = new_finish;
		}
		else {
			ForwardIterator mid = first;
			advance(mid, size());
			copy(first, mid, start);
			finish = uninitialized_copy(mid, last, finish);
		}
	}

	// insert_aux 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
		if (finish != end_of_storage) {	//如果还有备用空间
			construct(finish, *(finish - 1));	//在备用空间构造一个以vector最后一个元素为初值的元素
			++finish;	//调整水位
			copy_backward(position, finish - 2, finish - 1);	//position开始后移一位
			T x_copy = x;
			*position = x_copy;
		}
		else {
			const size_type old_size = size();
			// 如果原大小为0，则配置 1 个元素大小
			// 如果原大小不为0，则配置两倍原来的大小
			const size_type len = old_size != 0 ? 2 * old_size : 1;
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				//将原 vector 数据拷贝到新的 vector
				new_finish = uninitialized_copy(start, position, new_start);
				construct(new_finish, x);	//插入新元素
				++new_finish;	//调整水位
				new_finish = uninitialized_copy(position, finish, new_finish);	//拷贝剩余元素
			}
			catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			// 析构释放原 vector
			destroy_and_deallocate();
			// 调整迭代器，指向新的 vector
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}
	
	// insert_dispatch 函数
	template <class T, class Alloc>
	template <class Integer>
	void vector<T, Alloc>::insert_dispatch(iterator position, Integer n, Integer x,
		__true_type) {
		fill_insert(position, n, x);
	}

	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::insert_dispatch(iterator position, InputIterator first,
		InputIterator last, __false_type) {
		range_insert(position, first, last, iterator_category(first));
	}

	// fill_insert 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::fill_insert(iterator position, size_type n, const T& x) {
		if (n != 0) {
			if (size_type(end_of_storage - finish) >= n) {
				//备用空间大于等于增加的空间
				T x_copy = x;
				const size_type after_elems = finish - position;	//插入点后的元素个数
				iterator old_finish = finish;
				if (after_elems > n) {	//如果插入点后元素个数大于新增元素个数
					uninitialized_copy(finish - n, finish, finish);	//复制原元素
					finish += n;	//调整水位
					copy_backward(position, old_finish - n, old_finish);	//复制原剩余元素
					uninitialized_fill_n(position, n, x_copy);	//填充新元素
				}
				else {
					uninitialized_fill_n(finish, n - after_elems, x_copy);	//填充新元素
					finish += n - after_elems;	//调整水位
					uninitialized_copy(position, old_finish, finish);	//复制原元素
					finish += after_elems;	//调整水位
					uninitialized_fill(position, old_finish, x_copy);	//填充新元素
				}
			}
			else {
				//新长度：旧长度的两倍，旧长度+新增元素个数，取较大值
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);
				//配置新的 vector 空间
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try {
					// 先将旧 vector 插入点前的元素复制到新 vector 空间
					new_finish = uninitialized_copy(start, position, new_start);
					// 将新增元素加入新空间
					new_finish = uninitialized_fill_n(new_finish, n, x);
					// 将旧 vector 插入点后的元素复制到新空间
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (...) {
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				// 析构释放原 vector
				destroy_and_deallocate();
				//调整迭代器，指向新的 vector
				start = new_start;
				finish = new_finish;
				end_of_storage = start + len;
			}
		}
	}
	
	// range_insert 函数
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::range_insert(iterator position, InputIterator first,
		InputIterator last, input_iterator_tag) {
		for (; first != last; ++first) {
			position = insert(position, *first);
			++position;
		}
	}

	template <class T, class Alloc>
	template <class ForwardIterator>
	void vector<T, Alloc>::range_insert(iterator position, ForwardIterator first,
		ForwardIterator last, forward_iterator_tag) {
		if (first != last) {
			size_type n = distance(first, last);
			if (size_type(end_of_storage - finish) >= n) {
				const size_type after_elems = finish - position;
				iterator old_finish = finish;
				if (after_elems > n) {
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					copy(first, last, position);
				}
				else {
					ForwardIterator mid = first;
					advance(mid, after_elems);
					uninitialized_copy(mid, last, finish);
					finish += n - after_elems;
					uninitialized_copy(position, old_finish, finish);
					finish += after_elems;
					copy(first, mid, position);
				}
			}
			else {
				//新长度：旧长度的两倍，旧长度+新增元素个数，取较大值
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);
				//配置新的 vector 空间
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try {
					new_finish = uninitialized_copy(start, position, new_start);
					new_finish = uninitialized_copy(first, last, new_finish);
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (...) {
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				// 析构释放原 vector
				destroy_and_deallocate();
				//调整迭代器，指向新的 vector
				start = new_start;
				finish = new_finish;
				end_of_storage = start + len;
			}
		}
	}
	
	// 重载比较操作符
	template <class T, class Alloc>
	inline bool operator==(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return x.size() == y.size() && equal(x.begin(), x.end(), y.begin(), y.end());
	}

	template <class T, class Alloc>
	inline bool operator<(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return lexicographical_compare(x.begin(), x.end(), y.begin(), x.end());
	}

	template <class T, class Alloc>
	inline bool operator!=(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return !(x == y);
	}

	template <class T, class Alloc>
	inline bool operator>(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return y < x;
	}

	template <class T, class Alloc>
	inline bool operator<=(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return !(y < x);
	}

	template <class T, class Alloc>
	inline bool operator>=(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return !(x < y);
	}
	
	// 重载 MyTinySTL 的 swap
	template <class T, class Alloc>
	inline void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
		x.swap(y);
	}
}
#endif // !VECTOR_H

