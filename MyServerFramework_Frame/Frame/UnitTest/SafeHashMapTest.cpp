#include "FrameHeader.h"
 struct SafeHashMapScopeTestImpl
{

#define SafeHashMapScopeTestImpl_SHMS_ASSERT(condition) if (!(condition)) { ERROR("SafeHashMapScopeTest failed: " #condition); }

static void test()
{
    testSafeHashMapBasicInsert();
    testSafeHashMapBasicErase();
    testSafeHashMapScopeReadOnly();
    testSafeHashMapInsertDuringForeach();
    testSafeHashMapEraseDuringForeach();
    testSafeHashMapClearAfterForeach();
    testSafeHashMapContainsAndGet();
    testSafeHashMapEraseWithValue();
    testSafeHashMapSizeAndEmpty();
    testSafeSetBasicInsert();
    testSafeSetBasicErase();
    testSafeSetScopeReadOnly();
    testSafeSetInsertDuringForeach();
    testSafeSetEraseDuringForeach();
    testSafeSetClearAfterForeach();
    testSafeSetContains();
    testSafeSetSizeAndEmpty();
}

static void testSafeHashMapBasicInsert()
{
    SafeHashMap<int, string> m;
    bool r1 = m.add(1, "one");
    bool r2 = m.add(2, "two");
    bool r3 = m.add(3, "three");
    SafeHashMapScopeTestImpl_SHMS_ASSERT(r1);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(r2);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(r3);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.size() == 3);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.contains(1));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.contains(2));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.contains(3));
}

static void testSafeHashMapBasicErase()
{
    SafeHashMap<int, int> m;
    m.add(10, 100);
    m.add(20, 200);
    m.add(30, 300);
    bool erased = m.remove(20);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(erased);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.size() == 2);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!m.contains(20));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.contains(10));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.contains(30));

    bool notErased = m.remove(999);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!notErased);
}

static void testSafeHashMapScopeReadOnly()
{
    SafeHashMap<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    int count = 0;
    {
        SafeHashMapScope<int, int> scope(m);
        const HashMap<int, int>& readList = scope.getReadList();
        for (const auto& item : readList)
        {
            if (item.first > 0)
            {
                count++;
            }
        }
    }
    SafeHashMapScopeTestImpl_SHMS_ASSERT(count == 3);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!m.isForeaching());
}

static void testSafeHashMapInsertDuringForeach()
{
    SafeHashMap<int, string> m;
    m.add(1, "one");
    m.add(2, "two");
    {
        SafeHashMapScope<int, string> scope(m);
        const HashMap<int, string>& readList = scope.getReadList();
        int startSize = readList.size();
        SafeHashMapScopeTestImpl_SHMS_ASSERT(startSize == 2);
        m.add(3, "three");
        SafeHashMapScopeTestImpl_SHMS_ASSERT(m.contains(3));
        SafeHashMapScopeTestImpl_SHMS_ASSERT(m.size() == 3);
        SafeHashMapScopeTestImpl_SHMS_ASSERT(readList.size() == 2);
    }
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.size() == 3);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.contains(3));
}

static void testSafeHashMapEraseDuringForeach()
{
    SafeHashMap<int, string> m;
    m.add(1, "one");
    m.add(2, "two");
    m.add(3, "three");
    {
        SafeHashMapScope<int, string> scope(m);
        const HashMap<int, string>& readList = scope.getReadList();
        SafeHashMapScopeTestImpl_SHMS_ASSERT(readList.size() == 3);
        m.remove(2);
        SafeHashMapScopeTestImpl_SHMS_ASSERT(m.size() == 2);
        SafeHashMapScopeTestImpl_SHMS_ASSERT(!m.contains(2));
        SafeHashMapScopeTestImpl_SHMS_ASSERT(readList.size() == 3);
    }
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.size() == 2);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!m.contains(2));
}

static void testSafeHashMapClearAfterForeach()
{
    SafeHashMap<int, int> m;
    m.add(1, 11);
    m.add(2, 22);
    {
        SafeHashMapScope<int, int> scope(m);
        const HashMap<int, int>& readList = scope.getReadList();
        for (const auto& item : readList)
        {
            (void)item;
        }
    }
    m.clear();
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.size() == 0);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.isEmpty());
}

