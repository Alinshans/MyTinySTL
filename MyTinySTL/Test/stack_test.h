#ifndef MYTINYSTL_STACK_TEST_H_
#define MYTINYSTL_STACK_TEST_H_

// stack test : 测试 stack 的接口 和 push 的性能

#include <stack>

#include "../stack.h"
#include "test.h"

namespace mystl {
namespace test {
namespace stack_test {

// stack 的遍历输出
#define STACK_COUT(s) do {              \
    std::string s_name = #s;             \
    std::cout << " " << s_name << " :";  \
    stack_print(s);                      \
} while(0)

void stack_print(mystl::stack<int> s) {
    while (!s.empty()) {
        std::cout << " " << s.top();
        s.pop();
    }
    std::cout << std::endl;
}

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
    CON_TEST_P1(stack<int>, push, rand(), LEN2, LEN3, LEN4);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[----------------- End container test : stack ------------------]" << std::endl;
}

} // namespace stack_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_STACK_TEST_H_

