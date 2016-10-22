#ifndef MYTINYSTL_STACK_TEST_H
#define MYTINYSTL_STACK_TEST_H

// stack 测试: 测试 stack 的 API 和 push 的性能

#include <iomanip>
#include <iostream>
#include <stack>

#include "..\deque.h"
#include "..\stack.h"
#include "test.h"

using namespace std;

namespace mystl {
	namespace test {
		namespace stack_test {

			// 遍历输出 stack 的宏定义
			#define STACK_COUT(s) do { \
				std::string s_name = #s; \
				cout << " " << s_name << " :"; \
				stack_print(s); \
			} while(0)

			void stack_print(mystl::stack<int> s) {
				while (!s.empty()) {
					cout << " " << s.top();
					s.pop();
				}
				cout << endl;
			}

			// stack push 的性能测试
			#define STACK_PUSH_TEST(mode, count) do { \
				srand((int)time(0)); \
				clock_t start, end; \
				mode::stack<int> s; \
				char buf[10]; \
				start = clock(); \
				for (size_t i = 0; i < count; ++i) \
					s.push(rand()); \
				end = clock(); \
				int n = end - start; \
				sprintf_s(buf, "%d", n); \
				string t = buf; \
				t += "ms    |"; \
				cout << setw(14) << t; \
			} while(0)

			void stack_test() {
				cout << "[=========================================================]" << endl;
				cout << "[-------------- Run container test : stack ---------------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
				int a[] = { 1,2,3,4,5 };
				mystl::deque<int> d1(5);
				mystl::stack<int> s1;
				mystl::stack<int> s2(5);
				mystl::stack<int> s3(5, 1);
				mystl::stack<int> s4(a, a + 5);
				mystl::stack<int> s5(d1);
				mystl::stack<int> s6(std::move(d1));
				mystl::stack<int> s7(s2);
				mystl::stack<int> s8(std::move(s2));
				mystl::stack<int> s9 = s3;
				mystl::stack<int> s10 = std::move(s3);

				cout << " After s1 push 1,2,3,4,5 :" << endl;
				s1.push(1);
				s1.push(2);
				s1.push(3);
				s1.push(4);
				s1.push(5);
				STACK_COUT(s1);
				cout << boolalpha;
				FUN_VALUE(s1.empty());
				cout << noboolalpha;
				FUN_VALUE(s1.size());
				FUN_VALUE(s1.top());
				while (!s1.empty()) {
					cout << " After s1.pop():" << endl;
					s1.pop();
					STACK_COUT(s1);
				}
				cout << " After s1.swap(s4) :" << endl;
				s1.swap(s4);
				STACK_COUT(s1);
				cout << " After s1.clear() :" << endl;
				s1.clear();
				STACK_COUT(s1);
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|     push      |   1000000   |  10000000   |  100000000  |" << endl;
				cout << "|      std      |";
				STACK_PUSH_TEST(std, 1000000);
				STACK_PUSH_TEST(std, 10000000);
				STACK_PUSH_TEST(std, 100000000);
				cout << endl << "|     mystl     |";
				STACK_PUSH_TEST(mystl, 1000000);
				STACK_PUSH_TEST(mystl, 10000000);
				STACK_PUSH_TEST(mystl, 100000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[-------------- End container test : stack ---------------]" << endl;
			}
		}
	}
}
#endif // !MYTINYSTL_STACK_TEST_H

