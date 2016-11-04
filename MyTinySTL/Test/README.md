UnitTest
=====
###说明
  测试在不同环境下的结果会有所差异，内存过小会导致测试失败。简单测试可能没有覆盖到所有的测试情况，性能测试只能大致比较 std 与 mystl 的相对情况。<br>

  我运行了所有测试并截图保存在了 [images](https://github.com/Alinshans/MyTinySTL/tree/master/MyTinySTL/Test/images) 目录下，可自行查看，测试结果仅供参考。<br>
  
  若有任何疑问，可在`issues`中提出。
___
###测试框架
  测试框架的头文件定义在 [test.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/test.h) 中，用了两个 class template 实现了一个简单的测试框架。使用宏掩盖复杂的封装测试过程。因为要运行全部测试，并记录下成功与失败的案例数，所以没有使用`assert`断言，而是使用了`expect`断言，保证测试的完成。
  
###测试内容
####测试内容包括以下方面
  * [algorithm](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/algorithm_test.h)
  * [algorithm_performance](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/algorithm_performance_test.h)
  * [deque](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/deque_test.h)
  * [hash_map](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/hash_map_test.h)
    * hash_map
    * hash_multimap
  * [hash_set](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/hash_set_test.h)
    * hash_set
    * hash_multiset
  * [list](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/list_test.h)
  * [map](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/map_test.h)
    * map
    * multimap
  * [queue](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/queue_test.h)
    * queue
    * priority_queue
  * [set](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/set_test.h)
    * set
    * multiset
  * [stack](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/stack_test.h)
  * [vector](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/Test/vector_test.h)
  
