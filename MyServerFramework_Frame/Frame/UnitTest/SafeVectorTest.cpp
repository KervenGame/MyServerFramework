#include "FrameHeader.h"
struct SafeVectorTestImpl
{

	// SafeVectorTestImpl_SV_ASSERT: SafeVectorTest 专用断言宏
#define SafeVectorTestImpl_SV_ASSERT(expr) \
	if (!(expr)) { ERROR("SafeVectorTest FAILED: " #expr); }

// ========== SafeVector 基本操作 ==========
	static void testSafeVectorBasic()
	{
		SafeVector<int> sv;
		SafeVectorTestImpl_SV_ASSERT(sv.isEmpty());
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 0);
		SafeVectorTestImpl_SV_ASSERT(!sv.contains(1));

		// add
		sv.add(10);
		sv.add(20);
		sv.add(30);
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(!sv.isEmpty());
		SafeVectorTestImpl_SV_ASSERT(sv.contains(10));
		SafeVectorTestImpl_SV_ASSERT(sv.contains(20));
		SafeVectorTestImpl_SV_ASSERT(sv.contains(30));
		SafeVectorTestImpl_SV_ASSERT(!sv.contains(99));

		// getMainList
		const auto& ml = sv.getMainList();
		SafeVectorTestImpl_SV_ASSERT(ml.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(ml[0] == 10);
		SafeVectorTestImpl_SV_ASSERT(ml[1] == 20);
		SafeVectorTestImpl_SV_ASSERT(ml[2] == 30);

		// removeAt
		SafeVectorTestImpl_SV_ASSERT(sv.removeAt(1));  // 移除20
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(!sv.contains(20));
		SafeVectorTestImpl_SV_ASSERT(!sv.removeAt(5));  // 越界返回 false

		// remove
		sv.add(40);
		SafeVectorTestImpl_SV_ASSERT(sv.remove(40));
		SafeVectorTestImpl_SV_ASSERT(!sv.contains(40));
		SafeVectorTestImpl_SV_ASSERT(!sv.remove(99));  // 不存在

	}

	// ========== SafeVector clear ==========
	static void testSafeVectorClear()
	{
		SafeVector<int> sv;
		FOR(10)
		{
			sv.add(i);
		}
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 10);
		sv.clear();
		SafeVectorTestImpl_SV_ASSERT(sv.isEmpty());
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 0);

		// 再次 push
		sv.add(1);
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 1);

	}

	// ========== SafeVector 非遍历时 push/remove 同步到 updateList ==========
	static void testSafeVectorNonForeachSync()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);
		// 非遍历时 startForeach 返回的 updateList 应与 mainList 一致
		const auto& readList = sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 3);
		sv.endForeach();

		// 移除后再 startForeach
		sv.remove(2);
		const auto& readList2 = sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList2.size() == 2);
		sv.endForeach();

	}

	// ========== SafeVector 遍历时 add ==========
	static void testSafeVectorForeachPush()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);

		// 在 foreach 中添加元素：mainList 立即更新，updateList 本次不包含新加的
		const auto& readList = sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(sv.isForeaching());

		// 在遍历中添加
		sv.add(100);
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 4);     // mainList 已更新
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 3);  // updateList 本次不变

		sv.endForeach();
		// 遍历结束后 sync，updateList 应与 mainList 一致
		const auto& readList2 = sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList2.size() == 4);
		SafeVectorTestImpl_SV_ASSERT(sv.contains(100));
		sv.endForeach();

		SafeVectorTestImpl_SV_ASSERT(!sv.isForeaching());
	}

	// ========== SafeVector 遍历时 remove ==========
	static void testSafeVectorForeachErase()
	{
		SafeVector<int> sv;
		sv.add(10);
		sv.add(20);
		sv.add(30);
		sv.add(40);

		const auto& readList = sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 4);

		// 在遍历中移除：mainList 立即更新，updateList 元素被 reset 为默认值
		sv.remove(20);
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 3);  // mainList 已删除
		// updateList 由于在遍历中，元素只是被 reset 为 0（int 的默认值）

		sv.endForeach();
		// endForeach 后 clearDefaultElement 清理 reset 的元素
		// 再次 startForeach 应得到与 mainList 一致的数据
		const auto& readList2 = sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList2.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(!sv.contains(20));
		sv.endForeach();

	}

	// ========== SafeVector 遍历时 clear ==========
	static void testSafeVectorForeachClear()
	{
		SafeVector<int> sv;
		for (int i = 1; i <= 5; ++i)
		{
			sv.add(i);
		}

		const auto& readList = sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 5);

		sv.clear();  // 遍历中清空
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 0);

		sv.endForeach();
		SafeVectorTestImpl_SV_ASSERT(sv.isEmpty());
		const auto& readList2 = sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList2.size() == 0);
		sv.endForeach();
	}

	// ========== SafeVector data() ==========
	static void testSafeVectorData()
	{
		SafeVector<int> sv;
		sv.add(5);
		sv.add(10);
		sv.add(15);

		int* ptr = sv.data();
		SafeVectorTestImpl_SV_ASSERT(ptr[0] == 5 && ptr[1] == 10 && ptr[2] == 15);
	}

	// ========== SafeVector stress ==========
	static void testSafeVectorStress()
	{
		SafeVector<int> sv;
		// 批量插入 100 个
		FOR(100)
		{
			sv.add(i);
		}
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 100);

		// 逐一删除奇数
		for (int i = 1; i < 100; i += 2)
		{
			sv.remove(i);
		}
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 50);
		for (int i = 1; i < 100; i += 2)
		{
			SafeVectorTestImpl_SV_ASSERT(!sv.contains(i));
		}
		for (int i = 0; i < 100; i += 2)
		{
			SafeVectorTestImpl_SV_ASSERT(sv.contains(i));
		}

		// 遍历中批量插入
		const auto& readList = sv.startForeach();
		int countBefore = readList.size();  // 50
		for (int i = 200; i < 210; ++i)
		{
			sv.add(i);
		}
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 60);
		SafeVectorTestImpl_SV_ASSERT(readList.size() == (int)countBefore);  // updateList 未变
		sv.endForeach();

		// 遍历结束后同步
		const auto& readList2 = sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList2.size() == 60);
		sv.endForeach();

	}

	// ========== SafeHashMap 基本操作 ==========
	static void testSafeHashMapBasic()
	{
		SafeHashMap<int, string> shm;
		SafeVectorTestImpl_SV_ASSERT(shm.isEmpty());
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 0);
		SafeVectorTestImpl_SV_ASSERT(!shm.contains(1));

		// add
		SafeVectorTestImpl_SV_ASSERT(shm.add(1, "one"));
		SafeVectorTestImpl_SV_ASSERT(shm.add(2, "two"));
		SafeVectorTestImpl_SV_ASSERT(shm.add(3, "three"));
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(shm.contains(1));
		SafeVectorTestImpl_SV_ASSERT(shm.contains(2));
		SafeVectorTestImpl_SV_ASSERT(!shm.contains(99));

		// 重复 add 返回 false
		SafeVectorTestImpl_SV_ASSERT(!shm.add(1, "ONE"));
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 3);

		// getPtr
		const string* p = shm.getPtrConst(1);
		SafeVectorTestImpl_SV_ASSERT(p != nullptr && *p == "one");
		SafeVectorTestImpl_SV_ASSERT(shm.getPtrConst(99) == nullptr);

		// get
		const string& val = shm.get(2, string("default"));
		SafeVectorTestImpl_SV_ASSERT(val == "two");
		// 注意：get 返回 const ref，不能对临时 defaultValue 使用；key 不存在时用 getPtr 判空
		SafeVectorTestImpl_SV_ASSERT(shm.getPtr(99) == nullptr);  // 确认 key=99 不存在

		// remove
		SafeVectorTestImpl_SV_ASSERT(shm.remove(2));
		SafeVectorTestImpl_SV_ASSERT(!shm.contains(2));
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(!shm.remove(99));

	}

	// ========== SafeHashMap clear ==========
	static void testSafeHashMapClear()
	{
		SafeHashMap<int, int> shm;
		FOR(10)
		{
			shm.add(i, i * 10);
		}
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 10);
		shm.clear();
		SafeVectorTestImpl_SV_ASSERT(shm.isEmpty());

		// 再次插入
		shm.add(1, 100);
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 1);
	}

	// ========== SafeHashMap 遍历中 add ==========
	static void testSafeHashMapForeachInsert()
	{
		SafeHashMap<int, int> shm;
		shm.add(1, 10);
		shm.add(2, 20);

		const auto& readList = shm.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(shm.isForeaching());

		// 遍历中插入：mainList 立即更新，updateList 本次不变
		shm.add(3, 30);
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 2);

		shm.endForeach();
		// 结束后 sync
		const auto& readList2 = shm.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList2.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(shm.contains(3));
		shm.endForeach();

	}

	// ========== SafeHashMap 遍历中 remove ==========
	static void testSafeHashMapForeachErase()
	{
		SafeHashMap<int, int> shm;
		shm.add(1, 10);
		shm.add(2, 20);
		shm.add(3, 30);

		const auto& readList = shm.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 3);

		shm.remove(2);
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 3);  // updateList 未同步

		shm.endForeach();
		const auto& readList2 = shm.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList2.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(!shm.contains(2));
		shm.endForeach();

	}

	// ========== SafeHashMap remove with value out ==========
	static void testSafeHashMapEraseWithValue()
	{
		SafeHashMap<int, int> shm;
		shm.add(10, 100);
		shm.add(20, 200);

		int outVal = 0;
		SafeVectorTestImpl_SV_ASSERT(shm.remove(10, outVal));
		SafeVectorTestImpl_SV_ASSERT(outVal == 100);
		SafeVectorTestImpl_SV_ASSERT(!shm.contains(10));

		// 不存在的 key
		SafeVectorTestImpl_SV_ASSERT(!shm.remove(99, outVal));

	}

	// ========== SafeMap 基本操作 ==========
	static void testSafeMapBasic()
	{
		SafeMap<int, int> sm;
		SafeVectorTestImpl_SV_ASSERT(sm.isEmpty());
		SafeVectorTestImpl_SV_ASSERT(sm.size() == 0);

		SafeVectorTestImpl_SV_ASSERT(sm.add(3, 30));
		SafeVectorTestImpl_SV_ASSERT(sm.add(1, 10));
		SafeVectorTestImpl_SV_ASSERT(sm.add(2, 20));
		SafeVectorTestImpl_SV_ASSERT(sm.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(sm.contains(1));
		SafeVectorTestImpl_SV_ASSERT(sm.contains(2));
		SafeVectorTestImpl_SV_ASSERT(sm.contains(3));
		SafeVectorTestImpl_SV_ASSERT(!sm.contains(99));

		// 重复 add
		SafeVectorTestImpl_SV_ASSERT(!sm.add(1, 100));

		// getPtr
		int* p = sm.getPtr(2);
		SafeVectorTestImpl_SV_ASSERT(p != nullptr && *p == 20);
		SafeVectorTestImpl_SV_ASSERT(sm.getPtr(99) == nullptr);

		// get
		const int& v = sm.get(3, -1);
		SafeVectorTestImpl_SV_ASSERT(v == 30);
		const int& v2 = sm.get(99, -1);
		SafeVectorTestImpl_SV_ASSERT(v2 == -1);

		// remove
		SafeVectorTestImpl_SV_ASSERT(sm.remove(2));
		SafeVectorTestImpl_SV_ASSERT(!sm.contains(2));
		SafeVectorTestImpl_SV_ASSERT(sm.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(!sm.remove(99));

	}

	// ========== SafeMap 遍历中 add/remove ==========
	static void testSafeMapForeachModify()
	{
		SafeMap<int, string> sm;
		sm.add(1, "a");
		sm.add(2, "b");

		const auto& readList = sm.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 2);

		sm.add(3, "c");  // 遍历中插入
		SafeVectorTestImpl_SV_ASSERT(sm.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 2);  // updateList 未变

		sm.remove(1);  // 遍历中删除
		SafeVectorTestImpl_SV_ASSERT(sm.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(readList.size() == 2);

		sm.endForeach();
		const auto& readList2 = sm.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readList2.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(!sm.contains(1));
		SafeVectorTestImpl_SV_ASSERT(sm.contains(3));
		sm.endForeach();

	}

	// ========== SafeMap clear ==========
	static void testSafeMapClear()
	{
		SafeMap<int, float> sm;
		FOR(5)
		{
			sm.add(i, (float)i);
		}
		SafeVectorTestImpl_SV_ASSERT(sm.size() == 5);
		sm.clear();
		SafeVectorTestImpl_SV_ASSERT(sm.isEmpty());
	}

	// ========== SafeSet 基本操作 ==========
	static void testSafeSetBasic()
	{
		SafeSet<int> ss;
		SafeVectorTestImpl_SV_ASSERT(ss.isEmpty());
		SafeVectorTestImpl_SV_ASSERT(ss.size() == 0);

		SafeVectorTestImpl_SV_ASSERT(ss.add(10));
		SafeVectorTestImpl_SV_ASSERT(ss.add(20));
		SafeVectorTestImpl_SV_ASSERT(ss.add(30));
		SafeVectorTestImpl_SV_ASSERT(ss.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(ss.contains(10));
		SafeVectorTestImpl_SV_ASSERT(!ss.contains(99));

		// 重复 add
		SafeVectorTestImpl_SV_ASSERT(!ss.add(10));
		SafeVectorTestImpl_SV_ASSERT(ss.size() == 3);

		// getMainList
		const auto& ml = ss.getMainList();
		SafeVectorTestImpl_SV_ASSERT(ml.size() == 3);

	}

	// ========== SafeSet remove ==========
	static void testSafeSetErase()
	{
		SafeSet<int> ss;
		ss.add(1);
		ss.add(2);
		ss.add(3);

		SafeVectorTestImpl_SV_ASSERT(ss.remove(2));
		SafeVectorTestImpl_SV_ASSERT(!ss.contains(2));
		SafeVectorTestImpl_SV_ASSERT(ss.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(!ss.remove(99));

	}

	// ========== SafeSet 遍历中 add ==========
	static void testSafeSetForeachInsert()
	{
		SafeSet<int> ss;
		ss.add(1);
		ss.add(2);
		ss.add(3);

		const auto& readSet = ss.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readSet.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(ss.isForeaching());

		ss.add(100);
		SafeVectorTestImpl_SV_ASSERT(ss.size() == 4);
		SafeVectorTestImpl_SV_ASSERT(readSet.size() == 3);  // updateList 未变

		ss.endForeach();
		const auto& readSet2 = ss.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readSet2.size() == 4);
		SafeVectorTestImpl_SV_ASSERT(ss.contains(100));
		ss.endForeach();

	}

	// ========== SafeSet 遍历中 remove ==========
	static void testSafeSetForeachErase()
	{
		SafeSet<int> ss;
		ss.add(10);
		ss.add(20);
		ss.add(30);

		const auto& readSet = ss.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readSet.size() == 3);

		ss.remove(20);
		SafeVectorTestImpl_SV_ASSERT(ss.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(readSet.size() == 3);  // updateList 本次未变

		ss.endForeach();
		const auto& readSet2 = ss.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readSet2.size() == 2);
		SafeVectorTestImpl_SV_ASSERT(!ss.contains(20));
		ss.endForeach();

	}

	// ========== SafeSet clear ==========
	static void testSafeSetClear()
	{
		SafeSet<int> ss;
		FOR(8)
		{
			ss.add(i);
		}
		SafeVectorTestImpl_SV_ASSERT(ss.size() == 8);
		ss.clear();
		SafeVectorTestImpl_SV_ASSERT(ss.isEmpty());
	}

	// ========== SafeSet 非遍历 startForeach 同步 ==========
	static void testSafeSetForeachSync()
	{
		SafeSet<float> ss;
		ss.add(1.0f);
		ss.add(2.0f);
		ss.add(3.0f);

		const auto& readSet = ss.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readSet.size() == 3);
		ss.endForeach();

		ss.remove(1.0f);
		const auto& readSet2 = ss.startForeach();
		SafeVectorTestImpl_SV_ASSERT(readSet2.size() == 2);
		ss.endForeach();

	}

	// ========== SafeVector isForeaching 状态变化 ==========
	static void testSafeVectorForeachState()
	{
		SafeVector<int> sv;
		SafeVectorTestImpl_SV_ASSERT(!sv.isForeaching());
		sv.startForeach();
		SafeVectorTestImpl_SV_ASSERT(sv.isForeaching());
		sv.endForeach();
		SafeVectorTestImpl_SV_ASSERT(!sv.isForeaching());

	}

	// ========== SafeHashMap isForeaching 状态变化 ==========
	static void testSafeHashMapForeachState()
	{
		SafeHashMap<int, int> shm;
		SafeVectorTestImpl_SV_ASSERT(!shm.isForeaching());
		shm.startForeach();
		SafeVectorTestImpl_SV_ASSERT(shm.isForeaching());
		shm.endForeach();
		SafeVectorTestImpl_SV_ASSERT(!shm.isForeaching());

	}

	// ========== SafeVector 批量 foreach + 修改循环 ==========
	static void testSafeVectorMultiForeach()
	{
		SafeVector<int> sv;
		for (int i = 1; i <= 10; ++i)
		{
			sv.add(i);
		}

		// 多次 foreach，每次删一个元素
		for (int round = 0; round < 5; ++round)
		{
			const auto& rl = sv.startForeach();
			if (!rl.isEmpty())
			{
				sv.remove(rl[0]);  // 在遍历中删第一个（读的是 updateList[0]）
			}
			sv.endForeach();
		}
		// 5轮删掉5个（每轮删的都是 updateList[0]，即上轮末尾 updateList 的第一个）
		SafeVectorTestImpl_SV_ASSERT(sv.size() == 5);

	}

	// ========== SafeHashMap stress ==========
	static void testSafeHashMapStress()
	{
		SafeHashMap<int, int> shm;
		FOR(50)
		{
			shm.add(i, i * 100);
		}
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 50);

		// 删除偶数
		for (int i = 0; i < 50; i += 2)
		{
			shm.remove(i);
		}
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 25);
		for (int i = 0; i < 50; i += 2)
		{
			SafeVectorTestImpl_SV_ASSERT(!shm.contains(i));
		}
		for (int i = 1; i < 50; i += 2)
		{
			SafeVectorTestImpl_SV_ASSERT(shm.contains(i));
		}

		// 遍历中批量插入
		const auto& rl = shm.startForeach();
		int before = rl.size();
		for (int i = 100; i < 110; ++i)
		{
			shm.add(i, i);
		}
		SafeVectorTestImpl_SV_ASSERT(shm.size() == 35);
		SafeVectorTestImpl_SV_ASSERT(rl.size() == before);
		shm.endForeach();

		const auto& rl2 = shm.startForeach();
		SafeVectorTestImpl_SV_ASSERT(rl2.size() == 35);
		shm.endForeach();

	}

	// ========== SafeMap 复杂键值对 ==========
	static void testSafeMapComplex()
	{
		SafeMap<string, int> sm;
		sm.add("apple", 1);
		sm.add("banana", 2);
		sm.add("cherry", 3);
		SafeVectorTestImpl_SV_ASSERT(sm.size() == 3);
		SafeVectorTestImpl_SV_ASSERT(sm.contains("apple"));
		SafeVectorTestImpl_SV_ASSERT(!sm.contains("grape"));

		int* p = sm.getPtr("banana");
		SafeVectorTestImpl_SV_ASSERT(p != nullptr && *p == 2);

		sm.remove("banana");
		SafeVectorTestImpl_SV_ASSERT(!sm.contains("banana"));
		SafeVectorTestImpl_SV_ASSERT(sm.size() == 2);

		// get 默认值
		const int& v = sm.get("grape", -999);
		SafeVectorTestImpl_SV_ASSERT(v == -999);

	}

	// ========== 主入口 ==========
	static void test()
	{
		testSafeVectorBasic();
		testSafeVectorClear();
		testSafeVectorNonForeachSync();
		testSafeVectorForeachPush();
		testSafeVectorForeachErase();
		testSafeVectorForeachClear();
		testSafeVectorData();
		testSafeVectorStress();
		testSafeHashMapBasic();
		testSafeHashMapClear();
		testSafeHashMapForeachInsert();
		testSafeHashMapForeachErase();
		testSafeHashMapEraseWithValue();
		testSafeMapBasic();
		testSafeMapForeachModify();
		testSafeMapClear();
		testSafeSetBasic();
		testSafeSetErase();
		testSafeSetForeachInsert();
		testSafeSetForeachErase();
		testSafeSetClear();
		testSafeSetForeachSync();
		testSafeVectorForeachState();
		testSafeHashMapForeachState();
		testSafeVectorMultiForeach();
		testSafeHashMapStress();
		testSafeMapComplex();
	}
};

