
//递归实现链表逆置
ListNode* RReverseList(ListNode* List,ListNode*& Head)
{
	if (List == NULL)
	{
		return NULL;
	}
	if (List->_next == NULL)
	{
		//遍历到最后一个节点的时候把这个节点保存起来当做新的头结点
		Head = List;
		return List;
	}

	ListNode* pNode = NULL;
	pNode = RReverseList(List->_next, Head);
	if (pNode == NULL)
	{
		return NULL;
	}
	pNode->_next = List;
	List->_next = NULL; //这里必须置空，递归回到第一个节点的时候必须把第一个节点的next置成空，否则会死循环。
	return List;
 } 


////非递归实现链表逆置
ListNode* ReverseList(ListNode* List)
{
	if (List == NULL)
		return NULL;

	ListNode* NewHead = NULL;
	ListNode* cur = NULL;
	ListNode* Next = List;
	while (Next != NULL)
	{
		cur = Next;
		Next = Next->_next;

		cur->_next = NewHead;//这里使得cur节点的_next指向了前一个节点，那么就必须执行上一步，把List的next先保存下来
		NewHead = cur;
	}
	return NewHead;
}

void TestReverse()
{
	ListNode* Node1 = new ListNode(1);
	ListNode* Node2 = new ListNode(2);
	ListNode* Node3 = new ListNode(3);
	ListNode* Node4 = new ListNode(4);
	ListNode* Node5 = new ListNode(5);
	ListNode* Node6 = new ListNode(6);
	ListNode* Node7 = new ListNode(7);
	ListNode* Node = NULL;

	Node1->_next = Node2;
	Node2->_next = Node3;
	Node3->_next = Node4;
	Node4->_next = Node5;
	Node5->_next = Node6;
	Node6->_next = Node7;
	Node7->_next = NULL; 
	//ListNode* NewHead = ReverseList(Node1);
	ListNode* NewHead = NULL;
	RReverseList(Node1,NewHead);
	while (NewHead != NULL)
	{
		cout << NewHead->_data << " ";
		Sleep(1000);
		NewHead = NewHead->_next;
	}
	cout<<endl;
