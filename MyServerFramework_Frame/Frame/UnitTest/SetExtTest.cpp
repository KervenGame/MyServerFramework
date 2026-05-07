#include "FrameHeader.h"

// ============================================================
// SetExtTest — Set<T> 全方法深度测试
// ContainerTest 没有 Set 的测试，STLExtTest 只覆盖了 ArrayList/Map
// 本文件覆盖 Set 的所有方法：
//   insert(elem)/insert(Set)/insert(Vector) / erase(value)/erase(iter) /
//   contains / clear / size / isEmpty / find /
//   pop_front / setData(Array/ArrayList/rawPtr/Vector) /
//   cloneTo / move-ctor / move-assign / cbegin/cend / begin/end / iterator
// ============================================================

#define SETX_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("SetExtTest FAILED: ") + (msg)); }
#define SETX_EQ(a, b, msg) \
    if ((a) != (b)) { ERROR(string("SetExtTest FAILED: ") + (msg) + " | got=" + IToS((int)(a))); }

// -----------------------------------------------------------
// insert(elem) / contains / size / isEmpty
// -----------------------------------------------------------
static void testSetExt_BasicInsert()
{
    Set<int> s;
    SETX_ASSERT(s.isEmpty(), "init isEmpty");
    SETX_EQ(s.size(), 0, "init size==0");
    bool r1 = s.add(10);
    bool r2 = s.add(20);
    bool r3 = s.add(10); // dup
    SETX_ASSERT(r1, "insert 10 ok");
    SETX_ASSERT(r2, "insert 20 ok");
    SETX_ASSERT(!r3, "insert dup 10 fails");
    SETX_EQ(s.size(), 2, "size==2");
    SETX_ASSERT(!s.isEmpty(), "not empty");
    SETX_ASSERT(s.contains(10), "contains 10");
    SETX_ASSERT(s.contains(20), "contains 20");
    SETX_ASSERT(!s.contains(99), "not contains 99");
}

static void testSetExt_InsertMany()
{
    Set<int> s;
    for (int i = 0; i < 100; ++i)
    {
        bool r = s.add(i);
        SETX_ASSERT(r, "insertMany new ok");
    }
    SETX_EQ(s.size(), 100, "insertMany size==100");
    for (int i = 0; i < 100; ++i)
    {
        bool r = s.add(i);
        SETX_ASSERT(!r, "insertMany dup fails");
    }
    SETX_EQ(s.size(), 100, "insertMany dup size unchanged");
}

static void testSetExt_InsertStringType()
{
    Set<string> s;
    s.add("apple");
    s.add("banana");
    s.add("cherry");
    SETX_EQ(s.size(), 3, "insertStr size==3");
    SETX_ASSERT(s.contains("apple"), "insertStr contains apple");
    SETX_ASSERT(!s.contains("grape"), "insertStr not grape");
    bool r = s.add("apple");
    SETX_ASSERT(!r, "insertStr dup false");
}

// -----------------------------------------------------------
// erase(value) / erase(iterator)
// -----------------------------------------------------------
static void testSetExt_EraseByValue()
{
    Set<int> s;
    s.add(1);
    s.add(2);
    s.add(3);
    bool r1 = s.remove(2);
    bool r2 = s.remove(99);
    SETX_ASSERT(r1, "erase found true");
    SETX_ASSERT(!r2, "erase notFound false");
    SETX_EQ(s.size(), 2, "erase size==2");
    SETX_ASSERT(!s.contains(2), "erase 2 gone");
}

static void testSetExt_EraseByIterator()
{
    Set<int> s;
    s.add(10);
    s.add(20);
    s.add(30);
    auto it = s.find(20);
    SETX_ASSERT(it != s.end(), "find before eraseIter");
    s.remove(it);
    SETX_ASSERT(!s.contains(20), "eraseIter 20 gone");
    SETX_EQ(s.size(), 2, "eraseIter size==2");
}

static void testSetExt_EraseAll()
{
    Set<int> s;
    for (int i = 0; i < 10; ++i)
    {
        s.add(i);
    }
    for (int i = 0; i < 10; ++i)
    {
        bool r = s.remove(i);
        SETX_ASSERT(r, "eraseAll each ok");
    }
    SETX_ASSERT(s.isEmpty(), "eraseAll empty");
}

