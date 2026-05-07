#include "FrameHeader.h"

#define STL_ET_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("STLExtTest FAIL: ") + (msg)); } } while(0)
#define SET_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("STLExtTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define CT_ASSERT_EQ_STL(a, b, msg) SET_ASSERT_EQ(a, b, msg)
#define CT_ASSERT_STR_STL(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("STLExtTest FAIL: ") + (msg)); } } while(0)

//------------------------------------------------------------------------------
// ArrayList<Length, T>
//------------------------------------------------------------------------------
static void testArrayList()
{
	// default: empty
	ArrayList<16, int> list;
	STL_ET_ASSERT(list.isEmpty(), "ArrayList default isEmpty");
	SET_ASSERT_EQ(list.size(), 0, "ArrayList default size=0");
	SET_ASSERT_EQ(list.maxSize(), 16, "ArrayList maxSize=16");

	// add single element
	STL_ET_ASSERT(list.add(10), "ArrayList add 10");
	STL_ET_ASSERT(list.add(20), "ArrayList add 20");
	STL_ET_ASSERT(list.add(30), "ArrayList add 30");
	SET_ASSERT_EQ(list.size(), 3, "ArrayList size=3");
	STL_ET_ASSERT(!list.isEmpty(), "ArrayList not isEmpty");
	SET_ASSERT_EQ(list[0], 10, "ArrayList[0]=10");
	SET_ASSERT_EQ(list[1], 20, "ArrayList[1]=20");
	SET_ASSERT_EQ(list[2], 30, "ArrayList[2]=30");

	// get (bounds-safe)
	SET_ASSERT_EQ(list.get(1), 20, "ArrayList get(1)=20");
	SET_ASSERT_EQ(list.get(-1), 0, "ArrayList get(-1)=default");
	SET_ASSERT_EQ(list.get(100), 0, "ArrayList get(100)=default");

	// contains
	STL_ET_ASSERT(list.contains(20),  "ArrayList contains 20");
	STL_ET_ASSERT(!list.contains(99), "ArrayList not contains 99");

	// eraseFirstElement
	STL_ET_ASSERT(list.remove(20), "ArrayList eraseFirst 20");
	SET_ASSERT_EQ(list.size(), 2, "ArrayList size=2 after eraseFirst");
	STL_ET_ASSERT(!list.contains(20), "ArrayList 20 gone");
	SET_ASSERT_EQ(list[0], 10, "ArrayList[0]=10 after erase");
	SET_ASSERT_EQ(list[1], 30, "ArrayList[1]=30 after erase");

	// eraseLastElement
	list.add(10); // now [10, 30, 10]
	STL_ET_ASSERT(list.removeLast(10), "ArrayList eraseLast 10");
	SET_ASSERT_EQ(list.size(), 2, "ArrayList size=2 after eraseLast");
	SET_ASSERT_EQ(list[0], 10, "ArrayList[0]=10 after eraseLast");
	SET_ASSERT_EQ(list[1], 30, "ArrayList[1]=30 after eraseLast");

	// addUnique
	STL_ET_ASSERT(list.addUnique(99), "ArrayList addUnique 99");
	STL_ET_ASSERT(!list.addUnique(99), "ArrayList addUnique 99 dup fails");
	SET_ASSERT_EQ(list.size(), 3, "ArrayList size=3 after addUnique");

	// remove (remove all occurrences)
	list.add(30);    // now [10, 30, 99, 30]
	list.removeAll(30);
	STL_ET_ASSERT(!list.contains(30), "ArrayList remove removes all 30");
	SET_ASSERT_EQ(list.size(), 2, "ArrayList size=2 after remove(30)");

	// removeAt
	list.add(100);
	list.add(200); // now [10, 99, 100, 200]
	list.removeAt(0);
	SET_ASSERT_EQ(list[0], 99, "ArrayList removeAt(0) shifts");
	SET_ASSERT_EQ(list.size(), 3, "ArrayList size=3 after removeAt");

	// addRange from raw array
	const int src[] = { 1, 2, 3 };
	STL_ET_ASSERT(list.addRange(src, 3), "ArrayList addRange ok");
	SET_ASSERT_EQ(list.size(), 6, "ArrayList size=6 after addRange");
	SET_ASSERT_EQ(list[3], 1, "ArrayList[3]=1 from addRange");
	SET_ASSERT_EQ(list[5], 3, "ArrayList[5]=3 from addRange");

	// setRange (clears first)
	const int src2[] = { 5, 6, 7 };
	STL_ET_ASSERT(list.setRange(src2, 3), "ArrayList setRange ok");
	SET_ASSERT_EQ(list.size(), 3, "ArrayList size=3 after setRange");
	SET_ASSERT_EQ(list[0], 5, "ArrayList[0]=5 after setRange");
	SET_ASSERT_EQ(list[2], 7, "ArrayList[2]=7 after setRange");

	// clear
	list.clear();
	STL_ET_ASSERT(list.isEmpty(), "ArrayList clear -> isEmpty");
	SET_ASSERT_EQ(list.size(), 0, "ArrayList size=0 after clear");

	// capacity limit enforcement
	ArrayList<4, int> capList;
	STL_ET_ASSERT(capList.add(1),  "small add 1");
	STL_ET_ASSERT(capList.add(2),  "small add 2");
	STL_ET_ASSERT(capList.add(3),  "small add 3");
	STL_ET_ASSERT(capList.add(4),  "small add 4");
	STL_ET_ASSERT(!capList.add(5), "small add 5 overflow fails");
	SET_ASSERT_EQ(capList.size(), 4, "small size=4 at capacity");

	// addNotEqual
	ArrayList<8, int> notEqList;
	STL_ET_ASSERT(notEqList.addNotEqual(10, 99), "addNotEqual 10 != 99 ok");
	STL_ET_ASSERT(!notEqList.addNotEqual(99, 99), "addNotEqual 99 == 99 skip");
	SET_ASSERT_EQ(notEqList.size(), 1, "addNotEqual only 1 added");

	// add two elements
	ArrayList<8, int> pairList;
	STL_ET_ASSERT(pairList.add(1, 2), "add(1,2) ok");
	SET_ASSERT_EQ(pairList.size(), 2, "size=2 after add(1,2)");
	SET_ASSERT_EQ(pairList[0], 1, "pair[0]=1");
	SET_ASSERT_EQ(pairList[1], 2, "pair[1]=2");

	// add three elements
	ArrayList<8, int> triList;
	STL_ET_ASSERT(triList.add(10, 20, 30), "add(10,20,30) ok");
	SET_ASSERT_EQ(triList.size(), 3, "size=3 after add(10,20,30)");

	// isAll
	ArrayList<4, int> uniform;
	uniform.add(7);
	uniform.add(7);
	uniform.add(7);
	STL_ET_ASSERT(uniform.isAll(7), "isAll(7) true");
	STL_ET_ASSERT(!uniform.isAll(8), "isAll(8) false");

	// setAll
	ArrayList<4, int> fillList;
	fillList.setAll(42);
	FOR(4)
	{
		SET_ASSERT_EQ(fillList.mValue[i], 42, "fillArray[i]=42");
	}
}

