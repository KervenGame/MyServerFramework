#include "FrameHeader.h"

// ============================================================
// MapExtTest — Map<K,V> 全方法深度测试
// 注意：ContainerTest 只覆盖了 Map 的基础操作（add/[]/get/remove/clear/iterate）
// 本文件覆盖所有未测试的方法：
//   addOrSet / addOrGet / set / addRange / cloneTo / dispose /
//   getPtrConst / find(iterator) / rbegin/rend / remove(iterator) / remove(key,value) /
//   add(key-only) / move-ctor / move-assign / initializer-list ctor /
//   copy-assign / getAtIndex(含边界) / clear(disposeMemory=true)
//   大量场景组合测试
// ============================================================

#define MAP_ET_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("MapExtTest FAILED: ") + (msg)); }
#define MET_EQ(a, b, msg) \
    if ((a) != (b)) { ERROR(string("MapExtTest FAILED: ") + (msg) + " | got=" + IToS((int)(a))); }

// -----------------------------------------------------------
// addOrSet — 未在 ContainerTest 中测试
// -----------------------------------------------------------
static void testMapExt_InsertOrUpdate_New()
{
    Map<int, int> m;
    bool r = m.addOrSet(1, 100);
    MAP_ET_ASSERT(r, "addOrSet new returns true");
    MET_EQ(m.get(1, -1), 100, "addOrSet new value==100");
}

static void testMapExt_InsertOrUpdate_Existing()
{
    Map<int, int> m;
    m.add(1, 100);
    bool r = m.addOrSet(1, 999);
    MAP_ET_ASSERT(!r, "addOrSet existing returns false");
    MET_EQ(m.get(1, -1), 999, "addOrSet existing updated value==999");
}

static void testMapExt_InsertOrUpdate_Move()
{
    Map<int, string> m;
    string s1 = "hello";
    bool r = m.addOrSet(1, move(s1));
    MAP_ET_ASSERT(r, "addOrSet move new returns true");
    MAP_ET_ASSERT(m.get(1) == "hello", "addOrSet move value==hello");
    string s2 = "world";
    bool r2 = m.addOrSet(1, move(s2));
    MAP_ET_ASSERT(!r2, "addOrSet move update returns false");
    MAP_ET_ASSERT(m.get(1) == "world", "addOrSet move updated==world");
}

static void testMapExt_InsertOrUpdate_Multiple()
{
    Map<int, int> m;
    for (int i = 0; i < 100; ++i)
    {
        m.addOrSet(i, i * 10);
    }
    MET_EQ(m.size(), 100, "addOrSet loop size==100");
    for (int i = 0; i < 100; ++i)
    {
        m.addOrSet(i, i * 100);
    }
    MET_EQ(m.size(), 100, "addOrSet update loop size unchanged");
    for (int i = 0; i < 100; ++i)
    {
        MET_EQ(m.get(i, -1), i * 100, "addOrSet loop updated value");
    }
}

static void testMapExt_InsertOrUpdate_StringKey()
{
    Map<string, int> m;
    bool r1 = m.addOrSet("alpha", 1);
    bool r2 = m.addOrSet("beta", 2);
    bool r3 = m.addOrSet("alpha", 99);
    MAP_ET_ASSERT(r1 && r2, "addOrSet str new true");
    MAP_ET_ASSERT(!r3, "addOrSet str existing false");
    MET_EQ(m.get("alpha", -1), 99, "addOrSet str updated");
    MET_EQ(m.get("beta", -1), 2, "addOrSet str beta unchanged");
}

// -----------------------------------------------------------
// addOrGet — 未在 ContainerTest 中测试
// -----------------------------------------------------------
static void testMapExt_InsertOrGet_New()
{
    Map<int, int> m;
    int& v = m.addOrGet(5);
    v = 500;
    MET_EQ(m.get(5, -1), 500, "addOrGet new assign");
}

static void testMapExt_InsertOrGet_Existing()
{
    Map<int, int> m;
    m.add(3, 300);
    int& v = m.addOrGet(3);
    MET_EQ(v, 300, "addOrGet existing returns existing value");
    v = 999;
    MET_EQ(m.get(3, -1), 999, "addOrGet ref modifies map");
}

