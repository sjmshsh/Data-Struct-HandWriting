#pragma once
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <iostream>
#include "UnionFindSet.h"
#include <functional>

using namespace std;

// weight
namespace matrix
{
	template<class V, class W, W MAX_W = INT_MAX, bool Direction = false>
	class Graph
	{
		typedef Graph<V, W, MAX_W, Direction> Self;
	public:
		Graph() = default;

		// 图的创建
		// 1、IO输入  -- 不方便测试,oj中更适合
		// 2、图结构关系写到文件，读取文件
		// 3、手动添加边
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

		size_t GetVertexIndex(const V& v)
		{
			auto it = _indexMap.find(v);
			if (it != _indexMap.end())
			{
				return it->second;
			}
			else
			{
				//assert(false);
				throw invalid_argument("顶点不存在");

				return -1;
			}
		}


		void _AddEdge(size_t srci, size_t dsti, const W& w)
		{
			_matrix[srci][dsti] = w;
			// 无向图
			if (Direction == false)
			{
				_matrix[dsti][srci] = w;
			}
		}

		void AddEdge(const V& src, const V& dst, const W& w)
		{
			size_t srci = GetVertexIndex(src);
			size_t dsti = GetVertexIndex(dst);
			_AddEdge(srci, dsti, w);
		}

		void Print()
		{
			// 顶点
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				cout << "[" << i << "]" << "->" << _vertexs[i] << endl;
			}
			cout << endl;

