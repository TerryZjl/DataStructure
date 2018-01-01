void shellsort(int *a, size_t n)
{
	int tag = n;
	while (tag > 1)
	{
		tag = tag /3  + 1;

		for (int i = 0; i < n - tag; i++)
		{
			int tmp = a[i+tag];
			int j = i;
			while (j >= 0 && a[j]>tmp)
			{
					a[j+tag] = a[j];
					j-=tag;
			}
			a[j+tag] = tmp;
		}
	}
}