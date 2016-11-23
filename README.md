MyTinySTL
=======
####简介
   这是一个小型的STL，为了练习`数据结构`、`模板编程`和`c++11`的特性，而做的一个练习。这个小型的STL实现了绝大部分STL中原有的函数、容器，加入了一些c++标准库中的内容，也有部分作了修改。这个项目仅作为**学习用途**，请勿用作其它用途。若想下载使用或测试，只需在`Clone or download`下选择`Download ZIP`，然后将`MyTinySTL`下的`.h`文件加入项目中即可。如发现错误，还请在`Issues`中指出。欢迎`Fork`和`Pull requests`改善、补充代码。<br>
    
####编译环境
   * [MyTinySTL](https://github.com/Alinshans/MyTinySTL/tree/master/MyTinySTL)下的所有`header file`可跨平台使用
   * 本机测试环境
      * OS:  Windows 10
      * IDE: Visual Studio 2015
   * 若想进行全部单元测试，建议内存大小在**8G**以上

___
##目录
* [配置器](#配置器)
  * allocator
    * allocate
    * deallocate
  * constructor
    * construct
    * destroy
* [迭代器](#迭代器)
  * iterator
* [仿函数](#仿函数)
  * functional
  * hash_functional  
* [容器](#容器)
  * vector
  * list
  * deque
  * map
  * set
  * hash_map
  * hash_set
  * basic_string
* [算法](#算法)
  * 基本算法
  * 数值算法
  * set 算法
  * heap 算法
  * 其他算法
* [配接器](#配接器)
  * container adapters
    * stack
    * queue
    * priority_queue
  * iterator adapters
    * reverse_iterator
    
___


##配置器
   包括了 `allocator` 和 `constructor`，分别定义在 [allocator.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/allocator.h) 和 [construct.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/construct.h) 中。<br>
   `allocator` 负责空间的配置与回收，对应两个成员函数： `allocate` 和 `deallocate`。<br>
   `constructor` 负责对象的构造与析构，对应两个全局函数： `construct` 和 `destroy`。<br>
   
##迭代器
   `iterator`，连接着容器与算法，是一种`泛型指针`，定义在 [iterator.h](https://github.com/Alinshans/MyTinySTL/edit/master/MyTinySTL/iterator.h) 中。每个容器都附带专属的迭代器，是一种重载了 `operator*`，`operator->`，`operator++`，`operator--` 等操作的模板类。<br>

##仿函数
   仿函数，现在称为`函数对象`，包括了 `functional` 和 `hash_functional`，分别定义在 [functional.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/functional.h) 和 [hash_functional.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/hash_functional.h) 中。 <br>
###functional
   * 算术类
      * plus
      * minus
      * multiplies
      * divides
      * modulus
      * negate
      * identity_element
   * 关系运算类
      * equal_to
      * not_equal_to
      * greater
      * less
      * greater_equal
      * less_equal
   * 逻辑运算类
      * logical_and
      * logical_or
      * logical_not
   * 证同、选择、投影
      * identity
      * selectfirst
      * selectsecond
      * projectfirst
      * projectsecond <br>
      
###hash_functional
  `hash_functional` 中的 hash 函数对象用于计算元素的哈希值(在哈希表中的位置)，只对一些内置型别做了特化处理。
   
##容器
   以下列出主要容器以及它们主要的接口，详情请到项目中查看。
   * [vector](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/vector.h)（向量）
      * constructor
      * destructor
      * operator=
      * Iterators
         * begin
         * end
         * rbegin
         * rend
      * Capacity
         * empty
         * size
         * max_size
         * capacity
         * shrink_to_fit
      * Element access
         * operator[]
         * at
         * front
         * back
      * Modifiers
         * assign
         * push_back
         * pop_back
         * insert
         * erase
         * clear
         * resize
         * reserve
         * swap
      * Operations
         * reverse
   * [list](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/list.h)（双向链表）
      * constructor
      * destructor
      * operator=
      * Iterators
         * begin
         * end
         * rbegin
         * rend
      * Capacity
         * empty
         * size
         * max_size
      * Element access
         * front
         * back
      * Modifiers
         * assign
         * push_front
         * push_back
         * pop_front
         * pop_back
         * insert
         * erase
         * clear
         * resize
         * swap
      * Operations
         * splice
         * remove
         * remove_if
         * unique
         * merge
         * sort
         * reverse
   * [deque](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/deque.h)（双端队列）
      * constructor
      * destructor
      * operator=
      * Iterators
         * begin
         * end
         * rbegin
         * rend
      * Capacity
         * empty
         * size
         * max_size
      * Element access
         * operator[]
         * at
         * front
         * back   
      * Modifiers
         * assign
         * push_front
         * push_back
         * pop_front
         * pop_back
         * insert
         * erase
         * clear
         * resize
         * swap
   * [map](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/map.h)（映射）
      * constructor
      * operator=
      * Iterators
         * begin
         * end
         * rbegin
         * rend
      * Capacity
         * empty
         * size
         * max_size
      * Element access
         * operator[]
      * Modifiers
         * insert
         * erase
         * clear
         * swap
      * Operations
         * find
         * count
         * lower_bound
         * upper_bound
         * equal_range
   * [set](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/set.h)（集合）
      * constructor
      * operator=
      * Iterators
         * begin
         * end
         * rbegin
         * rend
      * Capacity
         * empty
         * size
         * max_size
      * Element access
         * operator[]
         * at
      * Modifiers
         * insert
         * erase
         * clear
         * swap
      * Operations
         * find
         * count
         * lower_bound
         * upper_bound
         * equal_range
   * [hash_map](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/hash_map.h)（哈希映射）
      * constructor
      * operator=
      * Iterators
         * begin
         * end
      * Capacity
         * empty
         * size
         * max_size
      * Element access
         * operator[]
      * Modifiers
         * insert 
         * insert_noresize
         * erase
         * clear
         * swap
      * Operations
         * find
         * count
         * equal_range
      * Buckets
         * reserve
         * bucker_count
         * max_bucket_count
         * elems_in_bucket
   * [hash_set](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/hash_set.h)（哈希集合）
      * constructor
      * operator=
      * Iterators
         * begin
         * end
      * Capacity
         * size
         * max_size
         * empty
      * Modifiers
         * insert 
         * insert_noresize
         * erase
         * clear
         * swap
      * Operations
         * find
         * count
         * equal_range
      * Buckets
         * reserve   
         * bucker_count
         * max_bucket_count
         * elems_in_bucket
   * [basic_string](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/basic_string.h) （字符串）
      * constructor
      * destructor
      * operator=
      * Iterators
         * begin
         * end
         * rbegin
         * rend
      * Capacity
         * empty
         * size
         * length
         * max_size
         * capacity
         * shrink_to_fit
      * Element access
         * operator[]
         * at
         * front
         * back
         * data
         * c_str
      * Modifiers
         * insert
         * erase
         * clear
         * add_back
         * add_front
         * operator+=
      * Operations
         * compare
         * substr
         * remove
         * remove_if
         * replace
         * replace
         * find
         * rfind
         * count
         * reverse
         * swap
         
##算法
   * [基本算法](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/algobase.h)（13个） 定义在 [algobase.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/algobase.h)
      * copy
      * copy_backward
      * copy_if
      * copy_n
      * equal
      * fill
      * fill_n
      * iter_swap
      * lexicographical_compare
      * max
      * min
      * mismatch
      * swap
   * [数值算法](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/numeric.h)（5个） 定义在 [numeric.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/numeric.h)
      * accumulate
      * adjacent_difference
      * inner_product
      * iota
      * partial_sum
   * [set 算法](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/set_algo.h)（4个） 定义在 [set_algo.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/heap_algo.h)
      * set_difference
      * set_intersection
      * set_symmetric_difference
      * set_union
   * [heap 算法](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/heap_algo.h)（4个） 定义在 [heap_algo.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/heap_algo.h)
      * make_heap
      * pop_heap
      * push_heap
      * sort_heap
   * [其它算法](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/algo.h)（53个） 定义在 [algo.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/algo.h)
      * adjacent_find
      * all_of
      * any_of
      * binary_search
      * count 
      * count_if
      * equal_range
      * find
      * find_end
      * find_first_of
      * find_if
      * find_if_not
      * for_each
      * generate
      * generate_n
      * includes
      * inplace_merge
      * is_heap
      * is_sorted
      * lower_bound
      * max_element
      * median
      * merge
      * min_elememt
      * next_permutation
      * none_of
      * nth_element
      * partial_sort
      * partial_sort_copy
      * partition
      * partition_copy
      * prev_permutation
      * random_shuffle
      * remove
      * remove_copy
      * remove_copy_if
      * remove_if
      * replace
      * replace_copy
      * replace_copy_if
      * replace_if
      * reverse
      * reverse_copy
      * rotate
      * rotate_copy
      * search
      * search_n
      * sort
      * swap_ranges
      * transform
      * unique
      * unique_copy
      * upper_bound

##配接器

###container adapters
   * [stack](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/stack.h) 
      * constructor
      * operator=
      * Element access
         * top
      * Capacity
         * empty
         * size
      * Modifiers
         * push
         * pop
         * clear
         * swap
   * [queue](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/queue.h)
      * constructor
      * operator=
      * Element access
         * front
         * back
      * Capacity
         * empty
         * size
      * Modifiers
         * push
         * pop
         * clear
         * swap
   * [priority_queue](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/queue.h)
      * constructor
      * operator=
      * Element access
         * top
      * Capacity
         * empty
         * size
      * Modifiers
         * push
         * pop
         * clear
         * swap
         
###iterator adapters
   * [reverse_iterator](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/reverse_iterator.h)
   `reverse_iterator`是一种反向迭代器，重载了`operator*`，`operator->`，`operator++`，`operator--`，`operator+`，`operator-`，`operator+=`，`operatpr-=`，`operator[]`等操作，变前进为后退，后退为前进。
      
