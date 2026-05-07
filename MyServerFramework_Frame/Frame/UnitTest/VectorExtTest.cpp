#include "FrameHeader.h"

// ============================================================
// VectorExtTest — Vector<T> 扩展全方位测试
// 覆盖：add(多参数)/addUnique/addNotEqual/addUniqueOrErase/
//        addRange/addRangeCopy/addRangePtr/setRange/setRangeCopy/
//        erase/removeAt/remove/eraseAllElement/eraseLast/
//        popBack/contains/isAll/findFirstIndex/findElementCount/
//        swapIndex/random/randomRemove/insert/cloneTo/sort/
//        resize/reserve/get/operator[]/operator==/operator!=
// ============================================================

#define VET_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("VectorExtTest FAILED: ") + (msg)); }

// -----------------------------------------------------------
// add 系列
// -----------------------------------------------------------
static void testVector_PushBackSingle()
{
    Vector<int> v;
    v.add(10);
    VET_ASSERT(v.size() == 1, "pushBackSingle size==1");
    VET_ASSERT(v[0] == 10, "pushBackSingle [0]==10");
}

static void testVector_PushBack2()
{
    Vector<int> v;
    v.add(1, 2);
    VET_ASSERT(v.size() == 2, "pushBack2 size==2");
    VET_ASSERT(v[0] == 1 && v[1] == 2, "pushBack2 values");
}

static void testVector_PushBack3()
{
    Vector<int> v;
    v.add(10, 20, 30);
    VET_ASSERT(v.size() == 3, "pushBack3 size==3");
    VET_ASSERT(v[0] == 10 && v[1] == 20 && v[2] == 30, "pushBack3 values");
}

static void testVector_PushBack4()
{
    Vector<int> v;
    v.add(1, 2, 3, 4);
    VET_ASSERT(v.size() == 4, "pushBack4 size==4");
    VET_ASSERT(v[3] == 4, "pushBack4 [3]==4");
}

static void testVector_PushBack5()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    VET_ASSERT(v.size() == 5, "pushBack5 size==5");
    VET_ASSERT(v[4] == 5, "pushBack5 [4]==5");
}

static void testVector_PushBack6()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5, 6);
    VET_ASSERT(v.size() == 6, "pushBack6 size==6");
    VET_ASSERT(v[5] == 6, "pushBack6 [5]==6");
}

static void testVector_PushBackMove()
{
    Vector<string> v;
    string s = "hello";
    v.add(move(s));
    VET_ASSERT(v.size() == 1, "pushBackMove size==1");
    VET_ASSERT(v[0] == "hello", "pushBackMove [0]==hello");
}

// -----------------------------------------------------------
// addUnique / addNotEqual / addUniqueOrErase
// -----------------------------------------------------------
static void testVector_AddUnique_VET()
{
    Vector<int> v;
    bool r1 = v.addUnique(10);
    bool r2 = v.addUnique(20);
    bool r3 = v.addUnique(10); // 重复
    VET_ASSERT(r1 && r2, "addUnique new elements return true");
    VET_ASSERT(!r3, "addUnique duplicate returns false");
    VET_ASSERT(v.size() == 2, "addUnique size==2");
}

static void testVector_AddNotEqual()
{
    Vector<int> v;
    v.addNotEqual(10, 0); // 不等于 0，添加
    v.addNotEqual(0, 0);  // 等于 0，不添加
    v.addNotEqual(5, 0);  // 不等于 0，添加
    VET_ASSERT(v.size() == 2, "addNotEqual size==2");
    VET_ASSERT(v[0] == 10 && v[1] == 5, "addNotEqual values");
}

static void testVector_AddUniqueOrErase_Add()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    v.addUniqueOrRemove(3, true); // add
    VET_ASSERT(v.contains(3), "addUniqueOrErase add contains 3");
    VET_ASSERT(v.size() == 3, "addUniqueOrErase add size==3");
}

static void testVector_AddUniqueOrErase_Erase()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    v.add(3);
    v.addUniqueOrRemove(2, false); // erase
    VET_ASSERT(!v.contains(2), "addUniqueOrErase erase not contains 2");
    VET_ASSERT(v.size() == 2, "addUniqueOrErase erase size==2");
}

