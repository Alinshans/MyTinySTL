#ifndef MYTINYSTL_UNORDERED_MAP_TEST_H_
#define MYTINYSTL_UNORDERED_MAP_TEST_H_

// unordered_map test : 测试 unordered_map, unordered_multimap 的接口与它们 insert 的性能

#include <unordered_map>

#include "../unordered_map.h"
#include "map_test.h"
#include "test.h"

namespace mystl {
namespace test {
namespace unordered_map_test {
            
void unordered_map_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[-------------- Run container test : unordered_map -------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    mystl::vector<PAIR> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(PAIR(5 - i, 5 - i));
    mystl::unordered_map<int, int> um1;
    mystl::unordered_map<int, int> um2(520);
    mystl::unordered_map<int, int> um3(520, mystl::hash<int>());
    mystl::unordered_map<int, int> um4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_map<int, int> um5(v.begin(), v.end());
    mystl::unordered_map<int, int> um6(v.begin(), v.end());
    mystl::unordered_map<int, int> um7(v.begin(), v.end(), mystl::hash<int>());
    mystl::unordered_map<int, int> um8(v.begin(), v.end(), mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_map<int, int> um9(um5);
    mystl::unordered_map<int, int> um10(std::move(um5));
    mystl::unordered_map<int, int> um11 = um6;
    mystl::unordered_map<int, int> um12 = std::move(um6);
    mystl::unordered_map<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
    mystl::unordered_map<int, int> um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };
    MAP_COUT(um1);
    MAP_COUT(um2);
    MAP_COUT(um3);
    MAP_COUT(um4);
    MAP_COUT(um5);
    MAP_COUT(um6);
    MAP_COUT(um7);
    MAP_COUT(um8);
    MAP_COUT(um9);
    MAP_COUT(um10);
    MAP_COUT(um11);
    MAP_COUT(um12);
    MAP_COUT(um13);
    MAP_COUT(um14);

    MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));
    MAP_FUN_AFTER(um1, um1.insert(PAIR(5, 5)));
    MAP_FUN_AFTER(um1, um1.insert_noresize(PAIR(5, 5)));
    MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
    MAP_FUN_AFTER(um1, um1.erase(1));
    MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(3)));
    MAP_FUN_AFTER(um1, um1.clear());
    MAP_FUN_AFTER(um1, um1.swap(um7));
    MAP_VALUE(*um1.begin());
    FUN_VALUE(um1[1]);
    std::cout << std::boolalpha;
    FUN_VALUE(um1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.max_size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.max_bucket_count());
    MAP_FUN_AFTER(um1, um1.reserve(1000));
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.elems_in_bucket(1));
    FUN_VALUE(um1.elems_in_bucket(2));
    FUN_VALUE(um1.elems_in_bucket(3));
    FUN_VALUE(um1.count(1));
    MAP_VALUE(*um1.find(3));
    auto first = *um1.equal_range(3).first;
    auto second = *um1.equal_range(3).second;
    std::cout << " um1.equal_range(3) : from pair<" << first.first << ", " << first.second
        << "> to pair<" << second.first << ", " << second.second << ">" << std::endl;
    PASSED;
#if PERFORMANCE_TEST_ON
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
#if MEMORY_IS_ENOUGH
    MAP_INSERT_TEST(unordered_map, LEN1 _M, LEN2 _M, LEN3 _M);
#else
    MAP_INSERT_TEST(unordered_map, LEN1 _S, LEN2 _S, LEN3 _S);
#endif
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
#endif
    std::cout << "[-------------- End container test : unordered_map -------------]" << std::endl;
}

void unordered_multimap_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[-------------- Run container test : unordered_multimap -------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    mystl::vector<PAIR> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(PAIR(5 - i, 5 - i));
    mystl::unordered_multimap<int, int> um1;
    mystl::unordered_multimap<int, int> um2(520);
    mystl::unordered_multimap<int, int> um3(520, mystl::hash<int>());
    mystl::unordered_multimap<int, int> um4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_multimap<int, int> um5(v.begin(), v.end());
    mystl::unordered_multimap<int, int> um6(v.begin(), v.end());
    mystl::unordered_multimap<int, int> um7(v.begin(), v.end(), mystl::hash<int>());
    mystl::unordered_multimap<int, int> um8(v.begin(), v.end(), mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_multimap<int, int> um9(um5);
    mystl::unordered_multimap<int, int> um10(std::move(um5));
    mystl::unordered_multimap<int, int> um11 = um6;
    mystl::unordered_multimap<int, int> um12 = std::move(um6);
    mystl::unordered_multimap<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
    mystl::unordered_multimap<int, int> um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };
    MAP_COUT(um1);
    MAP_COUT(um2);
    MAP_COUT(um3);
    MAP_COUT(um4);
    MAP_COUT(um5);
    MAP_COUT(um6);
    MAP_COUT(um7);
    MAP_COUT(um8);
    MAP_COUT(um9);
    MAP_COUT(um10);
    MAP_COUT(um11);
    MAP_COUT(um12);
    MAP_COUT(um13);
    MAP_COUT(um14);

    MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));
    MAP_FUN_AFTER(um1, um1.insert(PAIR(5, 5)));
    MAP_FUN_AFTER(um1, um1.insert_noresize(PAIR(5, 5)));
    MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
    MAP_FUN_AFTER(um1, um1.erase(1));
    MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(3)));
    MAP_FUN_AFTER(um1, um1.clear());
    MAP_FUN_AFTER(um1, um1.swap(um7));
    MAP_VALUE(*um1.begin());
    FUN_VALUE(um1[1]);
    std::cout << std::boolalpha;
    FUN_VALUE(um1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.max_size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.max_bucket_count());
    MAP_FUN_AFTER(um1, um1.reserve(1000));
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.elems_in_bucket(1));
    FUN_VALUE(um1.elems_in_bucket(2));
    FUN_VALUE(um1.elems_in_bucket(3));
    FUN_VALUE(um1.count(1));
    MAP_VALUE(*um1.find(3));
    auto first = *um1.equal_range(3).first;
    auto second = *um1.equal_range(3).second;
    std::cout << " um1.equal_range(3) : from pair<" << first.first << ", " << first.second
        << "> to pair<" << second.first << ", " << second.second << ">" << std::endl;
    PASSED;
#if PERFORMANCE_TEST_ON
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
#if MEMORY_IS_ENOUGH
    MAP_INSERT_TEST(unordered_multimap, LEN1 _M, LEN2 _M, LEN3 _M);
#else
    MAP_INSERT_TEST(unordered_multimap, LEN1 _SS, LEN2 _SS, LEN3 _SS);
#endif
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
#endif
    std::cout << "[-------------- End container test : unordered_multimap -------------]" << std::endl;
}

} // namespace unordered_map_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_UNORDERED_MAP_TEST_H_

