#ifndef MYTINYSTL_ALLOC_H_
#define MYTINYSTL_ALLOC_H_

// 这个头文件包含一个类 alloc，代表 mystl 默认的空间配置器

#include <cstdlib>
#include <iostream>

namespace mystl {

// 共用体: FreeList
// 采用链表的方式管理内存碎片，分配与回收小内存区块
// 维护16个自由链表，分别管理大小为8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128 bytes 的区块
union FreeList {
    union FreeList* next;  // 指向下一个区块
    char data[1];          // 指向本块内存，储存本块内存的首地址
};

enum { EAlign = 8 };        // 小型区块上调边界
enum { EMaxBytes = 128 };   // 小型区块上限
enum { ENFreeLists = 16 };  // free list 个数

// 空间配置类 alloc
// 如果内存较大，超过 128 bytes，直接调用 malloc, free
// 当内存较小时，以内存池管理，每次配置一大块内存，并维护对应的自由链表
class alloc {
private:
    static char*  start_free;  // 内存池起始位置
    static char*  end_free;    // 内存池结束位置
    static size_t heap_size;   // 申请 heap 空间附加值大小

    static FreeList* volatile free_list[ENFreeLists];  // 16个自由链表

public:
    static void* allocate(size_t n);
    static void  deallocate(void* p, size_t n);
    static void* reallocate(void* p, size_t old_size, size_t new_size);

private:
    static size_t round_up(size_t bytes);
    static size_t freelist_index(size_t bytes);
    static void*  refill(size_t n);
    static char*  chunk_alloc(size_t size, int &nobj);
};

// alloc 静态成员变量初值设定
char*  alloc::start_free = nullptr;
char*  alloc::end_free = nullptr;
size_t alloc::heap_size = 0;
FreeList* volatile alloc::free_list[ENFreeLists] =
    { nullptr,nullptr,nullptr,nullptr,
      nullptr,nullptr,nullptr,nullptr,
      nullptr,nullptr,nullptr,nullptr,
      nullptr,nullptr,nullptr,nullptr };

// 分配大小为 n 的空间， n > 0
void* alloc::allocate(size_t n) {
    FreeList* volatile* my_free_list;
    FreeList* result;
    // 大于128 bytes 就调用 malloc
    if (n > (size_t)EMaxBytes)
        return malloc(n);
    my_free_list = free_list + freelist_index(n);
    result = *my_free_list;
    if (result == 0) {  // 如果当前无可用区块，调用 refill
        void* r = refill(round_up(n));
        return r;
    }
    *my_free_list = result->next;
    return result;
}

// 释放 p 指向的大小为 n 的空间, p 不能为 0
void alloc::deallocate(void* p, size_t n) {
    // 大于128 bytes 就调用 free
    if (n > static_cast<size_t>(EMaxBytes)) {
        free(p);
        return;
    }
    FreeList* q = static_cast<FreeList*>(p);
    FreeList* volatile* my_free_list;
    my_free_list = free_list + freelist_index(n);  // 找到对应的自由链表
    q->next = *my_free_list;                       // 将空间回收到内存池
    *my_free_list = q;
}

// 重新分配空间，接受三个参数，参数1 为指向新空间的指针，参数2 为原来空间的大小，参数3 为申请空间的大小
void* alloc::reallocate(void* p, size_t old_size, size_t new_size) {
    deallocate(p, old_size);  // 释放原来的空间
    p = allocate(new_size);   // 分配新的空间
    return p;
}

// 将 bytes 上调至 8 的倍数
size_t alloc::round_up(size_t bytes) {
    return ((bytes + EAlign - 1) & ~(EAlign - 1));
}

// 根据区块大小，选择第 n 个 free list 
size_t alloc::freelist_index(size_t bytes) {
    return ((bytes + EAlign - 1) / EAlign - 1);
}

// 重新填充 free list
// 返回大小为 n 的对象，有时会适当为 free list 增加节点
void* alloc::refill(size_t n) {
    int nobj = 20;  // 尝试获得区块的个数
    // 调用 chunk_alloc, 尝试取得 nobj 个区块作为 free list 的新节点
    char* c = chunk_alloc(n, nobj);
    FreeList* volatile* my_free_list;
    FreeList* result, *cur_obj, *next_obj;
    // 如果只有一个区块，就把这个区块返回给调用者，free list 没有增加新节点
    if (1 == nobj)    return c;
    // 否则把一个区块给调用者，剩下的纳入 free list 作为新节点
    my_free_list = free_list + freelist_index(n);
    result = reinterpret_cast<FreeList*>(c);  // 这一块返回给调用者
    *my_free_list = next_obj = reinterpret_cast<FreeList*>(c + n);
    for (auto i = 1; ; ++i) {                 // 将新增节点串联起来
        cur_obj = next_obj;
        next_obj = reinterpret_cast<FreeList*>(reinterpret_cast<char*>(next_obj) + n);
        if (nobj - 1 == i) {
            cur_obj->next = 0;
            break;
        }
        else {
            cur_obj->next = next_obj;
        }
    }
    return result;
}

// 从内存池中取空间给 free list 使用，条件不允许时，会调整 nobj
char* alloc::chunk_alloc(size_t size, int& nobj) {
    char* result;
    size_t need_bytes = size * nobj;            // 需要分配的大小
    size_t pool_bytes = end_free - start_free;  // 内存池剩余大小
    // 如果内存池剩余大小完全满足需求量，返回它
    if (pool_bytes >= need_bytes) {
        result = start_free;
        start_free += need_bytes;
        return result;
    }
    // 如果内存池剩余大小不能完全满足需求量，但至少可以分配一个或一个以上的区块，就返回它
    else if (pool_bytes >= size) {
        nobj = pool_bytes / size;  // 调整返回的区块数
        need_bytes = size * nobj;  // 调整分配的大小
        result = start_free;
        start_free += need_bytes;
        return result;
    }
    // 如果内存池剩余大小连一个区块都无法满足
    else {
        if (pool_bytes > 0) {  // 如果内存池还有剩余，把剩余的空间加入到 free list中
            FreeList* volatile* my_free_list = free_list + freelist_index(pool_bytes);
            // 把内存池的残余空间纳入free_list
            (reinterpret_cast<FreeList*>(start_free))->next = *my_free_list;
            *my_free_list = reinterpret_cast<FreeList*>(start_free);
        }
        // 在 heap 中申请大小为 2 * 需求量 + 附加量 的空间
        // 附加量随着配置次数的增多而增大
        size_t bytes_to_get = 2 * need_bytes + round_up(heap_size >> 4);
        start_free = static_cast<char*>(malloc(bytes_to_get));
        if (start_free == 0) {  // 如果 heap 空间不足
            FreeList* volatile* my_free_list, *p;
            // 试着查找有无未用区块，且区块足够大的 free list
            for (auto i = size; i <= EMaxBytes; i += EAlign) {
                my_free_list = free_list + freelist_index(i);
                p = *my_free_list;
                if (p != 0) {   // 如果 free list有未用区块，调整 free list，释放区块
                    *my_free_list = p->next;
                    start_free = reinterpret_cast<char*>(p);
                    end_free = start_free + i;
                    return chunk_alloc(size, nobj);  // 递归调用自己，修正 nobj
                }
                end_free = 0;  // 到处都没有内存可用了
                std::cerr << "out of memory" << std::endl; 
                std::exit(1); 
            }
        }
        end_free = start_free + bytes_to_get;
        heap_size += bytes_to_get;       // 修正附加量
        return chunk_alloc(size, nobj);  // 递归调用自己，修正 nobj
    }
}

} // namespace mystl
#endif // !MYTINYSTL_ALLOC_H_

