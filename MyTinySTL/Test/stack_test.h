#ifndef MYTINYSTL_STACK_TEST_H_
#define MYTINYSTL_STACK_TEST_H_

// stack test : 测试 stack 的 API 和 push 的性能

#include <stack>

#include "../stack.h"
#include "test.h"

namespace mystl {
    namespace test {
        namespace stack_test {

            // 遍历输出 stack 的宏定义
            #define STACK_COUT(s) do { \
                std::string s_name = #s; \
                std::cout << " " << s_name << " :"; \
                stack_print(s); \
            } while(0)

            void stack_print(mystl::stack<int> s) {
                while (!s.empty()) {
                    std::cout << " " << s.top();
                    s.pop();
                }
                std::cout << std::endl;
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
                std::string t = buf; \
                t += "ms    |"; \
                std::cout << std::setw(WIDE) << t; \
            } while(0)

            void stack_test() {
                std::cout << "[===============================================================]" << std::endl;
                std::cout << "[----------------- Run container test : stack ------------------]" << std::endl;
                std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
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

                std::cout << " After s1 push 1,2,3,4,5 :" << std::endl;
                s1.push(1);
                s1.push(2);
                s1.push(3);
                s1.push(4);
                s1.push(5);
                STACK_COUT(s1);
                std::cout << std::boolalpha;
                FUN_VALUE(s1.empty());
                std::cout << std::noboolalpha;
                FUN_VALUE(s1.size());
                FUN_VALUE(s1.top());
                while (!s1.empty()) {
                    std::cout << " After s1.pop():" << std::endl;
                    s1.pop();
                    STACK_COUT(s1);
                }
                std::cout << " After s1.swap(s4) :" << std::endl;
                s1.swap(s4);
                STACK_COUT(s1);
                std::cout << " After s1.clear() :" << std::endl;
                s1.clear();
                STACK_COUT(s1);
                PASSED;
                std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
                std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
                std::cout << "|         push        |";
                TEST_LEN(LEN2, LEN3, LEN4, WIDE);
                std::cout << "|         std         |";
                STACK_PUSH_TEST(std, LEN2);
                STACK_PUSH_TEST(std, LEN3);
                STACK_PUSH_TEST(std, LEN4);
                std::cout << std::endl << "|        mystl        |";
                STACK_PUSH_TEST(mystl, LEN2);
                STACK_PUSH_TEST(mystl, LEN3);
                STACK_PUSH_TEST(mystl, LEN4);
                std::cout << std::endl;
                std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
                PASSED;
                std::cout << "[----------------- End container test : stack ------------------]" << std::endl;
            }
        }
    }
}
#endif // !MYTINYSTL_STACK_TEST_H_

