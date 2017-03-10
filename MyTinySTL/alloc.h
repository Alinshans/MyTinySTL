#ifndef MYTINYSTL_ALLOC_H_
#define MYTINYSTL_ALLOC_H_

// 这个头文件包含一个类 alloc，代表 mystl 的默认空间配置器

#include <cstdlib>
#include <cstddef>
#include <iostream>

namespace mystl
{

// 共用体: FreeList
// 采用链表的方式管理内存碎片，分配与回收小内存（<=4K）区块
union FreeList
{
  union FreeList* next;  // 指向下一个区块
  char data[1];          // 储存本块内存的首地址
};

// 不同内存范围的上调大小
enum
{
  EAlign128 = 8, EAlign256 = 16, EAlign512 = 32,
  EAlign1K = 64, EAlign2K = 128, EAlign4K = 256
};

// 小对象的内存大小
enum { ESmallObjectBytes = 4096 };

// free lists 个数
enum { EFreeListsNumber = 56 };

// 空间配置类 alloc
// 如果内存较大，超过 4K，直接调用 malloc, free
// 当内存较小时，以内存池管理，每次配置一大块内存，并维护对应的自由链表
class alloc
{
private:
  static char*  start_free;                      // 内存池起始位置
  static char*  end_free;                        // 内存池结束位置
  static size_t heap_size;                       // 申请 heap 空间附加值大小

