#ifndef MYTINYSTL_BASIC_STRING_H_
#define MYTINYSTL_BASIC_STRING_H_

#include <iostream>

#include "iterator.h"
#include "memory.h"
#include "type_traits.h"

namespace mystl {

#ifndef MYTINYSTL_THROW_RANGE_ERROR
#define MYTINYSTL_THROW_RANGE_ERROR do { \
	std::cerr << " out_of_range." << std::endl; \
	exit(1); \
	} while(0)
#endif // !MYTINYSTL_THROW_RANGE_ERROR

#ifndef MYTINYSTL_CHECK_INDEX_RANGE
#define MYTINYSTL_CHECK_INDEX_RANGE(index) do { \
	if(index < 0 || index >= static_cast<difference_type>(length())) \
		MYTINYSTL_THROW_RANGE_ERROR; \
	} while(0)
#endif // !MYTINYSTL_CHECK_INDEX_RANGE

#ifndef MYTINYSTL_CHECK_RANGE_COMP
#define MYTINYSTL_CHECK_RANGE_COMP(first, second) do { \
	if(first < second) \
		MYTINYSTL_THROW_RANGE_ERROR; \
	} while(0)
#endif // !MYTINYSTL_CHECK_RANGE_COMP

#ifndef MYTINYSTL_CHECK_END_ERROR
#define MYTINYSTL_CHECK_END_ERROR(first, last) do { \
	/Todo  \
} while(0)
#endif // !MYTINYSTL_CHECK_END_ERROR


	template<class CharType, class CharTraits = __char_type<CharType>, class Alloc = alloc>
	class basic_string {
	public:
		typedef CharType	value_type;
		typedef Alloc	allocator_type;
		typedef value_type*	pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&	reference;
		typedef const value_type&	const_reference;
		typedef size_t	size_type;
		typedef ptrdiff_t	difference_type;

		typedef value_type*	iterator;
		typedef const value_type*	const_iterator;
		typedef reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef reverse_iterator<iterator>	reverse_iterator;
		
	public:
		typedef allocator<value_type, Alloc>	data_allocator;
		allocator_type	get_allocator() { return allocator_type(); }

	public:
		static const size_type end_pos = -1;

	private:
		static const size_type	init_size = 8;
		static const value_type	init_char = 0x20;
		static const value_type eof = 0x24;

		iterator buffer_;
		iterator finish_;
		iterator end_;

	public:
		// 构造、复制、移动、析构函数
		basic_string() { __initialize_string(0, init_char); }
		explicit basic_string(size_type n) { __initialize_string(n, init_char); }
		basic_string(size_type n, value_type ch) { __initialize_string(n, ch); }

		basic_string(const basic_string& other, size_type count);
		basic_string(const basic_string& other, difference_type index, size_type count);

		basic_string(const_pointer str) { __copy_from(str, 0, __get_strlen(str)); }
		basic_string(const_pointer str, size_type count) { __copy_from(str, 0, count); }

		// *** 没有处理 basic_string 的 end
		template<class InputIterator>
		basic_string(InputIterator first, InputIterator last) {
			MYTINYSTL_CHECK_RANGE_COMP(last, first);
			__copy_from(first, 0, last - first);
		}

		basic_string(const basic_string& rhs) 
			:buffer_(nullptr), finish_(nullptr), end_(nullptr) {
			*this = rhs;
		}
		basic_string(basic_string&& rhs)
			:buffer_(nullptr), finish_(nullptr), end_(nullptr) {
			*this = std::move(rhs);
		}

		basic_string& operator=(const basic_string& rhs);
		basic_string& operator=(basic_string&& rhs);
		basic_string& operator=(const_pointer str) {
			basic_string tmp(str);
			*this = std::move(tmp);
			return *this;
		}
		basic_string& operator=(value_type ch) {
			basic_string tmp(1, ch);
			*this = std::move(tmp);
			return *this;
		}

		~basic_string() { __destroy_buffer(); }

	public:
		// 迭代器相关操作
		iterator begin() { return buffer_; }
		const_iterator begin() const { return buffer_; }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(end()); }

		iterator end() { return finish_; }
		const_iterator end() const { return finish_; }
		
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return reverse_iterator(begin()); }

		// 容量相关操作
		bool empty() const { return buffer_ == finish_; }
		size_type size() const { return finish_ - buffer_; }
		size_type length() const { return finish_ - buffer_; }
		size_type capacity() const { return end_ - buffer_; }
		size_type max_size() const { return static_cast<size_type>(-1) / sizeof(value_type); }
		void shrink_to_fit();

		// 访问元素相关操作
		reference operator[](difference_type n) { 
			MYTINYSTL_CHECK_INDEX_RANGE(n);
			return *(buffer_ + n);
		}
		const_reference operator[](difference_type n) const {
			MYTINYSTL_CHECK_INDEX_RANGE(n);
			return *(buffer_ + n); 
		}
		reference at(difference_type n) { return (*this)[n]; }
		const_reference at(difference_type n) const { return (*this)[n]; }
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }
		pointer data() { return __get_str(); }
		const_pointer data() const { return __get_str(); }
		pointer c_str() { return __get_str(); }
		const_pointer c_str() const { return __get_str(); }

		// 添加删除相关操作	
		void insert(difference_type index, const basic_string& str) {
			MYTINYSTL_CHECK_INDEX_RANGE(index);
			insert(begin() + index, str.begin(), str.end());
		}
		void insert(difference_type index, size_type count, value_type ch) {
			MYTINYSTL_CHECK_INDEX_RANGE(index);
			insert(begin() + index, count, ch);
		}
		void insert(difference_type index, const_pointer str) {
			MYTINYSTL_CHECK_INDEX_RANGE(index);
			insert(begin() + index, str, str + __get_strlen(str));
		}

		iterator insert(iterator pos, size_type count, value_type ch);
		iterator insert(iterator pos, value_type ch);
		iterator insert(iterator pos, const_pointer str) {
			insert(pos, str, str + __get_strlen(str));
			return pos + __get_strlen(str);
		}

		template <class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);

		void erase(difference_type index) { 
			MYTINYSTL_CHECK_INDEX_RANGE(index);
			erase(index, 1); 
		}
		void erase(difference_type index, size_type count) {
			MYTINYSTL_CHECK_INDEX_RANGE(index);
			erase(buffer_ + index, count);
		}

		iterator erase(iterator pos) { return erase(pos, 1); }
		iterator erase(iterator pos, size_type count);

		void erase(iterator first, iterator last) { erase(first, last - first); }

		void clear();

		basic_string& add_back(value_type ch) {
			insert(end(), ch);
			return *this;
		}
		basic_string& add_back(value_type ch, size_type count) {
			insert(end(), count, ch);
			return *this;
		}
		basic_string& add_back(const_pointer str) { return add_back(str, __get_strlen(str)); }
		basic_string& add_back(const_pointer str, size_type count);

		template <class InputIterator>
		basic_string& add_back(InputIterator first, InputIterator last) {
			insert(end(), first, last);
			return *this;
		}

		basic_string& add_front(value_type ch) { return add_front(ch, 1); }
		basic_string& add_front(value_type ch, size_type count) {
			insert(begin(), count, ch);
			return *this;
		}
		basic_string& add_front(const_pointer str) { return add_front(str, __get_strlen(str)); }
		basic_string& add_front(const_pointer str, size_type count);

		template <class InputIterator>
		basic_string& add_front(InputIterator first, InputIterator last) {
			insert(begin(), first, last);
			return *this;
		}

		// basic_string 相关操作
		int32_t compare(const basic_string& other) const;

		basic_string substr(size_type index) { return substr(index, length() - index); }
		basic_string substr(size_type index, size_type count);

		void remove(value_type ch);
		template <class UnaryPredicate>
		void remove_if(UnaryPredicate up);

		void replace(value_type ch, value_type rch);
		template <class UnaryPredicate>
		void replace_if(UnaryPredicate up, value_type rch);

		size_type find(value_type ch, size_type index) const;
		size_type find(value_type ch) const { return find(ch, 0); }
		size_type find(const_pointer str, size_type index) const;
		size_type find(const_pointer str) const { return find(str, 0); }
		size_type find(const basic_string& str) const { return find(str.data(), 0); }

		size_type rfind(value_type ch, size_type index) const;
		size_type rfind(value_type ch) const { return rfind(ch, length() - 1); }
		size_type rfind(const_pointer str, size_type index) const;
		size_type rfind(const_pointer str) const { return rfind(str, length() - 1); }
		size_type rfind(const basic_string& str) const { return rfind(str.data(), length() - 1); }

		size_type count(value_type ch) const { return count(ch, 0); }
		size_type count(value_type ch, size_type index) const;

		void swap(basic_string& rhs);

	public:
		//
		basic_string& operator+=(const basic_string& str) {
			if(str.length())
				add_back(str.begin(), str.end());
			return *this;
		}
		basic_string& operator+=(value_type ch) {
			add_back(ch);
			return *this;
		}
		basic_string& operator+=(const_pointer str) {
			if(__get_strlen(str))
				add_back(str, str + __get_strlen(str));
			return *this;
		}
	private:
		pointer __get_buffer(size_type n) { return data_allocator::allocate(n); }
		pointer __get_buffer(size_type n) const { return data_allocator::allocate(n); }
		void __put_buffer(pointer buf) { data_allocator::deallocate(buf); }
		void __destroy_buffer();
		pointer __get_str();
		const_pointer __get_str() const;
		size_type __get_strlen(const_pointer str) const;
		size_type __get_strlen(const_pointer str, char_type) const { return strlen(str); }
		size_type __get_strlen(const_pointer str, wchar_type) const { return wcslen(str); }
		void __initialize_string(size_type n, value_type ch);
		void __copy_from(const_pointer src, size_type pos, size_type n);
		iterator __reallocate_and_fill(iterator pos, size_type n, value_type ch);
		iterator __reallocate_and_copy(iterator pos, const_iterator first, const_iterator last);
	};

	/**********************************************************************************/

	// 构造函数
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>::
		basic_string(const basic_string& other, size_type count) {
		MYTINYSTL_CHECK_RANGE_COMP(other.length(), count);
		__copy_from(other.data(), 0, count);
	}

	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>::
		basic_string(const basic_string& other, difference_type index, size_type count) {
		if (index < 0 || static_cast<size_type>(index) > other.length())
			MYTINYSTL_THROW_RANGE_ERROR;
		MYTINYSTL_CHECK_RANGE_COMP(other.length() - index, count);
		__copy_from(other.data(), index, count);
	}

	// 复制赋值操作符
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>&
		basic_string<CharType, CharTraits, Alloc>::operator=(const basic_string& rhs) {
		if (this != &rhs) {
			__copy_from(rhs.data(), 0, rhs.length());
		}
		return *this;
	}

	// 移动赋值操作符
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>&
		basic_string<CharType, CharTraits, Alloc>::operator=(basic_string&& rhs) {
		if (this != &rhs) {
			__destroy_buffer();

			buffer_ = rhs.buffer_;
			finish_ = rhs.finish_;
			end_ = rhs.end_;

			rhs.buffer_ = nullptr;
			rhs.finish_ = nullptr;
			rhs.end_ = nullptr;
		}
		return *this;
	}

	// 减少不用的空间
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::shrink_to_fit() {
		if (finish_ < end_) {
			data_allocator::deallocate(finish_ + 1, end_ - finish_ - 1);
			end_ = finish_;
		}
	}

	// 在 pos 位置插入 n 个 ch
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::iterator 
		basic_string<CharType, CharTraits, Alloc>::insert(iterator pos,
			size_type count, value_type ch) {
		if (count < 1)	return pos;
		if (static_cast<size_type>(end_ - finish_) < count)	// 备用空间不够
			return __reallocate_and_fill(pos, count, ch);
		if (pos == end()) {	// 插入位置在尾部
			finish_ = mystl::fill_n(end(), count, ch);
			*finish_ = eof;
			return pos + count;
		}
		const auto elems_after = finish_ - pos;	// pos 后的元素数目
		auto old_finish = finish_;
		if (static_cast<size_type>(elems_after) > count) {	// pos 后的元素较多
			finish_ = mystl::uninitialized_copy(finish_ - count, finish_, finish_);
			mystl::copy_backward(pos, old_finish - count, old_finish);
			mystl::fill_n(pos, count, ch);
			*finish_ = eof;
		}
		else {	// 插入的元素较多
			finish_ = mystl::uninitialized_fill_n(finish_, count - elems_after, ch);
			finish_ = mystl::uninitialized_copy(pos, old_finish, finish_);
			mystl::fill(pos, old_finish, ch);
			*finish_ = eof;
		}
		return pos + count;
	}

	// 在 pos 位置插入一个元素
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::iterator 
		basic_string<CharType, CharTraits, Alloc>::insert(iterator pos, value_type ch) {
		if (end_ == finish_)
			return __reallocate_and_fill(pos, 1, ch);
		mystl::copy_backward(pos, finish_, finish_ + 1);
		*++finish_ = eof;
		*pos = ch;
		return pos + 1;
	}

	// 在 pos 位置插入 [first, last) 内的元素
	template<class CharType, class CharTraits, class Alloc>
	template <class InputIterator>
	void basic_string<CharType, CharTraits, Alloc>::insert(iterator pos,
		InputIterator first, InputIterator last) {
		MYTINYSTL_CHECK_RANGE_COMP(last, first);
		const auto len = last - first;
		if (static_cast<difference_type>(__get_strlen(first)) < len)
			MYTINYSTL_THROW_RANGE_ERROR;
		// Todo: check basic_string end
		if (len < 1)	return;
		if ((end_ - finish_) < len) {	// 备用空间不够
			__reallocate_and_copy(pos, first, last);
			return;
		}
		if (pos == end()) {	// 插入位置在尾部
			finish_ = mystl::copy(first, last, end());
			*finish_ = eof;
			return;
		}
		const auto elems_after = finish_ - pos;	// pos 后的元素数目
		auto old_finish = finish_;
		if (elems_after > len) {	// pos 后元素较多
			finish_ = mystl::uninitialized_copy(finish_ - len, finish_, finish_);
			mystl::copy_backward(pos, old_finish - len, old_finish);
			mystl::copy(first, last, pos);
			*finish_ = eof;
		}
		else {	// 插入元素较多
			auto mid = first + elems_after;
			finish_ = mystl::uninitialized_copy(mid, last, finish_);
			finish_ = mystl::uninitialized_copy(pos, old_finish, finish_);
			mystl::copy(first, mid, pos);
			*finish_ = eof;
		}
	}

	// 删除从 pos 位置开始的 n 个元素
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::iterator 
		basic_string<CharType, CharTraits, Alloc>::erase(iterator pos, size_type count) {
		MYTINYSTL_CHECK_RANGE_COMP(static_cast<size_type>(finish_ - pos), count);
		if (pos == begin() && count == length()) {	// 删除全部元素
			clear();
			return finish_;
		}
		auto new_finish = mystl::copy(pos + count, finish_, pos);
		mystl::destroy(new_finish, finish_);
		finish_ = new_finish;
		*finish_ = eof;
		return pos;
	}

	// 清空全部元素
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::clear() {
		mystl::destroy(buffer_, finish_);
		finish_ = buffer_;
		*finish_ = eof;
	}
	
	// 在尾部添加字符串前 count 个字符
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>& 
		basic_string<CharType, CharTraits, Alloc>::add_back(const_pointer str, size_type count) {
		MYTINYSTL_CHECK_RANGE_COMP(__get_strlen(str), count);
		insert(end(), str, str + count);
		return *this;
	}

	// 在头部添加字符串前 count 个字符
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>&
		basic_string<CharType, CharTraits, Alloc>::add_front(const_pointer str, size_type count) {
		MYTINYSTL_CHECK_RANGE_COMP(__get_strlen(str), count);
		insert(begin(), str, str + count);
		return *this;
	}

	// 比较两个 basic_string，小于返回一个负数，大于返回一个正数，等于返回 0
	template<class CharType, class CharTraits, class Alloc>
	int32_t basic_string<CharType, CharTraits, Alloc>::compare(const basic_string& other) const {
		if (length() - other.length())	return length() - other.length();
		for (size_type i = 0; i < length(); ++i) {
			if ((*this)[i] < other[i])	
				return -1;
			else if ((*this)[i] > other[i])	
				return 1;
		}
		return 0;
	}

	// 返回下标从 index 开始长度为 count 的子串
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>
		basic_string<CharType, CharTraits, Alloc>::substr(size_type index, size_type count) {
		MYTINYSTL_CHECK_INDEX_RANGE(index);
		count = mystl::min(count, length() - index);
		return basic_string(begin() + index, begin() + index + count - 1);
	}

	// 移除值为 ch 的元素
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::remove(value_type ch) {
		for (auto it = begin(); it != end();) {
			if (*it == ch)	
				it = erase(it);
			else
				++it;
		}
	}

	// 移除令一元操作为 true 的元素
	template<class CharType, class CharTraits, class Alloc>
	template <class UnaryPredicate>
	void basic_string<CharType, CharTraits, Alloc>::remove_if(UnaryPredicate up) {
		for (auto it = begin(); it != end();) {
			if (up(*it))
				it = erase(it);
			else
				++it;
		}
	}

	// 将值为 ch 的元素替换为 rch
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::replace(value_type ch, value_type rch) {
		for (auto it = begin(); it != end(); ++it) {
			if (*it == ch)	*it = rch;
		}
	}

	// 将令一元操作为 true 的元素替换为 rch
	template<class CharType, class CharTraits, class Alloc>
	template <class UnaryPredicate>
	void basic_string<CharType, CharTraits, Alloc>::replace_if(UnaryPredicate up, value_type rch) {
		for (auto it = begin(); it != end(); ++it) {
			if (up(*it))	*it = rch;
		}
	}

	// 从下标 index 开始查找值为 ch 的元素，若找到返回其下标，否则返回 end_pos
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type 
		basic_string<CharType, CharTraits, Alloc>::find(value_type ch, size_type index) const {
		MYTINYSTL_CHECK_INDEX_RANGE(index);
		for (auto i = index; i < length(); ++i) {
			if (*(buffer_ + i) == ch)	return i;
		}
		return end_pos;
	}

	// 从下标 index 开始查找字符串 str，若找到返回其开始的下标，否则返回 end_pos
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::find(const_pointer str, size_type index) const {
		MYTINYSTL_CHECK_INDEX_RANGE(index);
		auto len = __get_strlen(str);
		for (auto i = index; i <= length() - len; i++) {
			if (*(buffer_ + i) == *str) {
				size_type j = 1;
				for (; j < len; ++j) {
					if (*(buffer_ + i + j) != *(str + j))	break;
				}
				if (j == len)	return i;
			}
		}
		return end_pos;
	}

	// 从下标 index 开始反向查找值为 ch 的元素，与 find 类似
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::rfind(value_type ch, size_type index) const {
		MYTINYSTL_CHECK_INDEX_RANGE(index);
		for (auto i = index; i >= 0; --i) {
			if (*(buffer_ + i) == ch)	return i;
		}
		return end_pos;
	}

	// 从下标 index 开始反向查找字符串，与 find 类似
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::rfind(const_pointer str, size_type index) const {
		MYTINYSTL_CHECK_INDEX_RANGE(index);
		auto len = __get_strlen(str);
		for (auto i = index; i >= len; --i) {
			if (*(buffer_ + i) == *(str + len - 1)) {
				size_type j = 1;
				for (; j < len; ++j) {
					if (*(buffer_ + i - j) != *(str + len - j - 1))	break;
				}
				if (j == len)	return i - len + 1;
			}
		}
		return end_pos;
	}

	// 返回从下标 index 开始值为 ch 的元素出现的次数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::count(value_type ch, size_type index) const {
		MYTINYSTL_CHECK_INDEX_RANGE(index);
		size_type n = 0;
		for (auto i = index; i < length(); ++i) {
			if (*(buffer_ + i) == ch)	++n;
		}
		return n;
	}

	// 交换两个 basic_string
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::swap(basic_string& rhs) {
		mystl::swap(buffer_, rhs.buffer_);
		mystl::swap(finish_, rhs.finish_);
		mystl::swap(end_, rhs.end_);
	}

	// __destroy_buffer 函数
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::__destroy_buffer() {
		if (buffer_) {
			mystl::destroy(buffer_, finish_);
			__put_buffer(buffer_);
			buffer_ = nullptr;
			finish_ = nullptr;
			end_ = nullptr;
		}
	}

	// __get_str 函数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::pointer 
		basic_string<CharType, CharTraits, Alloc>::__get_str() {
		pointer tmp = __get_buffer(length());
		mystl::uninitialized_copy(buffer_, finish_, tmp);
		tmp[length()] = 0;
		return tmp;
	}
	
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::const_pointer
		basic_string<CharType, CharTraits, Alloc>::__get_str() const {
		pointer tmp = __get_buffer(length());
		mystl::uninitialized_copy(buffer_, finish_, tmp);
		tmp[length()] = 0;
		return static_cast<const_pointer>(tmp);
	}

	// __get_strlen 函数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::__get_strlen(const_pointer str) const {
		typedef typename CharTraits::value_type	T;
		return __get_strlen(str, T());
	}

	// __initialize_string 函数
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::__initialize_string(size_type n, value_type ch) {
		if (n == 0) {
			buffer_ = __get_buffer(init_size);
			finish_ = buffer_;
			end_ = buffer_ + init_size;
			*finish_ = eof;
			return;
		}
		auto len = mystl::max(init_size, n);
		buffer_ = __get_buffer(len);
		finish_ = mystl::uninitialized_fill_n(buffer_, n, ch);
		end_ = buffer_ + len;
		*finish_ = eof;
	}

	// __copy_from 函数
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::__copy_from(const_pointer src,
		size_type pos, size_type count) {
		MYTINYSTL_CHECK_RANGE_COMP(__get_strlen(src) - pos, count);
		auto len = mystl::max(init_size, count);
		buffer_ = __get_buffer(len);
		finish_ = mystl::copy_n(src + pos, count, buffer_).second;
		end_ = buffer_ + len;
		*finish_ = eof;
	}

	// __reallocate_and_fill 函数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::iterator
		basic_string<CharType, CharTraits, Alloc>::__reallocate_and_fill(iterator pos,
			size_type n, value_type ch) {
		const auto return_pos = pos - buffer_ + n;
		const auto old_len = length();
		const auto new_len = old_len + n + ((old_len + n) >> 1);
		auto new_buffer = __get_buffer(new_len);
		mystl::uninitialized_copy(pos, finish_, 
			mystl::uninitialized_fill_n(
				mystl::uninitialized_copy(buffer_, pos, new_buffer), n, ch));
		__put_buffer(buffer_);
		buffer_ = new_buffer;
		finish_ = buffer_ + (old_len + n);
		end_ = buffer_ + new_len;
		*finish_ = eof;
		return buffer_ + return_pos;
	}

	// __reallocate_and_copy 函数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::iterator
		basic_string<CharType, CharTraits, Alloc>::__reallocate_and_copy(iterator pos,
			const_iterator first, const_iterator last) {
		const auto return_pos = pos - buffer_ + (last - first);
		const auto old_len = length();
		const auto new_len = old_len + (last - first) + ((old_len + (last - first)) >> 1);
		auto new_buffer = __get_buffer(new_len);
		mystl::uninitialized_copy(pos, finish_,
			mystl::uninitialized_copy(first, last,
				mystl::uninitialized_copy(buffer_, pos, new_buffer)));
		__put_buffer(buffer_);
		buffer_ = new_buffer;
		finish_ = buffer_ + (old_len + (last - first));
		end_ = buffer_ + new_len;
		*finish_ = eof;
		return buffer_ + return_pos;
	}

	// 重载 operator>>, operator<<
	template<class CharType, class CharTraits, class Alloc>
	std::istream& operator>>(std::istream& in,
		basic_string<CharType, CharTraits, Alloc>& str) {
		// Todo 
		/*CharType* buf = new CharType[str.capacity() << 1];
		in >> buf;
		basic_string<CharType, CharTraits, Alloc> tmp(buf);
		str = std::move(tmp);*/
		return in;
	}

	template<class CharType, class CharTraits, class Alloc>
	std::ostream& operator<<(std::ostream& out, const basic_string<CharType, CharTraits, Alloc>& str) {
		for (auto it : str) out << it;
		return out;
	}

	// 重载 operator+
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>
		operator+(const basic_string<CharType, CharTraits, Alloc>& lhs,
			const basic_string<CharType, CharTraits, Alloc>& rhs) {
		basic_string<CharType, CharTraits, Alloc> tmp(lhs);
		tmp.add_back(rhs.begin(), rhs.end());
		return tmp;
	}

	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>
		operator+(const CharType* lhs, const basic_string<CharType, CharTraits, Alloc>& rhs) {
		basic_string<CharType, CharTraits, Alloc> tmp(lhs);
		tmp.add_back(rhs.begin(), rhs.end());
		return tmp;
	}

	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>
		operator+(const basic_string<CharType, CharTraits, Alloc>& lhs, const CharType* rhs) {
		basic_string<CharType, CharTraits, Alloc> tmp(lhs);
		tmp.add_back(rhs);
		return tmp;
	}

	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>
		operator+(CharType ch, const basic_string<CharType, CharTraits, Alloc>& rhs) {
		basic_string<CharType, CharTraits, Alloc> tmp(ch);
		tmp.add_back(rhs.begin(), rhs.end());
		return tmp;
	}

	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>
		operator+(const basic_string<CharType, CharTraits, Alloc>& lhs, CharType ch) {
		basic_string<CharType, CharTraits, Alloc> tmp(lhs);
		tmp.add_back(ch);
		return tmp;
	}

	// 重载比较操作符
	template<class CharType, class CharTraits, class Alloc>
	bool operator==(const basic_string<CharType, CharTraits, Alloc>& lhs,
		const basic_string<CharType, CharTraits, Alloc>& rhs) {
		return lhs.compare(rhs) == 0;
	}

	template<class CharType, class CharTraits, class Alloc>
	bool operator!=(const basic_string<CharType, CharTraits, Alloc>& lhs,
		const basic_string<CharType, CharTraits, Alloc>& rhs) {
		return lhs.compare(rhs) != 0;
	}

	template<class CharType, class CharTraits, class Alloc>
	bool operator<(const basic_string<CharType, CharTraits, Alloc>& lhs,
		const basic_string<CharType, CharTraits, Alloc>& rhs) {
		return lhs.compare(rhs) < 0;
	}

	template<class CharType, class CharTraits, class Alloc>
	bool operator<=(const basic_string<CharType, CharTraits, Alloc>& lhs,
		const basic_string<CharType, CharTraits, Alloc>& rhs) {
		return lhs.compare(rhs) <= 0;
	}

	template<class CharType, class CharTraits, class Alloc>
	bool operator>(const basic_string<CharType, CharTraits, Alloc>& lhs,
		const basic_string<CharType, CharTraits, Alloc>& rhs) {
		return lhs.compare(rhs) > 0;
	}

	template<class CharType, class CharTraits, class Alloc>
	bool operator>=(const basic_string<CharType, CharTraits, Alloc>& lhs,
		const basic_string<CharType, CharTraits, Alloc>& rhs) {
		return lhs.compare(rhs) >= 0;
	}

	// 重载 mystl 的 swap
	template<class CharType, class CharTraits, class Alloc>
	void swap(basic_string<CharType, CharTraits, Alloc>& lhs,
		basic_string<CharType, CharTraits, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}

#endif // !MYTINYSTL_BASIC_STRING_H_

