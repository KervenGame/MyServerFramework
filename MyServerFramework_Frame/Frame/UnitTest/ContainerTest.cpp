#include "FrameHeader.h"

#define CT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("ContainerTest FAIL: ") + (msg)); } } while(0)
#define CT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("ContainerTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define CT_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("ContainerTest FAIL: ") + (msg) + \
		" | expected=" + (b) + " actual=" + (a)); } } while(0)

//------------------------------------------------------------------------------
// Vector<T>
//------------------------------------------------------------------------------
static void testVector_Basic()
{
	// default ctor -> empty
	Vector<int> v;
	CT_ASSERT(v.isEmpty(), "Vector default isEmpty");
	CT_ASSERT_EQ(v.size(), 0, "Vector default size=0");

	// add / size / operator[]
	v.add(10);
	v.add(20);
	v.add(30);
	CT_ASSERT_EQ(v.size(), 3, "Vector size after push 3");
	CT_ASSERT_EQ(v[0], 10, "Vector[0]=10");
	CT_ASSERT_EQ(v[1], 20, "Vector[1]=20");
	CT_ASSERT_EQ(v[2], 30, "Vector[2]=30");
	CT_ASSERT(!v.isEmpty(), "Vector not empty");

	// contains
	CT_ASSERT(v.contains(20),  "Vector contains 20");
	CT_ASSERT(!v.contains(99), "Vector not contains 99");

	// remove (removes first match)
	CT_ASSERT(v.remove(20), "Vector remove 20 ok");
	CT_ASSERT_EQ(v.size(), 2, "Vector size=2 after erase");
	CT_ASSERT(!v.contains(20), "Vector 20 gone after erase");

	// removeAt
	v.removeAt(0);
	CT_ASSERT_EQ(v.size(), 1, "Vector size=1 after removeAt(0)");
	CT_ASSERT_EQ(v[0], 30, "Vector[0]=30 after removeAt");

	// eraseLast
	v.removeLast();
	CT_ASSERT_EQ(v.size(), 0, "Vector size=0 after eraseLast");

	// popBack
	v.add(100);
	v.add(200);
	const int popped = v.popBack(-1);
	CT_ASSERT_EQ(popped, 200, "Vector popBack=200");
	CT_ASSERT_EQ(v.size(), 1, "Vector size=1 after popBack");

	// clear
	v.add(1);
	v.add(2);
	v.clear();
	CT_ASSERT_EQ(v.size(), 0, "Vector size=0 after clear");
	CT_ASSERT(v.isEmpty(), "Vector isEmpty after clear");
}

static void testVector_AddUnique()
{
	Vector<int> v;
	CT_ASSERT(v.addUnique(5),  "addUnique 5 first time = true");
	CT_ASSERT(!v.addUnique(5), "addUnique 5 again = false");
	CT_ASSERT_EQ(v.size(), 1, "addUnique size=1");

	v.addUnique(10);
	CT_ASSERT_EQ(v.size(), 2, "addUnique size=2 after 10");
}

static void testVector_Initializer()
{
	// initializer_list ctor
	Vector<int> v{ 1, 2, 3, 4, 5 };
	CT_ASSERT_EQ(v.size(), 5, "Vector init-list size=5");
	CT_ASSERT_EQ(v[4], 5, "Vector init-list [4]=5");
}

static void testVector_EraseAllElement()
{
	Vector<int> v{ 1, 2, 2, 3, 2 };
	const int n = v.removeAll(2);
	CT_ASSERT_EQ(n, 3, "eraseAllElement count=3");
	CT_ASSERT_EQ(v.size(), 2, "eraseAllElement size=2");
	CT_ASSERT(!v.contains(2), "eraseAllElement 2 gone");
}

