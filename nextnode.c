BinTreeNode<int>* NextNode(BinTreeNode<int>* node)
{
	if (node == NULL)
		return NULL;
	if (node->_right != NULL)
	{
		node = node->_right;
		while (node->_left != NULL)
		{
			node = node->_left;
		}
		return node;
	}
	else if (node->_parent != NULL)
	{
		BinTreeNode<int>* Parent = node->_parent;
		if (Parent->_left == node)
		{
			return Parent;
		}
		else
		{
			while (Parent != NULL && Parent->_right == node)
			{
				Parent = Parent->_parent;
			}
			if (Parent != NULL)
			{
				return Parent;
			}
		}
	}
	return  NULL;
}
