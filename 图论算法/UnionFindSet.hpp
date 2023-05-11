#pragma once
#include <vector>
#include <map>

using namespace std;


// 数组的下标对应集合中元素的编号
// 数组中如果为负数，负号代表根，数字代表该集合中元素个数
// 数组中如果为非负数，代表该元素双亲在数组中的下标
class UnionFindSet
{
private:
	// 默认初始化为-1
	vector<int> _ufs;
public:
	UnionFindSet(size_t n)
		:_ufs(n, -1) 
	{}

	// 给一个元素的编号，找到该元素所在集合的名称
	int FindRoot(int x)
	{
		int root = x;
		// 如何数组中存储的是负数，找到，否则一直继续
		while (_ufs[root] >= 0)
		{
			root = _ufs[root];
		}

		// 路径压缩
		// 我们把路径上的东西直接挂到root上，避免串成链表了
		while (_ufs[x] >= 0)
		{
			int parent = _ufs[x];
			_ufs[x] = root;

			x = parent;
		}


		return root;
	}

	bool InSet(int x1, int x2)
	{
		return FindRoot(x1) == FindRoot(x2);
	}

	size_t SetSize()
	{
		size_t size = 0;
		for (size_t i = 0; i < _ufs.size(); ++i)
		{
			if (_ufs[i] < 0)
			{
				size++;
			}
		}
		return size;
	}

	bool Union(int x1, int x2)
	{
		int root1 = FindRoot(x1);
		int root2 = FindRoot(x2);
		// 如果本身就在一个集合就没有必要合并了
		if (root1 == root2)
		{
			return false;
		}

		// 控制数据量小的往大的合并
		if (abs(_ufs[root1]) < abs(_ufs[root2]))
		{
			swap(root1, root2);
		}

		// 将两个集合中元素合并
		_ufs[root1] += _ufs[root2];

		// 将其中一个集合名称改成另外一个
		_ufs[root2] = root1;

		return true;
	}
};
