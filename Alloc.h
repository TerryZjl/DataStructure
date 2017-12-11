#ifndef _ALLOC_H_
#define _ALLOC_H_

#include<iostream>
using namespace std;


/**********一级空间配置器*****************/
template<class inst>
class __MallocAllocTemplate
{
public:
	static void* Allocate(size_t n)
	{
		void* ret = malloc(n);
		if (ret == NULL)
		{
			ret = OomMalloc(n);
		}
		return ret;
	}
	static void Deallocate(size_t n)
	{
		free(n);
	}
private:
	static void *OomMalloc(size_t);   //内存不足时的处理函数
	static void(*__MallocALLocOomHandler)(); // 回调处理函数
};

template<class inst>
void(*__MallocAllocTemplate<inst>::__MallocALLocOomHandler)() = 0;


template<class inst>
void* __MallocAllocTemplate<inst>::OomMalloc(size_t n)
{
	void(*MyHandler)();
	void *ret;

	MyHandler = __MallocALLocOomHandler;
	while (1)
	{
		if (MyHandler == 0)
		{
			throw("内存申请失败");
		}
		(*MyHandler)();   //如果设置了回调处理函数，就执行这个函数，然后直到申请成功。
		ret = malloc(n);
		if (ret)
			return ret;
	}
}

void TestOneAllocate()
{
	__MallocAllocTemplate<int>::Allocate(0x7fffffff);
}



/***************二级空间配置器****************/
template<class thread, class inst>
class __DefaultAllocTemplate
{
private:
	enum 
	{
		__ALIGN = 8,
		__MAXBYTES = 128,
		__NFREELISTS = __MAXBYTES / __ALIGN
	};

	union Obj
	{
		union Obj* _FreeListNext;
	};


	typedef typename __MallocAllocTemplate <inst> MALLOC;
public:

    static void* Allocate(size_t n)
	{
		if (n > __MAXBYTES)
		{
			return MALLOC::Allocate(n);
		}
		size_t index = GetFreeListIndex(n);
		Obj* result = FreeList[index];
		if (result == NULL)
		{
			size_t num = RoundUp(n);
			void * ret = Refill(num); 
			return ret;
		}
		FreeList[index] = result->_FreeListNext;
		return result;
	}
private:
	static void* ChunkAlloc(size_t n, size_t& nobjs); //
	static void* Refill(size_t n);              //去内存池里面切想要大小的内存
	static size_t RoundUp(size_t n);            //把要申请的字节数向上补齐到__ALIGN的倍数
	static size_t GetFreeListIndex(size_t n);   //取出n映射到自由链表的下标位置
	static Obj* FreeList[__NFREELISTS] = { 0 };
private:
	static char* StartFree;
	static char* EndFree;
	static size_t HeapSize;

};
template<class thread, class inst>
static void* __DefaultAllocTemplate<thread, inst>::ChunkAlloc(size_t n, size_t& nobjs)
{}

template<class thread, class inst>
static void* __DefaultAllocTemplate<thread, inst>::Refill(size_t n)
{
	size_t  nobjs = 20; // 设置一次切20块n大小的内存块挂到自由连上
	void *result = ChunkAlloc(n, nobjs);

	if (nobjs == 1)
	{
		return result;
	}

	size_t index = GetFreeListIndex(n);
	Obj* cur = (Obj*)((char*)result + __ALIGN)；
	for (size_t i = 0; i < nobjs - 1; i++)
	{
		Obj* tmp = cur;
		tmp = FreeList[index]->_FreeListNext;
		FreeList[index] = tmp; 
		cur = (Obj*)((char*)cur + __ALIGN);
	}
}


template<class thread, class inst>
static size_t __DefaultAllocTemplate<thread, inst>::GetFreeListIndex(size_t n)
{
	return (n + __ALIGN - 1) /__ALIGN -1;
}

template<class thread, class inst>
static size_t __DefaultAllocTemplate<thread, inst>::RoundUp(size_t n)
{
	return (n + __ALIGN - 1) &(~(__ALIGN-1));
}

#endif