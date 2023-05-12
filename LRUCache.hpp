#pragma once
#include <unordered_map>
#include <list>

using namespace std;

class LRUCache
{
public:
	LRUCache(int capacity)
	{
		_capacity = capacity;
	}

	int get(int key)
	{
		// 如何key对应的值存在，则list it取出，这里就可以看出hashmap的value存的是list iterator的好处，找到key
		// 也就是找到key存的值在list中的iterator，也就直接删除，再进行头插，实现O(1)的数据挪动
		auto hashit = _hashMap.find(key);
		if (hashit != _hashMap.end())
		{
			// list<pair<int, int>>::iterator;
			auto listit = hashit->second;
			pair<int, int> kv = *listit;

			_list.erase(listit);
			_list.push_front(kv);
			_hashMap[key] = _list.begin();
			return kv.second;
		}
		else
		{
			// 没找到
			return -1;
		}
	}

	void put(int key, int value)
	{
		// 1. 如果没有数据进行插入数据
		// 2. 如果有数据则进行数据更新
		auto hashit = _hashMap.find(key);
		if (hashit == _hashMap.end())
		{
			// 插入数据的时候如果数据已经到达上限，则删除链表头的数据和hashMap中的数据
			// 两个删除操作都是O(1)
			if (_list.size() >= _capacity)
			{
				_hashMap.erase(_list.back().first);
				_list.pop_back();
			}

			_list.push_front(make_pair(key, value));
			_hashMap[key] = _list.begin();
		}
		else
		{
			// 再次put，将数据挪动到list前面
			auto listit = hashit->second;
			pair<int, int> kv = *listit;
			kv.second = value;

			_list.erase(listit);
			_list.push_front(kv);
			_hashMap[key] = _list.begin();
		}
	}

private:
	list<pair<int, int>> _list; // 将最近使用过的往链表的头上移动，保持LRU
	size_t _capacity; // 容量大小，超过容量则换出，保持LRU
	unordered_map<int, list<pair<int, int>>::iterator> _hashMap;
};
