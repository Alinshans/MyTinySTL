#ifndef MYTINYSTL_VECTOR_TEST_H
#define MYTINYSTL_VECTOR_TEST_H

// vector 测试: 测试 vector 的 API 与 push_back 的性能

#include <iomanip>
#include <iostream>
#include <vector>

#include "..\vector.h"
#include "test.h"

using namespace std;

namespace mystl {
	namespace test {
		namespace vector_test {
			
			// push_back 的性能测试
			#define PUSH_BACK_TEST(mode, count) do { \
				srand((int)time(0)); \
				clock_t start, end; \
				char buf[5]; \
				mode::vector<int> v; \
				start = clock(); \
				for (size_t i = 0; i < count; ++i) \
					v.push_back(rand()); \
				end = clock(); \
				int n = end - start; \
				sprintf_s(buf, "%d", n); \
				string t = buf; \
				t += "ms   /"; \
				cout << setw(12) << t; \
			} while(0)
			

			void vector_test() {
				cout << "[=========================================================]" << endl;
				cout << "[-------------- Run container test : vector --------------]" << endl;
				cout << "[----------------------- API test ------------------------]" << endl;
				int a[] = { 1,2,3,4,5 };
				mystl::vector<int> v1;
				mystl::vector<int> v2(10);
				mystl::vector<int> v3(10, 1);
				mystl::vector<int> v4(a, a + 5);
				mystl::vector<int> v5(v2);
				mystl::vector<int> v6(std::move(v2));
				mystl::vector<int> v7 = v3;
				mystl::vector<int> v8 = std::move(v3);
				v8.~vector();
			
				cout << " v1 assign(10):" << endl;
				v1.assign(10);
				COUT(v1);
				cout << " v1 assign(8,8):" << endl;
				v1.assign(8, 8);
				COUT(v1);
				cout << " v1 assign(a,a+5):" << endl;
				v1.assign(a, a + 5);
				COUT(v1);
				cout << " v1 push_back(6):" << endl;
				v1.push_back(6);
				COUT(v1);
				cout << " v1 insert(v1.begin()):" << endl;
				v1.insert(v1.begin());
				COUT(v1);
				cout << " v1 insert(v1.end(),7):" << endl;
				v1.insert(v1.end(), 7);
				COUT(v1);
				cout << " v1 insert(v1.begin()+3,2,3):" << endl;
				v1.insert(v1.begin() + 3, 2, 3);
				COUT(v1);
				cout << " v1 insert(v1.begin(),a,a+5):" << endl;
				v1.insert(v1.begin(), a, a + 5);
				COUT(v1);
				cout << " v1 pop_back():" << endl;
				v1.pop_back();
				COUT(v1);
				cout << " v1 erase(v1.begin()):" << endl;
				v1.erase(v1.begin());
				COUT(v1);
				cout << " v1 erase(v1.begin(),v1.begin()+2):" << endl;
				v1.erase(v1.begin(), v1.begin() + 2);
				COUT(v1);
				cout << " v1 reverse():" << endl;
				v1.reverse();
				COUT(v1);
				cout << " v1 swap(v4):" << endl;
				v1.swap(v4);
				COUT(v1);
				cout << " *v1.begin():" << *v1.begin() << endl;
				cout << " *(v1.end()-1):" << *(v1.end() - 1) << endl;
				cout << " *v1.rbegin():" << *v1.rbegin() << endl;
				cout << " *(v1.rend()-1):" << *(v1.rend() - 1) << endl;
				cout << " v1.front():" << v1.front() << endl;
				cout << " v1.back():" << v1.back() << endl;
				cout << " v1[0]:" << v1[0] << endl;
				cout << " v1.at(1):" << v1.at(1) << endl;
				cout << " v1 empty():" << boolalpha << v1.empty() << noboolalpha << endl;
				cout << " v1 size():" << v1.size() << endl;
				cout << " v1 max_size():" << v1.max_size() << endl;
				cout << " v1 capacity():" << v1.capacity() << endl;
				cout << " v1 resize(10):" << endl;
				v1.resize(10);
				COUT(v1);
				cout << " v1 size():" << v1.size() << " ,v1 capacity():" << v1.capacity() << endl;
				cout << " v1 resize(6,6):" << endl;
				v1.resize(6, 6);
				COUT(v1);
				cout << " v1 size():" << v1.size() << " ,v1 capacity():" << v1.capacity() << endl;
				cout << " v1 clear():" << endl;
				v1.clear();
				cout << " v1 size():" << v1.size() << " ,v1 capacity():" << v1.capacity() << endl;
				cout << " v1 reseve(5):" << endl;
				v1.reserve(5);
				cout << " v1 size():" << v1.size() << " ,v1 capacity():" << v1.capacity() << endl;
				cout << " v1 reserve(20):" << endl;
				v1.reserve(20);
				cout << " v1 size():" << v1.size() << " ,v1 capacity():" << v1.capacity() << endl;
				cout << "[ PASSED ]" << endl;
				cout << "[------------------ Performance Testing ------------------]" << endl;
				mystl::vector<int> mv;
				std::vector<int> sv;
				cout << "/-----------/-----------/-----------/-----------/" << endl;
				cout << "/ push_back /  1000000  / 10000000  / 100000000 /" << endl;
				cout << "/   std     /";
				PUSH_BACK_TEST(std, 1000000);
				PUSH_BACK_TEST(std, 10000000);
				PUSH_BACK_TEST(std, 100000000);
				cout << endl << "/   mystl   /";
				PUSH_BACK_TEST(mystl, 1000000);
				PUSH_BACK_TEST(mystl, 10000000);
				PUSH_BACK_TEST(mystl, 100000000);
				cout << endl;
				cout << "/-----------/-----------/-----------/-----------/" << endl;
				cout << "[ PASSED ]" << endl;
				cout << "[-------------- End container test : vector --------------]" << endl;
			}
		}
	}
}
#endif // !MYTINYSTL_VECTOR_TEST_H

