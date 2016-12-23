#ifndef MYTINYSTL_HEAP_ALGO_H_
#define MYTINYSTL_HEAP_ALGO_H_

// 这个头文件包含 heap 的四个算法 : push_heap, pop_heap, sort_heap, make_heap

#include "iterator.h"

namespace mystl {

/*****************************************************************************************/
// push_heap
// 该函数接受两个迭代器，表示一个 heap 容器的首尾，并且新元素已经插入到底部容器的最尾端，调整 heap
/*****************************************************************************************/
template <class RandomAccessIterator, class Distance, class T>
void __push_heap_aux(RandomAccessIterator first, Distance holeIndex, 
                     Distance topIndex, T value) {
    auto parent = (holeIndex - 1) / 2;             // 父节点与根节点的距离
    while (holeIndex > topIndex && *(first + parent) < value) {
        // 未到达根节点且父节点的值小于插入的值
        // 使用 operator<，所以 heap 为 max-heap
        *(first + holeIndex) = *(first + parent);  // 洞值为父节点的值
        holeIndex = parent;                        // 调整洞号，上升至父节点
        parent = (holeIndex - 1) / 2;              // 新洞号的父节点
    }
    *(first + holeIndex) = value;                  // 最后令洞值等于新值，调整完毕
}

template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap(RandomAccessIterator first, RandomAccessIterator last, 
                        Distance*, T*) {
    mystl::__push_heap_aux(first, (last - first) - 1, 
                           static_cast<Distance>(0), *(last - 1));
}

template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    // 新元素应该已置于底部容器的最尾端
    mystl::__push_heap(first, last, distance_type(first), value_type(first));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomAccessIterator, class Distance, class T, class Compared>
void __push_heap_aux(RandomAccessIterator first, Distance holeIndex,
    Distance topIndex, T value, Compared comp) {
    auto parent = (holeIndex - 1) / 2;             // 父节点与根节点的距离
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);  // 洞值为父节点的值
        holeIndex = parent;                        // 调整洞号，上升至父节点
        parent = (holeIndex - 1) / 2;              // 新洞号的父节点
    }
    *(first + holeIndex) = value;                  // 最后令洞值等于新值，调整完毕
}

template <class RandomAccessIterator, class Compared, class Distance, class T>
inline void __push_heap(RandomAccessIterator first, RandomAccessIterator last,
                        Distance*, T*, Compared comp) {
    mystl::__push_heap_aux(first, (last - first) - 1, 
                           static_cast<Distance>(0), *(last - 1), comp);
}

template <class RandomAccessIterator, class Compared>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, 
                      Compared comp) {
    mystl::__push_heap(first, last, distance_type(first), value_type(first), comp);
}

/*****************************************************************************************/
// pop_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，将 heap 的根节点取出放到容器尾部，调整 heap
/*****************************************************************************************/
template <class RandomAccessIterator, class T, class Distance>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
    // 先进行下溯(percolate down)过程
    auto topIndex = holeIndex;
    auto rchild = 2 * holeIndex + 2;  // 洞节点的右子节点
    while (rchild < len) {
        // 比较洞节点左右孩子的值，令较大的为 rchild
        if (*(first + rchild) < *(first + rchild - 1))  --rchild;
        // 令较大的值为洞值，然后洞号下调至较大节点
        *(first + holeIndex) = *(first + rchild);
        holeIndex = rchild;
        rchild = 2 * (rchild + 1);    // 找到新的右子节点
    }
    if (rchild == len) {              // 如果没有右子节点
        *(first + holeIndex) = *(first + (rchild - 1));
        holeIndex = rchild - 1;
    }
    // 再执行一次上溯(percolate up)过程
    mystl::__push_heap_aux(first, holeIndex, topIndex, value);
}

template <class RandomAccessIterator, class T, class Distance>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last,
                           RandomAccessIterator result, T value, Distance*) {
    *result = *first;  // 先将尾指设置成首值，即尾指为欲求结果
    mystl::__adjust_heap(first, static_cast<Distance>(0), last - first, value);
}