//------------------------------------------------------------------------------
// Map<Key, Value>
//------------------------------------------------------------------------------
static void testMap()
{
	// default: empty
	Map<int, string> m;
	STL_ET_ASSERT(m.isEmpty(), "Map default isEmpty");
	SET_ASSERT_EQ(m.size(), 0, "Map default size=0");

	// insert via [] operator
	m[1] = "one";
	m[2] = "two";
	m[3] = "three";
	SET_ASSERT_EQ(m.size(), 3, "Map size=3");
	STL_ET_ASSERT(!m.isEmpty(), "Map not isEmpty");

	// contains
	STL_ET_ASSERT(m.contains(1), "Map contains 1");
	STL_ET_ASSERT(m.contains(2), "Map contains 2");
	STL_ET_ASSERT(!m.contains(99), "Map not contains 99");

	// get via get
	STL_ET_ASSERT(m.get(1) == "one", "Map get(1)=one");
	STL_ET_ASSERT(m.get(2) == "two", "Map get(2)=two");

	// getPtr
	string* ptr = m.getPtr(2);
	STL_ET_ASSERT(ptr != nullptr, "Map getPtr(2) not null");
	STL_ET_ASSERT(*ptr == "two", "Map *getPtr(2)=two");
	string* noptr = m.getPtr(99);
	STL_ET_ASSERT(noptr == nullptr, "Map getPtr(99) null");

	// get with out via getPtr
	string* vptr = m.getPtr(3);
	STL_ET_ASSERT(vptr != nullptr, "Map getPtr(3) not null");
	STL_ET_ASSERT(*vptr == "three", "Map *getPtr(3)=three");
	STL_ET_ASSERT(m.getPtr(99) == nullptr, "Map getPtr(99) null");

	// getAtIndex
	const string& first = m.getAt(0, "");
	STL_ET_ASSERT(!first.empty(), "Map getAtIndex(0) not empty");

	// erase
	STL_ET_ASSERT(m.remove(2), "Map erase(2) ok");
	SET_ASSERT_EQ(m.size(), 2, "Map size=2 after erase");
	STL_ET_ASSERT(!m.contains(2), "Map 2 gone after erase");
	STL_ET_ASSERT(!m.remove(99), "Map erase(99) false");

	// iteration
	Map<int, int> numMap;
	numMap[10] = 100;
	numMap[20] = 200;
	numMap[30] = 300;
	int sum = 0;
	for (const auto& kv : numMap)
	{
		sum += kv.second;
	}
	SET_ASSERT_EQ(sum, 600, "Map iterate sum=600");

	// clear
	m.clear();
	STL_ET_ASSERT(m.isEmpty(), "Map clear -> isEmpty");
	SET_ASSERT_EQ(m.size(), 0, "Map size=0 after clear");

	// addOrSet: key exists -> update
	Map<int, int> iouMap;
	iouMap[5] = 50;
	iouMap.addOrSet(5, 55);
	SET_ASSERT_EQ(iouMap.get(5), 55, "Map addOrSet update");

	// addOrSet: key missing -> insert
	iouMap.addOrSet(6, 60);
	SET_ASSERT_EQ(iouMap.get(6), 60, "Map addOrSet insert");
	SET_ASSERT_EQ(iouMap.size(), 2, "Map size=2 after addOrSet");

	// initializer_list constructor
	Map<int, int> initMap({ {1, 11}, {2, 22}, {3, 33} });
	SET_ASSERT_EQ(initMap.size(), 3, "Map init list size=3");
	SET_ASSERT_EQ(initMap.get(2), 22, "Map init list get(2)=22");
}

