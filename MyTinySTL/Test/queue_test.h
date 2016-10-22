#ifndef MYTINYSTL_QUEUE_TEST_H
#define MYTINYSTL_QUEUE_TEST_H

// queue 测试: 测试 queue 与 priority_queue 的 API 和它们 push 的性能

#include <iomanip>
#include <iostream>
#include <queue>

#include "..\deque.h"
#include "..\queue.h"
#include "..\vector.h"
#include "test.h"

using namespace std;

namespace mystl {
	namespace test {
		namespace queue_test {

			// 遍历输出 queue 的宏定义
			#define QUEUE_COUT(q) do { \
				string q_name = #q; \
				cout << " " << q_name << " :"; \
				queue_print(q); \
			} while(0)

			// 遍历输出 priority_queue 的宏定义
			#define P_QUEUE_COUT(p) do { \
				string p_name = #p; \
				cout << " " << p_name << " :"; \
				p_queue_print(p); \
			} while(0)

			void queue_print(mystl::queue<int> q) {
				while (!q.empty()) {
					cout << " " << q.front();
					q.pop();
				}
				cout << endl;
			}

			void p_queue_print(mystl::priority_queue<int> p) {
				while (!p.empty()) {
					cout << " " << p.top();
					p.pop();
				}
				cout << endl;
			}

			// queue 与 priority_queue push 的性能测试
			#define QUEUE_PUSH_TEST(mode, con, count) do { \
				srand((int)time(0)); \
				clock_t start, end; \
				mode::con<int> q; \
				char buf[10]; \
				start = clock(); \
				for (size_t i = 0; i < count; ++i) \
					q.push(rand()); \
				end = clock(); \
				int n = end - start; \
				sprintf_s(buf, "%d", n); \
				string t = buf; \
				t += "ms    |"; \
				cout << setw(14) << t; \
			} while(0)

			void queue_test() {
				cout << "[=========================================================]" << endl;
				cout << "[-------------- Run container test : queue ---------------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
				int a[] = { 1,2,3,4,5 };
				mystl::deque<int> d1(5);
				mystl::queue<int> q1;
				mystl::queue<int> q2(5);
				mystl::queue<int> q3(5, 1);
				mystl::queue<int> q4(a, a + 5);
				mystl::queue<int> q5(d1);
				mystl::queue<int> q6(std::move(d1));
				mystl::queue<int> q7(q2);
				mystl::queue<int> q8(std::move(q2));
				mystl::queue<int> q9 = q3;
				mystl::queue<int> q10 = std::move(q3);

				cout << " After q1 push 1,2,3,4,5 :" << endl;
				q1.push(1);
				q1.push(2);
				q1.push(3);
				q1.push(4);
				q1.push(5);
				QUEUE_COUT(q1);
				cout << boolalpha;
				FUN_VALUE(q1.empty());
				cout << noboolalpha;
				FUN_VALUE(q1.size());
				FUN_VALUE(q1.front());
				FUN_VALUE(q1.back());
				while (!q1.empty()) {
					cout << " After q1.pop():" << endl;
					q1.pop();
					QUEUE_COUT(q1);
				}
				cout << " After q1.swap(q4) :" << endl;
				q1.swap(q4);
				QUEUE_COUT(q1);
				cout << " After q1.clear() :" << endl;
				q1.clear();
				QUEUE_COUT(q1);
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|     push      |   1000000   |  10000000   |  100000000  |" << endl;
				cout << "|      std      |";
				QUEUE_PUSH_TEST(std, queue, 1000000);
				QUEUE_PUSH_TEST(std, queue, 10000000);
				QUEUE_PUSH_TEST(std, queue, 100000000);
				cout << endl << "|     mystl     |";
				QUEUE_PUSH_TEST(mystl, queue, 1000000);
				QUEUE_PUSH_TEST(mystl, queue, 10000000);
				QUEUE_PUSH_TEST(mystl, queue, 100000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[-------------- End container test : queue ---------------]" << endl;
			}

			void priority_test() {
				cout << "[=========================================================]" << endl;
				cout << "[---------- Run container test : priority_queue ----------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
				int a[] = { 1,2,3,4,5 };
				mystl::vector<int> v1(5);
				mystl::priority_queue<int> p1;
				mystl::priority_queue<int> p2(5);
				mystl::priority_queue<int> p3(5, 1);
				mystl::priority_queue<int> p4(a, a + 5);
				mystl::priority_queue<int> p5(v1);
				mystl::priority_queue<int> p6(std::move(v1));
				mystl::priority_queue<int> p7(p2);
				mystl::priority_queue<int> p8(std::move(p2));
				mystl::priority_queue<int> p9 = p3;
				mystl::priority_queue<int> p10 = std::move(p3);

				cout << " After p1 push 1,2,3,4,5 :" << endl;
				p1.push(1);
				p1.push(2);
				p1.push(3);
				p1.push(4);
				p1.push(5);
				P_QUEUE_COUT(p1);
				cout << boolalpha;
				FUN_VALUE(p1.empty());
				cout << noboolalpha;
				FUN_VALUE(p1.size());
				FUN_VALUE(p1.top());
				while (!p1.empty()) {
					cout << " After p1.pop():" << endl;
					p1.pop();
					P_QUEUE_COUT(p1);
				}
				cout << " After p1.swap(p4) :" << endl;
				p1.swap(p4);
				P_QUEUE_COUT(p1);
				cout << " After p1.clear() :" << endl;
				p1.clear();
				P_QUEUE_COUT(p1);
				PASSED;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				cout << "|     push      |   1000000   |  10000000   |  100000000  |" << endl;
				cout << "|      std      |";
				QUEUE_PUSH_TEST(std, priority_queue, 1000000);
				QUEUE_PUSH_TEST(std, priority_queue, 10000000);
				QUEUE_PUSH_TEST(std, priority_queue, 100000000);
				cout << endl << "|     mystl     |";
				QUEUE_PUSH_TEST(mystl, priority_queue, 1000000);
				QUEUE_PUSH_TEST(mystl, priority_queue, 10000000);
				QUEUE_PUSH_TEST(mystl, priority_queue, 100000000);
				cout << endl;
				cout << "|---------------|-------------|-------------|-------------|" << endl;
				PASSED;
				cout << "[---------- Run container test : priority_queue ----------]" << endl;
			}
		}
	}
}
#endif // !MYTINYSTL_QUEUE_TEST_H

