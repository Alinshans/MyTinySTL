MyTinySTL
=======
   这是一个小型的STL，为了练习`数据结构`、`模板编程`和`c++11`的特性，而做的一个练习。这个小型的STL实现了绝大部分STL中原有的函数、容器，也加入了一些c++标准库中的内容。这个项目仅作为学习用途，若要下载运行，要将解决方案配置为`Release`。如发现错误，还请指出。
    
   编译环境：Visual Studio 2015  (ps: 建议内存大小: 8G 以上)
    
   以下介绍具体的实现内容。
  
=======
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
  * iterator adapters
    * reverse_iterator
    
___


##配置器
   包括了 `allocator` 和 `constructor`，分别定义在 [allocator.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/allocator.h) 和 [construct.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/construct.h) 中。<br>
   `allocator` 负责空间的配置与回收，对应两个成员函数： `allocate` 和 `deallocate`。<br>
   `construct` 负责对象的构造与析构，对应两个全局函数： `construct` 和 `destroy`。
   
##迭代器
   `iterator`，连接着容器与算法，是一种`泛型指针`，定义在 [iterator.h](https://github.com/Alinshans/MyTinySTL/edit/master/MyTinySTL/iterator.h) 中。每个容器都附带专属的迭代器，是一种重载了 `operator*`，`operator->`，`operator++`，`operator--` 等操作的模板类。

##仿函数
   仿函数，现在称为`函数对象`，包括了 `functional` 和 `hash_functional`，分别定义在 [functional.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/functional.h) 和 [hash_functional.h](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/hash_functional.h) 中。 <br>
   `functional` 中包含
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
      * projectsecond
   `hash_functional` 中的 hash 函数对象用于计算元素的哈希值(在哈希表中的位置)，并对一些内置型别做了特化处理。
   
##容器
   以下列出主要容器以及它们主要的 API，详情请到项目中查看。
   * [vector](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/vector.h)
      * constructor
      * destructor
      * operator=
      * Iterators
         * begin
         * end
         * rbegin
         * rend
      * Capacity
         * size
         * max_size
         * resize
         * capacity
         * empty
         * reserve
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
         * swap
   * [list](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/list.h)
      * constructor
      * destructor
      * operator=
      * Iterators
      * Capacity
      * Element access
      * Modifiers
      * Operations
   * [deque](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/deque.h)
      * constructor
      * destructor
      * operator=
      * Iterators
      * Capacity
      * Element access
      * Modifiers
      * Operations
   * [map](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/map.h)
      * constructor
      * destructor
      * operator=
      * Iterators
      * Capacity
      * Element access
      * Modifiers
      * Operations
   * [set](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/set.h)
      * constructor
      * destructor
      * operator=
      * Iterators
      * Capacity
      * Element access
      * Modifiers
      * Operations
   * [hash_map](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/hash_map.h)
      * constructor
      * destructor
      * operator=
      * Iterators
      * Capacity
      * Element access
      * Modifiers
      * Operations
   * [hash_set](https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/hash_set.h)
      * constructor
      * destructor
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
      * Operations
         * find
         * count
         * equal_range
         * swap
      * Buckets
         * reserve
         * bucker_count
         * max_bucket_count
         * elems_in_bucket
##算法


##配接器

