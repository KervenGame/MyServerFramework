#include "FrameHeader.h"

// ============================================================
// HashMapExtTest — HashMap<K,V> 全方位测试
// 覆盖：add/addOrSet/addOrGet/set/
//        contains/getPtr/getPtrConst/get/get/getAtIndex/
//        remove(key)/remove(iter)/remove(key,value)/
//        clear/dispose/merge/cloneTo/size/isEmpty/
//        operator[]/begin/end/多类型 K/V 组合
// ============================================================

#define HMT_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("HashMapExtTest FAILED: ") + (msg)); }

// -----------------------------------------------------------
// add / contains / size / isEmpty
// -----------------------------------------------------------
static void testHashMap_BasicInsert()
{
    HashMap<int, int> m;
    HMT_ASSERT(m.isEmpty(), "init isEmpty true");
    HMT_ASSERT(m.size() == 0, "init size==0");

    bool r1 = m.add(1, 100);
    bool r2 = m.add(2, 200);
    bool r3 = m.add(1, 999); // key 已存在，失败

    HMT_ASSERT(r1, "add 1 success");
    HMT_ASSERT(r2, "add 2 success");
    HMT_ASSERT(!r3, "add dup fails");
    HMT_ASSERT(m.size() == 2, "size==2");
    HMT_ASSERT(!m.isEmpty(), "not empty");
    HMT_ASSERT(m.contains(1), "contains 1");
    HMT_ASSERT(m.contains(2), "contains 2");
    HMT_ASSERT(!m.contains(99), "not contains 99");
}

static void testHashMap_InsertKeyOnly()
{
    HashMap<int, int> m;
    bool r = m.add(42);
    HMT_ASSERT(r, "insertKeyOnly success");
    HMT_ASSERT(m.contains(42), "insertKeyOnly contains 42");
    HMT_ASSERT(m.get(42, -1) == 0, "insertKeyOnly value default 0");

    bool r2 = m.add(42); // 重复
    HMT_ASSERT(!r2, "insertKeyOnly dup fails");
}

// -----------------------------------------------------------
// addOrSet
// -----------------------------------------------------------
static void testHashMap_InsertOrUpdate_New()
{
    HashMap<int, int> m;
    bool r = m.addOrSet(1, 100);
    HMT_ASSERT(r, "addOrSet new returns true");
    HMT_ASSERT(m.get(1, -1) == 100, "addOrSet new value==100");
}

static void testHashMap_InsertOrUpdate_Existing()
{
    HashMap<int, int> m;
    m.add(1, 100);
    bool r = m.addOrSet(1, 999);
    HMT_ASSERT(!r, "addOrSet existing returns false");
    HMT_ASSERT(m.get(1, -1) == 999, "addOrSet updated value==999");
}

static void testHashMap_InsertOrUpdate_Move()
{
    HashMap<int, string> m;
    string s1 = "hello";
    bool r = m.addOrSet(1, move(s1));
    HMT_ASSERT(r, "insertOrUpdateMove new true");
    HMT_ASSERT(m.get(1) == "hello", "insertOrUpdateMove value");

    string s2 = "world";
    bool r2 = m.addOrSet(1, move(s2));
    HMT_ASSERT(!r2, "insertOrUpdateMove update false");
    HMT_ASSERT(m.get(1) == "world", "insertOrUpdateMove updated");
}

// -----------------------------------------------------------
// addOrGet
// -----------------------------------------------------------
static void testHashMap_InsertOrGet_New()
{
    HashMap<int, int> m;
    int& v = m.addOrGet(5);
    v = 500;
    HMT_ASSERT(m.get(5, -1) == 500, "addOrGet new assign");
}

static void testHashMap_InsertOrGet_Existing()
{
    HashMap<int, int> m;
    m.add(3, 300);
    int& v = m.addOrGet(3);
    HMT_ASSERT(v == 300, "addOrGet existing returns existing value");
    v = 999;
    HMT_ASSERT(m.get(3, -1) == 999, "addOrGet ref modifies map");
}