static void testMapExt_InsertOrGet_Chain()
{
    Map<string, int> m;
    m.addOrGet("a") = 1;
    m.addOrGet("b") = 2;
    m.addOrGet("a") = 99;
    MET_EQ(m.get("a", -1), 99, "addOrGet chain a==99");
    MET_EQ(m.get("b", -1), 2, "addOrGet chain b==2");
}

static void testMapExt_InsertOrGet_LargeCount()
{
    Map<int, int> m;
    for (int i = 0; i < 200; ++i)
    {
        m.addOrGet(i) = i * 3;
    }
    MET_EQ(m.size(), 200, "addOrGet large size==200");
    for (int i = 0; i < 200; ++i)
    {
        MET_EQ(m.get(i, -1), i * 3, "addOrGet large value correct");
    }
}

// -----------------------------------------------------------
// set — 未在 ContainerTest 中测试
// -----------------------------------------------------------
static void testMapExt_TrySet_Existing()
{
    Map<int, int> m;
    m.add(1, 100);
    bool r = m.set(1, 999);
    MAP_ET_ASSERT(r, "set existing true");
    MET_EQ(m.get(1, -1), 999, "set value updated");
}

static void testMapExt_TrySet_NotExist()
{
    Map<int, int> m;
    bool r = m.set(99, 999);
    MAP_ET_ASSERT(!r, "set notExist false");
    MAP_ET_ASSERT(!m.contains(99), "set notExist not inserted");
}

static void testMapExt_TrySet_Multiple()
{
    Map<int, int> m;
    for (int i = 0; i < 50; ++i)
    {
        m.add(i, i);
    }
    for (int i = 0; i < 50; ++i)
    {
        bool r = m.set(i, i * 10);
        MAP_ET_ASSERT(r, "set multiple success");
    }
    for (int i = 0; i < 50; ++i)
    {
        MET_EQ(m.get(i, -1), i * 10, "set multiple updated");
    }
}

static void testMapExt_TrySet_StringKey()
{
    Map<string, string> m;
    m.add("name", "Alice");
    bool r1 = m.set("name", "Bob");
    bool r2 = m.set("city", "Shanghai");
    MAP_ET_ASSERT(r1, "set str existing true");
    MAP_ET_ASSERT(!r2, "set str missing false");
    MAP_ET_ASSERT(m.get("name") == "Bob", "set str value updated");
    MAP_ET_ASSERT(!m.contains("city"), "set str not inserted");
}

// -----------------------------------------------------------
// getPtrConst — 未在 ContainerTest 中测试
// -----------------------------------------------------------
static void testMapExt_GetPtrConst_Found()
{
    Map<int, int> m;
    m.add(7, 77);
    const Map<int, int>& cm = m;
    const int* p = cm.getPtrConst(7);
    MAP_ET_ASSERT(p != nullptr, "getPtrConst found non-null");
    MET_EQ(*p, 77, "getPtrConst value==77");
}

static void testMapExt_GetPtrConst_Missing()
{
    Map<int, int> m;
    m.add(1, 10);
    const Map<int, int>& cm = m;
    const int* p = cm.getPtrConst(99);
    MAP_ET_ASSERT(p == nullptr, "getPtrConst missing null");
}

static void testMapExt_GetPtrConst_ModifyViaGetPtr()
{
    Map<int, int> m;
    m.add(5, 50);
    int* p = m.getPtr(5);
    MAP_ET_ASSERT(p != nullptr, "getPtr non-null");
    *p = 500;
    MET_EQ(m.get(5, -1), 500, "getPtr modify in-place");
    const int* cp = m.getPtrConst(5);
    MAP_ET_ASSERT(cp != nullptr && *cp == 500, "getPtrConst after modify");
}

// -----------------------------------------------------------
// find(iterator) + remove(iterator) — 未在 ContainerTest 中测试
// -----------------------------------------------------------
static void testMapExt_FindIterator()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    auto it = m.find(2);
    MAP_ET_ASSERT(it != m.end(), "find(2) found");
    MET_EQ(it->first, 2, "find(2) key==2");
    MET_EQ(it->second, 20, "find(2) value==20");
    auto it2 = m.find(99);
    MAP_ET_ASSERT(it2 == m.end(), "find(99)==end");
}

static void testMapExt_FindIteratorConst()
{
    Map<int, int> m;
    m.add(10, 100);
    const Map<int, int>& cm = m;
    auto it = cm.find(10);
    MAP_ET_ASSERT(it != cm.end(), "const find found");
    MET_EQ(it->second, 100, "const find value==100");
}

