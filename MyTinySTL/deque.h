#ifndef DEQUE_H
#define DEQUE_H

#include "iterator.h"
#include "reverse_iterator.h"
#include "memory.h"

namespace MyTinySTL {

	// 这是一个全局函数，决定缓冲区的大小
	inline size_t __deque_buf_size(size_t n, size_t sz) {
		return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
	}

	// deque 的迭代器设计
	template <class T, class Ref, class Ptr, size_t BufSiz>
	struct __deque_iterator : public iterator<random_access_iterator_tag, T> {
		typedef __deque_iterator<T, T&, T*>	iterator;
		typedef __deque_iterator<T, const T&, const T*>	const_iterator;
		static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }

		typedef T	value_type;
		typedef Ptr	pointer;
		typedef Ref	reference;
		typedef size_t	size_type;
		typedef T** map_pointer;

		typedef __deque_iterator self;

		// 保持与容器的联结
		T*	cur;	//指向缓冲区当前元素
		T*	first;	//指向缓冲区的头
		T*	last;	//指向缓冲区的尾
		map_pointer node;	//控制中心

		// 构造函数
		__deque_iterator() :cur(0), first(0), last(0), node(0) {}
		__deque_iterator(T* x, map_pointer y)
			:cur(x), first(*y), last(*y + buffer_size()), node(y) {}
		__deque_iterator(const iterator& x)
			:cur(x.cur), first(x.first), last(x.last), node(x.node) {}

