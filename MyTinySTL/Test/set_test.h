#ifndef MYTINYSTL_SET_TEST_H
#define MYTINYSTL_SET_TEST_H

// set 测试: 测试 set、multiset 的 API 与它们 insert 的性能

#include <iomanip>
#include <iostream>
#include <set>

#include "..\set.h"
#include "test.h"

using namespace std;

namespace mystl {
	namespace test {
		namespace set_test {

			// set，multiset insert 的性能测试
			#define SET_INSERT_TEST(mode, con, count) do { \
				srand((int)time(0)); \
				clock_t start, end; \
				mode::con<int> s; \
				char buf[10]; \
				start = clock(); \
				for (size_t i = 0; i < count; ++i) \
					s.insert(rand()); \
				end = clock(); \
				int n = end - start; \
				sprintf_s(buf, "%d", n); \
				string t = buf; \
				t += "ms    |"; \
				cout << setw(14) << t; \
			} while(0)

			void set_test(){
				cout << "[=========================================================]" << endl;
				cout << "[--------------- Run container test : set ----------------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
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
				cout << boolalpha;
				FUN_VALUE(s1.empty());
				cout << noboolalpha;
				FUN_VALUE(s1.size());
				FUN_VALUE(s1.max_size());
				FUN_VALUE(s1.count(1));
				FUN_VALUE(*s1.find(3));
				FUN_VALUE(*s1.lower_bound(3));
				FUN_VALUE(*s1.upper_bound(3));
				auto first = *s1.equal_range(3).first;
				auto second = *s1.equal_range(3).second;
				cout << " s1.equal_range(3) : from " << first << " to " << second << endl;
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|    insert     |   100000    |   1000000   |  10000000   |" << endl;
				cout << "|      std      |";
				SET_INSERT_TEST(std, set, 100000);
				SET_INSERT_TEST(std, set, 1000000);
				SET_INSERT_TEST(std, set, 10000000);
				cout << endl << "|     mystl     |";
				SET_INSERT_TEST(mystl, set, 100000);
				SET_INSERT_TEST(mystl, set, 1000000);
				SET_INSERT_TEST(mystl, set, 10000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[--------------- End container test : set ----------------]" << endl;
			}

			void multiset_test() {
				cout << "[=========================================================]" << endl;
				cout << "[------------- Run container test : multiset -------------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
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
				cout << boolalpha;
				FUN_VALUE(s1.empty());
				cout << noboolalpha;
				FUN_VALUE(s1.size());
				FUN_VALUE(s1.max_size());
				FUN_VALUE(s1.count(1));
				FUN_VALUE(*s1.find(3));
				FUN_VALUE(*s1.lower_bound(3));
				FUN_VALUE(*s1.upper_bound(3));
				auto first = *s1.equal_range(3).first;
				auto second = *s1.equal_range(3).second;
				cout << " s1.equal_range(3) : from " << first << " to " << second << endl;
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|    insert     |   100000    |   1000000   |  10000000   |" << endl;
				cout << "|      std      |";
				SET_INSERT_TEST(std, multiset, 100000);
				SET_INSERT_TEST(std, multiset, 1000000);
				SET_INSERT_TEST(std, multiset, 10000000);
				cout << endl << "|     mystl     |";
				SET_INSERT_TEST(mystl, multiset, 100000);
				SET_INSERT_TEST(mystl, multiset, 1000000);
				SET_INSERT_TEST(mystl, multiset, 10000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[------------- End container test : multiset -------------]" << endl;
			}
		}
	}
}
#endif // !MYTINYSTL_SET_TEST_H