static void testMapExt_EraseByIterator()
{
    Map<int, int> m;
    m.add(10, 100);
    m.add(20, 200);
    m.add(30, 300);
    auto it = m.find(20);
    MAP_ET_ASSERT(it != m.end(), "find before remove");
    m.remove(it);
    MAP_ET_ASSERT(!m.contains(20), "eraseIter removed");
    MET_EQ(m.size(), 2, "eraseIter size==2");
    MAP_ET_ASSERT(m.contains(10), "eraseIter 10 still there");
    MAP_ET_ASSERT(m.contains(30), "eraseIter 30 still there");
}

static void testMapExt_EraseByIterator_AllElements()
{
    Map<int, int> m;
    for (int i = 0; i < 10; ++i)
    {
        m.add(i, i * 100);
    }
    auto it = m.begin();
    while (it != m.end())
    {
        it = m.remove(it);
    }
    MAP_ET_ASSERT(m.isEmpty(), "eraseIter all isEmpty");
}

// -----------------------------------------------------------
// remove(key, value) — 未在 ContainerTest 中测试
// -----------------------------------------------------------
static void testMapExt_EraseGetValue()
{
    Map<int, int> m;
    m.add(5, 555);
    int out = 0;
    bool r = m.remove(5, out);
    MAP_ET_ASSERT(r, "eraseGetValue true");
    MET_EQ(out, 555, "eraseGetValue out==555");
    MAP_ET_ASSERT(!m.contains(5), "eraseGetValue not contains 5");
}

static void testMapExt_EraseGetValue_Missing()
{
    Map<int, int> m;
    m.add(1, 10);
    int out = -1;
    bool r = m.remove(99, out);
    MAP_ET_ASSERT(!r, "eraseGetValue missing false");
    MET_EQ(out, -1, "eraseGetValue out unchanged");
    MET_EQ(m.size(), 1, "eraseGetValue size unchanged");
}

static void testMapExt_EraseGetValue_StringValue()
{
    Map<int, string> m;
    m.add(1, "one");
    m.add(2, "two");
    string got;
    bool r = m.remove(1, got);
    MAP_ET_ASSERT(r, "eraseGetValue string true");
    MAP_ET_ASSERT(got == "one", "eraseGetValue got==one");
    MAP_ET_ASSERT(!m.contains(1), "eraseGetValue 1 gone");
    MET_EQ(m.size(), 1, "eraseGetValue size==1");
}

// -----------------------------------------------------------
// add(key-only) — 未测试
// -----------------------------------------------------------
static void testMapExt_InsertKeyOnly()
{
    Map<int, int> m;
    bool r = m.add(42);
    MAP_ET_ASSERT(r, "insertKeyOnly success");
    MAP_ET_ASSERT(m.contains(42), "insertKeyOnly contains 42");
    MET_EQ(m.get(42, -1), 0, "insertKeyOnly default value==0");
    bool r2 = m.add(42);
    MAP_ET_ASSERT(!r2, "insertKeyOnly dup fails");
}

static void testMapExt_InsertKeyOnly_StringValue()
{
    Map<int, string> m;
    bool r = m.add(7);
    MAP_ET_ASSERT(r, "insertKeyOnlyStr success");
    MAP_ET_ASSERT(m.get(7).empty(), "insertKeyOnlyStr value empty");
}

// -----------------------------------------------------------
// add(key, value&&) move semantics
// -----------------------------------------------------------
static void testMapExt_InsertMoveValue()
{
    Map<int, string> m;
    string s = "moved_string";
    bool r = m.add(1, move(s));
    MAP_ET_ASSERT(r, "insertMove success");
    MAP_ET_ASSERT(m.get(1) == "moved_string", "insertMove value correct");
}

// -----------------------------------------------------------
// addRange — ContainerTest 有但不完整，本文件测试更多场景
// -----------------------------------------------------------
static void testMapExt_Merge_NoConflict()
{
    Map<int, int> m1;
    m1.add(1, 100);
    m1.add(2, 200);
    Map<int, int> m2;
    m2.add(3, 300);
    m2.add(4, 400);
    bool r = m1.addRange(m2);
    MAP_ET_ASSERT(r, "addRange no conflict true");
    MET_EQ(m1.size(), 4, "addRange size==4");
    MET_EQ(m1.get(3, -1), 300, "addRange key3==300");
    MET_EQ(m1.get(4, -1), 400, "addRange key4==400");
}

