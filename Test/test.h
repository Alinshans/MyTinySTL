#ifndef MYTINYSTL_TEST_H_
#define MYTINYSTL_TEST_H_

// 一个简单的单元测试框架，定义了两个类 TestCase 和 UnitTest，以及一系列用于测试的宏

#include <ctime>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#include "Lib/redbud/io/color.h"

namespace mystl
{
namespace test
{

#define green redbud::io::state::manual << redbud::io::hfg::green
#define red   redbud::io::state::manual << redbud::io::hfg::red

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#endif

} // namespace test

namespace test
{

// TestCase 类
// 封装单个测试案例
class TestCase
{
public:
  // 构造函数，接受一个字符串代表案例名称
  TestCase(const char* case_name) : testcase_name(case_name) {}

  // 一个纯虚函数，用于测试案例
  virtual void Run() = 0;

public:
  const char* testcase_name;  // 测试案例的名称
  int         nTestResult;    // 测试案例的执行结果 
  double      nFailed;        // 测试失败的案例数
  double      nPassed;        // 测试通过的案例数
};

// UnitTest 类
// 单元测试，把所有测试案例加入到 vector 中，依次执行测试案例
class UnitTest
{
public:
  // 获取一个案例
  static UnitTest* GetInstance();

  // 将案例依次加入 vector
  TestCase* RegisterTestCase(TestCase* testcase);