// -----------------------------------------------------------
// clear
// -----------------------------------------------------------
static void testSetExt_Clear()
{
    Set<int> s;
    s.add(1);
    s.add(2);
    s.add(3);
    s.clear();
    SETX_ASSERT(s.isEmpty(), "clear isEmpty");
    SETX_EQ(s.size(), 0, "clear size==0");
}

static void testSetExt_ClearEmpty()
{
    Set<int> s;
    s.clear(); // 不应崩溃
    SETX_ASSERT(s.isEmpty(), "clearEmpty still empty");
}

static void testSetExt_ClearThenInsert()
{
    Set<int> s;
    s.add(100);
    s.add(200);
    s.clear();
    s.add(1);
    SETX_EQ(s.size(), 1, "clearThenInsert size==1");
    SETX_ASSERT(s.contains(1), "clearThenInsert contains 1");
    SETX_ASSERT(!s.contains(100), "clearThenInsert 100 gone");
}

// -----------------------------------------------------------
// pop_front — 未在任何现有测试中测试
// -----------------------------------------------------------
static void testSetExt_PopFront()
{
    Set<int> s;
    s.add(42);
    int v = s.popFirst();
    SETX_EQ(v, 42, "popFront value==42");
    SETX_ASSERT(s.isEmpty(), "popFront empty after");
}

static void testSetExt_PopFrontMultiple()
{
    Set<int> s;
    for (int i = 0; i < 5; ++i)
    {
        s.add(i);
    }
    SETX_EQ(s.size(), 5, "popFrontMultiple init size==5");
    // pop 全部
    while (!s.isEmpty())
    {
        int v = s.popFirst();
        SETX_ASSERT(v >= 0 && v < 5, "popFrontMultiple value in range");
    }
    SETX_ASSERT(s.isEmpty(), "popFrontMultiple empty");
}

static void testSetExt_PopFrontRemovesElement()
{
    Set<int> s;
    s.add(10);
    s.add(20);
    s.add(30);
    int v = s.popFirst();
    SETX_EQ(s.size(), 2, "popFront size==2 after");
    SETX_ASSERT(!s.contains(v), "popFront element removed");
}

// -----------------------------------------------------------
// setData — 4 种重载，均未在任何现有测试中测试
// -----------------------------------------------------------
static void testSetExt_SetDataFromRawPtr()
{
    Set<int> s;
    int arr[] = { 5, 10, 15, 20, 25 };
    s.setData(arr, 5);
    SETX_EQ(s.size(), 5, "setDataRawPtr size==5");
    SETX_ASSERT(s.contains(5), "setDataRawPtr contains 5");
    SETX_ASSERT(s.contains(25), "setDataRawPtr contains 25");
    SETX_ASSERT(!s.contains(99), "setDataRawPtr not 99");
}

static void testSetExt_SetDataFromRawPtr_WithDups()
{
    Set<int> s;
    int arr[] = { 1, 2, 3, 2, 1 }; // 含重复
    s.setData(arr, 5);
    SETX_EQ(s.size(), 3, "setDataRawPtrDups size==3 (dedup)");
}

static void testSetExt_SetDataFromVector()
{
    Set<int> s;
    Vector<int> v;
    v.add(100);
    v.add(200);
    v.add(300);
    s.setData(v);
    SETX_EQ(s.size(), 3, "setDataVector size==3");
    SETX_ASSERT(s.contains(100), "setDataVector contains 100");
    SETX_ASSERT(s.contains(200), "setDataVector contains 200");
    SETX_ASSERT(s.contains(300), "setDataVector contains 300");
}

static void testSetExt_SetDataFromVector_Empty()
{
    Set<int> s;
    Vector<int> v;
    s.setData(v);
    SETX_ASSERT(s.isEmpty(), "setDataVectorEmpty isEmpty");
}

static void testSetExt_SetDataFromVector_Merges()
{
    Set<int> s;
    s.add(1);
    s.add(2);
    Vector<int> v;
    v.add(3);
    v.add(4);
    v.add(1); // dup
    s.setData(v);
    SETX_EQ(s.size(), 4, "setDataVectorMerge size==4 (dedup 1)");
}