//------------------------------------------------------------------------------
// Set<T>
//------------------------------------------------------------------------------
static void testSet()
{
	// default: empty
	Set<int> s;
	STL_ET_ASSERT(s.isEmpty(), "Set default isEmpty");
	SET_ASSERT_EQ(s.size(), 0, "Set default size=0");

	// insert
	STL_ET_ASSERT(s.add(10), "Set insert 10");
	STL_ET_ASSERT(s.add(20), "Set insert 20");
	STL_ET_ASSERT(s.add(30), "Set insert 30");
	SET_ASSERT_EQ(s.size(), 3, "Set size=3");
	STL_ET_ASSERT(!s.isEmpty(), "Set not isEmpty");

	// duplicate insert returns false
	STL_ET_ASSERT(!s.add(10), "Set insert dup 10 fails");
	SET_ASSERT_EQ(s.size(), 3, "Set size still 3 after dup");

	// contains
	STL_ET_ASSERT(s.contains(20),  "Set contains 20");
	STL_ET_ASSERT(!s.contains(99), "Set not contains 99");

	// erase by value
	STL_ET_ASSERT(s.remove(20), "Set erase(20) ok");
	SET_ASSERT_EQ(s.size(), 2, "Set size=2 after erase");
	STL_ET_ASSERT(!s.contains(20), "Set 20 gone");
	STL_ET_ASSERT(!s.remove(99), "Set erase(99) false");

	// iteration: sum of elements
	Set<int> sumSet;
	sumSet.add(1);
	sumSet.add(2);
	sumSet.add(3);
	int total = 0;
	for (const int v : sumSet)
	{
		total += v;
	}
	SET_ASSERT_EQ(total, 6, "Set iterate sum=6");

	// insert from Vector
	Vector<int> vec;
	vec.add(100);
	vec.add(200);
	vec.add(100); // duplicate
	Set<int> fromVec;
	fromVec.addRange(vec);
	SET_ASSERT_EQ(fromVec.size(), 2, "Set insert(vec) dedup size=2");
	STL_ET_ASSERT(fromVec.contains(100), "Set from vec has 100");
	STL_ET_ASSERT(fromVec.contains(200), "Set from vec has 200");

	// setData from raw array
	int arr[] = { 5, 6, 7, 5 };
	Set<int> fromArr;
	fromArr.setData(arr, 4);
	SET_ASSERT_EQ(fromArr.size(), 3, "Set setData dedup size=3");

	// setData from Vector
	Vector<int> dataVec;
	dataVec.add(11);
	dataVec.add(22);
	dataVec.add(11);
	Set<int> fromVecData;
	fromVecData.setData(dataVec);
	SET_ASSERT_EQ(fromVecData.size(), 2, "Set setData(vec) dedup size=2");

	// insert from another Set
	Set<int> src;
	src.add(50);
	src.add(60);
	Set<int> dst;
	dst.addRange(src);
	SET_ASSERT_EQ(dst.size(), 2, "Set insert(Set) size=2");

	// popFirst
	Set<int> popSet;
	popSet.add(42);
	int popped = popSet.popFirst();
	SET_ASSERT_EQ(popped, 42, "Set pop_front=42");
	STL_ET_ASSERT(popSet.isEmpty(), "Set empty after pop_front");

	// clear
	s.clear();
	STL_ET_ASSERT(s.isEmpty(), "Set clear -> isEmpty");
	SET_ASSERT_EQ(s.size(), 0, "Set size=0 after clear");

	// cloneTo
	Set<int> origin;
	origin.add(7);
	origin.add(8);
	Set<int> copy;
	origin.cloneTo(copy);
	SET_ASSERT_EQ(copy.size(), 2, "Set cloneTo size=2");
	STL_ET_ASSERT(copy.contains(7), "Set cloneTo has 7");
	STL_ET_ASSERT(copy.contains(8), "Set cloneTo has 8");
}