//------------------------------------------------------------------------------
// HashMap<K,V>
//------------------------------------------------------------------------------
static void testHashMap_Basic()
{
	HashMap<int, string> m;
	CT_ASSERT(m.isEmpty(), "HashMap default isEmpty");
	CT_ASSERT_EQ(m.size(), 0, "HashMap default size=0");

	// insert
	CT_ASSERT(m.add(1, string("one")),   "HashMap insert 1 ok");
	CT_ASSERT(m.add(2, string("two")),   "HashMap insert 2 ok");
	CT_ASSERT(m.add(3, string("three")), "HashMap insert 3 ok");
	CT_ASSERT(!m.add(1, string("ONE")),  "HashMap insert dup 1 fail");
	CT_ASSERT_EQ(m.size(), 3, "HashMap size=3");
	CT_ASSERT(!m.isEmpty(), "HashMap not empty");

	// contains
	CT_ASSERT(m.contains(2),  "HashMap contains 2");
	CT_ASSERT(!m.contains(9), "HashMap not contains 9");

	// get / get
	CT_ASSERT_STR(m.get(1).c_str(),              "one",     "HashMap get 1=one");
	CT_ASSERT_STR(m.get(2, string("?")).c_str(), "two",  "HashMap get 2=two");
	CT_ASSERT_STR(m.get(9, string("?")).c_str(), "?",    "HashMap get miss=?");

	// getPtr
	string* p = m.getPtr(3);
	CT_ASSERT(p != nullptr,           "HashMap getPtr 3 non-null");
	CT_ASSERT_STR(p->c_str(), "three", "HashMap getPtr 3=three");
	CT_ASSERT(m.getPtr(9) == nullptr, "HashMap getPtr miss=null");

	// trySet
	CT_ASSERT(m.set(1, string("ONE")),  "HashMap trySet 1 ok");
	CT_ASSERT_STR(m.get(1).c_str(), "ONE", "HashMap after trySet 1=ONE");
	CT_ASSERT(!m.set(9, string("x")),   "HashMap trySet miss=false");

	// addOrSet
	CT_ASSERT(!m.addOrSet(1, string("one_v2")), "HashMap addOrSet 1 = update");
	CT_ASSERT_STR(m.get(1).c_str(), "one_v2", "HashMap after update 1=one_v2");
	CT_ASSERT(m.addOrSet(4, string("four")), "HashMap addOrSet 4 = insert");
	CT_ASSERT_EQ(m.size(), 4, "HashMap size=4 after addOrSet");

	// remove(key)
	CT_ASSERT(m.remove(2),  "HashMap remove 2 ok");
	CT_ASSERT_EQ(m.size(), 3, "HashMap size=3 after remove");
	CT_ASSERT(!m.contains(2), "HashMap 2 gone after remove");
	CT_ASSERT(!m.remove(9), "HashMap remove miss=false");

	// remove(key, value)
	string outVal;
	CT_ASSERT(m.remove(3, outVal), "HashMap remove(3,v) ok");
	CT_ASSERT_STR(outVal.c_str(), "three", "HashMap erase got 'three'");

	// clear
	m.clear();
	CT_ASSERT(m.isEmpty(), "HashMap isEmpty after clear");
}

static void testHashMap_InsertOrGet()
{
	HashMap<int, int> m;
	int& v1 = m.addOrGet(10, 100);
	CT_ASSERT_EQ(v1, 100, "addOrGet new key=100");
	CT_ASSERT_EQ(m.size(), 1, "addOrGet size=1");

	int& v2 = m.addOrGet(10, 999);
	CT_ASSERT_EQ(v2, 100, "addOrGet existing key still 100");
	CT_ASSERT_EQ(m.size(), 1, "addOrGet size still 1");
}

static void testHashMap_Merge()
{
	HashMap<int, int> a;
	a.add(1, 10);
	a.add(2, 20);

	HashMap<int, int> b;
	b.add(3, 30);
	b.add(1, 99);  // dup -> merge keeps a's value

	const bool result = a.addRange(b);
	CT_ASSERT(!result, "merge returns false (dup key 1)");
	CT_ASSERT_EQ(a.size(), 3, "merge size=3");
	CT_ASSERT_EQ(*a.getPtr(3), 30, "merge key 3=30");
	CT_ASSERT_EQ(*a.getPtr(1), 10, "merge dup key 1 unchanged=10");
}