static void testMapExt_Merge_WithConflict()
{
    Map<int, int> m1;
    m1.add(1, 100);
    m1.add(2, 200);
    Map<int, int> m2;
    m2.add(2, 999);
    m2.add(3, 300);
    bool r = m1.addRange(m2);
    MAP_ET_ASSERT(!r, "addRange conflict returns false");
    MET_EQ(m1.size(), 3, "addRange conflict size==3");
    MET_EQ(m1.get(2, -1), 200, "addRange conflict original kept");
    MET_EQ(m1.get(3, -1), 300, "addRange new key added");
}

static void testMapExt_Merge_Empty()
{
    Map<int, int> m1;
    m1.add(1, 1);
    Map<int, int> empty;
    bool r = m1.addRange(empty);
    MAP_ET_ASSERT(r, "addRange empty returns true");
    MET_EQ(m1.size(), 1, "addRange empty size unchanged");
}

static void testMapExt_Merge_IntoEmpty()
{
    Map<int, int> m1;
    Map<int, int> m2;
    for (int i = 0; i < 10; ++i)
    {
        m2.add(i, i * 10);
    }
    bool r = m1.addRange(m2);
    MAP_ET_ASSERT(r, "addRange intoEmpty true");
    MET_EQ(m1.size(), 10, "addRange intoEmpty size==10");
}

static void testMapExt_Merge_LargeNoConflict()
{
    Map<int, int> a;
    Map<int, int> b;
    for (int i = 0; i < 100; ++i)
    {
        a.add(i, i);
    }
    for (int i = 100; i < 200; ++i)
    {
        b.add(i, i);
    }
    bool r = a.addRange(b);
    MAP_ET_ASSERT(r, "addRange large noConflict true");
    MET_EQ(a.size(), 200, "addRange large size==200");
}

static void testMapExt_Merge_AllConflict()
{
    Map<int, int> a;
    Map<int, int> b;
    for (int i = 0; i < 5; ++i)
    {
        a.add(i, i * 10);
        b.add(i, i * 100);
    }
    bool r = a.addRange(b);
    MAP_ET_ASSERT(!r, "addRange allConflict false");
    MET_EQ(a.size(), 5, "addRange allConflict size unchanged");
    for (int i = 0; i < 5; ++i)
    {
        MET_EQ(a.get(i, -1), i * 10, "addRange allConflict original kept");
    }
}

// -----------------------------------------------------------
// cloneTo — 未在 ContainerTest 中测试
// -----------------------------------------------------------
static void testMapExt_CloneTo_Basic()
{
    Map<int, int> src;
    src.add(1, 10);
    src.add(2, 20);
    src.add(3, 30);
    Map<int, int> dst;
    src.cloneTo(dst);
    MET_EQ(dst.size(), 3, "cloneTo size==3");
    MET_EQ(dst.get(1, -1), 10, "cloneTo key1==10");
    MET_EQ(dst.get(3, -1), 30, "cloneTo key3==30");
    src.add(4, 40);
    MAP_ET_ASSERT(!dst.contains(4), "cloneTo independent");
}

static void testMapExt_CloneTo_OverwriteExisting()
{
    Map<int, int> src;
    src.add(1, 10);
    src.add(2, 20);
    Map<int, int> dst;
    dst.add(100, 999);
    dst.add(200, 888);
    src.cloneTo(dst);
    MET_EQ(dst.size(), 2, "cloneTo overwrite size==2");
    MET_EQ(dst.get(1, -1), 10, "cloneTo overwrite key1==10");
    MAP_ET_ASSERT(!dst.contains(100), "cloneTo overwrite old keys gone");
}

static void testMapExt_CloneTo_EmptySource()
{
    Map<int, int> src;
    Map<int, int> dst;
    dst.add(1, 100);
    src.cloneTo(dst);
    MET_EQ(dst.size(), 0, "cloneTo emptySource dst cleared");
}

