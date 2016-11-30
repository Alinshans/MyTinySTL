#ifndef MYTINYSTL_HASH_MAP_TEST_H_
#define MYTINYSTL_HASH_MAP_TEST_H_

// hash_map test : 测试 hash_map, hash_multimap 的接口与它们 insert 的性能

#include <unordered_map>

#include "../hash_map.h"
#include "map_test.h"
#include "test.h"

namespace mystl {
namespace test {
namespace hash_map_test {
            
void hash_map_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[---------------- Run container test : hash_map ----------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    mystl::vector<PAIR> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(PAIR(5 - i, 5 - i));
    mystl::hash_map<int, int> hm1;
    mystl::hash_map<int, int> hm2(520);
    mystl::hash_map<int, int> hm3(520, mystl::hash<int>());
    mystl::hash_map<int, int> hm4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::hash_map<int, int> hm5(v.begin(), v.end());
    mystl::hash_map<int, int> hm6(v.begin(), v.end(), 520);
    mystl::hash_map<int, int> hm7(v.begin(), v.end(), 520, mystl::hash<int>());
    mystl::hash_map<int, int> hm8(v.begin(), v.end(), 520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::hash_map<int, int> hm9(hm2);
    mystl::hash_map<int, int> hm10(std::move(hm2));
    mystl::hash_map<int, int> hm11 = hm3;
    mystl::hash_map<int, int> hm12 = std::move(hm3);

    MAP_FUN_AFTER(hm1, hm1.insert(v.begin(), v.end()));
    MAP_FUN_AFTER(hm1, hm1.insert(PAIR(5, 5)));
    MAP_FUN_AFTER(hm1, hm1.insert_noresize(PAIR(5, 5)));
    MAP_FUN_AFTER(hm1, hm1.erase(hm1.begin()));
    MAP_FUN_AFTER(hm1, hm1.erase(1));
    MAP_FUN_AFTER(hm1, hm1.erase(hm1.begin(), hm1.find(3)));
    MAP_FUN_AFTER(hm1, hm1.clear());
    MAP_FUN_AFTER(hm1, hm1.swap(hm5));
    MAP_VALUE(*hm1.begin());
    FUN_VALUE(hm1[1]);
    std::cout << std::boolalpha;
    FUN_VALUE(hm1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(hm1.size());
    FUN_VALUE(hm1.max_size());
    FUN_VALUE(hm1.bucket_count());
    FUN_VALUE(hm1.max_bucket_count());
    MAP_FUN_AFTER(hm1, hm1.reserve(1000));
    FUN_VALUE(hm1.size());
    FUN_VALUE(hm1.bucket_count());
    FUN_VALUE(hm1.elems_in_bucket(1));
    FUN_VALUE(hm1.elems_in_bucket(2));
    FUN_VALUE(hm1.elems_in_bucket(3));
    FUN_VALUE(hm1.count(1));
    MAP_VALUE(*hm1.find(3));
    auto first = *hm1.equal_range(3).first;
    auto second = *hm1.equal_range(3).second;
    std::cout << " hm1.equal_range(3) : from pair<" << first.first << ", " << first.second
        << "> to pair<" << second.first << ", " << second.second << ">" << std::endl;
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
    TEST_LEN(LEN1, LEN2, LEN3, WIDE);
    std::cout << "|         std         |";
    MAP_INSERT_TEST(std, unordered_map, LEN1);
    MAP_INSERT_TEST(std, unordered_map, LEN2);
    MAP_INSERT_TEST(std, unordered_map, LEN3);
    std::cout << std::endl << "|        mystl        |";
    MAP_INSERT_TEST(mystl, hash_map, LEN1);
    MAP_INSERT_TEST(mystl, hash_map, LEN2);
    MAP_INSERT_TEST(mystl, hash_map, LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[---------------- End container test : hash_map ----------------]" << std::endl;
}

void hash_multimap_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[-------------- Run container test : hash_multimap -------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    mystl::vector<PAIR> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(PAIR(5 - i, 5 - i));
    mystl::hash_multimap<int, int> hm1;
    mystl::hash_multimap<int, int> hm2(520);
    mystl::hash_multimap<int, int> hm3(520, mystl::hash<int>());
    mystl::hash_multimap<int, int> hm4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::hash_multimap<int, int> hm5(v.begin(), v.end());
    mystl::hash_multimap<int, int> hm6(v.begin(), v.end(), 520);
    mystl::hash_multimap<int, int> hm7(v.begin(), v.end(), 520, mystl::hash<int>());
    mystl::hash_multimap<int, int> hm8(v.begin(), v.end(), 520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::hash_multimap<int, int> hm9(hm2);
    mystl::hash_multimap<int, int> hm10(std::move(hm2));
    mystl::hash_multimap<int, int> hm11 = hm3;
    mystl::hash_multimap<int, int> hm12 = std::move(hm3);

    MAP_FUN_AFTER(hm1, hm1.insert(v.begin(), v.end()));
    MAP_FUN_AFTER(hm1, hm1.insert(PAIR(5, 5)));
    MAP_FUN_AFTER(hm1, hm1.insert_noresize(PAIR(5, 5)));
    MAP_FUN_AFTER(hm1, hm1.erase(hm1.begin()));
    MAP_FUN_AFTER(hm1, hm1.erase(1));
    MAP_FUN_AFTER(hm1, hm1.erase(hm1.begin(), hm1.find(3)));
    MAP_FUN_AFTER(hm1, hm1.clear());
    MAP_FUN_AFTER(hm1, hm1.swap(hm5));
    MAP_VALUE(*hm1.begin());
    FUN_VALUE(hm1[1]);
    std::cout << std::boolalpha;
    FUN_VALUE(hm1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(hm1.size());
    FUN_VALUE(hm1.max_size());
    FUN_VALUE(hm1.bucket_count());
    FUN_VALUE(hm1.max_bucket_count());
    MAP_FUN_AFTER(hm1, hm1.reserve(1000));
    FUN_VALUE(hm1.size());
    FUN_VALUE(hm1.bucket_count());
    FUN_VALUE(hm1.elems_in_bucket(1));
    FUN_VALUE(hm1.elems_in_bucket(2));
    FUN_VALUE(hm1.elems_in_bucket(3));
    FUN_VALUE(hm1.count(1));
    MAP_VALUE(*hm1.find(3));
    auto first = *hm1.equal_range(3).first;
    auto second = *hm1.equal_range(3).second;
    std::cout << " hm1.equal_range(3) : from pair<" << first.first << ", " << first.second
        << "> to pair<" << second.first << ", " << second.second << ">" << std::endl;
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       insert        |";
    TEST_LEN(LEN1, LEN2, LEN3, WIDE);
    std::cout << "|         std         |";
    MAP_INSERT_TEST(std, unordered_multimap, LEN1);
    MAP_INSERT_TEST(std, unordered_multimap, LEN2);
    MAP_INSERT_TEST(std, unordered_multimap, LEN3);
    std::cout << std::endl << "|        mystl        |";
    MAP_INSERT_TEST(mystl, hash_multimap, LEN1);
    MAP_INSERT_TEST(mystl, hash_multimap, LEN2);
    MAP_INSERT_TEST(mystl, hash_multimap, LEN3);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[-------------- End container test : hash_multimap -------------]" << std::endl;
}

} // namespace hash_map_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_HASH_MAP_TEST_H_