// -----------------------------------------------------------
// addRange / setRange (基础类型)
// -----------------------------------------------------------
static void testVector_AddRange()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    Vector<int> src;
    src.add(3);
    src.add(4);
    src.add(5);
    v.addRange(src);
    VET_ASSERT(v.size() == 5, "addRange size==5");
    VET_ASSERT(v[4] == 5, "addRange [4]==5");
}

static void testVector_SetRange()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    Vector<int> src;
    src.add(10);
    src.add(20);
    v.setRange(src);
    VET_ASSERT(v.size() == 2, "setRange size==2");
    VET_ASSERT(v[0] == 10 && v[1] == 20, "setRange values");
}

static void testVector_AddRangeRawPtr()
{
    Vector<int> v;
    int arr[] = { 10, 20, 30, 40, 50 };
    v.addRange(arr, 5);
    VET_ASSERT(v.size() == 5, "addRangeRawPtr size==5");
    VET_ASSERT(v[0] == 10 && v[4] == 50, "addRangeRawPtr values");
}

static void testVector_SetRangeRawPtr()
{
    Vector<int> v;
    v.add(999);
    int arr[] = { 1, 2, 3 };
    v.setRange(arr, 3);
    VET_ASSERT(v.size() == 3, "setRangeRawPtr size==3");
    VET_ASSERT(v[0] == 1 && v[2] == 3, "setRangeRawPtr values");
}

static void testVector_AddRangeCopy()
{
    Vector<string> v;
    v.add("a");
    // addRangeCopy with Vector<string>
    Vector<string> src;
    src.add("b");
    src.add("c");
    src.add("d");
    v.addRangeCopy(src);
    VET_ASSERT(v.size() == 4, "addRangeCopy size==4");
    VET_ASSERT(v[1] == "b" && v[3] == "d", "addRangeCopy values");
}

static void testVector_AddRangeMove()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    Vector<int> src;
    src.add(3);
    src.add(4);
    v.addRange(move(src));
    VET_ASSERT(v.size() == 4, "addRangeMove size==4");
    VET_ASSERT(src.isEmpty(), "addRangeMove src cleared");
}

static void testVector_AddRangeFromArrayList()
{
    Vector<int> v;
    ArrayList<10, int> al;
    al.add(1);
    al.add(2);
    al.add(3);
    v.addRange(al);
    VET_ASSERT(v.size() == 3, "addRangeArrayList size==3");
    VET_ASSERT(v[0] == 1 && v[2] == 3, "addRangeArrayList values");
}

// -----------------------------------------------------------
// erase 系列
// -----------------------------------------------------------
static void testVector_EraseAt()
{
    Vector<int> v;
    v.add(10, 20, 30, 40, 50);
    v.removeAt(2); // 删除 30
    VET_ASSERT(v.size() == 4, "removeAt size==4");
    VET_ASSERT(v[2] == 40, "removeAt [2]==40");
}

static void testVector_EraseAtOutOfRange()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.removeAt(-1); // 不应崩溃
    v.removeAt(99); // 不应崩溃
    VET_ASSERT(v.size() == 3, "eraseAtOutOfRange size unchanged");
}

static void testVector_EraseAtCount()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.add(i);
    }
    v.removeRange(2, 3); // 删除 index 2,3,4 三个元素
    VET_ASSERT(v.size() == 7, "eraseAtCount size==7");
    VET_ASSERT(v[2] == 5, "eraseAtCount [2]==5");
}

static void testVector_EraseElement()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    bool r1 = v.remove(3);
    bool r2 = v.remove(99);
    VET_ASSERT(r1, "remove found returns true");
    VET_ASSERT(!r2, "remove notFound returns false");
    VET_ASSERT(v.size() == 4, "remove size==4");
    VET_ASSERT(!v.contains(3), "remove not contains 3");
}

