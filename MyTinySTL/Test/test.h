#ifndef MYTINYSTL_TEST_H_
#define MYTINYSTL_TEST_H_

// 一个简单的单元测试框架，定义了两个类 TestCase 和 UnitTest，以及一系列用于测试的宏

#include <ctime>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace mystl {
namespace test {

// 改变输出流文本的颜色
inline std::ostream& red(std::ostream &os) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
    return os;
}
inline std::ostream& green(std::ostream &os) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    return os;
}

// TestCase 类
// 封装单个测试案例
// 包含了测试案例的名称，通过案例的数目和失败的数目，一个纯虚函数 Run()，代表执行测试案例的方法
class TestCase {
public :
    TestCase(const char* case_name) : testcase_name(case_name) {}

    virtual void Run() = 0;

    int nTestResult; //测试案例的执行结果 
    const char* testcase_name; //测试案例名称
    double nFailed;    //通过测试的 case
    double nPassed; //通过测试的 case
};

// UnitTest 类
// 单元测试，把所有测试案例加入到 vector 中，依次执行测试案例
class UnitTest {
public:
    static UnitTest* GetInstance();    //获取单例

    TestCase* RegisterTestCase(TestCase* testcase);    //注册测试案例
    
    void Run();    //执行单元测试

    TestCase* CurrentTestCase; //当前执行的测试案例
    double nPassed; //通过案例数
    double nFailed; //失败案例数

protected:
    std::vector<TestCase*> testcases_; // 案例集合
};

UnitTest* UnitTest::GetInstance() {
    static UnitTest instance;
    return &instance;
}

TestCase* UnitTest::RegisterTestCase(TestCase* testcase) {
    testcases_.push_back(testcase);
    return testcase;
}

void UnitTest::Run() {
    for (auto it = testcases_.begin(); it != testcases_.end(); ++it) {
        TestCase* testcase = *it;
        CurrentTestCase = testcase;
        testcase->nTestResult = 1;
        testcase->nFailed = testcase->nPassed = 0;
        std::cout << green <<  "============================================" << std::endl;
        std::cout << green << " Run TestCase:" << testcase->testcase_name << " " << std::endl;
        testcase->Run();
        if (testcase->nFailed == 0)    std::cout << green;
        else std::cout << red;
        std::cout << " " << testcase->nPassed << " / " << testcase->nFailed + testcase->nPassed
            << " Cases passed. ( " << testcase->nPassed / (testcase->nFailed + testcase->nPassed) * 100 << "% )" << std::endl;
        std::cout << green << " End TestCase:" << testcase->testcase_name << " " << std::endl;
        if (testcase->nTestResult)
            ++nPassed;
        else
            ++nFailed;
    }

    std::cout << green << "============================================" << std::endl;
    std::cout << green << " Total TestCase : " << nPassed + nFailed << std::endl;
    std::cout << green << " Total Passed : " << nPassed << std::endl;
    std::cout << red << " Total Failed : " << nFailed << std::endl;
    if (nFailed == 0)    std::cout << green;
    else
        std::cout << " " << nPassed << " / " << nFailed + nPassed
        << " TestCases passed. ( " << nPassed / (nFailed + nPassed) * 100 << "% )" << std::endl;
    std::cout << green;
}

// 用于测试案例的类名，替换为 test_cast_TEST
#define TESTCASE_NAME(testcase_name) \
    testcase_name##_TEST

