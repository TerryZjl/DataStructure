
int partsort1(int *a, int begin, int end)
{
	
	int key = end;
	
	while (begin<end)
	{
		while (begin<end)
		{
			if (a[begin] >= a[key])
			{
				break;
			}
			++begin;
		}
		while (begin<end)
		{
			if (a[end] < a[key])
			{
				break;
			}
			--end;
		}

		if (begin == end)
		{
			swap(a[begin], a[key]);
			break;
		}
		else
		{
			swap(a[begin], a[end]);
		}
	}
	return begin; 
}
//[)
void quicksort(int *a, int begin ,int end)
{
	if (begin >= end)
	{
		return;
	}

	int mid = partsort2(a, begin, end);
	int end1 = mid - 1;
	int begin2 = mid + 1;
	quicksort(a, begin, end1);
	quicksort(a, begin2, end);

}