static void testVector_EraseAllElement_VET()
{
    Vector<int> v;
    v.add(1, 2, 3, 2, 4, 2);
    int count = v.removeAll(2);
    VET_ASSERT(count == 3, "eraseAll count==3");
    VET_ASSERT(v.size() == 3, "eraseAll size==3");
    VET_ASSERT(!v.contains(2), "eraseAll not contains 2");
}

static void testVector_EraseLast()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.removeLast();
    VET_ASSERT(v.size() == 2, "eraseLast size==2");
    VET_ASSERT(v[1] == 2, "eraseLast [1]==2");
}

static void testVector_EraseLastEmpty()
{
    Vector<int> v;
    v.removeLast(); // 不应崩溃
    VET_ASSERT(v.isEmpty(), "eraseLastEmpty still empty");
}

static void testVector_PopBack()
{
    Vector<int> v;
    v.add(10, 20, 30);
    int val = v.popBack(-1);
    VET_ASSERT(val == 30, "popBack val==30");
    VET_ASSERT(v.size() == 2, "popBack size==2");
}

static void testVector_PopBackEmpty()
{
    Vector<int> v;
    int val = v.popBack(-99);
    VET_ASSERT(val == -99, "popBackEmpty returns default");
    VET_ASSERT(v.isEmpty(), "popBackEmpty still empty");
}

// -----------------------------------------------------------
// contains / isAll / findFirstIndex / findElementCount
// -----------------------------------------------------------
static void testVector_Contains()
{
    Vector<int> v;
    v.add(5, 10, 15);
    VET_ASSERT(v.contains(10), "contains 10 true");
    VET_ASSERT(!v.contains(99), "contains 99 false");
    VET_ASSERT(!Vector<int>().contains(1), "empty contains false");
}

static void testVector_IsAll()
{
    Vector<int> v;
    v.add(5, 5, 5);
    VET_ASSERT(v.isAll(5), "isAll(5) true");
    VET_ASSERT(!v.isAll(4), "isAll(4) false");

    Vector<int> empty;
    VET_ASSERT(empty.isAll(0), "isAll empty true");
}

static void testVector_FindFirstIndex()
{
    Vector<int> v;
    v.add(10, 20, 30, 20, 10);
    VET_ASSERT(v.findIndex(20) == 1, "findFirstIndex 20==1");
    VET_ASSERT(v.findIndex(20, 2) == 3, "findFirstIndex 20 from2==3");
    VET_ASSERT(v.findIndex(99) == -1, "findFirstIndex 99==-1");
}

static void testVector_FindElementCount()
{
    Vector<int> v;
    v.add(1, 2, 3, 2, 2, 4);
    VET_ASSERT(v.findCount(2) == 3, "findElementCount(2)==3");
    VET_ASSERT(v.findCount(1) == 1, "findElementCount(1)==1");
    VET_ASSERT(v.findCount(99) == 0, "findElementCount(99)==0");
}

// -----------------------------------------------------------
// swapIndex / random / randomRemove
// -----------------------------------------------------------
static void testVector_SwapIndex()
{
    Vector<int> v;
    v.add(10, 20, 30, 40, 50);
    v.swapIndex(0, 4);
    VET_ASSERT(v[0] == 50, "swapIndex [0]==50");
    VET_ASSERT(v[4] == 10, "swapIndex [4]==10");
}

static void testVector_Random()
{
    Vector<int> v;
    for (int i = 0; i < 100; ++i) { v.add(i); }
    // random 应返回在范围内的值
    for (int t = 0; t < 10; ++t)
    {
        const int& r = v.random();
        VET_ASSERT(r >= 0 && r < 100, "random in range");
    }
}

static void testVector_RandomEmpty()
{
    Vector<int> v;
    const int& r = v.random();
    VET_ASSERT(r == 0, "randomEmpty returns default");
}

static void testVector_RandomErase()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i) { v.add(i); }
    int before = v.size();
    v.randomRemove();
    VET_ASSERT(v.size() == before - 1, "randomRemove size decreased");
}

// -----------------------------------------------------------
// insert
// -----------------------------------------------------------
static void testVector_InsertAt()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.insert(1, 99);
    VET_ASSERT(v.size() == 4, "insertAt size==4");
    VET_ASSERT(v[1] == 99, "insertAt [1]==99");
    VET_ASSERT(v[2] == 2, "insertAt [2]==2");
}