//------------------------------------------------------------------------------
// Stack<T>
//------------------------------------------------------------------------------
static void testStack()
{
	Stack<int> st;
	SET_ASSERT_EQ(st.size(), 0, "Stack default size=0");

	// push
	st.push(1);
	st.push(2);
	st.push(3);
	SET_ASSERT_EQ(st.size(), 3, "Stack size=3 after push 3");

	// top
	SET_ASSERT_EQ(st.top(), 3, "Stack top=3 (LIFO)");

	// pop (LIFO order)
	const int a = st.pop();
	SET_ASSERT_EQ(a, 3, "Stack pop=3");
	SET_ASSERT_EQ(st.size(), 2, "Stack size=2");

	const int b = st.pop();
	SET_ASSERT_EQ(b, 2, "Stack pop=2");

	const int c = st.pop();
	SET_ASSERT_EQ(c, 1, "Stack pop=1");
	SET_ASSERT_EQ(st.size(), 0, "Stack empty after 3 pops");

	// multiple types: string stack
	Stack<string> strSt;
	strSt.push("hello");
	strSt.push("world");
	SET_ASSERT_EQ(strSt.size(), 2, "StrStack size=2");
	STL_ET_ASSERT(strSt.top() == "world", "StrStack top=world");
	const string top = strSt.pop();
	STL_ET_ASSERT(top == "world", "StrStack pop=world");
	STL_ET_ASSERT(strSt.top() == "hello", "StrStack top=hello after pop");

	// stress: push 100, pop 100 in LIFO order
	Stack<int> stress;
	FOR(100)
	{
		stress.push(i);
	}
	SET_ASSERT_EQ(stress.size(), 100, "Stress stack size=100");
	FOR_INVERSE(100)
	{
		const int val = stress.pop();
		SET_ASSERT_EQ(val, i, "Stress stack LIFO order");
	}
	SET_ASSERT_EQ(stress.size(), 0, "Stress stack empty");
}

