UnitTest
=====
###测试环境
  测试直接在 `Travis CI` 和 `AppVeyor` 上进行，分别在 `Linux` 和 `Windows` 上对不同编译器进行测试：
  * linux, g++-4.7
  * linux, g++-4.8
  * linux, g++-5
  * linux, g++-6
  * linux, clang++3.5
  * linux, clang++3.6
  * linux, clang++3.7
  * linux, clang++3.8
  * windows, msvc 14.0 (Visual Studio 2015)
  
  
###测试框架
  在 [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/test.h) 中，用了两个 `class template` 实现了一个简单的测试框架。定义了大量宏封装测试。</br>
  
  
###测试内容
  在 [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/test.h) 中定义了两个宏，`PERFORMANCE_TEST_ON`, `MEMORY_IS_ENOUGH`，`PERFORMANCE_TEST_ON` 宏定义为 `1`，开启性能测试，`MEMORY_IS_ENOUGH` 宏定义为 `1`，增大测试数据（内存消耗）。默认开启 `PERFORMANCE_TEST_ON` 关闭 `MEMORY_IS_ENOUGH`。<br>
  测试案例如下：
  
  * [algorithm](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/algorithm_test.h) *(100%/100%)*
  * [algorithm_performance](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/algorithm_performance_test.h) *(100%/100%)*
  * [deque](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/deque_test.h) *(100%/100%)*
  * [list](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/list_test.h) *(100%/100%)*
  * [map](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/map_test.h) *(100%/100%)*
    * map
    * multimap
  * [queue](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/queue_test.h) *(100%/100%)*
    * queue
    * priority_queue
  * [set](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/set_test.h) *(100%/100%)*
    * set
    * multiset
  * [stack](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/stack_test.h) *(100%/100%)*
  * [string_test](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/string_test.h) *(100%/100%)*
  * [unordered_map](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/unordered_map_test.h) *(100%/100%)*
    * unordered_map
    * unordered_multimap
  * [unordered_set](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/unordered_set_test.h) *(100%/100%)*
    * unordered_set
    * unordered_multiset
  * [vector](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/vector_test.h) *(100%/100%)*
  
  
###测试结果
  见 [Travis CI](https://travis-ci.org/Alinshans/MyTinySTL) 和 [AppVeyor](https://ci.appveyor.com/project/Alinshans/mytinystl)。
  
