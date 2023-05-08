#pragma once
#include <vector>
#include <map>

using namespace std;

namespace matrix
{
	template<class V, class W, W MAX_W = INT_MAX, bool Direction = false>
	class Graph
	{
	public:
		// 图的创建
		// 1. IO输入  -- 不方便测试 oj中更适合
		// 2. 图结果关系写到文件 读取文件
		// 3. 手动添加边
		Graph(const V* a, size_t n)
		{
			_vertexs.reserve(n);
			for (size_t i = 0; i < n; ++i)
			{
				_vertexs.push_back(a[i]);
				_indexMap[a[i]] = i;
			}

			_matrix.resize(n);
			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				_matrix[i].resize(n, MAX_W);
			}
		}

		// 顶点的下标
		size_t GetVertexIndex(const V& v)
		{
			auto it = _indexMap.find(v);
			if (it != _indexMap.end())
			{
				return it->second;
			}
			else
			{
				throw invalid_argument("顶点不存在");
				return -1;
			}
		}

		void AddEdge(const V& src, const V& dst, const W& w)
		{
			size_t srci = GetVertexIndex(src);
			size_t dsti = GetVertexIndex(dst);

			_vertexs[srci][dsti] = w;
			// 无向图
			if (Direction == false)
			{
				_vertexs[dsti][srci] = w;
			}
		}

		void Print()
		{
			// 顶点
			for (size_t i = 0; i < n; i++)
			{
				cout << "[" << i << "]" << "->" << _vertexs[i] << endl;
			}
			cout << endl;
			// 矩阵
			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				for (size_t j = 0; j < _matrix[i].size(); ++j)
				{
					
				}
			}
		}
	private:
		vector<V> _vertexs; // 顶点集合
		map<V, int> _indexMap; // 顶点映射下标 
		vector<vector<W>> _matrix; // 邻接矩阵
	};
}
