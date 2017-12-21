#ifndef _ALLOC_H_
#define _ALLOC_H_

#include<iostream>
#include<string>
using namespace std;

//#define __USE_MALLOC


/*****************trace************************/
#define __DEBUG__
static string GetFileName(const string& path)
{
	char ch = '/';
#ifdef _WIN32
	ch = '\\';
#endif
	size_t pos = path.rfind(ch);
	if (pos == string::npos)
		return path;
	else
		return path.substr(pos + 1);
}
//���ڵ���׷�ݵ�trace log
inline static void __trace_debug(const char* function,\
	const char* filename, int line, char* format, ...)
{
#ifdef __DEBUG__
	//������ú�������Ϣ
	fprintf(stdout, "��%s:%d��%s", GetFileName(filename).c_str(), line, function);
	//����û����trace��Ϣ
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
#endif
}
#define __TRACE_DEBUG(...) \
__trace_debug(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);

/**********һ���ռ�������*****************/
template<int inst>
class __MallocAllocTemplate
{
public:
	static void* Allocate(size_t n)
	{
		void* ret = malloc(n);
		__TRACE_DEBUG("һ���ռ�����������%dbytes�ڴ�\n", n);

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
	static bool (*SetOomHandler(bool(*f)()))()
	{
		bool (*Old)() = __MallocALLocOomHandler;
		__MallocALLocOomHandler = f;
		return Old;
	}
private:
	static void *OomMalloc(size_t);           //�ڴ治��ʱ�Ĵ�����
	static bool(*__MallocALLocOomHandler)();  // �û��Զ����ڴ�����ʧ�ܴ�����
};

template<int inst>
bool (*__MallocAllocTemplate<inst>::__MallocALLocOomHandler)() = 0;

template<int inst>
void* __MallocAllocTemplate<inst>::OomMalloc(size_t n)
{
	bool (*MyHandler)();
	void *ret;
	bool IsSetHandler = true;
	MyHandler = __MallocALLocOomHandler;
	while (1)
	{
		if ((MyHandler == 0) || IsSetHandler == false)
		{
			throw bad_alloc();
		}
		IsSetHandler = MyHandler();   //��������˻ص�����������ִ�����������Ȼ��ֱ������ɹ���
		ret = malloc(n);
		if (ret)
			return ret;
	}
}

bool DefinedOomhandler()  //�û��Զ��庯��
{
	cout << "�ڴ�ûӴ��" << endl;
	return false;
}

void TestOneAllocate()
{
	__MallocAllocTemplate<0>::SetOomHandler(&DefinedOomhandler);
	__MallocAllocTemplate<0>::Allocate(2147483646);
}

#ifdef __USE_MALLOC
	typedef __MallocAllocTemplate<0> Alloc;
#else

/***************�����ռ�������****************/
union Obj
{
	union Obj* _FreeListNext;
	char client_data[1];    /* The client sees this.*/
};
template<bool thread, int inst>
class __DefaultAllocTemplate
{
private:
	enum 
	{
		__ALIGN = 8,
		__MAXBYTES = 128,
		__NFREELISTS = __MAXBYTES / __ALIGN
	};

	typedef typename __MallocAllocTemplate <inst> MALLOC;
public:

    static void* Allocate(size_t n)
	{
		if (n > __MAXBYTES)
		{
			__TRACE_DEBUG("����һ���ռ���������Allocate(%d)\n", n);
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
		__TRACE_DEBUG("����0x%x�ڴ��\n", result);
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
		//���߳���Ҫ����
		del->_FreeListNext = MyFreeList;
		MyFreeList = del;
		FreeList[GetFreeListIndex(n)] = MyFreeList;

	}
private:
	static char* ChunkAlloc(size_t n, size_t& nobjs); //
	static void* Refill(size_t n);              //ȥ�ڴ����������Ҫ��С���ڴ�;
	static size_t RoundUp(size_t n);           //��Ҫ������ֽ������ϲ��뵽__ALIGN�ı���
	static size_t GetFreeListIndex(size_t n);   //ȡ��nӳ�䵽����������±�λ��
private:
	static Obj* FreeList[__NFREELISTS];

	static char* StartFree;
	static char* EndFree;
	static size_t HeapSize; //��ǰ�ڴ�صĴ�С

};
template<bool thread,int inst>
Obj* __DefaultAllocTemplate<thread, inst>::FreeList[__NFREELISTS] = { 0 };
template<bool thread, int inst>
char* __DefaultAllocTemplate<thread, inst>::StartFree = NULL;
template<bool thread, int inst>
char* __DefaultAllocTemplate<thread, inst>::EndFree = NULL;
template<bool thread, int inst>
size_t __DefaultAllocTemplate<thread, inst>::HeapSize = 0;


template<bool thread, int inst>
char* __DefaultAllocTemplate<thread, inst>::ChunkAlloc(size_t n, size_t& nobjs)
{
	size_t PoolSizeBytes = EndFree - StartFree;
	size_t total = n*nobjs;
	char *result;
	Obj* MyFreeList;
	if (PoolSizeBytes >= total)  //�ڴ������ֽ��㹻����nobjs��n��ȥ�ڴ��������
	{
		__TRACE_DEBUG("���ڴ��������%d�飬ÿ��%dbytes\n", nobjs,n);
		result = StartFree;
		StartFree = StartFree + total;
		return result;
	}
	else if (PoolSizeBytes >= n) // �ڴ������ֽ�ֻ����1�������ϸ�n�ֽ��ڴ档
	{
		nobjs = PoolSizeBytes / n;
		__TRACE_DEBUG("���ڴ��������%d�飬ÿ��%dbytes\n", nobjs, n);
		result = StartFree;
		StartFree = StartFree + n*nobjs;
		return result;
	}
	else // �ڴ������һ��n�ֽڴ�С���ڴ涼û��
	{
		if (PoolSizeBytes> 0)
		{
			size_t index = GetFreeListIndex(PoolSizeBytes);
			MyFreeList = FreeList[index];
			((Obj*)StartFree)->_FreeListNext = MyFreeList;
			MyFreeList = (Obj*)StartFree;
			FreeList[index] = MyFreeList;
			StartFree = NULL;
		}
		
		size_t GetNBytes = 2*total + (HeapSize >> 4);
		__TRACE_DEBUG("�ڴ�����ڴ治�㣬malloc(%d)bytes�ڴ�\n",GetNBytes);

		StartFree = (char*)malloc(GetNBytes);
		if (StartFree == NULL)
		{
			// ȥ������������������ڴ�
			size_t i = total + __ALIGN;
			for (; i < __MAXBYTES; i += __ALIGN)
			{
				MyFreeList = FreeList[GetFreeListIndex(i)];
				if (MyFreeList)
				{
					StartFree = (char*)MyFreeList;
					EndFree = StartFree + i;
					FreeList[GetFreeListIndex(i)] = NULL;
					return  __DefaultAllocTemplate<thread, inst>::ChunkAlloc(n, nobjs);
				}
			}
			__TRACE_DEBUG("�ڴ�����ʧ�ܣ�����һ���ռ�������\n");
			EndFree = NULL;  //��һ���ռ������������쳣��ϣ���һ�������ڴ�ʱ������StartFree��EndFreeӦ�ö�Ϊ�գ���EndFree�����ÿգ�����ʾ��һ����ڴ�
			StartFree = (char*)MALLOC::Allocate(GetNBytes);
		}

		HeapSize += GetNBytes;
		EndFree = StartFree + GetNBytes;
		return __DefaultAllocTemplate<thread, inst>::ChunkAlloc(n, nobjs);
	}
}

template<bool thread, int inst>
void* __DefaultAllocTemplate<thread, inst>::Refill(size_t n)
{
	size_t  nobjs = 20; // ����һ����20��n��С���ڴ��ҵ���������
	char *Chunk = ChunkAlloc(n, nobjs);
	Obj* MyFreeList;
	Obj* next;
	Obj* cur;
	Obj* result;

	if (nobjs == 1)
	{
		__TRACE_DEBUG("����0x%x�ڴ��\n", Chunk);
		return Chunk;
	}

	size_t index = GetFreeListIndex(n);
	MyFreeList = FreeList[index];
	
	result = (Obj *)Chunk;
	MyFreeList = next = (Obj *)(Chunk + n);

	for (size_t i = 0; ; i++)
	{
		cur = next;
		next = (Obj *)((char*)next + n);
		if (i == (nobjs - 2))
		{
			cur->_FreeListNext = NULL;
			break;
		}
		else
		{
			cur->_FreeListNext = next;
		}
	}
	__TRACE_DEBUG("����0x%x�ڴ�飬�������FreeList[%d]\n",result,index);

	FreeList[index] = MyFreeList;
	return result;
}

template<bool thread, int inst>
size_t __DefaultAllocTemplate<thread, inst>::GetFreeListIndex(size_t n)
{
	return (n + __ALIGN - 1) /__ALIGN -1;
}

template<bool thread, int inst>
size_t __DefaultAllocTemplate<thread, inst>::RoundUp(size_t n)
{
	return (n + __ALIGN - 1) &(~(__ALIGN-1));
}

typedef __DefaultAllocTemplate<false, 0> Alloc;
#endif

template<class T, class Alloc>
class simple_alloc {

public:
	static T *Allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::Allocate(n * sizeof (T));
	}

	static T *Allocate(void)
	{
		return (T*)Alloc::Allocate(sizeof (T));
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