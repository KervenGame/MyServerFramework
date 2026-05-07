#include "FrameHeader.h"

// ─── Assertion macros ───
#define SV0_ASSERT(expr, msg) \
	if (!(expr)) { ERROR(string("SafeVector0Test FAIL: ") + (msg)); }
#define SV0_ASSERT_EQ(a, b, msg) \
	if ((a) != (b)) { ERROR(string("SafeVector0Test FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); }

// =====================================================================
// SafeVector0<int> 基础操作
// =====================================================================

static void testSv0IntDefaultState()
{
	SafeVector0<int> sv;
	SV0_ASSERT_EQ(sv.size(), 0, "int default size 0");
	SV0_ASSERT(sv.isEmpty(), "int default empty");
}

static void testSv0IntAdd()
{
	SafeVector0<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	SV0_ASSERT_EQ(sv.size(), 3, "int add size 3");
	SV0_ASSERT(!sv.isEmpty(), "int add not empty");

	auto& vec = sv.startForeach();
	SV0_ASSERT_EQ(vec[0], 10, "int add[0]");
	SV0_ASSERT_EQ(vec[1], 20, "int add[1]");
	SV0_ASSERT_EQ(vec[2], 30, "int add[2]");
	sv.endForeach();
}

static void testSv0IntAddMultiple()
{
	SafeVector0<int> sv;
	constexpr int COUNT = 100;
	for (int i = 0; i < COUNT; ++i)
	{
		sv.add(i * 10);
	}
	SV0_ASSERT_EQ(sv.size(), COUNT, "int add 100");

	auto& vec = sv.startForeach();
	for (int i = 0; i < COUNT; ++i)
	{
		SV0_ASSERT_EQ(vec[i], i * 10, "int add bulk");
	}
	sv.endForeach();
}

static void testSv0IntAddAfterClear()
{
	SafeVector0<int> sv;
	sv.add(1);
	sv.add(2);
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "int cleared size 0");
	sv.add(100);
	SV0_ASSERT_EQ(sv.size(), 1, "int add after clear size 1");
	auto& vec = sv.startForeach();
	SV0_ASSERT_EQ(vec[0], 100, "int add after clear value");
	sv.endForeach();
}

// =====================================================================
// SafeVector0<int> remove
// =====================================================================

static void testSv0IntRemoveExisting()
{
	SafeVector0<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.remove(20);
	auto& vec = sv.startForeach();
	SV0_ASSERT_EQ(vec[0], 10, "int remove existing[0]");
	SV0_ASSERT_EQ(vec[1], 30, "int remove existing[1]");
	sv.endForeach();
}

static void testSv0IntRemoveFirst()
{
	SafeVector0<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.remove(10);
	auto& vec = sv.startForeach();
	SV0_ASSERT_EQ(vec[0], 20, "int remove first[0]");
	SV0_ASSERT_EQ(vec[1], 30, "int remove first[1]");
	sv.endForeach();
}

static void testSv0IntRemoveLast()
{
	SafeVector0<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.remove(30);
	auto& vec = sv.startForeach();
	SV0_ASSERT_EQ(vec[0], 10, "int remove last[0]");
	SV0_ASSERT_EQ(vec[1], 20, "int remove last[1]");
	sv.endForeach();
}

static void testSv0IntRemoveNonexistent()
{
	SafeVector0<int> sv;
	sv.add(10);
	sv.add(20);
	sv.remove(999); // 不存在
	// 不应崩溃，size 不变
	SV0_ASSERT_EQ(sv.size(), 2, "int remove nonexistent size unchanged");
	auto& vec = sv.startForeach();
	SV0_ASSERT_EQ(vec[0], 10, "int remove nonexistent[0]");
	SV0_ASSERT_EQ(vec[1], 20, "int remove nonexistent[1]");
	sv.endForeach();
}

static void testSv0IntRemoveDuplicates()
{
	SafeVector0<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(10);
	sv.add(30);
	sv.remove(10);
	// 应只移除第一个匹配项
	auto& vec = sv.startForeach();
	int expected[] = {20, 10, 30};
	for (int i = 0; i < 3; ++i)
	{
		SV0_ASSERT_EQ(vec[i], expected[i], "int remove duplicate");
	}
	sv.endForeach();
}

static void testSv0IntRemoveFromEmpty()
{
	SafeVector0<int> sv;
	sv.remove(1);
	SV0_ASSERT_EQ(sv.size(), 0, "int remove from empty");
}

static void testSv0IntRemoveAll()
{
	SafeVector0<int> sv;
	for (int i = 0; i < 5; ++i) sv.add(i);
	sv.remove(0);
	sv.remove(1);
	sv.remove(2);
	sv.remove(3);
	sv.remove(4);
	SV0_ASSERT_EQ(sv.size(), 0, "int remove all empty");
}

// =====================================================================
// SafeVector0<int> foreach (遍历中修改)
// =====================================================================

static void testSv0IntForeachBasic()
{
	SafeVector0<int> sv;
	for (int i = 0; i < 5; ++i) sv.add(i);

	auto& vec = sv.startForeach();
	int sum = 0;
	for (int i = 0; i < (int)vec.size(); ++i)
	{
		sum += vec[i];
	}
	sv.endForeach();
	SV0_ASSERT_EQ(sum, 10, "int foreach sum 0+1+2+3+4=10");
}

static void testSv0IntForeachNested()
{
	SafeVector0<int> sv;
	sv.add(1);
	sv.add(2);

	// 嵌套遍历（已验证 SafeVector0 支持嵌套遍历）
	auto& outer = sv.startForeach();
	for (int i = 0; i < (int)outer.size(); ++i)
	{
		auto& inner = sv.startForeach();
		for (int j = 0; j < (int)inner.size(); ++j)
		{
			SV0_ASSERT_EQ(inner[j], j + 1, "int nested foreach value");
		}
		sv.endForeach();
	}
	sv.endForeach();
}

static void testSv0IntForeachRemoveDuringIteration()
{
	SafeVector0<int> sv;
	for (int i = 0; i < 5; ++i)
	{
		sv.add(i);
	}

	auto& vec = sv.startForeach();
	// 在遍历期间标记删除（不会立即生效）
	sv.remove(2);
	SV0_ASSERT_EQ(vec[2], 0, "int foreach remove deferred - still visible");
	sv.endForeach();
	// 遍历结束后删除生效
}

// =====================================================================
// SafeVector0<int> clear
// =====================================================================

static void testSv0IntClear()
{
	SafeVector0<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	SV0_ASSERT_EQ(sv.size(), 3, "int clear before size 3");
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "int clear after size 0");
}

static void testSv0IntClearEmpty()
{
	SafeVector0<int> sv;
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "int clear empty size 0");
}

