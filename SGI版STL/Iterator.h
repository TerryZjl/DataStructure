#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include<iostream>
using namespace std;

/*
*这些类只做标记使用
*
*作用一：是使用这个标签左第三个参数促成算法重载，方便萃取。
*作用二：是通过这种继承关系可以消除“单纯只做传递功能而被调用”的函数，
*	   例如没有实现Forward_iterator,那么inputiterator版本的算法也可以
*      通过继承切片的行为被Forward_iterator使用。
*/
struct InputIteratorTag {};
struct OutputIteratorTag {};
struct ForwardIteratorTag : public InputIteratorTag {};
struct BidirectionalIteratorTag : public ForwardIteratorTag {};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};



/*
*这个类是给设计迭代器的人继承的，防止有的设计者粗心大意漏写一个内嵌类型
*/
template <class Category, class T, class Distance = size_t, class Pointer = T*, class Reference = T&>
struct Iterator {
	typedef Category  IteratorCategory;
	typedef T         ValueType;
	typedef Distance  DifferenceType;
	typedef Pointer   Pointer;
	typedef Reference Reference;
};


/*******************特性萃取机****************************/
template<class It>
struct IteratorTraits{
	typedef typename It::IteratorCategory  IteratorCategory;
	typedef typename It::ValueType ValueType;
	typedef typename It::DifferenceType DifferenceType;
	typedef typename It::Pointer Pointer;
	typedef typename It::Reference Reference;

};

//偏特化——针对迭代器是原生指针
template<class T>
struct IteratorTraits<T*>{
	typedef RandomAccessIteratorTag  IteratorCategory;
	typedef T ValueType;
	typedef size_t DifferenceType;
	typedef T* Pointer;
	typedef T& Reference;

}; 
//偏特化——针对迭代器是常指针，若用上面的特化这种情况，ValueType就是常数，就不能被改变。
template<class T>
struct IteratorTraits<const T*>{
	typedef RandomAccessIteratorTag  IteratorCategory;
	typedef T ValueType;
	typedef size_t DifferenceType;
	typedef const T*  Pointer;
	typedef const T& Reference;

};


/******************对“特性萃取机“进行封装方便用户使用*************************/

//方便使用者决定某个迭代器类型
/*template<class It>
inline typename IteratorTraits<It>::IteratorCategory IteratorCategory(const It&)
{
	typedef typename IteratorTraits<It>::IteratorCategory Category;
	return Category(); //这里返回的是一个迭代器类型对象，在使用的地方可以能过模板的推演自动推演出它的类型
}
//方便使用者决定某个valueType
template<class It>
inline typename IteratorTraits<It>::ValueType* ValueType(const It&)
{
	typedef typename IteratorTraits<It>::ValueType ValueType;
	return ValueType(0);
}
//方便使用者决定某个DifferenceType
template<class It>
inline typename IteratorTraits<It>::DifferenceType* DifferenceType(const It&)
{
	typedef typename IteratorTraits<It>::DifferenceType DifferenceType;
	return DifferenceType(0);
}*/



//计算双向迭代器及派生类的迭代器的距离
template<class InputIterator>
inline typename IteratorTraits<InputIterator>::DifferenceType \
__Distance(InputIterator Begin, InputIterator End, InputIteratorTag)
{
	IteratorTraits<InputIterator>::DifferenceType n = 0;
	while (Begin != End)
	{
		++n;
		++Begin;
	}
	return n;
}

//计算自由迭代器的距离
template<class RandomAccessIterator>
inline typename IteratorTraits<RandomAccessIterator>::DifferenceType \
__Distance(RandomAccessIterator Begin, RandomAccessIterator End, RandomAccessIteratorTag)
{
	return End - Begin;
}

//计算两个迭代器之间的距离
template<class InputIterator>
inline typename IteratorTraits<InputIterator>::DifferenceType \
Distance(InputIterator Begin, InputIterator End)
{
	IteratorTraits<InputIterator>::IteratorCategory  Category;
	return __Distance(Begin, End, Category);
}

/***********************反向迭代器（适配器）***********************************/
template<class It>
struct ReverseIterator 
{
	It _it;  //某个正向迭代器
	typedef typename IteratorTraits<It>::IteratorCategory IteratorCategory;
	typedef typename IteratorTraits<It>::ValueType ValueType;
	typedef typename IteratorTraits<It>::DifferenceType DifferenceType;
	typedef typename IteratorTraits<It>::Pointer Pointer;
	typedef typename IteratorTraits<It>::Reference Reference;
	typedef ReverseIterator<It> Self;

	//explicit 避免隐式类型转换，例如把 ReIt = l.Begin(),这是不允许的。
	explicit ReverseIterator(It it)
		:_it(it)
	{}

 	Self& operator++()
	{
		--_it;
		return *this;
	}
	Self& operator++(int)
	{
		Self tmp = *this;
		--_it;
		return tmp;
	}
	Self& operator--()
	{
		++_it;
		return *this;
	}
	Self& operator--(int)
	{
		Self tmp = *this;
		++_it;
		return tmp;
	}
	Reference operator*()
	{
		//为了RBegin和REnd符合常规思维，随意这里解引用后一个位置，
		It tmp = _it;
		--tmp;
		return (*tmp);
	}
	Pointer operator->()
	{
		return &(*_it);
	}
	bool operator==(Self& it)
	{
		return _it == it._it ? true : false;
	}
	bool operator!=(Self& it)
	{
		return _it != it._it ? true : false;
	}

};
#endif