struct SafeVectorDeepTestImpl
{

#define SafeVectorDeepTestImpl_SVD_ASSERT(cond, msg) \
	if (!(cond)) { ERROR(string("SafeVectorDeepTest FAIL: ") + (msg)); }

	// ---------------------------------------------------------------------------
	// 辅助：计算 SafeVector<int> 快照行数（通过 startForeach/endForeach 读取）
	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------

	static void test()
	{
		testBasicPushBack();
		testBasicEraseAt();
		testBasicEraseElement();
		testBasicClear();
		testSizeAndIsEmpty();
		testContains();
		testGetMainList();
		testForeachFlag();
		testPushBackDuringForeach();
		testEraseAtDuringForeach();
		testEraseElementDuringForeach();
		testClearDuringForeach();
		testMoveMainListToOutsideForeach();
		testMoveMainListToDuringForeach();
		testMultiplePushBackDuringForeach();
		testMultipleEraseAtDuringForeach();
		testMixedAddRemoveDuringForeach();
		testLargeModifyTriggersCloneTo();
		testEraseAtOutOfBounds();
		testEraseElementNotFound();
		testSyncAfterForeach();
		testUpdateListConsistency();
		testDataPointer();
		testDestructorCleansUp();
		testResetElementDuringForeach();
		testHasResetCountCleared();
		testMultipleForeachCycles();
		testEraseAllDuringForeach();
		testPushBackThenClearDuringForeach();
		runExtraTests();
	}

