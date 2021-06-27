单元测试 (Unit test)
=====
## 测试环境 (Test environment)
  测试直接在 `Travis CI` 和 `AppVeyor` 上构建并运行，已在以下环境中做过测试：
  
  Tests were built and run directly on `Tracis CI` and `AppVeyor` and had been tested in the following environments:

  * linux, ubuntu 14.04, gcc5/6/7
  * osx, xcode5/6/7/8
  * windows, VS2015/VS2017, [x64|x86], [Release|Debug]
  
## 测试框架 (Test frame)
  在 [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/Test/test.h) 中，用了两个类实现了一个简单的测试框架，并定义了大量宏来封装测试过程。</br>
  In this file [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/Test/test.h), I used two class to implement a simple test framework, and defined a lot of macros to package testing process.
  
## 测试内容 (Test content)
  在 [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/Test/test.h) 中定义了两个宏，`PERFORMANCE_TEST_ON` 和 `LARGER_TEST_DATA_ON`。`PERFORMANCE_TEST_ON` 代表开启性能测试，默认定义为 `1`。`LARGER_TEST_DATA_ON` 代表增大测试数据，默认定义为 `0`。**如果你想把 `LARGER_TEST_DATA_ON` 设置为 `1`，建议电脑配置为：处理器 i5 或以上，内存 8G 以上。**<br>
  In this file [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/Test/test.h), I defined two marcos: `PERFORMANCE_TEST_ON` and `LARGER_TEST_DATA_ON`. `PERFORMANCE_TEST_ON` means to run performance test, the default is defined as `1`. `LARGER_TEST_DATA_ON` means to increase the test data, the default is defined as `0`. **If you want to set `LARGER_TEST_DATA_ON` to `1`, the proposed computer configuration is: CPU i5 or above, memory 8G or more.**

  测试案例如下：<br>
  The test cases are as follows:

  * [algorithm](https://github.com/Alinshans/MyTinySTL/blob/master/Test/algorithm_test.h) *(100%/100%)*
  * [algorithm_performance](https://github.com/Alinshans/MyTinySTL/blob/master/Test/algorithm_performance_test.h) *(100%/100%)*
  * [deque](https://github.com/Alinshans/MyTinySTL/blob/master/Test/deque_test.h) *(100%/100%)*
  * [list](https://github.com/Alinshans/MyTinySTL/blob/master/Test/list_test.h) *(100%/100%)*
  * [map](https://github.com/Alinshans/MyTinySTL/blob/master/Test/map_test.h) *(100%/100%)*
    * map
    * multimap
  * [queue](https://github.com/Alinshans/MyTinySTL/blob/master/Test/queue_test.h) *(100%/100%)*
    * queue
    * priority_queue
  * [set](https://github.com/Alinshans/MyTinySTL/blob/master/Test/set_test.h) *(100%/100%)*
    * set
    * multiset
  * [stack](https://github.com/Alinshans/MyTinySTL/blob/master/Test/stack_test.h) *(100%/100%)*
  * [string_test](https://github.com/Alinshans/MyTinySTL/blob/master/Test/string_test.h) *(100%/100%)*
  * [unordered_map](https://github.com/Alinshans/MyTinySTL/blob/master/Test/unordered_map_test.h) *(100%/100%)*
    * unordered_map
    * unordered_multimap
  * [unordered_set](https://github.com/Alinshans/MyTinySTL/blob/master/Test/unordered_set_test.h) *(100%/100%)*
    * unordered_set
    * unordered_multiset
  * [vector](https://github.com/Alinshans/MyTinySTL/blob/master/Test/vector_test.h) *(100%/100%)*
  
  
## 测试结果 (Test result)
  见 [Travis CI](https://travis-ci.org/Alinshans/MyTinySTL) 和 [AppVeyor](https://ci.appveyor.com/project/Alinshans/mytinystl)。

  See [Travis CI](https://travis-ci.org/Alinshans/MyTinySTL) and [AppVeyor](https://ci.appveyor.com/project/Alinshans/mytinystl).
  
