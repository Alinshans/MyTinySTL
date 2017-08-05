单元测试 (Unit test)
=====
## 测试环境 (Test environment)
  测试直接在 `Travis CI` 和 `AppVeyor` 上构建并运行，已在以下环境中做过测试：
  
  Tests were built and run directly on `Tracis CI` and `AppVeyor` and had been tested in the following environments:

  * linux, g++-5
  * linux, g++-6
  * linux, g++-7
  * osx, g++-5
  * osx, clang++
  * windows, msvc 14.0 (VS2015)
  
## 测试框架 (Test frame)
  在 [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/test.h) 中，用了两个类实现了一个简单的测试框架，并定义了大量宏来封装测试过程。</br>
  In this file [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/test.h), I used two class to implement a simple test framework, and defined a lot of macros to package testing process.
  
## 测试内容 (Test content)
  在 [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/test.h) 中定义了两个宏，`PERFORMANCE_TEST_ON` 和 `MEMORY_IS_ENOUGH`。`PERFORMANCE_TEST_ON` 宏定义为 `1`，开启性能测试，`MEMORY_IS_ENOUGH` 宏定义为 `1`，增大测试数据（更多内存消耗）。默认 `PERFORMANCE_TEST_ON` 为 `1`， `MEMORY_IS_ENOUGH` 为 `0`。<br>
  In this file [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/test.h), I defined two marcos: `PERFORMANCE_TEST_ON` and `MEMORY_IS_ENOUGH`. If `PERFORMANCE_TEST_ON` is defined as `1`, it will run the performance test, if `MEMORY_IS_ENOUGH` is defined as `1`, it will take larger test data(which will consume more memory). By default, `PERFORMANCE_TEST_ON` is `1` and `MEMORY_IS_ENOUGH` is `0`.

  测试案例如下：<br>
  The test cases are as follows:

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
  
  
## 测试结果 (Test result)
  见 [Travis CI](https://travis-ci.org/Alinshans/MyTinySTL) 和 [AppVeyor](https://ci.appveyor.com/project/Alinshans/mytinystl)。

  See [Travis CI](https://travis-ci.org/Alinshans/MyTinySTL) and [AppVeyor](https://ci.appveyor.com/project/Alinshans/mytinystl).
  