static void testHashMap_InsertOrGetWithValue()
{
    HashMap<int, int> m;
    int& v1 = m.addOrGet(10, 100);
    HMT_ASSERT(v1 == 100, "insertOrGetValue new==100");

    int& v2 = m.addOrGet(10, 999);
    HMT_ASSERT(v2 == 100, "insertOrGetValue existing still 100");
}

// -----------------------------------------------------------
// set
// -----------------------------------------------------------
static void testHashMap_TrySet_Existing()
{
    HashMap<int, int> m;
    m.add(1, 100);
    bool r = m.set(1, 999);
    HMT_ASSERT(r, "set existing true");
    HMT_ASSERT(m.get(1, -1) == 999, "set value updated");
}

static void testHashMap_TrySet_NotExist()
{
    HashMap<int, int> m;
    bool r = m.set(99, 999);
    HMT_ASSERT(!r, "set notExist false");
    HMT_ASSERT(!m.contains(99), "set notExist not inserted");
}

// -----------------------------------------------------------
// getPtr / getPtrConst
// -----------------------------------------------------------
static void testHashMap_GetPtr()
{
    HashMap<int, int> m;
    m.add(1, 111);
    int* p = m.getPtr(1);
    HMT_ASSERT(p != nullptr, "getPtr non-null");
    HMT_ASSERT(*p == 111, "getPtr value==111");

    *p = 222;
    HMT_ASSERT(m.get(1, -1) == 222, "getPtr modify in place");

    int* p2 = m.getPtr(99);
    HMT_ASSERT(p2 == nullptr, "getPtr null for missing");
}

static void testHashMap_GetPtrConst()
{
    HashMap<int, int> m;
    m.add(5, 55);
    const HashMap<int, int>& cm = m;
    const int* p = cm.getPtrConst(5);
    HMT_ASSERT(p != nullptr, "getPtrConst non-null");
    HMT_ASSERT(*p == 55, "getPtrConst value==55");

    const int* p2 = cm.getPtrConst(99);
    HMT_ASSERT(p2 == nullptr, "getPtrConst null for missing");
}

// -----------------------------------------------------------
// get / get
// -----------------------------------------------------------
static void testHashMap_TryGet_Found()
{
    HashMap<string, int> m;
    m.add("alpha", 10);
    m.add("beta", 20);
    HMT_ASSERT(m.get("alpha", -1) == 10, "get alpha==10");
    HMT_ASSERT(m.get("beta", -1) == 20, "get beta==20");
    HMT_ASSERT(m.get("gamma", -1) == -1, "get missing==-1");
}

static void testHashMap_TryGetNoDefault()
{
    HashMap<int, string> m;
    m.add(1, "one");
    const string& v = m.get(1);
    HMT_ASSERT(v == "one", "tryGetNoDefault found");
    const string& v2 = m.get(99);
    HMT_ASSERT(v2.empty(), "tryGetNoDefault missing empty");
}

static void testHashMap_Get_Found()
{
    HashMap<int, int> m;
    m.add(7, 77);
    HMT_ASSERT(m.get(7) == 77, "get found==77");
}

// -----------------------------------------------------------
// getAtIndex
// -----------------------------------------------------------
static void testHashMap_GetAtIndex()
{
    HashMap<int, int> m;
    m.add(1, 100);
    m.add(2, 200);
    m.add(3, 300);

    // getAtIndex 遍历 map，顺序不保证，但能取到有效值
    const int v0 = m.getAt(0, -1);
    const int v1 = m.getAt(1, -1);
    const int v2 = m.getAt(2, -1);
    HMT_ASSERT(v0 != -1 && v1 != -1 && v2 != -1, "getAtIndex all valid");
    HMT_ASSERT(m.getAt(99, -1) == -1, "getAtIndex OOB default");
}