//------------------------------------------------------------------------------
// Map (advanced: key ordering, reverse iterator, merge-like)
//------------------------------------------------------------------------------
static void testMapOrdering()
{
	Map<int, int> m;
	m[30] = 3;
	m[10] = 1;
	m[20] = 2;

	// std::map is ordered by key ascending
	int prevKey = -1;
	for (const auto& kv : m)
	{
		STL_ET_ASSERT(kv.first > prevKey, "Map keys ascending");
		prevKey = kv.first;
	}

	// reverse iterator: descending
	int prevKeyRev = 9999;
	for (auto it = m.rbegin(); it != m.rend(); ++it)
	{
		STL_ET_ASSERT(it->first < prevKeyRev, "Map rbegin descending");
		prevKeyRev = it->first;
	}

	// trySet: update if exists
	STL_ET_ASSERT(m.set(10, 99), "Map trySet existing key");
	SET_ASSERT_EQ(m.get(10), 99, "Map trySet updated value");
	STL_ET_ASSERT(!m.set(999, 0), "Map trySet missing key false");

	// mapKeyToVector style via iteration
	Vector<int> keys;
	for (const auto& kv : m)
	{
		keys.add(kv.first);
	}
	SET_ASSERT_EQ(keys.size(), 3, "Map keys vector size=3");
	SET_ASSERT_EQ(keys[0], 10, "Map key[0]=10");
	SET_ASSERT_EQ(keys[1], 20, "Map key[1]=20");
	SET_ASSERT_EQ(keys[2], 30, "Map key[2]=30");
}

//------------------------------------------------------------------------------
// ArrayList advanced: addRange between ArrayLists, zero
//------------------------------------------------------------------------------
static void testArrayListAdvanced()
{
	// addRange from another ArrayList
	ArrayList<8, int> src;
	src.add(1);
	src.add(2);
	src.add(3);

	ArrayList<8, int> dst;
	dst.add(10);
	STL_ET_ASSERT(dst.addRange(src), "ArrayList addRange(ArrayList) ok");
	SET_ASSERT_EQ(dst.size(), 4, "ArrayList size=4 after addRange");
	SET_ASSERT_EQ(dst[1], 1, "ArrayList[1]=1 after addRange");
	SET_ASSERT_EQ(dst[3], 3, "ArrayList[3]=3 after addRange");

	// zero (memset to 0)
	ArrayList<4, int> z;
	z.add(1);
	z.add(2);
	z.zero();
	FOR(4)
	{
		SET_ASSERT_EQ(z.mValue[i], 0, "ArrayList zero mValue[i]=0");
	}
	// zero does NOT reset mSize
	SET_ASSERT_EQ(z.size(), 2, "ArrayList zero mSize unchanged");

	// fillArray with start index
	ArrayList<8, int> fillList;
	fillList.add(100);
	fillList.add(200);
	fillList.setAll(2, 999);
	for (int i = 2; i < 8; ++i)
	{
		SET_ASSERT_EQ(fillList.mValue[i], 999, "ArrayList fillArray(2) mValue[i]=999");
	}
	// first two should be unchanged
	SET_ASSERT_EQ(fillList.mValue[0], 100, "ArrayList fillArray keeps [0]");
	SET_ASSERT_EQ(fillList.mValue[1], 200, "ArrayList fillArray keeps [1]");
}

