#ifndef _LIST_H_
#define _LIST_H_

#include<iostream>
using namespace std;

#include"Alloc.h"
#include"Iterator.h"
template<class T>
struct ListNode
{
	T _data;
	ListNode* _next;
	ListNode* _prev;
	ListNode(const T& x = 0)
		:_data(x)
		, _next(NULL)
		, _prev(NULL)
	{}
};


template<class T, class P, class R>
struct __ListIterator : public Iterator<BidirectionalIteratorTag,T>
{
	typedef __ListIterator<T, P, R> self;
	typedef ListNode<T> Node;
	
	Node* _it;

	__ListIterator()
	{}
	__ListIterator(Node* node) 
		:_it(node)
	{}

	__ListIterator(const Node* node)
		: _it(node)
	{}

	__ListIterator(const self& It)
	{
		_it = It._it;
	}

	~__ListIterator()
	{
		_it = NULL;
	}
	self& operator++()
	{
		_it = _it->_next;
		return *this;
	}
	//先构造了一个tmp对象，然后返回时又调用拷贝构造，构造一个临时对象，把这个临时对象返回。然后调用析构函数析构tmp对象
	//self operator++(int)
	//{
	//	self tmp(_it);
	//	_it = _it->_next;
	//	return tmp;
	//}
	
	//而这种方法不用创建临时对象。
	self& operator++(int)
	{
		Node* tmp = _it;
		_it = _it->_next;
		return self(tmp);
	}

	self& operator--()
	{
		_it = _it->_prev;
		return self(_it);
	}
	self& operator--(int)
	{
		Node* tmp = _it;
		_it = _it->_prev;
		return self(tmp);
	}

	R operator*() const
	{
		return _it->_data;
	}

	P operator->() const
	{
		return &(_it->_data);
	}

	bool operator==(self& It)
	{
		return It._it == _it;
	}
	bool operator!=(self& It)
	{
		if (_it != It._it)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
template<class T , class Alloc>
class List
{
	typedef ListNode<T> Node;
	typedef const T* ConstPtr;
	typedef const T& ConstRef;
	typedef T* Ptr;
	typedef T& Ref;
	typedef  simple_alloc<Node, Alloc> ListNodeMalloc;
public:
	typedef __ListIterator<T, T*, T&> Iterator;
	typedef __ListIterator<T, const T*, const T&> ConstIterator;
	typedef typename ReverseIterator<Iterator> ReIterator;
	typedef typename ReverseIterator<ConstIterator> ReConstIterator;
public:
	List()
		:_head(GetNode(T()))
	{
		_head->_next = _head;
		_head->_prev = _head;
	}
	Node* GetNode(const T& x)
	{
		Node* tmp = ListNodeMalloc::Allocate();
		new(tmp)Node(x);
		//tmp->_data = x;
		//tmp->_next = NULL;
		//tmp->_prev = NULL;
		return tmp;
	}
	void PushBack(const T& x)
	{
		//Node* tmp = new Node(x);
		Node* tmp = GetNode(x);
		Node* tail = _head->_prev;
		tmp->_prev = tail;
		tmp->_next = _head;
		_head->_prev = tmp;
		tmp->_prev->_next = tmp;
	}
	void PushFront(const T& x)
	{
		//Node* tmp = new Node(x);
		
		Node* tmp = GetNode(x);

		Node* Next = _head;
		Node* Prev = _head->_prev;
		_head = tmp;
		_head->_next = Next;
		_head->_prev = Prev;
	}
	void PopBack()
	{
		Node* Next = _head;
		Node* Prev = _head->_prev->_prev;
		//delete _head->_prev;
		ListNodeMalloc::Deallocate(_head->_prev);

		Prev->_next = Next;
		Next->_prev = Prev;
	}
	void PopFront()
	{
		Node* Next = _head->_next;
		Node* Prev = _head->_prev;
		//delete _head;
		ListNodeMalloc::Deallocate(_head);
		_head = Next;
		_head->_prev = Prev;

		Prev->_next = _head;


	}
	Iterator Insert(Iterator It, const T& x)
	{
		Node* tmp = It._it->_prev;
		//Node* newNode = new Node(x);
		Node* newNode = GetNode(x);
		tmp->_next = newNode;
		newNode->_prev = tmp;

		newNode->_next = It._it;
		It._it->_prev = newNode;
		return newNode;
	}

	Iterator Erase(Iterator It)
	{
		Node* tmp = It._it;
		Node* Prev = tmp->_prev;
		Node* Next = tmp->_next;

		Prev->_next = Next;
		Next->_prev = Prev;
		//delete tmp;
		ListNodeMalloc::Deallocate(tmp);
		return Iterator(Next);
	}

	//下面这种办法可以在erase之后可以直接++，加到下一个数字。
	/*Iterator Erase(Iterator It)
	{
		Node* tmp = It._it;
		Node* Prev = tmp->_prev;
		Node* Next = tmp->_next;

		Prev->_next = Next;
		Next->_prev = Prev;
		delete tmp;
		It = Prev;
		return It;
	}*/

	Ref Front()
	{
		return *(Begin());
	}
	ConstRef Front() const
	{
		return *(Begin());
	}

	Ref Back()
	{
		return *(--End());
	}

	ConstRef Back()	const
	{
		return *(--End());
	}

	ConstIterator Begin() const
	{
		return ConstIterator(_head->_next);
	}
	ConstIterator End() const
	{
		return ConstIterator(_head);
	}
	Iterator Begin()
	{
		return _head->_next;
	}
	Iterator End()
	{
		return _head;
	}
	ReIterator RBegin()
	{
		//因为使用了避免隐式类型转换的explicit ，所以这里必须显示类型转换
		return ReIterator(End()); //符合常规思维
	}
	ReIterator REnd()
	{
		return ReIterator(Begin());
	}
	

	void Clear()
	{
		Node* cur = _head->_next;
		while (cur != _head)
		{
			Node* tmp = cur->_next;
			ListNodeMalloc::Deallocate(cur);
			//delete cur;
			cur = tmp;
		}

		_head->_next = _head;
		_head->_prev = _head;
	}

	~List()
	{
		Clear();
		//delete _head;
		ListNodeMalloc::Deallocate(_head);
		_head = NULL;
	}

private:
	Node* _head;
};



#endif