static void testSv0IntClearAndReadd()
{
	SafeVector0<int> sv;
	for (int i = 0; i < 100; ++i)
	{
		sv.add(i);
	}
	SV0_ASSERT_EQ(sv.size(), 100, "int clear readd size 100");
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "int clear readd after clear");
	for (int i = 0; i < 50; ++i)
	{
		sv.add(i * 2);
	}
	SV0_ASSERT_EQ(sv.size(), 50, "int clear readd size 50");
	auto& vec = sv.startForeach();
	for (int i = 0; i < 50; ++i)
	{
		SV0_ASSERT_EQ(vec[i], i * 2, "int clear readd value");
	}
	sv.endForeach();
}

// =====================================================================
// SafeVector0<int> 批量 + 压力
// =====================================================================

static void testSv0IntAddRemoveInterleaved()
{
	SafeVector0<int> sv;
	for (int cycle = 0; cycle < 5; ++cycle)
	{
		for (int i = 0; i < 10; ++i)
		{
			sv.add(i);
		}
		for (int i = 0; i < 10; ++i)
		{
			sv.remove(i);
		}
	}
	SV0_ASSERT_EQ(sv.size(), 0, "int interleaved add/remove final empty");
}

static void testSv0IntLargeVolume()
{
	SafeVector0<int> sv;
	constexpr int COUNT = 1000;
	for (int i = 0; i < COUNT; ++i)
	{
		sv.add(i);
	}
	SV0_ASSERT_EQ(sv.size(), COUNT, "int large volume size");
	auto& vec = sv.startForeach();
	for (int i = 0; i < COUNT; ++i)
	{
		SV0_ASSERT_EQ(vec[i], i, "int large volume value");
	}
	sv.endForeach();

	// 删除奇数项
	for (int i = COUNT - 1; i >= 0; i -= 2)
	{
		sv.remove(i);
	}
}

