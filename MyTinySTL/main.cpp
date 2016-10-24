#include "Test\algorithm_test.h"
#include "Test\algorithm_performance_test.h"
#include "Test\vector_test.h"
#include "Test\list_test.h"
#include "Test\deque_test.h"
#include "Test\queue_test.h"
#include "Test\stack_test.h"
#include "Test\map_test.h"
#include "Test\set_test.h"
#include "Test\hash_map_test.h"
#include "Test\hash_set_test.h"

int main() {

	// 运行所有简单测试、性能测试、容器测试

	RUN_ALL_TESTS();
	mystl::test::algorithm_performance_test::algorithm_performance_test();
	mystl::test::vector_test::vector_test(); 
	mystl::test::list_test::list_test();
	mystl::test::deque_test::deque_test();
	mystl::test::queue_test::queue_test();
	mystl::test::queue_test::priority_test();
	mystl::test::stack_test::stack_test();
	mystl::test::map_test::map_test();
	mystl::test::map_test::multimap_test();
	mystl::test::set_test::set_test();
	mystl::test::set_test::multiset_test();
	mystl::test::hash_map_test::hash_map_test();
	mystl::test::hash_map_test::hash_multimap_test();
	mystl::test::hash_set_test::hash_set_test();
	mystl::test::hash_set_test::hash_multiset_test();
}
