#ifndef MYTINYSTL_QUEUE_TEST_H_
#define MYTINYSTL_QUEUE_TEST_H_

// queue test : 测试 queue, priority_queue 的接口和它们 push 的性能

#include <queue>

#include "../queue.h"
#include "test.h"

namespace mystl {
namespace test {
namespace queue_test {

//  queue 的遍历输出
#define QUEUE_COUT(q) do { \
    std::string q_name = #q; \
    std::cout << " " << q_name << " :"; \
    queue_print(q); \
} while(0)

// priority_queue 的遍历输出
#define P_QUEUE_COUT(p) do { \
    std::string p_name = #p; \
    std::cout << " " << p_name << " :"; \
    p_queue_print(p); \
} while(0)

void queue_print(mystl::queue<int> q) {
    while (!q.empty()) {
        std::cout << " " << q.front();
        q.pop();
    }
    std::cout << std::endl;
}

void p_queue_print(mystl::priority_queue<int> p) {
    while (!p.empty()) {
        std::cout << " " << p.top();
        p.pop();
    }
    std::cout << std::endl;
}

void queue_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------------- Run container test : queue ------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
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

    std::cout << " After q1 push 1,2,3,4,5 :" << std::endl;
    q1.push(1);
    q1.push(2);
    q1.push(3);
    q1.push(4);
    q1.push(5);
    QUEUE_COUT(q1);
    std::cout << std::boolalpha;
    FUN_VALUE(q1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(q1.size());
    FUN_VALUE(q1.front());
    FUN_VALUE(q1.back());
    while (!q1.empty()) {
        std::cout << " After q1.pop():" << std::endl;
        q1.pop();
        QUEUE_COUT(q1);
    }
    std::cout << " After q1.swap(q4) :" << std::endl;
    q1.swap(q4);
    QUEUE_COUT(q1);
    std::cout << " After q1.clear() :" << std::endl;
    q1.clear();
    QUEUE_COUT(q1);
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|         push        |";
    CON_TEST_P1(queue<int>, push, rand(), LEN2, LEN3, LEN4);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[----------------- End container test : queue ------------------]" << std::endl;
}

void priority_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[------------- Run container test : priority_queue -------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
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

    std::cout << " After p1 push 1,2,3,4,5 :" << std::endl;
    p1.push(1);
    p1.push(2);
    p1.push(3);
    p1.push(4);
    p1.push(5);
    P_QUEUE_COUT(p1);
    std::cout << std::boolalpha;
    FUN_VALUE(p1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(p1.size());
    FUN_VALUE(p1.top());
    while (!p1.empty()) {
        std::cout << " After p1.pop():" << std::endl;
        p1.pop();
        P_QUEUE_COUT(p1);
    }
    std::cout << " After p1.swap(p4) :" << std::endl;
    p1.swap(p4);
    P_QUEUE_COUT(p1);
    std::cout << " After p1.clear() :" << std::endl;
    p1.clear();
    P_QUEUE_COUT(p1);
    PASSED;
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|         push        |";
    CON_TEST_P1(priority_queue<int>, push, rand(), LEN2, LEN3, LEN4);
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
    std::cout << "[------------- End container test : priority_queue -------------]" << std::endl;
}

} // namespace queue_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_QUEUE_TEST_H_