static void testSafeHashMapContainsAndGet()
{
    SafeHashMap<string, int> m;
    m.add("alpha", 1);
    m.add("beta", 2);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.contains("alpha"));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.contains("beta"));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!m.contains("gamma"));

    const int& val = m.get("alpha", -1);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(val == 1);

    const int& missing = m.get("gamma", -99);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(missing == -99);
}

static void testSafeHashMapEraseWithValue()
{
    SafeHashMap<int, int> m;
    m.add(5, 500);
    m.add(6, 600);
    int val = 0;
    bool erased = m.remove(5, val);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(erased);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(val == 500);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!m.contains(5));
}

static void testSafeHashMapSizeAndEmpty()
{
    SafeHashMap<int, int> m;
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.isEmpty());
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.size() == 0);
    m.add(1, 10);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!m.isEmpty());
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.size() == 1);
    m.remove(1);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(m.isEmpty());
}

static void testSafeSetBasicInsert()
{
    SafeSet<int> s;
    s.add(10);
    s.add(20);
    s.add(30);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.size() == 3);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.contains(10));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.contains(20));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.contains(30));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!s.contains(99));
}

static void testSafeSetBasicErase()
{
    SafeSet<int> s;
    s.add(1);
    s.add(2);
    s.add(3);
    bool erased = s.remove(2);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(erased);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.size() == 2);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!s.contains(2));

    bool notErased = s.remove(999);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!notErased);
}

static void testSafeSetScopeReadOnly()
{
    SafeSet<string> s;
    s.add("a");
    s.add("b");
    s.add("c");
    int count = 0;
    {
        SafeSetScope<string> scope(s);
        const Set<string>& readSet = scope.getReadList();
        for (const auto& item : readSet)
        {
            if (item != "")
            {
                count++;
            }
        }
    }
    SafeHashMapScopeTestImpl_SHMS_ASSERT(count == 3);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!s.isForeaching());
}

static void testSafeSetInsertDuringForeach()
{
    SafeSet<int> s;
    s.add(1);
    s.add(2);
    {
        SafeSetScope<int> scope(s);
        const Set<int>& readSet = scope.getReadList();
        SafeHashMapScopeTestImpl_SHMS_ASSERT(readSet.size() == 2);
        s.add(3);
        SafeHashMapScopeTestImpl_SHMS_ASSERT(s.contains(3));
        SafeHashMapScopeTestImpl_SHMS_ASSERT(s.size() == 3);
        SafeHashMapScopeTestImpl_SHMS_ASSERT(readSet.size() == 2);
    }
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.size() == 3);
}

static void testSafeSetEraseDuringForeach()
{
    SafeSet<int> s;
    s.add(10);
    s.add(20);
    s.add(30);
    {
        SafeSetScope<int> scope(s);
        const Set<int>& readSet = scope.getReadList();
        SafeHashMapScopeTestImpl_SHMS_ASSERT(readSet.size() == 3);
        s.remove(20);
        SafeHashMapScopeTestImpl_SHMS_ASSERT(!s.contains(20));
        SafeHashMapScopeTestImpl_SHMS_ASSERT(s.size() == 2);
        SafeHashMapScopeTestImpl_SHMS_ASSERT(readSet.size() == 3);
    }
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.size() == 2);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!s.contains(20));
}

static void testSafeSetClearAfterForeach()
{
    SafeSet<int> s;
    s.add(100);
    s.add(200);
    {
        SafeSetScope<int> scope(s);
        const Set<int>& readSet = scope.getReadList();
        for (const int val : readSet)
        {
            (void)val;
        }
    }
    s.clear();
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.size() == 0);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.isEmpty());
}

static void testSafeSetContains()
{
    SafeSet<string> s;
    s.add("foo");
    s.add("bar");
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.contains("foo"));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.contains("bar"));
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!s.contains("baz"));
}

static void testSafeSetSizeAndEmpty()
{
    SafeSet<int> s;
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.isEmpty());
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.size() == 0);
    s.add(42);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(!s.isEmpty());
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.size() == 1);
    s.remove(42);
    SafeHashMapScopeTestImpl_SHMS_ASSERT(s.isEmpty());
}
};

void SafeHashMapTest::test()
{
	SafeHashMapScopeTestImpl::test();
}