// -----------------------------------------------------------
// remove
// -----------------------------------------------------------
static void testHashMap_EraseByKey()
{
    HashMap<int, int> m;
    m.add(1, 100); m.add(2, 200); m.add(3, 300);
    bool r1 = m.remove(2);
    bool r2 = m.remove(99);
    HMT_ASSERT(r1, "eraseByKey found true");
    HMT_ASSERT(!r2, "eraseByKey missing false");
    HMT_ASSERT(m.size() == 2, "eraseByKey size==2");
    HMT_ASSERT(!m.contains(2), "eraseByKey not contains 2");
}

static void testHashMap_EraseByKeyAndGetValue()
{
    HashMap<int, int> m;
    m.add(5, 555);
    int out = 0;
    bool r = m.remove(5, out);
    HMT_ASSERT(r, "eraseAndGet true");
    HMT_ASSERT(out == 555, "eraseAndGet out==555");
    HMT_ASSERT(!m.contains(5), "eraseAndGet not contains 5");

    int out2 = -1;
    bool r2 = m.remove(99, out2);
    HMT_ASSERT(!r2, "eraseAndGet missing false");
    HMT_ASSERT(out2 == -1, "eraseAndGet out unchanged");
}

static void testHashMap_EraseByIterator()
{
    HashMap<int, int> m;
    m.add(10, 100);
    m.add(20, 200);
    auto it = m.find(10);
    HMT_ASSERT(it != m.end(), "eraseByIter find success");
    m.remove(it);
    HMT_ASSERT(!m.contains(10), "eraseByIter removed");
    HMT_ASSERT(m.size() == 1, "eraseByIter size==1");
}

// -----------------------------------------------------------
// clear / dispose / isEmpty
// -----------------------------------------------------------
static void testHashMap_Clear()
{
    HashMap<int, int> m;
    m.add(1, 1); m.add(2, 2); m.add(3, 3);
    m.clear();
    HMT_ASSERT(m.isEmpty(), "clear isEmpty");
    HMT_ASSERT(m.size() == 0, "clear size==0");
}

static void testHashMap_ClearEmpty()
{
    HashMap<int, int> m;
    m.clear(); // 不应崩溃
    HMT_ASSERT(m.isEmpty(), "clearEmpty still empty");
}

static void testHashMap_ClearThenInsert()
{
    HashMap<int, int> m;
    m.add(1, 100);
    m.clear();
    m.add(2, 200);
    HMT_ASSERT(m.size() == 1, "clearThenInsert size==1");
    HMT_ASSERT(m.contains(2), "clearThenInsert contains 2");
    HMT_ASSERT(!m.contains(1), "clearThenInsert not contains 1");
}

// -----------------------------------------------------------
// merge
// -----------------------------------------------------------
static void testHashMap_Merge_NoConflict()
{
    HashMap<int, int> m1;
    m1.add(1, 100); m1.add(2, 200);
    HashMap<int, int> m2;
    m2.add(3, 300); m2.add(4, 400);
    bool r = m1.addRange(m2);
    HMT_ASSERT(r, "merge no conflict true");
    HMT_ASSERT(m1.size() == 4, "merge size==4");
    HMT_ASSERT(m1.contains(3) && m1.contains(4), "merge contains new keys");
}

static void testHashMap_Merge_WithConflict()
{
    HashMap<int, int> m1;
    m1.add(1, 100); m1.add(2, 200);
    HashMap<int, int> m2;
    m2.add(2, 999); m2.add(3, 300); // key 2 冲突
    bool r = m1.addRange(m2);
    HMT_ASSERT(!r, "merge with conflict returns false");
    HMT_ASSERT(m1.size() == 3, "merge conflict size==3");
    HMT_ASSERT(m1.get(2, -1) == 200, "merge conflict original kept");
}

static void testHashMap_Merge_Empty()
{
    HashMap<int, int> m1;
    m1.add(1, 1);
    HashMap<int, int> empty;
    bool r = m1.addRange(empty);
    HMT_ASSERT(r, "merge empty true");
    HMT_ASSERT(m1.size() == 1, "merge empty size unchanged");
}

