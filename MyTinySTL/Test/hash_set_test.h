#ifndef MYTINYSTL_HASH_SET_TEST_H_
#define MYTINYSTL_HASH_SET_TEST_H_

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

// hash_set test : 测试 hash_set,hash_multiset 的 API 与它们 insert 的性能

#include <iomanip>
#include <iostream>
#include <hash_set>

#include "..\hash_set.h"
#include "set_test.h"
#include "test.h"

using namespace std;

namespace mystl {
	namespace test {
		namespace hash_set_test {

			void hash_set_test() {
				cout << "[=========================================================]" << endl;
				cout << "[------------- Run container test : hash_set -------------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
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
				cout << boolalpha;
				FUN_VALUE(hs1.empty());
				cout << noboolalpha;
				FUN_VALUE(hs1.size());
				FUN_VALUE(hs1.max_size());
				FUN_VALUE(hs1.bucket_count());
				FUN_VALUE(hs1.max_bucket_count());
				FUN_AFTER(hs1, hs1.resize(1000));
				FUN_VALUE(hs1.size());
				FUN_VALUE(hs1.bucket_count());
				FUN_VALUE(hs1.elems_in_bucket(1));
				FUN_VALUE(hs1.elems_in_bucket(2));
				FUN_VALUE(hs1.elems_in_bucket(3));
				FUN_VALUE(hs1.count(1));
				FUN_VALUE(*hs1.find(3));
				auto first = *hs1.equal_range(3).first;
				auto second = *hs1.equal_range(3).second;
				cout << " hs1.equal_range(3) : from " << first << " to " << second << endl;
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|    insert     |   100000    |   1000000   |  10000000   |" << endl;
				cout << "|      std      |";
				SET_INSERT_TEST(std, hash_set, 100000);
				SET_INSERT_TEST(std, hash_set, 1000000);
				SET_INSERT_TEST(std, hash_set, 10000000);
				cout << endl << "|     mystl     |";
				SET_INSERT_TEST(mystl, hash_set, 100000);
				SET_INSERT_TEST(mystl, hash_set, 1000000);
				SET_INSERT_TEST(mystl, hash_set, 10000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[------------- End container test : hash_set -------------]" << endl;
			}

			void hash_multiset_test() {
				cout << "[=========================================================]" << endl;
				cout << "[----------- Run container test : hash_multiset ----------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
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
				cout << boolalpha;
				FUN_VALUE(hs1.empty());
				cout << noboolalpha;
				FUN_VALUE(hs1.size());
				FUN_VALUE(hs1.max_size());
				FUN_VALUE(hs1.bucket_count());
				FUN_VALUE(hs1.max_bucket_count());
				FUN_AFTER(hs1, hs1.resize(1000));
				FUN_VALUE(hs1.size());
				FUN_VALUE(hs1.bucket_count());
				FUN_VALUE(hs1.elems_in_bucket(1));
				FUN_VALUE(hs1.elems_in_bucket(2));
				FUN_VALUE(hs1.elems_in_bucket(3));
				FUN_VALUE(hs1.count(1));
				FUN_VALUE(*hs1.find(3));
				auto first = *hs1.equal_range(3).first;
				auto second = *hs1.equal_range(3).second;
				cout << " hs1.equal_range(3) : from " << first << " to " << second << endl;
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|    insert     |   100000    |   1000000   |  10000000   |" << endl;
				cout << "|      std      |";
				SET_INSERT_TEST(std, hash_multiset, 100000);
				SET_INSERT_TEST(std, hash_multiset, 1000000);
				SET_INSERT_TEST(std, hash_multiset, 10000000);
				cout << endl << "|     mystl     |";
				SET_INSERT_TEST(mystl, hash_multiset, 100000);
				SET_INSERT_TEST(mystl, hash_multiset, 1000000);
				SET_INSERT_TEST(mystl, hash_multiset, 10000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[----------- Run container test : hash_multiset ----------]" << endl;
			}
		}
	}
}
#endif // !MYTINYSTL_HASH_SET_TEST_H_