static void testVector_InsertAtBegin()
{
    Vector<int> v;
    v.add(10, 20, 30);
    v.insert(0, 5);
    VET_ASSERT(v[0] == 5, "insertAtBegin [0]==5");
    VET_ASSERT(v.size() == 4, "insertAtBegin size==4");
}

static void testVector_InsertAtEnd()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.insert(3, 999);
    VET_ASSERT(v[3] == 999, "insertAtEnd [3]==999");
    VET_ASSERT(v.size() == 4, "insertAtEnd size==4");
}

// -----------------------------------------------------------
// cloneTo
// -----------------------------------------------------------
static void testVector_CloneTo()
{
    Vector<int> src;
    src.add(10, 20, 30);
    Vector<int> dst;
    src.cloneTo(dst);
    VET_ASSERT(dst.size() == 3, "cloneTo size==3");
    VET_ASSERT(dst[0] == 10 && dst[2] == 30, "cloneTo values");
    // 修改 src 不影响 dst
    src[0] = 999;
    VET_ASSERT(dst[0] == 10, "cloneTo independent");
}

// -----------------------------------------------------------
// sort
// -----------------------------------------------------------
static void testVector_Sort()
{
    Vector<int> v;
    v.add(5, 3, 1, 4, 2);
    v.sort();
    VET_ASSERT(v[0] == 1 && v[1] == 2 && v[4] == 5, "sort ascending");
}

static void testVector_SortSingleElement()
{
    Vector<int> v;
    v.add(42);
    v.sort();
    VET_ASSERT(v[0] == 42, "sortSingle unchanged");
}

static void testVector_SortAlreadySorted()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    v.sort();
    for (int i = 0; i < 5; ++i)
    {
        VET_ASSERT(v[i] == i + 1, "sortAlreadySorted correct");
    }
}

// -----------------------------------------------------------
// resize / reserve / get
// -----------------------------------------------------------
static void testVector_Resize()
{
    Vector<int> v;
    v.resize(5);
    VET_ASSERT(v.size() == 5, "resize size==5");
    VET_ASSERT(v[0] == 0, "resize default zero");
}

static void testVector_ResizeDown()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i) { v.add(i); }
    v.resize(3);
    VET_ASSERT(v.size() == 3, "resizeDown size==3");
}

static void testVector_Reserve()
{
    Vector<int> v;
    v.reserve(100);
    // 只预分配，不改变 size
    VET_ASSERT(v.size() == 0, "reserve size still 0");
    VET_ASSERT(v.isEmpty(), "reserve isEmpty true");
}

static void testVector_GetDefault()
{
    Vector<int> v;
    v.add(10, 20, 30);
    VET_ASSERT(v.get(1) == 20, "get[1]==20");
    VET_ASSERT(v.get(-1) == 0, "get[-1] default 0");
    VET_ASSERT(v.get(99) == 0, "get[99] default 0");
}

static void testVector_GetWithCustomDefault()
{
    Vector<int> v;
    v.add(1, 2, 3);
    VET_ASSERT(v.get(10, -99) == -99, "getCustomDefault -99");
    VET_ASSERT(v.get(1, -99) == 2, "getCustomDefault in range 2");
}

// -----------------------------------------------------------
// operator== / operator!=
// -----------------------------------------------------------
static void testVector_EqualityOp()
{
    Vector<int> v1;
    v1.add(1, 2, 3);
    Vector<int> v2;
    v2.add(1, 2, 3);
    VET_ASSERT(v1 == v2, "equality true");
    VET_ASSERT(!(v1 != v2), "inequality false");
}

static void testVector_InequalityOp()
{
    Vector<int> v1;
    v1.add(1, 2, 3);
    Vector<int> v2;
    v2.add(1, 2, 4);
    VET_ASSERT(v1 != v2, "inequality true");
    VET_ASSERT(!(v1 == v2), "equality false");
}

static void testVector_EqualityDifferentSize()
{
    Vector<int> v1;
    v1.add(1, 2);
    Vector<int> v2;
    v2.add(1, 2, 3);
    VET_ASSERT(v1 != v2, "diffSize inequality true");
}

