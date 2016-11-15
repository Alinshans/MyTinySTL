#ifndef MYTINYSTL_BASIC_STRING_H_
#define MYTINYSTL_BASIC_STRING_H_

#include <iostream>

#include "iterator.h"
#include "memory.h"
#include "type_traits.h"

namespace mystl {

#ifndef throw_range_error
#define throw_range_error	do { \
	std::cerr << " out_of_range." << std::endl; \
	exit(1); \
	} while(0)
#endif // !throw_range_error

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
		static const size_type	init_size = 8;
		static const value_type	init_char = 0x20;
		static const value_type eof = 0x24;
		static const size_type end_pos = -1;

	private:
		pointer buffer_;
		difference_type start_;
		difference_type finish_;
		size_type buffer_size_;

	public:
		// 构造、复制、移动、析构函数
		basic_string() { __initialize_string(init_size, init_char); }
		explicit basic_string(size_type n) { __initialize_string(n, init_char); }
		basic_string(size_type n, value_type ch) { __initialize_string(n, ch); }

		basic_string(const basic_string& other, size_type count);
		basic_string(const basic_string& other, difference_type pos, size_type count);

		basic_string(const_pointer str) { __copy_from(str, 0, __get_strlen(str)); }
		basic_string(const_pointer str, size_type count) { __copy_from(str, 0, count); }

		template<class InputIterator>
		basic_string(InputIterator first, InputIterator last) {
			__copy_from(first, 0, static_cast<size_type>(last - first));
		}

		basic_string(const basic_string& rhs) 
			:buffer_(nullptr), start_(0), finish_(0), buffer_size_(0) {
			*this = rhs;
		}
		basic_string(basic_string&& rhs)
			:buffer_(nullptr), start_(0), finish_(0), buffer_size_(0) {
			*this = std::move(rhs);
		}

		basic_string& operator=(const basic_string& rhs);
		basic_string& operator=(basic_string&& rhs);

		~basic_string() { __destroy_buffer(); }

	public:
		// 迭代器相关操作
		iterator begin() { return buffer_ + start_; }
		const_iterator begin() const { return buffer_ + start_; }
		const_iterator cbegin() const { return buffer_ + start_; }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
		const_reverse_iterator crbegin() const { return reverse_iterator(end()); }

		iterator end() { return buffer_ + finish_; }
		const_iterator end() const { return buffer_ + finish_; }
		const_iterator cend() const { return buffer_ + finish_; }
		
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return reverse_iterator(begin()); }
		const_reverse_iterator crend() const { return reverse_iterator(begin()); }

		// 容量相关操作
		bool empty() const { return start_ == finish_; }
		size_type size() const { return finish_ - start_; }
		size_type length() const { return finish_ - start_; }
		size_type capacity() const { return buffer_size_; }
		size_type max_size() const { return static_cast<size_type>(-1) / sizeof(value_type); }
		//void shrink_to_fit();

		// 访问元素相关操作
		reference operator[](difference_type n) { 
			if (n >= finish_ - start_)
				throw_range_error;
			return *(begin() + n); 
		}
		const_reference operator[](difference_type n) const {
			if (n >= finish_ - start_)
				throw_range_error;
			return *(begin() + n); 
		}
		reference at(difference_type n) { return *this[n]; }
		const_reference at(difference_type n) const { return *this[n]; }
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }
		pointer data() { return __get_str(); }
		const_pointer data() const { return __get_str(); }
		pointer c_str() { return __get_str(); }
		const_pointer c_str() const { return __get_str(); }

		// 添加删除相关操作	
		void insert(size_type index, value_type ch, size_type count) {
			insert(begin() + index, ch, count);
		}
		void insert(size_type index, value_type ch) { 
			insert(index, ch, 1); 
		}
		void insert(size_type index, const_pointer str) {
			insert(begin() + static_cast<difference_type>(index), str, str + __get_strlen(str));
		}

		iterator insert(iterator pos, value_type ch, size_type count);
		iterator insert(iterator pos, value_type ch) { return insert(pos, ch, 1); }

		template <class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);

		void erase(size_type index) { erase(index, 1); }
		void erase(size_type index, size_type count);
		iterator erase(iterator pos) { return erase(pos, 1); }
		iterator erase(iterator pos, size_type count);

		void clear() { mystl::destroy(buffer_ + start_, buffer_ + finish_); finish_ = start_; }

		basic_string& add_back(value_type ch) { return add_back(ch, 1); }
		basic_string& add_back(value_type ch, size_type count) {
			insert(end(), ch, count);
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
			insert(begin(), ch, count);
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

		basic_string substr(difference_type index) { return substr(index, finish_ - index); }
		basic_string substr(difference_type index, size_type count);

		void remove(value_type ch);
		template <class UnaryPredicate>
		void remove_if(UnaryPredicate up);

		void replace(value_type ch, value_type rch);
		template <class UnaryPredicate>
		void replace_if(UnaryPredicate up, value_type rch);

		size_type find(const_iterator first, const_iterator last, value_type ch);
		size_type find(const_iterator first, const_iterator last, const_pointer str);
		size_type rfind(const_iterator first, const_iterator last, value_type ch);
		size_type rfind(const_iterator first, const_iterator last, const_pointer str);
		size_type count(const_iterator first, const_iterator last, value_type ch) const;

		void swap(basic_string& rhs);

	public:
		basic_string& operator+=(const basic_string& str) {
			this->add_back(str.begin(), str.end());
			return *this;
		}
		basic_string& operator+=(value_type ch) {
			this->add_back(ch);
			return *this;
		}
		basic_string& operator+=(const_pointer str) {
			this->add_back(str, str + __get_strlen(str));
			return *this;
		}
	private:
		pointer __get_buffer(size_type n) { return data_allocator::allocate(n + 1); }
		pointer __get_buffer(size_type n) const { return data_allocator::allocate(n + 1); }
		void __put_buffer(pointer buf) { data_allocator::deallocate(buf); }
		void __destroy_buffer();
		pointer __get_str();
		const_pointer __get_str() const;
		size_type __get_strlen(const_pointer str);
		size_type __get_strlen(const_pointer str, char_type) { return strlen(str); }
		size_type __get_strlen(const_pointer str, wchar_type) { return wcslen(str); }
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
		if (count > other.length()) {
			throw_range_error;
		} 
		__copy_from(other.buffer_, 0, count);
	}

	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>::
		basic_string(const basic_string& other, difference_type pos, size_type count) {
		if (count > other.length() - pos) {
			throw_range_error;
		}
		__copy_from(other.buffer_, pos, count);
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
			start_ = rhs.start_;
			finish_ = rhs.finish_;
			buffer_size_ = rhs.buffer_size_;

			rhs.buffer_ = nullptr;
			rhs.start_ = 0;
			rhs.finish_ = 0;
			rhs.buffer_size_ = 0;
		}
		return *this;
	}

	//// 减少不用的空间
	//template<class CharType, class CharTraits, class Alloc>
	//void basic_string<CharType, CharTraits, Alloc>::shrink_to_fit() {
	//	if (static_cast<size_type>(finish_) < buffer_size_) {
	//		data_allocator::deallocate(buffer_ + finish_ + 1, buffer_size_ - finish_);
	//		buffer_size_ -= buffer_size_ - finish_;
	//	}
	//}

	// 在 pos 位置插入 n 个 ch
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::iterator 
		basic_string<CharType, CharTraits, Alloc>::insert(iterator pos,
			value_type ch, size_type count) {
		if (buffer_size_ - static_cast<size_type>(finish_) < count)
			return __reallocate_and_fill(pos, count, ch);
		const auto elems_after = finish_ - (pos - (buffer_ + start_));
		auto old_finish = finish_;
		if (static_cast<size_type>(elems_after) > count) {
			mystl::uninitialized_copy(buffer_ + finish_ - count, buffer_ + finish_, buffer_ + finish_);
			finish_ += count;
			mystl::copy_backward(pos, buffer_ + old_finish - count, buffer_ + old_finish);
			mystl::fill_n(pos, count, ch);
			buffer_[finish_] = eof;
		} 
		else {
			mystl::uninitialized_fill_n(buffer_ + finish_, count - elems_after, ch);
			finish_ += count - elems_after;
			mystl::uninitialized_copy(pos, buffer_ + old_finish, buffer_ + finish_);
			finish_ += elems_after;
			mystl::fill(pos, buffer_ + old_finish, ch);
			buffer_[finish_] = eof;
		}
		return pos + count;
	}

	// 在 pos 位置插入 [first, last) 内的元素
	template<class CharType, class CharTraits, class Alloc>
	template <class InputIterator>
	void basic_string<CharType, CharTraits, Alloc>::insert(iterator pos,
		InputIterator first, InputIterator last) {
		auto len = last - first;
		if (static_cast<difference_type>(buffer_size_) - finish_ < len) {
			__reallocate_and_copy(pos, first, last);
			return;
		}
		const auto elems_after = finish_ - (pos - (buffer_ + start_));
		auto old_finish = finish_;
		if (static_cast<difference_type>(elems_after) > len) {
			mystl::uninitialized_copy(buffer_ + finish_ - len, buffer_ + finish_, buffer_ + finish_);
			finish_ += len;
			mystl::copy_backward(pos, buffer_ + old_finish - len, buffer_ + old_finish);
			mystl::copy(first, last, pos);
			buffer_[finish_] = eof;
		}
		else {
			auto mid = first + elems_after;
			mystl::uninitialized_copy(mid, last, buffer_ + finish_);
			finish_ += len - elems_after;
			mystl::uninitialized_copy(pos, buffer_ + old_finish, buffer_ + finish_);
			finish_ += elems_after;
			mystl::copy(first, mid, pos);
			buffer_[finish_] = eof;
		}
	}

	// 删除从下标 index 开始的 n 个元素
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::erase(size_type index, size_type count) {
		if (index >= length())
			throw_range_error;
		erase(buffer_ + index, count);
	}

	// 删除从 pos 位置开始的 n 个元素
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::iterator 
		basic_string<CharType, CharTraits, Alloc>::erase(iterator pos, size_type count) {
		size_type pos_after = buffer_ + finish_ - pos;
		if (pos_after < count)
			throw_range_error;
		mystl::copy(pos + count, buffer_ + finish_, pos);
		finish_ -= count;
		buffer_[finish_] = eof;
		return pos;
	}

	// 在尾部添加字符串前 count 个字符
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>& 
		basic_string<CharType, CharTraits, Alloc>::add_back(const_pointer str, size_type count) {
		if (__get_strlen(str) < count)
			throw_range_error;
		insert(end(), str, str + count);
		return *this;
	}

	// 在头部添加字符串前 count 个字符
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>&
		basic_string<CharType, CharTraits, Alloc>::add_front(const_pointer str, size_type count) {
		if (__get_strlen(str) < count)
			throw_range_error;
		insert(begin(), str, str + count);
		return *this;
	}

	// 比较两个 basic_string，小于返回一个负数，大于返回一个正数，等于返回 0
	template<class CharType, class CharTraits, class Alloc>
	int32_t basic_string<CharType, CharTraits, Alloc>::compare(const basic_string& other) const {
		if (length() - other.length() != 0)	return length() - other.length();
		for (size_type i = 0; i < length(); ++i) {
			if (*this[i] < other[i])	
				return -1;
			else if (*this[i] > other[i])	
				return 1;
		}
		return 0;
	}

	// 返回下标从 index 开始长度为 count 的子串
	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>
		basic_string<CharType, CharTraits, Alloc>::substr(difference_type index, size_type count) {
		if (static_cast<size_type>(finish_ - index) < count)
			count = finish_ - index;
		/*auto tmp = __get_buffer(count);
		mystl::uninitialized_copy(begin() + index, begin() + index + count, tmp);
		tmp[count] = 0;
		return basic_string(tmp);*/
		return basic_string(begin() + index, begin() + index + count);
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

	// 在[first, last)区间查找值为 ch 的元素，若找到返回其下标，否则返回 end_pos
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type 
		basic_string<CharType, CharTraits, Alloc>::find(const_iterator first,
			const_iterator last, value_type ch) {
		for (auto it = first; it != last; ++it) {
			if (*it == ch)	return it - begin();
		}
		return end_pos;
	}

	// 在[first, last)区间查找字符串 str，若找到返回其开始的下标，否则返回 end_pos
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::find(const_iterator first,
			const_iterator last, const_pointer str) {
		difference_type len = __get_strlen(str);
		for (auto it = first; it != last; ++it) {
			if (*it == *str) {
				if (last - it < len)	return end_pos;
				else {
					auto i = 1;
					for (; i < len; ++i) {
						if (*(it + i) != *(str + i))	break;
					}
					if (i == len)	return it - begin();
				}
			}
		}
		return end_pos;
	}

	// 在[first, last)区间反向查找值为 ch 的元素，与 find 类似
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::rfind(const_iterator first,
			const_iterator last, value_type ch) {
		reverse_iterator rfirst(last);
		reverse_iterator rlast(first);
		for (auto it = rfirst; it != rlast; ++it) {
			if (*it == ch)	return it - begin();
		}
		return end_pos;
	}

	// 在[first, last)区间反向查找字符串，与 find 类似
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::rfind(const_iterator first,
			const_iterator last, const_pointer str) {
		const_reverse_iterator rfirst(last);
		const_reverse_iterator rlast(first);
		difference_type len = __get_strlen(str);
		for (const_reverse_iterator it = rfirst; it != rlast; ++it) {
			if (*it == *(str + len - 1)) {
				if (rlast - it < len)	return end_pos;
				else {
					auto i = 1;
					for (; i < len; ++i) {
						if (*(it + i) != *(str + len - i - 1))	break;
					}
					if (i == len)	return it.base() - begin() - len;
				}
			}
		}
		return end_pos;
	}

	// 返回[first, last)内值为 ch 的元素的出现次数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::count(const_iterator first,
			const_iterator last, value_type ch) const {
		size_type n = 0;
		for (auto it = first; it != last; ++it) {
			if (*it == ch)	++n;
		}
		return n;
	}

	// 交换两个 basic_string
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::swap(basic_string& rhs) {
		mystl::swap(buffer_, rhs.buffer_);
		mystl::swap(start_, rhs.start_);
		mystl::swap(finish_, rhs.finish_);
		mystl::swap(buffer_size_, rhs.buffer_size_);
	}

	// __destroy_buffer 函数
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::__destroy_buffer() {
		if (buffer_) {
			mystl::destroy(buffer_ + start_, buffer_ + finish_);
			__put_buffer(buffer_);
			buffer_ = nullptr;
			start_ = 0;
			finish_ = 0;
			buffer_size_ = 0;
		}
	}

	// __get_str 函数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::pointer 
		basic_string<CharType, CharTraits, Alloc>::__get_str() {
		pointer tmp = __get_buffer(length());
		mystl::uninitialized_copy(buffer_ + start_, buffer_ + finish_, tmp);
		tmp[length()] = 0;
		return tmp;
	}
	
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::const_pointer
		basic_string<CharType, CharTraits, Alloc>::__get_str() const {
		pointer tmp = __get_buffer(length());
		mystl::uninitialized_copy(buffer_ + start_, buffer_ + finish_, tmp);
		tmp[length()] = 0;
		return static_cast<const_pointer>(tmp);
	}

	// __get_strlen 函数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::size_type
		basic_string<CharType, CharTraits, Alloc>::__get_strlen(const_pointer str) {
		typedef typename CharTraits::value_type	T;
		return __get_strlen(str, T());
	}

	// __initialize_string 函数
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::__initialize_string(size_type n, value_type ch) {
		auto len = mystl::max(init_size, n);
		buffer_ = __get_buffer(len);
		buffer_size_ = __get_strlen(buffer_);
		mystl::uninitialized_fill_n(buffer_, n, ch);
		start_ = 0;
		finish_ = n;
		buffer_[finish_] = eof;
	}

	// __copy_from 函数
	template<class CharType, class CharTraits, class Alloc>
	void basic_string<CharType, CharTraits, Alloc>::__copy_from(const_pointer src,
		size_type pos, size_type n) {
		buffer_ = __get_buffer(n);
		buffer_size_ = __get_strlen(buffer_);
		if (__get_strlen(src) - pos < n)
			throw_range_error;
		mystl::copy_n(src + static_cast<difference_type>(pos), n, buffer_);
		start_ = 0;
		finish_ = n;
		buffer_[finish_] = eof;
	}

	// __reallocate_and_fill 函数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::iterator
		basic_string<CharType, CharTraits, Alloc>::__reallocate_and_fill(iterator pos,
			size_type n, value_type ch) {
		auto return_pos = pos - (buffer_ + start_) + n;
		auto len = length();
		auto buf_size = buffer_size_;
		auto old_buffer = buffer_;
		auto new_buffer = __get_buffer(buf_size + n);
		buffer_ = new_buffer;
		new_buffer = mystl::uninitialized_copy(old_buffer + start_, pos, new_buffer);
		new_buffer = mystl::uninitialized_fill_n(new_buffer, n, ch);
		mystl::uninitialized_copy(pos, old_buffer + finish_, new_buffer);
		__put_buffer(old_buffer);
		start_ = 0;
		finish_ = len + n;
		buffer_size_ = __get_strlen(buffer_);
		buffer_[finish_] = eof;
		return buffer_ + static_cast<difference_type>(return_pos);
	}

	// __reallocate_and_copy 函数
	template<class CharType, class CharTraits, class Alloc>
	typename basic_string<CharType, CharTraits, Alloc>::iterator
		basic_string<CharType, CharTraits, Alloc>::__reallocate_and_copy(iterator pos,
			const_iterator first, const_iterator last) {
		auto return_pos = pos - (buffer_ + start_) + (last - first);
		auto len = length();
		auto buf_size = buffer_size_;
		auto old_buffer = buffer_;
		auto new_buffer = __get_buffer(buf_size + static_cast<size_type>(last - first));
		buffer_ = new_buffer;
		new_buffer = mystl::uninitialized_copy(old_buffer + start_, pos, new_buffer);
		new_buffer = mystl::uninitialized_copy(first, last, new_buffer);
		mystl::uninitialized_copy(pos, old_buffer + finish_, new_buffer);
		__put_buffer(old_buffer);
		start_ = 0;
		finish_ = static_cast<difference_type>(len) + (last - first);
		buffer_size_ = __get_strlen(buffer_);
		buffer_[finish_] = eof;
		return buffer_ + static_cast<difference_type>(return_pos);
	}

	// 重载operator>>, operator<<
	template<class CharType, class CharTraits, class Alloc>
	std::istream& operator>>(std::istream& in,
		basic_string<CharType, CharTraits, Alloc>& str) {
		CharType* buf = new CharType[str.capacity() << 1];
		in >> buf;
		basic_string<CharType, CharTraits, Alloc> tmp(buf);
		str = std::move(tmp);
		return in;
	}

	template<class CharType, class CharTraits, class Alloc>
	std::ostream& operator<<(std::ostream& out,
		const basic_string<CharType, CharTraits, Alloc>& str) {
		for (auto& it : str) out << it;
		return out;
	}

	template<class CharType, class CharTraits, class Alloc>
	basic_string<CharType, CharTraits, Alloc>
		operator+(const basic_string<CharType, CharTraits, Alloc>& lhs,
			const basic_string<CharType, CharTraits, Alloc>& rhs) {
		basic_string<CharType, CharTraits, Alloc> tmp(lhs);
		tmp.add_back(rhs.begin(), rhs.end());
		return tmp;
	}

	
}

#endif // !MYTINYSTL_BASIC_STRING_H_

