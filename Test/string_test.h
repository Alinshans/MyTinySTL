#ifndef MYTINYSTL_STRING_TEST_H_
#define MYTINYSTL_STRING_TEST_H_

// string test : 测试 string 的接口和 insert 的性能

#include <string>

#include "../MyTinySTL/astring.h"
#include "test.h"

namespace mystl
{
namespace test
{
namespace string_test
{

void string_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[----------------- Run container test : string -----------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  const char* s = "abcdefg";
  mystl::string str;
  mystl::string str1(5, 'a');
  mystl::string str2(str1, 3);
  mystl::string str3(str1, 0, 3);
  mystl::string str4("abc");
  mystl::string str5("abcde",3);
  mystl::string str6(s, s + 5);
  mystl::string str7(str1);
  mystl::string str8(std::move(str1));
  mystl::string str9;
  str9 = str2;
  mystl::string str10;
  str10 = std::move(str2);
  mystl::string str11;
  str11 = "123";
  mystl::string str12;
  str12 = 'A';

  STR_FUN_AFTER(str, str = 'a');
  STR_FUN_AFTER(str, str = "string");
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
  FUN_VALUE(str.size());
  FUN_VALUE(str.length());
  FUN_VALUE(str.capacity());
  FUN_VALUE(str.max_size());
  STR_FUN_AFTER(str, str.shrink_to_fit());
  FUN_VALUE(str.capacity());

  STR_FUN_AFTER(str, str.insert(str.begin(), 'a'));
  STR_FUN_AFTER(str, str.insert(str.end(), 3, 'x'));
  STR_FUN_AFTER(str, str.insert(str.end(), s, s + 3));
  STR_FUN_AFTER(str, str.erase(str.begin()));
  STR_FUN_AFTER(str, str.erase(str.begin(), str.begin() + 3));
  STR_FUN_AFTER(str, str.clear());
  STR_FUN_AFTER(str, str.push_back('s'));
  STR_FUN_AFTER(str, str.push_back('t'));
  STR_FUN_AFTER(str, str.pop_back());
  STR_FUN_AFTER(str, str.append(1, 't'));
  STR_FUN_AFTER(str, str.append(str4));
  STR_FUN_AFTER(str, str.append(str4, 1));
  STR_FUN_AFTER(str, str.append(str4, 2, 1));
  STR_FUN_AFTER(str, str.append("str"));
  STR_FUN_AFTER(str, str.append("inging", 3));
  STR_FUN_AFTER(str, str.append(s, s + 3));
  STR_FUN_AFTER(str, str.resize(10));
  FUN_VALUE(str.size());
  STR_FUN_AFTER(str, str.resize(20, 'x'));
  FUN_VALUE(str.size());
  STR_FUN_AFTER(str, str.clear());

  STR_FUN_AFTER(str, str = "string");
  STR_FUN_AFTER(str3, str3 = "astrings");
  FUN_VALUE(str.compare(str3));
  FUN_VALUE(str.compare(0, 6, str3));
  FUN_VALUE(str.compare(0, 6, str3, 1, 6));
  FUN_VALUE(str.compare("atringgg"));
  FUN_VALUE(str.compare("zzz"));
  FUN_VALUE(str.compare(0, 3, "str"));
  FUN_VALUE(str.compare(0, 3, "stri", 4));
  FUN_VALUE(str.compare(0, 3, "s", 3));
  FUN_VALUE(str.compare(0, 9, "stringabc", 9));
  FUN_VALUE(str.substr(0));
  FUN_VALUE(str.substr(3));
  FUN_VALUE(str.substr(0, 3));
  FUN_VALUE(str.substr(0, 10));
  STR_FUN_AFTER(str, str.replace(0, 6, str3));
  STR_FUN_AFTER(str, str.replace(str.end() - 1, str.end(), str3));
  STR_FUN_AFTER(str, str.replace(0, 1, "my "));
  STR_FUN_AFTER(str, str.replace(str.end() - 8, str.end(), " test"));
  STR_FUN_AFTER(str, str.replace(10, 4, "replace"));
  STR_FUN_AFTER(str, str.replace(str.end(), str.end(), " test"));
  STR_FUN_AFTER(str, str.replace(0, 2, 3, '6'));
  STR_FUN_AFTER(str, str.replace(str.begin(), str.begin() + 3, 6, '6'));
  STR_FUN_AFTER(str, str.replace(0, 3, str3, 1, 3));
  STR_FUN_AFTER(str, str.replace(str.begin(), str.begin() + 6, s, s + 3));
  STR_FUN_AFTER(str, str.reverse());
  STR_FUN_AFTER(str, str.reverse());

  STR_FUN_AFTER(str, str = "abcabc stringgg");
  STR_FUN_AFTER(str3, str3 = "abc");
  FUN_VALUE(str.find('a'));
  FUN_VALUE(str.find('a', 3));
  FUN_VALUE(str.find('a', 4));
  FUN_VALUE(str.find("abc"));
  FUN_VALUE(str.find("abc", 1));
  FUN_VALUE(str.find("abc", 1, 1));
  FUN_VALUE(str.find(str3));
  FUN_VALUE(str.find(str3, 1));
  FUN_VALUE(str.rfind('g'));
  FUN_VALUE(str.rfind('g', 3));
  FUN_VALUE(str.rfind("gg"));
  FUN_VALUE(str.rfind("bc", 10));
  FUN_VALUE(str.rfind(str3));
  FUN_VALUE(str.rfind(str3, 3));
  FUN_VALUE(str.find_first_of('g'));
  FUN_VALUE(str.find_first_of('k'));
  FUN_VALUE(str.find_first_of("bca"));
  FUN_VALUE(str.find_first_of("defg", 10));
  FUN_VALUE(str.find_first_of("gnirts"));
  FUN_VALUE(str.find_first_of("abc", 6));
  FUN_VALUE(str.find_first_of("abcdf", 2, 3));
  FUN_VALUE(str.find_first_of(str3, 1));
  FUN_VALUE(str.find_first_of(str3, 10));
  FUN_VALUE(str.find_first_not_of('a'));
  FUN_VALUE(str.find_first_not_of('d'));
  FUN_VALUE(str.find_first_not_of('g', 14));
  FUN_VALUE(str.find_first_not_of("abc"));
  FUN_VALUE(str.find_first_not_of("ggggg", 14, 4));
  FUN_VALUE(str.find_first_not_of(str3));
  FUN_VALUE(str.find_first_not_of(str3, 3));
  FUN_VALUE(str.find_last_of('a'));
  FUN_VALUE(str.find_last_of('a', 4));
  FUN_VALUE(str.find_last_of('g'));
  FUN_VALUE(str.find_last_of("gg"));
  FUN_VALUE(str.find_last_of("gg", 14));
  FUN_VALUE(str.find_last_of("ggg", 14, 1));
  FUN_VALUE(str.find_last_of(str3));
  FUN_VALUE(str.find_last_of(str3, 3));
  FUN_VALUE(str.find_last_not_of('g'));
  FUN_VALUE(str.find_last_not_of('a'));
  FUN_VALUE(str.find_last_not_of('a', 1));
  FUN_VALUE(str.find_last_not_of("ggg"));
  FUN_VALUE(str.find_last_not_of("ggg", 14));
  FUN_VALUE(str.find_last_not_of("abc", 3, 1));
  FUN_VALUE(str.find_last_not_of(str3));
  FUN_VALUE(str.find_last_not_of(str3, 2));
  FUN_VALUE(str.count('a'));
  FUN_VALUE(str.count('a', 2));
  FUN_VALUE(str.count('d', 10));

  STR_FUN_AFTER(str, str.swap(str3));
  FUN_VALUE(str.size());
  FUN_VALUE(str.length());
  FUN_VALUE(str.capacity());
  STR_FUN_AFTER(str, str += str);
  STR_FUN_AFTER(str, str += 'a');
  STR_FUN_AFTER(str, str += "bc");
  FUN_VALUE(str.size());
  FUN_VALUE(str.length());
  FUN_VALUE(str.capacity());
  STR_FUN_AFTER(str, str.shrink_to_fit());
  FUN_VALUE(str.capacity());
  STR_FUN_AFTER(str, str.reserve(50));
  FUN_VALUE(str.capacity());
  STR_FUN_AFTER(str3, str3 = "test");
  STR_FUN_AFTER(str4, str4 = " ok!");
  std::cout << " str3 + '!' : " << str3 + '!' << std::endl;
  std::cout << " '#' + str3 : " << '#' + str3 << std::endl;
  std::cout << " str3 + \" success\" : " << str3 + " success" << std::endl;
  std::cout << " \"My \" + str3 : " << "My " + str3 << std::endl;
  std::cout << " str3 + str4 : " << str3 + str4 << std::endl;
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|        append       |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(string, append, "s", LEN1 _LL, LEN2 _LL, LEN3 _LL);
#else
  CON_TEST_P1(string, append, "s", LEN1 _L, LEN2 _L, LEN3 _L);
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[----------------- End container test : string -----------------]" << std::endl;
}

} // namespace string_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_STRING_TEST_H_