static void testMapExt_CloneTo_LargeMap()
{
    Map<int, int> src;
    for (int i = 0; i < 500; ++i)
    {
        src.add(i, i * 3);
    }
    Map<int, int> dst;
    src.cloneTo(dst);
    MET_EQ(dst.size(), 500, "cloneTo large size==500");
    for (int i = 0; i < 500; i += 50)
    {
        MET_EQ(dst.get(i, -1), i * 3, "cloneTo large value");
    }
}

// -----------------------------------------------------------
// dispose — 未在 ContainerTest 中测试
// -----------------------------------------------------------
static void testMapExt_Dispose()
{
    Map<int, int> m;
    for (int i = 0; i < 100; ++i)
    {
        m.add(i, i);
    }
    m.dispose();
    MAP_ET_ASSERT(m.isEmpty(), "dispose isEmpty");
    m.add(1, 1);
    MET_EQ(m.size(), 1, "dispose reuse size==1");
}

static void testMapExt_ClearDisposeMemory()
{
    Map<int, int> m;
    for (int i = 0; i < 50; ++i)
    {
        m.add(i, i);
    }
    m.clear(true);
    MAP_ET_ASSERT(m.isEmpty(), "clear(true) isEmpty");
    m.add(42, 420);
    MET_EQ(m.get(42, -1), 420, "clear(true) reuse ok");
}

// -----------------------------------------------------------
// rbegin / rend — 反向迭代器，未测试
// -----------------------------------------------------------
static void testMapExt_ReverseIterator()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.add(4, 40);
    m.add(5, 50);
    // map 是有序的，rbegin 指向最大 key
    auto rit = m.rbegin();
    MET_EQ(rit->first, 5, "rbegin key==5");
    MET_EQ(rit->second, 50, "rbegin value==50");
    ++rit;
    MET_EQ(rit->first, 4, "rbegin+1 key==4");
}

static void testMapExt_ReverseIterator_Sum()
{
    Map<int, int> m;
    for (int i = 1; i <= 10; ++i)
    {
        m.add(i, i);
    }
    int sum = 0;
    for (auto rit = m.rbegin(); rit != m.rend(); ++rit)
    {
        sum += rit->second;
    }
    MET_EQ(sum, 55, "rbegin/rend sum==55");
}

// -----------------------------------------------------------
// getAtIndex — ContainerTest 只测了基础，本文件测试更多场景
// -----------------------------------------------------------
static void testMapExt_GetAtIndex_Ordered()
{
    Map<int, int> m;
    m.add(10, 100);
    m.add(20, 200);
    m.add(30, 300);
    // map 按 key 有序
    MET_EQ(m.getAt(0, -1), 100, "getAtIndex(0)==100");
    MET_EQ(m.getAt(1, -1), 200, "getAtIndex(1)==200");
    MET_EQ(m.getAt(2, -1), 300, "getAtIndex(2)==300");
    MET_EQ(m.getAt(3, -1), -1, "getAtIndex(3)==-1 OOB");
    MET_EQ(m.getAt(-1, -1), -1, "getAtIndex(-1)==-1 OOB");
}

static void testMapExt_GetAtIndex_AfterErase()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.remove(2);
    MET_EQ(m.getAt(0, -1), 10, "getAtIndex after remove [0]==10");
    MET_EQ(m.getAt(1, -1), 30, "getAtIndex after remove [1]==30");
    MET_EQ(m.getAt(2, -1), -1, "getAtIndex after remove [2]==-1");
}

// -----------------------------------------------------------
// move constructor / move assignment — 未测试
// -----------------------------------------------------------
static void testMapExt_MoveCtor()
{
    Map<int, int> src;
    src.add(1, 10);
    src.add(2, 20);
    Map<int, int> dst(move(src));
    MET_EQ(dst.size(), 2, "moveCtor dst size==2");
    MET_EQ(dst.get(1, -1), 10, "moveCtor dst key1==10");
    MAP_ET_ASSERT(src.isEmpty(), "moveCtor src empty");
}

static void testMapExt_MoveAssign()
{
    Map<int, int> src;
    src.add(5, 50);
    src.add(6, 60);
    Map<int, int> dst;
    dst.add(99, 999);
    dst = move(src);
    MET_EQ(dst.size(), 2, "moveAssign dst size==2");
    MET_EQ(dst.get(5, -1), 50, "moveAssign dst key5==50");
    MAP_ET_ASSERT(src.isEmpty(), "moveAssign src empty");
}

