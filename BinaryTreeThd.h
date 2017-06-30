#ifndef _BTT_H_
#define _BTT_H_

#include<iostream>
#include<stack>
using namespace std;

enum NodeTag
{
	THERAD,
	LINK,
};

template <class T>
struct BinaryTreeNode
{
	BinaryTreeNode<T>* _left;
	BinaryTreeNode<T>* _right;
	NodeTag _leftTag;
	NodeTag _rightTag;
	T _data;

	BinaryTreeNode(const T& x)
		:_data(x)
		, _left(NULL)
		, _right(NULL)
		, _leftTag(LINK)
		, _rightTag(LINK)
	{}
};

template<class T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;
public:
	BinaryTree()
		:_root(NULL)
	{}

	BinaryTree(T* arr, int n, const T& value)
	{
		int i = 0;
		_root = CreatTree(arr, n, value, i);
	}

	BinaryTree(const BinaryTree<T>& tree)
		:_root(NULL)
	{
		_root = Copy(tree._root);
	}

	void InOrderThreading()
	{
		Node* prev = NULL;
		_InOrderThreading(_root, prev);
		if (prev)   //线索化到最后一个结点时root为空,prev指向最后一个节点了，但是prev->_rightTag为LINK，因为需要线索化，所以要将_rightTag至成THERAD
		{
			prev->_right == NULL;
			prev->_rightTag = THERAD;
		}

	}
	void InOrderThd();
	void PrevOrderThreading()
	{
		Node* prev = NULL;
		_PrevOrderThreading(_root, prev);
	}
	void PrevOrderThd();
	BinaryTree<T> operator=(BinaryTree<T> tree)
	{
		swap(_root, tree._root);
		return *this;
	}

	void PrevOrder()
	{
		cout << "先序：";
		_PrevOrder(_root);
		cout << endl;
	}

	void InOrder()
	{
		cout << "中序：";
		_InOrder(_root);
		cout << endl;
	}

	void PostOrder()
	{
		cout << "后序：";
		_PostOrder(_root);
		cout << endl;
	}
	//~BinaryTree()
	//{
	//	Destroy(_root);
	//}
protected:
	void _PostOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_PostOrder(root->_left);
		_PostOrder(root->_right);
		cout << root->_data << " ";
	}
	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_data << " ";
		_InOrder(root->_right);
	}

	void _PrevOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		cout << root->_data << " ";
		_PrevOrder(root->_left);
		_PrevOrder(root->_right);
	}
	Node* CreatTree(T* arr, int n, const T& value, int& i)
	{
		Node* NewNode = NULL;
		if ((i < n) && (arr[i] != value))
		{
			NewNode = new Node(arr[i]);
			NewNode->_left = CreatTree(arr, n, value, ++i);
			NewNode->_right = CreatTree(arr, n, value, ++i);
		}

		return NewNode;
	}

	void _InOrderThreading(Node* root, Node*& prev)
	{
		if (root == NULL)
			return;
		_InOrderThreading(root->_left, prev);
		if (root->_left == NULL)   //若当前节点的左子树为空，这可以把左子树线索化到前一个访问的节点（prev）
		{
			root->_left = prev;
			root->_leftTag = THERAD;
		}
		if (prev&&prev->_right == NULL)  //若当前节点的右子树为空，则可以把右子树线索化到下一个节点，
		{                               //因为当前还没访问下一个节点，所以需要等到访问到下一个节点时，在线索化上一个节点的右子树
			prev->_right = root;
			prev->_rightTag = THERAD;
		}
		prev = root;                    //记录上一个节点
		_InOrderThreading(root->_right, prev);
	}

	void _PrevOrderThreading(Node* root, Node*& prev)
	{
		if (root == NULL)
			return;

		if (root->_left == NULL&&root->_leftTag==LINK)
		{
			root->_left = prev;
			root->_leftTag = THERAD;
		}
		if (prev&&prev->_right == NULL)  //若当前节点的右子树为空，则可以把右子树线索化到下一个节点，
		{                               //因为当前还没访问下一个节点，所以需要等到访问到下一个节点时，在线索化上一个节点的右子树
			prev->_right = root;
			prev->_rightTag = THERAD;
		}
		prev = root;
		_PrevOrderThreading(root->_left, prev);
		_PrevOrderThreading(root->_right, prev);
	}

	Node* Copy(Node* root)
	{
		if (root == NULL)
		{
			return NULL;
		}
		Node* NewNode = new Node(root->_data);
		NewNode->_left = Copy(root->_left);
		NewNode->_right = Copy(root->_right);

		return NewNode;
	}
	void Destroy(Node* root)
	{
		if (root == NULL)
			return;

		Destroy(root->_left);
		Destroy(root->_right);

		delete root;
	}
private:
	Node* _root;
};


#endif