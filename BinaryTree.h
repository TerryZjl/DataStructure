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
		_root = CreatTree(arr, n, value, i);//��Ϊ�����Ա��������������Thisָ�룬���Բ���ʵ�ֵݹ�
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

		while (!s.empty() || cur != NULL)  //�������curΪ�գ���˵������������Ϊ�գ�ֻ��ջ��������ͬʱΪ��ʱ���������ŷ�����
		{
			while (cur != NULL)
			{
				cout << cur->_data << " ";
				s.push(cur);
				cur = cur->_left;
			}

			//����������Ϊ��ʱ�������ߵ������ʼȡջ��Ԫ����������һ�㸸�ڵ��������
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

		while (!s.empty() || cur != NULL)  //�������curΪ�գ���˵������������Ϊ��
		{
			while (cur != NULL)
			{
				s.push(cur);
				cur = cur->_left;
			}

			//��������Ϊ��ʱ�������ߵ������ʼȡջ��Ԫ������ӡ���ڵ㣬�����ʸ��ڵ��������
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
		Node* Prev = NULL;   //��Prev��¼��һ�η��ʵĽڵ㣬�����Ϳ����ж�����������Ϊ��ʱ���ǵ�һ�η��ʣ����ǵڶ��η���
		while (!s.empty() || cur != NULL)             //��һ�η��ʻ�����������������⴦�����������������Prev�ͻ��ó����ӽڵ�         
		{                                            //����һ�λص������������ĸ��ڵ�ʱ��������һ�ε�Prev�����ӽڵ㣬��˵���Ѿ����ʹ��нڵ㣬���������������    
			while (cur != NULL)    //�Ӹ���㿪ʼ��������ȫ��ѹ��ջ
			{
				s.push(cur);
				cur = cur->_left;
			}
			Node* top = s.top();  //������Ϊ��ʱ��ȡ��ջ��Ԫ��
			if (top->_right == NULL || top->_right == Prev)//ȡ��ջ��Ԫ�أ��������������Ƿ�Ϊ�ջ����Ƿ��������Ѿ����ʹ����������ӡ��ǰ�ڵ㣬���ѵ�ǰ�ڵ��ջ
			{
				cout << top->_data << " ";
				Prev = top;
				s.pop();
			}
			else            //��������û���ʹ����Ҳ�Ϊ�գ�������������������⣬���������ѹջ����
			{
				cur = top->_right;
			}
		}
		cout << endl;
	}
	void PrevOrder()
	{
		cout << "����";
		_PrevOrder(_root);
		cout << endl;
	}

	void InOrder()
	{
		cout << "����";
		_InOrder(_root);
		cout << endl;
	}

	void PostOrder()
	{
		cout << "����";
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
	//���K��Ľڵ���
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
		if (ret != NULL)            //��������������ҵ��ˣ��Ͳ�����ȥ������Ѱ���ˣ�ֱ�ӷ���
		{
			return ret;
		}
		return _Find(root->_right, x);  //����������û���ҵ�����ȥ������Ѱ�ң������ҵ�û�Ҷ���Ҫһ������ֵ������ֱ�ӷ���
	}
	size_t _Depth(Node* root) //��Ϊ�ڵݹ鷵��ʱ����Ҫ�ۼ�ÿһ��ݹ飬���Լ�1������������
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

	//��K��Ľڵ���
	size_t _GetKLevel(Node* root, size_t k)  //���ﲻ�õ������K������������ȣ���Ϊ������ˣ��ͻ᷵��0.
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
	//Ҷ�ӽ��
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
	//�����
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


