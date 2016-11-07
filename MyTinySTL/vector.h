#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

// 这个头文件包含一个模板类 vector
// vector 是一个能存放任意数据类型的动态数组，是一个具有较全面操作与算法的容器

#include "iterator.h"
#include "memory.h"

namespace mystl {

	// 模板类: vector
	// 接受两个参数，参数一代表数据类型，参数二代表空间配置器类型，缺省使用 mystl 的 alloc
	// 使用方法与 STL vector 类似
	template <class T, class Alloc = alloc>
	class vector {
	public:
		// vector 的嵌套型别定义
		typedef T					value_type;
		typedef Alloc				allocator_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

		typedef value_type*			iterator;
		typedef const value_type*	const_iterator;		
		typedef reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef reverse_iterator<iterator>	reverse_iterator;	

	public:
		typedef allocator<T, Alloc>	data_allocator;	//vector 的空间配置器
		allocator_type get_allocator() { return allocator_type(); }

	private:
		iterator start_;			//表示目前使用空间的头
		iterator finish_;			//表示目前使用空间的尾
		iterator end_of_storage_;	//表示目前可用空间的尾

	public:
		// 构造、复制、移动、析构函数
		vector() :start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {}
		explicit vector(size_type n) { __allocate_and_fill(n, T()); }
		vector(size_type n, const T& value) { __allocate_and_fill(n, value); }
		template <class InputIterator>
		vector(InputIterator first, InputIterator last);

		vector(const vector& rhs);
		vector(vector&& rhs);

		vector& operator=(const vector& rhs);
		vector& operator=(vector&& rhs);

		~vector() { __destroy_and_deallocate(); }

	public:
		// 迭代器相关操作
		iterator begin() { return start_; }
		const_iterator begin() const { return start_; }
		iterator end() { return finish_; }
		const_iterator end() const { return finish_; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

		// 容量相关操作
		bool empty() const { return begin() == end(); }
		size_type size() const { return static_cast<size_type>(end() - begin()); }
		size_type max_size() const { return static_cast<size_type>(-1) / sizeof(T); }
		size_type capacity() const { return static_cast<size_type>(end_of_storage_ - begin()); }
		void shrink_to_fit();
		void reserve(size_type n);

		// 访问元素相关操作
		reference operator[](size_type n) { return *(begin() + n); }
		const_reference operator[](size_type n) const { return *(begin() + n); }
		reference at(size_type n) { return (*this)[n]; }
		const_reference at(size_type n)	const { return (*this)[n]; }
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }
		pointer data() { return begin(); }
		const_pointer data() const { return begin(); }

		// 修改容器相关操作
		void assign(size_type n, const T& value) { __fill_assign(n, value); }
		void assign(size_type n) { __fill_assign(n, T()); }
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		void push_back(const T& x);
		void pop_back();
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void clear() { erase(begin(), end()); }
		iterator insert(iterator position, const T& x);
		iterator insert(iterator position);
		void insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		void resize(size_type new_size, const T& x);
		void resize(size_type new_size) { return resize(new_size, T()); }
		void reverse() { mystl::reverse(begin(), end()); }
		void swap(vector& rhs);

	private:
		// vector 成员函数
		template <class Integer>
		void __vector_construct(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __vector_construct(InputIterator first, InputIterator last, __false_type);
		void __destroy_and_deallocate();
		void __allocate_and_fill(size_type n, const T& value);
		template <class InputIterator>
		void __allocate_and_copy(InputIterator first, InputIterator last);
		void __fill_assign(size_type n, const T& value);
		template <class Integer>
		void __assign_dispatch(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __assign_dispatch(InputIterator first, InputIterator last, __false_type);
		template <class InputIterator>
		void __assign_aux(InputIterator first, InputIterator last, input_iterator_tag);
		template <class ForwardIterator>
		void __assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
		void __insert_aux(iterator position, const T& x);
		template <class Integer>
		void __insert_dispatch(iterator position, Integer n, Integer x, __true_type);
		template <class InputIterator>
		void __insert_dispatch(iterator position, InputIterator first, InputIterator last,
			__false_type);
		void __fill_insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void __range_insert(iterator position, InputIterator first, InputIterator last,
			input_iterator_tag);
		template <class ForwardIterator>
		void __range_insert(iterator position, ForwardIterator first, ForwardIterator last,
			forward_iterator_tag);
	};

	/******************************************************************************************/

	// 构造函数
	template <class T, class Alloc>
	template <class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer Integer;
		__vector_construct(first, last, Integer());
	}