  static FreeList* free_list[EFreeListsNumber];  // 自由链表

public:
  static void* allocate(size_t n);
  static void  deallocate(void* p, size_t n);
  static void* reallocate(void* p, size_t old_size, size_t new_size);

private:
  static size_t align_bytes(size_t bytes);
  static size_t round_up(size_t bytes);
  static size_t freelist_index(size_t bytes);
  static size_t get_blocks(size_t bytes);
  static void*  refill(size_t n);
  static char*  chunk_alloc(size_t size, size_t &nobj);
};

// 静态成员变量初始化
char*  alloc::start_free = nullptr;
char*  alloc::end_free = nullptr;
size_t alloc::heap_size = 0;
FreeList* alloc::free_list[EFreeListsNumber] = {
  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr, };

// 分配大小为 n 的空间， n > 0
void* alloc::allocate(size_t n)
{
  FreeList* my_free_list;
  FreeList* result;
  if (n > static_cast<size_t>(ESmallObjectBytes))
    return std::malloc(n);
  my_free_list = free_list[freelist_index(n)];
  result = my_free_list;
  if (result == nullptr)
  {
    void* r = refill(round_up(n));
    return r;
  }
  my_free_list = result->next;
  return result;
}

// 释放 p 指向的大小为 n 的空间, p 不能为 0
void alloc::deallocate(void* p, size_t n)
{
  if (n > static_cast<size_t>(ESmallObjectBytes))
  {
    std::free(p);
    return;
  }
  FreeList* q = (FreeList*)p;
  FreeList* my_free_list;
  my_free_list = free_list[freelist_index(n)];
  q->next = my_free_list;
  my_free_list = q;
}

// 重新分配空间，接受三个参数，参数一为指向新空间的指针，参数二为原来空间的大小，参数三为申请空间的大小
void* alloc::reallocate(void* p, size_t old_size, size_t new_size)
{
  deallocate(p, old_size);
  p = allocate(new_size);
  return p;
}

// bytes 对应上调大小
inline size_t alloc::align_bytes(size_t bytes)
{
  if (bytes <= 128)
  {
    return EAlign128;
  }
  else if (bytes <= 256)
  {
    return EAlign256;
  }
  else if (bytes <= 512)
  {
    return EAlign512;
  }
  else if (bytes <= 1024)
  {
    return EAlign1K;
  }
  else if (bytes <= 2048)
  {
    return EAlign2K;
  }
  else
  {
    return EAlign4K;
  }
}

// 将 bytes 上调至对应区间大小
inline size_t alloc::round_up(size_t bytes)
{
  return ((bytes + align_bytes(bytes) - 1) & ~(align_bytes(bytes) - 1));
}

// 根据区块大小，选择第 n 个 free lists
inline size_t alloc::freelist_index(size_t bytes)
{
  if (bytes <= 128)
  {
    return ((bytes + EAlign128 - 1) / EAlign128 - 1);
  }
  else if (bytes <= 256)
  {
    return (15 + (bytes + EAlign256 - 129) / EAlign256);
  }
  else if (bytes <= 512)
  {
    return (23 + (bytes + EAlign512 - 257) / EAlign512);
  }
  else if (bytes <= 1024)
  {
    return (31 + (bytes + EAlign1K - 513) / EAlign1K);
  }
  else if (bytes <= 2048)
  {
    return (39 + (bytes + EAlign2K - 1025) / EAlign2K);
  }
  else
  {
    return (47 + (bytes + EAlign4K - 2049) / EAlign4K);
  }
}

// 根据大小获取区块数目
inline size_t alloc::get_blocks(size_t bytes)
{
  if (bytes <= 128)
  {
    return 8;
  }
  else if (bytes <= 256)
  {
    return 4;
  }
  /*else if (bytes <= 512) {
  return 2;
  }*/
  else if (bytes <= 1024)
  {
    return 2;
  }
  /*else if (bytes <= 2048) {
  return 1;
  }*/
  else
  {
    return 1;
  }
}

// 重新填充 free list
void* alloc::refill(size_t n)
{
  size_t nblock = get_blocks(n);
  char* c = chunk_alloc(n, nblock);
  FreeList* my_free_list;
  FreeList* result, *cur, *next;
  // 如果只有一个区块，就把这个区块返回给调用者，free list 没有增加新节点
  if (nblock == 1)
    return c;
  // 否则把一个区块给调用者，剩下的纳入 free list 作为新节点
  my_free_list = free_list[freelist_index(n)];
  result = (FreeList*)c;
  my_free_list = next = (FreeList*)(c + n);
  for (size_t i = 1; ; ++i)
  {
    cur = next;
    next = (FreeList*)((char*)next + n);
    if (nblock - 1 == i)
    {
      cur->next = nullptr;
      break;
    }
    else
    {
      cur->next = next;
    }
  }
  return result;
}

// 从内存池中取空间给 free list 使用，条件不允许时，会调整 nblock
char* alloc::chunk_alloc(size_t size, size_t& nblock)
{
  char* result;
  size_t need_bytes = size * nblock;
  size_t pool_bytes = end_free - start_free;
  // 如果内存池剩余大小完全满足需求量，返回它
  if (pool_bytes >= need_bytes)
  {
    result = start_free;
    start_free += need_bytes;
    return result;
  }
  // 如果内存池剩余大小不能完全满足需求量，但至少可以分配一个或一个以上的区块，就返回它
  else if (pool_bytes >= size)
  {
    nblock = pool_bytes / size;
    need_bytes = size * nblock;
    result = start_free;
    start_free += need_bytes;
    return result;
  }
  // 如果内存池剩余大小连一个区块都无法满足
  else
  {
    if (pool_bytes > 0)
    {  // 如果内存池还有剩余，把剩余的空间加入到 free list 中
      FreeList* my_free_list = free_list[freelist_index(pool_bytes)];
      ((FreeList*)start_free)->next = my_free_list;
      my_free_list = (FreeList*)start_free;
    }
    // 申请 heap 空间
    size_t bytes_to_get = (need_bytes << 1) + round_up(heap_size >> 4);
    start_free = (char*)std::malloc(bytes_to_get);
    if (!start_free)
    {
      FreeList* my_free_list, *p;
      // 试着查找有无未用区块，且区块足够大的 free list
      for (auto i = size; i <= ESmallObjectBytes; i += align_bytes(i))
      {
        my_free_list = free_list[freelist_index(i)];
        p = my_free_list;
        if (p)
        {
          my_free_list = p->next;
          start_free = (char*)p;
          end_free = start_free + i;
          return chunk_alloc(size, nblock);
        }
        end_free = nullptr;
        std::cerr << "out of memory" << std::endl;
        std::exit(1);
      }
    }
    end_free = start_free + bytes_to_get;
    heap_size += bytes_to_get;
    return chunk_alloc(size, nblock);
  }
}

} // namespace mystl
#endif // !MYTINYSTL_ALLOC_H_