static void testSetExt_SetDataFromArrayList()
{
    Set<int> s;
    ArrayList<8, int> al;
    al.add(50);
    al.add(60);
    al.add(70);
    al.add(50); // dup
    s.setData(al);
    SETX_EQ(s.size(), 3, "setDataArrayList size==3");
    SETX_ASSERT(s.contains(50), "setDataArrayList contains 50");
    SETX_ASSERT(s.contains(70), "setDataArrayList contains 70");
}

static void testSetExt_SetDataFromArray()
{
    Set<int> s;
    Array<5, int> arr;
    arr.mValue[0] = 11;
    arr.mValue[1] = 22;
    arr.mValue[2] = 33;
    arr.mValue[3] = 44;
    arr.mValue[4] = 55;
    s.setData(arr, 5);
    SETX_EQ(s.size(), 5, "setDataArray size==5");
    SETX_ASSERT(s.contains(11), "setDataArray contains 11");
    SETX_ASSERT(s.contains(55), "setDataArray contains 55");
}

static void testSetExt_SetDataFromArray_PartialCount()
{
    Set<int> s;
    Array<5, int> arr;
    arr.mValue[0] = 1;
    arr.mValue[1] = 2;
    arr.mValue[2] = 3;
    arr.mValue[3] = 4;
    arr.mValue[4] = 5;
    s.setData(arr, 3); // 只取前3个
    SETX_EQ(s.size(), 3, "setDataArrayPartial size==3");
    SETX_ASSERT(s.contains(1) && s.contains(2) && s.contains(3), "setDataArrayPartial first 3");
    SETX_ASSERT(!s.contains(4) && !s.contains(5), "setDataArrayPartial no 4/5");
}

// -----------------------------------------------------------
// insert(Set<TElement>) / insert(Vector<TElement>)
// -----------------------------------------------------------
static void testSetExt_InsertFromSet()
{
    Set<int> s1;
    s1.add(1);
    s1.add(2);
    s1.add(3);
    Set<int> s2;
    s2.add(4);
    s2.add(5);
    s2.add(1); // dup
    s1.addRange(s2);
    SETX_EQ(s1.size(), 5, "insertFromSet size==5 (dedup 1)");
    SETX_ASSERT(s1.contains(4), "insertFromSet contains 4");
    SETX_ASSERT(s1.contains(5), "insertFromSet contains 5");
}

static void testSetExt_InsertFromSet_Empty()
{
    Set<int> s1;
    s1.add(1);
    Set<int> empty;
    s1.addRange(empty);
    SETX_EQ(s1.size(), 1, "insertFromSetEmpty size unchanged");
}

static void testSetExt_InsertFromVector()
{
    Set<int> s;
    s.add(1);
    Vector<int> v;
    v.add(2);
    v.add(3);
    v.add(1); // dup
    s.addRange(v);
    SETX_EQ(s.size(), 3, "insertFromVector size==3");
    SETX_ASSERT(s.contains(2), "insertFromVector contains 2");
    SETX_ASSERT(s.contains(3), "insertFromVector contains 3");
}

static void testSetExt_InsertFromVector_Empty()
{
    Set<int> s;
    s.add(42);
    Vector<int> empty;
    s.addRange(empty);
    SETX_EQ(s.size(), 1, "insertFromVectorEmpty size unchanged");
}

// -----------------------------------------------------------
// cloneTo — 未测试
// -----------------------------------------------------------
static void testSetExt_CloneTo_Basic()
{
    Set<int> src;
    src.add(10);
    src.add(20);
    src.add(30);
    Set<int> dst;
    src.cloneTo(dst);
    SETX_EQ(dst.size(), 3, "cloneTo size==3");
    SETX_ASSERT(dst.contains(10), "cloneTo contains 10");
    SETX_ASSERT(dst.contains(20), "cloneTo contains 20");
    SETX_ASSERT(dst.contains(30), "cloneTo contains 30");
}

static void testSetExt_CloneTo_Independent()
{
    Set<int> src;
    src.add(1);
    src.add(2);
    Set<int> dst;
    src.cloneTo(dst);
    src.add(99);
    SETX_ASSERT(!dst.contains(99), "cloneTo independent");
    dst.remove(1);
    SETX_ASSERT(src.contains(1), "cloneTo src unchanged after dst erase");
}