//------------------------------------------------------------------------------
// STLExtTest entry point
//------------------------------------------------------------------------------
void STLExtTest::test()
{
	testArrayList();
	testArrayListAdvanced();
	testMap();
	testMapOrdering();
	testSet();
	testStack();

//------------------------------------------------------------------------------
// ArrayList<N,T> — 扩展测试
//------------------------------------------------------------------------------

	// add and size tracking
	{
		ArrayList<10, int> list;
		CT_ASSERT_EQ_STL(list.size(), 0, "ArrayList init size=0");
		STL_ET_ASSERT(list.isEmpty(), "ArrayList init empty");
		FOR(200) list.add(i);
		CT_ASSERT_EQ_STL(list.size(), 10, "ArrayList full size=10");
		STL_ET_ASSERT(list.size() == list.maxSize(), "ArrayList isFull");
	}

	// contains
	{
		ArrayList<8, int> list;
		list.add(5); list.add(10); list.add(15);
		STL_ET_ASSERT(list.contains(10), "ArrayList contains 10");
		STL_ET_ASSERT(!list.contains(99), "ArrayList not contains 99");
	}

	// clear
	{
		ArrayList<8, int> list;
		FOR(200) list.add(i);
		list.clear();
		CT_ASSERT_EQ_STL(list.size(), 0, "ArrayList clear size=0");
		STL_ET_ASSERT(list.isEmpty(), "ArrayList clear empty");
	}

	// add at capacity
	{
		ArrayList<3, int> list;
		list.add(1); list.add(2); list.add(3);
		STL_ET_ASSERT(list.size() == list.maxSize(), "ArrayList 3/3 full");
		STL_ET_ASSERT(!list.add(4), "ArrayList overflow add false");
	}

	// erase
	{
		ArrayList<5, int> list;
		list.add(10); list.add(20); list.add(30);
		list.removeAt(1); // remove index 1 (value 20)
		CT_ASSERT_EQ_STL(list.size(), 2, "ArrayList erase size=2");
		CT_ASSERT_EQ_STL(list[0], 10, "ArrayList erase [0]=10");
		CT_ASSERT_EQ_STL(list[1], 30, "ArrayList erase [1]=30");
	}

	// remove
	{
		ArrayList<5, int> list;
		list.add(1); 
		list.add(2); 
		list.add(3);
		list.removeAll(2);
		STL_ET_ASSERT(!list.contains(2), "ArrayList remove removed");
		CT_ASSERT_EQ_STL(list.size(), 2, "ArrayList remove size=2");
	}

	// addRange from ArrayList
	{
		ArrayList<5, int> src, dst;
		src.add(10); src.add(20);
		dst.add(1);
		STL_ET_ASSERT(dst.addRange(src), "ArrayList addRange ok");
		CT_ASSERT_EQ_STL(dst.size(), 3, "ArrayList addRange size=3");
	}


//------------------------------------------------------------------------------
// Map<K,V> — 扩展测试
//------------------------------------------------------------------------------

	// basic operations
	{
		Map<int, string> map;
		map.add(3, "three");
		map.add(1, "one");
		map.add(2, "two");
		CT_ASSERT_EQ_STL((int)map.size(), 3, "Map size=3");

		// Ordered: should iterate 1,2,3
		int prev = -1;
		for (auto& p : map)
		{
			STL_ET_ASSERT(p.first > prev, "Map ordered iteration");
			prev = p.first;
		}
	}

	// erase
	{
		Map<string, int> map;
		map.add("a", 1);
		map.add("b", 2);
		map.add("c", 3);
		map.remove("b");
		STL_ET_ASSERT(!map.contains("b"), "Map erase 'b'");
		CT_ASSERT_EQ_STL((int)map.size(), 2, "Map erase size=2");
	}

	// update
	{
		Map<int, int> map;
		map.add(1, 10);
		map.addOrSet(1, 100);
		int defVal = -1;
		CT_ASSERT_EQ_STL(map.get(1, defVal), 100, "Map addOrSet overwrite");
	}

	// find / contains
	{
		Map<int, string> map;
		map.add(42, "answer");
		STL_ET_ASSERT(map.contains(42), "Map contains 42");
		STL_ET_ASSERT(!map.contains(99), "Map not contains 99");
	}

	// clear
	{
		Map<int, int> map;
		FOR(200)
		{
			map.add(i, i);
		}
		map.clear();
		STL_ET_ASSERT(map.isEmpty(), "Map clear empty");
	}


//------------------------------------------------------------------------------
// Set<T> — 扩展测试
//------------------------------------------------------------------------------

	// insert / contains / size
	{
		Set<int> s;
		s.add(1); 
		s.add(2); 
		s.add(3); 
		s.add(2); // dup
		CT_ASSERT_EQ_STL((int)s.size(), 3, "Set no dup size=3");
		STL_ET_ASSERT(s.contains(2), "Set contains 2");
		STL_ET_ASSERT(!s.contains(99), "Set not contains 99");
	}

	// erase
	{
		Set<string> s;
		s.add("a");
		s.add("b"); 
		s.add("c");
		s.remove("b");
		STL_ET_ASSERT(!s.contains("b"), "Set erase 'b'");
		CT_ASSERT_EQ_STL((int)s.size(), 2, "Set erase size=2");
	}

	// iteration (unordered_set, verify all elements present)
	{
		Set<int> s;
		s.add(5); 
		s.add(3);
		s.add(7);
		s.add(1);
		int sum = 0;
		for (auto val : s)
		{
			sum += val;
		}
		CT_ASSERT_EQ_STL(sum, 16, "Set iteration sum=16");
		CT_ASSERT_EQ_STL(s.size(), 4, "Set iteration size=4");
	}

	// clear
	{
		Set<int> s;
		FOR(20)
		{
			s.add(i);
		}
		s.clear();
		STL_ET_ASSERT(s.isEmpty(), "Set clear empty");
	}

	// large volume
	{
		Set<int> s;
		FOR(100)
		{
			s.add(i);
		}
		CT_ASSERT_EQ_STL((int)s.size(), 100, "Set large size=100");
		FOR(50)
		{
			s.remove(i);
		}
		CT_ASSERT_EQ_STL((int)s.size(), 50, "Set large after erase=50");
	}


//------------------------------------------------------------------------------
// Stack<T> — 扩展测试
//------------------------------------------------------------------------------

	// push / pop / top
	{
		Stack<int> stack;
		STL_ET_ASSERT(stack.size() == 0, "Stack init empty");
		stack.push(10);
		stack.push(20);
		stack.push(30);
		CT_ASSERT_EQ_STL((int)stack.size(), 3, "Stack size=3");
		CT_ASSERT_EQ_STL(stack.top(), 30, "Stack top=30");
		stack.pop();
		CT_ASSERT_EQ_STL(stack.top(), 20, "Stack top after pop=20");
	}

	// LIFO order
	{
		Stack<int> stack;
		for (int i = 1; i <= 5; ++i) stack.push(i);
		for (int i = 5; i >= 1; --i)
		{
			CT_ASSERT_EQ_STL(stack.top(), i, "Stack LIFO pop order");
			stack.pop();
		}
		STL_ET_ASSERT(stack.size() == 0, "Stack empty after all pops");
	}

	// clear (manual)
	{
		Stack<string> stack;
		stack.push("a"); stack.push("b"); stack.push("c");
		while (stack.size() > 0) { stack.pop(); }
		STL_ET_ASSERT(stack.size() == 0, "Stack clear empty");
	}


//------------------------------------------------------------------------------
// Queue<T> — 测试
//------------------------------------------------------------------------------

	// push / pop / front — FIFO order
	{
		Queue<int> q;
		STL_ET_ASSERT(q.isEmpty(), "Queue init empty");
		q.add(1); q.add(2); q.add(3);
		CT_ASSERT_EQ_STL((int)q.size(), 3, "Queue size=3");
		CT_ASSERT_EQ_STL(q.front(), 1, "Queue front=1");
		q.popOnly();
		CT_ASSERT_EQ_STL(q.front(), 2, "Queue front after pop=2");
		q.popOnly(); q.popOnly();
		STL_ET_ASSERT(q.isEmpty(), "Queue empty after all pops");
	}

	// FIFO order verification
	{
		Queue<int> q;
		for (int i = 1; i <= 5; ++i) q.add(i);
		for (int i = 1; i <= 5; ++i)
		{
			CT_ASSERT_EQ_STL(q.front(), i, "Queue FIFO order");
			q.popOnly();
		}
	}

	// string queue
	{
		Queue<string> q;
		q.add("first"); q.add("second"); q.add("third");
		CT_ASSERT_STR_STL(q.front(), "first", "Queue string front");
		q.popOnly();
		CT_ASSERT_STR_STL(q.front(), "second", "Queue string after pop");
	}


}