		// 这个函数用于跳出一个缓冲区
		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		// 重载运算符
		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }
		difference_type operator-(const self& x) const {
			return difference_type(buffer_size()) * (node - x.node - 1) +
				(cur - first) + (x.last - x.cur);
		}

		self& operator++() {
			++cur;
			if (cur == last) {	//如果到达缓冲区的尾
				set_node(node + 1);	//跳到下一个缓冲区
				cur = first;
			}
			return *this;
		}

		self& operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}

		self& operator--() {
			if (cur == first) {	//如果到达缓冲区的头
				set_node(node - 1);	//跳到前一个缓冲区
				cur = last;
			}
			--cur;
			return *this;
		}

		self& operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}

		self& operator+=(difference_type n) {
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				// 增进后还在同一个缓冲区
				cur += n;
			else {
				difference_type node_offset = 
					offset > 0 ? offset / difference_type(buffer_size()) 
					: -difference_type((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = first + (offset - node_offset*difference_type(buffer_size()));
			}
			return *this;
		}

		self& operator+(difference_type n) const {
			self tmp = *this;
			return tmp += n;
		}

		self& operator-=(difference_type n) const {
			return *this += -n;
		}

		self& operator-(difference_type n) const {
			self tmp = *this;
			return tmp -= n;
		}

		reference operator[](difference_type n) const {
			return *(*this + n);
		}

		bool operator==(const self& x) const {
			return cur == x.cur;
		}
		 
		bool operator!=(const self& x) const {
			return !(*this == x);
		}

		bool operator<(const self& x) const {
			return node == x.node ? (cur < x.cur) : (node < x.node);
		}

		bool operator>(const self& x) const {
			return x < *this;
		}

		bool operator<=(const self& x) const {
			return !(x < *this);
		}

		bool operator>=(const self& x) const {
			return !(*this < x);
		}
	};

	// deque:双端队列
	template <class T, class Alloc = alloc, size_t BufSiz = 0>
	class deque {
	public:
		// deque 的嵌套型别定义
		typedef T	value_type;
		typedef Alloc	allocate_type;
		typedef	value_type*	pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&	reference;
		typedef	const value_type&	const_reference;
		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;

		typedef __deque_iterator<T, T&, T*, BufSiz>	iterator;
		typedef __deque_iterator<T, const T&, const T*, BufSiz>	const_iterator;
		typedef reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef reverse_iterator<iterator>	reverse_iterator;

		static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }

	public:
		typedef	allocator<T, Alloc>	data_allocator;	//元素的配置器
		typedef allocator<T*, Alloc>	map_allocator;	//缓冲区的配置器

	protected:
		typedef pointer*	map_pointer;
		iterator start;		//指向第一个节点
		iterator finish;	//指向最后一个结点
		map_pointer map;	//指向一块 map 区域，map 中的每个元素都是一个指针，指向一个缓冲区
		size_type map_size;	//map 内指针的数目
		enum { __initial_map_size = 8 };	//map 的初始化大小

	public:
		// 构造函数
		deque() { __fill_initialize(size_type(), T()); }
		deque(size_type n, const T& value) { __fill_initialize(n, value); }
		explicit deque(size_type n) { __fill_initialize(n, T()); }
		template <class InputIterator>
		deque(InputIterator first, InputIterator last);

		// 复制构造函数
		deque(const deque& x);

		// 赋值操作符
		deque& operator=(const deque& x);

		// 析构函数
		~deque();

	public:
		// 迭代器相关操作
		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		iterator end() { return finish; }
		const_iterator end() const { return finish; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return reverse_iterator(begin()); }

		// 容量相关操作
		bool empty() { return begin() == end(); }
		size_type size() const { return finish - start; }
		size_type max_size() const { return size_type(-1); }

		// 访问元素相关操作
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }
		reference operator[](size_type n) { return start[difference_type(n)]; }
		const_reference operator[](size_type n) const { return start[difference_type(n)]; }
		reference at(size_type n) { return (*this)[n]; }
		const_reference at(size_type n) const { return (*this)[n]; }

		// 调整容器相关操作
		void assign(size_type n, const T& value) { __fill_assign(n, value); }
		void assign(size_type n) { __fill_assign(n, T()); }
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		iterator insert(iterator position, const T& x);
		iterator insert(iterator position) { return insert(position, T()); }
		void insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		iterator clear();
		void push_back(const T& x);
		void push_front(const T& x);
		void pop_back();
		void pop_front();
		void resize(size_type new_size, const T& x);
		void resize(size_type new_size) { resize(new_size, T()); }
		void swap(const deque& x);

		// 配置器相关操作
		allocate_type get_allocate() { return allocate_type(); }

	protected:
		// 内部成员函数
		void __create_node(map_pointer nstart, map_pointer nfinish);
		void __destroy_node(map_pointer nstart, map_pointer nfinish);
		void __map_initialize(size_t nelem);
		void __fill_initialize(size_type n, const T& value);
		template <class InputIterator>
		void __range_initialize(InputIterator, InputIterator, input_iterator_tag);
		template <class ForwardIterator>
		void __range_initialize(ForwardIterator, ForwardIterator, forward_iterator_tag);
		template <class Integer>
		void __initizlize_dispatch(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __initizlize_dispatch(InputIterator first, InputIterator last, __false_type);
		void __fill_assign(size_type n, const T& value);
		template <class Integer>
		void __assign_dispatch(Integer n, Integer value, __true_type);
		template <class InputIterator>
		void __assign_dispatch(InputIterator first, InputIterator last, __false_type);
		template <class InputIterator>
		void __assign_aux(InputIterator first, InputIterator last, input_iterator_tag);
		template <class ForwardIterator>
		void __assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
		iterator __insert_aux(iterator position, const T& x);
		void __fill_insert(iterator position, size_type n, const T& x);
		void __fill_insert_aux(iterator position, size_type n, const T& x);
		template <class ForwardIterator>
		void __fill_insert_aux(iterator, ForwardIterator, ForwardIterator, size_type);
		template <class Integer>
		void __insert_dispatch(iterator, Integer, Integer, __true_type);
		template <class InputIterator>
		void __insert_dispatch(iterator, InputIterator, InputIterator, __false_type);
		template <class InputIterator>
		void __insert_dispatch_aux(iterator, InputIterator, InputIterator,
			size_type, input_iterator_tag);
		template <class ForwardIterator>
		void __insert_dispatch_aux(iterator, ForwardIterator, ForwardIterator,
			size_type, forward_iterator_tag);
		void __push_back_aux(const T& x);
		void __push_front_aux(const T& x);
		void __pop_back_aux();
		void __pop_front_aux();
		iterator __reserve_elements_at_back(size_type n);
		iterator __reserve_elements_at_front(size_type n);
		void __reserve_map_at_back(size_type n);
		void __reserve_map_at_front(size_type n);
		void __reallocate_map(size_type node_to_add, bool add_at_front);
	};

	// 构造函数
	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	deque<T, Alloc, BufSiz>::deque(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer integer;
		__initialize_dispatch(first, last, integer());
	}

	// 复制构造函数
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::deque(const deque<T, Alloc, BufSiz>& x) {
		__map_initialize(x.size());
		MyTinySTL::uninitialized_copy(x.begin(), x.end(), start);
	}

	// 赋值操作符operator=
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>& deque<T, Alloc, BufSiz>::operator=(const deque& x) {
		size_type len = size();
		if (*this != x) {
			if (len >= x.size())
				erase(MyTinySTL::copy(x.begin(), x.end(), start), finish);
			else {
				const_iterator mid = x.begin() + difference_type(len);
				copy(x.begin(), mid, start);
				insert(finish, mid, x.end());
			}
		}
		return *this;
	}

	// 析构函数
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::~deque() {
		data_allocator::destroy(start, finish);
		data_allocator::deallocate(start, finish - start);
	}

	// 初始化容器
	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::assign(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer integer;
		__assign_dispatch(first, last, integer());
	}

	// 在 position 位置插入元素
	template <class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator
		deque<T, Alloc, BufSiz>::insert(iterator position, const T& x) {
		if (position.cur == start.cur) {	//如果 position 位于头部
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur) {	//如果 position 位于尾部
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else
			return __insert_aux(position, x);
	}

	// 在 position 位置插入 n 个元素
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::insert(iterator position, size_type n, const T& x) {
		__fill_insert(position, n, x);
	}

	// 在 position 位置插入[first, last)内的元素
	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::insert(iterator position, InputIterator first,
		InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer integer;
		__insert_dispatch(position, first, last, integer());
	}

	// 删除 position 位置的元素
	template <class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator
		deque<T, Alloc, BufSiz>::erase(iterator position) {
		iterator next = position;
		++next;
		difference_type index = position - start;	//清除点之前的元素个数
		if (index < (size() >> 1)) {	//清除点之前的元素比较少
			MyTinySTL::copy_backward(start, position, next);
			pop_front();
		}
		else {	//清除点之后的元素比较少
			MyTinySTL::copy(next, finish, position);
			pop_back();
		}
		return start + index;
	}

	// 删除[first, last)上的元素
	template <class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator
		deque<T, Alloc, BufSiz>::erase(iterator first, iterator last) {
		if (first == start && last == finish) {
			clear();
			return finish;
		}
		else {
			difference_type n = last - first;	//清除区间长度
			difference_type elem_before = first - start;	//清除区间之前的元素个数
			if (elem_before < (size() - n) / 2) {	//清除区间前的元素较少
				MyTinySTL::copy_backward(start, first, last);
				iterator new_start = start + n;	//新起点
				data_allocator::destroy(start, new_start);	//析构多余元素
				//释放缓冲区
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
					data_allocator::deallocate(*cur, buffer_size());
				start = new_start;
			}
			else {	//清除区间后的元素较少
				MyTinySTL::copy(last, finish, first);
				iterator new_finish = finish - n;	//新结尾
				data_allocator::destroy(new_finish, finish);	//析构多余元素
				//释放缓冲区
				for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
					data_allocator::deallocate(*cur, buffer_size());
				finish = new_finish;
			}
			return start + elem_before;
		}
	}

	// 清空容器
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::clear() {
		// clear() 会保留至少一个缓冲区
		for (map_pointer cur = start.node + 1; cur < finish.node; ++cur) {
			data_allocator::destroy(*cur, *cur + buffer_size());
			data_allocator::deallocate(*cur, buffer_size());
		}
		if (start.node != finish.node) {	//有两个以上的缓冲区
			data_allocator::destroy(start.cur, start.last);
			data_allocator::destroy(finish.first, finish.cur);
			// 释放尾部的缓冲区，保留头部
			data_allocator::deallocate(finish.first, buffer_size());
		}
		else {	//只有一个缓冲区
			data_allocator::destroy(start.cur, finish.cur);
			// 保留唯一的缓冲区
		}
		finish = start;
	}

	// 在尾部插入元素
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_back(const T& x) {
		if (finish.cur != finish.last - 1) {
			data_allocator::construct(finish.cur, x);
			++finish.cur;
		}
		else
			__push_back_aux(x);
	}

	// 在头部插入元素
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_front(const T& x) {
		if (start.cur != start.first) {
			data_allocator::construct(start.cur, x);
			--start.cur;
		}
		else
			__push_front_aux(x);
	}

	// 弹出尾部元素
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_back() {
		if (finish.cur != finish.first) {
			--finish.cur;
			data_allocator::destroy(finish.cur);
		}
		else
			__pop_back_aux();
	}

	// 弹出头部元素
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_front() {
		if (start.cur != start.last - 1) {
			data_allocator::destroy(start.cur);
			++start.cur;
		}
		else
			__pop_front_aux();
	}

	// 重置容器大小
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::resize(size_type new_size, const T& x) {
		const size_type len = size();
		if (new_size < len)
			erase(start + new_size, finish);
		else
			insert(finish, len - new_size, x);
	}

	// 交换两个 deque 容器
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::swap(const deque& x) {
		MyTinySTL::swap(start, x.start);
		MyTinySTL::swap(finish, x.finish);
		MyTinySTL::swap(map, x.map);
		MyTinySTL::swap(map_size, x.map_size);
	}

	// __create_node 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__create_node(map_pointer nstart, map_pointer nfinish) {
		map_pointer cur;
		try {
			for (cur = nstart; cur < nfinish; ++cur)
				*cur = data_allocator::allocate(__deque_buf_size(BufSiz, sizeof(T)));
		}
		catch (...) {
			__destroy_node(nstart, nfinish);
		}
	}

	// __destroy_node 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__destroy_node(map_pointer nstart, map_pointer nfinish) {
		for (map_pointer n = nstart; n < nfinish; ++n) {
			data_allocator::deallocate(*n, __deque_buf_size(BufSiz, sizeof(T)));
		}
	}

	// __map_initialize 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__map_initialize(size_t nElem) {
		size_type nNode = nElem / buffer_size() + 1;	//需要分配的缓冲区个数
		map_size = MyTinySTL::max(__initial_map_size, nNode + 2);
		map = map_allocator::allocate(map_size);
		// 让 nstart 和 nfinish 都指向 map 最中央的区域，方便向头尾扩充
		map_pointer nstart = map + (map_size - nNode) / 2;
		map_pointer nfinish = nstart + nNode - 1;
		try {
			__create_node(nstart, nfinish);
		}
		catch (...) {
			map_allocator::deallocate(map, map_size);
			map = 0;
			map_size = 0;
		}
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + nElem % buffer_size();
	}

	// __fill_initialize 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__fill_initialize(size_type n, const T& value) {
		__map_initialize(n);
		map_pointer = cur;
		try {
			for (cur = start.node; cur < finish.node; ++cur)
				MyTinySTL::uninitialized_fill(*cur, *cur + buffer_size(), value);
			MyTinySTL::uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...) {
			data_allocator::destroy(start, iterator(*cur, cur));
		}
	}

	// __range_initialize 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::__range_initialize(InputIterator first, 
		InputIterator last, input_iterator_tag) {
		__map_initialize(0);
		try {
			for (; first != last; ++first)
				push_back(*first);
		}
		catch (...) {
			clear();
		}
	}

	template <class T, class Alloc, size_t BufSiz>
	template <class ForwardIterator>
	void deque<T, Alloc, BufSiz>::__range_initialize(ForwardIterator first,
		ForwardIterator last, forward_iterator_tag) {
		size_type n = distance(first, last);
		__map_initialize(0);
		map_pointer cur;
		try {
			for (cur = start.node; cur < finish.node; ++cur) {
				ForwardIterator mid = first; 
				advance(mid, buffer_size());
				MyTinySTL::uninitialized_copy(first, mid, *cur);
				first = mid;
			}
			MyTinySTL::uninitialized_copy(first, last, finish.first);
		}
		catch (...) {
			data_allocator::destroy(start, iterator(*cur, cur));
		}
	}

	// __initizlize_dispatch 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class Integer>
	void deque<T, Alloc, BufSiz>::__initizlize_dispatch(Integer n, Integer value,
		__true_type) {
		__fill_initialize(n, value);
	}

	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::__initizlize_dispatch(InputIterator first,
		InputIterator last, __false_type) {
		__range_initialize(first, last, iterator_category(first));
	}

	// __fill_assign 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__fill_assign(size_type n, const T& value) {

	}
	// __assign_dispatch 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class Integer>
	void deque<T, Alloc, BufSiz>::__assign_dispatch(Integer n, Integer value, __true_type) {
		__fill_assign((size_type)n, (value_type)value);
	}

	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::__assign_dispatch(InputIterator first,
		InputIterator last, __false_type) {
		__assign_aux(first, last, iterator_category(first));
	}

	// __assign_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::__assign_aux(InputIterator first, InputIterator last,
		input_iterator_tag) {

	}

	template <class T, class Alloc, size_t BufSiz>
	template <class ForwardIterator>
	void deque<T, Alloc, BufSiz>::__assign_aux(ForwardIterator first, ForwardIterator last,
		forward_iterator_tag) {
		size_type len = distance(first, last);
		if (len > size()) {
			ForwardIterator mid = first;
			advance(mid, size());
			MyTinySTL::copy(first, mid, start);
			insert(finish, mid, last);
		}
		else
			erase(MyTinySTL::copy(first, last, start), finish);
	}

	// __insert_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator
		deque<T, Alloc, BufSiz>::__insert_aux(iterator position, const T& x) {
		difference_type index = position - start;	//插入点之前的元素个数
		value_type x_copy = x;
		if (index < size() / 2) {	//如果插入点之前的元素较少
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			position = start + index;
			iterator pos1 = position;
			++pos1;
			MyTinySTL::copy(front2, pos1, front1);
		}
		else {	//如果插入点之后的元素较少
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			position = start + index;
			MyTinySTL::copy_backward(pos, back2, back1);
		}
		*position = x_copy;
		return position;
	}

	// __fill_insert 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__fill_insert(iterator position, size_type n, const T& x) {
		if (position.cur == start.cur) {
			iterator new_start = __reserve_elements_at_front(n);
			try {
				MyTinySTL::uninitialized_fill(new_start, start, x);
				start = new_start;
			}
			catch (...) {
				__destroy_node(new_start.node, start.node);
			}
		}
		else if (position.cur == finish.cur) {
			iterator new_finish = __reserve_elements_at_back(n);
			try {
				MyTinySTL::uninitialized_fill(finish, new_finish, x);
				finish = new_finish;
			}
			catch (...) {
				__destroy_node(finish.node + 1, new_finish.node + 1);
			}
		}
		else
			__fill_insert_aux(position, n, x);
	}

	// __fill_insert_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__fill_insert_aux(iterator position,
		size_type n, const T& x) {

	}

	template <class T, class Alloc, size_t BufSiz>
	template <class ForwardIterator>
	void deque<T, Alloc, BufSiz>::__fill_insert_aux(iterator position,
		ForwardIterator first, ForwardIterator last, size_type n) {

	}

	// __insert_dispatch 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class Integer>
	void deque<T, Alloc, BufSiz>::__insert_dispatch(iterator position,
		Integer n, Integer x, __true_type) {
		__fill_insert(position, n, x);
	}

	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::__insert_dispatch(iterator position,
		InputIterator first, InputIterator last, __false_type) {
		__insert_dispatch_aux(position, first, last, iterator_category(first));
	}

	// __insert_dispatch_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::__insert_dispatch_aux(iterator position,
		InputIterator first, InputIterator last, size_type, input_iterator_tag) {

	}

	template <class T, class Alloc, size_t BufSiz>
	template <class ForwardIterator>
	void deque<T, Alloc, BufSiz>::__insert_dispatch_aux(iterator position,
		ForwardIterator first, ForwardIterator last, size_type, forward_iterator_tag) {

	}

	// __push_back_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__push_back_aux(const T& x) {
		value_type x_copy = x;
		__reserve_map_at_back();
		*(finish.node + 1) = data_allocator::allocate(__deque_buf_size(sizeof(T)));
		try {
			data_allocator::construct(finish.cur, x_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		catch (...) {
			data_allocator::deallocate(*(finish.node + 1));
		}
	}

	// __push_front_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__push_front_aux(const T& x) {
		value_type x_copy = x;
		__reserve_map_at_front();
		*(start.node - 1) = data_allocator::allocate(__deque_buf_size(sizeof(T)));
		try {
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			data_allocator::construct(start.cur, x_copy);
		}
		catch (...) {
			++start;
			data_allocator::deallocate(*(start.node - 1));
		}
	}

	// __pop_back_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__pop_back_aux() {
		data_allocator::deallocate(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		data_allocator::destroy(finish.cur);
	}

	// __pop_front_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__pop_front_aux() {
		data_allocator::destroy(start.cur);
		data_allocator::deallocate(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

	// __reserve_elements_at_back 函数
	template <class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator
		deque<T, Alloc, BufSiz>::__reserve_elements_at_back(size_type n) {
		if (n > (size_type)(finish.last - finish.cur - 1)) {
			size_type new_node = (n + buffer_size() - 1) / buffer_size();	//申请缓冲区的个数
			__reserve_map_at_back(new_node);
			size_type i;
			try {
				for (i = 1; i < new_node; ++i)
					*(finish.node + i) = data_allocator::allocate(__deque_buf_size(sizeof(T)));
			}
			catch (...) {
				for (size_type j = 1; j < i; ++j)
					data_allocator::deallocate(finish.node + j);
			}
		}
		return finish + difference_type(n);
	}

	// __reserve_elements_at_front 函数
	template <class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator
		deque<T, Alloc, BufSiz>::__reserve_elements_at_front(size_type n) {
		if (n > (size_type)(start.cur - start.last)) {
			size_type new_node = (n + buffer_size() - 1) / buffer_size();	//申请缓冲区的个数
			__reserve_map_at_front(new_node);
			size_type i;
			try {
				for (i = 1; i < new_node; ++i)
					*(start.node - i) = data_allocator::allocate(__deque_buf_size(sizeof(T)));
			}
			catch (...) {
				for (size_type j = 1; j < i; ++j)
					data_allocator::deallocate(start.node - j);
			}
		}
	}

	// __reserve_map_at_back 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__reserve_map_at_back(size_type n = 1) {
		if (n + 1 > map_size - (finish.node - map))
			__reallocate_map(n, false);
	}

	// __reserve_map_at_front 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__reserve_map_at_front(size_type n = 1) {
		if (n > start.node - map)
			__reallocate_map(n, true);
	}

	// __reallocate_map 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::__reallocate_map(size_type node_to_add,
		bool add_at_front) {
		size_type old_nodes_num = finish.node - start.node + 1;
		size_type new_nodes_num = old_nodes_num + node_to_add;
		map_pointer new_start;
		if (map_size > 2 * new_nodes_num) {
			new_start = map + (map_size - new_nodes_num) / 2 
				+ (add_at_front ? node_to_add : 0);
			if (new_start < start.node)
				MyTinySTL::copy(start.node, finish.node + 1, new_start);
			else
				MyTinySTL::copy_backward(start.node, finish.node + 1, new_start + old_nodes_num);
		}
		else {
			size_type new_map_size = map_size + MyTinySTL::max(map_size, node_to_add) + 2;
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_start = new_map + (new_map_size - new_nodes_num) / 2
				+ (add_at_front ? node_to_add : 0);
			MyTinySTL::copy(start.node, finish.node + 1, new_start);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		// 调整新的 start 和 finish
		start.set_node(new_start);
		finish.set_node(new_start + old_nodes_num - 1);
	}
}
#endif // !DEQUE_H

