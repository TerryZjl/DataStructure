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
	static void Deallocate(void* p,size_t n)
	{
		free(p);
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
	static void Deallocate(void* p, size_t n)
	{
		Obj * del = (Obj*)p;
		Obj* MyFreeList;

		if (n > __MAXBYTES)
		{
			return MALLOC::Deallocate(p,n);
		}
		
		MyFreeList = FreeList[GetFreeListIndex(n)];
		//多线程需要加锁
		del->_FreeListNext = *MyFreeList;
		*MyFreeList = del;
	}
private:
	static void* ChunkAlloc(size_t n, size_t& nobjs); //
	static void* Refill(size_t n);              //去内存池里面切想要大小的内存
	static size_t RoundUp(size_t n);            //把要申请的字节数向上补齐到__ALIGN的倍数
	static size_t GetFreeListIndex(size_t n);   //取出n映射到自由链表的下标位置
private:
	static Obj* FreeList[__NFREELISTS];

	static char* StartFree;
	static char* EndFree;
	static size_t HeapSize; //当前内存池的大小

};
template<class thread, class inst>
char* __DefaultAllocTemplate<thread, inst>::FreeList[__NFREELISTS] = { 0 };
template<class thread, class inst>
char* __DefaultAllocTemplate<thread, inst>::StartFree = NULL;
template<class thread, class inst>
char* __DefaultAllocTemplate<thread, inst>::EndFree = NULL;
template<class thread, class inst>
char* __DefaultAllocTemplate<thread, inst>::HeapSize = 0;


template<class thread, class inst>
static void* __DefaultAllocTemplate<thread, inst>::ChunkAlloc(size_t n, size_t& nobjs)
{
	size_t SizeBytes = EndFree - StartFree;
	size_t total = n*nobjs;
	void *result;
	Obj* MyFreeList;
	if (SizeBytes >= total)  //内存池里的字节足够申请nobjs个n，去内存池里面切
	{
		result = StartFree;
		StartFree = StartFree + total;
		return result;
	}
	else if (SizeBytes >= n) // 内存池里的字节只够切1个及以上个n字节内存。
	{
		nobjs = SizeBytes % n;
		result = StartFree;
		StartFree = StartFree + n*nobjs;
		return result;
	}
	else // 内存池里面一个n字节大小的内存都没有
	{
		if (total > 0)
		{
			size_t index = GetFreeListIndex(SizeBytes);
			MyFreeList = FreeList[index];
			((Obj*)StartFree)->_FreeListNext = *MyFreeList;
			*MyFreeList = (Obj*)StartFree;
			StartFree = NULL;
		}
		
		size_t GetNBytes = total + (HeapSize >> 4);
		StartFree = (char*)malloc(GetNBytes);
		if (StartFree == NULL)
		{
			// 去更大的自由链表里找内存
			size_t i = total + __ALIGN;
			for (; i < __MAXBYTES; i += __ALIGN)
			{
				MyFreeList = FreeList[GetFreeListIndex(i)];
				if (MyFreeList)
				{
					StartFree = (char*)MyFreeList;
					EndFree = StartFree + i;
					return  __DefaultAllocTemplate<thread, inst>::ChunkAlloc(n, nobjs);
				}
			}
			StartFree = (char*)MALLOC::Allocate(GetNBytes);
		}

		HeapSize += GetNBytes;
		EndFree = StartFree + GetNBytes;
		return __DefaultAllocTemplate<thread, inst>::ChunkAlloc(n, nobjs);
	}
}

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
	return result;
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

template<class T, class Alloc>
class simple_alloc {

public:
	static T *Allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::Allocate(n * sizeof (T));
	}
	static T *Allocate(void)
	{
		return (T*)Alloc:Allocate(sizeof (T));
	}
	static void Deallocate(T *p, size_t n)
	{
		if (0 != n) Alloc::Deallocate(p, n * sizeof (T));
	}
	static void Deallocate(T *p)
	{
		Alloc::Deallocate(p, sizeof (T));
	}
};
#endif