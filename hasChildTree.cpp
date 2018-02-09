
struct BinaryTreeNode
{
	int _data;
	BinaryTreeNode* _left;
	BinaryTreeNode* _right;
	BinaryTreeNode(int x= 0)
		:_data(x)
		, _left(NULL)
		, _right(NULL)
	{}
};

bool IsChildTree(BinaryTreeNode* tree1, BinaryTreeNode* tree2)
{
	if (tree1 == NULL && tree2 != NULL)
		return false;
	if (tree2 == NULL)
		return true;
	
	if (tree1->_data != tree2->_data)
		return false;
	bool ret = false;
	ret = IsChildTree(tree1->_left, tree2->_left);
	if (ret == false)
		return false;
	ret = IsChildTree(tree1->_right, tree2->_right);
	if (ret == false)
		return false;
	return true;
}
bool ChildTree(BinaryTreeNode* tree1, BinaryTreeNode* tree2)
{
	if (tree1 == NULL||tree2 == NULL)
		return false;
	bool ret = false;
	if (tree1->_data == tree2->_data)
	{
		ret = IsChildTree(tree1, tree2);
		if (ret)
			return ret;
	}
	ret = ChildTree(tree1->_left, tree2);
	if (ret)
		return true;
	ret = ChildTree(tree1->_right, tree2);
	if (ret)
		return true;
	return false;
}
void PrintTree(BinaryTreeNode* tree)
{
	if (tree == NULL)
		return;
	PrintTree(tree->_left);
	cout << tree->_data << " ";
	PrintTree(tree->_right);
}
void TestChildTree()
{
	BinaryTreeNode* Node1 = new BinaryTreeNode(8);
	BinaryTreeNode* Node2 = new BinaryTreeNode(8);
	BinaryTreeNode* Node3 = new BinaryTreeNode(7);
	BinaryTreeNode* Node4 = new BinaryTreeNode(9);
	BinaryTreeNode* Node5 = new BinaryTreeNode(2);
	BinaryTreeNode* Node6 = new BinaryTreeNode(4);
	BinaryTreeNode* Node7 = new BinaryTreeNode(7);
	Node1->_left = Node2;
	Node1->_right = Node3;
	Node2->_left = Node4;
	Node2->_right = Node5;
	Node3->_left = Node3->_right = NULL;
	Node4->_left = Node4->_right = NULL;
	Node5->_left = Node6;
	Node5->_right = Node7;
	Node6->_left = Node6->_right = Node7->_left = Node7->_right = NULL;

	BinaryTreeNode* node1 = new BinaryTreeNode(2);
	BinaryTreeNode* node2 = new BinaryTreeNode(4);
	BinaryTreeNode* node3 = new BinaryTreeNode(1);
	node1->_left = node2;
	node1->_right = node3;
	node2->_left = node2->_right = node3->_left = node3->_right = NULL;

	bool ret = ChildTree(Node1, node1);
	cout << ret << endl;
}