	// ---------------------------------------------------------------------------

	static void testBasicPushBack()
	{
		SafeVector<int> sv;
		sv.add(10);
		sv.add(20);
		sv.add(30);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 3, "testBasicPushBack size");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.getMainList()[0] == 10, "testBasicPushBack [0]");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.getMainList()[1] == 20, "testBasicPushBack [1]");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.getMainList()[2] == 30, "testBasicPushBack [2]");
	}

	static void testBasicEraseAt()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);
		bool ok = sv.removeAt(1);
		SafeVectorDeepTestImpl_SVD_ASSERT(ok, "testBasicEraseAt return true");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 2, "testBasicEraseAt size after remove");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.getMainList()[0] == 1, "testBasicEraseAt [0]");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.getMainList()[1] == 3, "testBasicEraseAt [1]");
	}

	static void testBasicEraseElement()
	{
		SafeVector<int> sv;
		sv.add(100);
		sv.add(200);
		sv.add(300);
		bool ok = sv.remove(200);
		SafeVectorDeepTestImpl_SVD_ASSERT(ok, "testBasicEraseElement return true");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 2, "testBasicEraseElement size");
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.contains(200), "testBasicEraseElement not contains 200");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.contains(100), "testBasicEraseElement still has 100");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.contains(300), "testBasicEraseElement still has 300");
	}

	static void testBasicClear()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);
		sv.clear();
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "testBasicClear size");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.isEmpty(), "testBasicClear isEmpty");
	}

	static void testSizeAndIsEmpty()
	{
		SafeVector<int> sv;
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "testSizeAndIsEmpty initial size");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.isEmpty(), "testSizeAndIsEmpty initial isEmpty");
		sv.add(5);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 1, "testSizeAndIsEmpty after push size");
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.isEmpty(), "testSizeAndIsEmpty after push not empty");
		sv.removeAt(0);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "testSizeAndIsEmpty after remove size");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.isEmpty(), "testSizeAndIsEmpty after remove isEmpty");
	}

	static void testContains()
	{
		SafeVector<int> sv;
		sv.add(42);
		sv.add(99);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.contains(42), "testContains 42");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.contains(99), "testContains 99");
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.contains(0), "testContains 0 not found");
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.contains(-1), "testContains -1 not found");
	}

	static void testGetMainList()
	{
		SafeVector<int> sv;
		sv.add(7);
		sv.add(8);
		sv.add(9);
		const Vector<int>& ml = sv.getMainList();
		SafeVectorDeepTestImpl_SVD_ASSERT(ml.size() == 3, "testGetMainList size");
		SafeVectorDeepTestImpl_SVD_ASSERT(ml[0] == 7, "testGetMainList [0]");
		SafeVectorDeepTestImpl_SVD_ASSERT(ml[1] == 8, "testGetMainList [1]");
		SafeVectorDeepTestImpl_SVD_ASSERT(ml[2] == 9, "testGetMainList [2]");
	}

	static void testForeachFlag()
	{
		SafeVector<int> sv;
		sv.add(1);
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.isForeaching(), "testForeachFlag before start");
		sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.isForeaching(), "testForeachFlag during foreach");
		sv.endForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.isForeaching(), "testForeachFlag after end");
	}

	static void testPushBackDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		// 开始遍历：快照为 {1,2}
		const Vector<int>& snapshot = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 2, "testPushBackDuringForeach snapshot size");
		// 遍历期间新增
		sv.add(3);
		// mainList 已更新
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 3, "testPushBackDuringForeach mainList updated");
		// 快照仍为旧值（updateList 未同步）
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 2, "testPushBackDuringForeach snapshot unchanged");
		sv.endForeach();
		// 结束后 updateList 同步为 {1,2,3}
		const Vector<int>& snap2 = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.size() == 3, "testPushBackDuringForeach after sync");
		sv.endForeach();
	}

	static void testEraseAtDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(10);
		sv.add(20);
		sv.add(30);
		const Vector<int>& snapshot = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 3, "testEraseAtDuringForeach snapshot before");
		sv.removeAt(1); // 移除 20
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 2, "testEraseAtDuringForeach mainList");
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 3, "testEraseAtDuringForeach snapshot unchanged");
		sv.endForeach();
		// 同步后
		const Vector<int>& snap2 = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.size() == 2, "testEraseAtDuringForeach synced size");
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap2.contains(20), "testEraseAtDuringForeach 20 removed");
		sv.endForeach();
	}

	static void testEraseElementDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(100);
		sv.add(200);
		sv.add(300);
		sv.startForeach();
		// 遍历期间 remove：主列表立即移除，updateList 重置元素（不直接删除）
		sv.remove(200);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 2, "testEraseElementDuringForeach mainList size");
		sv.endForeach();
		// endForeach 后 clearDefaultElement 清理重置元素
		const Vector<int>& snap2 = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap2.contains(200), "testEraseElementDuringForeach 200 gone");
		sv.endForeach();
	}

	static void testClearDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);
		const Vector<int>& snapshot = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 3, "testClearDuringForeach snapshot before");
		sv.clear();
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "testClearDuringForeach mainList cleared");
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 3, "testClearDuringForeach snapshot unchanged during");
		sv.endForeach();
		// 同步后
		const Vector<int>& snap2 = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.size() == 0, "testClearDuringForeach synced empty");
		sv.endForeach();
	}

	static void testMoveMainListToOutsideForeach()
	{
		SafeVector<int> sv;
		sv.add(11);
		sv.add(22);
		sv.add(33);
		Vector<int> target;
		sv.moveMainListTo(target);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "testMoveMainListToOutsideForeach sv empty");
		SafeVectorDeepTestImpl_SVD_ASSERT(target.size() == 3, "testMoveMainListToOutsideForeach target size");
		SafeVectorDeepTestImpl_SVD_ASSERT(target[0] == 11, "testMoveMainListToOutsideForeach target[0]");
		SafeVectorDeepTestImpl_SVD_ASSERT(target[1] == 22, "testMoveMainListToOutsideForeach target[1]");
		SafeVectorDeepTestImpl_SVD_ASSERT(target[2] == 33, "testMoveMainListToOutsideForeach target[2]");
	}

	static void testMoveMainListToDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(5);
		sv.add(6);
		const Vector<int>& snapshot = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 2, "testMoveMainListToDuringForeach snapshot before");
		Vector<int> target;
		sv.moveMainListTo(target);
		// 主列表已被移走
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "testMoveMainListToDuringForeach sv empty");
		SafeVectorDeepTestImpl_SVD_ASSERT(target.size() == 2, "testMoveMainListToDuringForeach target size");
		sv.endForeach();
		// 同步后 updateList 也应为空
		const Vector<int>& snap2 = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.size() == 0, "testMoveMainListToDuringForeach synced empty");
		sv.endForeach();
	}

	static void testMultiplePushBackDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		const Vector<int>& snapshot = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 2, "testMultiplePushBackDuringForeach before");
		sv.add(3);
		sv.add(4);
		sv.add(5);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 5, "testMultiplePushBackDuringForeach mainList");
		sv.endForeach();
		const Vector<int>& snap2 = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.size() == 5, "testMultiplePushBackDuringForeach synced");
		sv.endForeach();
	}

	static void testMultipleEraseAtDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(10);
		sv.add(20);
		sv.add(30);
		sv.add(40);
		const Vector<int>& snapshot = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 4, "testMultipleEraseAtDuringForeach before");
		sv.removeAt(0); // 移除 10
		sv.removeAt(0); // 移除 20（原 index 1 变为 0）
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 2, "testMultipleEraseAtDuringForeach mainList");
		sv.endForeach();
		const Vector<int>& snap2 = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.size() == 2, "testMultipleEraseAtDuringForeach synced");
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap2.contains(10), "testMultipleEraseAtDuringForeach 10 gone");
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap2.contains(20), "testMultipleEraseAtDuringForeach 20 gone");
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.contains(30), "testMultipleEraseAtDuringForeach 30 remains");
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.contains(40), "testMultipleEraseAtDuringForeach 40 remains");
		sv.endForeach();
	}

	static void testMixedAddRemoveDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);
		const Vector<int>& snapshot = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 3, "testMixedAddRemoveDuringForeach before");
		sv.add(4);
		sv.removeAt(0); // 移除 1
		// mainList: {2,3,4}
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 3, "testMixedAddRemoveDuringForeach mainList");
		sv.endForeach();
		const Vector<int>& snap2 = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.size() == 3, "testMixedAddRemoveDuringForeach synced");
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap2.contains(1), "testMixedAddRemoveDuringForeach 1 gone");
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.contains(4), "testMixedAddRemoveDuringForeach 4 added");
		sv.endForeach();
	}

	static void testLargeModifyTriggersCloneTo()
	{
		// 当 modifyCount >= mainCount 时，sync() 走 cloneTo 分支
		SafeVector<int> sv;
		// 先填充 4 个元素
		sv.add(1);
		sv.add(2);
		sv.add(3);
		sv.add(4);
		const Vector<int>& snapshot = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snapshot.size() == 4, "testLargeModifyTriggersCloneTo before");
		// 修改次数（5）>= mainCount（4） → cloneTo 分支
		sv.add(5);
		sv.add(6);
		sv.removeAt(0); // 移除 1
		sv.removeAt(0); // 移除 2
		sv.add(7);
		// modifyList 有 5 项，mainCount 在开始时为 4
		sv.endForeach();
		const Vector<int>& snap2 = sv.startForeach();
		// mainList 最终为 {3,4,5,6,7}
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.size() == 5, "testLargeModifyTriggersCloneTo synced size");
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.contains(3), "testLargeModifyTriggersCloneTo has 3");
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.contains(7), "testLargeModifyTriggersCloneTo has 7");
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap2.contains(1), "testLargeModifyTriggersCloneTo 1 gone");
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap2.contains(2), "testLargeModifyTriggersCloneTo 2 gone");
		sv.endForeach();
	}

	static void testEraseAtOutOfBounds()
	{
		SafeVector<int> sv;
		sv.add(10);
		bool ok1 = sv.removeAt(-1);
		SafeVectorDeepTestImpl_SVD_ASSERT(!ok1, "testEraseAtOutOfBounds -1 returns false");
		bool ok2 = sv.removeAt(1);
		SafeVectorDeepTestImpl_SVD_ASSERT(!ok2, "testEraseAtOutOfBounds 1 returns false (size=1)");
		bool ok3 = sv.removeAt(0);
		SafeVectorDeepTestImpl_SVD_ASSERT(ok3, "testEraseAtOutOfBounds 0 returns true");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "testEraseAtOutOfBounds now empty");
	}

	static void testEraseElementNotFound()
	{
		SafeVector<int> sv;
		sv.add(10);
		bool ok = sv.remove(999);
		SafeVectorDeepTestImpl_SVD_ASSERT(!ok, "testEraseElementNotFound returns false");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 1, "testEraseElementNotFound size unchanged");
	}

	static void testSyncAfterForeach()
	{
		SafeVector<int> sv;
		sv.add(10);
		sv.add(20);
		// 第一轮：加入 30
		sv.startForeach();
		sv.add(30);
		sv.endForeach();
		// 验证快照已同步
		const Vector<int>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 3, "testSyncAfterForeach snap size");
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.contains(30), "testSyncAfterForeach contains 30");
		sv.endForeach();
	}

	static void testUpdateListConsistency()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);
		// 遍历中删除所有元素
		sv.startForeach();
		sv.removeAt(2);
		sv.removeAt(1);
		sv.removeAt(0);
		sv.endForeach();
		const Vector<int>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 0, "testUpdateListConsistency empty after delete all");
		sv.endForeach();
	}

	static void testDataPointer()
	{
		SafeVector<int> sv;
		sv.add(111);
		sv.add(222);
		const int* ptr = sv.data();
		SafeVectorDeepTestImpl_SVD_ASSERT(ptr != nullptr, "testDataPointer not null");
		SafeVectorDeepTestImpl_SVD_ASSERT(ptr[0] == 111, "testDataPointer [0]");
		SafeVectorDeepTestImpl_SVD_ASSERT(ptr[1] == 222, "testDataPointer [1]");
	}

	static void testDestructorCleansUp()
	{
		// 仅验证析构不崩溃
		{
			SafeVector<int> sv;
			sv.add(1);
			sv.add(2);
			sv.add(3);
		}
		SafeVectorDeepTestImpl_SVD_ASSERT(true, "testDestructorCleansUp no crash");
	}

	static void testResetElementDuringForeach()
	{
		// remove 在遍历期间：主列表移除，updateList 重置为默认值
		SafeVector<int> sv;
		sv.add(10);
		sv.add(20);
		sv.add(30);
		sv.startForeach();
		sv.remove(20);
		// 主列表 size 变 2
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 2, "testResetElementDuringForeach mainList size");
		sv.endForeach();
		// endForeach 之后 clearDefaultElement(1) 清除 updateList 中的重置元素
		const Vector<int>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 2, "testResetElementDuringForeach snap size");
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap.contains(20), "testResetElementDuringForeach 20 gone");
		sv.endForeach();
	}

	static void testHasResetCountCleared()
	{
		// endForeach 后 mHasResetCount 应归零
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.startForeach();
		sv.remove(1);
		sv.remove(2);
		sv.endForeach();
		// 再次遍历，不应有遗留的 resetCount
		sv.add(3);
		sv.add(4);
		const Vector<int>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 2, "testHasResetCountCleared fresh snap size");
		sv.endForeach();
	}

	static void testMultipleForeachCycles()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);
		// 第一轮：加 4
		sv.startForeach();
		sv.add(4);
		sv.endForeach();
		// 第二轮：加 5，删 1
		sv.startForeach();
		sv.add(5);
		sv.removeAt(0);
		sv.endForeach();
		// 第三轮验证
		const Vector<int>& snap = sv.startForeach();
		// mainList 最终为 {2,3,4,5}
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 4, "testMultipleForeachCycles final size");
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.contains(5), "testMultipleForeachCycles has 5");
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap.contains(1), "testMultipleForeachCycles 1 gone");
		sv.endForeach();
	}

	static void testEraseAllDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(10);
		sv.add(20);
		sv.add(30);
		sv.startForeach();
		// 从后往前删所有元素
		sv.removeAt(2);
		sv.removeAt(1);
		sv.removeAt(0);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "testEraseAllDuringForeach mainList empty");
		sv.endForeach();
		const Vector<int>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 0, "testEraseAllDuringForeach synced empty");
		sv.endForeach();
	}

	static void testPushBackThenClearDuringForeach()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.startForeach();
		sv.add(3);
		sv.add(4);
		sv.clear(); // clear 期间 foreaching，记录所有 remove 操作
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "testPushBackThenClearDuringForeach mainList empty");
		sv.endForeach();
		const Vector<int>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 0, "testPushBackThenClearDuringForeach synced empty");
		sv.endForeach();
	}

	// ---------------------------------------------------------------------------
	// 额外覆盖：SafeVector<string> 类型
	// ---------------------------------------------------------------------------

	static void testSafeVectorString()
	{
		SafeVector<string> sv;
		sv.add("hello");
		sv.add("world");
		sv.add("foo");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 3, "SaVecString size");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.contains("world"), "SaVecString contains world");
		sv.remove("hello");
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.contains("hello"), "SaVecString hello gone");
		sv.clear();
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.isEmpty(), "SaVecString clear isEmpty");
	}

	static void testSafeVectorStringForeach()
	{
		SafeVector<string> sv;
		sv.add("a");
		sv.add("b");
		sv.add("c");
		sv.startForeach();
		sv.add("d");
		sv.remove("a");
		sv.endForeach();
		const Vector<string>& snap2 = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.size() == 3, "SaVecStringForeach size");
		SafeVectorDeepTestImpl_SVD_ASSERT(snap2.contains("d"), "SaVecStringForeach has d");
		SafeVectorDeepTestImpl_SVD_ASSERT(!snap2.contains("a"), "SaVecStringForeach a gone");
		sv.endForeach();
	}

	static void testSafeVectorPointer()
	{
		SafeVector<int*> sv;
		int a = 1, b = 2, c = 3;
		sv.add(&a);
		sv.add(&b);
		sv.add(&c);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 3, "SaVecPtr size");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.contains(&b), "SaVecPtr contains b");
		sv.remove(&b);
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.contains(&b), "SaVecPtr b removed");
		sv.clear();
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.isEmpty(), "SaVecPtr clear");
	}

	static void testSafeVectorLargeData()
	{
		SafeVector<int> sv;
		const int N = 200;
		FOR(N)
		{
			sv.add(i * 3);
		}
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == N, "SaVecLarge initial size");
		// 遍历期间删除所有偶数索引元素（从后往前）
		sv.startForeach();
		for (int i = N - 1; i >= 0; i -= 2)
		{
			sv.removeAt(i);
		}
		sv.endForeach();
		const Vector<int>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == N / 2, "SaVecLarge half removed");
		sv.endForeach();
	}

	static void testSafeVectorRepeatedCycles()
	{
		SafeVector<int> sv;
		const int CYCLES = 20;
		FOR(CYCLES)
		{
			sv.add(i + 1);
			sv.add(i + 1001);
			sv.startForeach();
			sv.removeAt(0);
			sv.endForeach();
			sv.startForeach();
			// 每轮：保留上一轮尾 + 新加两个 - 删一个
			sv.endForeach();
		}
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.isEmpty(), "SaVecRepeatedCycles not empty");
	}

	static void testSafeVectorEraseByValueAll()
	{
		SafeVector<int> sv;
		sv.add(42);
		sv.add(42);
		sv.add(42);
		// remove 只移除第一个
		bool ok = sv.remove(42);
		SafeVectorDeepTestImpl_SVD_ASSERT(ok, "SaVecEraseByValueAll first ok");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 2, "SaVecEraseByValueAll size 2");
		sv.remove(42);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 1, "SaVecEraseByValueAll size 1");
		sv.remove(42);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 0, "SaVecEraseByValueAll size 0");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.isEmpty(), "SaVecEraseByValueAll isEmpty");
	}

	static void testSafeVectorSingleElement()
	{
		SafeVector<int> sv;
		sv.add(999);
		sv.startForeach();
		sv.removeAt(0);
		sv.endForeach();
		const Vector<int>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 0, "SaVecSingleElement empty after remove");
		sv.endForeach();
	}

	static void testSafeVectorEmptyForeach()
	{
		SafeVector<int> sv;
		const Vector<int>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 0, "SaVecEmptyForeach snap empty");
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.isForeaching(), "SaVecEmptyForeach isForeaching");
		sv.endForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(!sv.isForeaching(), "SaVecEmptyForeach not foreaching after end");
	}

	static void testSafeVectorMixedTypes()
	{
		SafeVector<float> sv;
		sv.add(1.0f);
		sv.add(2.5f);
		sv.add(3.14f);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 3, "SaVecFloat size");
		sv.remove(2.5f);
		SafeVectorDeepTestImpl_SVD_ASSERT(sv.size() == 2, "SaVecFloat remove 2.5");
		sv.startForeach();
		sv.add(100.0f);
		sv.endForeach();
		const Vector<float>& snap = sv.startForeach();
		SafeVectorDeepTestImpl_SVD_ASSERT(snap.size() == 3, "SaVecFloat after add 100");
		sv.endForeach();
	}

	static void runExtraTests()
	{
		testSafeVectorString();
		testSafeVectorStringForeach();
		testSafeVectorPointer();
		testSafeVectorLargeData();
		testSafeVectorRepeatedCycles();
		testSafeVectorEraseByValueAll();
		testSafeVectorSingleElement();
		testSafeVectorEmptyForeach();
		testSafeVectorMixedTypes();
	}

	// runExtraTests() 由 test() 显式调用
};

