#ifndef MYTINYSTL_LIST_TEST_H_
#define MYTINYSTL_LIST_TEST_H_

// list test : 测试 list 的接口与 insert, sort 的性能

#include <list>

#include "../MyTinySTL/list.h"
#include "test.h"

namespace mystl
{
namespace test
{
namespace list_test
{

// 一个辅助测试函数
bool is_odd(int x) { return x & 1; }

void list_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------------ Run container test : list ------------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 1,2,3,4,5 };
  mystl::list<int> l1;
  mystl::list<int> l2(5);
  mystl::list<int> l3(5, 1);
  mystl::list<int> l4(a, a + 5);
  mystl::list<int> l5(l2);
  mystl::list<int> l6(std::move(l2));
  mystl::list<int> l7{ 1,2,3,4,5,6,7,8,9 };
  mystl::list<int> l8;
  l8 = l3;
  mystl::list<int> l9;
  l9 = std::move(l3);
  mystl::list<int> l10;
  l10 = { 1, 2, 2, 3, 5, 6, 7, 8, 9 };

  FUN_AFTER(l1, l1.assign(8, 8));
  FUN_AFTER(l1, l1.assign(a, a + 5));
  FUN_AFTER(l1, l1.assign({ 1,2,3,4,5,6 }));
  FUN_AFTER(l1, l1.insert(l1.end(), 6));
  FUN_AFTER(l1, l1.insert(l1.end(), 2, 7));
  FUN_AFTER(l1, l1.insert(l1.begin(), a, a + 5));
  FUN_AFTER(l1, l1.push_back(2));
  FUN_AFTER(l1, l1.push_front(1));
  FUN_AFTER(l1, l1.emplace(l1.begin(),1));
  FUN_AFTER(l1, l1.emplace_front(0));
  FUN_AFTER(l1, l1.emplace_back(10));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.pop_front());
  FUN_AFTER(l1, l1.pop_back());
  FUN_AFTER(l1, l1.erase(l1.begin()));
  FUN_AFTER(l1, l1.erase(l1.begin(), l1.end()));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.resize(10));
  FUN_AFTER(l1, l1.resize(5, 1));
  FUN_AFTER(l1, l1.resize(8, 2));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.splice(l1.end(), l4));
  FUN_AFTER(l1, l1.splice(l1.begin(), l5, l5.begin()));
  FUN_AFTER(l1, l1.splice(l1.end(), l6, l6.begin(), ++l6.begin()));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.remove(0));
  FUN_AFTER(l1, l1.remove_if(is_odd));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.assign({ 9,5,3,3,7,1,3,2,2,0,10 }));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.sort());
  FUN_AFTER(l1, l1.unique());
  FUN_AFTER(l1, l1.unique([&](int a, int b) {return b == a + 1; }));
  FUN_AFTER(l1, l1.merge(l7));
  FUN_AFTER(l1, l1.sort(mystl::greater<int>()));
  FUN_AFTER(l1, l1.merge(l8, mystl::greater<int>()));
  FUN_AFTER(l1, l1.reverse());
  FUN_AFTER(l1, l1.clear());
  FUN_AFTER(l1, l1.swap(l9));
  FUN_VALUE(*l1.begin());
  FUN_VALUE(*l1.rbegin());
  FUN_VALUE(l1.front());
  FUN_VALUE(l1.back());
  std::cout << std::boolalpha;
  FUN_VALUE(l1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(l1.size());
  FUN_VALUE(l1.max_size());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P2(list<int>, insert, end, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#else
  CON_TEST_P2(list<int>, insert, end, rand(), SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|         sort        |";
#if LARGER_TEST_DATA_ON
  LIST_SORT_TEST(SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#else
  LIST_SORT_TEST(SCALE_S(LEN1), SCALE_S(LEN2), SCALE_S(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[------------------ End container test : list ------------------]" << std::endl;
}

} // namespace list_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_LIST_TEST_H_

