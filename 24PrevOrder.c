
//面试题24.二叉搜索树的后续遍历序列
bool PrevOrder(int arr[], int length)
{
	if (arr == NULL || length <= 0)
		return false;

	//判断左子树是否比根结点大
	int i = 0;
	for (; i < length - 1; i++)
	{
		if (arr[i] > arr[length - 1])
			break;
	}

	//判断右子树是否比根结点小
	int j = i;
	for (; j < length-1;j++)
	{
		if (arr[j] < arr[length - 1])
			return false;
	}

	//递归判断左子树是不是在二叉搜索树中
	bool left = true;
	if (i > 0)
	{
		left = PrevOrder(arr, i);
	}


	//递归判断右子树是不是在二叉搜索树中
	bool right = true;
	if (i < length-1)
	{
		right = PrevOrder(arr + i, length - i - 1);
	}
	//左右子树的大小合理
	return (right &&left);
}
