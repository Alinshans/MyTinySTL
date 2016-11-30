#ifndef MYTINYSTL_HASH_SET_TEST_H_
#define MYTINYSTL_HASH_SET_TEST_H_

// hash_set test : 测试 hash_set, hash_multiset 的接口与它们 insert 的性能

#include <unordered_set>

#include "../hash_set.h"
#include "set_test.h"
#include "test.h"

namespace mystl {
namespace test {
namespace hash_set_test {
            
void hash_set_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[---------------- Run container test : hash_set ----------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 5,4,3,2,1 };
    mystl::hash_set<int> hs1;
    mystl::hash_set<int> hs2(520);
    mystl::hash_set<int> hs3(520, mystl::hash<int>());
    mystl::hash_set<int> hs4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::hash_set<int> hs5(a, a + 5);
    mystl::hash_set<int> hs6(a, a + 5, 520);
    mystl::hash_set<int> hs7(a, a + 5, 520, mystl::hash<int>());
    mystl::hash_set<int> hs8(a, a + 5, 520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::hash_set<int> hs9(hs2);
    mystl::hash_set<int> hs10(std::move(hs2));
    mystl::hash_set<int> hs11 = hs3;
    mystl::hash_set<int> hs12 = std::move(hs3);

    FUN_AFTER(hs1, hs1.insert(a, a + 5));
    FUN_AFTER(hs1, hs1.insert(5));
    FUN_AFTER(hs1, hs1.insert_noresize(5));
    FUN_AFTER(hs1, hs1.erase(hs1.begin()));
    FUN_AFTER(hs1, hs1.erase(1));
    FUN_AFTER(hs1, hs1.erase(hs1.begin(), hs1.find(3)));
    FUN_AFTER(hs1, hs1.clear());
    FUN_AFTER(hs1, hs1.swap(hs5));
    FUN_VALUE(*hs1.begin());
    std::cout << std::boolalpha;
    FUN_VALUE(hs1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(hs1.size());
    FUN_VALUE(hs1.max_size());
    FUN_VALUE(hs1.bucket_count());
    FUN_VALUE(hs1.max_bucket_count());
    FUN_AFTER(hs1, hs1.reserve(1000));
    FUN_VALUE(hs1.size());
    FUN_VALUE(hs1.bucket_count());
    FUN_VALUE(hs1.elems_in_bucket(1));
    FUN_VALUE(hs1.elems_in_bucket(2));
    FUN_VALUE(hs1.elems_in_bucket(3));
    FUN_VALUE(hs1.count(1));
    FUN_VALUE(*hs1.find(3));
    auto first = *hs1.equal_range(3).first;
    auto second = *hs1.equal_range(3).second;
    std::cout << " hs1.equal_range(3) : from " << first << " to " << second << std::endl;
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
    TEST_LEN(LEN1, LEN2, LEN3, WIDE);
    std::cout << "|         std         |";
    FUN_TEST_FORMAT1(std::unordered_set<int>, insert, rand(), LEN1);
    FUN_TEST_FORMAT1(std::unordered_set<int>, insert, rand(), LEN2);
    FUN_TEST_FORMAT1(std::unordered_set<int>, insert, rand(), LEN3);
    std::cout << std::endl << "|        mystl        |";
    FUN_TEST_FORMAT1(mystl::hash_set<int>, insert, rand(), LEN1);
    FUN_TEST_FORMAT1(mystl::hash_set<int>, insert, rand(), LEN2);
    FUN_TEST_FORMAT1(mystl::hash_set<int>, insert, rand(), LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[---------------- End container test : hash_set ----------------]" << std::endl;
}

void hash_multiset_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[-------------- Run container test : hash_multiset -------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 5,4,3,2,1 };
    mystl::hash_multiset<int> hs1;
    mystl::hash_multiset<int> hs2(520);
    mystl::hash_multiset<int> hs3(520, mystl::hash<int>());
    mystl::hash_multiset<int> hs4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::hash_multiset<int> hs5(a, a + 5);
    mystl::hash_multiset<int> hs6(a, a + 5, 520);
    mystl::hash_multiset<int> hs7(a, a + 5, 520, mystl::hash<int>());
    mystl::hash_multiset<int> hs8(a, a + 5, 520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::hash_multiset<int> hs9(hs2);
    mystl::hash_multiset<int> hs10(std::move(hs2));
    mystl::hash_multiset<int> hs11 = hs3;
    mystl::hash_multiset<int> hs12 = std::move(hs3);

    FUN_AFTER(hs1, hs1.insert(a, a + 5));
    FUN_AFTER(hs1, hs1.insert(5));
    FUN_AFTER(hs1, hs1.insert_noresize(5));
    FUN_AFTER(hs1, hs1.erase(hs1.begin()));
    FUN_AFTER(hs1, hs1.erase(1));
    FUN_AFTER(hs1, hs1.erase(hs1.begin(), hs1.find(3)));
    FUN_AFTER(hs1, hs1.clear());
    FUN_AFTER(hs1, hs1.swap(hs5));
    FUN_VALUE(*hs1.begin());
    std::cout << std::boolalpha;
    FUN_VALUE(hs1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(hs1.size());
    FUN_VALUE(hs1.max_size());
    FUN_VALUE(hs1.bucket_count());
    FUN_VALUE(hs1.max_bucket_count());
    FUN_AFTER(hs1, hs1.reserve(1000));
    FUN_VALUE(hs1.size());
    FUN_VALUE(hs1.bucket_count());
    FUN_VALUE(hs1.elems_in_bucket(1));
    FUN_VALUE(hs1.elems_in_bucket(2));
    FUN_VALUE(hs1.elems_in_bucket(3));
    FUN_VALUE(hs1.count(1));
    FUN_VALUE(*hs1.find(3));
    auto first = *hs1.equal_range(3).first;
    auto second = *hs1.equal_range(3).second;
    std::cout << " hs1.equal_range(3) : from " << first << " to " << second << std::endl;
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
    TEST_LEN(LEN1, LEN2, LEN3, WIDE);
    std::cout << "|         std         |";
    FUN_TEST_FORMAT1(std::unordered_multiset<int>, insert, rand(), LEN1);
    FUN_TEST_FORMAT1(std::unordered_multiset<int>, insert, rand(), LEN2);
    FUN_TEST_FORMAT1(std::unordered_multiset<int>, insert, rand(), LEN3);
    std::cout << std::endl << "|        mystl        |";
    FUN_TEST_FORMAT1(mystl::hash_multiset<int>, insert, rand(), LEN1);
    FUN_TEST_FORMAT1(mystl::hash_multiset<int>, insert, rand(), LEN2);
    FUN_TEST_FORMAT1(mystl::hash_multiset<int>, insert, rand(), LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[-------------- End container test : hash_multiset -------------]" << std::endl;
}

} // namespace hash_set_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_HASH_SET_TEST_H_

