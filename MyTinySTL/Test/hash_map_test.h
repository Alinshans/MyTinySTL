#ifndef MYTINYSTL_HASH_MAP_TEST_H_
#define MYTINYSTL_HASH_MAP_TEST_H_

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

// hash_map test : 测试 hash_map,hash_multimap 的 API 与它们 insert 的性能

#include <iomanip>
#include <iostream>
#include <hash_map>

#include "..\hash_map.h"
#include "map_test.h"
#include "test.h"

using namespace std;

namespace mystl {
	namespace test {
		namespace hash_map_test {

			void hash_map_test() {
				cout << "[=========================================================]" << endl;
				cout << "[------------- Run container test : hash_map -------------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
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
				cout << boolalpha;
				FUN_VALUE(hm1.empty());
				cout << noboolalpha;
				FUN_VALUE(hm1.size());
				FUN_VALUE(hm1.max_size());
				FUN_VALUE(hm1.bucket_count());
				FUN_VALUE(hm1.max_bucket_count());
				MAP_FUN_AFTER(hm1, hm1.resize(1000));
				FUN_VALUE(hm1.size());
				FUN_VALUE(hm1.bucket_count());
				FUN_VALUE(hm1.elems_in_bucket(1));
				FUN_VALUE(hm1.elems_in_bucket(2));
				FUN_VALUE(hm1.elems_in_bucket(3));
				FUN_VALUE(hm1.count(1));
				MAP_VALUE(*hm1.find(3));
				auto first = *hm1.equal_range(3).first;
				auto second = *hm1.equal_range(3).second;
				cout << " hm1.equal_range(3) : from pair<" << first.first << ", " << first.second
					<< "> to pair<" << second.first << ", " << second.second << ">" << endl;
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|    insert     |   100000    |   1000000   |  10000000   |" << endl;
				cout << "|      std      |";
				MAP_INSERT_TEST(std, hash_map, 100000);
				MAP_INSERT_TEST(std, hash_map, 1000000);
				MAP_INSERT_TEST(std, hash_map, 10000000);
				cout << endl << "|     mystl     |";
				MAP_INSERT_TEST(mystl, hash_map, 100000);
				MAP_INSERT_TEST(mystl, hash_map, 1000000);
				MAP_INSERT_TEST(mystl, hash_map, 10000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[------------- Run container test : hash_map -------------]" << endl;
			}

			void hash_multimap_test() {
				cout << "[=========================================================]" << endl;
				cout << "[----------- Run container test : hash_multimap ----------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
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
				cout << boolalpha;
				FUN_VALUE(hm1.empty());
				cout << noboolalpha;
				FUN_VALUE(hm1.size());
				FUN_VALUE(hm1.max_size());
				FUN_VALUE(hm1.bucket_count());
				FUN_VALUE(hm1.max_bucket_count());
				MAP_FUN_AFTER(hm1, hm1.resize(1000));
				FUN_VALUE(hm1.size());
				FUN_VALUE(hm1.bucket_count());
				FUN_VALUE(hm1.elems_in_bucket(1));
				FUN_VALUE(hm1.elems_in_bucket(2));
				FUN_VALUE(hm1.elems_in_bucket(3));
				FUN_VALUE(hm1.count(1));
				MAP_VALUE(*hm1.find(3));
				auto first = *hm1.equal_range(3).first;
				auto second = *hm1.equal_range(3).second;
				cout << " hm1.equal_range(3) : from pair<" << first.first << ", " << first.second
					<< "> to pair<" << second.first << ", " << second.second << ">" << endl;
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|    insert     |   100000    |   1000000   |  10000000   |" << endl;
				cout << "|      std      |";
				MAP_INSERT_TEST(std, hash_multimap, 100000);
				MAP_INSERT_TEST(std, hash_multimap, 1000000);
				MAP_INSERT_TEST(std, hash_multimap, 10000000);
				cout << endl << "|     mystl     |";
				MAP_INSERT_TEST(mystl, hash_multimap, 100000);
				MAP_INSERT_TEST(mystl, hash_multimap, 1000000);
				MAP_INSERT_TEST(mystl, hash_multimap, 10000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[----------- Run container test : hash_multimap ----------]" << endl;
			}
		}
	}
}
#endif // !MYTINYSTL_HASH_MAP_TEST_H_