//------------------------------------------------------------------------------
// Array<Length,T>
//------------------------------------------------------------------------------
static void testArray_Basic()
{
	Array<8, int> arr;
	arr.zero();

	// contains (search in full length)
	CT_ASSERT(!arr.contains(1), "Array contains 1 in zeroed = false");

	// set some values and check contains
	arr.mValue[0] = 10;
	arr.mValue[3] = 20;
	CT_ASSERT(arr.contains(10), "Array contains 10");
	CT_ASSERT(arr.contains(20), "Array contains 20");
	CT_ASSERT(!arr.contains(99), "Array not contains 99");

	// eraseFirstElement
	arr.mValue[1] = 10;  // now 10 appears at index 0 and 1
	int count = 2;
	const bool ok = arr.remove(10, count);
	CT_ASSERT(ok, "remove 10 ok");
	// after erase, count stays but arr[0] shifts
	// (removeAt shifts left, so arr[0] should be the old arr[1]=10... wait, arr[0]=10 was removed)
	// arr before: [10,10,0,20,0,0,0,0], removeAt(0) -> [10,0,20,0,0,0,0,0]
	CT_ASSERT_EQ(arr.mValue[0], 10, "remove arr[0] after erase");
}

static void testArray_FillAndZero()
{
	Array<4, int> arr;
	arr.zero();
	FOR(4)
	{
		CT_ASSERT_EQ(arr.mValue[i], 0, "Array zero fills 0");
	}
}

//------------------------------------------------------------------------------
// MyString<Length>
//------------------------------------------------------------------------------
static void testMyString_Basic()
{
	MyString<64> s;

	// default: isEmpty
	CT_ASSERT(s.isEmpty(), "MyString default isEmpty");
	CT_ASSERT_EQ(s.length(), 0, "MyString default length=0");

	// set / str / length
	s.set("hello");
	CT_ASSERT(!s.isEmpty(), "MyString not empty after set");
	CT_ASSERT_STR(s.str(), "hello", "MyString str=hello");
	CT_ASSERT_EQ(s.length(), 5, "MyString length=5");

	// clear
	s.clear();
	CT_ASSERT(s.isEmpty(), "MyString isEmpty after clear");
	CT_ASSERT_EQ(s.length(), 0, "MyString length=0 after clear");

	// operator[] read
	s.set("abcde");
	CT_ASSERT_EQ((int)s[0], (int)'a', "MyString [0]='a'");
	CT_ASSERT_EQ((int)s[4], (int)'e', "MyString [4]='e'");

	// size / getMaxLength
	CT_ASSERT_EQ(s.getMaxLength(), 64, "MyString getMaxLength=64");

	// copy from string
	string src = "test_copy";
	s.set(src);
	CT_ASSERT_STR(s.str(), "test_copy", "MyString set(std::string)");
	CT_ASSERT_EQ(s.length(), 9, "MyString length=9");
}

//------------------------------------------------------------------------------
// StreamBuffer
//------------------------------------------------------------------------------
static void testStreamBuffer_Basic()
{
	StreamBuffer buf(64);

	// initial state
	CT_ASSERT_EQ(buf.getDataLength(), 0, "StreamBuffer init length=0");

	// addDataToBack
	const char data[] = { 1, 2, 3, 4, 5 };
	CT_ASSERT(buf.addDataToBack(data, 5), "StreamBuffer addData ok");
	CT_ASSERT_EQ(buf.getDataLength(), 5, "StreamBuffer length=5");
	CT_ASSERT_EQ((int)(unsigned char)buf.getData()[0], 1, "StreamBuffer getData[0]=1");
	CT_ASSERT_EQ((int)(unsigned char)buf.getData()[4], 5, "StreamBuffer getData[4]=5");

	// isAvailable
	CT_ASSERT(buf.isAvailable(59),  "StreamBuffer isAvailable(59)");
	CT_ASSERT(!buf.isAvailable(60), "StreamBuffer not isAvailable(60)");

	// removeDataFromFront
	buf.removeDataFromFront(2);
	CT_ASSERT_EQ(buf.getDataLength(), 3, "StreamBuffer length=3 after remove");
	CT_ASSERT_EQ((int)(unsigned char)buf.getData()[0], 3, "StreamBuffer getData[0]=3 after remove");

	// add more then check again
	const char extra[] = { 10, 20 };
	CT_ASSERT(buf.addDataToBack(extra, 2), "StreamBuffer addExtra ok");
	CT_ASSERT_EQ(buf.getDataLength(), 5, "StreamBuffer length=5 after extra");

	// clear
	buf.clear();
	CT_ASSERT_EQ(buf.getDataLength(), 0, "StreamBuffer length=0 after clear");
}