  void Run();

public:
  TestCase* CurrentTestCase;          // 当前执行的测试案例
  double    nPassed;                  // 通过案例数
  double    nFailed;                  // 失败案例数

protected:
  std::vector<TestCase*> testcases_;  // 保存案例集合
};

UnitTest* UnitTest::GetInstance()
{
  static UnitTest instance;
  return &instance;
}

TestCase* UnitTest::RegisterTestCase(TestCase* testcase)
{
  testcases_.push_back(testcase);
  return testcase;
}

void UnitTest::Run()
{
  for (auto it : testcases_)
  {
    TestCase* testcase = it;
    CurrentTestCase = testcase;
    testcase->nTestResult = 1;
    testcase->nFailed = 0;
    testcase->nPassed = 0;
    std::cout << green << "============================================\n";
    std::cout << green << " Run TestCase:" << testcase->testcase_name << "\n";
    testcase->Run();
    if (testcase->nFailed == 0)
      std::cout << green;
    else
      std::cout << red;
    std::cout << " " << testcase->nPassed << " / " << testcase->nFailed + testcase->nPassed
      << " Cases passed. ( " << testcase->nPassed / 
      (testcase->nFailed + testcase->nPassed) * 100 << "% )\n";
    std::cout << green << " End TestCase:" << testcase->testcase_name << "\n";
    if (testcase->nTestResult)
      ++nPassed;
    else
      ++nFailed;
  }
  std::cout << green << "============================================\n";
  std::cout << green << " Total TestCase : " << nPassed + nFailed << "\n";
  std::cout << green << " Total Passed : " << nPassed << "\n";
  std::cout << red << " Total Failed : " << nFailed << "\n";
  std::cout << green << " " << nPassed << " / " << nFailed + nPassed
    << " TestCases passed. ( " << nPassed / (nFailed + nPassed) * 100 << "% )\n";
}

/*****************************************************************************************/

// 测试案例的类名，替换为 test_cast_TEST
#define TESTCASE_NAME(testcase_name) \
    testcase_name##_TEST

// 使用宏定义掩盖复杂的测试样例封装过程，把 TEXT 中的测试案例放到单元测试中
#define MYTINYSTL_TEST_(testcase_name)                        \
class TESTCASE_NAME(testcase_name) : public TestCase {        \
public:                                                       \
    TESTCASE_NAME(testcase_name)(const char* case_name)       \
        : TestCase(case_name) {};                             \
    virtual void Run();                                       \
private:                                                      \
    static TestCase* const testcase_;                         \
};                                                            \
                                                              \
TestCase* const TESTCASE_NAME(testcase_name)                  \
    ::testcase_ = UnitTest::GetInstance()->RegisterTestCase(  \
        new TESTCASE_NAME(testcase_name)(#testcase_name));    \
void TESTCASE_NAME(testcase_name)::Run()

/*
Run()后边没有写实现，是为了用宏定义将测试用例放入到 Run 的实现里，例如：
TEST(AddTestDemo)
{
EXPECT_EQ(3, Add(1, 2));
EXPECT_EQ(2, Add(1, 1));
}
上述代码将 { EXPECT_EQ(3, Add(1, 2)); EXPECT_EQ(2, Add(1, 1)); } 接到 Run() 的后面
*/


/*****************************************************************************************/

// 简单测试的宏定义
// 断言 : 宏定义形式为 EXPECT_* ，符合验证条件的，案例测试通过，否则失败
// 使用一系列的宏来封装验证条件，分为以下几大类 :

/*
真假断言
EXPECT_TRUE  验证条件: Condition 为 true
EXPECT_FALSE 验证条件: Condition 为 false

Example:
bool isPrime(int n);         一个判断素数的函数
EXPECT_TRUE(isPrime(2));     通过
EXPECT_FALSE(isPrime(4));    通过
EXPECT_TRUE(isPrime(6));     失败
EXPECT_FALSE(isPrime(3));    失败
*/
#define EXPECT_TRUE(Condition) do {                             \
  if (Condition) {                                              \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_TRUE succeeded!\n";          \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_TRUE failed!\n";               \
}} while(0)

#define EXPECT_FALSE(Condition) do {                            \
  if (!Condition) {                                             \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_FALSE succeeded!\n";         \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << "  EXPECT_FALSE failed!\n";             \
}} while(0)

/*
比较断言
EXPECT_EQ(v1, v2) 验证条件: v1 == v2
EXPECT_NE(v1, v2) 验证条件: v1 != v2
EXPECT_LT(v1, v2) 验证条件: v1 <  v2
EXPECT_LE(v1, v2) 验证条件: v1 <= v2
EXPECT_GT(v1, v2) 验证条件: v1 >  v2
EXPECT_GE(v1, v2) 验证条件: v1 >= v2

Note:
1. 参数应满足 EXPECT_*(Expect, Actual)的格式，左边是期望值，右边是实际值
2. 在断言失败时，会将期望值与实际值打印出来
3. 参数值必须是可通过断言的比较操作符进行比较的，参数值还必须支持 << 操作符来
将值输入到 ostream 中
4. 这些断言可以用于用户自定义型别，但必须重载相应的比较操作符（如 == 、< 等）
5. EXPECT_EQ 对指针进行的是地址比较。即比较的是它们是否指向相同的内存地址，
而不是它们指向的内容是否相等。如果想比较两个 C 字符串(const char*)的值，
请使用 EXPECT_STREQ 。特别一提的是，要验证一个 C 字符串是否为空(NULL)，
请使用 EXPECT_STREQ(NULL, c_str)。但是要比较两个 string 对象时，
应该使用 EXPECT_EQ

Example:
EXPECT_EQ(3, foo());
EXPECT_NE(NULL, pointer);
EXPECT_LT(len, v.size());
*/
#define EXPECT_EQ(v1, v2) do { \
  if (v1 == v2) {                                               \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_EQ succeeded!\n";            \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_EQ failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

#define EXPECT_NE(v1, v2) do {                                  \
  if (v1 != v2) {                                               \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
   std::cout << green << " EXPECT_NE succeeded!\n";             \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_NE failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

#define EXPECT_LT(v1, v2) do {                                  \
  if (v1 < v2) {                                                \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_LT succeeded!\n";            \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_LT failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

#define EXPECT_LE(v1, v2) do {                                  \
  if (v1 <= v2) {                                               \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_LE succeeded!\n";            \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_LE failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

#define EXPECT_GT(v1, v2) do {                                  \
  if (v1 > v2) {                                                \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_GT succeeded!\n";            \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_GT failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

#define EXPECT_GE(v1, v2) do {                                  \
  if (v1 >= v2) {                                               \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_GE succeeded!\n";            \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_GE failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

/*
字符串比较
EXPECT_STREQ(s1, s2) 验证条件: 两个 C 字符串有相同的值
EXPECT_STRNE(s1, s2) 验证条件: 两个 C 字符串有不同的值

Note:
1. 参数应满足 EXPECT_STR*(Expect, Actual)的格式，左边是期望值，右边是实际值
2. 该组断言用于比较两个 C 字符串。如果你想要比较两个 string 对象，相应地使用
EXPECT_EQ、EXPECT_NE 等断言
3. EXPECT_STREQ 和 EXPECT_STRNE 不接受宽字符串（wchar_t*）
4. 一个 NULL 指针和一个空字符串会不是一样的

Example:
char* s1 = "", char* s2 = "abc", char* s3 = NULL;
EXPECT_STREQ("abc", s2);  通过
EXPECT_STREQ(s1, s3);     失败
EXPECT_STREQ(NULL, s3);   通过
EXPECT_STRNE(" ", s1);    通过
*/

#define EXPECT_STREQ(s1, s2) do {                                 \
  if (s1 == NULL || s2 == NULL) {                                 \
    if (s1 == NULL && s2 == NULL) {                               \
      UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
      std::cout << green << " EXPECT_STRED succeeded!\n";         \
    }                                                             \
    else {                                                        \
      UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
      UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
      std::cout << red << " EXPECT_STRED failed!\n";              \
      if(s1 == NULL) std::cout << " Expect: NULL\n";              \
      else std::cout << " Expect:\"" << s1 << "\"\n";             \
      if(s2 == NULL) std::cout << " Actual: NULL\n";              \
      else std::cout << " Actual:\"" << s2 << "\"\n";             \
    }                                                             \
  }                                                               \
  else if (strcmp(s1, s2) == 0) {                                 \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;          \
    std::cout << green << " EXPECT_STRED succeeded!\n";           \
  }                                                               \
  else {                                                          \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;    \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;          \
    std::cout << red << " EXPECT_STRED failed!\n";                \
    std::cout << red << " Expect:\"" << s1 << "\"\n";             \
    std::cout << red << " Actual:\"" << s2 << "\"\n";             \
}} while(0)

#define EXPECT_STRNE(s1, s2) do {                                 \
  if (s1 == NULL || s2 == NULL) {                                 \
    if (s1 != NULL || s2 != NULL) {                               \
      UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
      std::cout << green << " EXPECT_STRNE succeeded!\n";         \
    }                                                             \
    else {                                                        \
      UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
      UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
      std::cout << red << " EXPECT_STRNE failed!\n";              \
      if(s1 == NULL) std::cout << " Expect: NULL\n";              \
      else std::cout << " Expect:\"" << s1 << "\"\n";             \
      if(s2 == NULL) std::cout << " Actual: NULL\n";              \
      else std::cout << " Actual:\"" << s2 << "\"\n";             \
    }                                                             \
  }                                                               \
  else if (strcmp(s1, s2) != 0) {                                 \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;          \
    std::cout << green << " EXPECT_STRNE succeeded!\n";           \
  }                                                               \
  else {                                                          \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;    \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;          \
    std::cout << red << " EXPECT_STRNE failed!\n";                \
    std::cout << red << " Expect:\"" << s1 << "\"\n";             \
    std::cout << red << " Actual:\"" << s2 << "\"\n";             \
}} while(0)

/*
指针比较
EXPECT_PTR_EQ(p1, p2)            验证条件: *p1 == *p2
EXPECT_PTR_NE(p1, p2)            验证条件: *p1 != *p2
EXPECT_PTR_RANGE_EQ(p1, p2, len) 验证条件: 任意 i (*p1 + i) == (*p2 + i)  i∈[0,len)
EXPECT_PTR_RANGE_NE(p1, p2, len) 验证条件: 存在 i (*p1 + i) != (*p2 + i)  i∈[0,len)

Note:
1. 参数应满足 EXPECT_PTR_*(Expect, Actual)、
EXPECT_PTR_RANGE_*(Expect, Actual, len)的格式，
即参数表中期望值在实际值左边
2. EXPECT_PTR_EQ 比较的是指针所指元素的值，如果要比较
指针指向的地址是否相等，请用 EXPECT_EQ
3. EXPECT_PTR_RANGE_* 比较的是从 p1，p2 开始，
长度为 len 的区间，请确保区间长度有效

Example:
int a[] = {1,2,3,4,5};
int b[] = {1,2,3,4,6};
int *p1 = a, *p2 = b;
EXPECT_PTR_EQ(p1, p2);                      通过
p1 = a + 4, p2 = b + 4;
EXPECT_PTR_EQ(p1, p2);                      失败
EXPECT_PTR_EQ(p1, std::find(a, a + 5, 5));  通过
EXPECT_PTR_RANGE_EQ(a, b, 5);               失败
EXPECT_PTR_RANGE_EQ(a, b, 4);               通过
*/
#define EXPECT_PTR_EQ(p1, p2) do {                              \
  if (*p1 == *p2) {                                             \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_PTR_EQ succeeded!\n";        \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_PTR_EQ failed!\n";             \
    std::cout << red << " Expect:" << *p1 << "\n";              \
    std::cout << red << " Actual:" << *p2 << "\n";              \
}} while(0)

#define EXPECT_PTR_NE(p1, p2) do {                              \
  if (*p1 != *p2) {                                             \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_PTR_NE succeeded!\n";        \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_PTR_NE failed!\n";             \
    std::cout << red << " Expect:" << *p1 << "\n";              \
    std::cout << red << " Actual:" << *p2 << "\n";              \
}} while(0)

#define EXPECT_PTR_RANGE_EQ(p1, p2, len) do {                   \
  if (std::equal(p1, p1 + len, p2)) {                           \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_PTR_RANGE_EQ succeeded!\n";  \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_PTR_RANGE_EQ failed!\n";       \
}} while(0)

#define EXPECT_PTR_RANGE_NE(p1, p2, len) do {                   \
  if (!std::equal(p1, p1 + len, p2)) {                          \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_PTR_RANGE_NE succeeded!\n";  \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_PTR_RANGE_NE failed!\n";       \
}} while(0)

/*
容器比较
EXPECT_CON_EQ(c1, c2) 验证条件: c1 == c2
EXPECT_CON_NE(c1, c2) 验证条件: c1 != c2

Note:
1. 容器可以是 STL 容器，自定义的容器，或者数组，但不可以是指针
2. 容器的数据类型要能够进行比较，类型一致或可以发生隐式转换
3. EXPECT_CON_EQ 测试失败时，会打印首次不相等的两个值

Example:
int arr[] = {1,2,3};
std::vector<int> v1{1, 2, 3};
std::vector<int> v2{2, 3, 4};
mystl::vector<long> v3(arr, arr + 3);
EXPECT_CON_NE(v1, v2)   ok
EXPECT_CON_EQ(arr, v1)  ok
EXPECT_CON_EQ(v1, v3)   ok
*/
#define EXPECT_CON_EQ(c1, c2) do {                                  \
  auto first1 = std::begin(c1), last1 = std::end(c1);               \
  auto first2 = std::begin(c2), last2 = std::end(c2);               \
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {  \
    if (*first1 != *first2)  break;                                 \
  }                                                                 \
  if (first1 == last1 && first2 == last2) {                         \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;            \
    std::cout << green << " EXPECT_CON_EQ succeeded!\n";            \
  }                                                                 \
  else {                                                            \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;      \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;            \
    std::cout << red << " EXPECT_CON_EQ failed!\n";                 \
    std::cout << red << " Expect:" << *first1 << "\n";              \
    std::cout << red << " Actual:" << *first2 << "\n";              \
}} while(0)

#define EXPECT_CON_NE(c1, c2) do {                                  \
  auto first1 = std::begin(c1), last1 = std::end(c1);               \
  auto first2 = std::begin(c2), last2 = std::end(c2);               \
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {  \
    if (*first1 != *first2)  break;                                 \
  }                                                                 \
  if (first1 != last1 || first2 != last2) {                         \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;            \
    std::cout << green << " EXPECT_CON_NE succeeded!\n";            \
  }                                                                 \
  else {                                                            \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;      \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;            \
    std::cout << red << " EXPECT_CON_NE failed!\n";                 \
}} while(0)

/*****************************************************************************************/
// 常用的宏定义

// 不同情况的测试数量级
#if defined(_DEBUG) || defined(DEBUG)
#define LEN1    10000
#define LEN2    100000
#define LEN3    1000000
#else
#define LEN1    100000
#define LEN2    1000000
#define LEN3    10000000
#endif

#define _LLL * 20
#define _LL  * 10
#define _L   * 5
#define _M
#define _S   / 5
#define _SS  / 10
#define _SSS / 20

#define WIDE    14

// 输出通过提示
#define PASSED    std::cout << "[ PASSED ]\n"

// 遍历输出容器
#define COUT(container) do {                             \
  std::string con_name = #container;                     \
  std::cout << " " << con_name << " :";                  \
  for (auto it : container)                              \
    std::cout << " " << it;                              \
  std::cout << "\n";                                     \
} while(0)

#define STR_COUT(str) do {                               \
  std::string str_name = #str;                           \
  std::cout << " " << str_name << " : " << str << "\n";  \
} while(0)

