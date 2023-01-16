#pragma once

#include <vector>

using namespace std;

namespace bit
{
	template<size_t N>
	class bit_set
	{
	public:
		bit_set()
		{
			_bits.resize(N / 8 + 1);
		}

		// 设置为1
		void set(size_t x)
		{
			// 这个i算的是它在第几个char里面
			size_t i = x / 8;
			// 这个j算的是它是第几个位
			size_t j = x % 8;
			_bits[i] |= (1 << j);
		}

		// 清理为0
		void reset(size_t x)
		{
			size_t i = x / 8;
			size_t j = x % 8; 
			_bits[i] &= (~(1 << j));
		}

		// 探测这个位是否是1
		bool test(size_t x)
		{
			size_t i = x / 8;
			size_t j = x % 8;
			return _bits[i] & (1 << j);
		}
	private:
		vector<char> _bits;
		// vector<int> _bits;
	};

	void test_bit_set()
	{
		bit_set<100> bs;
		bs.set(5);
		bs.set(10);
		bs.set(20);
		cout << bs.test(5) << endl;
		cout << bs.test(21) << endl;
	}
}
