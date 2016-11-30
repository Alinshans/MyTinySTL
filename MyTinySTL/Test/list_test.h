#ifndef MYTINYSTL_LIST_TEST_H_
#define MYTINYSTL_LIST_TEST_H_

// list test : 测试 list 的接口与 insert, sort 的性能

#include <list>

#include "../list.h"
#include "test.h"

namespace mystl {
namespace test {
namespace list_test {

// 一个辅助测试函数
bool is_odd(int x) { return x & 1; }

void list_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[------------------ Run container test : list ------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 1,2,3,4,5 };
    mystl::list<int> l1;
    mystl::list<int> l2(5);
    mystl::list<int> l3(5, 1);
    mystl::list<int> l4(a, a + 5);
    mystl::list<int> l5(l2);
    mystl::list<int> l6(std::move(l2));
    mystl::list<int> l7 = l3;
    mystl::list<int> l8 = std::move(l3);
    l8.~list();

    FUN_AFTER(l1, l1.assign(10));
    FUN_AFTER(l1, l1.assign(8, 8));
    FUN_AFTER(l1, l1.assign(a, a + 5));
    FUN_AFTER(l1, l1.insert(l1.begin()));
    FUN_AFTER(l1, l1.insert(l1.end(), 6));
    FUN_AFTER(l1, l1.insert(l1.end(), 2, 7));
    FUN_AFTER(l1, l1.insert(l1.begin(), a, a + 5));
    FUN_AFTER(l1, l1.erase(l1.begin()));
    FUN_AFTER(l1, l1.erase(l1.begin(), l1.end()));
    FUN_AFTER(l1, l1.push_back(2));
    FUN_AFTER(l1, l1.push_front(1));
    FUN_AFTER(l1, l1.pop_front());
    FUN_AFTER(l1, l1.pop_back());
    FUN_AFTER(l1, l1.resize(10));
    FUN_AFTER(l1, l1.resize(5, 1));
    FUN_AFTER(l1, l1.splice(l1.end(), l4));
    FUN_AFTER(l1, l1.splice(l1.end(), l5, l5.begin()));
    FUN_AFTER(l1, l1.splice(l1.begin(), l6, l6.begin(), l6.end()));
    FUN_AFTER(l1, l1.remove(1));
    FUN_AFTER(l1, l1.remove_if(is_odd));
    FUN_AFTER(l1, l1.sort());
    FUN_AFTER(l1, l1.unique());
    FUN_AFTER(l1, l1.merge(l7));
    FUN_AFTER(l1, l1.reverse());
    FUN_AFTER(l1, l1.clear());
    mystl::list<int> l9(a, a + 5);
    FUN_AFTER(l1, l1.swap(l9));
    FUN_VALUE(*l1.begin());
    FUN_VALUE(*l1.rbegin());
    FUN_VALUE(l1.front());
    FUN_VALUE(l1.back());
    std::cout << std::boolalpha;
    FUN_VALUE(l1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(l1.size());
    FUN_VALUE(l1.max_size());
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
    CON_TEST_P2(list<int>, insert, end, rand(), LEN1, LEN2, LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|         sort        |";
    TEST_LEN(LEN1, LEN2, LEN3, WIDE);
    std::cout << "|         std         |";
    LIST_SORT_TEST(std, LEN1);
    LIST_SORT_TEST(std, LEN2);
    LIST_SORT_TEST(std, LEN3);
    std::cout << std::endl << "|        mystl        |";
    LIST_SORT_TEST(mystl, LEN1);
    LIST_SORT_TEST(mystl, LEN2);
    LIST_SORT_TEST(mystl, LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[------------------ End container test : list ------------------]" << std::endl;
}

} // namespace list_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_LIST_TEST_H_

