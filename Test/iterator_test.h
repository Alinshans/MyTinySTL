#ifndef MYTINYSTL_ITERATOR_TEST_H_
#define MYTINYSTL_ITERATOR_TEST_H_

#include "test.h"
#include "../MyTinySTL/iterator.h"
#include "../MyTinySTL/stream_iterator.h"

namespace mystl
{
namespace test
{
namespace iterator_test
{

void stream_iterator_test()
{
  std::cout << "[===============================================================]\n";
  std::cout << "[------------- Run iterator test : stream_iterator--------------]\n";
  std::cout << "[-------------------------- API test ---------------------------]\n";

  static_assert(mystl::is_exactly_input_iterator<mystl::istream_iterator<int>>::value, 
                "istream_iterator must have input_iterator_tag)");

  std::istringstream is("1 2 3");  
  mystl::istream_iterator<int> first{is}, last;   
  std::cout << mystl::distance(first, last) << '\n';

  std::istringstream istream("1 2 3 4 5 6");  
  mystl::istream_iterator<int> beg{istream}, end;  
  for (; beg != end; ++beg) {
    std::cout << *beg << " ";
  }
  std::cout << '\n';

  PASSED;
}

} // namespace stream_iterator_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_STREAM_ITERATOR_TEST_H_