// -----------------------------------------------------------
// clear / dispose / isEmpty
// -----------------------------------------------------------
static void testVector_Clear()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.clear();
    VET_ASSERT(v.isEmpty(), "clear isEmpty");
    VET_ASSERT(v.size() == 0, "clear size==0");
}

static void testVector_ClearEmpty()
{
    Vector<int> v;
    v.clear(); // 不应崩溃
    VET_ASSERT(v.isEmpty(), "clearEmpty still empty");
}

static void testVector_IsEmpty()
{
    Vector<int> v;
    VET_ASSERT(v.isEmpty(), "isEmpty true on fresh");
    v.add(1);
    VET_ASSERT(!v.isEmpty(), "isEmpty false after push");
}

// -----------------------------------------------------------
// 综合场景测试
// -----------------------------------------------------------
static void testVector_LargeVolumePushBack()
{
    Vector<int> v;
    const int N = 10000;
    FOR(N)
    {
        v.add(i);
    }
    VET_ASSERT(v.size() == N, "largeVolume size==10000");
    VET_ASSERT(v[0] == 0 && v[N - 1] == N - 1, "largeVolume boundary values");
}

static void testVector_EraseAllThenAdd()
{
    Vector<int> v;
    v.add(5, 5, 5, 5, 5);
    int c = v.removeAll(5);
    VET_ASSERT(c == 5, "eraseAllThenAdd erased 5");
    VET_ASSERT(v.isEmpty(), "eraseAllThenAdd empty");
    v.add(1);
    VET_ASSERT(v.size() == 1 && v[0] == 1, "eraseAllThenAdd re-add");
}

static void testVector_MixedOperations()
{
    Vector<int> v;
    FOR(20)
    {
        v.add(i);
    }
    // erase 一半
    for (int i = 19; i >= 0; i -= 2)
    {
        v.removeAt(i);
    }
    VET_ASSERT(v.size() == 10, "mixedOps size==10");
    // 确保只剩偶数
    FOR_VECTOR(v)
    {
        VET_ASSERT(v[i] % 2 == 0, "mixedOps only evens");
    }
}

static void testVector_AddRangeThenSort()
{
    Vector<int> v1;
    Vector<int> v2;
    v2.add(5, 3, 1, 4, 2);
    v1.addRange(v2);
    v1.sort();
    VET_ASSERT(v1.size() == 5, "addRangeSort size==5");
    VET_ASSERT(v1[0] == 1 && v1[4] == 5, "addRangeSort sorted");
}

static void testVector_SwapIndexMultiple()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.add(i);
    }
    // 手动反转
    for (int i = 0; i < 5; ++i)
    {
        v.swapIndex(i, 9 - i);
    }
    VET_ASSERT(v[0] == 9 && v[9] == 0, "swapMultiple reversed ends");
    VET_ASSERT(v[4] == 5 && v[5] == 4, "swapMultiple middle");
}

static void testVector_FindFirstIndexMultipleOccurrences()
{
    Vector<int> v;
    v.add(0, 1, 2, 0, 1, 2);
    v.add(0);
    VET_ASSERT(v.findIndex(0) == 0, "findIndex 0 start==0");
    VET_ASSERT(v.findIndex(0, 1) == 3, "findIndex 0 from1==3");
    VET_ASSERT(v.findIndex(0, 4) == 6, "findIndex 0 from4==6");
    VET_ASSERT(v.findIndex(0, 7) == -1, "findIndex 0 from7==-1");
}

static void testVector_EraseElementNotExist()
{
    Vector<int> v;
    v.add(1, 2, 3);
    bool r = v.remove(99);
    VET_ASSERT(!r, "eraseNotExist false");
    VET_ASSERT(v.size() == 3, "eraseNotExist size unchanged");
}

static void testVector_PopBackUntilEmpty()
{
    Vector<int> v;
    v.add(10, 20, 30);
    VET_ASSERT(v.popBack(0) == 30, "popBack 30");
    VET_ASSERT(v.popBack(0) == 20, "popBack 20");
    VET_ASSERT(v.popBack(0) == 10, "popBack 10");
    VET_ASSERT(v.popBack(-1) == -1, "popBack empty default");
    VET_ASSERT(v.isEmpty(), "popBack until empty");
}

