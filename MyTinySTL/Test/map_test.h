#ifndef MYTINYSTL_MAP_TEST_H
#define MYTINYSTL_MAP_TEST_H

// map 测试: 测试 map、multimap 的 API 与它们 insert 的性能

#include <iomanip>
#include <iostream>
#include <map>

#include "..\map.h"
#include "test.h"

using namespace std;

namespace mystl {
	namespace test {
		namespace map_test {

			// pair 的宏定义
			#define PAIR	mystl::pair<int, int>

			// 遍历输出 map 的宏定义
			#define MAP_COUT(m) do { \
				std::string m_name = #m; \
				cout << " " << m_name << " :"; \
				for (auto it : m)	cout << " " << m_name << "[" << it.first << "] = " << it.second; \
				cout << endl; \
			} while(0)

			// map 函数操作的宏定义
			#define MAP_FUN_AFTER(con, fun) do { \
				std::string str = #fun; \
				cout << " After " << str << " :" << endl; \
				fun; \
				MAP_COUT(con); \
			} while(0)

			// map 返回函数值的宏定义
			#define MAP_VALUE(fun) do { \
				std::string str = #fun; \
				auto it = fun; \
				cout << " " << str << " :" << " pair<" << it.first << ", " << it.second << ">" << endl; \
			} while(0)

			// map，multimap insert 的性能测试
			#define MAP_INSERT_TEST(mode, con, count) do { \
				srand((int)time(0)); \
				clock_t start, end; \
				mode::con<int, int> m; \
				char buf[10]; \
				start = clock(); \
				for (size_t i = 0; i < count; ++i) \
					m.insert(mode::pair<int, int>(rand(), rand())); \
				end = clock(); \
				int n = end - start; \
				sprintf_s(buf, "%d", n); \
				string t = buf; \
				t += "ms    |"; \
				cout << setw(14) << t; \
			} while(0)

			void map_test() {
				cout << "[=========================================================]" << endl;
				cout << "[--------------- Run container test : map ----------------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
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
				cout << boolalpha;
				FUN_VALUE(m1.empty());
				cout << noboolalpha;
				FUN_VALUE(m1.size());
				FUN_VALUE(m1.max_size());
				FUN_VALUE(m1.count(1));
				MAP_VALUE(*m1.find(3));
				MAP_VALUE(*m1.lower_bound(3));
				MAP_VALUE(*m1.upper_bound(3));
				auto first = *m1.equal_range(3).first;
				auto second = *m1.equal_range(3).second;
				cout << " m1.equal_range(3) : from pair<" << first.first << ", " << first.second
					<< "> to pair<" << second.first << ", " << second.second << ">" << endl;
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|    insert     |   100000    |   1000000   |  10000000   |" << endl;
				cout << "|      std      |";
				MAP_INSERT_TEST(std, map, 100000);
				MAP_INSERT_TEST(std, map, 1000000);
				MAP_INSERT_TEST(std, map, 10000000);
				cout << endl << "|     mystl     |";
				MAP_INSERT_TEST(mystl, map, 100000);
				MAP_INSERT_TEST(mystl, map, 1000000);
				MAP_INSERT_TEST(mystl, map, 10000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[--------------- End container test : map ----------------]" << endl;
			}

			void multimap_test() {
				cout << "[=========================================================]" << endl;
				cout << "[------------- Run container test : multimap -------------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
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
				cout << boolalpha;
				FUN_VALUE(m1.empty());
				cout << noboolalpha;
				FUN_VALUE(m1.size());
				FUN_VALUE(m1.max_size());
				FUN_VALUE(m1.count(3));
				MAP_VALUE(*m1.find(3));
				MAP_VALUE(*m1.lower_bound(3));
				MAP_VALUE(*m1.upper_bound(3));
				auto first = *m1.equal_range(3).first;
				auto second = *m1.equal_range(3).second;
				cout << " m1.equal_range(3) : from pair<" << first.first << ", " << first.second
					<< "> to pair<" << second.first << ", " << second.second << ">" << endl;
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|    insert     |   100000    |   1000000   |  10000000   |" << endl;
				cout << "|      std      |";
				MAP_INSERT_TEST(std, multimap, 100000);
				MAP_INSERT_TEST(std, multimap, 1000000);
				MAP_INSERT_TEST(std, multimap, 10000000);
				cout << endl << "|     mystl     |";
				MAP_INSERT_TEST(mystl, multimap, 100000);
				MAP_INSERT_TEST(mystl, multimap, 1000000);
				MAP_INSERT_TEST(mystl, multimap, 10000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[------------- Run container test : multimap -------------]" << endl;
			}
		}
	}
}
#endif // !MYTINYSTL_MAP_TEST_H

