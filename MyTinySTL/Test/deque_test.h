#ifndef MYTINYSTL_DEQUE_TEST_H_
#define MYTINYSTL_DEQUE_TEST_H_

// deque test : 测试 deque 的 API 和 insert 的性能

#include <iomanip>
#include <iostream>
#include <deque>

#include "..\deque.h"
#include "test.h"

namespace mystl {
	namespace test {
		namespace deque_test {

			// deque insert 的性能测试
			#define DEQUE_INSERT_TEST(mode, count) do { \
				srand((int)time(0)); \
				clock_t start, end; \
				mode::deque<int> d; \
				char buf[10]; \
				start = clock(); \
				for (size_t i = 0; i < count; ++i) \
					d.insert(d.end(), rand()); \
				end = clock(); \
				int n = end - start; \
				sprintf_s(buf, "%d", n); \
				std::string t = buf; \
				t += "ms    |"; \
				std::cout << std::setw(WIDE) << t; \
			} while(0)

			void deque_test() {
				std::cout << "[===============================================================]" << std::endl;
				std::cout << "[----------------- Run container test : deque ------------------]" << std::endl;
				std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
				int a[] = { 1,2,3,4,5 };
				mystl::deque<int> d1;
				mystl::deque<int> d2(5);
				mystl::deque<int> d3(5, 1);
				mystl::deque<int> d4(a, a + 5);
				mystl::deque<int> d5(d2);
				mystl::deque<int> d6(std::move(d2));
				mystl::deque<int> d7 = d3;
				mystl::deque<int> d8 = std::move(d3);
				d8.~deque();

				FUN_AFTER(d1, d1.assign(10));
				FUN_AFTER(d1, d1.assign(8, 8));
				FUN_AFTER(d1, d1.assign(a, a + 5));
				FUN_AFTER(d1, d1.insert(d1.begin()));
				FUN_AFTER(d1, d1.insert(d1.end(), 6));
				FUN_AFTER(d1, d1.insert(d1.end(), 2, 7));
				FUN_AFTER(d1, d1.insert(d1.begin(), a, a + 5));
				FUN_AFTER(d1, d1.erase(d1.begin()));
				FUN_AFTER(d1, d1.erase(d1.begin(), d1.begin() + 4));
				FUN_AFTER(d1, d1.push_front(1));
				FUN_AFTER(d1, d1.push_back(2));
				FUN_AFTER(d1, d1.pop_back());
				FUN_AFTER(d1, d1.pop_front());
				FUN_AFTER(d1, d1.resize(5));
				FUN_AFTER(d1, d1.resize(8, 8));
				FUN_AFTER(d1, d1.clear());
				FUN_AFTER(d1, d1.swap(d4));
				FUN_VALUE(*(d1.begin()));
				FUN_VALUE(*(d1.end() - 1));
				FUN_VALUE(*(d1.rbegin()));
				FUN_VALUE(*(d1.rend() - 1));
				FUN_VALUE(d1.front());
				FUN_VALUE(d1.back());
				FUN_VALUE(d1.at(1));
				FUN_VALUE(d1[2]);
				std::cout << std::boolalpha;
				FUN_VALUE(d1.empty());
				std::cout << std::noboolalpha;
				FUN_VALUE(d1.size());
				FUN_VALUE(d1.max_size());
				PASSED;
				std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
				std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
				std::cout << "|       insert        |";
				TEST_LEN(LEN1, LEN2, LEN3, WIDE);
				std::cout << "|         std         |";
				DEQUE_INSERT_TEST(std, LEN1);
				DEQUE_INSERT_TEST(std, LEN2);
				DEQUE_INSERT_TEST(std, LEN3);
				std::cout << std::endl << "|        mystl        |";
				DEQUE_INSERT_TEST(mystl, LEN1);
				DEQUE_INSERT_TEST(mystl, LEN2);
				DEQUE_INSERT_TEST(mystl, LEN3);
				std::cout << std::endl;
				std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
				PASSED;
				std::cout << "[----------------- End container test : deque ------------------]" << std::endl;
			}
		}
	}
}
#endif // !MYTINYSTL_DEQUE_TEST_H_