struct SafeVectorExtTestImpl
{

	// ============================================================
	// SafeVectorExtTest — SafeVector<T> moveMainListTo 未测试方法
	// SafeVectorTest.cpp 已覆盖：add/removeAt/remove/clear/
	//   startForeach/endForeach/getMainList/size/isEmpty/contains
	//   SafeHashMap/SafeMap/SafeSet 的全部方法
	// 本文件覆盖未测试的方法：
	//   SafeVector::moveMainListTo (非遍历中/遍历中)
	//   isForeaching 在不同状态下的返回值
	//   data() 直接访问
	//   综合场景：转移主列表后继续操作
	// ============================================================

#define SafeVectorExtTestImpl_SVEX_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("SafeVectorExtTest FAILED: ") + (msg)); }
#define SafeVectorExtTestImpl_SVEX_EQ(a, b, msg) \
    if ((a) != (b)) { ERROR(string("SafeVectorExtTest FAILED: ") + (msg) + \
        " | got=" + IToS((int)(a))); }

// ============================================================
// moveMainListTo — 非遍历中
// ============================================================
	static void testSVEX_MoveMainListTo_Basic()
	{
		SafeVector<int> sv;
		sv.add(10);
		sv.add(20);
		sv.add(30);
		SafeVectorExtTestImpl_SVEX_EQ(sv.size(), 3, "before move size==3");

		Vector<int> target;
		sv.moveMainListTo(target);

		SafeVectorExtTestImpl_SVEX_EQ(target.size(), 3, "target size==3 after move");
		SafeVectorExtTestImpl_SVEX_EQ(target[0], 10, "target[0]==10");
		SafeVectorExtTestImpl_SVEX_EQ(target[1], 20, "target[1]==20");
		SafeVectorExtTestImpl_SVEX_EQ(target[2], 30, "target[2]==30");
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.isEmpty(), "sv empty after moveMainListTo");
	}

	static void testSVEX_MoveMainListTo_Empty()
	{
		SafeVector<int> sv;
		Vector<int> target;
		target.add(999);
		sv.moveMainListTo(target);
		SafeVectorExtTestImpl_SVEX_ASSERT(target.isEmpty(), "moveMainListTo empty clears target");
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.isEmpty(), "sv empty after move empty");
	}

	static void testSVEX_MoveMainListTo_ThenReuse()
	{
		SafeVector<int> sv;
		FOR(5)
		{
			sv.add(i);
		}
		Vector<int> target;
		sv.moveMainListTo(target);
		SafeVectorExtTestImpl_SVEX_EQ(target.size(), 5, "moveMainListTo target size==5");

		// sv 被清空后可以继续使用
		sv.add(100);
		sv.add(200);
		SafeVectorExtTestImpl_SVEX_EQ(sv.size(), 2, "sv reuse after move size==2");
		SafeVectorExtTestImpl_SVEX_EQ(sv.getMainList()[0], 100, "sv reuse [0]==100");
	}

	static void testSVEX_MoveMainListTo_LargeList()
	{
		SafeVector<int> sv;
		const int N = 1000;
		FOR(N)
		{
			sv.add(i);
		}
		SafeVectorExtTestImpl_SVEX_EQ(sv.size(), N, "before move large size==1000");

		Vector<int> target;
		sv.moveMainListTo(target);

		SafeVectorExtTestImpl_SVEX_EQ(target.size(), N, "large move target size==1000");
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.isEmpty(), "large move sv empty");
		// 抽样验证
		for (int i = 0; i < N; i += 100)
		{
			SafeVectorExtTestImpl_SVEX_EQ(target[i], i, "large move target value correct");
		}
	}

	static void testSVEX_MoveMainListTo_OverwriteTarget()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);

		Vector<int> target;
		target.add(10);
		target.add(20);
		// move 会覆盖 target
		sv.moveMainListTo(target);

		SafeVectorExtTestImpl_SVEX_EQ(target.size(), 3, "overwrite target size==3");
		SafeVectorExtTestImpl_SVEX_EQ(target[0], 1, "overwrite target[0]==1");
		SafeVectorExtTestImpl_SVEX_EQ(target[2], 3, "overwrite target[2]==3");
	}

	static void testSVEX_MoveMainListTo_MultipleRounds()
	{
		SafeVector<int> sv;
		FOR(5)
		{
			sv.add(i * 10);
			sv.add(i * 10 + 1);
			Vector<int> target;
			sv.moveMainListTo(target);
			SafeVectorExtTestImpl_SVEX_EQ(target.size(), 2, "multi round target size==2");
			SafeVectorExtTestImpl_SVEX_ASSERT(sv.isEmpty(), "multi round sv empty");
		}
	}

	// ============================================================
	// moveMainListTo — 在遍历中调用
	// ============================================================
	static void testSVEX_MoveMainListTo_DuringForeach()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);

		sv.startForeach();
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.isForeaching(), "isForeaching==true after startForeach");

		// 在遍历中 moveMainListTo
		Vector<int> target;
		sv.moveMainListTo(target);

		// 遍历中：mMainList 被移走，target 得到数据
		SafeVectorExtTestImpl_SVEX_EQ(target.size(), 3, "during foreach move target size==3");
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.isEmpty(), "during foreach sv.main empty after move");

		sv.endForeach();
		SafeVectorExtTestImpl_SVEX_ASSERT(!sv.isForeaching(), "isForeaching==false after endForeach");
	}

	static void testSVEX_MoveMainListTo_DuringForeach_ThenPush()
	{
		SafeVector<int> sv;
		sv.add(10);
		sv.add(20);

		sv.startForeach();

		// 遍历中：先 moveMainListTo，再 add
		Vector<int> target;
		sv.moveMainListTo(target);
		sv.add(30);

		sv.endForeach();

		// 结束遍历后，sv 应该只有 30
		SafeVectorExtTestImpl_SVEX_EQ(sv.getMainList().size(), 1, "after foreach push 30 size==1");
		SafeVectorExtTestImpl_SVEX_EQ(sv.getMainList()[0], 30, "after foreach push 30 value==30");
	}

	// ============================================================
	// isForeaching — 状态追踪
	// ============================================================
	static void testSVEX_IsForeaching_InitFalse()
	{
		SafeVector<int> sv;
		SafeVectorExtTestImpl_SVEX_ASSERT(!sv.isForeaching(), "isForeaching init==false");
	}

	static void testSVEX_IsForeaching_DuringForeach()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.startForeach();
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.isForeaching(), "isForeaching==true");
		sv.endForeach();
		SafeVectorExtTestImpl_SVEX_ASSERT(!sv.isForeaching(), "isForeaching==false after end");
	}

	static void testSVEX_IsForeaching_AfterModify()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.startForeach();
		sv.add(3);
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.isForeaching(), "isForeaching still true after push during");
		sv.endForeach();
		SafeVectorExtTestImpl_SVEX_ASSERT(!sv.isForeaching(), "isForeaching false after end");
	}

	// ============================================================
	// data() — 直接访问内部指针
	// ============================================================
	static void testSVEX_Data_Basic()
	{
		SafeVector<int> sv;
		sv.add(100);
		sv.add(200);
		sv.add(300);
		const int* ptr = sv.data();
		SafeVectorExtTestImpl_SVEX_ASSERT(ptr != nullptr, "data() non-null");
		SafeVectorExtTestImpl_SVEX_EQ(ptr[0], 100, "data()[0]==100");
		SafeVectorExtTestImpl_SVEX_EQ(ptr[1], 200, "data()[1]==200");
		SafeVectorExtTestImpl_SVEX_EQ(ptr[2], 300, "data()[2]==300");
	}

	static void testSVEX_Data_AfterPushBack()
	{
		SafeVector<int> sv;
		FOR(10)
		{
			sv.add(i * 5);
		}
		const int* ptr = sv.data();
		SafeVectorExtTestImpl_SVEX_ASSERT(ptr != nullptr, "data after push non-null");
		FOR(10)
		{
			SafeVectorExtTestImpl_SVEX_EQ(ptr[i], i * 5, "data[] value correct");
		}
	}

	static void testSVEX_Data_AfterErase()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);
		sv.removeAt(1);
		const int* ptr = sv.data();
		SafeVectorExtTestImpl_SVEX_EQ(ptr[0], 1, "data after remove [0]==1");
		SafeVectorExtTestImpl_SVEX_EQ(ptr[1], 3, "data after remove [1]==3");
	}

	// ============================================================
	// 综合场景：模拟帧更新中的对象移交
	// ============================================================
	static void testSVEX_FrameTransfer()
	{
		// 模拟：每帧的活跃对象列表，到一定条件转移给历史列表
		SafeVector<int> activeList;
		Vector<int> historyList;

		FOR(20)
		{
			activeList.add(i);
		}
		SafeVectorExtTestImpl_SVEX_EQ(activeList.size(), 20, "active size==20");

		// 转移
		activeList.moveMainListTo(historyList);
		SafeVectorExtTestImpl_SVEX_EQ(historyList.size(), 20, "history size==20");
		SafeVectorExtTestImpl_SVEX_ASSERT(activeList.isEmpty(), "active empty after transfer");

		// 重新填充
		for (int i = 20; i < 30; ++i)
		{
			activeList.add(i);
		}
		SafeVectorExtTestImpl_SVEX_EQ(activeList.size(), 10, "active refilled size==10");
	}

	static void testSVEX_ForeachWithMoveAndPush()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);

		// 遍历所有元素，同时将列表移出、再加回新元素
		Vector<int> old;
		int sum = 0;
		for (int v : sv.startForeach())
		{
			sum += v;
		}
		SafeVectorExtTestImpl_SVEX_EQ(sum, 6, "foreach sum==6");

		sv.moveMainListTo(old);
		sv.add(10);
		sv.add(20);
		sv.endForeach();

		SafeVectorExtTestImpl_SVEX_EQ(old.size(), 3, "old list size==3");
		SafeVectorExtTestImpl_SVEX_EQ(sv.size(), 2, "sv after move+push size==2");
	}

	static void testSVEX_ContainsAfterMove()
	{
		SafeVector<int> sv;
		sv.add(5);
		sv.add(10);
		sv.add(15);
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.contains(10), "contains 10 before move");

		Vector<int> target;
		sv.moveMainListTo(target);

		SafeVectorExtTestImpl_SVEX_ASSERT(!sv.contains(10), "contains 10 after move==false");
		SafeVectorExtTestImpl_SVEX_ASSERT(target.contains(10), "target contains 10 after move");
	}

	static void testSVEX_SizeAfterMove()
	{
		SafeVector<int> sv;
		FOR(7)
		{
			sv.add(i);
		}
		SafeVectorExtTestImpl_SVEX_EQ(sv.size(), 7, "before move size==7");

		Vector<int> t;
		sv.moveMainListTo(t);
		SafeVectorExtTestImpl_SVEX_EQ(sv.size(), 0, "after move sv size==0");
		SafeVectorExtTestImpl_SVEX_EQ(t.size(), 7, "after move target size==7");
	}

	static void testSVEX_MoveEmpty_ToNonEmpty()
	{
		SafeVector<int> sv;  // empty
		Vector<int> target;
		FOR(5)
		{
			target.add(i);
		}
		sv.moveMainListTo(target);
		SafeVectorExtTestImpl_SVEX_ASSERT(target.isEmpty(), "move empty to non-empty: target cleared");
	}

	static void testSVEX_IsEmpty_AfterPushAndMove()
	{
		SafeVector<int> sv;
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.isEmpty(), "init isEmpty");
		sv.add(1);
		SafeVectorExtTestImpl_SVEX_ASSERT(!sv.isEmpty(), "after push not empty");
		Vector<int> t;
		sv.moveMainListTo(t);
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.isEmpty(), "after move isEmpty");
	}

	static void testSVEX_GetMainListAfterMove()
	{
		SafeVector<int> sv;
		sv.add(42);
		sv.add(43);
		Vector<int> t;
		sv.moveMainListTo(t);
		SafeVectorExtTestImpl_SVEX_ASSERT(sv.getMainList().isEmpty(), "getMainList after move isEmpty");
		SafeVectorExtTestImpl_SVEX_EQ(sv.getMainList().size(), 0, "getMainList after move size==0");
	}

	static void testSVEX_PushAfterMove_GetsCorrectData()
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		Vector<int> t;
		sv.moveMainListTo(t);
		sv.add(99);
		sv.add(100);
		SafeVectorExtTestImpl_SVEX_EQ(sv.size(), 2, "push after move size==2");
		SafeVectorExtTestImpl_SVEX_EQ(sv.getMainList()[0], 99, "push after move [0]==99");
		SafeVectorExtTestImpl_SVEX_EQ(sv.getMainList()[1], 100, "push after move [1]==100");
	}

	// ============================================================
	// 主入口
	// ============================================================
	static void test()
	{
		testSVEX_MoveMainListTo_Basic();
		testSVEX_MoveMainListTo_Empty();
		testSVEX_MoveMainListTo_ThenReuse();
		testSVEX_MoveMainListTo_LargeList();
		testSVEX_MoveMainListTo_OverwriteTarget();
		testSVEX_MoveMainListTo_MultipleRounds();
		testSVEX_MoveMainListTo_DuringForeach();
		testSVEX_MoveMainListTo_DuringForeach_ThenPush();
		testSVEX_IsForeaching_InitFalse();
		testSVEX_IsForeaching_DuringForeach();
		testSVEX_IsForeaching_AfterModify();
		testSVEX_Data_Basic();
		testSVEX_Data_AfterPushBack();
		testSVEX_Data_AfterErase();
		testSVEX_FrameTransfer();
		testSVEX_ForeachWithMoveAndPush();
		testSVEX_ContainsAfterMove();
		testSVEX_SizeAfterMove();
		testSVEX_MoveEmpty_ToNonEmpty();
		testSVEX_IsEmpty_AfterPushAndMove();
		testSVEX_GetMainListAfterMove();
		testSVEX_PushAfterMove_GetsCorrectData();
	}
};

void SafeVectorTest::test()
{
	SafeVectorTestImpl::test();
	SafeVectorDeepTestImpl::test();
	SafeVectorExtTestImpl::test();
}