// 输出容器调用函数后的结果
#define FUN_AFTER(con, fun) do {                         \
  std::string fun_name = #fun;                           \
  std::cout << " After " << fun_name << " :\n";          \
  fun;                                                   \
  COUT(con);                                             \
} while(0)

#define STR_FUN_AFTER(str, fun) do {                     \
  std::string fun_name = #fun;                           \
  std::cout << " After " << fun_name << " :\n";          \
  fun;                                                   \
  STR_COUT(str);                                         \
} while(0)

// 输出容器调用函数的值
#define FUN_VALUE(fun) do {                              \
  std::string fun_name = #fun;                           \
  std::cout << " " << fun_name << " : " << fun << "\n";  \
} while(0)

// 输出测试数量级
void test_len(size_t len1, size_t len2, size_t len3, size_t wide)
{
  std::string str1, str2, str3;
  std::stringstream ss;
  ss << len1 << " " << len2 << " " << len3;
  ss >> str1 >> str2 >> str3;
  str1 += "   |";
  std::cout << std::setw(wide) << str1;
  str2 += "   |";
  std::cout << std::setw(wide) << str2;
  str3 += "   |";
  std::cout << std::setw(wide) << str3 << "\n";
}

#define TEST_LEN(len1, len2, len3, wide) \
  test_len(len1, len2, len3, wide)

