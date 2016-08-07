#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>	//for malloc/free
#include <iostream>	//for cerr
namespace MyTinySTL {

#if !defined(_THROW_BAD_ALLOC)
#define _THROW_BAD_ALLOC	std::cerr<<"out of memory"<<std::endl;exit(1);


	//给alloc调用
	class _alloc {

	private:
		//oom: out of memory
		static void *oom_malloc(size_t);
		static void(*__oom_handler)();

	public:
		static void * allocate(size_t n) {
			void *result = malloc(n);	
			if (result == 0)	result = oom_malloc(n);	//内存不足时，调用oom_malloc
			return result;
		}

		static void deallocate(void *p, size_t n) {
			free(p);
		}

		//指定自己的out-of-memory handler
		static void(*set_malloc_handler(void(*f)()))(){
			void(*old)() = __oom_handler;
			__oom_handler = f;
			return old;
		}
	};

	void(*_alloc::__oom_handler)() = 0;
	void * _alloc::oom_malloc(size_t n) {
		void(*my_malloc_handler)();
		void * result;

		for (;;) {	//不断尝试释放、配置、再释放、再配置……
			my_malloc_handler = __oom_handler;
			if (my_malloc_handler == 0) { _THROW_BAD_ALLOC; }
			(*my_malloc_handler)();	//调用处理例程，企图释放内存
			result = malloc(n);	//再次尝试分配内存
			if (result)	return result;
		}
	}
#endif

	//内部空间配置器
	class alloc {

	private:
		enum { _ALIGN = 8 };	//	小型区块上调边界
		enum { _MAX_BYTES = 128 };	//	小型区块上限
		enum { _NFREELISTS = 16 };	//free-list 个数

	private:
		//将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + _ALIGN - 1) & ~(_ALIGN - 1));
		}

	private:
		//free-list 的节点结构
		union obj {
			union obj * free_list_link;	//指向下一个节点
			char data[1];	//指向本块内存，data表示本块内存的首地址
		};

	private:
		//16个free-list
		static obj * volatile free_list[_NFREELISTS];
		//根据区块大小，选择第 n 号 free_list 。 
		static size_t FREELIST_INDEX(size_t bytes) {
			return ((bytes + _ALIGN - 1) / (_ALIGN - 1));
		}
		//为free_list重新填充空间，返回一个大小为 n 的对象，可能加入大小为 n 的其它区块到free_list
		static void* refill(size_t n);
		//配置 nobj 个大小为 size 的区块，条件不允许时，nobj 可能会降低
		static char* chunk_alloc(size_t size, int &nobj);

		static char* start_free;	//内存池起始位置
		static char* end_free;	//内存池结束位置
		static size_t heap_size;

	public:
		static void * allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void * reallocate(void *p, size_t old_size, size_t new_size);
	};

	//静态成员变量初值设定
	char* alloc::start_free = 0;
	char* alloc::end_free = 0;
	size_t alloc::heap_size = 0;
	alloc::obj * volatile alloc::free_list[_NFREELISTS] =
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	//空间配置函数 ， n > 0
	void* alloc::allocate(size_t n) {
		obj * volatile * my_free_list;
		obj * result;

		//大于128 bytes 就调用 _alloc::allocate
		if (n > (size_t)_MAX_BYTES)
			return _alloc::allocate(n);

		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (result == 0) {	//当前无可用区块，调用refill()
			void* r = refill(ROUND_UP(n));
			return r;
		}

		*my_free_list = result->free_list_link;
		return result;
	}

	//空间释放函数 , p 不能为 0
	void alloc::deallocate(void *p, size_t n) {
		obj *q = (obj *)p;
		obj * volatile * my_free_list;

		//大于128 bytes 就调用 _alloc::deallocate
		if (n > (size_t)_NFREELISTS) {
			_alloc::deallocate(p, n);
			return;
		}

		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}

	//重新配置空间函数
	void* alloc::reallocate(void *p, size_t old_size, size_t new_size) {
		deallocate(p, old_size);
		p = allocate(new_size);
		return p;
	}

	//重新填充free_list
	//返回大小为 n 的对象，有时会适当为free_list增加节点
	void* alloc::refill(size_t n) {
		int nobj = 20;

		//调用 chunk_alloc(),尝试取得 nobj 个区块作为 free_lists 的新节点
		char *c = chunk_alloc(n, nobj);
		obj * volatile * my_free_list;
		obj * result, *cur_obj, *nex_obj;
		
		//如果只有一个区块，就把这个区块返回给调用者，free_list没有增加新节点
		if (1 == nobj)	return c;

		//否则把一个区块给调用者，剩下的纳入free_lists作为新节点
		my_free_list = free_list + FREELIST_INDEX(n);
		result = (obj*)c;	//这一块返回给调用者
		*my_free_list = nex_obj = (obj*)(c + n);
		//将新增节点串联起来
		for (int i = 1; i < nobj - 1; i++) {
			cur_obj = nex_obj;
			nex_obj = (obj*)((char*)nex_obj + n);
			cur_obj->free_list_link = nex_obj;
		}
		cur_obj->free_list_link = 0;
		return result;
	}

	//从内存池中取空间给free_list使用
	char* alloc::chunk_alloc(size_t size, int &nobj) {
		char * result;

		size_t need_bytes = size * nobj;	//需要分配的大小
		size_t pool_bytes = end_free - start_free;	//内存池剩余大小

		//内存池剩余大小完全满足需求量
		if (pool_bytes >= need_bytes) {
			result = start_free;
			start_free += need_bytes;
			return result;
		}

		//内存池剩余大小不能完全满足需求量，但至少可以分配一个或一个以上的区块
		else if (pool_bytes >= size) {
			nobj = pool_bytes / size;	//调整返回的区块数
			need_bytes = size * nobj;
			result = start_free;
			start_free += need_bytes;
			return result;
		}

		//内存池剩余大小连一个区块都无法满足
		else {
			//如果内存池还有剩余，把剩余的空间加入到free_list中
			if (pool_bytes > 0) {
				obj * volatile * my_free_list = free_list + FREELIST_INDEX(pool_bytes);
				//把内存池的残余空间纳入free_list
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}

			//申请大小为2 * 需求量 + 附加量
			//附加量随着配置次数的增多而增大
			size_t bytes_to_get = 2 * need_bytes + ROUND_UP(heap_size >> 4);
			start_free = (char*)malloc(bytes_to_get);
			if (start_free == 0) {
				//heap空间不足
				obj * volatile * my_free_list, *p;
				//试着查找有无尚有未有区块，且区块足够大的free_list
				for (int i = size; i <= _MAX_BYTES; i += _ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0) {
						//free_list有未用区块，调整free_list，释放区块
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						//递归调用自己，为了修正nobj
						return chunk_alloc(size, nobj);
					}

					//到处都没有内存可用了
					end_free = 0;
					//调用 _alloc 中的 oom 机制，看看能否尽点力
					start_free = (char *)_alloc::allocate(bytes_to_get);
				}
			}
			end_free = start_free + bytes_to_get;
			heap_size += bytes_to_get;
			//递归调用自己，为了修正nobj
			return chunk_alloc(size, nobj);
		}
	}
}
#endif // !ALLOC_H

