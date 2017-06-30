#ifndef _BT_H_
#define _BT_H_

#include<iostream>
#include<stack>
using namespace std;

template <class T>
struct BinTreeNode
{
	BinTreeNode<T>* _left;
	BinTreeNode<T>* _right;
	T _data;

	struct BinTreeNode(const T& x)
		:_left(NULL)
		, _right(NULL)
		, _data(x)
	{
	};
};

template <class T>
class  BinTree
{
	typedef BinTreeNode<T> Node;
public:
	BinTree()
		:_root(NULL)
	{}
	BinTree(T* arr, int n, const T& value)
	{
		int i = 0;
		_root = CreatTree(arr, n, value, i);//因为这里成员函数含有隐含的This指针，所以不能实现递归
	}

	BinTree(const BinTree<T>& tree)
		:_root(NULL)
	{
		_root = _copy(tree._root);
	}

	BinTree<T>& operator=(BinTree<T>& tree)
	{
		if (this != &tree)
		{
			Destroy(_root);
			_root = _copy(tree._root);
		}

		return *this;
	}

	void PrevOrderR()
	{
		Node* cur = _root;
		stack<Node*> s;

		while (!s.empty() || cur != NULL)  //若这里的cur为空，则说明左右子树都为空，只有栈和右子树同时为空时，二叉树才访问完
		{
			while (cur != NULL)
			{
				cout << cur->_data << " ";
				s.push(cur);
				cur = cur->_left;
			}

			//若左子树和为空时程序则走到这里，开始取栈顶元素来访问上一层父节点的右子树
			Node* top = s.top();
			cur = top->_right;
			s.pop();
		}
		cout << endl;
	}
	void InOrderR()
	{
		Node* cur = _root;
		stack<Node*> s;

		while (!s.empty() || cur != NULL)  //若这里的cur为空，则说明左右子树都为空
		{
			while (cur != NULL)
			{
				s.push(cur);
				cur = cur->_left;
			}

			//若左子树为空时程序则走到这里，开始取栈顶元素来打印父节点，并访问父节点的右子树
			Node* top = s.top();
			cout << top->_data << " ";
			cur = top->_right;
			s.pop();
		}
		cout << endl;
	}
	void PostOrderR()
	{
		Node* cur = _root;
		stack<Node*> s;
		Node* Prev = NULL;   //用Prev记录上一次访问的节点，这样就可以判断右子树（不为空时）是第一次访问，还是第二次访问
		while (!s.empty() || cur != NULL)             //第一次访问会把右子树当成子问题处理，当右子问题输出后，Prev就会置成右子节点         
		{                                            //当在一次回到访问右子树的父节点时，发现上一次的Prev是右子节点，作说明已经访问过有节点，这可以输出根结点了    
			while (cur != NULL)    //从根结点开始把左子树全部压入栈
			{
				s.push(cur);
				cur = cur->_left;
			}
			Node* top = s.top();  //左子树为空时，取出栈顶元素
			if (top->_right == NULL || top->_right == Prev)//取出栈顶元素，看他的右子树是否为空或者是否右子树已经访问过，若是则打印当前节点，并把当前节点出栈
			{
				cout << top->_data << " ";
				Prev = top;
				s.pop();
			}
			else            //若右子树没访问过，且不为空，则把右子树当成子问题，继续上面的压栈操作
			{
				cur = top->_right;
			}
		}
		cout << endl;
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
	size_t Size()
	{
		return _Size(_root);
	}

	size_t LeafSize()
	{
		return _LeafSize(_root);
	}
	//求第K层的节点数
	size_t GetKLevel(size_t k)
	{
		return _GetKLevel(_root, k);
	}

	size_t Depth()
	{
		return _Depth(_root);
	}
	Node* Find(const T& x)
	{
		return _Find(_root, x);
	}
	~BinTree()
	{
		Destroy(_root);
	}

private:
	Node* _copy(Node* root)
	{
		if (root == NULL)
		{
			return NULL;
		}
		Node* NewRoot = new Node(root->_data);
		NewRoot->_left = _copy(root->_left);
		NewRoot->_right = _copy(root->_right);
		return NewRoot;
	}
	Node* _Find(Node* root, const T& x)
	{
		if (root == NULL){ return NULL; }

		if (root->_data == x){ return root; }

		Node* ret = _Find(root->_left, x);
		if (ret != NULL)            //这里如果左子树找到了，就不用再去右子树寻找了，直接返回
		{
			return ret;
		}
		return _Find(root->_right, x);  //若果左子树没有找到，再去右子树寻找，无论找到没找都需要一个返回值，所以直接返回
	}
	size_t _Depth(Node* root) //因为在递归返回时，需要累加每一层递归，所以加1，依次求出深度
	{
		if (root == NULL)
		{
			return 0;
		}

		if ((root->_left == NULL) && (root->_right == NULL))
		{
			return 1;
		}
		size_t left = _Depth(root->_left);
		size_t right = _Depth(root->_right);
		return left >= right ? left + 1 : right + 1;
	}

	//第K层的节点数
	size_t _GetKLevel(Node* root, size_t k)  //这里不用担心如果K超过了最深深度，因为如果超了，就会返回0.
	{
		if (root == NULL)
		{
			return 0;
		}
		if (k == 1)
		{
			return 1;
		}

		return _GetKLevel(root->_left, k - 1) + _GetKLevel(root->_right, k - 1);
	}
	//叶子结点
	size_t _LeafSize(Node* root)
	{
		if (root == NULL)
		{
			return 0;
		}

		if (root->_left == NULL&&root->_right == NULL)
		{
			return 1;
		}

		return _LeafSize(root->_left) + _LeafSize(root->_right);
	}
	//结点数
	size_t _Size(Node* root)
	{
		size_t left = 0;
		size_t right = 0;

		if (root == NULL)
		{
			return 0;
		}

		left = _Size(root->_left);
		right = _Size(root->_right);
		return left + right + 1;
	}
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
		Node* root = NULL;
		if ((i<n) && (arr[i] != value))
		{
			root = new Node(arr[i]);
			root->_left = CreatTree(arr, n, value, ++i);
			root->_right = CreatTree(arr, n, value, ++i);
		}

		return root;
	}
	void Destroy(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		Destroy(root->_left);
		Destroy(root->_right);

		delete root;
	}
private:
	Node* _root;
};

#endif


