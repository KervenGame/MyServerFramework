#include "FrameHeader.h"

namespace
{
#define HMB_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("HashMapBoundaryTest FAILED: ") + (msg)); } } while(0)
#define HMB_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("HashMapBoundaryTest FAILED: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

	// ---------- 大量插入后查找 ----------
	static void testHM_BulkInsert()
	{
		HashMap<int, int> m;
		const int N = 10000;
		FOR(N)
		{
			m.add(i, i * 10);
		}
		HMB_ASSERT_EQ(m.size(), N, "bulk insert size");

		FOR(N)
		{
			int* p = m.getPtr(i);
			HMB_ASSERT(p != nullptr && *p == i * 10, "bulk lookup");
		}
	}

	// ---------- 大量插入后删除 ----------
	static void testHM_BulkInsertRemove()
	{
		HashMap<int, int> m;
		const int N = 5000;

		// 插入
		for (int i = 0; i < N; ++i)
		{
			m.add(i, i);
		}

		// 删除一半（偶数）
		int removed = 0;
		for (int i = 0; i < N; i += 2)
		{
			m.remove(i);
			++removed;
		}

		HMB_ASSERT_EQ(m.size(), N - removed, "after remove size");

		// 验证删除的 key 不存在
		for (int i = 0; i < N; i += 2)
		{
			HMB_ASSERT(!m.contains(i), "removed key not found");
		}

		// 验证保留的 key 还在
		for (int i = 1; i < N; i += 2)
		{
			int* p = m.getPtr(i);
			HMB_ASSERT(p != nullptr && *p == i, "remaining key valid");
		}
	}

	// ---------- 删除后 rehash 稳定性 ----------
	static void testHM_RemoveThenRehash()
	{
		HashMap<int, int> m;
		// 先大量插入
		FOR(2000)
		{
			m.add(i, i);
		}
		// 再几乎全部删光
		for (int i = 1; i < 2000; ++i)
		{
			m.remove(i);
		}

		HMB_ASSERT_EQ(m.size(), 1, "near-empty after mass remove");
		HMB_ASSERT(m.contains(0), "surviving key exists");

		// 重新插入，验证没有损坏
		for (int i = 100; i < 300; ++i)
		{
			m.add(i, i * 100);
		}

		HMB_ASSERT_EQ(m.size(), 201, "reinsert count correct");
		for (int i = 100; i < 300; ++i)
		{
			int* p = m.getPtr(i);
			HMB_ASSERT(p != nullptr && *p == i * 100, "post-rehash value ok");
		}
	}

	// ---------- addOrSet: 新增和更新 ----------
	static void testHM_AddOrSet()
	{
		HashMap<string, int> m;

		bool isNew1 = m.addOrSet("hello", 10);
		HMB_ASSERT(isNew1, "addOrSet new returns true");
		HMB_ASSERT_EQ(m.get("hello", -1), 10, "addOrSet new value");

		bool isNew2 = m.addOrSet("hello", 20); // 更新
		HMB_ASSERT(!isNew2, "addOrSet update returns false");
		HMB_ASSERT_EQ(m.get("hello", -1), 20, "addOrSet updated value");
	}

	// ---------- set: 已有/不存在 ----------
	static void testHM_Set()
	{
		HashMap<int, string> m;
		m.add(1, "old");

		bool ok1 = m.set(1, "new");
		HMB_ASSERT(ok1, "set existing key success");
		HMB_ASSERT(m.get(1, "") == "new", "set updated value");

		bool ok2 = m.set(999, "newkey");
		HMB_ASSERT(!ok2, "set non-existing key fails");
	}

	// ---------- getAt / get 默认值 ----------
	static void testHM_GetWithDefault()
	{
		HashMap<int, int> m;
		m.add(5, 500);

		HMB_ASSERT_EQ(m.get(5, -1), 500, "get existing");
		HMB_ASSERT_EQ(m.get(99, -1), -1, "get missing with default");

		HMB_ASSERT_EQ(m.getAt(0, -999), 500, "getAt index 0");
		HMB_ASSERT_EQ(m.getAt(5, -999), -999, "getAt out of range default");
	}

	// ---------- contains / find ----------
	static void testHM_ContainsAndFind()
	{
		HashMap<string, int> m;
		m.add("abc", 42);

		HMB_ASSERT(m.contains("abc"), "contains existing");
		HMB_ASSERT(!m.contains("xyz"), "contains missing");

		auto it = m.find("abc");
		HMB_ASSERT(it != m.end() && it->second == 42, "find existing");

		auto miss = m.find("nope");
		HMB_ASSERT(miss == m.end(), "find missing");
	}