// 常用测试性能的宏
#define FUN_TEST_FORMAT1(mode, fun, arg, count) do {         \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode c;                                                    \
  char buf[10];                                              \
  start = clock();                                           \
  for (size_t i = 0; i < count; ++i)                         \
    c.fun(arg);                                              \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                         \
} while(0)

#define FUN_TEST_FORMAT2(mode, fun, arg1, arg2, count) do {  \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode c;                                                    \
  char buf[10];                                              \
  start = clock();                                           \
  for (size_t i = 0; i < count; ++i)                         \
    c.fun(c.arg1(), arg2);                                   \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                         \
} while(0)

#define LIST_SORT_DO_TEST(mode, count) do {                  \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode::list<int> l;                                         \
  char buf[10];                                              \
  for (size_t i = 0; i < count; ++i)                         \
    l.insert(l.end(), rand());                               \
  start = clock();                                           \
  l.sort();                                                  \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                         \
} while(0)

#define MAP_EMPLACE_DO_TEST(mode, con, count) do {           \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode::con<int, int> c;                                     \
  char buf[10];                                              \
  start = clock();                                           \
  for (size_t i = 0; i < count; ++i)                         \
    c.emplace(mode::make_pair(rand(), rand()));              \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                         \
} while(0)

// 重构重复代码
#define CON_TEST_P1(con, fun, arg, len1, len2, len3)         \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  FUN_TEST_FORMAT1(std::con, fun, arg, len1);                \
  FUN_TEST_FORMAT1(std::con, fun, arg, len2);                \
  FUN_TEST_FORMAT1(std::con, fun, arg, len3);                \
  std::cout << "\n|        mystl        |";                  \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len1);              \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len2);              \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len3);    