	// 复制构造函数
	template <class T, class Alloc>
	vector<T, Alloc>::vector(const vector& rhs) {
		__allocate_and_copy(rhs.start_, rhs.finish_);
	}

	// 移动构造函数
	template <class T, class Alloc>
	vector<T, Alloc>::vector(vector&& rhs) {
		start_ = rhs.start_;
		finish_ = rhs.finish_;
		end_of_storage_ = rhs.end_of_storage_;
		rhs.start_ = rhs.finish_ = rhs.end_of_storage_ = nullptr;
	}
	
	// 复制赋值运算符
	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& rhs) {
		if (this != &rhs) {
			const auto xlen = rhs.size();
			if (xlen > capacity()) {	//如果要赋值的 vector 大小超过原 vector 容量大小
				__destroy_and_deallocate();
				__allocate_and_copy(rhs.begin(), rhs.end());
			}
			else if (size() >= xlen) {	//如果原 vector 大小大于等于要赋值的 vector 大小
				auto i = mystl::copy(rhs.begin(), rhs.end(), begin());
				mystl::destroy(i, finish_);
				finish_ = start_ + xlen;
			}
			else { //原 vector 大小小于要赋值的 vector 大小
				mystl::copy(rhs.begin(), rhs.begin() + size(), start_);
				mystl::uninitialized_copy(rhs.begin() + size(), rhs.end(), finish_);
				end_of_storage_ = finish_ = start_ + xlen;
			}
		}
		return *this;
	}