// 宏定义，掩盖复杂的测试样例封装过程，把 TEXT 中的测试案例放到单元测试中
#define MYTINYSTL_TEST_(testcase_name) \
class TESTCASE_NAME(testcase_name) : public TestCase { \
public: \
    TESTCASE_NAME(testcase_name)(const char* case_name) : TestCase(case_name){}; \
    virtual void Run(); \
private: \
    static TestCase* const testcase_; \
}; \
\
TestCase* const TESTCASE_NAME(testcase_name) \
    ::testcase_ = UnitTest::GetInstance()->RegisterTestCase( \
        new TESTCASE_NAME(testcase_name)(#testcase_name)); \
void TESTCASE_NAME(testcase_name)::Run()

// Run()后边没有{}，之所以这么做是为了宏定义将测试用例放入到 Run 的方法主体里。例如：
// TEST(FooTestDemo)
// {
//    EXPECT_EQ(3, Foo(1, 2));
//    EXPECT_EQ(2, Foo(1, 1));
// }
// 上述代码将 EXPECT_EQ(3, Foo(1, 2)); EXPECT_EQ(2, Foo(1, 1)); 代码放入到Run的方法主体里



// 简单测试的宏定义
// 断言: MyTinySTL 全部使用非致命断言 EXPECT_* ，符合验证条件的，测试案例通过，否则失败
// 使用一系列的宏来封装验证条件，分为以下几大类：

// 真假断言
// EXPECT_TRUE 验证条件: Condition 为 true
// EXPECT_FALSE 验证条件: Condition 为 false
//
// Example:
// bool isPrime(int n);            一个判断素数的函数
// EXPECT_TRUE(isPrime(2));        通过
// EXPECT_FALSE(isPrime(4));    通过
// EXPECT_TRUE(isPrime(6));        失败
// EXPECT_FALSE(isPrime(3));    失败

#define EXPECT_TRUE(Condition) do { \
    if(Condition) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_TRUE succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_TRUE failed!" << std::endl; \
    }} while(0)
#define EXPECT_FALSE(Condition) do { \
    if(!Condition) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_FALSE succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << "  EXPECT_FALSE failed!" << std::endl; \
    }} while(0)

// 比较断言
// EXPECT_EQ(v1, v2) 验证条件: v1 == v2
// EXPECT_NE(v1, v2) 验证条件: v1 != v2
// EXPECT_LT(v1, v2) 验证条件: v1 < v2
// EXPECT_LE(v1, v2) 验证条件: v1 <= v2
// EXPECT_GT(v1, v2) 验证条件: v1 > v2
// EXPECT_GE(v1, v2) 验证条件: v1 >= v2
//
// Note:
// 1. 断言应满足 EXPECT_*(Expect, Actual)的格式，即左边是期望值，右边是实际值
// 2. 在断言失败时，会将期望值与实际值打印出来
// 3. 参数值必须是可通过断言的比较操作符进行比较的，参数值还必须支持 << 操作符来将值输入到 ostream 中
// 4. 这些断言可以用于用户自定义的型别，但必须重载相应的比较操作符（如 == 、< 等）
// 5. EXPECT_EQ()对指针进行的是指针比较。即，如果被用在两个C字符串(const char*)上，比较的是它们是否指向相同的内存地址，
//    而不是它们指向的内容是否相等。如果想比较两个C字符串(const char*)的值，请使用 EXPECT_STREQ 。特别一提的是，
//    要验证一个C字符串是否为空(NULL)，请使用 EXPECT_STREQ(NULL, c_string)。但是要比较两个 string 对象时，
//    应该使用 EXPECT_EQ()

// Example:
// EXPECT_EQ(3, foo());            
// EXPECT_NE(NULL, pointer);
// EXPECT_LT(len, v.size());

