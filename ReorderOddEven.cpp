#include"test.h"

//ͨ������ָ��������Ƚϵķ����������ͷ��㴦��һ�����⣬
void ReorderOddEven(int *arr, unsigned int length, bool (*fun)(int))
{
	if (arr == NULL || length == 0)
		return;

	int begin = 0;
	int end = length - 1;
	while (begin < end)
	{
		//��ż��
		while ((begin<end) && !fun(arr[begin]))
		{
			++begin;
		}
		//�һ�����
		while ((begin <end) && fun(arr[end]))
		{
			--end;
		}
		if (begin < end)
			swap(arr[begin], arr[end]);
	}
}
bool fun(int num)
{
	return (num & 0x1) == 0;
}
void TestReorderOddEven()
{
	int a[] = { 12, 76, 7, 5, 6, 23, 24, 56, 57 };
	int length = sizeof(a) / sizeof(a[0]);
	ReorderOddEven(a, length,fun);
	for (int i = 0; i < length; ++i)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}

