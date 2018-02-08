

ListNode* Merge(ListNode* List1, ListNode* List2)
{
	if (List1 == NULL)
		return List2;
	if (List2 == NULL)
		return List1;


	ListNode* MergeNode = NULL;
	if (List1->_data < List2->_data)
	{
		MergeNode = List1;
		List1 = List1->_next;
	}
	else
	{
		MergeNode = List2;
		List2 = List2->_next;
	}

	ListNode* cur = MergeNode;

	while(List1!=NULL && List2 != NULL)
	{
		while (List1!= NULL && List1->_data<= List2->_data)
		{
			cur->_next = List1;
			cur = cur->_next;
			List1 = List1->_next;
		}
		if (List1 == NULL)
			break;
		while (List2!= NULL&&List2->_data < List1->_data)
		{
			cur->_next = List2;
			cur = cur->_next;
			List2 = List2->_next;
		}
	}
	if (List1 == NULL)
	{
		cur->_next = List2;
	}
	else
	{
		cur->_next = List1;
	}
	return MergeNode;
}

ListNode* Rmerge(ListNode* List1, ListNode* List2)
{
	if (List1 == NULL)
		return List2;
	else if (List2 == NULL)
		return List1;

	ListNode* PNode = NULL;

	if (List1->_data <= List2->_data)
	{
		PNode = List1;
		PNode->_next = Rmerge(List1->_next, List2);
	}
	if (List2->_data < List1->_data)
	{
		PNode = List2;
		PNode->_next = Rmerge(List1, List2->_next);
	}
	return PNode;
}
void TestMerge()
{
	ListNode* Node1 = new ListNode(1);
	ListNode* Node2 = new ListNode(3);
	ListNode* Node3 = new ListNode(4);
	ListNode* Node4 = new ListNode(6);
	ListNode* Node5 = new ListNode(8);
	ListNode* Node6 = new ListNode(10);
	Node1->_next = Node2;
	Node2->_next = Node3;
	Node3->_next = Node4;
	Node4->_next = Node5;
	Node5->_next = Node6;
	Node6->_next = NULL;
	ListNode* NewNode1 = new ListNode(2);
	ListNode* NewNode2 = new ListNode(5);
	ListNode* NewNode3 = new ListNode(7);
	ListNode* NewNode4 = new ListNode(9);
	ListNode* NewNode5 = new ListNode(11);
	ListNode* NewNode6 = new ListNode(13);
	NewNode1->_next = NewNode2;
	NewNode2->_next = NewNode3;
	NewNode3->_next = NewNode4;
	NewNode4->_next = NewNode5;
	NewNode5->_next = NewNode6;
	NewNode6->_next = NULL;

	ListNode* MergeList = Rmerge(Node1, NewNode1);
	//ListNode* MergeList = merge(Node1, NewNode1);
	while (MergeList != NULL)
	{
		cout << MergeList->_data << " ";
		MergeList = MergeList->_next;
	}
	cout << endl;
}