#define CON_TEST_P2(con, fun, arg1, arg2, len1, len2, len3)  \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len1);         \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len2);         \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len3);         \
  std::cout << "\n|        mystl        |";                  \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len1);       \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len2);       \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len3);    

#define MAP_EMPLACE_TEST(con, len1, len2, len3)              \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  MAP_EMPLACE_DO_TEST(std, con, len1);                       \
  MAP_EMPLACE_DO_TEST(std, con, len2);                       \
  MAP_EMPLACE_DO_TEST(std, con, len3);                       \
  std::cout << "\n|        mystl        |";                  \
  MAP_EMPLACE_DO_TEST(mystl, con, len1);                     \
  MAP_EMPLACE_DO_TEST(mystl, con, len2);                     \
  MAP_EMPLACE_DO_TEST(mystl, con, len3);

#define LIST_SORT_TEST(len1, len2, len3)                     \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  LIST_SORT_DO_TEST(std, len1);                              \
  LIST_SORT_DO_TEST(std, len2);                              \
  LIST_SORT_DO_TEST(std, len3);                              \
  std::cout << "\n|        mystl        |";                  \
  LIST_SORT_DO_TEST(mystl, len1);                            \
  LIST_SORT_DO_TEST(mystl, len2);                            \
  LIST_SORT_DO_TEST(mystl, len3);

// 简单测试的宏定义
#define TEST(testcase_name) \
  MYTINYSTL_TEST_(testcase_name)

// 运行所有测试案例
#define RUN_ALL_TESTS() \
  mystl::test::UnitTest::GetInstance()->Run()

// 是否开启性能测试
#ifndef PERFORMANCE_TEST_ON
#define PERFORMANCE_TEST_ON 1
#endif // !PERFORMANCE_TEST_ON

// 是否开启大数据量测试
#ifndef LARGER_TEST_DATA_ON
#define LARGER_TEST_DATA_ON 0
#endif // !LARGER_TEST_DATA_ON

}    // namespace test
}    // namespace mystl
#endif // !MYTINYSTL_TEST_H_