static void testSetExt_CloneTo_Overwrite()
{
    Set<int> src;
    src.add(1);
    Set<int> dst;
    dst.add(100);
    dst.add(200);
    src.cloneTo(dst);
    SETX_EQ(dst.size(), 1, "cloneTo overwrite size==1");
    SETX_ASSERT(dst.contains(1), "cloneTo overwrite contains 1");
    SETX_ASSERT(!dst.contains(100), "cloneTo overwrite 100 gone");
}

// -----------------------------------------------------------
// move ctor / move assign — 未测试
// -----------------------------------------------------------
static void testSetExt_MoveCtor()
{
    Set<int> src;
    src.add(1);
    src.add(2);
    src.add(3);
    Set<int> dst(move(src));
    SETX_EQ(dst.size(), 3, "moveCtor dst size==3");
    SETX_ASSERT(dst.contains(1), "moveCtor dst has 1");
    SETX_ASSERT(src.isEmpty(), "moveCtor src empty");
}

static void testSetExt_MoveAssign()
{
    Set<int> src;
    src.add(10);
    src.add(20);
    Set<int> dst;
    dst.add(99);
    dst = move(src);
    SETX_EQ(dst.size(), 2, "moveAssign dst size==2");
    SETX_ASSERT(dst.contains(10), "moveAssign dst has 10");
    SETX_ASSERT(src.isEmpty(), "moveAssign src empty");
}

// -----------------------------------------------------------
// find / begin / end / cbegin / cend
// -----------------------------------------------------------
static void testSetExt_FindFound()
{
    Set<int> s;
    s.add(7);
    auto it = s.find(7);
    SETX_ASSERT(it != s.end(), "find found");
    SETX_EQ(*it, 7, "find value==7");
}

static void testSetExt_FindNotFound()
{
    Set<int> s;
    s.add(1);
    auto it = s.find(99);
    SETX_ASSERT(it == s.end(), "find notFound==end");
}

static void testSetExt_Iterator_Sum()
{
    Set<int> s;
    FOR(10)
    {
        s.add(i + 1);
    }
    int sum = 0;
    for (const auto& v : s)
    {
        sum += v;
    }
    SETX_EQ(sum, 55, "iterator sum==55");
}

static void testSetExt_ConstIterator()
{
    Set<int> s;
    s.add(2);
    s.add(4);
    s.add(6);
    const Set<int>& cs = s;
    int count = 0;
    for (auto it = cs.cbegin(); it != cs.cend(); ++it)
    {
        SETX_ASSERT(*it == 2 || *it == 4 || *it == 6, "constIter value in set");
        ++count;
    }
    SETX_EQ(count, 3, "constIter count==3");
}

static void testSetExt_ConstBeginEnd()
{
    Set<int> s;
    s.add(100);
    s.add(200);
    const Set<int>& cs = s;
    int count = 0;
    for (auto it = cs.begin(); it != cs.end(); ++it)
    {
        ++count;
    }
    SETX_EQ(count, 2, "const begin/end count==2");
}

// -----------------------------------------------------------
// 综合场景测试
// -----------------------------------------------------------
static void testSetExt_LargeVolume()
{
    Set<int> s;
    const int N = 500;
    FOR(N)
    {
        s.add(i);
    }
    SETX_EQ(s.size(), N, "largeVolume size==500");
    for (int i = 0; i < N; i += 2)
    {
        s.remove(i);
    }
    SETX_EQ(s.size(), N / 2, "largeVolume after erase size==250");
    for (int i = 0; i < N; i += 2)
    {
        SETX_ASSERT(!s.contains(i), "largeVolume even gone");
    }
    for (int i = 1; i < N; i += 2)
    {
        SETX_ASSERT(s.contains(i), "largeVolume odd exists");
    }
}

static void testSetExt_InsertEraseReinsert()
{
    Set<int> s;
    s.add(1);
    s.add(2);
    s.add(3);
    s.remove(2);
    bool r = s.add(2);
    SETX_ASSERT(r, "reinsert after erase ok");
    SETX_ASSERT(s.contains(2), "reinsert contains 2");
    SETX_EQ(s.size(), 3, "reinsert size==3");
}

