#include "FrameHeader.h"

#define SCT_ASSERT(expr) \
	if (!(expr)) \
	{ \
		LOG("SafeContainerTest FAILED: " #expr); \
	}

//==============================================================
// SafeHashMap 测试
// 核心语义：支持遍历期间安全增删，结束后同步到 mUpdateList
//==============================================================

void SafeContainerTest::testSafeHashMapBasicInsert()
{
	SafeHashMap<int, int> m;
	SCT_ASSERT(m.size() == 0);
	SCT_ASSERT(m.isEmpty());
	SCT_ASSERT(m.add(1, 100));
	SCT_ASSERT(m.add(2, 200));
	SCT_ASSERT(m.add(3, 300));
	SCT_ASSERT(m.size() == 3);
	SCT_ASSERT(!m.isEmpty());
	// 重复 key 插入失败
	SCT_ASSERT(!m.add(1, 999));
}

void SafeContainerTest::testSafeHashMapBasicErase()
{
	SafeHashMap<int, int> m;
	m.add(10, 1000);
	m.add(20, 2000);
	m.add(30, 3000);
	SCT_ASSERT(m.remove(20));
	SCT_ASSERT(m.size() == 2);
	SCT_ASSERT(!m.contains(20));
	// 删除不存在的 key 返回 false
	SCT_ASSERT(!m.remove(999));
}

void SafeContainerTest::testSafeHashMapContains()
{
	SafeHashMap<int, int> m;
	m.add(5, 50);
	SCT_ASSERT(m.contains(5));
	SCT_ASSERT(!m.contains(6));
}

void SafeContainerTest::testSafeHashMapTryGet()
{
	SafeHashMap<int, int> m;
	m.add(7, 77);
	const int& v = m.get(7);
	SCT_ASSERT(v == 77);
	const int& def = m.get(999, -1);
	SCT_ASSERT(def == -1);
}

void SafeContainerTest::testSafeHashMapGetPtr()
{
	SafeHashMap<int, int> m;
	m.add(42, 420);
	int* p = m.getPtr(42);
	SCT_ASSERT(p != nullptr);
	SCT_ASSERT(*p == 420);
	int* np = m.getPtr(99);
	SCT_ASSERT(np == nullptr);
}

void SafeContainerTest::testSafeHashMapClear()
{
	SafeHashMap<int, int> m;
	m.add(1, 1);
	m.add(2, 2);
	m.clear();
	SCT_ASSERT(m.isEmpty());
	SCT_ASSERT(!m.contains(1));
}

void SafeContainerTest::testSafeHashMapInsertDuringForeach()
{
	SafeHashMap<int, int> m;
	m.add(1, 10);
	m.add(2, 20);
	m.add(3, 30);

	// 遍历期间插入 key=4
	{
		const HashMap<int, int>& readList = m.startForeach();
		SCT_ASSERT(m.isForeaching());
		int sum = 0;
		for (const auto& pair : readList)
		{
			sum += pair.second;
		}
		SCT_ASSERT(sum == 60);
		// 遍历中插入（会记录到 mModifyList，不立即同步到 readList）
		m.add(4, 40);
		SCT_ASSERT(m.contains(4));   // mainList 已包含
		m.endForeach();
	}

	// endForeach 后 mUpdateList 已同步
	SCT_ASSERT(m.size() == 4);
	SCT_ASSERT(m.contains(4));

	// 再次遍历，新元素出现在 readList
	{
		const HashMap<int, int>& readList = m.startForeach();
		int sum = 0;
		for (const auto& pair : readList)
		{
			sum += pair.second;
		}
		m.endForeach();
		SCT_ASSERT(sum == 100);
	}
}

void SafeContainerTest::testSafeHashMapEraseDuringForeach()
{
	SafeHashMap<int, int> m;
	m.add(1, 10);
	m.add(2, 20);
	m.add(3, 30);

	{
		const HashMap<int, int>& readList = m.startForeach();
		int cnt = readList.size();
		SCT_ASSERT(cnt == 3);
		// 遍历中删除 key=2
		m.remove(2);
		SCT_ASSERT(!m.contains(2));  // mainList 已删除
		m.endForeach();
	}

	// endForeach 后同步
	SCT_ASSERT(m.size() == 2);
	SCT_ASSERT(!m.contains(2));

	// 再次遍历，readList 里没有 key=2
	{
		const HashMap<int, int>& readList = m.startForeach();
		bool found2 = false;
		for (const auto& pair : readList)
		{
			if (pair.first == 2)
			{
				found2 = true;
			}
		}
		m.endForeach();
		SCT_ASSERT(!found2);
	}
}

void SafeContainerTest::testSafeHashMapInsertAndEraseDuringForeach()
{
	SafeHashMap<int, int> m;
	for (int i = 1; i <= 5; ++i)
	{
		m.add(i, i * 10);
	}

	{
		const HashMap<int, int>& readList = m.startForeach();
		for (const auto& pair : readList)
		{
			(void)pair;
		}
		m.add(6, 60);
		m.remove(3);
		m.endForeach();
	}

	SCT_ASSERT(m.size() == 5);
	SCT_ASSERT(m.contains(6));
	SCT_ASSERT(!m.contains(3));
}

void SafeContainerTest::testSafeHashMapLargeModifyDuringForeach()
{
	// 修改数量 >= mainCount 时走 cloneTo 分支
	SafeHashMap<int, int> m;
	for (int i = 0; i < 5; ++i)
	{
		m.add(i, i);
	}

	{
		const HashMap<int, int>& readList = m.startForeach();
		for (const auto& pair : readList)
		{
			(void)pair;
		}
		// 修改5次（等于 mainCount=5），触发 cloneTo 分支
		for (int i = 10; i < 15; ++i)
		{
			m.add(i, i);
		}
		m.endForeach();
	}

	SCT_ASSERT(m.size() == 10);
	for (int i = 10; i < 15; ++i)
	{
		SCT_ASSERT(m.contains(i));
	}
}

void SafeContainerTest::testSafeHashMapForeachEmpty()
{
	SafeHashMap<int, int> m;

	{
		int cnt = m.startForeach().size();
		SCT_ASSERT(cnt == 0);
		m.add(99, 990);
		m.endForeach();
	}

	// mainList 清空后同步：mUpdateList.clear()
	SCT_ASSERT(m.size() == 1);

	// 再遍历应看到 99
	{
		int sum = 0;
		for (const auto& pair : m.startForeach())
		{
			sum += pair.second;
		}
		m.endForeach();
		SCT_ASSERT(sum == 990);
	}
}

void SafeContainerTest::testSafeHashMapSizeAndEmpty()
{
	SafeHashMap<int, int> m;
	SCT_ASSERT(m.isEmpty());
	m.add(1, 1);
	SCT_ASSERT(!m.isEmpty());
	SCT_ASSERT(m.size() == 1);
	m.remove(1);
	SCT_ASSERT(m.isEmpty());
}

void SafeContainerTest::testSafeHashMapMainListAccess()
{
	SafeHashMap<int, int> m;
	m.add(100, 1000);
	const HashMap<int, int>& ml = m.getMainList();
	SCT_ASSERT(ml.contains(100));
	const int* vp = ml.getPtrConst(100);
	SCT_ASSERT(vp != nullptr);
	SCT_ASSERT(*vp == 1000);
}

void SafeContainerTest::testSafeHashMapEraseWithValue()
{
	SafeHashMap<int, int> m;
	m.add(7, 70);
	int outVal = 0;
	SCT_ASSERT(m.remove(7, outVal));
	SCT_ASSERT(outVal == 70);
	SCT_ASSERT(!m.contains(7));
}

//==============================================================
// SafeMap 测试（基于有序 Map）
//==============================================================

void SafeContainerTest::testSafeMapBasicInsert()
{
	SafeMap<int, int> m;
	SCT_ASSERT(m.isEmpty());
	SCT_ASSERT(m.add(3, 30));
	SCT_ASSERT(m.add(1, 10));
	SCT_ASSERT(m.add(2, 20));
	SCT_ASSERT(m.size() == 3);
	// 重复插入失败
	SCT_ASSERT(!m.add(1, 99));
}

void SafeContainerTest::testSafeMapBasicErase()
{
	SafeMap<int, int> m;
	m.add(5, 50);
	m.add(10, 100);
	SCT_ASSERT(m.remove(5));
	SCT_ASSERT(!m.contains(5));
	SCT_ASSERT(m.size() == 1);
	SCT_ASSERT(!m.remove(999));
}

void SafeContainerTest::testSafeMapContains()
{
	SafeMap<string, int> m;
	m.add("hello", 1);
	SCT_ASSERT(m.contains("hello"));
	SCT_ASSERT(!m.contains("world"));
}

void SafeContainerTest::testSafeMapTryGet()
{
	SafeMap<int, int> m;
	m.add(42, 420);
	const int& v = m.get(42);
	SCT_ASSERT(v == 420);
	const int& def = m.get(99, -1);
	SCT_ASSERT(def == -1);
}

void SafeContainerTest::testSafeMapClear()
{
	SafeMap<int, int> m;
	m.add(1, 1);
	m.add(2, 2);
	m.clear();
	SCT_ASSERT(m.isEmpty());
}

void SafeContainerTest::testSafeMapInsertDuringForeach()
{
	SafeMap<int, int> m;
	m.add(1, 10);
	m.add(2, 20);

	{
		int cnt = m.startForeach().size();
		SCT_ASSERT(cnt == 2);
		m.add(3, 30);
		m.endForeach();
	}

	SCT_ASSERT(m.size() == 3);
	SCT_ASSERT(m.contains(3));

	// 再次遍历包含新元素
	{
		int sum = 0;
		for (const auto& pair : m.startForeach())
		{
			sum += pair.second;
		}
		m.endForeach();
		SCT_ASSERT(sum == 60);
	}
}

void SafeContainerTest::testSafeMapEraseDuringForeach()
{
	SafeMap<int, int> m;
	m.add(1, 10);
	m.add(2, 20);
	m.add(3, 30);

	{
		for (const auto& pair : m.startForeach())
		{
			(void)pair;
		}
		m.remove(2);
		m.endForeach();
	}

	SCT_ASSERT(m.size() == 2);
	SCT_ASSERT(!m.contains(2));

	{
		bool found2 = false;
		for (const auto& pair : m.startForeach())
		{
			if (pair.first == 2)
			{
				found2 = true;
			}
		}
		m.endForeach();
		SCT_ASSERT(!found2);
	}
}

void SafeContainerTest::testSafeMapSizeAndEmpty()
{
	SafeMap<int, int> m;
	SCT_ASSERT(m.isEmpty());
	m.add(1, 1);
	SCT_ASSERT(!m.isEmpty());
	SCT_ASSERT(m.size() == 1);
}

//==============================================================
// SafeSet 测试
//==============================================================

void SafeContainerTest::testSafeSetBasicInsert()
{
	SafeSet<int> s;
	SCT_ASSERT(s.isEmpty());
	SCT_ASSERT(s.add(10));
	SCT_ASSERT(s.add(20));
	SCT_ASSERT(s.add(30));
	SCT_ASSERT(s.size() == 3);
	// 重复插入失败
	SCT_ASSERT(!s.add(10));
}

void SafeContainerTest::testSafeSetBasicErase()
{
	SafeSet<int> s;
	s.add(5);
	s.add(15);
	SCT_ASSERT(s.remove(5));
	SCT_ASSERT(!s.contains(5));
	SCT_ASSERT(s.size() == 1);
	SCT_ASSERT(!s.remove(999));
}

void SafeContainerTest::testSafeSetContains()
{
	SafeSet<string> s;
	s.add("abc");
	SCT_ASSERT(s.contains("abc"));
	SCT_ASSERT(!s.contains("xyz"));
}

void SafeContainerTest::testSafeSetClear()
{
	SafeSet<int> s;
	s.add(1);
	s.add(2);
	s.add(3);
	s.clear();
	SCT_ASSERT(s.isEmpty());
}

void SafeContainerTest::testSafeSetInsertDuringForeach()
{
	SafeSet<int> s;
	s.add(1);
	s.add(2);
	s.add(3);

	{
		const Set<int>& readList = s.startForeach();
		int sum = 0;
		for (int v : readList)
		{
			sum += v;
		}
		SCT_ASSERT(sum == 6);
		// 遍历期间插入 4
		s.add(4);
		SCT_ASSERT(s.contains(4));
		s.endForeach();
	}

	SCT_ASSERT(s.size() == 4);

	// 再次遍历包含 4
	{
		const Set<int>& readList = s.startForeach();
		int sum = 0;
		for (int v : readList)
		{
			sum += v;
		}
		s.endForeach();
		SCT_ASSERT(sum == 10);
	}
}

void SafeContainerTest::testSafeSetEraseDuringForeach()
{
	SafeSet<int> s;
	s.add(10);
	s.add(20);
	s.add(30);

	{
		const Set<int>& readList = s.startForeach();
		for (int v : readList)
		{
			(void)v;
		}
		s.remove(20);
		s.endForeach();
	}

	SCT_ASSERT(s.size() == 2);
	SCT_ASSERT(!s.contains(20));

	{
		const Set<int>& readList = s.startForeach();
		bool found20 = false;
		for (int v : readList)
		{
			if (v == 20)
			{
				found20 = true;
			}
		}
		s.endForeach();
		SCT_ASSERT(!found20);
	}
}

void SafeContainerTest::testSafeSetSizeAndEmpty()
{
	SafeSet<int> s;
	SCT_ASSERT(s.isEmpty());
	s.add(99);
	SCT_ASSERT(!s.isEmpty());
	SCT_ASSERT(s.size() == 1);
	s.remove(99);
	SCT_ASSERT(s.isEmpty());
}

void SafeContainerTest::testSafeSetInsertAndEraseDuringForeach()
{
	SafeSet<int> s;
	for (int i = 1; i <= 6; ++i)
	{
		s.add(i);
	}

	{
		const Set<int>& readList = s.startForeach();
		for (int v : readList)
		{
			(void)v;
		}
		s.add(7);
		s.add(8);
		s.remove(3);
		s.remove(4);
		s.endForeach();
	}

	SCT_ASSERT(s.size() == 6); // 6 - 2 + 2 = 6
	SCT_ASSERT(s.contains(7));
	SCT_ASSERT(s.contains(8));
	SCT_ASSERT(!s.contains(3));
	SCT_ASSERT(!s.contains(4));
}

void SafeContainerTest::test()
{
	// SafeHashMap
	testSafeHashMapBasicInsert();
	testSafeHashMapBasicErase();
	testSafeHashMapContains();
	testSafeHashMapTryGet();
	testSafeHashMapGetPtr();
	testSafeHashMapClear();
	testSafeHashMapInsertDuringForeach();
	testSafeHashMapEraseDuringForeach();
	testSafeHashMapInsertAndEraseDuringForeach();
	testSafeHashMapLargeModifyDuringForeach();
	testSafeHashMapForeachEmpty();
	testSafeHashMapSizeAndEmpty();
	testSafeHashMapMainListAccess();
	testSafeHashMapEraseWithValue();
	// SafeMap
	testSafeMapBasicInsert();
	testSafeMapBasicErase();
	testSafeMapContains();
	testSafeMapTryGet();
	testSafeMapClear();
	testSafeMapInsertDuringForeach();
	testSafeMapEraseDuringForeach();
	testSafeMapSizeAndEmpty();
	// SafeSet
	testSafeSetBasicInsert();
	testSafeSetBasicErase();
	testSafeSetContains();
	testSafeSetClear();
	testSafeSetInsertDuringForeach();
	testSafeSetEraseDuringForeach();
	testSafeSetSizeAndEmpty();
	testSafeSetInsertAndEraseDuringForeach();
}
