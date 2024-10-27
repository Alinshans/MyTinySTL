#ifndef MYTINYSTL_STREAM_ITERATOR_H_
#define MYTINYSTL_STREAM_ITERATOR_H_

#include "basic_string.h"

namespace mystl
{

template<typename T, typename CharT = char,
        typename Traits = std::char_traits<CharT>, typename Dist = ptrdiff_t>
class istream_iterator
: public iterator<input_iterator_tag, T, Dist, const T*, const T&>
{
public:
    using char_type = CharT;
    using traits_type = Traits;
    using istream_type = std::basic_istream<CharT, Traits>;  

    istream_iterator() /* noexcept(std::is_nothrow_default_constructible<T>::value) */
        : m_stream{nullptr}, m_value{} {}

    istream_iterator(istream_type& is)
        : m_stream{std::addressof(is)}
    { read(); }

    istream_iterator(const istream_iterator& other) /* noexcept(std::is_nothrow_copy_constructible<T>::value) */
        = default;   // memberwise copy

    istream_iterator& operator=(const istream_iterator&) = default;   // memberwise copy-asgn
    
    ~istream_iterator() = default;

    const T& operator*() const noexcept {
        MYSTL_DEBUG(m_stream != nullptr);
        return m_value;
    }

    const T* operator->() const noexcept {
        return std::addressof(*this);
    }

    istream_iterator& operator++() {
        MYSTL_DEBUG(m_stream != nullptr);
        read();
        return *this;
    }

     istream_iterator operator++(int) {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

private:
    istream_type* m_stream;
    T m_value; 

    void read() {
        if (m_stream && !(*m_stream >> m_value)) {  // m_stream 有效且读到 EOS
            m_stream = nullptr;  
        }
    }  

    friend bool operator==(const istream_iterator& lhs, const istream_iterator& rhs) {
        return lhs.m_stream == rhs.m_stream;
    }  

    friend bool operator!=(const istream_iterator& lhs, const istream_iterator& rhs) {
        return lhs.m_stream != rhs.m_stream;
    }  
};


// TODO 
// template<typename T, typename CharT = char,
//          typename Traits = char_traits<CharT> >
// class ostream_iterator : public iterator<output_iterator_tag, void, void, void, void> {};
}


#endif