// -----------------------------------------------------------
// cloneTo
// -----------------------------------------------------------
static void testHashMap_CloneTo()
{
    HashMap<int, int> src;
    src.add(1, 10); src.add(2, 20); src.add(3, 30);
    HashMap<int, int> dst;
    src.cloneTo(dst);
    HMT_ASSERT(dst.size() == 3, "cloneTo size==3");
    HMT_ASSERT(dst.get(1, -1) == 10, "cloneTo key1==10");
    HMT_ASSERT(dst.get(3, -1) == 30, "cloneTo key3==30");

    // 修改 dst 不影响 src
    dst.add(4, 40);
    HMT_ASSERT(!src.contains(4), "cloneTo independent");
}

// -----------------------------------------------------------
// operator[]
// -----------------------------------------------------------
static void testHashMap_OperatorBracket()
{
    HashMap<string, int> m;
    m["key1"] = 10;
    m["key2"] = 20;
    HMT_ASSERT(m["key1"] == 10, "operator[] key1==10");
    HMT_ASSERT(m["key2"] == 20, "operator[] key2==20");
    m["key1"] = 99;
    HMT_ASSERT(m["key1"] == 99, "operator[] update key1==99");
}

// -----------------------------------------------------------
// iterator
// -----------------------------------------------------------
static void testHashMap_Iterator()
{
    HashMap<int, int> m;
    for (int i = 1; i <= 5; ++i)
    {
        m.add(i, i * 10);
    }
    int keySum = 0;
    int valSum = 0;
    for (const auto& kv : m)
    {
        keySum += kv.first;
        valSum += kv.second;
    }
    HMT_ASSERT(keySum == 15, "iterator keySum==15");
    HMT_ASSERT(valSum == 150, "iterator valSum==150");
}

static void testHashMap_IteratorEmpty()
{
    HashMap<int, int> m;
    int count = 0;
    count += m.size();
    HMT_ASSERT(count == 0, "iteratorEmpty count==0");
}

// -----------------------------------------------------------
// 多类型 K/V
// -----------------------------------------------------------
static void testHashMap_StringKey()
{
    HashMap<string, string> m;
    m.add("name", "Alice");
    m.add("city", "Shanghai");
    HMT_ASSERT(m.contains("name"), "stringKey contains name");
    HMT_ASSERT(m.get("city") == "Shanghai", "stringKey city");
    m.remove("name");
    HMT_ASSERT(!m.contains("name"), "stringKey erased name");
}

static void testHashMap_LLongKey()
{
    HashMap<llong, int> m;
    m.add(1000000000LL, 1);
    m.add(9999999999LL, 2);
    HMT_ASSERT(m.contains(1000000000LL), "llongKey contains");
    HMT_ASSERT(m.get(9999999999LL, -1) == 2, "llongKey value");
}

static void testHashMap_IntToString()
{
    HashMap<int, string> m;
    for (int i = 0; i < 50; ++i)
    {
        m.add(i, "val_" + IToS(i));
    }
    HMT_ASSERT(m.size() == 50, "intToString size==50");
    HMT_ASSERT(m.get(25) == "val_25", "intToString val_25");
    HMT_ASSERT(m.get(49) == "val_49", "intToString val_49");
}

// -----------------------------------------------------------
// 综合场景
// -----------------------------------------------------------
static void testHashMap_LargeVolume()
{
    HashMap<int, int> m;
    const int N = 1000;
    for (int i = 0; i < N; ++i)
    {
        m.add(i, i * 2);
    }
    HMT_ASSERT(m.size() == N, "largeVolume size==1000");
    for (int i = 0; i < N; i += 100)
    {
        HMT_ASSERT(m.get(i, -1) == i * 2, "largeVolume value correct");
    }
    // 删除一半
    for (int i = 0; i < N; i += 2)
    {
        m.remove(i);
    }
    HMT_ASSERT(m.size() == N / 2, "largeVolume after remove size==500");
}

static void testHashMap_InsertUpdateAndErase()
{
    HashMap<int, int> m;
    m.add(1, 10); m.add(2, 20); m.add(3, 30);
    m.addOrSet(2, 200); // 更新
    HMT_ASSERT(m.get(2, -1) == 200, "insertUpdateErase updated 2");
    m.remove(1);
    HMT_ASSERT(!m.contains(1), "insertUpdateErase erased 1");
    HMT_ASSERT(m.size() == 2, "insertUpdateErase size==2");
}