// -----------------------------------------------------------
// initializer_list ctor — 未测试
// -----------------------------------------------------------
static void testMapExt_InitializerList()
{
    Map<int, int> m({ {1, 10}, {2, 20}, {3, 30} });
    MET_EQ(m.size(), 3, "initList size==3");
    MET_EQ(m.get(1, -1), 10, "initList key1==10");
    MET_EQ(m.get(2, -1), 20, "initList key2==20");
    MET_EQ(m.get(3, -1), 30, "initList key3==30");
}

static void testMapExt_InitializerList_StringKey()
{
    Map<string, int> m({ {"a", 1}, {"b", 2}, {"c", 3} });
    MET_EQ(m.size(), 3, "initListStr size==3");
    MET_EQ(m.get("b", -1), 2, "initListStr b==2");
}

// -----------------------------------------------------------
// cbegin / cend — 未测试
// -----------------------------------------------------------
static void testMapExt_ConstIterator()
{
    Map<int, int> m;
    for (int i = 1; i <= 5; ++i)
    {
        m.add(i, i * 10);
    }
    const Map<int, int>& cm = m;
    int keySum = 0;
    int valSum = 0;
    for (auto it = cm.cbegin(); it != cm.cend(); ++it)
    {
        keySum += it->first;
        valSum += it->second;
    }
    MET_EQ(keySum, 15, "cbegin/cend keySum==15");
    MET_EQ(valSum, 150, "cbegin/cend valSum==150");
}

static void testMapExt_ConstBeginEnd()
{
    Map<int, int> m;
    m.add(10, 1);
    m.add(20, 2);
    const Map<int, int>& cm = m;
    int count = 0;
    for (auto it = cm.begin(); it != cm.end(); ++it)
    {
        ++count;
    }
    MET_EQ(count, 2, "const begin/end count==2");
}

// -----------------------------------------------------------
// 综合场景测试
// -----------------------------------------------------------
static void testMapExt_LargeVolume()
{
    Map<int, int> m;
    const int N = 1000;
    for (int i = 0; i < N; ++i)
    {
        m.add(i, i * 2);
    }
    MET_EQ(m.size(), N, "largeVolume size==1000");
    for (int i = 0; i < N; i += 100)
    {
        MET_EQ(m.get(i, -1), i * 2, "largeVolume value correct");
    }
    for (int i = 0; i < N; i += 2)
    {
        m.remove(i);
    }
    MET_EQ(m.size(), N / 2, "largeVolume after remove size==500");
}

static void testMapExt_InsertUpdateErase()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.addOrSet(2, 200);
    MET_EQ(m.get(2, -1), 200, "insertUpdateErase updated 2");
    m.remove(1);
    MAP_ET_ASSERT(!m.contains(1), "insertUpdateErase erased 1");
    MET_EQ(m.size(), 2, "insertUpdateErase size==2");
}

static void testMapExt_MergeAndClone()
{
    Map<int, int> base;
    for (int i = 0; i < 10; ++i)
    {
        base.add(i, i * 100);
    }
    Map<int, int> ext;
    for (int i = 10; i < 20; ++i)
    {
        ext.add(i, i * 100);
    }
    base.addRange(ext);
    MET_EQ(base.size(), 20, "mergeAndClone size==20");
    Map<int, int> copy;
    base.cloneTo(copy);
    MET_EQ(copy.size(), 20, "mergeAndClone cloneTo size==20");
    MET_EQ(copy.get(15, -1), 1500, "mergeAndClone cloneTo val 15");
}

static void testMapExt_EraseAllThenReinsert()
{
    Map<int, int> m;
    for (int i = 0; i < 5; ++i)
    {
        m.add(i, i);
    }
    for (int i = 0; i < 5; ++i)
    {
        m.remove(i);
    }
    MAP_ET_ASSERT(m.isEmpty(), "eraseAll empty");
    for (int i = 0; i < 5; ++i)
    {
        m.add(i, i * 10);
    }
    MET_EQ(m.size(), 5, "eraseAllReinsert size==5");
    MET_EQ(m.get(4, -1), 40, "eraseAllReinsert val 4");
}

static void testMapExt_LLongKey()
{
    Map<llong, string> m;
    m.add(1000000000LL, "billion");
    m.add(9999999999LL, "ten_billion");
    MAP_ET_ASSERT(m.contains(1000000000LL), "llongKey contains");
    MAP_ET_ASSERT(m.get(9999999999LL) == "ten_billion", "llongKey value");
}