static void testSetExt_SizeAfterOps()
{
    Set<int> s;
    SETX_EQ(s.size(), 0, "init size 0");
    s.add(1);
    SETX_EQ(s.size(), 1, "after insert 1");
    s.add(1);
    SETX_EQ(s.size(), 1, "after dup insert still 1");
    s.add(2);
    SETX_EQ(s.size(), 2, "after insert 2");
    s.remove(1);
    SETX_EQ(s.size(), 1, "after erase size 1");
    s.clear();
    SETX_EQ(s.size(), 0, "after clear size 0");
}

static void testSetExt_LLongType()
{
    Set<llong> s;
    s.add(1000000000LL);
    s.add(9999999999LL);
    s.add(-1LL);
    SETX_EQ(s.size(), 3, "llongType size==3");
    SETX_ASSERT(s.contains(1000000000LL), "llongType contains billion");
    SETX_ASSERT(s.contains(-1LL), "llongType contains -1");
    s.remove(9999999999LL);
    SETX_ASSERT(!s.contains(9999999999LL), "llongType erased");
}

static void testSetExt_SetDataCumulativeInsert()
{
    Set<int> s;
    int arr1[] = { 1, 2, 3 };
    int arr2[] = { 4, 5, 6 };
    int arr3[] = { 1, 4, 7 }; // partially overlaps
    s.setData(arr1, 3);
    s.setData(arr2, 3);
    s.setData(arr3, 3);
    SETX_EQ(s.size(), 7, "setDataCumulative size==7 (1-7)");
    for (int i = 1; i <= 7; ++i)
    {
        SETX_ASSERT(s.contains(i), "setDataCumulative contains each");
    }
}

static void testSetExt_PopFrontUntilEmpty()
{
    Set<int> s;
    FOR(20)
    {
        s.add(i);
    }
    SETX_EQ(s.size(), 20, "popFrontUntilEmpty init");
    while (!s.isEmpty())
    {
        int v = s.popFirst();
        SETX_ASSERT(v >= 0 && v < 20, "popFrontUntilEmpty val in range");
    }
    SETX_ASSERT(s.isEmpty(), "popFrontUntilEmpty all done");
}

static void testSetExt_CloneThenModifyOriginal()
{
    Set<int> src;
    FOR(10)
    {
        src.add(i);
    }
    Set<int> dst;
    src.cloneTo(dst);
    // 修改 src
    for (int i = 0; i < 5; ++i)
    {
        src.remove(i);
    }
    SETX_EQ(src.size(), 5, "cloneThenModify src size==5");
    SETX_EQ(dst.size(), 10, "cloneThenModify dst size==10 (unchanged)");
}

// ============================================================
// 主入口
// ============================================================
void SetExtTest::test()
{
    testSetExt_BasicInsert();
    testSetExt_InsertMany();
    testSetExt_InsertStringType();
    testSetExt_EraseByValue();
    testSetExt_EraseByIterator();
    testSetExt_EraseAll();
    testSetExt_Clear();
    testSetExt_ClearEmpty();
    testSetExt_ClearThenInsert();
    testSetExt_PopFront();
    testSetExt_PopFrontMultiple();
    testSetExt_PopFrontRemovesElement();
    testSetExt_SetDataFromRawPtr();
    testSetExt_SetDataFromRawPtr_WithDups();
    testSetExt_SetDataFromVector();
    testSetExt_SetDataFromVector_Empty();
    testSetExt_SetDataFromVector_Merges();
    testSetExt_SetDataFromArrayList();
    testSetExt_SetDataFromArray();
    testSetExt_SetDataFromArray_PartialCount();
    testSetExt_InsertFromSet();
    testSetExt_InsertFromSet_Empty();
    testSetExt_InsertFromVector();
    testSetExt_InsertFromVector_Empty();
    testSetExt_CloneTo_Basic();
    testSetExt_CloneTo_Independent();
    testSetExt_CloneTo_Overwrite();
    testSetExt_MoveCtor();
    testSetExt_MoveAssign();
    testSetExt_FindFound();
    testSetExt_FindNotFound();
    testSetExt_Iterator_Sum();
    testSetExt_ConstIterator();
    testSetExt_ConstBeginEnd();
    testSetExt_LargeVolume();
    testSetExt_InsertEraseReinsert();
    testSetExt_SizeAfterOps();
    testSetExt_LLongType();
    testSetExt_SetDataCumulativeInsert();
    testSetExt_PopFrontUntilEmpty();
    testSetExt_CloneThenModifyOriginal();
}