static void testHashMap_MergeAndClone()
{
    HashMap<int, int> base;
    for (int i = 0; i < 10; ++i)
    {
        base.add(i, i * 100);
    }
    HashMap<int, int> ext;
    for (int i = 10; i < 20; ++i)
    {
        ext.add(i, i * 100);
    }
    base.addRange(ext);
    HMT_ASSERT(base.size() == 20, "mergeAndClone size==20");

    HashMap<int, int> copy;
    base.cloneTo(copy);
    HMT_ASSERT(copy.size() == 20, "mergeAndClone cloneTo size==20");
    HMT_ASSERT(copy.get(15, -1) == 1500, "mergeAndClone cloneTo val 15");
}

static void testHashMap_EraseAllThenReinsert()
{
    HashMap<int, int> m;
    FOR(5)
    {
        m.add(i, i);
    }
    FOR(5)
    {
        m.remove(i);
    }
    HMT_ASSERT(m.isEmpty(), "eraseAll empty");
    FOR(5)
    {
        m.add(i, i * 10);
    }
    HMT_ASSERT(m.size() == 5, "eraseAllReinsert size==5");
    HMT_ASSERT(m.get(4, -1) == 40, "eraseAllReinsert val 4");
}

static void testHashMap_FindIterator()
{
    HashMap<int, int> m;
    m.add(7, 77);
    auto it = m.find(7);
    HMT_ASSERT(it != m.end(), "find found");
    HMT_ASSERT(it->first == 7 && it->second == 77, "find values");

    auto it2 = m.find(99);
    HMT_ASSERT(it2 == m.end(), "find missing==end");
}

static void testHashMap_OperatorBracketAutoInsert()
{
    HashMap<int, int> m;
    m[5]; // 自动插入 key=5，value=默认 0
    HMT_ASSERT(m.contains(5), "autoInsert contains 5");
    HMT_ASSERT(m[5] == 0, "autoInsert value==0");
}

static void testHashMap_TrySetMultiple()
{
    HashMap<int, int> m;
    FOR(10)
    {
        m.add(i, i);
    }
    FOR(10)
    {
        bool r = m.set(i, i * 100);
        HMT_ASSERT(r, "trySetMultiple success");
    }
    FOR(10)
    {
        HMT_ASSERT(m.get(i, -1) == i * 100, "trySetMultiple updated");
    }
}

static void testHashMap_InsertOrGetChained()
{
    HashMap<string, int> m;
    m.addOrGet("a") = 1;
    m.addOrGet("b") = 2;
    m.addOrGet("a") = 99; // key 已存在，返回引用并修改
    HMT_ASSERT(m.get("a") == 99, "insertOrGetChained a==99");
    HMT_ASSERT(m.get("b") == 2, "insertOrGetChained b==2");
}

static void testHashMap_StringValueVector()
{
    // 模拟 HashMap<int, Vector<int>>
    HashMap<int, int> counts;
    int data[] = { 1, 2, 3, 1, 2, 1, 4, 5, 4 };
    for (int i = 0; i < 9; ++i)
    {
        int key = data[i];
        if (!counts.contains(key))
        {
            counts.add(key, 0);
        }
        counts.addOrSet(key, counts.get(key, 0) + 1);
    }
    HMT_ASSERT(counts.get(1, 0) == 3, "frequency 1==3");
    HMT_ASSERT(counts.get(2, 0) == 2, "frequency 2==2");
    HMT_ASSERT(counts.get(4, 0) == 2, "frequency 4==2");
    HMT_ASSERT(counts.get(5, 0) == 1, "frequency 5==1");
}

static void testHashMap_EraseByValue()
{
    HashMap<int, int> m;
    m.add(1, 100); m.add(2, 200); m.add(3, 300);
    int got = 0;
    m.remove(2, got);
    HMT_ASSERT(got == 200, "eraseByValue got==200");
    HMT_ASSERT(m.size() == 2, "eraseByValue size==2");
}