template <class RandomAccessIterator, class T>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, T*) {
    // 先将首值调至尾节点，然后调整[first, last - 1)使之重新成为一个 max-heap
    mystl::__pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first));
}

template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    mystl::__pop_heap(first, last, value_type(first));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomAccessIterator, class T, class Distance, class Compared>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex,
    Distance len, T value, Compared comp) {
    // 先进行下溯(percolate down)过程
    auto topIndex = holeIndex;
    auto rchild = 2 * holeIndex + 2;  // 洞节点的右子节点
    while (rchild < len) {
        if (comp(*(first + rchild), *(first + rchild - 1)))  --rchild;
        *(first + holeIndex) = *(first + rchild);
        holeIndex = rchild;
        rchild = 2 * (rchild + 1);    // 找到新的右子节点
    }
    if (rchild == len) {              // 如果没有右子节点
        *(first + holeIndex) = *(first + (rchild - 1));
        holeIndex = rchild - 1;
    }
    // 再执行一次上溯(percolate up)过程
    mystl::__push_heap_aux(first, holeIndex, topIndex, value, comp);
}

template <class RandomAccessIterator, class T, class Distance, class Compared>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last,
                           RandomAccessIterator result, T value, Distance*, 
                           Compared comp) {
    *result = *first;  // 先将尾指设置成首值，即尾指为欲求结果
    mystl::__adjust_heap(first, static_cast<Distance>(0), last - first, value, comp);
}

template <class RandomAccessIterator, class T, class Compared>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
                       T*, Compared comp) {
    mystl::__pop_heap_aux(first, last - 1, last - 1, *(last - 1),
                          distance_type(first), comp);
}

template <class RandomAccessIterator, class Compared>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, 
                     Compared comp) {
    mystl::__pop_heap(first, last, value_type(first), comp);
}

/*****************************************************************************************/
// sort_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，不断执行 pop_heap 操作，直到首尾最多相差1
/*****************************************************************************************/
template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
    // 每执行一次 pop_heap，最大的元素都被放到尾部，直到容器最多只有一个元素，完成排序
    while (last - first > 1) {
        mystl::pop_heap(first, last--);
    }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomAccessIterator, class Compared>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compared comp) {
    while (last - first > 1) {
        mystl::pop_heap(first, last--, comp);
    }
}

/*****************************************************************************************/
// make_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，把容器内的数据变为一个 heap
/*****************************************************************************************/
template <class RandomAccessIterator, class T, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
    if (last - first < 2)  return;    // 长度为0或1时不需要重排
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;   // 第一个需要重排的子树头部节点
    while (true) {
        // 重排以 holeIndex 为首的子树
        mystl::__adjust_heap(first, holeIndex, len, static_cast<T>(*(first + holeIndex)));
        if (holeIndex == 0)  return;  // 走完根节点
        holeIndex--;                  // 向前移一个节点
    }
}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
    mystl::__make_heap(first, last, value_type(first), distance_type(first));;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomAccessIterator, class T, class Distance, class Compared>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last,
    T*, Distance*, Compared comp) {
    if (last - first < 2)  return;    // 长度为0或1时不需要重排
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;   // 第一个需要重排的子树头部节点
    while (true) {
        // 重排以 holeIndex 为首的子树
        mystl::__adjust_heap(first, holeIndex, len, 
                             static_cast<T>(*(first + holeIndex)), comp);
        if (holeIndex == 0)  return;  // 走完根节点
        holeIndex--;                  // 向前移一个节点
    }
}

template <class RandomAccessIterator, class Compared>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last,
                      Compared comp) {
    mystl::__make_heap(first, last, value_type(first), distance_type(first), comp);
}

} // namespace mystl
#endif // !MYTINYSTL_HEAP_ALGO_H_