static void testVector_CloneToOverwrite()
{
    Vector<int> src;
    src.add(1, 2, 3);
    Vector<int> dst;
    dst.add(99, 88);
    src.cloneTo(dst);
    VET_ASSERT(dst.size() == 3, "cloneToOverwrite size==3");
    VET_ASSERT(dst[0] == 1, "cloneToOverwrite [0]==1");
}

static void testVector_StringVector()
{
    Vector<string> v;
    v.add("alpha");
    v.add("beta");
    v.add("gamma");
    VET_ASSERT(v.size() == 3, "stringVec size==3");
    VET_ASSERT(v.contains("beta"), "stringVec contains beta");
    VET_ASSERT(!v.contains("delta"), "stringVec not contains delta");

    v.remove("beta");
    VET_ASSERT(v.size() == 2, "stringVec erase size==2");
    VET_ASSERT(!v.contains("beta"), "stringVec erased beta");
}

static void testVector_IntegrationSortAndSearch()
{
    Vector<int> v;
    for (int i = 100; i >= 1; --i)
    {
        v.add(i);
    }
    v.sort();
    VET_ASSERT(v[0] == 1 && v[99] == 100, "integrationSort boundary");
    VET_ASSERT(v.findIndex(50) == 49, "integrationSort findIndex 50");
    VET_ASSERT(v.findCount(50) == 1, "integrationSort count 50==1");
}

static void testVector_AddUniqueMultiple()
{
    Vector<int> v;
    for (int i = 0; i < 100; ++i)
    {
        v.addUnique(i % 10);
    }
    VET_ASSERT(v.size() == 10, "addUniqueMultiple size==10");
    for (int i = 0; i < 10; ++i)
    {
        VET_ASSERT(v.contains(i), "addUniqueMultiple contains " + IToS(i));
    }
}

static void testVector_InsertInMiddle()
{
    Vector<int> v;
    for (int i = 0; i < 5; ++i)
    {
        v.add(i * 10);
    }
    v.insert(2, 999);
    VET_ASSERT(v.size() == 6, "insertMiddle size==6");
    VET_ASSERT(v[2] == 999, "insertMiddle [2]==999");
    VET_ASSERT(v[3] == 20, "insertMiddle [3]==20 (shifted)");
}

static void testVector_EraseAtMultipleSegments()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.add(i);
    }
    v.removeRange(0, 3); // 删除前 3 个
    VET_ASSERT(v.size() == 7, "eraseAtMulti size==7");
    VET_ASSERT(v[0] == 3, "eraseAtMulti [0]==3");
}

static void testVector_EmplaceBack()
{
    Vector<int> v;
    v.emplace(42);
    v.emplace(100);
    VET_ASSERT(v.size() == 2, "emplaceBack size==2");
    VET_ASSERT(v[0] == 42 && v[1] == 100, "emplaceBack values");
}

static void testVector_ClearAndReserve()
{
    Vector<int> v;
    for (int i = 0; i < 50; ++i)
    {
        v.add(i);
    }
    v.clear();
    v.reserve(200);
    VET_ASSERT(v.isEmpty(), "clearAndReserve empty");
    v.add(1);
    VET_ASSERT(v.size() == 1, "clearAndReserve after push size==1");
}

static void testVector_SetRangeCopy_String()
{
    Vector<string> v;
    v.add("old1"); v.add("old2");
    Vector<string> src;
    src.add("new1"); src.add("new2"); src.add("new3");
    v.setRangeCopy(src);
    VET_ASSERT(v.size() == 3, "setRangeCopyStr size==3");
    VET_ASSERT(v[0] == "new1" && v[2] == "new3", "setRangeCopyStr values");
}

static void testVector_ResetElement()
{
    Vector<int> v;
    v.add(10, 20, 30, 20, 40);
    bool r = v.resetElement(20);
    VET_ASSERT(r, "resetElement found true");
    VET_ASSERT(v[1] == 0, "resetElement [1]==0 (default)");
    VET_ASSERT(v[3] == 20, "resetElement only first occurrence");
}

