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

			void * ret = Refill(n);
			return ret;
		}
		FreeList[index] = result->_FreeListNext;
		return result;
	}
private:
	static size_t RoundUp(size_t n);
	static size_t GetFreeListIndex(size_t n);
	static Obj* FreeList[__NFREELISTS] = { 0 };
private:
	static char* StartFree;
	static char* EndFree;
	static size_t HeapSize;

};

template<class thread, class inst>
static size_t __DefaultAllocTemplate<thread, inst>::GetFreeListIndex(size_t n)
{
	return (n + __ALIGN - 1) /__ALIGN -1;
}


#endif