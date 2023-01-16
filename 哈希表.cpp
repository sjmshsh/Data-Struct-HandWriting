#pragma once
#include <vector>
#include <string>

using namespace std;


template<class K>
struct Hash
{
	size_t operator() (const K& key)
	{
		return key;
	}
};

// 如果K是stribg类型会走这个特化版本
template<>
struct Hash<string>
{
	size_t operator() (const string& s)
	{
		// BKDR
		size_t value = 0;
		for (auto ch : s)
		{
			value *= 31;
			value += ch;
		}
		return value;
	}
};


namespace CloseHash
{
	enum Status
	{
		EXIST,
		EMPTY,
		DELETE
	};

	template<class K, class V>
	struct HashData
	{
		pair<K, V> _kv;
		Status _status = EMPTY;
	};

	template<class K, class V, class HashFunc = Hash<K>>
	class HashTable
	{
	public:
		bool Erase(const K& key)
		{
			HashData<K, V>* ret = Find(key);
			if (ret == nullptr)
			{
				// 没有找到说明没有这个数据，当然是不能删除的
				return false;
			}
			else
			{
				--_n;
				ret->_status = DELETE;
				return true;
			}
		}

		HashData<K, V>* Find(const K& key)
		{
			if (_tables.size() == 0)
			{
				return nullptr;
			}
			HashFunc hf;
			size_t start = hf(key) % _tables.size();
			size_t i = 0;
			size_t index = start;
			// 线性探测 or 二次探测
			while (_tables[index]._status != EMPTY)
			{
				if (_tables[index]._kv.first == key && _tables[index]._status == EXIST)
				{
					return &_tables[index];
				}
				i++;
				index = start + i * i;
				index %= _tables.size();
			}
			return nullptr;
		}

		bool Insert(const pair<K, V>& kv)
		{
			HashData<K, V>* ret = Find(kv.first);
			if (ret)
			{
				return false;
			}
			// 负载因子到0.7
			// 负载因子越小，冲突概率就越小，效率越高，空间浪费越多
			if (_tables.size() == 0 || _n * 10 / _tables.size() >= 7)
			{
				// 扩容
				size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;
				//vector<HashData<K, V>> newTables;
				//newTables.resize(newSize);
				//// 遍历原表，把原表中的数据重新按newSize映射到新表
				//for (size_t i = 0; i < _tables.size(); i++)
				//{
				//	// 
				//}
				//// 交换内存并把以前的内存销毁
				//_tables.swap(newTables);
				HashTable<K, V> newHT;
				newHT._tables.resize(newSize);
				for (size_t i = 0; i < _tables.size(); i++)
				{
					if (_tables[i]._status == EXIST)
					{
						newHT.Insert(_tables[i]._kv);
					}
				}
				_tables.swap(newHT._tables);
			}

			HashFunc hf;
			size_t start = hf(kv.first) % _tables.size();
			size_t i = 0;
			size_t index = start;
			// 不能取模capacity
			// size_t i = kv.first % _tables.capacity();

			// 线性探测
			while (_tables[index]._status == EXIST)
			{
				i++;
				index = start + i * i;
				index %= _tables.size();
			}
			_tables[index]._kv = kv;
			_tables[index]._status = EXIST;
			++_n;
			return true;
		}
	private:
		vector<HashData<K, V>> _tables;
		size_t _n; // 有效数据的个数
	};


	struct Date
	{
	public:
		int a = 1;
	};

	struct HashDate
	{
		size_t operator() (const Date* d)
		{
			// ...
		}
	};

	void TestHashTable1()
	{
		HashTable<string, int> ht;
		ht.Insert(make_pair("lxy", 12));
		cout << ht.Find("lxy") << endl;
		ht.Erase("lxy");
		cout << ht.Find("lxy") << endl;

		// 当key是一个定义类型时，需要配置这个仿函数，将key转换成整形
		HashTable<Date, string, HashDate> htds;
	}

	void testhashtable2()
	{

	}
}

namespace LinkHash
{
	template<class K, class V>
	struct HashNode
	{
		pair<K, V> _kv;
		HashNode<K, V>* _next;

		HashNode(const pair<K, V>& kv)
			:_kv(kv)
			,_next(nullptr)
		{}
	};

	template<class K, class V, class HashFunc = Hash<K>>
	class HashTable
	{
		typedef HashNode<K, V> Node;
	public:
		bool Erase(const K& key)
		{
			if (_tables.empty())
			{
				return false;
			}
			HashFunc hf;
			size_t index = hf(key) % _tables.size();
			Node* cur = _tables[index];
			Node* prev = nullptr;
			while (cur)
			{
				if (cur->_kv.first == key)
				{
					if (prev == nullptr)
					{
						// 头删
						_tables[index] = cur->_next;
					}
					else
					{
						// 中间的地方进行删除
						prev->_next = cur->_next;
					}
					--_n;
					delete cur;
					return true;
				}
				else
				{
					prev = cur;
				}
			}
		}

		Node* Find(const K& key)
		{
			if (_tables.empty())
			{
				return nullptr;
			}
			HashFunc hf;
			size_t index = hf(key) % _tables.size();
			Node* cur = _tables[index];
			while (cur)
			{
				if (cur->_kv.first == key)
				{
					return cur;
				}
				else
				{
					cur = cur->_next;
				}
			}
			return nullptr;
		}

		bool Insert(const pair<K, V>& kv)
		{
			Node* ret = Find(kv.first);
			if (ret)
			{
				return false;
			}
			// 负载因子 == 1 时扩容
			if (_n == _tables.size())
			{
				size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;
				vector<Node*> newTables;
				newTables.resize(newSize);
				for (size_t i = 0; i < _tables.size(); i++)
				{
					HashFunc hf;
					Node* cur = _tables[i];
					while (cur)
					{
						Node* next = cur->_next;
						size_t index = hf(cur->_kv.first) % newTables.size();
						// 头插
						cur->_next = newTables[index];
						cur = next;
					}
					_tables[i] = nullptr;
				}
				newTables.swap(_tables);
			}

			HashFunc hf;
			size_t index = hf(kv.first) % _tables.size();
			Node* newnode = new Node(kv);
			// 头插
			newnode->_next = _tables[index];
			_tables[index] = newnode;

			++_n;
			return true;
		}
	private:
		//struct Data
		//{
		//	forward_list<T> _list;
		//	set<T> _rbtree;
		//	size_t _len;
		//};
		// 这里就不考虑极端场景了
		vector<Node*> _tables;
		size_t _n; // 有效数据的个数
	};

	void TestHashTable()
	{
		int a[] = { 4, 24, 14, 7,37,37,57,67,34,14,54 };
		HashTable<int, int> ht;
		for (auto e : a)
		{
			ht.Insert(make_pair(e, e));
		}
	}
}
