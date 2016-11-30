#ifndef MYTINYSTL_SET_TEST_H_
#define MYTINYSTL_SET_TEST_H_

// set test : 测试 set, multiset 的接口与它们 insert 的性能

#include <set>

#include "../set.h"
#include "test.h"

namespace mystl {
namespace test {
namespace set_test {

void set_test(){
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[------------------ Run container test : set -------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 5,4,3,2,1 };
    mystl::set<int> s1;
    mystl::set<int> s2(a, a + 5);
    mystl::set<int> s3(a, a + 5);
    mystl::set<int> s4(s2);
    mystl::set<int> s5(std::move(s2));
    mystl::set<int> s6 = s3;
    mystl::set<int> s7 = std::move(s3);

    FUN_AFTER(s1, s1.insert(a, a + 5));
    FUN_AFTER(s1, s1.insert(5));
    FUN_AFTER(s1, s1.insert(s1.end(), 5));
    FUN_AFTER(s1, s1.erase(s1.begin()));
    FUN_AFTER(s1, s1.erase(1));
    FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(3)));
    FUN_AFTER(s1, s1.clear());
    FUN_AFTER(s1, s1.swap(s4));
    FUN_VALUE(*s1.begin());
    FUN_VALUE(*s1.rbegin());
    std::cout << std::boolalpha;
    FUN_VALUE(s1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(s1.size());
    FUN_VALUE(s1.max_size());
    FUN_VALUE(s1.count(1));
    FUN_VALUE(*s1.find(3));
    FUN_VALUE(*s1.lower_bound(3));
    FUN_VALUE(*s1.upper_bound(3));
    auto first = *s1.equal_range(3).first;
    auto second = *s1.equal_range(3).second;
    std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
    CON_TEST_P1(set<int>, insert, rand(), LEN1, LEN2, LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[------------------ End container test : set -------------------]" << std::endl;
}

void multiset_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[---------------- Run container test : multiset ----------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 5,4,3,2,1 };
    mystl::multiset<int> s1;
    mystl::multiset<int> s2(a, a + 5);
    mystl::multiset<int> s3(a, a + 5);
    mystl::multiset<int> s4(s2);
    mystl::multiset<int> s5(std::move(s2));
    mystl::multiset<int> s6 = s3;
    mystl::multiset<int> s7 = std::move(s3);
            
    FUN_AFTER(s1, s1.insert(a, a + 5));
    FUN_AFTER(s1, s1.insert(5));
    FUN_AFTER(s1, s1.insert(s1.end(), 5));
    FUN_AFTER(s1, s1.erase(s1.begin()));
    FUN_AFTER(s1, s1.erase(1));
    FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(3)));
    FUN_AFTER(s1, s1.clear());
    FUN_AFTER(s1, s1.swap(s4));
    FUN_VALUE(*s1.begin());
    FUN_VALUE(*s1.rbegin());
    std::cout << std::boolalpha;
    FUN_VALUE(s1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(s1.size());
    FUN_VALUE(s1.max_size());
    FUN_VALUE(s1.count(1));
    FUN_VALUE(*s1.find(3));
    FUN_VALUE(*s1.lower_bound(3));
    FUN_VALUE(*s1.upper_bound(3));
    auto first = *s1.equal_range(3).first;
    auto second = *s1.equal_range(3).second;
    std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
    CON_TEST_P1(multiset<int>, insert, rand(), LEN1, LEN2, LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[---------------- End container test : multiset ----------------]" << std::endl;
}

} // namespace set_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_SET_TEST_H_

