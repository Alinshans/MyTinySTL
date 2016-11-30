#ifndef MYTINYSTL_VECTOR_TEST_H_
#define MYTINYSTL_VECTOR_TEST_H_

// vector test : 测试 vector 的接口与 push_back 的性能

#include <vector>

#include "../vector.h"
#include "test.h"

namespace mystl {
namespace test {
namespace vector_test {

void vector_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------------- Run container test : vector -----------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 1,2,3,4,5 };
    mystl::vector<int> v1;
    mystl::vector<int> v2(10);
    mystl::vector<int> v3(10, 1);
    mystl::vector<int> v4(a, a + 5);
    mystl::vector<int> v5(v2);
    mystl::vector<int> v6(std::move(v2));
    mystl::vector<int> v7 = v3;
    mystl::vector<int> v8 = std::move(v3);
    v8.~vector();
                
    FUN_AFTER(v1, v1.assign(10));
    FUN_AFTER(v1, v1.assign(8, 8));
    FUN_AFTER(v1, v1.assign(a, a + 5));
    FUN_AFTER(v1, v1.push_back(6));
    FUN_AFTER(v1, v1.insert(v1.begin()));
    FUN_AFTER(v1, v1.insert(v1.end(), 7));
    FUN_AFTER(v1, v1.insert(v1.begin() + 3, 2, 3));
    FUN_AFTER(v1, v1.insert(v1.begin(), a, a + 5));
    FUN_AFTER(v1, v1.pop_back());
    FUN_AFTER(v1, v1.erase(v1.begin()));
    FUN_AFTER(v1, v1.erase(v1.begin(), v1.begin() + 2));
    FUN_AFTER(v1, v1.reverse());
    FUN_AFTER(v1, v1.swap(v4));
    FUN_VALUE(*v1.begin());
    FUN_VALUE(*(v1.end() - 1));
    FUN_VALUE(*v1.rbegin());
    FUN_VALUE(*(v1.rend() - 1));
    FUN_VALUE(v1.front());
    FUN_VALUE(v1.back());
    FUN_VALUE(v1[0]);
    FUN_VALUE(v1.at(1));
    int* p = v1.data();
    *p = 10;
    *++p = 20;
    p[1] = 30;
    std::cout << " After change v1.data() :" << std::endl;
    COUT(v1);
    std::cout << std::boolalpha;
    FUN_VALUE(v1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.max_size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.resize(10));
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.shrink_to_fit());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.resize(6, 6));
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.shrink_to_fit());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.clear());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.reserve(5));
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.reserve(20));
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.shrink_to_fit());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|      push_back      |";
    CON_TEST_P1(vector<int>, push_back, rand(), LEN2, LEN3, LEN4);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[----------------- End container test : vector -----------------]" << std::endl;
}

} // namespace vector_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_VECTOR_TEST_H_