static void testMapExt_StringKeyLargeVolume()
{
    Map<string, int> m;
    for (int i = 0; i < 100; ++i)
    {
        m.add("key_" + IToS(i), i);
    }
    MET_EQ(m.size(), 100, "strKeyLargeVol size==100");
    for (int i = 0; i < 100; i += 10)
    {
        MET_EQ(m.get("key_" + IToS(i), -1), i, "strKeyLargeVol value");
    }
}

static void testMapExt_FindAfterInsertOrUpdate()
{
    Map<int, int> m;
    m.addOrSet(1, 10);
    m.addOrSet(2, 20);
    m.addOrSet(1, 100);
    auto it = m.find(1);
    MAP_ET_ASSERT(it != m.end(), "findAfterUpdate found");
    MET_EQ(it->second, 100, "findAfterUpdate value==100");
}

static void testMapExt_GetPtrModifyThenVerify()
{
    Map<int, int> m;
    m.add(1, 1);
    m.add(2, 2);
    m.add(3, 3);
    for (int i = 1; i <= 3; ++i)
    {
        int* p = m.getPtr(i);
        MAP_ET_ASSERT(p != nullptr, "getPtr loop non-null");
        *p *= 10;
    }
    MET_EQ(m.get(1, -1), 10, "getPtr modify 1");
    MET_EQ(m.get(2, -1), 20, "getPtr modify 2");
    MET_EQ(m.get(3, -1), 30, "getPtr modify 3");
}

static void testMapExt_ClearAndReinsertion()
{
    Map<int, int> m;
    for (int i = 0; i < 20; ++i)
    {
        m.add(i, i);
    }
    m.clear();
    MAP_ET_ASSERT(m.isEmpty(), "clear isEmpty");
    for (int i = 0; i < 5; ++i)
    {
        m.add(i, i * 100);
    }
    MET_EQ(m.size(), 5, "clearReinsertion size==5");
    MET_EQ(m.get(3, -1), 300, "clearReinsertion val 3");
}

static void testMapExt_IterationOrderedByKey()
{
    Map<int, int> m;
    m.add(50, 5);
    m.add(10, 1);
    m.add(30, 3);
    m.add(20, 2);
    m.add(40, 4);
    int prevKey = -1;
    bool isOrdered = true;
    for (const auto& kv : m)
    {
        if (kv.first < prevKey)
        {
            isOrdered = false;
        }
        prevKey = kv.first;
    }
    MAP_ET_ASSERT(isOrdered, "map iterate in order");
    MET_EQ(m.getAt(0, -1), 1, "map ordered [0]==1");
    MET_EQ(m.getAt(4, -1), 5, "map ordered [4]==5");
}

static void testMapExt_TryGetDefault()
{
    Map<int, int> m;
    m.add(1, 100);
    const int& v1 = m.get(1, -1);
    MET_EQ(v1, 100, "get found==100");
    const int& v2 = m.get(99, -1);
    MET_EQ(v2, -1, "get missing==-1");
}

static void testMapExt_TryGetNoDefault()
{
    Map<int, string> m;
    m.add(1, "one");
    const string& v1 = m.get(1);
    MAP_ET_ASSERT(v1 == "one", "tryGetNoDefault found");
    const string& v2 = m.get(99);
    MAP_ET_ASSERT(v2.empty(), "tryGetNoDefault missing empty");
}

static void testMapExt_OperatorBracket_AutoInsert()
{
    Map<int, int> m;
    m[5];
    MAP_ET_ASSERT(m.contains(5), "operator[] autoInsert contains 5");
    MET_EQ(m[5], 0, "operator[] autoInsert value==0");
}

static void testMapExt_OperatorBracket_Update()
{
    Map<string, int> m;
    m["x"] = 10;
    m["y"] = 20;
    MET_EQ(m["x"], 10, "op[] x==10");
    m["x"] = 99;
    MET_EQ(m["x"], 99, "op[] update x==99");
}

static void testMapExt_ContainsMany()
{
    Map<int, int> m;
    for (int i = 0; i < 200; ++i)
    {
        m.add(i, i);
    }
    for (int i = 0; i < 200; ++i)
    {
        MAP_ET_ASSERT(m.contains(i), "containsMany found");
    }
    for (int i = 200; i < 250; ++i)
    {
        MAP_ET_ASSERT(!m.contains(i), "containsMany not found");
    }
}