	// 移动赋值运算符
	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& rhs) {
		if (this != &rhs) {
			__destroy_and_deallocate();	//释放原资源
			start_ = rhs.start_;
			finish_ = rhs.finish_;
			end_of_storage_ = rhs.end_of_storage_;
			rhs.start_ = rhs.finish_ = rhs.end_of_storage_ = nullptr;
		}
		return *this;
	}
	
	// 放弃额外的容量
	template <class T, class Alloc>
	void vector<T, Alloc>::shrink_to_fit() {
		if (finish_ != end_of_storage_) {
			data_allocator::deallocate(finish_ + 1, end_of_storage_ - finish_);
			end_of_storage_ = finish_;
		}
	}

	// 重新配置大小
	template <class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type n) {
		if (capacity() < n) {
			const auto old_size = size();
			auto tmp = data_allocator::allocate(n);
			mystl::uninitialized_copy(start_, finish_, tmp);
			__destroy_and_deallocate();
			start_ = tmp;
			finish_ = tmp + old_size;
			end_of_storage_ = start_ + n;
		}
	}

	// 初始化容器
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer Integer;
		__assign_dispatch(first, last, Integer());
	}

	// 在容器尾部插入元素
	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(const T& x) {
		if (finish_ != end_of_storage_) {
			mystl::construct(finish_, x);
			++finish_;
		}
		else
			__insert_aux(end(), x);
	}

	// 删除最后一个元素
	template <class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		--finish_;
		mystl::destroy(finish_);
	}

	// 删除 position 位置上的元素
	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::erase(iterator position) {
		if (position + 1 != end())
			mystl::copy(position + 1, finish_, position);	//后面的元素往前移
		--finish_;
		mystl::destroy(finish_);
		return position;	//返回被删除的下一个元素
	}

	// 删除[first, last)上的元素
	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::erase(iterator first, iterator last) {
		auto i = mystl::copy(last, finish_, first);	//把last之后的元素复制到first为起始的空间
		mystl::destroy(i, finish_);	//销毁不要的元素
		finish_ = finish_ - (last - first);	//调整水位
		return first;
	}

	// 在 position 位置插入元素									 
	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(iterator position, const T& x) {
		auto n = position - begin();
		if (finish_ != end_of_storage_ && position == end()) {
			mystl::construct(finish_, x);
			++finish_;
		}
		else
			__insert_aux(position, x);
		return begin() + n;
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(iterator position) {
		auto n = position - begin();
		if (finish_ != end_of_storage_ && position == end()) {
			mystl::construct(finish_);
			++finish_;
		}
		else
			__insert_aux(position, T());
		return begin() + n;
	}

	// 在 position 位置开始插入 n 个元素
	template <class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
		__fill_insert(position, n, x);
	}

	// 在 position 位置插入[first, last)内的元素
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::insert(iterator position,
		InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer Integer;
		__insert_dispatch(position, first, last, Integer());
	}

	// 重置容器大小
	template <class T, class Alloc>
	void vector<T, Alloc>::resize(size_type new_size, const T& x) {
		if (new_size < size())
			erase(begin() + new_size, end());
		else
			insert(end(), new_size - size(), x);
	}

	// 与另一个 vector 交换
	template <class T, class Alloc>
	void vector<T, Alloc>::swap(vector<T, Alloc>& rhs) {
		if (this != &rhs) {
			mystl::swap(start_, rhs.start_);
			mystl::swap(finish_, rhs.finish_);
			mystl::swap(end_of_storage_, rhs.end_of_storage_);
		}
	}

	// __vector_construct 函数
	template <class T, class Alloc>
	template <class Integer>
	void vector<T, Alloc>::__vector_construct(Integer n, Integer value, __true_type) {
		__allocate_and_fill(static_cast<size_type>(n), static_cast<T>(value));
	}

	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::__vector_construct(InputIterator first, InputIterator last, __false_type) {
		__allocate_and_copy(first, last);
	}

	// __destroy_and_deallocate 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::__destroy_and_deallocate() {
		mystl::destroy(start_, finish_);
		data_allocator::deallocate(start_, end_of_storage_ - start_);
	}

	// __allocate_and_fill 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::__allocate_and_fill(size_type n, const T& value) {
		start_ = data_allocator::allocate(n);
		finish_ = mystl::uninitialized_fill_n(start_, n, value);
		end_of_storage_ = finish_;
	}

	// __allocate_and_copy 函数
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::__allocate_and_copy(InputIterator first, InputIterator last) {
		auto n = last - first;
		start_ = data_allocator::allocate(static_cast<size_type>(n));
		finish_ = mystl::uninitialized_copy(first, last, start_);
		end_of_storage_ = finish_;
	}
	
	// __fill_assign 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::__fill_assign(size_type n, const T& value) {
		if (n > capacity()) {
			vector<T, Alloc> tmp(n, value);
			tmp.swap(*this);
		}
		else if (n > size()) {
			mystl::fill(begin(), end(), value);
			finish_ = mystl::uninitialized_fill_n(finish_, n - size(), value);
		}
		else
			erase(mystl::fill_n(start_, n, value), finish_);
	}
	
	// __assign_dispatch 函数
	template <class T, class Alloc>
	template <class Integer>
	void vector<T, Alloc>::__assign_dispatch(Integer n, Integer value, __true_type) {
		__fill_assign(static_cast<size_type>(n), static_cast<T>(value));
	}

	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::__assign_dispatch(InputIterator first, InputIterator last, __false_type) {
		__assign_aux(first, last, iterator_category(first));
	}

	// __assign_aux 函数
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::__assign_aux(InputIterator first, InputIterator last,
		input_iterator_tag) {
		auto cur = begin();
		for (; first != last && cur != end(); ++first, ++cur) {
			*cur = *first;
		}
		if (first == last)	//如果[first, last)区间内的元素复制完成
			erase(cur, end());	//删除多余的元素
		else
			insert(end(), first, last);	//否则插入区间剩余元素
	}

	template <class T, class Alloc>
	template <class ForwardIterator>
	void vector<T, Alloc>::__assign_aux(ForwardIterator first, ForwardIterator last,
		forward_iterator_tag) {
		auto len = distance(first, last);
		if (static_cast<size_type>(len) > capacity()) {	//如果区间长度大于容器容量
			__destroy_and_deallocate();	//销毁原来的容器
			__allocate_and_copy(first, last);	//重新分配空间并复制区间
		}
		else if (size() >= static_cast<size_type>(len)) {	//如果容器大小大于等于区间长度
			auto new_finish = mystl::copy(first, last, start_);	//复制区间到起始处
			mystl::destroy(new_finish, finish_);	//销毁多余的元素
			finish_ = new_finish;	//调整 finish_ 水位
		}
		else {	//如果区间长度大于容器大小并且小于容器容量
			auto mid = first;
			advance(mid, size()); //以容器大小把区间分为前后段
			mystl::copy(first, mid, start_);	//先复制前一段区间
			finish_ = mystl::uninitialized_copy(mid, last, finish_); //再复制后一段区间
		}
	}

	// __insert_aux 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::__insert_aux(iterator position, const T& x) {
		if (finish_ != end_of_storage_) {	//如果还有备用空间
			// 在备用空间构造一个以 vector 最后一个元素为初值的元素
			mystl::construct(finish_, *(finish_ - 1));
			++finish_;	//调整水位
			mystl::copy_backward(position, finish_ - 2, finish_ - 1);	//后移一位
			auto x_copy = x;
			*position = x_copy;	//将 position 位置元素修改为新值
		}
		else {
			const auto old_size = size();
			// 如果原大小为 0，则配置 1 个元素大小，否则配置两倍原来的大小
			const auto len = old_size == 0 ? 1 : 2 * old_size;
			auto new_start = data_allocator::allocate(len);
			auto new_finish = new_start;
			try {
				// 将原 vector 数据拷贝到新的 vector
				new_finish = mystl::uninitialized_copy(start_, position, new_start);
				mystl::construct(new_finish, x);	//插入新元素
				++new_finish;	//调整水位
				// 将剩余元素拷贝到新的 vector
				new_finish = mystl::uninitialized_copy(position, finish_, new_finish);	
			}
			catch (...) {
				mystl::destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
			}
			__destroy_and_deallocate();	//析构释放原 vector
			start_ = new_start;	//调整迭代器，指向新的 vector
			finish_ = new_finish;
			end_of_storage_ = new_start + len;
		}
	}
	
	// __insert_dispatch 函数
	template <class T, class Alloc>
	template <class Integer>
	void vector<T, Alloc>::__insert_dispatch(iterator position, Integer n, Integer x, __true_type) {
		__fill_insert(position, static_cast<size_type>(n), static_cast<T>(x));
	}

	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::__insert_dispatch(iterator position, InputIterator first,
		InputIterator last, __false_type) {
		__range_insert(position, first, last, mystl::iterator_category(first));
	}

	// __fill_insert 函数
	template <class T, class Alloc>
	void vector<T, Alloc>::__fill_insert(iterator position, size_type n, const T& x) {
		if (n != 0) {
			if (static_cast<size_type>(end_of_storage_ - finish_) >= n) {
				// 如果备用空间大于等于增加的空间
				auto x_copy = x;
				const auto after_elems = finish_ - position;	//插入点后的元素个数
				auto old_finish = finish_;
				if (static_cast<size_type>(after_elems) > n) {	//如果插入点后元素个数大于新增元素个数
					mystl::uninitialized_copy(finish_ - static_cast<difference_type>(n), finish_, finish_);	//复制原元素
					finish_ += static_cast<difference_type>(n);	//调整水位
					mystl::copy_backward(position, old_finish - static_cast<difference_type>(n), old_finish);	//复制剩余元素
					mystl::fill_n(position, n, x_copy);	//填充新元素
				}
				else {
					mystl::uninitialized_fill_n(finish_, n - static_cast<size_type>(after_elems), x_copy);	//填充新元素
					finish_ += static_cast<difference_type>(n) - after_elems;	//调整水位
					mystl::uninitialized_copy(position, old_finish, finish_);	//复制原元素
					finish_ += after_elems;	//调整水位
					mystl::fill(position, old_finish, x_copy);	//填充新元素
				}
			}
			else {	//备用空间不足，需要重新配置空间
				// 新长度在 旧长度的两倍，旧长度+新增元素个数 中取较大值
				const auto old_size = size();
				const auto len = old_size + mystl::max(old_size, n);
				// 配置新的 vector 空间
				auto new_start = data_allocator::allocate(len);
				auto new_finish = new_start;
				try {
					// 先将旧 vector 插入点前的元素复制到新 vector 空间
					new_finish = mystl::uninitialized_copy(start_, position, new_start);
					// 将新增元素加入新空间
					new_finish = mystl::uninitialized_fill_n(new_finish, n, x);
					// 将旧 vector 插入点后的元素复制到新空间
					new_finish = mystl::uninitialized_copy(position, finish_, new_finish);
				}
				catch (...) {
					mystl::destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
				}
				__destroy_and_deallocate();	//析构释放原 vector
				start_ = new_start;	//调整迭代器，指向新的 vector
				finish_ = new_finish;
				end_of_storage_ = start_ + len;
			}
		}
	}
	
	// __range_insert 函数
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::__range_insert(iterator position, InputIterator first,
		InputIterator last, input_iterator_tag) {
		for (; first != last; ++first) {	//一个一个元素插入
			position = insert(position, *first);
			++position;
		}
	}

	template <class T, class Alloc>
	template <class ForwardIterator>
	void vector<T, Alloc>::__range_insert(iterator position, ForwardIterator first,
		ForwardIterator last, forward_iterator_tag) {
		if (first != last) {
			auto n = distance(first, last);
			if ((end_of_storage_ - finish_) >= n) { //如果剩余可用大小大于插入元素个数
				const auto after_elems = finish_ - position; //position 后面的元素个数
				auto old_finish = finish_;
				if (after_elems > n) {	//如果 position 后面的元素个数大于插入元素个数
					// 先把最后 n 个元素复制到 finish_ 为起始的位置
					mystl::uninitialized_copy(finish_ - n, finish_, finish_);
					finish_ += n;
					// 原剩余元素接着后移
					mystl::copy_backward(position, old_finish - n, old_finish);
					mystl::copy(first, last, position); //将要插入的元素复制到 position 处
				}
				else {	//如果 position 后面的元素个数小于等于插入元素个数
					auto mid = first;
					advance(mid, after_elems);	//以 after_elems 把要插入的区间分为前后段
					// 把后段复制到 finish_ 为起始的位置
					mystl::uninitialized_copy(mid, last, finish_);	
					finish_ += n - after_elems;	//调整水位
					// 原元素复制到 finish_ 为起始的位置
					mystl::uninitialized_copy(position, old_finish, finish_);
					finish_ += after_elems;	//调整水位
					mystl::copy(first, mid, position); //复制前半段到 position 位置
				}
			}
			else {	//如果剩余可用大小小于插入元素个数
				// 新长度在 旧长度的两倍，旧长度+新增元素个数 中取较大值
				const auto old_size = size();
				const auto len = old_size + mystl::max(old_size, static_cast<size_type>(n));
				// 配置新的 vector 空间
				auto new_start = data_allocator::allocate(len);
				auto new_finish = new_start;
				try {
					// 原 vector 的前半段复制到新的 vector 空间
					new_finish = mystl::uninitialized_copy(start_, position, new_start);
					// 把插入区间复制到新 vector 空间尾端
					new_finish = mystl::uninitialized_copy(first, last, new_finish);
					// 原 vector 的后半段复制到新的 vector 空间尾端
					new_finish = mystl::uninitialized_copy(position, finish_, new_finish);
				}
				catch (...) {
					mystl::destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
				}
				__destroy_and_deallocate();	//析构释放原 vector
				start_ = new_start;	//调整迭代器，指向新的 vector
				finish_ = new_finish;
				end_of_storage_ = start_ + len;
			}
		}
	}
	
	// 重载比较操作符
	template <class T, class Alloc>
	inline bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <class T, class Alloc>
	inline bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
	}

	template <class T, class Alloc>
	inline bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template <class T, class Alloc>
	inline bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return rhs < lhs;
	}

	template <class T, class Alloc>
	inline bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return !(rhs < lhs);
	}

	template <class T, class Alloc>
	inline bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return !(lhs < rhs);
	}
	
	// 重载 mystl 的 swap
	template <class T, class Alloc>
	inline void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}
#endif // !MYTINYSTL_VECTOR_H_