static void testVector_ResetElementAt()
{
    Vector<int> v;
    v.add(1, 2, 3, 4);
    bool r = v.resetElementAt(2);
    VET_ASSERT(r, "resetElementAt found true");
    VET_ASSERT(v[2] == 0, "resetElementAt [2]==0");
    VET_ASSERT(!v.resetElementAt(-1), "resetElementAt -1 false");
    VET_ASSERT(!v.resetElementAt(99), "resetElementAt 99 false");
}

static void testVector_DataPointer()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    const int* p = v.data();
    VET_ASSERT(p != nullptr, "data() non-null");
    VET_ASSERT(p[0] == 1 && p[4] == 5, "data() values");
}

static void testVector_IteratorBasic()
{
    Vector<int> v;
    v.add(10, 20, 30, 40, 50);
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        sum += *it;
    }
    VET_ASSERT(sum == 150, "iterator sum==150");
}

static void testVector_ReverseIterator()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    int last = -1;
    for (auto it = v.rbegin(); it != v.rend(); ++it)
    {
        last = *it;
        break;
    }
    VET_ASSERT(last == 5, "reverseIterator first==5");
}

// ============================================================
// 主入口
// ============================================================
void VectorExtTest::test()
{
    testVector_PushBackSingle();
    testVector_PushBack2();
    testVector_PushBack3();
    testVector_PushBack4();
    testVector_PushBack5();
    testVector_PushBack6();
    testVector_PushBackMove();
    testVector_AddUnique_VET();
    testVector_AddNotEqual();
    testVector_AddUniqueOrErase_Add();
    testVector_AddUniqueOrErase_Erase();
    testVector_AddRange();
    testVector_SetRange();
    testVector_AddRangeRawPtr();
    testVector_SetRangeRawPtr();
    testVector_AddRangeCopy();
    testVector_AddRangeMove();
    testVector_AddRangeFromArrayList();
    testVector_EraseAt();
    testVector_EraseAtOutOfRange();
    testVector_EraseAtCount();
    testVector_EraseElement();
    testVector_EraseAllElement_VET();
    testVector_EraseLast();
    testVector_EraseLastEmpty();
    testVector_PopBack();
    testVector_PopBackEmpty();
    testVector_Contains();
    testVector_IsAll();
    testVector_FindFirstIndex();
    testVector_FindElementCount();
    testVector_SwapIndex();
    testVector_Random();
    testVector_RandomEmpty();
    testVector_RandomErase();
    testVector_InsertAt();
    testVector_InsertAtBegin();
    testVector_InsertAtEnd();
    testVector_CloneTo();
    testVector_Sort();
    testVector_SortSingleElement();
    testVector_SortAlreadySorted();
    testVector_Resize();
    testVector_ResizeDown();
    testVector_Reserve();
    testVector_GetDefault();
    testVector_GetWithCustomDefault();
    testVector_EqualityOp();
    testVector_InequalityOp();
    testVector_EqualityDifferentSize();
    testVector_Clear();
    testVector_ClearEmpty();
    testVector_IsEmpty();
    testVector_LargeVolumePushBack();
    testVector_EraseAllThenAdd();
    testVector_MixedOperations();
    testVector_AddRangeThenSort();
    testVector_SwapIndexMultiple();
    testVector_FindFirstIndexMultipleOccurrences();
    testVector_EraseElementNotExist();
    testVector_PopBackUntilEmpty();
    testVector_CloneToOverwrite();
    testVector_StringVector();
    testVector_IntegrationSortAndSearch();
    testVector_AddUniqueMultiple();
    testVector_InsertInMiddle();
    testVector_EraseAtMultipleSegments();
    testVector_EmplaceBack();
    testVector_ClearAndReserve();
    testVector_SetRangeCopy_String();
    testVector_ResetElement();
    testVector_ResetElementAt();
    testVector_DataPointer();
    testVector_IteratorBasic();
    testVector_ReverseIterator();

}
