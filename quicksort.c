
int partsort2(int *a, int begin, int end)
{
	int key = end;

	int cur = begin;
	int prev = cur - 1;
		while (cur<key)
		{
			if (a[cur] < a[key])
			{
				++prev;
				if (prev != cur)
					swap(a[prev], a[cur]);
			}
			++cur;

		}
		++prev;

		swap(a[end], a[prev]);
		return prev;
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
