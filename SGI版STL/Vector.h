#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "Iterator.h"
#include"Alloc.h"

template<class T, class Alloc>
class Vector
{
public:
	typedef T* Iterator;
	typedef const T* ConstIterator;
	typedef ReverseIterator<ConstIterator> ConstReveratorIterator;
	typedef ReverseIterator<Iterator> ReveratorIterator;
	typedef simple_alloc<T, Alloc> VectorNodeMalloc;

	Vector()
		:_start(NULL)
		, _finish(NULL)
		, _endOfStorage(NULL)
	{}

	~Vector()
	{
		if (_start)
		{
			VectorNodeMalloc::Deallocate(_start, (_endOfStorage - _start));
			//delete[] _start;
			_start = _finish = _endOfStorage = NULL;
		}
	}

	void Resize(size_t n, const T& val = T())
	{
		if (n > Capacity())
		{
			Expand(n);
		}

		size_t size = Size();
		if (n < size)
		{
			_finish = _start + n;
		}
		else
		{
			for (size_t i = 0; i < n - size; ++i)
			{
				PushBack(val);
			}
		}
	}

	void Reserve(size_t n)
	{
		Expand(n);
	}

	void PopBack()
	{
		assert(_finish > _start);

		--_finish;
	}

	void PushBack(const T& x)
	{
		if (_finish == _endOfStorage)
		{
			size_t capacity = Capacity();
			size_t n = capacity == 0 ? 3 : 2 * capacity;
			Expand(n);
		}

		*_finish = x;
		++_finish;
	}

	Iterator Begin()
	{
		return _start;
	}

	Iterator End()
	{
		return _finish;
	}

	ReveratorIterator RBegin()
	{
		return ReveratorIterator(End());
	}

	ReveratorIterator REnd()
	{
		return ReveratorIterator(Begin());
	}

	size_t Capacity()
	{
		return _endOfStorage - _start;
	}

	size_t Size()
	{
		return _finish - _start;
	}

	void Expand(size_t n)
	{
		size_t capacity = Capacity();
		size_t size = Size();
		if (n > capacity)
		{
			T* tmp = VectorNodeMalloc::Allocate(n);
			//T* tmp = new T[n];
			for (size_t i = 0; i < size; ++i)
			{
				tmp[i] = _start[i];
			}
			VectorNodeMalloc::Deallocate(_start, _endOfStorage - _start);
			//delete[] _start;

			_start = tmp;
			_finish = _start + size;
			_endOfStorage = _start + n;
		}
	}

	T& operator[](size_t index)
	{
		assert(index < Size());

		return _start[index];
	}

	const T& operator[](size_t index) const
	{
		assert(index < Size());

		return _start[index];
	}

protected:
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
};



#endif