			// 矩阵
			// 横下标
			cout << "  ";
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				//cout << i << " ";
				printf("%4d", i);
			}
			cout << endl;

			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				cout << i << " "; // 竖下标
				for (size_t j = 0; j < _matrix[i].size(); ++j)
				{
					//cout << _matrix[i][j] << " ";
					if (_matrix[i][j] == MAX_W)
					{
						//cout << "* ";
						printf("%4c", '*');
					}
					else
					{
						//cout << _matrix[i][j] << " ";
						printf("%4d", _matrix[i][j]);
					}
				}
				cout << endl;
			}
			cout << endl;
		}

		void DFS(const V& src)
		{
			size_t srci = GetVertexIndex(src);
			vector<bool> visted(_vertexs.size(), false);

			_DFS(srci, visted);
		}

		void _DFS(size_t srci, vector<bool>& visited)
		{
			cout << srci << ":" << _vertexs[srci] << endl;
			visited[srci] = true;

			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				if (_matrix[srci][i] != INT_MAX && visited[i] == false)
				{
					_DFS(i, visited);
				}
			}
		}

		void BFS(const V& src)
		{
			size_t srci = GetVertexIndex(src);

			// 队列和标记数组
			queue<int> q;
			vector<bool> visited(_vertexs.size(), false);

			q.push(srci);
			visited[srci] = true;
			int levelSize = 1;

			size_t n = _vertexs.size();
			while (!q.empty())
			{
				// 一层一层的出
				for (int i = 0; i < levelSize; ++i)
				{
					int front = q.front();
					q.pop();
					cout << front << ":" << _vertexs[front] << endl;
					// 把front顶点的邻接点入队列
					for (size_t i = 0; i < n; ++i)
					{
						if (_matrix[front][i] != INT_MAX)
						{
							if (visited[i] == false)
							{
								q.push(i);
								visited[i] = true;
							}
						}
					}
				}
				cout << endl;

				levelSize = q.size();
			}
			cout << endl;
		}

		struct Edge
		{
			size_t _srci;
			size_t _dsti;
			W      _w;

			Edge(size_t srci, size_t dsti, const W& w)
				:_srci(srci)
				, _dsti(dsti)
				, _w(w)
			{}

			// 这个结构体还重载了 > 运算符，用于在优先队列中进行元素的比较。
			// 因为这里使用的是 greater<Edge> 作为比较函数，所以实际上是将 > 运算符定义为“当前边的权值大于另一条边的权值时，
			// 当前边的优先级更高”。

			// 注意，这里重载的是 > 运算符而不是 < 运算符，这是因为使用 greater<Edge> 作为比较函数时，
			// 优先队列会将元素按照“大于”（greater）的方式排序，因此需要重载 > 运算符。
			// 如果使用 less<Edge> 作为比较函数，则需要重载 < 运算符。
			bool operator>(const Edge& e) const
			{
				return _w > e._w;
			}
		};

		// 带权无向连通图
		// 输出型参数 传出最小生成树
		W Kruskal(Self& minTree)
		{
			size_t n = _vertexs.size();
			minTree._vertexs = _vertexs;
			minTree._indexMap = _indexMap;
			minTree._matrix = vector<vector<W>>(n, vector<W>(n, MAX_W));

			// 将所有边的权值添加到优先级队列中
			// 代表从小到大
			priority_queue < Edge, vector<Edge>, greater<Edge>> minQ;
			for (int i = 0; i < n; ++i)
			{
				for (int j = i + 1; j < n; ++j)
				{
					if (_matrix[i][j] != MAX_W)
					{
						minQ.push(Edge(i, j, _matrix[i][j]));
					}
				}
			}

			// 这意味着将ans初始化成W类型的默认值
			// 对于大部分的内置类型，例如int，double等，默认值为0
			// 但是对于自定义类型，可以通过重载默认构造函数来指定默认值
			W ans = W();
			int size = 0;
			UnionFindSet ufs(n);
			while (!minQ.empty())
			{
				// 从优先级队列里面弹出一个最小的边
				Edge min = minQ.top();
				minQ.pop();

				// 如果不是一个集合的，意思就是如果没有连通
				if (!ufs.InSet(min._srci, min._dsti))
				{
					minTree._AddEdge(min._srci, min._dsti, min._w);
					ufs.Union(min._srci, min._dsti);
					++size;
					ans += min._w;
				}
			}

			// 如果最小生成树中包含了n - 1条边，则代表是有效的生成树
			if (size == n - 1)
			{
				return ans;
			}
			else
			{
				// 返回一个表示无解的特殊值W(), 即ans重置为W类型的默认值
				return W();
			}
		}

	private:
		vector<V> _vertexs;			// 顶点集合
		map<V, int> _indexMap;		// 顶点映射下标
		vector<vector<W>> _matrix;  // 邻接矩阵
	};

	void TestGraph1()
	{
		Graph<char, int, INT_MAX, true> g("0123", 4);
		g.AddEdge('0', '1', 1);
		g.AddEdge('0', '3', 4);
		g.AddEdge('1', '3', 2);
		g.AddEdge('1', '2', 9);
		g.AddEdge('2', '3', 8);
		g.AddEdge('2', '1', 5);
		g.AddEdge('2', '0', 3);
		g.AddEdge('3', '2', 6);

		g.Print();
	}

	void TestBDFS()
	{
		string a[] = { "张三", "李四", "王五", "赵六", "周七" };
		Graph<string, int> g1(a, sizeof(a) / sizeof(string));
		g1.AddEdge("张三", "李四", 100);
		g1.AddEdge("张三", "王五", 200);
		g1.AddEdge("王五", "赵六", 30);
		g1.AddEdge("王五", "周七", 30);
		g1.Print();

		g1.BFS("张三");
		g1.DFS("张三");
	}

	void TestGraphMinTree() {
		const char str[] = "abcdefghi";
		Graph<char, int> g(str, strlen(str));
		g.AddEdge('a', 'b', 4);
		g.AddEdge('a', 'h', 8);
		g.AddEdge('b', 'c', 8);
		g.AddEdge('b', 'h', 11);
		g.AddEdge('c', 'i', 2);
		g.AddEdge('c', 'f', 4);
		g.AddEdge('c', 'd', 7);
		g.AddEdge('d', 'f', 14);
		g.AddEdge('d', 'e', 9);
		g.AddEdge('e', 'f', 10);
		g.AddEdge('f', 'g', 2);
		g.AddEdge('g', 'h', 1);
		g.AddEdge('g', 'i', 6);
		g.AddEdge('h', 'i', 7);

		/*Graph<char, int> kminTree;
		cout << "Kruskal:" << g.Kruskal(kminTree) << endl;
		kminTree.Print();
		cout << endl << endl;*/

		Graph<char, int> pminTree;
		cout << "Prim:" << g.Kruskal(pminTree) << endl;
		pminTree.Print();
	}

}