static void testSv0IntStress()
{
	SafeVector0<int> sv;
	constexpr int OPS = 500;
	for (int i = 0; i < OPS; ++i)
	{
		sv.add(i * 2);
	}
	SV0_ASSERT_EQ(sv.size(), OPS, "int stress size");

	for (int i = 0; i < OPS; ++i)
	{
		if (i % 3 == 0)
		{
			sv.remove(i * 2);
		}
	}
	// 不应崩溃
	SV0_ASSERT(true, "int stress done");
}

// =====================================================================
// SafeVector0<string>
// =====================================================================

static void testSv0StringAdd()
{
	SafeVector0<string> sv;
	sv.add("hello");
	sv.add("world");
	sv.add("test");
	SV0_ASSERT_EQ(sv.size(), 3, "string add size 3");
	auto& vec = sv.startForeach();
	SV0_ASSERT(vec[0] == "hello", "string add[0]");
	SV0_ASSERT(vec[1] == "world", "string add[1]");
	SV0_ASSERT(vec[2] == "test", "string add[2]");
	sv.endForeach();
}

static void testSv0StringRemove()
{
	SafeVector0<string> sv;
	sv.add("apple");
	sv.add("banana");
	sv.add("cherry");
	sv.remove(string("banana"));
	auto& vec = sv.startForeach();
	SV0_ASSERT(vec[0] == "apple", "string remove[0]");
	SV0_ASSERT(vec[1] == "cherry", "string remove[1]");
	sv.endForeach();
}

static void testSv0StringClear()
{
	SafeVector0<string> sv;
	sv.add("data");
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "string clear size 0");
}

// =====================================================================
// SafeVector0<float>
// =====================================================================

static void testSv0FloatAddRemove()
{
	SafeVector0<float> sv;
	sv.add(1.5f);
	sv.add(2.5f);
	sv.add(3.5f);
	SV0_ASSERT_EQ(sv.size(), 3, "float add size");
	sv.remove(2.5f);
	SV0_ASSERT_EQ(sv.size(), 2, "float remove size");
	auto& vec = sv.startForeach();
	SV0_ASSERT(vec[0] == 1.5f, "float[0]");
	SV0_ASSERT(vec[1] == 3.5f, "float[1]");
	sv.endForeach();
}

// =====================================================================
// 主入口
// =====================================================================
void SafeVector0Test::test()
{
	testSv0IntDefaultState();
	testSv0IntAdd();
	testSv0IntAddMultiple();
	testSv0IntAddAfterClear();

	testSv0IntRemoveExisting();
	testSv0IntRemoveFirst();
	testSv0IntRemoveLast();
	testSv0IntRemoveNonexistent();
	testSv0IntRemoveDuplicates();
	testSv0IntRemoveFromEmpty();
	testSv0IntRemoveAll();

	testSv0IntForeachBasic();
	testSv0IntForeachNested();
	testSv0IntForeachRemoveDuringIteration();

	testSv0IntClear();
	testSv0IntClearEmpty();
	testSv0IntClearAndReadd();

	testSv0IntAddRemoveInterleaved();
	testSv0IntLargeVolume();
	testSv0IntStress();

	testSv0StringAdd();
	testSv0StringRemove();
	testSv0StringClear();

	testSv0FloatAddRemove();
}