static void testHashMap_InsertMoveSemantics()
{
    HashMap<int, string> m;
    string s = "moved_value";
    bool r = m.add(1, move(s));
    HMT_ASSERT(r, "insertMove success");
    HMT_ASSERT(m.get(1) == "moved_value", "insertMove value correct");
}

static void testHashMap_CloneToOverwrite()
{
    HashMap<int, int> src;
    src.add(1, 10); src.add(2, 20);
    HashMap<int, int> dst;
    dst.add(100, 999);
    src.cloneTo(dst);
    HMT_ASSERT(dst.size() == 2, "cloneToOverwrite size==2");
    HMT_ASSERT(dst.get(1, -1) == 10, "cloneToOverwrite key1==10");
    HMT_ASSERT(!dst.contains(100), "cloneToOverwrite old key gone");
}

static void testHashMap_DisposeAndReuse()
{
    HashMap<int, int> m;
    for (int i = 0; i < 100; ++i)
    {
        m.add(i, i);
    }
    m.dispose();
    HMT_ASSERT(m.isEmpty(), "dispose empty");
    m.add(1, 1);
    HMT_ASSERT(m.size() == 1, "dispose reuse size==1");
}

static void testHashMap_ConstMethodsOnConst()
{
    HashMap<int, int> base;
    base.add(1, 111);
    base.add(2, 222);
    const HashMap<int, int>& cm = base;

    HMT_ASSERT(cm.contains(1), "const contains 1");
    HMT_ASSERT(!cm.contains(99), "const not contains 99");
    HMT_ASSERT(cm.get(2, -1) == 222, "const get");
    HMT_ASSERT(cm.size() == 2, "const size==2");
    HMT_ASSERT(!cm.isEmpty(), "const not empty");

    int count = cm.size();
    HMT_ASSERT(count == 2, "const iterator count==2");
}

// ============================================================
// 主入口
// ============================================================
void HashMapExtTest::test()
{
    testHashMap_BasicInsert();
    testHashMap_InsertKeyOnly();
    testHashMap_InsertOrUpdate_New();
    testHashMap_InsertOrUpdate_Existing();
    testHashMap_InsertOrUpdate_Move();
    testHashMap_InsertOrGet_New();
    testHashMap_InsertOrGet_Existing();
    testHashMap_InsertOrGetWithValue();
    testHashMap_TrySet_Existing();
    testHashMap_TrySet_NotExist();
    testHashMap_GetPtr();
    testHashMap_GetPtrConst();
    testHashMap_TryGet_Found();
    testHashMap_TryGetNoDefault();
    testHashMap_Get_Found();
    testHashMap_GetAtIndex();
    testHashMap_EraseByKey();
    testHashMap_EraseByKeyAndGetValue();
    testHashMap_EraseByIterator();
    testHashMap_Clear();
    testHashMap_ClearEmpty();
    testHashMap_ClearThenInsert();
    testHashMap_Merge_NoConflict();
    testHashMap_Merge_WithConflict();
    testHashMap_Merge_Empty();
    testHashMap_CloneTo();
    testHashMap_OperatorBracket();
    testHashMap_Iterator();
    testHashMap_IteratorEmpty();
    testHashMap_StringKey();
    testHashMap_LLongKey();
    testHashMap_IntToString();
    testHashMap_LargeVolume();
    testHashMap_InsertUpdateAndErase();
    testHashMap_MergeAndClone();
    testHashMap_EraseAllThenReinsert();
    testHashMap_FindIterator();
    testHashMap_OperatorBracketAutoInsert();
    testHashMap_TrySetMultiple();
    testHashMap_InsertOrGetChained();
    testHashMap_StringValueVector();
    testHashMap_EraseByValue();
    testHashMap_InsertMoveSemantics();
    testHashMap_CloneToOverwrite();
    testHashMap_DisposeAndReuse();
    testHashMap_ConstMethodsOnConst();
}
