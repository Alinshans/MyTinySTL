#ifndef MYTINYSTL_STRING_H_
#define MYTINYSTL_STRING_H_

// 定义了 string, wstring 类型

#include <cstring>
#include <string>

#include "basic_string.h"

namespace mystl {

    typedef basic_string<char>       string;
    typedef basic_string<wchar_t>    wstring;

}
#endif // !MYTINYSTL_STRING_H_

