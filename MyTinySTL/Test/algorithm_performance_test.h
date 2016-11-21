#ifndef MYTINYSTL_ALGORITHM_PERFORMANCE_TEST_H_
#define MYTINYSTL_ALGORITHM_PERFORMANCE_TEST_H_

// 仅仅针对 sort, binary_search 做了性能测试

#include <algorithm>

#include "../algorithm.h"

namespace mystl {
    namespace test {
        namespace algorithm_performance_test {

            // 第一种函数类型测试
            #define FUN_TEST1(mode, fun, count) do { \
                std::string fun_name = #fun; \
                srand((int)time(0)); \
                char buf[10]; \
                clock_t start, end; \
                int *arr = new int[count]; \
                for(size_t i = 0; i < count; ++i)    *(arr + i) = rand(); \
                start = clock(); \
                mode::fun(arr, arr + count); \
                end = clock(); \
                int n = end - start; \
                sprintf_s(buf, "%d", n); \
                std::string t = buf; \
                t += "ms   |"; \
                std::cout << std::setw(WIDE) << t; \
                delete []arr; \
            } while(0)

            // 第二种函数类型测试
            #define FUN_TEST2(mode, fun, count) do { \
                std::string fun_name = #fun; \
                srand((int)time(0)); \
                char buf[10]; \
                clock_t start, end; \
                int *arr = new int[count]; \
                for(size_t i = 0; i < count; ++i)    *(arr + i) = rand(); \
                start = clock(); \
                for(size_t i = 0; i < count; ++i) \
                    mode::fun(arr, arr + count, rand()); \
                end = clock(); \
                int n = end - start; \
                sprintf_s(buf, "%d", n); \
                std::string t = buf; \
                t += "ms   |"; \
                std::cout << std::setw(WIDE) << t; \
                delete []arr; \
            } while(0)

            void binary_search_test() {
                std::cout << "[------------------- function : binary_search ------------------]" << std::endl;
                std::cout << "| orders of magnitude |";
                TEST_LEN(LEN2, LEN3, LEN4, WIDE);
                std::cout << "|         std         |";
                FUN_TEST2(std, binary_search, LEN2);
                FUN_TEST2(std, binary_search, LEN3);
                FUN_TEST2(std, binary_search, LEN4);
                std::cout << std::endl << "|        mystl        |";
                FUN_TEST2(mystl, binary_search, LEN2);
                FUN_TEST2(mystl, binary_search, LEN3);
                FUN_TEST2(mystl, binary_search, LEN4);
                std::cout << std::endl;
            }
    
            void sort_test() {
                std::cout << "[----------------------- function : sort -----------------------]" << std::endl;
                std::cout << "| orders of magnitude |";
                TEST_LEN(LEN2, LEN3, LEN4, WIDE);
                std::cout << "|         std         |";
                FUN_TEST1(std, sort, LEN2);
                FUN_TEST1(std, sort, LEN3);
                FUN_TEST1(std, sort, LEN4);
                std::cout << std::endl << "|        mystl        |";
                FUN_TEST1(mystl, sort, LEN2);
                FUN_TEST1(mystl, sort, LEN3);
                FUN_TEST1(mystl, sort, LEN4);
                std::cout << std::endl;
            }

            void algorithm_performance_test() {
                std::cout << "[===============================================================]" << std::endl;
                std::cout << "[--------------- Run algorithm performance test ----------------]" << std::endl;
                sort_test();
                binary_search_test();
                std::cout << "[--------------- End algorithm performance test ----------------]" << std::endl;
                std::cout << "[===============================================================]" << std::endl;
            }
        }
    }
}
#endif // !MYTINYSTL_ALGORITHM_PERFORMANCE_TEST_H_