static void testStreamBuffer_AddEmpty()
{
	StreamBuffer buf(32);
	CT_ASSERT(buf.addEmptyToBack(8), "StreamBuffer addEmptyToBack 8 ok");
	CT_ASSERT_EQ(buf.getDataLength(), 8, "StreamBuffer length=8 after addEmpty");
}

//------------------------------------------------------------------------------
// Entry point
//------------------------------------------------------------------------------
void ContainerTest::test()
{
	testVector_Basic();
	testVector_AddUnique();
	testVector_Initializer();
	testVector_EraseAllElement();
	testHashMap_Basic();
	testHashMap_InsertOrGet();
	testHashMap_Merge();
	testArray_Basic();
	testArray_FillAndZero();
	testMyString_Basic();
	testStreamBuffer_Basic();
	testStreamBuffer_AddEmpty();

//------------------------------------------------------------------------------
// Vector<T> — 扩展测试
//------------------------------------------------------------------------------

	// add large volume
	{
		Vector<int> v;
		FOR(1000)
		{
			v.add(i); 
		}
		CT_ASSERT_EQ(v.size(), 1000, "Vector large add size=1000");
		CT_ASSERT_EQ(v[999], 999, "Vector large [999]=999");
	}

	// contains / findFirstIndex
	{
		Vector<int> v;
		v.add(10); 
		v.add(20); 
		v.add(30);
		CT_ASSERT(v.contains(20), "Vector contains 20");
		CT_ASSERT(!v.contains(99), "Vector not contains 99");
		CT_ASSERT_EQ(v.findIndex(20), 1, "Vector findFirstIndex 20=1");
		CT_ASSERT_EQ(v.findIndex(99), -1, "Vector findFirstIndex 99=-1");
	}

	// removeAt single
	{
		Vector<int> v;
		v.add(1); 
		v.add(2);
		v.add(3);
		v.removeAt(1);
		CT_ASSERT_EQ(v.size(), 2, "removeAt size=2");
		CT_ASSERT_EQ(v[0], 1, "removeAt [0]=1");
		CT_ASSERT_EQ(v[1], 3, "removeAt [1]=3");
	}

	// removeAt range
	{
		Vector<int> v;
		FOR(5)
		{
			v.add(i);
		}
		v.removeRange(1, 2); // erase index 1 and 2
		CT_ASSERT_EQ(v.size(), 3, "removeAt range size=3");
		CT_ASSERT_EQ(v[0], 0, "removeAt range [0]=0");
		CT_ASSERT_EQ(v[1], 3, "removeAt range [1]=3");
		CT_ASSERT_EQ(v[2], 4, "removeAt range [2]=4");
	}

	// remove
	{
		Vector<int> v;
		v.add(5); 
		v.add(10); 
		v.add(15);
		bool removed = v.remove(10);
		CT_ASSERT(removed, "remove found");
		CT_ASSERT_EQ(v.size(), 2, "remove size=2");
		CT_ASSERT(!v.contains(10), "remove removed 10");
	}

	// eraseLast / popBack
	{
		Vector<int> v;
		v.add(1); 
		v.add(2); 
		v.add(3);
		v.removeLast();
		CT_ASSERT_EQ(v.size(), 2, "eraseLast size=2");
		CT_ASSERT_EQ(v[v.size()-1], 2, "eraseLast last=2");

		int popped = v.popBack(0);
		CT_ASSERT_EQ(popped, 2, "popBack=2");
		CT_ASSERT_EQ(v.size(), 1, "popBack size=1");
	}

	// sort
	{
		Vector<int> v;
		v.add(5); 
		v.add(1);
		v.add(3);
		v.add(2);
		v.add(4);
		v.sort();
		for (int i = 0; i < (int)v.size() - 1; ++i)
		{
			CT_ASSERT(v[i] <= v[i+1], "Vector sort ascending");
		}
	}

	// addUnique
	{
		Vector<int> v;
		v.addUnique(1);
		v.addUnique(2);
		v.addUnique(1); // duplicate
		CT_ASSERT_EQ(v.size(), 2, "addUnique no dup");
	}

	// resize / reserve
	{
		Vector<int> v;
		v.reserve(50);
		CT_ASSERT_EQ(v.size(), 0, "reserve size=0");
		v.resize(10);
		FOR(10)
		{
			v[i] = 99;
		}
		CT_ASSERT_EQ(v.size(), 10, "resize size=10");
		CT_ASSERT_EQ(v[5], 99, "resize fill=99");
	}

	// operator== / operator!=
	{
		Vector<int> v1, v2;
		v1.add(1);
		v1.add(2);
		v2.add(1);
		v2.add(2);
		CT_ASSERT(v1 == v2, "Vector == equal");
		v2.add(3);
		CT_ASSERT(v1 != v2, "Vector != not equal");
	}

	// addRange
	{
		Vector<int> src;
		for (int i = 10; i < 15; ++i)
		{
			src.add(i);
		}
		Vector<int> dst;
		dst.add(1);
		dst.addRange(src);
		CT_ASSERT_EQ(dst.size(), 6, "addRange size=6");
		CT_ASSERT_EQ(dst[1], 10, "addRange [1]=10");
		CT_ASSERT_EQ(dst[5], 14, "addRange [5]=14");
	}

	// cloneTo
	{
		Vector<int> src;
		FOR(5)
		{
			src.add(i * i);
		}
		Vector<int> dst;
		src.cloneTo(dst);
		CT_ASSERT(src == dst, "cloneTo equal");
	}

	// swapIndex
	{
		Vector<int> v;
		v.add(10);
		v.add(20); 
		v.add(30);
		v.swapIndex(0, 2);
		CT_ASSERT_EQ(v[0], 30, "swapIndex [0]=30");
		CT_ASSERT_EQ(v[2], 10, "swapIndex [2]=10");
	}

	// eraseAllElement
	{
		Vector<int> v;
		v.add(5); 
		v.add(5); 
		v.add(3); 
		v.add(5);
		int cnt = v.removeAll(5);
		CT_ASSERT_EQ(cnt, 3, "eraseAllElement count=3");
		CT_ASSERT_EQ(v.size(), 1, "eraseAllElement remaining=1");
		CT_ASSERT_EQ(v[0], 3, "eraseAllElement remaining[0]=3");
	}

	// findElementCount
	{
		Vector<int> v;
		v.add(1); 
		v.add(2); 
		v.add(1); 
		v.add(1);
		CT_ASSERT_EQ(v.findCount(1), 3, "findElementCount 1=3");
		CT_ASSERT_EQ(v.findCount(2), 1, "findElementCount 2=1");
		CT_ASSERT_EQ(v.findCount(9), 0, "findElementCount 9=0");
	}

	// insert
	{
		Vector<int> v;
		v.add(1); 
		v.add(3);
		v.insert(1, 2);
		CT_ASSERT_EQ(v.size(), 3, "insert size=3");
		CT_ASSERT_EQ(v[1], 2, "insert [1]=2");
		CT_ASSERT_EQ(v[2], 3, "insert [2]=3");
	}

	// isAll (value equality)
	{
		Vector<int> v;
		v.add(2); 
		v.add(2); 
		v.add(2);
		CT_ASSERT(v.isAll(2), "isAll 2 true");
		v.add(3);
		CT_ASSERT(!v.isAll(2), "isAll 2 false after 3");
	}

	// string vector
	{
		Vector<string> sv;
		sv.add("apple"); 
		sv.add("banana"); 
		sv.add("cherry");
		CT_ASSERT_EQ(sv.size(), 3, "string vector size=3");
		CT_ASSERT(sv.contains("banana"), "string vector contains banana");
		sv.remove("banana");
		CT_ASSERT(!sv.contains("banana"), "string vector removed banana");
	}


//------------------------------------------------------------------------------
// HashMap<K,V> — 扩展测试
//------------------------------------------------------------------------------

	// insertOrUpdate
	{
		HashMap<int, string> map;
		map.add(1, "one");
		map.addOrSet(1, "ONE");
		CT_ASSERT_STR(map.get(1), "ONE", "insertOrUpdate overwrite");
		map.addOrSet(2, "two");
		CT_ASSERT_EQ(map.size(), 2, "insertOrUpdate new key");
	}

	// addOrGet
	{
		HashMap<string, int> map;
		int& v1 = map.addOrGet("key", 42);
		CT_ASSERT_EQ(v1, 42, "addOrGet new=42");
		int& v2 = map.addOrGet("key", 99);
		CT_ASSERT_EQ(v2, 42, "addOrGet existing=42 (not overwritten)");
	}

	// get（返回引用语义：找到返回值引用，找不到返回默认值）
	{
		HashMap<int, int> map;
		map.add(5, 50);
		CT_ASSERT_EQ(map.get(5, 0), 50, "get found=50");
		CT_ASSERT_EQ(map.get(99, -1), -1, "get not found=-1");
	}

	// erase by key
	{
		HashMap<int, int> map;
		map.add(1, 10);
		map.add(2, 20); 
		map.add(3, 30);
		map.remove(2);
		CT_ASSERT(!map.contains(2), "erase by key");
		CT_ASSERT_EQ(map.size(), 2, "erase size=2");
	}

	// getPtr / getPtrConst
	{
		HashMap<int, int> map;
		map.add(1, 100);
		int* ptr = map.getPtr(1);
		CT_ASSERT(ptr != nullptr, "getPtr found");
		CT_ASSERT_EQ(*ptr, 100, "getPtr value=100");
		CT_ASSERT(map.getPtr(99) == nullptr, "getPtr not found=null");
	}

	// operator[]
	{
		HashMap<string, int> map;
		map["a"] = 1;
		map["b"] = 2;
		CT_ASSERT_EQ(map["a"], 1, "op[] a=1");
		CT_ASSERT_EQ(map["b"], 2, "op[] b=2");
		map["a"] = 10;
		CT_ASSERT_EQ(map["a"], 10, "op[] overwrite a=10");
	}

	// iterator
	{
		HashMap<int, int> map;
		FOR(5)
		{
			map.add(i, i * 10);
		}
		int sum = 0;
		for (auto& pair : map) { sum += pair.second; }
		CT_ASSERT_EQ(sum, 100, "HashMap iterator sum=100");
	}

	// merge
	{
		HashMap<int, int> src, dst;
		src.add(1, 10); 
		src.add(2, 20);
		dst.add(3, 30);
		dst.addRange(src);
		CT_ASSERT_EQ(dst.size(), 3, "merge size=3");
		CT_ASSERT(dst.contains(1), "merge contains key 1");
		CT_ASSERT(dst.contains(3), "merge contains key 3");
	}

	// cloneTo
	{
		HashMap<int, string> src, dst;
		src.add(1, "a");
		src.add(2, "b");
		src.cloneTo(dst);
		CT_ASSERT_EQ(dst.size(), 2, "HashMap cloneTo size=2");
		CT_ASSERT_STR(dst.get(1), "a", "HashMap cloneTo key1='a'");
	}

	// large volume
	{
		HashMap<int, int> map;
		for (int i = 0; i < 500; ++i)
		{
			map.add(i, i);
		}
		CT_ASSERT_EQ(map.size(), 500, "HashMap large size=500");
		CT_ASSERT_EQ(map.get(250), 250, "HashMap large get[250]");
		for (int i = 0; i < 250; ++i)
		{
			map.remove(i);
		}
		CT_ASSERT_EQ(map.size(), 250, "HashMap after erase 250 size=250");
	}


//------------------------------------------------------------------------------
// Array<N,T> — 扩展测试
//------------------------------------------------------------------------------

	// default (zero) initialization
	{
		Array<10, int> arr;
		memset(arr.data(), 0, sizeof(int) * 10);
		FOR(10)
		{
			CT_ASSERT_EQ(arr[i], 0, "Array zero init");
		}
	}

	// fill
	{
		Array<5, float> arr;
		FOR(5)
		{
			arr[i] = 3.14f;
		}
		FOR(5)
		{
			CT_ASSERT(fabsf(arr[i] - 3.14f) < 1e-5f, "Array fill 3.14");
		}
	}

	// size
	{
		Array<20, byte> arr;
		CT_ASSERT_EQ((int)arr.size(), 20, "Array size=20");
	}

	// direct assignment
	{
		Array<3, string> arr;
		arr[0] = "hello";
		arr[1] = "world";
		arr[2] = "!";
		CT_ASSERT_STR(arr[0], "hello", "Array string[0]");
		CT_ASSERT_STR(arr[2], "!", "Array string[2]");
	}


//------------------------------------------------------------------------------
// MyString<N> — 扩展测试
//------------------------------------------------------------------------------

	// set / str()
	{
		MyString<64> ms;
		ms.set("hello");
		CT_ASSERT_STR(ms.str(), "hello", "MyString set");
		CT_ASSERT_EQ(ms.length(), 5, "MyString length=5");
	}

	// append (simulate via set)
	{
		MyString<128> ms;
		ms.set("hello world");
		CT_ASSERT_STR(ms.str(), "hello world", "MyString set combined");
	}

	// clear
	{
		MyString<32> ms;
		ms.set("test");
		ms.clear();
		CT_ASSERT_EQ(ms.length(), 0, "MyString clear length=0");
	}

	// operator== (use strcmp)
	{
		MyString<32> ms1, ms2;
		ms1.set("abc");
		ms2.set("abc");
		CT_ASSERT(strcmp(ms1.str(), ms2.str()) == 0, "MyString == via strcmp");
		ms2.set("xyz");
		CT_ASSERT(strcmp(ms1.str(), ms2.str()) != 0, "MyString != via strcmp");
	}

	// capacity check (no overflow)
	{
		MyString<8> ms;
		ms.set("1234567"); // 7 chars + null = 8, fits
		CT_ASSERT_EQ(ms.length(), 7, "MyString capacity 7 chars");
	}
//------------------------------------------------------------------------------
// StreamBuffer — 扩展测试
//------------------------------------------------------------------------------

	// capacity boundary
	{
		StreamBuffer buf(10);
		const char data[] = { 1,2,3,4,5,6,7,8,9,10 };
		CT_ASSERT(buf.addDataToBack(data, 10), "StreamBuffer add 10 ok");
		CT_ASSERT_EQ(buf.getDataLength(), 10, "StreamBuffer full length=10");
		CT_ASSERT(!buf.isAvailable(1), "StreamBuffer full not available");
	}

	// multiple add/remove cycles
	{
		StreamBuffer buf(64);
		FOR(5)
		{
			const char data[] = {(char)i, (char)(i + 1)};
			CT_ASSERT(buf.addDataToBack(data, 2), "StreamBuffer cycle add");
			buf.removeDataFromFront(1);
		}
		CT_ASSERT_EQ(buf.getDataLength(), 5, "StreamBuffer cycles length=5");
	}

	// addEmptyToBack
	{
		StreamBuffer buf(32);
		CT_ASSERT(buf.addEmptyToBack(16), "StreamBuffer addEmpty 16");
		CT_ASSERT_EQ(buf.getDataLength(), 16, "StreamBuffer addEmpty length=16");
		CT_ASSERT(buf.isAvailable(0), "StreamBuffer isAvailable(0)");
	}

	// clear and reuse
	{
		StreamBuffer buf(32);
		const char data[] = {1,2,3};
		buf.addDataToBack(data, 3);
		buf.clear();
		CT_ASSERT_EQ(buf.getDataLength(), 0, "StreamBuffer clear=0");
		buf.addDataToBack(data, 3);
		CT_ASSERT_EQ(buf.getDataLength(), 3, "StreamBuffer reuse=3");
	}


}