	// ---------- clear 后复用 ----------
	static void testHM_ClearAndReuse()
	{
		HashMap<int, int> m;
		FOR(100)
		{
			m.add(i, i);
		}
		m.clear();
		HMB_ASSERT(m.isEmpty(), "clear empties map");
		HMB_ASSERT_EQ(m.size(), 0, "clear size=0");

		// 复用
		for (int i = 200; i < 210; ++i)
		{
			m.add(i, i * 2);
		}
		HMB_ASSERT_EQ(m.size(), 10, "reuse after clear");
		for (int i = 200; i < 210; ++i)
		{
			HMB_ASSERT(m.contains(i), "reuse element exists");
		}
	}

	// ---------- string key ----------
	static void testHM_StringKey()
	{
		HashMap<string, string> m;
		m.add("key1", "val1");
		m.add("key2", "val2");
		m.add("key3", "val3");

		HMB_ASSERT_EQ(m.size(), 3, "string key size");
		HMB_ASSERT(m.get("key1", "") == "val1", "string key val1");
		HMB_ASSERT(m.get("key3", "") == "val3", "string key val3");
		HMB_ASSERT(!m.contains("key4"), "string key not found");
	}

	// ---------- 拷贝构造 / 移动构造 ----------
	static void testHM_CopyMove()
	{
		HashMap<int, int> src;
		src.add(1, 10);
		src.add(2, 20);
		src.add(3, 30);

		// 移动构造
		HashMap<int, int> dst(move(src));
		HMB_ASSERT_EQ(dst.size(), 3, "move construct size");
		HMB_ASSERT_EQ(dst.get(1, -1), 10, "move data intact");
		HMB_ASSERT(src.isEmpty() || true, "source may be empty after move"); // 不强制

		// 拷贝构造
		HashMap<int, int> copy;
		dst.cloneTo(copy);
		HMB_ASSERT_EQ(copy.size(), 3, "copy construct size");
		HMB_ASSERT(copy.contains(1) && copy.contains(2) && copy.contains(3), "copy data intact");
	}

	// ---------- 迭代器遍历 ----------
	static void testHM_Iteration()
	{
		HashMap<int, int> m;
		const int N = 100;
		FOR(N)
		{
			m.add(i, i * 7);
		}

		int count = 0;
		int sumKey = 0;
		int sumVal = 0;
		for (auto& pair : m)
		{
			sumKey += pair.first;
			sumVal += pair.second;
			++count;
		}
		HMB_ASSERT_EQ(count, N, "iteration count");
		// sum of 0..99 = 4950
		HMB_ASSERT_EQ(sumKey, 4950, "iteration sum keys");
		// sum of i*7 for i in 0..99 = 7 * 4950 = 34650
		HMB_ASSERT_EQ(sumVal, 34650, "iteration sum values");
	}

	// ---------- llong key ----------
	static void testHM_LLongKey()
	{
		HashMap<llong, double> m;
		llong k1 = 123456789012LL;
		llong k2 = 987654321098LL;
		m.add(k1, 3.14);
		m.add(k2, 2.71);

		double* p1 = m.getPtr(k1);
		double* p2 = m.getPtr(k2);
		HMB_ASSERT(p1 != nullptr && abs(*p1 - 3.14) < 1e-6, "llong key val1");
		HMB_ASSERT(p2 != nullptr && abs(*p2 - 2.71) < 1e-6, "llong key val2");
	}

	// ---------- 重复 add 失败 ----------
	static void testHM_DuplicateAddFails()
	{
		HashMap<int, int> m;
		HMB_ASSERT(m.add(1, 10), "first add succeeds");
		HMB_ASSERT(!m.add(1, 20), "duplicate add fails");
		HMB_ASSERT_EQ(m.get(1, -1), 10, "value unchanged on dup add");
	}
}

void HashMapBoundaryTest::test()
{
	testHM_BulkInsert();
	testHM_BulkInsertRemove();
	testHM_RemoveThenRehash();
	testHM_AddOrSet();
	testHM_Set();
	testHM_GetWithDefault();
	testHM_ContainsAndFind();
	testHM_ClearAndReuse();
	testHM_StringKey();
	testHM_CopyMove();
	testHM_Iteration();
	testHM_LLongKey();
	testHM_DuplicateAddFails();
}
