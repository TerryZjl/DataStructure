#include<iostream>
#include<windows.h>
#include<string>
#include<algorithm>
#include"List.h"
using namespace std;

#include"Alloc.h"
#include"Iterator.h"
#include"Vector.h"
template<class Container>
void Print(Container& c)
{
	Container::Iterator it = c.Begin();
	while (it != c.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}

void TestList()
{

	List<int, Alloc> l;
	for (int i = 1; i < 20; i++)
	{
		l.PushBack(i);
	}
	Print(l);

	List<int, Alloc>::ReIterator Rit = l.RBegin();
	while (Rit!= l.REnd())
	{
		cout << *Rit << " ";
		++Rit;
	}
	cout << endl;
	//List<int, Alloc>::Iterator it = find(l.Begin(), l.End(), 15);
	//if (it != l.End())
	//{
	//	cout << "find £º" << *it << endl;
	//}
	//else
	//{
	//	cout << "Not find" << endl;
	//}
	l.Clear();
}

void TestIterator()
{
	List<int, Alloc> l;
	for (int i = 0; i < 20; i++)
		l.PushBack(i);
	cout << Distance(l.Begin(), l.End()) << endl;
	cout << "--------------------------------------" << endl;
	Vector<int, Alloc> v;
	for (int i = 0; i < 20; i++)
		v.PushBack(i);
	cout << Distance(v.Begin(), v.End()) << endl;



}

int main()
{
	try
	{
		TestIterator();
		//TestOneAllocate();
		//TestList();
	}
	catch (exception& error)
	{
		cout << error.what()<< endl;
	}
	system("pause");
	return 0;
}