static void testMapExt_IsEmpty_AfterAllOps()
{
    Map<int, int> m;
    MAP_ET_ASSERT(m.isEmpty(), "init empty");
    m.add(1, 1);
    MAP_ET_ASSERT(!m.isEmpty(), "after add not empty");
    m.remove(1);
    MAP_ET_ASSERT(m.isEmpty(), "after remove empty again");
    m.addOrSet(2, 2);
    MAP_ET_ASSERT(!m.isEmpty(), "after addOrSet not empty");
    m.clear();
    MAP_ET_ASSERT(m.isEmpty(), "after clear empty");
}

static void testMapExt_SizeTracking()
{
    Map<int, int> m;
    MET_EQ(m.size(), 0, "size init==0");
    for (int i = 0; i < 30; ++i)
    {
        m.add(i, i);
        MET_EQ(m.size(), i + 1, "size track after add");
    }
    for (int i = 0; i < 10; ++i)
    {
        m.remove(i);
        MET_EQ(m.size(), 29 - i, "size track after remove");
    }
}

// ============================================================
// 主入口
// ============================================================
void MapExtTest::test()
{
    testMapExt_InsertOrUpdate_New();
    testMapExt_InsertOrUpdate_Existing();
    testMapExt_InsertOrUpdate_Move();
    testMapExt_InsertOrUpdate_Multiple();
    testMapExt_InsertOrUpdate_StringKey();
    testMapExt_InsertOrGet_New();
    testMapExt_InsertOrGet_Existing();
    testMapExt_InsertOrGet_Chain();
    testMapExt_InsertOrGet_LargeCount();
    testMapExt_TrySet_Existing();
    testMapExt_TrySet_NotExist();
    testMapExt_TrySet_Multiple();
    testMapExt_TrySet_StringKey();
    testMapExt_GetPtrConst_Found();
    testMapExt_GetPtrConst_Missing();
    testMapExt_GetPtrConst_ModifyViaGetPtr();
    testMapExt_FindIterator();
    testMapExt_FindIteratorConst();
    testMapExt_EraseByIterator();
    testMapExt_EraseByIterator_AllElements();
    testMapExt_EraseGetValue();
    testMapExt_EraseGetValue_Missing();
    testMapExt_EraseGetValue_StringValue();
    testMapExt_InsertKeyOnly();
    testMapExt_InsertKeyOnly_StringValue();
    testMapExt_InsertMoveValue();
    testMapExt_Merge_NoConflict();
    testMapExt_Merge_WithConflict();
    testMapExt_Merge_Empty();
    testMapExt_Merge_IntoEmpty();
    testMapExt_Merge_LargeNoConflict();
    testMapExt_Merge_AllConflict();
    testMapExt_CloneTo_Basic();
    testMapExt_CloneTo_OverwriteExisting();
    testMapExt_CloneTo_EmptySource();
    testMapExt_CloneTo_LargeMap();
    testMapExt_Dispose();
    testMapExt_ClearDisposeMemory();
    testMapExt_ReverseIterator();
    testMapExt_ReverseIterator_Sum();
    testMapExt_GetAtIndex_Ordered();
    testMapExt_GetAtIndex_AfterErase();
    testMapExt_MoveCtor();
    testMapExt_MoveAssign();
    testMapExt_InitializerList();
    testMapExt_InitializerList_StringKey();
    testMapExt_ConstIterator();
    testMapExt_ConstBeginEnd();
    testMapExt_LargeVolume();
    testMapExt_InsertUpdateErase();
    testMapExt_MergeAndClone();
    testMapExt_EraseAllThenReinsert();
    testMapExt_LLongKey();
    testMapExt_StringKeyLargeVolume();
    testMapExt_FindAfterInsertOrUpdate();
    testMapExt_GetPtrModifyThenVerify();
    testMapExt_ClearAndReinsertion();
    testMapExt_IterationOrderedByKey();
    testMapExt_TryGetDefault();
    testMapExt_TryGetNoDefault();
    testMapExt_OperatorBracket_AutoInsert();
    testMapExt_OperatorBracket_Update();
    testMapExt_ContainsMany();
    testMapExt_IsEmpty_AfterAllOps();
    testMapExt_SizeTracking();
}
