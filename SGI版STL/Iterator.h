#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include<iostream>
using namespace std;

/*
*��Щ��ֻ�����ʹ��
*
*����һ����ʹ�������ǩ������������ٳ��㷨���أ�������ȡ��
*���ö�����ͨ�����ּ̳й�ϵ��������������ֻ�����ݹ��ܶ������á��ĺ�����
*	   ����û��ʵ��Forward_iterator,��ôinputiterator�汾���㷨Ҳ����
*      ͨ���̳���Ƭ����Ϊ��Forward_iteratorʹ�á�
*/
struct InputIteratorTag {};
struct OutputIteratorTag {};
struct ForwardIteratorTag : public InputIteratorTag {};
struct BidirectionalIteratorTag : public ForwardIteratorTag {};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};



/*
*������Ǹ���Ƶ��������˼̳еģ���ֹ�е�����ߴ��Ĵ���©дһ����Ƕ����
*/
template <class Category, class T, class Distance = size_t, class Pointer = T*, class Reference = T&>
struct Iterator {
	typedef Category  IteratorCategory;
	typedef T         ValueType;
	typedef Distance  DifferenceType;
	typedef Pointer   Pointer;
	typedef Reference Reference;
};


/*******************������ȡ��****************************/
template<class It>
struct IteratorTraits{
	typedef typename It::IteratorCategory  IteratorCategory;
	typedef typename It::ValueType ValueType;
	typedef typename It::DifferenceType DifferenceType;
	typedef typename It::Pointer Pointer;
	typedef typename It::Reference Reference;

};

//ƫ�ػ�������Ե�������ԭ��ָ��
template<class T>
struct IteratorTraits<T*>{
	typedef RandomAccessIteratorTag  IteratorCategory;
	typedef T ValueType;
	typedef size_t DifferenceType;
	typedef T* Pointer;
	typedef T& Reference;

}; 
//ƫ�ػ�������Ե������ǳ�ָ�룬����������ػ����������ValueType���ǳ������Ͳ��ܱ��ı䡣
template<class T>
struct IteratorTraits<const T*>{
	typedef RandomAccessIteratorTag  IteratorCategory;
	typedef T ValueType;
	typedef size_t DifferenceType;
	typedef const T*  Pointer;
	typedef const T& Reference;

};


/******************�ԡ�������ȡ�������з�װ�����û�ʹ��*************************/

//����ʹ���߾���ĳ������������
/*template<class It>
inline typename IteratorTraits<It>::IteratorCategory IteratorCategory(const It&)
{
	typedef typename IteratorTraits<It>::IteratorCategory Category;
	return Category(); //���ﷵ�ص���һ�����������Ͷ�����ʹ�õĵط������ܹ�ģ��������Զ����ݳ���������
}
//����ʹ���߾���ĳ��valueType
template<class It>
inline typename IteratorTraits<It>::ValueType* ValueType(const It&)
{
	typedef typename IteratorTraits<It>::ValueType ValueType;
	return ValueType(0);
}
//����ʹ���߾���ĳ��DifferenceType
template<class It>
inline typename IteratorTraits<It>::DifferenceType* DifferenceType(const It&)
{
	typedef typename IteratorTraits<It>::DifferenceType DifferenceType;
	return DifferenceType(0);
}*/



//����˫���������������ĵ������ľ���
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

//�������ɵ������ľ���
template<class RandomAccessIterator>
inline typename IteratorTraits<RandomAccessIterator>::DifferenceType \
__Distance(RandomAccessIterator Begin, RandomAccessIterator End, RandomAccessIteratorTag)
{
	return End - Begin;
}

//��������������֮��ľ���
template<class InputIterator>
inline typename IteratorTraits<InputIterator>::DifferenceType \
Distance(InputIterator Begin, InputIterator End)
{
	IteratorTraits<InputIterator>::IteratorCategory  Category;
	return __Distance(Begin, End, Category);
}

/***********************�������������������***********************************/
template<class It>
struct ReverseIterator 
{
	It _it;  //ĳ�����������
	typedef typename IteratorTraits<It>::IteratorCategory IteratorCategory;
	typedef typename IteratorTraits<It>::ValueType ValueType;
	typedef typename IteratorTraits<It>::DifferenceType DifferenceType;
	typedef typename IteratorTraits<It>::Pointer Pointer;
	typedef typename IteratorTraits<It>::Reference Reference;
	typedef ReverseIterator<It> Self;

	//explicit ������ʽ����ת��������� ReIt = l.Begin(),���ǲ�����ġ�
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
		//Ϊ��RBegin��REnd���ϳ���˼ά��������������ú�һ��λ�ã�
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




