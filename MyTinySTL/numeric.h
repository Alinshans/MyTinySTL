#ifndef MYTINYSTL_NUMERIC_H_
#define MYTINYSTL_NUMERIC_H_

// 这个头文件包含了 mystl 的数值算法

#include "iterator.h"

namespace mystl
{

/*****************************************************************************************/
// accumulate
// 版本1：以初值 init 对每个元素进行累加
// 版本2：以初值 init 对每个元素进行二元操作
/*****************************************************************************************/
// 版本1
template <class InputIter, class T>
T accumulate(InputIter first, InputIter last, T init)
{
  for (; first != last; ++first)
  {
    init += *first;
  }
  return init;
}

// 版本2
template <class InputIter, class T, class BinaryOp>
T accumulate(InputIter first, InputIter last, T init, BinaryOp binary_op)
{
  for (; first != last; ++first)
  {
    init = binary_op(init, *first);
  }
  return init;
}

/*****************************************************************************************/
// adjacent_difference
// 版本1：计算相邻元素的差值，结果保存到以 result 为起始的区间上
// 版本2：自定义相邻元素的二元操作
/*****************************************************************************************/
// 版本1
template <class InputIter, class OutputIter>
OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result)
{
  if (first == last)  return result;
  *result = *first;  // 记录第一个元素
  auto value = *first;
  while (++first != last)
  {
    auto tmp = *first;
    *++result = tmp - value;
    value = tmp;
  }
  return ++result;
}

// 版本2
template <class InputIter, class OutputIter, class BinaryOp>
OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result,
                               BinaryOp binary_op)
{
  if (first == last)  return result;
  *result = *first;  // 记录第一个元素
  auto value = *first;
  while (++first != last)
  {
    auto tmp = *first;
    *++result = binary_op(tmp, value);
    value = tmp;
  }
  return ++result;
}

/*****************************************************************************************/
// inner_product
// 版本1：以 init 为初值，计算两个区间的内积   
// 版本2：自定义 operator+ 和 operator*
/*****************************************************************************************/
// 版本1
template <class InputIter1, class InputIter2, class T>
T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init)
{
  for (; first1 != last1; ++first1, ++first2)
  {
    init = init + (*first1 * *first2);
  }
  return init;
}

// 版本2
template <class InputIter1, class InputIter2, class T, class BinaryOp1, class BinaryOp2>
T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init,
                BinaryOp1 binary_op1, BinaryOp2 binary_op2)
{
  for (; first1 != last1; ++first1, ++first2)
  {
    init = binary_op1(init, binary_op2(*first1, *first2));
  }
  return init;
}

/*****************************************************************************************/
// iota
// 填充[first, last)，以 value 为初值开始递增
/*****************************************************************************************/
template <class ForwardIter, class T>
void iota(ForwardIter first, ForwardIter last, T value)
{
  while (first != last)
  {
    *first++ = value;
    ++value;
  }
}

/*****************************************************************************************/
// partial_sum
// 版本1：计算局部累计求和，结果保存到以 result 为起始的区间上
// 版本2：进行局部进行自定义二元操作
/*****************************************************************************************/
template <class InputIter, class OutputIter>
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result)
{
  if (first == last)  return result;
  *result = *first;  // 记录第一个元素
  auto value = *first;
  while (++first != last)
  {
    value = value + *first;
    *++result = value;
  }
  return ++result;
}

// 版本2
template <class InputIter, class OutputIter, class BinaryOp>
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result,
                       BinaryOp binary_op)
{
  if (first == last)  return result;
  *result = *first;  //记录第一个元素
  auto value = *first;
  while (++first != last)
  {
    value = binary_op(value, *first);
    *++result = value;
  }
  return ++result;
}

} // namespace mystl
#endif // !MYTINYSTL_NUMERIC_H_

