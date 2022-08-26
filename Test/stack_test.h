#ifndef MYTINYSTL_STACK_TEST_H_
#define MYTINYSTL_STACK_TEST_H_

// stack test : 测试 stack 的接口 和 push 的性能

#include <stack>

#include "../MyTinySTL/stack.h"
#include "test.h"

namespace mystl
{
namespace test
{
namespace stack_test
{

void stack_print(mystl::stack<int> s)
{
  while (!s.empty())
  {
    std::cout << " " << s.top();
    s.pop();
  }
  std::cout << std::endl;
}

// stack 的遍历输出
#define STACK_COUT(s) do {                       \
    std::string s_name = #s;                     \
    std::cout << " " << s_name << " :";          \
    stack_print(s);                              \
} while(0)

#define STACK_FUN_AFTER(con, fun) do {           \
  std::string fun_name = #fun;                   \
  std::cout << " After " << fun_name << " :\n";  \
  fun;                                           \
  STACK_COUT(con);                               \
} while(0)

void stack_test()
{
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
  mystl::stack<int> s9;
  s9 = s3;
  mystl::stack<int> s10;
  s10 = std::move(s3);
  mystl::stack<int> s11{ 1,2,3,4,5 };
  mystl::stack<int> s12;
  s12 = { 1,2,3,4,5 };

  STACK_FUN_AFTER(s1, s1.push(1));
  STACK_FUN_AFTER(s1, s1.push(2));
  STACK_FUN_AFTER(s1, s1.push(3));
  STACK_FUN_AFTER(s1, s1.pop());
  STACK_FUN_AFTER(s1, s1.emplace(4));
  STACK_FUN_AFTER(s1, s1.emplace(5));
  std::cout << std::boolalpha;
  FUN_VALUE(s1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(s1.size());
  FUN_VALUE(s1.top());
  while (!s1.empty())
  {
    STACK_FUN_AFTER(s1, s1.pop());
  }
  STACK_FUN_AFTER(s1, s1.swap(s4));
  STACK_FUN_AFTER(s1, s1.clear());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|         push        |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(stack<int>, push, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
  CON_TEST_P1(stack<int>, push, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[----------------- End container test : stack ------------------]" << std::endl;
}

} // namespace stack_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_STACK_TEST_H_

