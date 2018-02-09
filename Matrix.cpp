
void PrintMatrix(int **arr, int col, int row, int start)
{
	if (start < 0)
		return;
	cout << "打印第" << start + 1 << "圈:" << endl;
	//向右
	int x = 0; //代表列
	for (x = start; x< col-start; ++x)
	{
		cout << arr[start][x]<<" ";
	}
	//向下
	int y = start+1; //代表行
	if (y <= row - start)
	{
		cout << endl;
		return;
	}
	x--;
	for (; y< row - start; ++y)
	{
		cout << arr[y][x] << " ";
	}
	//向左
	--x;
	--y;
	for (; x>start; --x)
	{
		cout << arr[y][x]<<" ";
	}
	//向上
	for (;y>start;--y)
	{
		cout << arr[y][x] << " ";
	}
	cout << endl;
}	
void Matrix(int **arr, int col, int row)
{
	if (arr == NULL || col < 0 || row < 0)
		return;

	cout << "原数组:" << endl;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j< col;j++)
		{
			cout << arr[i][j] << "  ";
		}
		cout << endl;
	}
	int start = 0;
	while (((start*2) < col) && ((2*start) < row))
	{
		PrintMatrix(arr, col, row, start);
		++start;
	}
}
void TestMatrix()
{
	int a[][4] = {  { 1, 2, 3, 4 },
					{ 5, 6, 7, 8 },
					{ 9, 10, 11, 12 }
					 };
	int *arr[3];
	arr[0] = a[0];
	arr[1] = a[1];
	arr[2] = a[2];
	//arr[3] = a[3];
	Matrix(arr, 4, 3);
}