#pragma once
#include <iostream>
#include <cassert>

using namespace std;

// AVL树的节点结构
template <class K, class V>
struct AVLTreeNode
{
	AVLTreeNode(const pair<K, V>& kv)
		: _parent(nullptr)
		, _left(nullptr)
		, _right(nullptr)
		, _kv(kv)
		, _bf(0)
	{}

	AVLTreeNode<K, V>* _parent;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	pair<K, V> _kv;
	int _bf;					// 平衡因子,用于控制平衡
};

template <class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;

public:
	AVLTree()
		: _root(nullptr)
	{}

	bool Insert(const pair<K, V>& kv)
	{
		// 第一个节点
		if (_root == nullptr)
		{
			_root = new Node(kv);

			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;

		// 寻找插入位置
		while (cur != nullptr)
		{
			if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = parent->_left;
			}
			else if (cur->_kv.first < kv.first)
			{
				parent = cur;
				cur = parent->_right;
			}
			else
			{
				return false;
			}
		}

		// 将节点插入到AVL树中
		cur = new Node(kv);
		if (parent->_kv.first > kv.first)
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_right = cur;
			cur->_parent = parent;
		}

		// 更新AVL树的平衡因子控制平衡
		// 更新新增节点到其祖先的平衡因子
		// 出现异常平衡因子需要旋转处理
		while (parent != nullptr)
		{
			// 增加在左因子减小,右增加
			if (cur == parent->_left)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}

			if (parent->_bf == 0)	// 此时不需要处理
			{
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				// 继续向上更新
				cur = parent;
				parent = cur->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				// 旋转处理

				if (parent->_bf == -2 && cur->_bf == -1)	// 右单旋
				{
					RotateR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == 1)	// 左单旋
				{
					RotateL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					RotateLR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					RotateRL(parent);
				}
				else
				{
					assert(false);
				}

				break;
			}
			else
			{
				// 说明前面就出现问题了
				assert(false);
			}
		}

		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
	}

	bool IsBalance()
	{
		return _IsBalance(_root);
	}

private:
	int Height(Node* root)
	{
		if (root == nullptr)
		{
			return 0;
		}

		int leftHeight = Height(root->_left);
		int rightHeight = Height(root->_right);

		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}

	bool _IsBalance(Node* root)
	{
		if (root == nullptr)
		{
			return true;
		}

		// 对当前树进行检查
		int leftHeight = Height(root->_left);
		int rightHeight = Height(root->_right);

		if (rightHeight - leftHeight != root->_bf)
		{
			cout << root->_kv.first << "现在是:" << root->_bf << endl;
			cout << root->_kv.first << "应该是:" << rightHeight - leftHeight << endl;

			return false;
		}

		// abs用与整数求绝对值
		// 并判断每棵子树是否平衡
		return abs(rightHeight - leftHeight) < 2
			&& _IsBalance(root->_left)
			&& _IsBalance(root->_right);
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* pParent = parent->_parent;

		// 更改链接关系
		parent->_left = subLR;
		if (subLR != nullptr)
		{
			subLR->_parent = parent;
		}

		parent->_parent = subL;
		subL->_right = parent;

		if (parent == _root)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			if (pParent->_left == parent)
			{
				pParent->_left = subL;
			}
			else
			{
				pParent->_right = subL;
			}

			subL->_parent = pParent;
		}

		subL->_bf = 0;
		parent->_bf = 0;
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* pParent = parent->_parent;

		parent->_right = subRL;
		if (subRL != nullptr)
		{
			subRL->_parent = parent;
		}

		parent->_parent = subR;
		subR->_left = parent;

		if (parent == _root)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			if (pParent->_left == parent)
			{
				pParent->_left = subR;
			}
			else
			{
				pParent->_right = subR;
			}

			subR->_parent = pParent;
		}

		subR->_bf = 0;
		parent->_bf = 0;
	}

	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(subL);
		RotateR(parent);

		if (bf == 1)
		{
			parent->_bf = 0;
			subL->_bf = -1;
			subLR->_bf = 0;
		}
		else if (bf == -1)
		{
			parent->_bf = 1;
			subL->_bf = 0;
			subLR->_bf = 0;
		}
		else if (bf == 0)
		{
			parent->_bf = 0;
			subL->_bf = 0;
			subLR->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(subR);
		RotateL(parent);

		if (bf == 1)		// 新插入的节点是subRL的右子树
		{
			parent->_bf = -1;
			subR->_bf = 0;
			subRL->_bf = 0;
		}
		else if (bf == -1)	// 新插入的节点是subRL的左子树
		{
			parent->_bf = 0;
			subR->_bf = 1;
			subRL->_bf = 0;
		}
		else if (bf == 0)	// 新插入的节点为subRL时
		{
			parent->_bf = 0;
			subR->_bf = 0;
			subRL->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

private:
	Node* _root;
};

void TestAVLTree()
{
	AVLTree<int, int> t;
	int a[] = { 4,2,6,1,3,5,15,7,16,14 };
	for (auto e : a)
	{
		t.Insert(make_pair(e, e));
		cout << "Insert" << e  << ":" << t.IsBalance() << endl;
	}
	t.InOrder();
	cout << t.IsBalance() << endl;
}