#define EXPECT_EQ(v1, v2) do { \
    if (v1 == v2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_EQ succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_EQ failed!" << std::endl; \
        std::cout << red << " Expect:" << v1 << std::endl; \
        std::cout << red << " Actual:" << v2 << std::endl; \
    }} while(0)
#define EXPECT_NE(v1, v2) do { \
    if (v1 != v2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_NE succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_NE failed!" << std::endl; \
        std::cout << red << " Expect:" << v1 << std::endl; \
        std::cout << red << " Actual:" << v2 << std::endl; \
    }} while(0)
#define EXPECT_LT(v1, v2) do { \
    if (v1 < v2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_LT succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_LT failed!" << std::endl; \
        std::cout << red << " Expect:" << v1 << std::endl; \
        std::cout << red << " Actual:" << v2 << std::endl; \
    }} while(0)
#define EXPECT_LE(v1, v2) do { \
    if (v1 <= v2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_LE succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_LE failed!" << std::endl; \
        std::cout << red << " Expect:" << v1 << std::endl; \
        std::cout << red << " Actual:" << v2 << std::endl; \
    }} while(0)
#define EXPECT_GT(v1, v2) do { \
    if (v1 > v2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_GT succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_GT failed!" << std::endl; \
        std::cout << red << " Expect:" << v1 << std::endl; \
        std::cout << red << " Actual:" << v2 << std::endl; \
    }} while(0)
#define EXPECT_GE(v1, v2) do { \
    if (v1 >= v2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_GE succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_GE failed!" << std::endl; \
        std::cout << red << " Expect:" << v1 << std::endl; \
        std::cout << red << " Actual:" << v2 << std::endl; \
    }} while(0)

// 字符串比较
// EXPECT_STREQ(s1, s2)    验证条件: 两个C字符串有相同的内容
// EXPECT_STRNE(s1, s2) 验证条件: 两个C字符串有不同的内容
//
// Note:
// 1. 断言应满足 EXPECT_STR*(Expect, Actual)的格式，即左边是期望值，右边是实际值
// 2. 该组断言用于比较两个C字符串。如果你想要比较两个 string 对象，相应地使用 EXPECT_EQ、EXPECT_NE 等断言
// 3. EXPECT_STREQ* 和 EXPECT_STRNE* 不接受宽字符串（wchar_t*）
// 4. 一个 NULL 空指针和一个空字符串会被认为是不一样的
// 
// Example:
// char* s1 = "", char* s2 = "abc", char* s3 = NULL;
// EXPECT_STREQ("abc", s2);    通过
// EXPECT_STREQ(s1, s3);    失败
// EXPECT_STREQ(NULL, s3);    通过
// EXPECT_STRNE(" ", s1);    通过

#define EXPECT_STREQ(s1, s2) do { \
    if(s1 == NULL || s2 == NULL) { \
        if(s1 == NULL && s2 == NULL) { \
            UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
            std::cout << green << " EXPECT_STRED succeeded!" << std::endl; \
        } \
        else { \
            UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
            UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
            std::cout << red << " EXPECT_STRED failed!" << std::endl; \
            if(s1 == NULL) std::cout << " Expect: NULL" << std::endl; \
            else std::cout << " Expect:\"" << s1 << "\"" << std::endl; \
            if(s2 == NULL) std::cout << " Actual: NULL" << std::endl; \
            else std::cout << " Actual:\"" << s2 << "\"" << std::endl; \
        } \
    } \
    else if(strcmp(s1, s2) == 0) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_STRED succeeded!" << std::endl; \
    } \
    else{ \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_STRED failed!" << std::endl; \
        std::cout << red << " Expect:\"" << s1 << "\"" << std::endl; \
        std::cout << red << " Actual:\"" << s2 << "\"" << std::endl; \
    }} while(0)
#define EXPECT_STRNE(s1, s2) do { \
    if(s1 == NULL || s2 == NULL) { \
        if(s1 != NULL || s2 != NULL) { \
            UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
            std::cout << green << " EXPECT_STRNE succeeded!" << std::endl; \
        } \
        else { \
            UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
            UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
            std::cout << red << " EXPECT_STRNE failed!" << std::endl; \
            if(s1 == NULL) std::cout << " Expect: NULL" << std::endl; \
            else std::cout << " Expect:\"" << s1 << "\"" << std::endl; \
            if(s2 == NULL) std::cout << " Actual: NULL" << std::endl; \
            else std::cout << " Actual:\"" << s2 << "\"" << std::endl; \
        } \
    } \
    else if(strcmp(s1, s2) != 0) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_STRNE succeeded!" << std::endl; \
    } \
    else{ \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_STRNE failed!" << std::endl; \
        std::cout << red << " Expect:\"" << s1 << "\"" << std::endl; \
        std::cout << red << " Actual:\"" << s2 << "\"" << std::endl; \
    }} while(0)

// 指针比较
// EXPECT_PTR_EQ(p1, p2) 验证条件: *p1 == *p2
// EXPECT_PTR_NE(p1, p2) 验证条件: *p1 != *p2
// EXPECT_PTR_RANGE_EQ(p1, p2, len) 验证条件 对任意i有(*p1 + i) == (*p2 + i) i∈[0,len)
// EXPECT_PTR_RANGE_NE(p1, p2, len) 验证条件 存在i有(*p1 + i) != (*p2 + i) i∈[0,len)
//
// Note:
// 1. 断言应满足 EXPECT_PTR_*(Expect, Actual)、EXPECT_PTR_RANGE_*(Expect, Actual, len)的格式，
//    即参数表中期望值在实际值左边
// 2. EXPECT_PTR_EQ 比较的是指针所指元素的值，如果要比较指针指向的地址是否相等，请用 EXPECT_EQ
// 3. EXPECT_PTR_RANGE_* 比较的是从 p1，p2 开始，长度为 len 的区间，请确保区间长度有效
//
// Example:
// int a[] = {1,2,3,4,5};
// int b[] = {1,2,3,4,6};
// int *p1 = a, *p2 = b;
// EXPECT_PTR_EQ(p1, p2);    通过
// p1 = a + 4, p2 = b + 4;
// EXPECT_PTR_EQ(p1, p2);    失败
// EXPECT_PTR_EQ(p1, std::find(a, a + 5, 5));    通过
// EXPECT_PTR_RANGE_EQ(a, b, 5);    //失败
// EXPECT_PTR_RANGE_EQ(a, b, 4);    //通过

#define EXPECT_PTR_EQ(p1, p2) do { \
    if(*p1 == *p2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_PTR_EQ succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_PTR_EQ failed!" << std::endl; \
        std::cout << red << " Expect:" << *p1 << std::endl; \
        std::cout << red << " Actual:" << *p2 << std::endl; \
    }} while(0)
#define EXPECT_PTR_NE(p1, p2) do { \
    if(*p1 != *p2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_PTR_NE succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_PTR_NE failed!" << std::endl; \
        std::cout << red << " Expect:" << *p1 << std::endl; \
        std::cout << red << " Actual:" << *p2 << std::endl; \
    }} while(0)
#define EXPECT_PTR_RANGE_EQ(p1, p2, len) do { \
    if(std::equal(p1, p1 + len, p2)) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_PTR_RANGE_EQ succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_PTR_RANGE_EQ failed!" << std::endl; \
    }} while(0)
#define EXPECT_PTR_RANGE_NE(p1, p2, len) do { \
    if(!std::equal(p1, p1 + len, p2)) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_PTR_RANGE_NE succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_PTR_RANGE_NE failed!" << std::endl; \
    }} while(0)

// 容器比较
// EXPECT_CON_EQ(c1, c2) 验证条件: c1 == c2
// EXPECT_CON_NE(c1, c2) 验证条件: c1 != c2
//
// Note:
// 1. 容器可以是 STL 容器， 自定义的容器，或者数组，但不可以是指针
// 2. 容器的数据类型要可以比较，保持一致或可以发生隐式转换
// 3. EXPECT_CON_EQ 断言失败时，会打印首次失败的两个值
// 
// Example:
// int arr[] = {1,2,3};
// std::vector<int> v1{1, 2, 3};
// std::vector<int> v2{2, 3, 4};
// mystl::vector<long> v3(arr, arr + 3);
// EXPECT_CON_NE(v1, v2)    可以
// EXPECT_CON_EQ(arr, v1)    可以
// EXPECT_CON_EQ(v1, v3)    可以

#define EXPECT_CON_EQ(c1, c2) do { \
    auto first1 = std::begin(c1), last1 = std::end(c1); \
    auto first2 = std::begin(c2), last2 = std::end(c2); \
    for(; first1 != last1 && first2 != last2; ++first1, ++first2) { \
        if(*first1 != *first2)    break; \
    } \
    if(first1 == last1 && first2 == last2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_CON_EQ succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_CON_EQ failed!" << std::endl; \
        std::cout << red << " Expect:" << *first1 << std::endl; \
        std::cout << red << " Actual:" << *first2 << std::endl; \
    }} while(0)
#define EXPECT_CON_NE(c1, c2) do { \
    auto first1 = std::begin(c1), last1 = std::end(c1); \
    auto first2 = std::begin(c2), last2 = std::end(c2); \
    for(; first1 != last1 && first2 != last2; ++first1, ++first2) { \
        if(*first1 != *first2)    break; \
    } \
    if(first1 != last1 || first2 != last2) { \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++; \
        std::cout << green << " EXPECT_CON_NE succeeded!" << std::endl; \
    } \
    else { \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0; \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++; \
        std::cout << red << " EXPECT_CON_NE failed!" << std::endl; \
    }} while(0)


/***************************************************************************************/
// 常用宏定义

// 不同情况的测试数量级
#if defined(_MSC_VER) && defined(_DEBUG)

#define LEN1    10000
#define LEN2    100000
#define LEN3    1000000
#define LEN4    10000000

#else

#define LEN1    100000
#define LEN2    1000000
#define LEN3    10000000
#define LEN4    100000000

#endif 

#define WIDE    14

// 输出通过提示
#define PASSED    std::cout << "[ PASSED ]" << std::endl

// 遍历输出容器
#define COUT(container) do { \
    std::string con_name = #container; \
    std::cout << " " << con_name << " :"; \
    for(auto it : container)    std::cout << " " << it; std::cout << std::endl; \
} while(0)
    
#define STR_COUT(str) do { \
    std::string str_name = #str; \
    std::cout << " " << str_name << " : " << str << std::endl; \
} while(0)
    
// 输出容器调用函数后的结果
#define FUN_AFTER(con, fun) do { \
    std::string fun_name = #fun; \
    std::cout << " After " << fun_name << " :" << std::endl; \
    fun; \
    COUT(con); \
} while(0)

#define STR_FUN_AFTER(str, fun) do { \
    std::string fun_name = #fun; \
    std::cout << " After " << fun_name << " :" << std::endl; \
    fun; \
    STR_COUT(str); \
} while(0)

// 输出容器调用函数的值
#define FUN_VALUE(fun) do { \
    std::string fun_name = #fun; \
    std::cout << " " << fun_name << " : " << fun << std::endl; \
} while(0)

// 输出测试数量级
void test_len(size_t len1, size_t len2, size_t len3, size_t wide) {
    std::string str1, str2, str3;
    std::stringstream ss;
    ss << len1 << " " << len2 << " " << len3;
    ss >> str1 >> str2 >> str3;
    str1 += "   |";
    std::cout << std::setw(wide) << str1;
    str2 += "   |";
    std::cout << std::setw(wide) << str2;
    str3 += "   |";
    std::cout << std::setw(wide) << str3 << std::endl;
}

#define TEST_LEN(len1, len2, len3, wide) \
    test_len(len1, len2, len3, wide)


// 简单测试的宏定义
#define TEST(testcase_name) \
    MYTINYSTL_TEST_(testcase_name)

// 运行所有测试案例
#define RUN_ALL_TESTS() \
    mystl::test::UnitTest::GetInstance()->Run()

}    // test
}    // mystl

#endif // !MYTINYSTL_TEST_H_

