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
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type*			iterator;
		typedef const value_type*	const_iterator;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

	protected:
		typedef Alloc	data_allocator;	//vector 的空间配置器
		typedef reverse_iterator<iterator>	reverse_iterator;	//反向迭代器

	protected:
		iterator start;				//表示目前使用空间的头
		iterator finish;			//表示目前使用空间的尾
		iterator end_of_storage;	//表示目前可用空间的尾

		// 插入元素
		void insert_aux(iterator position, const T& x);

		void deallocate() {
			if (start)
				data_allocator::deallocate(start, end_of_storage - start);
		}

		// 初始化：插入 n 个值为 value 的元素
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		// 配置空间并填充内容
		void allocate_and_fill(size_type n, class T& value) {
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, value);
			return result;
		}

		// 配置空间并复制内容
		iterator allocate_and_copy(size_type n, const_iterator first, const_iterator last) {
			iterator result = data_allocator::allocate(n);
			try {
				uninitialized_copy(first, last, result);
				return result;
			}
		}

	public:
		// 构造函数
		vector() :start(0), finish(0), end_of_storage(0) {}

		vector(size_type n, const T& value) {
			fill_initialize(n, value);
		}

		vector(int n, const T& value) {
			fill_initialize(n, value);
		}

		vector(long n, const T& value) {
			fill_initialize(n, value);
		}

		explicit vector(size_type n) {
			fill_initialize(n, T());
		}

		// 复制构造函数
		vector(const vector<T, Alloc>& x) {
			fill_initialize(x.size(), T());
			finish = uninitialized_copy(x.begin(), x.end(), start);
		}

		// 赋值操作符 operator=
		vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& x);

		// 析构函数
		~vector() {
			destroy(start, finish);
			deallocate();
		}

	public:
		// 返回第一个元素的迭代器
		iterator begin() { 
			return start; 
		}

		// 返回最后一个元素下一位置的迭代器
		iterator end() { 
			return finish; 
		}

		// 返回尾部的逆向迭代器
		reverse_iterator rbegin() {
			return reverse_iterator(end());
		}

		// 返回头部的逆向迭代器
		reverse_iterator rend() {
			return reverse_iterator(begin());
		}

		// 返回元素的个数
		size_type size() const { 
			return size_type(end() - begin()); 
		}

		// 返回容器最大可容纳个数
		size_type max_size() const {
			return size_type(-1) / sizeof(T);
		}
		// 返回容器的大小
		size_type capacity() const {
			return size_type(end_of_storage - begin());
		}

		// 判断容器是否为空
		bool empty() const {
			return begin() == end();
		}

		// 使用下标取元素
		reference operator[](size_type n) {
			return *(begin() + n);
		}

		// 取第 n 个元素 (n 从 0 开始)
		reference at(size_type n) {
			return (*this)[n];
		}

		// 第一个元素
		reference front() {
			return *begin();
		}

		// 最后一个元素
		reference back() {
			return *(end() - 1);
		}

		// 将元素插入到最尾端
		void push_back(const T& x) {
			if (finish != end_of_storage) {
				construct(finish, x);
				++finish;
			}
			else
				insert_aux(end(), x);
		}

		// 将最尾端元素取出
		void pop_back() {
			--finish;
			destroy(finish);
		}

		// 清除某位置上的元素
		iterator earse(iterator position) {
			if (position + 1 != end())
				copy(position + 1, finish, position);	//后面的元素往前移
			--finish;
			destroy(finish);
			return position;	//返回被删除的下一个元素
		}

		// 清除[first,last)上的元素
		iterator earse(iterator first, iterator last) {
			iterator i = copy(last, finish, first);	//把last之后的元素复制到first为起始的空间
			destroy(i, finish);	//析构清除不要的元素
			data_allocator::deallocate(i, finish);
			finish = finish - (last - first);	//调整水位
			return first;
		}

		// 重新配置容器大小
		void resize(size_type new_size, const T& x) {
			if (new_size < size())
				earse(begin() + new_size, end());
			else
				insert(end(), new_size - size(), x);
		}
		void resize(size_type new_size) {
			return resize(new_size, T());
		}

		// 清空容器
		void clear() {
			earse(begin(), end());
		}

		// 插入 n 个元素
		void insert(iterator position, size_type n, const T& x);

		// 交换两个 vector
		void swap(vector<T, Alloc>& x) {
			swap(start, x.start);
			swap(finish, x.finish);
			swap(end_of_storage, x.end_of_storage);
		}
	};

	// 重载 swap
	template <class T, class Alloc = allocator<T>>
	inline void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
		x.swap(y);
	}								 

	// 重载比较操作符
	template <class T, class Alloc = allocator<T>>
	inline bool operator==(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
	}

	template <class T, class Alloc = allocator<T>>
	inline bool operator<(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return lexicographical_compare(x.begin(), x.end(), y.begin(), x.end());
	}

	template <class T, class Alloc = allocator<T>>
	inline bool operator!=(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return !(x == y);
	}

	template <class T, class Alloc = allocator<T>>
	inline bool operator>(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return y < x;
	}

	template <class T, class Alloc = allocator<T>>
	inline bool operator<=(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return !(y < x);
	}

	template <class T, class Alloc = allocator<T>>
	inline bool operator>=(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
		return !(x < y);
	}

	// 赋值操作符 operator=
	template <class T, class Alloc = allocator<T>>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& x) {
		if (&x != this) {
			const size_type xlen = x.size();
			if (xlen > capacity()) {	//如果要赋值的 vector 大小超过原 vector 容量大小
				iterator tmp = allocate_and_copy(xlen, x.begin(), x.end());
				destroy(start, finish);
				deallocate();
				start = tmp;
				end_of_storage = start + xlen;
			}
			else if (size() >= xlen) {	//如果原 vector 大小大于等于要赋值的 vector 大小
				iterator i = copy(x.begin(), x.end(), begin());
				destroy(i, finish);
			}
			else { //原 vector 大小小于要赋值的 vector 大小
				copy(x.begin(), x.begin() + size(), start);
				uninitialized_copy(x.begin() + size(), x.end(), finish);
			}
			finish = start + xlen;
		}
		return *this;
	}

	// insert_aux 函数
	template <class T, class Alloc = allocator<T>>
	void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
		if (position + 1 != end()) {	//如果还有备用空间
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
			destroy(start, finish);
			deallocate();

			// 调整迭代器，指向新的 vector
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}

	// insert 函数
	template <class T, class Alloc = allocator<T>>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
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
					data_allocator::deallocate(new_start, new_finish);
					throw;
				}

				// 析构释放原 vector
				destroy(start, finish);
				deallocate();

				//调整迭代器，指向新的 vector
				start = new_start;
				finish = new_finish;
				end_of_storage = start + len;
			}
		}
	}
}
#endif // !VECTOR_H

