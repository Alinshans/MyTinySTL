#ifndef MYTINYSTL_STRING_TEST_H_
#define MYTINYSTL_STRING_TEST_H_

// string test : 测试 string 的接口和 insert 的性能

#include <string>

#include "../string.h"
#include "test.h"

namespace mystl {
namespace test {
namespace string_test {

void string_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------------- Run container test : string -----------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    const char* s = "abcdefg";
    mystl::string str;
    mystl::string str1(10);
    mystl::string str2(5, 'a');
    mystl::string str3(str2, 3);
    mystl::string str4(str2, 0, 5);
    mystl::string str5(s);
    mystl::string str6(s, 5);
    mystl::string str7(s, s + 7);
    mystl::string str8(str5);
    mystl::string str9(std::move(str8));
    mystl::string str10 = str6;
    mystl::string str11 = std::move(str10);
    str11.~basic_string();

    STR_COUT(str);
    STR_FUN_AFTER(str, str = 'x');
    STR_FUN_AFTER(str, str = "qwertyuiop");
    FUN_VALUE(*str.begin());
    FUN_VALUE(*str.rbegin());
    FUN_VALUE(*(str.end() - 1));
    FUN_VALUE(*(str.rend() - 1));
    FUN_VALUE(str.front());
    FUN_VALUE(str.back());
    FUN_VALUE(str[1]);
    FUN_VALUE(str.at(2));
    STR_COUT(str.data());
    STR_COUT(str.c_str());
    std::cout << std::boolalpha;
    FUN_VALUE(str.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(str.length());
    FUN_VALUE(str.size());
    FUN_VALUE(str.capacity());
    FUN_VALUE(str.max_size());

    STR_FUN_AFTER(str, str.insert(str.begin(), 'a'));
    STR_FUN_AFTER(str, str.insert(str.end(), 3, 'x'));
    STR_FUN_AFTER(str, str.insert(str.end(), "yz"));
    STR_FUN_AFTER(str, str.insert(str.end(), s, s + 3));
    STR_FUN_AFTER(str, str.erase(str.begin()));
    STR_FUN_AFTER(str, str.erase(str.begin(), 1));
    STR_FUN_AFTER(str, str.erase(str.begin(), str.begin() + 1));
    STR_FUN_AFTER(str, str.clear());
    STR_FUN_AFTER(str, str.add_back('x'));
    STR_FUN_AFTER(str, str.add_back('x', 2));
    STR_FUN_AFTER(str, str.add_back("yz"));
    STR_FUN_AFTER(str, str.add_back("add three", 3));
    STR_FUN_AFTER(str, str.add_back(s + 3, s + 6));
    STR_FUN_AFTER(str, str.add_front('c'));
    STR_FUN_AFTER(str, str.add_front('c',2));
    STR_FUN_AFTER(str, str.add_front("ab"));
    STR_FUN_AFTER(str, str.add_front("none", 0));
    STR_FUN_AFTER(str, str.add_front(s, s + 3));

    FUN_VALUE(str.compare("zzzzzzzzzzzzzzzz"));
    FUN_VALUE(str.compare("aaaaaaaaaaaaaaaaaaaaaa"));
    FUN_VALUE(str.compare("zzzzzzzzzzzzzzzzzzz"));
    FUN_VALUE(str.compare("aazzzzzzzzzzzzzzzzz"));
    FUN_VALUE(str.compare("abcabcccxxxyzadddef"));
    FUN_VALUE(str.substr(0));
    FUN_VALUE(str.substr(3));
    FUN_VALUE(str.substr(0,3));
    FUN_VALUE(str.substr(10));
    FUN_VALUE(str.substr(10,5));
    FUN_VALUE(str.substr(10,20));
    STR_FUN_AFTER(str, str.remove('a'));
    STR_FUN_AFTER(str, str.remove_if([](char c) {return c == 'b'; }));
    STR_FUN_AFTER(str, str.replace('c','x'));
    STR_FUN_AFTER(str, str.replace_if([](char c) {return c == 'x'; }, 'o'));
    FUN_VALUE(str.find('o'));
    FUN_VALUE(str.find('o', 5));
    FUN_VALUE(str.find('c'));
    FUN_VALUE(str.find("ooo"));
    FUN_VALUE(str.find("def",10));
    FUN_VALUE(str.find(str2));
    FUN_VALUE(str.rfind('o'));
    FUN_VALUE(str.rfind('o', 3));
    FUN_VALUE(str.rfind('c'));
    FUN_VALUE(str.rfind("ooo"));
    FUN_VALUE(str.rfind("def", 10));
    FUN_VALUE(str.rfind(str2));
    FUN_VALUE(str.count('c'));
    FUN_VALUE(str.count('o'));
    FUN_VALUE(str.count('o', 1));
    STR_FUN_AFTER(str, str.reverse());
    STR_FUN_AFTER(str, str.swap(str2));
    FUN_VALUE(str.size());
    FUN_VALUE(str.length());
    FUN_VALUE(str.capacity());
    STR_FUN_AFTER(str, str += str);
    STR_FUN_AFTER(str, str += 'b');
    STR_FUN_AFTER(str, str += "cdef");
    FUN_VALUE(str.size());
    FUN_VALUE(str.length());
    FUN_VALUE(str.capacity());
    mystl::string text = "test";
    mystl::string text2 = " ok!";
    std::cout << " text + '!' : " << text + '!' << std::endl;
    std::cout << " '#' + text : " << '#' + text << std::endl;
    std::cout << " text + \" success\" : " << text + " success" << std::endl;
    std::cout << " \"My \" + text : " << "My " + text << std::endl;
    std::cout << " text + text2 : " << text + text2 << std::endl;
    std::cout << " Please input a string to the text :\n ";
    std::cin >> text;
    STR_COUT(text);
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|        insert       |";
    CON_TEST_P2(string, insert, end, 's', LEN2, LEN3, LEN4);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[----------------- End container test : string -----------------]" << std::endl;
}

} // namespace string_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_STRING_TEST_H_

