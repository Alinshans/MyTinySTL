#ifndef MYTINYSTL_MAP_TEST_H_
#define MYTINYSTL_MAP_TEST_H_

// map test : 测试 map, multimap 的接口与它们 insert 的性能

#include <map>

#include "../map.h"
#include "test.h"

namespace mystl {
namespace test {
namespace map_test {

// pair 的宏定义
#define PAIR    mystl::pair<int, int>

// map 的遍历输出
#define MAP_COUT(m) do { \
    std::string m_name = #m; \
    std::cout << " " << m_name << " :"; \
    for (auto it : m)    std::cout << " " << m_name << "[" << it.first << "] = " << it.second; \
    std::cout << std::endl; \
} while(0)

// map 的函数操作
#define MAP_FUN_AFTER(con, fun) do { \
    std::string str = #fun; \
    std::cout << " After " << str << " :" << std::endl; \
    fun; \
    MAP_COUT(con); \
} while(0)

// map 的函数值
#define MAP_VALUE(fun) do { \
    std::string str = #fun; \
    auto it = fun; \
    std::cout << " " << str << " :" << " pair<" << it.first << ", " << it.second << ">" << std::endl; \
} while(0)

void map_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[------------------ Run container test : map -------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    mystl::vector<PAIR> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(PAIR(i, i));
    mystl::map<int, int> m1;
    mystl::map<int, int> m2(v.begin(), v.end());
    mystl::map<int, int> m3(v.begin(), v.end());
    mystl::map<int, int> m4(m2);
    mystl::map<int, int> m5(std::move(m2));
    mystl::map<int, int> m6 = m3;
    mystl::map<int, int> m7 = std::move(m3);

    MAP_FUN_AFTER(m1, m1.insert(v.begin(), v.end()));
    MAP_FUN_AFTER(m1, m1.insert(PAIR(5, 5)));
    MAP_FUN_AFTER(m1, m1.insert(m1.end(), PAIR(5, 5)));
    MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
    MAP_FUN_AFTER(m1, m1.erase(1));
    MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.find(3)));
    MAP_FUN_AFTER(m1, m1.clear());
    MAP_FUN_AFTER(m1, m1.swap(m4));
    MAP_VALUE(*m1.begin());
    MAP_VALUE(*m1.rbegin());
    FUN_VALUE(m1[1]);
    std::cout << std::boolalpha;
    FUN_VALUE(m1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(m1.size());
    FUN_VALUE(m1.max_size());
    FUN_VALUE(m1.count(1));
    MAP_VALUE(*m1.find(3));
    MAP_VALUE(*m1.lower_bound(3));
    MAP_VALUE(*m1.upper_bound(3));
    auto first = *m1.equal_range(3).first;
    auto second = *m1.equal_range(3).second;
    std::cout << " m1.equal_range(3) : from pair<" << first.first << ", " << first.second
        << "> to pair<" << second.first << ", " << second.second << ">" << std::endl;
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
    TEST_LEN(LEN1, LEN2, LEN3, WIDE);
    std::cout << "|         std         |";
    MAP_INSERT_TEST(std, map, LEN1);
    MAP_INSERT_TEST(std, map, LEN2);
    MAP_INSERT_TEST(std, map, LEN3);
    std::cout << std::endl << "|        mystl        |";
    MAP_INSERT_TEST(mystl, map, LEN1);
    MAP_INSERT_TEST(mystl, map, LEN2);
    MAP_INSERT_TEST(mystl, map, LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[------------------ End container test : map -------------------]" << std::endl;
}

void multimap_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[---------------- Run container test : multimap ----------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    mystl::vector<PAIR> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(PAIR(i, i));
    mystl::multimap<int, int> m1;
    mystl::multimap<int, int> m2(v.begin(), v.end());
    mystl::multimap<int, int> m3(v.begin(), v.end());
    mystl::multimap<int, int> m4(m2);
    mystl::multimap<int, int> m5(std::move(m2));
    mystl::multimap<int, int> m6 = m3;
    mystl::multimap<int, int> m7 = std::move(m3);

    MAP_FUN_AFTER(m1, m1.insert(v.begin(), v.end()));
    MAP_FUN_AFTER(m1, m1.insert(PAIR(5, 5)));
    MAP_FUN_AFTER(m1, m1.insert(m1.end(), PAIR(5, 5)));
    MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
    MAP_FUN_AFTER(m1, m1.erase(1));
    MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.find(3)));
    MAP_FUN_AFTER(m1, m1.clear());
    MAP_FUN_AFTER(m1, m1.swap(m4));
    MAP_FUN_AFTER(m1, m1.insert(PAIR(3, 3)));
    MAP_VALUE(*m1.begin());
    MAP_VALUE(*m1.rbegin());
    FUN_VALUE(m1[1]);
    std::cout << std::boolalpha;
    FUN_VALUE(m1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(m1.size());
    FUN_VALUE(m1.max_size());
    FUN_VALUE(m1.count(3));
    MAP_VALUE(*m1.find(3));
    MAP_VALUE(*m1.lower_bound(3));
    MAP_VALUE(*m1.upper_bound(3));
    auto first = *m1.equal_range(3).first;
    auto second = *m1.equal_range(3).second;
    std::cout << " m1.equal_range(3) : from pair<" << first.first << ", " << first.second
        << "> to pair<" << second.first << ", " << second.second << ">" << std::endl;
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
    TEST_LEN(LEN1, LEN2, LEN3, WIDE);
    std::cout << "|         std         |";
    MAP_INSERT_TEST(std, multimap, LEN1);
    MAP_INSERT_TEST(std, multimap, LEN2);
    MAP_INSERT_TEST(std, multimap, LEN3);
    std::cout << std::endl << "|        mystl        |";
    MAP_INSERT_TEST(mystl, multimap, LEN1);
    MAP_INSERT_TEST(mystl, multimap, LEN2);
    MAP_INSERT_TEST(mystl, multimap, LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[---------------- End container test : multimap ----------------]" << std::endl;
}

} // namespace map_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_MAP_TEST_H_

