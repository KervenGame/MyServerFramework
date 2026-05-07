#include "FrameHeader.h"
 struct FrameUtilityTestImpl
{

#define FrameUtilityTestImpl_FUT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("FrameUtilityTest FAIL: ") + (msg)); } } while(0)
#define FrameUtilityTestImpl_FUT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("FrameUtilityTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define FrameUtilityTestImpl_FUT_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((float)(a) - (float)(b)) > 1e-4f) { ERROR(string("FrameUtilityTest FAIL: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)

//------------------------------------------------------------------------------
// tickTimerOnce
// Returns true once time reaches 0, decrements time by elapsedTime
//------------------------------------------------------------------------------
static void testTickTimerOnce()
{
	// time=1.0, elapsed=0.4 -> time=0.6, not triggered
	float t = 1.0f;
	FrameUtilityTestImpl_FUT_ASSERT(!tickTimerOnce(t, 0.4f), "tickOnce 0.6 not triggered");
	FrameUtilityTestImpl_FUT_ASSERT_FLOAT(t, 0.6f, "tickOnce t=0.6");

	// elapsed=0.4 -> t=0.2, not triggered
	FrameUtilityTestImpl_FUT_ASSERT(!tickTimerOnce(t, 0.4f), "tickOnce 0.2 not triggered");
	FrameUtilityTestImpl_FUT_ASSERT_FLOAT(t, 0.2f, "tickOnce t=0.2");

	// elapsed=0.3 -> t goes <= 0, triggered
	FrameUtilityTestImpl_FUT_ASSERT(tickTimerOnce(t, 0.3f), "tickOnce triggered");

	// once triggered, timer remains <= 0 (no auto-reset)
	FrameUtilityTestImpl_FUT_ASSERT(!tickTimerOnce(t, 1.0f), "tickOnce not triggered again");

	// starting from 0: immediately triggered
	float t2 = 0.0f;
	FrameUtilityTestImpl_FUT_ASSERT(tickTimerOnce(t2, 0.1f), "tickOnce 0 start triggered");
}

//------------------------------------------------------------------------------
// tickTimerLoop
// 倒计时设计：time 从 interval 开始向下减，<= 0 时触发，然后 time += interval
//------------------------------------------------------------------------------
static void testTickTimerLoop()
{
	// interval=1.0，从 1.0 开始倒计时
	float t = 1.0f;

	// elapsed=0.4 -> t=0.6, not triggered
	FrameUtilityTestImpl_FUT_ASSERT(!tickTimerLoop(t, 0.4f, 1.0f), "tickLoop 0.4 not triggered");
	FrameUtilityTestImpl_FUT_ASSERT_FLOAT(t, 0.6f, "tickLoop t=0.6 after 0.4");

	// elapsed=0.4 -> t=0.2, not triggered
	FrameUtilityTestImpl_FUT_ASSERT(!tickTimerLoop(t, 0.4f, 1.0f), "tickLoop 0.8 not triggered");
	FrameUtilityTestImpl_FUT_ASSERT_FLOAT(t, 0.2f, "tickLoop t=0.2 after 0.8");

	// elapsed=0.4 -> t=-0.2 <= 0, triggered, t += 1.0 = 0.8
	FrameUtilityTestImpl_FUT_ASSERT(tickTimerLoop(t, 0.4f, 1.0f), "tickLoop triggered at 1.2");
	FrameUtilityTestImpl_FUT_ASSERT_FLOAT(t, 0.8f, "tickLoop t=0.8 after trigger");

	// elapsed=0.5 -> t=0.3, not triggered
	FrameUtilityTestImpl_FUT_ASSERT(!tickTimerLoop(t, 0.5f, 1.0f), "tickLoop 0.3 not triggered");

	// elapsed=0.4 -> t=-0.1 <= 0, triggered, t += 1.0 = 0.9
	FrameUtilityTestImpl_FUT_ASSERT(tickTimerLoop(t, 0.4f, 1.0f), "tickLoop triggered 2nd");
	FrameUtilityTestImpl_FUT_ASSERT_FLOAT(t, 0.9f, "tickLoop t=0.9 after 2nd trigger");
}

//------------------------------------------------------------------------------
// tickTimerLoopEnsure
// 与 tickTimerLoop 类似，但不做 += interval，触发后直接 time = interval（重置满）
//------------------------------------------------------------------------------
static void testTickTimerLoopEnsure()
{
	float t = 1.0f;
	// elapsed=0.3 -> t=0.7, not triggered
	FrameUtilityTestImpl_FUT_ASSERT(!tickTimerLoopEnsure(t, 0.3f, 1.0f), "tickLoopEnsure 0.3 not triggered");
	// elapsed=0.5 -> t=0.2, not triggered
	FrameUtilityTestImpl_FUT_ASSERT(!tickTimerLoopEnsure(t, 0.5f, 1.0f), "tickLoopEnsure 0.8 not triggered");
	// elapsed=0.3 -> t=-0.1 <= 0, triggered, t = 1.0 (重置满)
	FrameUtilityTestImpl_FUT_ASSERT(tickTimerLoopEnsure(t, 0.3f, 1.0f), "tickLoopEnsure triggered at 1.1");

	// after trigger, t = interval = 1.0
	FrameUtilityTestImpl_FUT_ASSERT_FLOAT(t, 1.0f, "tickLoopEnsure t=1.0 after trigger");
}

//------------------------------------------------------------------------------
// setToVector: Set<T> -> Vector<T>
//------------------------------------------------------------------------------
static void testSetToVector()
{
	Set<int> s;
	s.add(10);
	s.add(20);
	s.add(30);

	Vector<int> v;
	setToVector(s, v);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v.size(), 3, "setToVector size=3");
	// verify all elements present (order may vary since Set uses hash)
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(10), "setToVector has 10");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(20), "setToVector has 20");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(30), "setToVector has 30");

	// version returning value
	Vector<int> v2 = setToVector(s);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v2.size(), 3, "setToVector (return) size=3");

	// empty set -> empty vector
	Set<int> empty;
	Vector<int> emptyV;
	setToVector(empty, emptyV);
	FrameUtilityTestImpl_FUT_ASSERT(emptyV.isEmpty(), "setToVector empty -> isEmpty");
}

//------------------------------------------------------------------------------
// mapValueToVector: Map<Key,Value> -> Vector<Value>
//------------------------------------------------------------------------------
static void testMapValueToVector2()
{
	Map<int, int> m;
	m[1] = 100;
	m[2] = 200;
	m[3] = 300;

	Vector<int> v;
	mapValueToVector(m, v);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v.size(), 3, "mapValueToVector size=3");
	// Map is sorted by key, so values come out in order
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v[0], 100, "mapValueToVector v[0]=100");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v[1], 200, "mapValueToVector v[1]=200");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v[2], 300, "mapValueToVector v[2]=300");

	// empty map -> no change
	Map<int, int> emptyM;
	Vector<int> emptyV;
	mapValueToVector(emptyM, emptyV);
	FrameUtilityTestImpl_FUT_ASSERT(emptyV.isEmpty(), "mapValueToVector empty -> isEmpty");
}

//------------------------------------------------------------------------------
// hashMapValueToVector: HashMap<Key,Value> -> Vector<Value>
//------------------------------------------------------------------------------
static void testHashMapValueToVector()
{
	HashMap<int, int> hm;
	hm.add(1, 100);
	hm.add(2, 200);
	hm.add(3, 300);

	Vector<int> v;
	hashMapValueToVector(hm, v);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v.size(), 3, "hashMapValueToVector size=3");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(100), "hashMapValueToVector has 100");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(200), "hashMapValueToVector has 200");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(300), "hashMapValueToVector has 300");
}

//------------------------------------------------------------------------------
// mapKeyToVector: Map<Key,Value> -> Vector<Key>
//------------------------------------------------------------------------------
static void testMapKeyToVector2()
{
	Map<int, string> m;
	m[5] = "five";
	m[3] = "three";
	m[8] = "eight";

	Vector<int> keys;
	mapKeyToVector(m, keys);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(keys.size(), 3, "mapKeyToVector size=3");
	// Map is sorted, keys come out ascending
	FrameUtilityTestImpl_FUT_ASSERT_EQ(keys[0], 3, "mapKeyToVector keys[0]=3");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(keys[1], 5, "mapKeyToVector keys[1]=5");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(keys[2], 8, "mapKeyToVector keys[2]=8");

	// empty map
	Map<int, string> emptyM;
	Vector<int> emptyKeys;
	mapKeyToVector(emptyM, emptyKeys);
	FrameUtilityTestImpl_FUT_ASSERT(emptyKeys.isEmpty(), "mapKeyToVector empty -> isEmpty");
}

//------------------------------------------------------------------------------
// hashMapKeyToVector: HashMap<Key,Value> -> Vector<Key>
//------------------------------------------------------------------------------
static void testHashMapKeyToVector2()
{
	HashMap<int, int> hm;
	hm.add(10, 1);
	hm.add(20, 2);
	hm.add(30, 3);

	Vector<int> keys;
	hashMapKeyToVector(hm, keys);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(keys.size(), 3, "hashMapKeyToVector size=3");
	FrameUtilityTestImpl_FUT_ASSERT(keys.contains(10), "hashMapKeyToVector has 10");
	FrameUtilityTestImpl_FUT_ASSERT(keys.contains(20), "hashMapKeyToVector has 20");
	FrameUtilityTestImpl_FUT_ASSERT(keys.contains(30), "hashMapKeyToVector has 30");

	// return-value version
	Vector<int> keys2 = hashMapKeyToVector(hm);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(keys2.size(), 3, "hashMapKeyToVector (ret) size=3");
}

//------------------------------------------------------------------------------
// mapValueToArray: Map -> Array
//------------------------------------------------------------------------------
static void testMapValueToArray()
{
	Map<int, int> m;
	m[1] = 10;
	m[2] = 20;
	m[3] = 30;
	m[4] = 40;

	Array<3, int> arr;
	arr.zero();
	const int count = mapValueToArray(m, arr, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count, 3, "mapValueToArray count=3");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(arr[0], 10, "mapValueToArray arr[0]=10");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(arr[1], 20, "mapValueToArray arr[1]=20");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(arr[2], 30, "mapValueToArray arr[2]=30");

	// start from index 1 -> skip first element
	Array<3, int> arr2;
	arr2.zero();
	const int count2 = mapValueToArray(m, arr2, 1);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count2, 3, "mapValueToArray startIndex=1 count=3");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(arr2[0], 20, "mapValueToArray si=1 arr[0]=20");

	// empty map -> count=0
	Map<int, int> emptyM;
	Array<3, int> emptyArr;
	emptyArr.zero();
	const int emptyCount = mapValueToArray(emptyM, emptyArr, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(emptyCount, 0, "mapValueToArray empty count=0");

	// startIndex >= map.size() -> count=0
	const int overCount = mapValueToArray(m, arr, 10);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(overCount, 0, "mapValueToArray over startIndex count=0");
}

//------------------------------------------------------------------------------
// vectorToSet: Vector<T> -> Set<T>
//------------------------------------------------------------------------------
static void testVectorToSet()
{
	Vector<int> v;
	v.add(1);
	v.add(2);
	v.add(1); // duplicate
	v.add(3);

	Set<int> s;
	vectorToSet(v, s, false);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(s.size(), 3, "vectorToSet dedup size=3");
	FrameUtilityTestImpl_FUT_ASSERT(s.contains(1), "vectorToSet has 1");
	FrameUtilityTestImpl_FUT_ASSERT(s.contains(2), "vectorToSet has 2");
	FrameUtilityTestImpl_FUT_ASSERT(s.contains(3), "vectorToSet has 3");

	// append mode: true preserves existing elements
	s.add(100); // pre-existing
	Vector<int> v2;
	v2.add(200);
	v2.add(300);
	vectorToSet(v2, s, true);
	FrameUtilityTestImpl_FUT_ASSERT(s.contains(100), "vectorToSet append keeps 100");
	FrameUtilityTestImpl_FUT_ASSERT(s.contains(200), "vectorToSet append adds 200");
	FrameUtilityTestImpl_FUT_ASSERT(s.contains(300), "vectorToSet append adds 300");

	// clear mode: false replaces
	vectorToSet(v2, s, false);
	FrameUtilityTestImpl_FUT_ASSERT(!s.contains(100), "vectorToSet clear removes 100");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(s.size(), 2, "vectorToSet clear size=2");
}

//------------------------------------------------------------------------------
// removeDumplicate (de-duplicate Vector<T>)
//------------------------------------------------------------------------------
static void testRemoveDuplicate()
{
	Vector<int> v;
	v.add(1);
	v.add(2);
	v.add(1);
	v.add(3);
	v.add(2);

	removeDumplicate(v);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v.size(), 3, "removeDuplicate size=3");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(1), "removeDuplicate has 1");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(2), "removeDuplicate has 2");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(3), "removeDuplicate has 3");

	// no duplicates: unchanged
	Vector<int> v2;
	v2.add(10);
	v2.add(20);
	removeDumplicate(v2);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v2.size(), 2, "removeDuplicate no-dup size=2");

	// empty vector: no crash
	Vector<int> empty;
	removeDumplicate(empty);
	FrameUtilityTestImpl_FUT_ASSERT(empty.isEmpty(), "removeDuplicate empty ok");

	// single element: no crash
	Vector<int> single;
	single.add(42);
	removeDumplicate(single);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(single.size(), 1, "removeDuplicate single size=1");
}

//------------------------------------------------------------------------------
// getListDiffPod: compute diff between two POD arrays
//------------------------------------------------------------------------------
static void testGetListDiffPod()
{
	// array0 = [1,2,3,4], array1 = [2,3,5]
	// diff0 (in 0 not in 1) = [1,4], diff1 (in 1 not in 0) = [5]
	const int arr0[] = { 1, 2, 3, 4 };
	const int arr1[] = { 2, 3, 5 };

	Vector<int> diff0, diff1;
	getListDiffPod(arr0, 4, arr1, 3, diff0, diff1);

	FrameUtilityTestImpl_FUT_ASSERT_EQ(diff0.size(), 2, "getListDiffPod diff0 size=2");
	FrameUtilityTestImpl_FUT_ASSERT(diff0.contains(1), "getListDiffPod diff0 has 1");
	FrameUtilityTestImpl_FUT_ASSERT(diff0.contains(4), "getListDiffPod diff0 has 4");

	FrameUtilityTestImpl_FUT_ASSERT_EQ(diff1.size(), 1, "getListDiffPod diff1 size=1");
	FrameUtilityTestImpl_FUT_ASSERT(diff1.contains(5), "getListDiffPod diff1 has 5");

	// identical arrays -> both diffs empty
	const int sameArr[] = { 10, 20, 30 };
	Vector<int> d0, d1;
	getListDiffPod(sameArr, 3, sameArr, 3, d0, d1);
	FrameUtilityTestImpl_FUT_ASSERT(d0.isEmpty(), "getListDiffPod same -> diff0 empty");
	FrameUtilityTestImpl_FUT_ASSERT(d1.isEmpty(), "getListDiffPod same -> diff1 empty");

	// empty arrays
	Vector<int> e0, e1;
	getListDiffPod<int>(nullptr, 0, nullptr, 0, e0, e1);
	FrameUtilityTestImpl_FUT_ASSERT(e0.isEmpty(), "getListDiffPod empty -> d0 empty");
	FrameUtilityTestImpl_FUT_ASSERT(e1.isEmpty(), "getListDiffPod empty -> d1 empty");
}

//------------------------------------------------------------------------------
// generateCRC16: deterministic for same input, different for different inputs
//------------------------------------------------------------------------------
static void testGenerateCRC16()
{
	const char* str1 = "hello";
	const char* str2 = "world";
	const char* str3 = "hello";

	const ushort crc1 = generateCRC16(str1, (int)strlen(str1));
	const ushort crc2 = generateCRC16(str2, (int)strlen(str2));
	const ushort crc3 = generateCRC16(str3, (int)strlen(str3));

	// same input -> same CRC
	FrameUtilityTestImpl_FUT_ASSERT_EQ(crc1, crc3, "generateCRC16 same input same result");
	// different input -> different CRC (with very high probability)
	FrameUtilityTestImpl_FUT_ASSERT(crc1 != crc2, "generateCRC16 diff input diff result");

	// empty string
	const ushort crcEmpty = generateCRC16("", 0);
	// just verify it doesn't crash and returns a value
	(void)crcEmpty;

	// integer overload
	const ushort crcInt1 = generateCRC16(12345);
	const ushort crcInt2 = generateCRC16(12345);
	const ushort crcInt3 = generateCRC16(99999);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(crcInt1, crcInt2, "generateCRC16<int> same value same CRC");
	FrameUtilityTestImpl_FUT_ASSERT(crcInt1 != crcInt3, "generateCRC16<int> diff value diff CRC");
}

//------------------------------------------------------------------------------
// hashMapValueToArray with startIndex
//------------------------------------------------------------------------------
static void testHashMapValueToArray()
{
	HashMap<int, int> hm;
	hm.add(1, 100);
	hm.add(2, 200);
	hm.add(3, 300);

	Array<8, int> arr;
	arr.zero();
	const int count = hashMapValueToArray(hm, arr, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count, 3, "hashMapValueToArray count=3");
	// values may be in any order since HashMap is unordered
	int sum = 0;
	for (int i = 0; i < count; ++i)
	{
		sum += arr[i];
	}
	FrameUtilityTestImpl_FUT_ASSERT_EQ(sum, 600, "hashMapValueToArray sum=600");

	// startIndex >= size -> count=0
	Array<8, int> arr2;
	const int countOver = hashMapValueToArray(hm, arr2, 10);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(countOver, 0, "hashMapValueToArray over startIndex=0");
}

//------------------------------------------------------------------------------
// mapValueToArrayFilter: filter out specific value
//------------------------------------------------------------------------------
static void testMapValueToArrayFilter()
{
	Map<int, int> m;
	m[1] = 10;
	m[2] = 99; // this one will be filtered out
	m[3] = 30;
	m[4] = 40;

	Array<4, int> arr;
	arr.zero();
	const int count = mapValueToArrayFilter(m, arr, 99, 0);
	// 99 is filtered, so count=3
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count, 3, "mapValueToArrayFilter count=3 (99 excluded)");
	FrameUtilityTestImpl_FUT_ASSERT(!arr.contains(99), "mapValueToArrayFilter arr has no 99");

	// filter a value not in map: all pass
	Array<4, int> arr2;
	arr2.zero();
	const int count2 = mapValueToArrayFilter(m, arr2, 999, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count2, 4, "mapValueToArrayFilter no filter count=4");
}

//------------------------------------------------------------------------------
// setToArray: copy Set elements into a fixed Array
//------------------------------------------------------------------------------
static void testSetToArray()
{
	Set<int> s;
	s.add(10);
	s.add(20);
	s.add(30);
	s.add(40);
	s.add(50);

	Array<5, int> arr;
	arr.zero();
	const int count = setToArray(s, arr, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count, 5, "setToArray count=5");
	FrameUtilityTestImpl_FUT_ASSERT(arr.contains(10), "setToArray contains 10");
	FrameUtilityTestImpl_FUT_ASSERT(arr.contains(50), "setToArray contains 50");

	// startIndex=2 -> skip first 2
	Array<5, int> arr2;
	arr2.zero();
	const int count2 = setToArray(s, arr2, 2);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count2, 3, "setToArray startIndex=2 count=3");

	// startIndex >= size -> 0
	Array<5, int> arr3;
	arr3.zero();
	const int count3 = setToArray(s, arr3, 10);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count3, 0, "setToArray startIndex=10 count=0");

	// capacity limit: Array<3>
	Array<3, int> arr4;
	arr4.zero();
	const int count4 = setToArray(s, arr4, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count4, 3, "setToArray capacity=3 count=3");
}

//------------------------------------------------------------------------------
// hashMapValueToArrayFilter
//------------------------------------------------------------------------------
static void testHashMapValueToArrayFilter()
{
	HashMap<int, int> hm;
	hm.add(1, 100);
	hm.add(2, 200);
	hm.add(3, 999);  // filtered
	hm.add(4, 400);

	Array<4, int> arr;
	arr.zero();
	const int count = hashMapValueToArrayFilter(hm, arr, 999, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count, 3, "hmValueToArrayFilter count=3");
	FrameUtilityTestImpl_FUT_ASSERT(!arr.contains(999), "hmValueToArrayFilter no 999");

	// filter value not in map -> all pass
	Array<4, int> arr2;
	arr2.zero();
	const int count2 = hashMapValueToArrayFilter(hm, arr2, -1, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count2, 4, "hmValueToArrayFilter no filter count=4");

	// startIndex >= size -> 0
	Array<4, int> arr3;
	arr3.zero();
	const int count3 = hashMapValueToArrayFilter(hm, arr3, 999, 100);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count3, 0, "hmValueToArrayFilter over startIndex count=0");
}

//------------------------------------------------------------------------------
// mapValueToVectorFilter
//------------------------------------------------------------------------------
static void testMapValueToVectorFilter()
{
	Map<int, int> m;
	m[1] = 10;
	m[2] = 20;
	m[3] = 99;  // filtered
	m[4] = 30;

	Vector<int> v;
	mapValueToVectorFilter(m, v, 99);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v.size(), 3, "mapValueToVectorFilter size=3");
	FrameUtilityTestImpl_FUT_ASSERT(!v.contains(99), "mapValueToVectorFilter no 99");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(10), "mapValueToVectorFilter has 10");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(20), "mapValueToVectorFilter has 20");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(30), "mapValueToVectorFilter has 30");

	// empty map
	Map<int, int> empty;
	Vector<int> v2;
	mapValueToVectorFilter(empty, v2, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v2.size(), 0, "mapValueToVectorFilter empty map size=0");
}

//------------------------------------------------------------------------------
// hashMapValueToVectorFilter
//------------------------------------------------------------------------------
static void testHashMapValueToVectorFilter()
{
	HashMap<int, int> hm;
	hm.add(1, 100);
	hm.add(2, 200);
	hm.add(3, 999);  // filtered
	hm.add(4, 400);
	hm.add(5, 500);

	Vector<int> v;
	hashMapValueToVectorFilter(hm, v, 999);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v.size(), 4, "hmValueToVectorFilter size=4");
	FrameUtilityTestImpl_FUT_ASSERT(!v.contains(999), "hmValueToVectorFilter no 999");
	FrameUtilityTestImpl_FUT_ASSERT(v.contains(100), "hmValueToVectorFilter has 100");

	// filter value not present
	Vector<int> v2;
	hashMapValueToVectorFilter(hm, v2, -999);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v2.size(), 5, "hmValueToVectorFilter no filter size=5");

	// empty map
	HashMap<int, int> empty;
	Vector<int> v3;
	hashMapValueToVectorFilter(empty, v3, 0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v3.size(), 0, "hmValueToVectorFilter empty size=0");
}

//------------------------------------------------------------------------------
// hashMapKeyValueToArrayList
//------------------------------------------------------------------------------
static void testHashMapKeyValueToArrayList2()
{
	HashMap<int, int> hm;
	hm.add(10, 100);
	hm.add(20, 200);
	hm.add(30, 300);

	ArrayList<5, int> keys;
	ArrayList<5, int> values;
	hashMapKeyValueToArrayList(hm, keys, values);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(keys.size(), 3, "hmKeyValueToArrayList keys.size=3");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(values.size(), 3, "hmKeyValueToArrayList values.size=3");
	// Every key should have a corresponding value
	bool allMatched = true;
	for (int i = 0; i < keys.size(); ++i)
	{
		const int k = keys[i];
		const int v = values[i];
		if (k == 10)
        { 
            allMatched &= (v == 100); 
        }
		else if (k == 20) 
        {
            allMatched &= (v == 200); 
        }
		else if (k == 30) 
        {
            allMatched &= (v == 300); 
        }
	}
	FrameUtilityTestImpl_FUT_ASSERT(allMatched, "hmKeyValueToArrayList key-value pairs match");

	// empty map
	HashMap<int, int> empty;
	ArrayList<5, int> k2, v2;
	hashMapKeyValueToArrayList(empty, k2, v2);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(k2.size(), 0, "hmKeyValueToArrayList empty keys=0");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v2.size(), 0, "hmKeyValueToArrayList empty values=0");
}

//------------------------------------------------------------------------------
// hashMapKeyToArrayList
//------------------------------------------------------------------------------
static void testHashMapKeyToArrayList2()
{
	HashMap<int, int> hm;
	hm.add(1, 10);
	hm.add(2, 20);
	hm.add(3, 30);
	hm.add(4, 40);

	ArrayList<8, int> keys;
	hashMapKeyToArrayList(hm, keys);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(keys.size(), 4, "hmKeyToArrayList size=4");
	FrameUtilityTestImpl_FUT_ASSERT(keys.contains(1), "hmKeyToArrayList has 1");
	FrameUtilityTestImpl_FUT_ASSERT(keys.contains(4), "hmKeyToArrayList has 4");

	// empty
	HashMap<int, int> empty;
	ArrayList<8, int> keys2;
	hashMapKeyToArrayList(empty, keys2);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(keys2.size(), 0, "hmKeyToArrayList empty size=0");
}

//------------------------------------------------------------------------------
// CRC16 extended: multiple types and edge cases
//------------------------------------------------------------------------------
static void testGenerateCRC16Extended()
{
	// same value same result
	const ushort crc1 = generateCRC16(12345);
	const ushort crc2 = generateCRC16(12345);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(crc1, crc2, "CRC16 same int same result");

	// different values different result
	const ushort crc3 = generateCRC16(12346);
	FrameUtilityTestImpl_FUT_ASSERT(crc1 != crc3, "CRC16 diff int diff result");

	// llong
	const ushort crcLL1 = generateCRC16((llong)999999999LL);
	const ushort crcLL2 = generateCRC16((llong)999999999LL);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(crcLL1, crcLL2, "CRC16 same llong same result");

	// float
	const ushort crcF1 = generateCRC16(3.14f);
	const ushort crcF2 = generateCRC16(3.14f);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(crcF1, crcF2, "CRC16 same float same result");

	// zero
	const ushort crcZ = generateCRC16(0);
	const ushort crcZ2 = generateCRC16(0);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(crcZ, crcZ2, "CRC16 zero consistent");

	// string buffer
	const char* s1 = "hello";
	const char* s2 = "hello";
	const char* s3 = "world";
	const ushort crcS1 = generateCRC16(s1, 5);
	const ushort crcS2 = generateCRC16(s2, 5);
	const ushort crcS3 = generateCRC16(s3, 5);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(crcS1, crcS2, "CRC16 same string same result");
	FrameUtilityTestImpl_FUT_ASSERT(crcS1 != crcS3, "CRC16 diff string diff result");

	// single byte
	const ushort crcA = generateCRC16("A", 1);
	const ushort crcB = generateCRC16("B", 1);
	FrameUtilityTestImpl_FUT_ASSERT(crcA != crcB, "CRC16 single byte diff");
	// same byte same crc
	const ushort crcA2 = generateCRC16("A", 1);
	FrameUtilityTestImpl_FUT_ASSERT_EQ(crcA, crcA2, "CRC16 single byte same");
}

//------------------------------------------------------------------------------
// tickTimerOnce: edge cases
//------------------------------------------------------------------------------
static void testTickTimerOnceEdge()
{
	// elapsed exactly equals time -> triggered
	float t = 0.5f;
	FrameUtilityTestImpl_FUT_ASSERT(tickTimerOnce(t, 0.5f), "tickOnce exact trigger");

	// large elapsed -> triggered
	float t2 = 0.001f;
	FrameUtilityTestImpl_FUT_ASSERT(tickTimerOnce(t2, 100.0f), "tickOnce large elapsed trigger");

	// time=0 already -> triggered immediately
	float t3 = 0.0f;
	FrameUtilityTestImpl_FUT_ASSERT(tickTimerOnce(t3, 0.0f), "tickOnce zero time trigger");

	// very small elapsed many steps
	float t4 = 1.0f;
	int steps = 0;
	FOR(200)
	{
		if (tickTimerOnce(t4, 0.01f))
		{
			steps = i + 1;
			break;
		}
	}
	FrameUtilityTestImpl_FUT_ASSERT(steps >= 99 && steps <= 101, "tickOnce small steps ~100");
}

//------------------------------------------------------------------------------
// tickTimerLoop: edge cases
//------------------------------------------------------------------------------
static void testTickTimerLoopEdge()
{
	// with interval=0: implementation-specific, just verify no crash
	// We only test valid intervals > 0
	float t2 = 0.0f;
	int count2 = 0;
	FOR(30)
	{
		if (tickTimerLoop(t2, 0.1f, 1.0f))
		{
			++count2;
		}
	}
	FrameUtilityTestImpl_FUT_ASSERT_EQ(count2, 4, "tickTimerLoop 30*0.1=4 triggers (t=0 fires immediately)");

	// large elapsed -> multiple triggers possible
	float t3 = 0.0f;
	// single step of 5.0 with interval 1.0: should trigger
	FrameUtilityTestImpl_FUT_ASSERT(tickTimerLoop(t3, 5.0f, 1.0f), "tickTimerLoop large elapsed trigger");
}

//------------------------------------------------------------------------------
// getListDiffPod: extended cases
//------------------------------------------------------------------------------
static void testGetListDiffPodExtended()
{
	// identical arrays -> both diffs empty
	int arr0[] = {1, 2, 3, 4, 5};
	int arr1[] = {1, 2, 3, 4, 5};
	Vector<int> d0, d1;
	getListDiffPod(arr0, 5, arr1, 5, d0, d1);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)d0.size(), 0, "diffPod identical d0 empty");
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)d1.size(), 0, "diffPod identical d1 empty");

	// completely disjoint
	int a[] = {10, 20, 30};
	int b[] = {40, 50, 60};
	Vector<int> da, db;
	getListDiffPod(a, 3, b, 3, da, db);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)da.size(), 3, "diffPod disjoint da=3");
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)db.size(), 3, "diffPod disjoint db=3");

	// one empty
	int c[] = {1, 2, 3};
	Vector<int> dc0, dc1;
	getListDiffPod(c, 3, c, 0, dc0, dc1);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)dc0.size(), 3, "diffPod one empty dc0=3");
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)dc1.size(), 0, "diffPod one empty dc1=0");

	// both empty
	Vector<int> de0, de1;
	getListDiffPod(c, 0, c, 0, de0, de1);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)de0.size(), 0, "diffPod both empty de0=0");
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)de1.size(), 0, "diffPod both empty de1=0");

	// partial overlap
	int p0[] = {1, 2, 3, 4};
	int p1[] = {3, 4, 5, 6};
	Vector<int> dp0, dp1;
	getListDiffPod(p0, 4, p1, 4, dp0, dp1);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)dp0.size(), 2, "diffPod partial dp0=2");
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)dp1.size(), 2, "diffPod partial dp1=2");
	FrameUtilityTestImpl_FUT_ASSERT(dp0.contains(1), "diffPod partial dp0 has 1");
	FrameUtilityTestImpl_FUT_ASSERT(dp0.contains(2), "diffPod partial dp0 has 2");
	FrameUtilityTestImpl_FUT_ASSERT(dp1.contains(5), "diffPod partial dp1 has 5");
	FrameUtilityTestImpl_FUT_ASSERT(dp1.contains(6), "diffPod partial dp1 has 6");
}

//------------------------------------------------------------------------------
// removeDuplicate: extended
//------------------------------------------------------------------------------
static void testRemoveDuplicateExtended()
{
	// all unique -> unchanged size
	Vector<int> v;
	v.add(1);
	v.add(2);
	v.add(3);
	v.add(4);
	removeDumplicate(v);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v.size(), 4, "removeDuplicate all unique size=4");

	// all same -> size=1
	Vector<int> v2;
	v2.add(5);
	v2.add(5);
	v2.add(5);
	removeDumplicate(v2);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v2.size(), 1, "removeDuplicate all same size=1");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v2[0], 5, "removeDuplicate all same val=5");

	// mixed duplicates
	Vector<int> v3;
	v3.add(1); v3.add(2); v3.add(1); v3.add(3); v3.add(2);
	removeDumplicate(v3);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v3.size(), 3, "removeDuplicate mixed size=3");
	FrameUtilityTestImpl_FUT_ASSERT(v3.contains(1), "removeDuplicate mixed has 1");
	FrameUtilityTestImpl_FUT_ASSERT(v3.contains(2), "removeDuplicate mixed has 2");
	FrameUtilityTestImpl_FUT_ASSERT(v3.contains(3), "removeDuplicate mixed has 3");

	// single element -> unchanged
	Vector<int> v4;
	v4.add(42);
	removeDumplicate(v4);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v4.size(), 1, "removeDuplicate single size=1");
	FrameUtilityTestImpl_FUT_ASSERT_EQ(v4[0], 42, "removeDuplicate single val=42");

	// empty -> unchanged
	Vector<int> v5;
	removeDumplicate(v5);
	FrameUtilityTestImpl_FUT_ASSERT_EQ((int)v5.size(), 0, "removeDuplicate empty size=0");
}

//------------------------------------------------------------------------------
// FrameUtilityTest entry point
//------------------------------------------------------------------------------
static void test()
{
	testTickTimerOnce();
	testTickTimerLoop();
	testTickTimerLoopEnsure();
	testSetToVector();
	testMapValueToVector2();
	testHashMapValueToVector();
	testMapKeyToVector2();
	testHashMapKeyToVector2();
	testMapValueToArray();
	testVectorToSet();
	testRemoveDuplicate();
	testGetListDiffPod();
	testGenerateCRC16();
	testHashMapValueToArray();
	testMapValueToArrayFilter();
	testSetToArray();
	testHashMapValueToArrayFilter();
	testMapValueToVectorFilter();
	testHashMapValueToVectorFilter();
	testHashMapKeyValueToArrayList2();
	testHashMapKeyToArrayList2();
	testGenerateCRC16Extended();
	testTickTimerOnceEdge();
	testTickTimerLoopEdge();
	testGetListDiffPodExtended();
	testRemoveDuplicateExtended();
}
};

struct FrameUtilityArrayTestImpl
{

#define FrameUtilityArrayTestImpl_FUAT_ASSERT(condition) if (!(condition)) { ERROR("FrameUtilityArrayTest failed: " #condition); }

static void test()
{
    testMapValueToArrayBasic();
    testMapValueToArrayStartIndex();
    testMapValueToArrayOverflow();
    testHashMapValueToArrayBasic();
    testHashMapValueToArrayStartIndex();
    testHashMapValueToArrayListBasic();
    testMapValueToArrayFilterBasic();
    testHashMapValueToArrayFilterBasic();
    testMapValueToListBasic();
    testHashMapValueToListBasic();
    testMapValueToVectorBasic();
    testHashMapValueToVectorBasic();
    testMapValueToVectorFilterBasic();
    testHashMapValueToVectorFilterBasic();
    testMapValueToArrayEmptyMap();
    testHashMapValueToArrayEmptyMap();
    testMapValueToArrayStringValue();
    testHashMapValueToArrayStringValue();
}

static void testMapValueToArrayBasic()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    Array<8, int> arr;
    int count = mapValueToArray(m, arr);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 3);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[0] == 10);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[1] == 20);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[2] == 30);
}

static void testMapValueToArrayStartIndex()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    Array<8, int> arr;
    int count = mapValueToArray(m, arr, 1);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 2);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[0] == 20);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[1] == 30);
}

static void testMapValueToArrayOverflow()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.add(4, 40);
    m.add(5, 50);
    Array<3, int> arr;
    int count = mapValueToArray(m, arr);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 3);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[0] == 10);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[1] == 20);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[2] == 30);
}

static void testHashMapValueToArrayBasic()
{
    HashMap<int, int> m;
    m.add(100, 1000);
    m.add(200, 2000);
    Array<8, int> arr;
    int count = hashMapValueToArray(m, arr);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 2);
    int sum = arr[0] + arr[1];
    FrameUtilityArrayTestImpl_FUAT_ASSERT(sum == 3000);
}

static void testHashMapValueToArrayStartIndex()
{
    HashMap<int, int> m;
    m.add(10, 100);
    m.add(20, 200);
    m.add(30, 300);
    Array<8, int> arr;
    int count = hashMapValueToArray(m, arr, 2);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 1);
}

static void testHashMapValueToArrayListBasic()
{
    HashMap<int, int> m;
    m.add(1, 11);
    m.add(2, 22);
    m.add(3, 33);
    ArrayList<8, int> arr;
    hashMapValueToArray(m, arr);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr.size() == 3);
}

static void testMapValueToArrayFilterBasic()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 0);
    m.add(3, 30);
    m.add(4, 0);
    m.add(5, 50);
    Array<8, int> arr;
    int count = mapValueToArrayFilter(m, arr, 0);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 3);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[0] == 10);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[1] == 30);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[2] == 50);
}

static void testHashMapValueToArrayFilterBasic()
{
    HashMap<int, int> m;
    m.add(1, 100);
    m.add(2, -1);
    m.add(3, 300);
    m.add(4, -1);
    Array<8, int> arr;
    int count = hashMapValueToArrayFilter(m, arr, -1);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 2);
}

static void testMapValueToListBasic()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    int listBuf[8];
    int count = mapValueToList(m, listBuf, 8);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 3);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(listBuf[0] == 10);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(listBuf[1] == 20);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(listBuf[2] == 30);
}

static void testHashMapValueToListBasic()
{
    HashMap<int, int> m;
    m.add(5, 500);
    m.add(6, 600);
    int listBuf[8];
    int count = hashMapValueToList(m, listBuf, 8);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 2);
    int sum = listBuf[0] + listBuf[1];
    FrameUtilityArrayTestImpl_FUAT_ASSERT(sum == 1100);
}

static void testMapValueToVectorBasic()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    Vector<int> vec;
    mapValueToVector(m, vec);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(vec.size() == 3);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(vec[0] == 10);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(vec[1] == 20);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(vec[2] == 30);
}

static void testHashMapValueToVectorBasic()
{
    HashMap<string, int> m;
    m.add("apple", 1);
    m.add("banana", 2);
    m.add("cherry", 3);
    Vector<int> vec;
    hashMapValueToVector(m, vec);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(vec.size() == 3);
    int sum = 0;
    for (int i = 0; i < vec.size(); ++i)
    {
        sum += vec[i];
    }
    FrameUtilityArrayTestImpl_FUAT_ASSERT(sum == 6);
}

static void testMapValueToVectorFilterBasic()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, -99);
    m.add(3, 30);
    m.add(4, -99);
    Vector<int> vec;
    mapValueToVectorFilter(m, vec, -99);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(vec.size() == 2);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(vec[0] == 10);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(vec[1] == 30);
}

static void testHashMapValueToVectorFilterBasic()
{
    HashMap<int, int> m;
    m.add(1, 100);
    m.add(2, 0);
    m.add(3, 200);
    m.add(4, 0);
    Vector<int> vec;
    hashMapValueToVectorFilter(m, vec, 0);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(vec.size() == 2);
}

static void testMapValueToArrayEmptyMap()
{
    Map<int, int> m;
    Array<8, int> arr;
    int count = mapValueToArray(m, arr);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 0);
}

static void testHashMapValueToArrayEmptyMap()
{
    HashMap<int, int> m;
    Array<8, int> arr;
    int count = hashMapValueToArray(m, arr);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 0);
}

static void testMapValueToArrayStringValue()
{
    Map<int, string> m;
    m.add(1, "hello");
    m.add(2, "world");
    m.add(3, "foo");
    Array<8, string> arr;
    int count = mapValueToArray(m, arr);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 3);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[0] == "hello");
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[1] == "world");
    FrameUtilityArrayTestImpl_FUAT_ASSERT(arr[2] == "foo");
}

static void testHashMapValueToArrayStringValue()
{
    HashMap<string, string> m;
    m.add("k1", "v1");
    m.add("k2", "v2");
    Array<8, string> arr;
    int count = hashMapValueToArray(m, arr);
    FrameUtilityArrayTestImpl_FUAT_ASSERT(count == 2);
}
};

struct FrameUtilityDiffTestImpl
{

#define FrameUtilityDiffTestImpl_FUDT_ASSERT(condition) if (!(condition)) { ERROR("FrameUtilityDiffTest failed: " #condition); }

static void test()
{
    testRemoveDumplicateBasic();
    testRemoveDumplicateNoDup();
    testRemoveDumplicateSingle();
    testRemoveDumplicateAllSame();
    testVectorToSetBasic();
    testVectorToSetAppend();
    testSetToVectorBasic();
    testSetToVectorReturnValue();
    testSetToArrayBasic();
    testSetToArrayStartIndex();
    testSetToArrayListBasic();
    testGetListDiffPodBasic();
    testGetListDiffPodNoOverlap();
    testGetListDiffPodAllSame();
    testGetListDiffPodArrayList();
    testGetListDiffNonPod();
    testHashMapKeyToArrayList();
    testHashMapKeyValueToArrayList();
}

static void testRemoveDumplicateBasic()
{
    Vector<int> vec;
    vec.add(1);
    vec.add(2);
    vec.add(1);
    vec.add(3);
    vec.add(2);
    removeDumplicate(vec);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(vec.size() == 3);
    bool has1 = false;
    bool has2 = false;
    bool has3 = false;
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i] == 1) { has1 = true; }
        if (vec[i] == 2) { has2 = true; }
        if (vec[i] == 3) { has3 = true; }
    }
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has2);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has3);
}

static void testRemoveDumplicateNoDup()
{
    Vector<int> vec;
    vec.add(10);
    vec.add(20);
    vec.add(30);
    removeDumplicate(vec);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(vec.size() == 3);
}

static void testRemoveDumplicateSingle()
{
    Vector<int> vec;
    vec.add(42);
    removeDumplicate(vec);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(vec.size() == 1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(vec[0] == 42);
}

static void testRemoveDumplicateAllSame()
{
    Vector<int> vec;
    vec.add(5);
    vec.add(5);
    vec.add(5);
    vec.add(5);
    removeDumplicate(vec);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(vec.size() == 1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(vec[0] == 5);
}

static void testVectorToSetBasic()
{
    Vector<int> vec;
    vec.add(1);
    vec.add(2);
    vec.add(3);
    Set<int> s;
    vectorToSet(vec, s, false);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(s.size() == 3);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(s.contains(1));
    FrameUtilityDiffTestImpl_FUDT_ASSERT(s.contains(2));
    FrameUtilityDiffTestImpl_FUDT_ASSERT(s.contains(3));
}

static void testVectorToSetAppend()
{
    Vector<int> vec;
    vec.add(4);
    vec.add(5);
    Set<int> s;
    s.add(1);
    s.add(2);
    vectorToSet(vec, s, true);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(s.size() == 4);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(s.contains(1));
    FrameUtilityDiffTestImpl_FUDT_ASSERT(s.contains(4));
    FrameUtilityDiffTestImpl_FUDT_ASSERT(s.contains(5));
}

static void testSetToVectorBasic()
{
    Set<int> s;
    s.add(10);
    s.add(20);
    s.add(30);
    Vector<int> vec;
    setToVector(s, vec);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(vec.size() == 3);
    bool has10 = false;
    bool has20 = false;
    bool has30 = false;
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i] == 10) { has10 = true; }
        if (vec[i] == 20) { has20 = true; }
        if (vec[i] == 30) { has30 = true; }
    }
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has10);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has20);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has30);
}

static void testSetToVectorReturnValue()
{
    Set<int> s;
    s.add(7);
    s.add(8);
    Vector<int> vec = setToVector(s);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(vec.size() == 2);
}

static void testSetToArrayBasic()
{
    Set<int> s;
    s.add(1);
    s.add(2);
    s.add(3);
    Array<8, int> arr;
    int count = setToArray(s, arr, 0);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(count == 3);
}

static void testSetToArrayStartIndex()
{
    Set<int> s;
    s.add(1);
    s.add(2);
    s.add(3);
    s.add(4);
    Array<8, int> arr;
    int count = setToArray(s, arr, 2);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(count == 2);
}

static void testSetToArrayListBasic()
{
    Set<int> s;
    s.add(100);
    s.add(200);
    s.add(300);
    ArrayList<8, int> arr;
    setToArray(s, arr, 0);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(arr.size() == 3);
}

static void testGetListDiffPodBasic()
{
    int arr0[] = { 1, 2, 3, 4, 5 };
    int arr1[] = { 3, 4, 5, 6, 7 };
    Vector<int> diff0;
    Vector<int> diff1;
    getListDiffPod(arr0, 5, arr1, 5, diff0, diff1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff0.size() == 2);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff1.size() == 2);
    bool has1 = false;
    bool has2 = false;
    for (int i = 0; i < diff0.size(); ++i)
    {
        if (diff0[i] == 1) { has1 = true; }
        if (diff0[i] == 2) { has2 = true; }
    }
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has2);
}

static void testGetListDiffPodNoOverlap()
{
    int arr0[] = { 1, 2, 3 };
    int arr1[] = { 4, 5, 6 };
    Vector<int> diff0;
    Vector<int> diff1;
    getListDiffPod(arr0, 3, arr1, 3, diff0, diff1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff0.size() == 3);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff1.size() == 3);
}

static void testGetListDiffPodAllSame()
{
    int arr0[] = { 10, 20, 30 };
    int arr1[] = { 10, 20, 30 };
    Vector<int> diff0;
    Vector<int> diff1;
    getListDiffPod(arr0, 3, arr1, 3, diff0, diff1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff0.size() == 0);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff1.size() == 0);
}

static void testGetListDiffPodArrayList()
{
    ArrayList<8, int> arr0;
    arr0.add(1);
    arr0.add(2);
    arr0.add(3);
    ArrayList<8, int> arr1;
    arr1.add(2);
    arr1.add(3);
    arr1.add(4);
    ArrayList<8, int> diff0;
    ArrayList<8, int> diff1;
    getListDiffPod(arr0, arr1, diff0, diff1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff0.size() == 1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff1.size() == 1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff0[0] == 1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff1[0] == 4);
}

static void testGetListDiffNonPod()
{
    string arr0[] = { "apple", "banana", "cherry" };
    string arr1[] = { "banana", "cherry", "date" };
    Vector<string> diff0;
    Vector<string> diff1;
    getListDiff(arr0, 3, arr1, 3, diff0, diff1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff0.size() == 1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff1.size() == 1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff0[0] == "apple");
    FrameUtilityDiffTestImpl_FUDT_ASSERT(diff1[0] == "date");
}

static void testHashMapKeyToArrayList()
{
    HashMap<int, string> m;
    m.add(1, "one");
    m.add(2, "two");
    m.add(3, "three");
    ArrayList<8, int> keyList;
    hashMapKeyToArrayList(m, keyList);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(keyList.size() == 3);
    bool has1 = false;
    bool has2 = false;
    bool has3 = false;
    for (int i = 0; i < keyList.size(); ++i)
    {
        if (keyList[i] == 1) { has1 = true; }
        if (keyList[i] == 2) { has2 = true; }
        if (keyList[i] == 3) { has3 = true; }
    }
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has1);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has2);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(has3);
}

static void testHashMapKeyValueToArrayList()
{
    HashMap<int, string> m;
    m.add(10, "ten");
    m.add(20, "twenty");
    ArrayList<8, int> keyList;
    ArrayList<8, string> valueList;
    hashMapKeyValueToArrayList(m, keyList, valueList);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(keyList.size() == 2);
    FrameUtilityDiffTestImpl_FUDT_ASSERT(valueList.size() == 2);
}
};

struct FrameUtilityExt2TestImpl
{

// ============================================================
// FrameUtilityExt2Test
// 覆盖 FrameUtility 中原有 FrameUtilityTest 未测到的函数：
//   mapKeyToVector / hashMapKeyToVector(2版本)
//   hashMapKeyToArrayList / hashMapKeyValueToArrayList
//   mapValueToVector / hashMapValueToVector / hashMapToVector
//   mapValueToList / hashMapValueToList
//   mapKeyToList / hashMapKeyToList
//   getListDiff (非POD类型)
//   getListDiffPod (ArrayList版本)
// ============================================================

#define FrameUtilityExt2TestImpl_FUE2_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("FrameUtilityExt2Test FAILED: ") + (msg)); } } while(0)
#define FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("FrameUtilityExt2Test FAILED: ") + (msg) \
        + " expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

// ============================================================
// mapKeyToVector
// ============================================================
static void testMapKeyToVector1()
{
    // 基本用法
    {
        Map<int, string> m;
        m.add(3, "c");
        m.add(1, "a");
        m.add(2, "b");
        Vector<int> keys;
        mapKeyToVector(m, keys);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 3, "mapKeyToVector size==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(1), "mapKeyToVector contains 1");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(2), "mapKeyToVector contains 2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(3), "mapKeyToVector contains 3");
    }

    // 空 map
    {
        Map<int, int> empty;
        Vector<int> keys;
        mapKeyToVector(empty, keys);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 0, "mapKeyToVector empty map size==0");
    }

    // string key
    {
        Map<string, int> m;
        m.add("alpha", 1);
        m.add("beta", 2);
        m.add("gamma", 3);
        Vector<string> keys;
        mapKeyToVector(m, keys);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 3, "mapKeyToVector string key size==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains("alpha"), "mapKeyToVector contains alpha");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains("beta"), "mapKeyToVector contains beta");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains("gamma"), "mapKeyToVector contains gamma");
    }

    // 大量数据
    {
        Map<int, int> m;
        FOR(100)
        {
            m.add(i, i * 2);
        }
        Vector<int> keys;
        mapKeyToVector(m, keys);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 100, "mapKeyToVector large size==100");
        FOR(100)
        {
            FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(i), "mapKeyToVector large contains i");
        }
    }

    // Map 有序，keys 顺序与 Map 遍历顺序一致
    {
        Map<int, int> m;
        m.add(5, 50);
        m.add(2, 20);
        m.add(8, 80);
        m.add(1, 10);
        m.add(3, 30);
        Vector<int> keys;
        mapKeyToVector(m, keys);
        // Map 是有序的，键从小到大
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 5, "mapKeyToVector ordered size==5");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys[0] == 1, "mapKeyToVector ordered keys[0]==1");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys[1] == 2, "mapKeyToVector ordered keys[1]==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys[2] == 3, "mapKeyToVector ordered keys[2]==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys[3] == 5, "mapKeyToVector ordered keys[3]==5");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys[4] == 8, "mapKeyToVector ordered keys[4]==8");
    }

}

// ============================================================
// hashMapKeyToVector (两个版本)
// ============================================================
static void testHashMapKeyToVector1()
{
    // 输出参数版本
    {
        HashMap<int, string> m;
        m.add(10, "ten");
        m.add(20, "twenty");
        m.add(30, "thirty");
        Vector<int> keys;
        hashMapKeyToVector(m, keys);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 3, "hashMapKeyToVector out-param size==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(10), "hashMapKeyToVector contains 10");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(20), "hashMapKeyToVector contains 20");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(30), "hashMapKeyToVector contains 30");
    }

    // 返回值版本
    {
        HashMap<int, int> m;
        m.add(100, 1);
        m.add(200, 2);
        m.add(300, 3);
        m.add(400, 4);
        Vector<int> keys = hashMapKeyToVector(m);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 4, "hashMapKeyToVector return-val size==4");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(100), "hashMapKeyToVector return-val contains 100");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(400), "hashMapKeyToVector return-val contains 400");
    }

    // 空 HashMap
    {
        HashMap<int, int> empty;
        Vector<int> keys;
        hashMapKeyToVector(empty, keys);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 0, "hashMapKeyToVector empty map size==0");
    }

    // 返回值版本空 HashMap
    {
        HashMap<string, int> empty;
        Vector<string> keys = hashMapKeyToVector(empty);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 0, "hashMapKeyToVector return-val empty size==0");
    }

    // string key
    {
        HashMap<string, float> m;
        m.add("a", 1.0f);
        m.add("b", 2.0f);
        m.add("c", 3.0f);
        Vector<string> keys;
        hashMapKeyToVector(m, keys);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 3, "hashMapKeyToVector string key size==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains("a"), "hashMapKeyToVector string key contains a");
    }

    // 大量数据
    {
        HashMap<int, int> m;
        FOR(200)
        {
            m.add(i * 3, i);
        }
        Vector<int> keys = hashMapKeyToVector(m);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), 200, "hashMapKeyToVector large size==200");
        FOR(200)
        {
            FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(i * 3), "hashMapKeyToVector large contains i*3");
        }
    }

}

// ============================================================
// hashMapKeyToArrayList
// ============================================================
static void testHashMapKeyToArrayList1()
{
    // 基本用法
    {
        HashMap<int, string> m;
        m.add(1, "one");
        m.add(2, "two");
        m.add(3, "three");
        ArrayList<16, int> keyList;
        hashMapKeyToArrayList(m, keyList);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keyList.size(), 3, "hashMapKeyToArrayList size==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keyList.contains(1), "hashMapKeyToArrayList contains 1");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keyList.contains(2), "hashMapKeyToArrayList contains 2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keyList.contains(3), "hashMapKeyToArrayList contains 3");
    }

    // 空 HashMap
    {
        HashMap<int, int> empty;
        ArrayList<16, int> keyList;
        keyList.add(999); // 先填一个，确认被清空
        hashMapKeyToArrayList(empty, keyList);
        // 空 HashMap 直接返回，不清空，size 仍为 1
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keyList.size(), 1, "hashMapKeyToArrayList empty map no change");
    }

    // ArrayList 容量限制（容量不足时停止）
    {
        HashMap<int, int> m;
        for (int i = 0; i < 20; ++i)
        {
            m.add(i, i);
        }
        ArrayList<8, int> keyListSmall;
        hashMapKeyToArrayList(m, keyListSmall);
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keyListSmall.size() <= 8, "hashMapKeyToArrayList capacity limit");
    }

    // 调用前已有数据会被 clear
    {
        HashMap<int, int> m;
        m.add(10, 100);
        m.add(20, 200);
        ArrayList<16, int> keyList;
        keyList.add(999);
        keyList.add(888);
        hashMapKeyToArrayList(m, keyList);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keyList.size(), 2, "hashMapKeyToArrayList clears before fill");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(!keyList.contains(999), "hashMapKeyToArrayList old data cleared");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keyList.contains(10), "hashMapKeyToArrayList new key 10");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keyList.contains(20), "hashMapKeyToArrayList new key 20");
    }

}

// ============================================================
// hashMapKeyValueToArrayList
// ============================================================
static void testHashMapKeyValueToArrayList1()
{
    // 基本用法
    {
        HashMap<int, int> m;
        m.add(1, 100);
        m.add(2, 200);
        m.add(3, 300);
        ArrayList<16, int> keyList;
        ArrayList<16, int> valList;
        hashMapKeyValueToArrayList(m, keyList, valList);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keyList.size(), 3, "hashMapKeyValueToArrayList key size==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(valList.size(), 3, "hashMapKeyValueToArrayList val size==3");
        // 验证 key 和 value 对应关系
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keyList.size() == valList.size(), "hashMapKeyValueToArrayList key/val same size");
    }

    // 空 HashMap
    {
        HashMap<int, int> empty;
        ArrayList<16, int> keyList;
        ArrayList<16, int> valList;
        hashMapKeyValueToArrayList(empty, keyList, valList);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keyList.size(), 0, "hashMapKeyValueToArrayList empty key size==0");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(valList.size(), 0, "hashMapKeyValueToArrayList empty val size==0");
    }

    // 旧数据被清除
    {
        HashMap<int, int> m;
        m.add(5, 50);
        ArrayList<16, int> keyList;
        ArrayList<16, int> valList;
        keyList.add(999);
        valList.add(999);
        hashMapKeyValueToArrayList(m, keyList, valList);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keyList.size(), 1, "hashMapKeyValueToArrayList clears before fill key");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(valList.size(), 1, "hashMapKeyValueToArrayList clears before fill val");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(!keyList.contains(999), "hashMapKeyValueToArrayList old key cleared");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(!valList.contains(999), "hashMapKeyValueToArrayList old val cleared");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(keyList.contains(5), "hashMapKeyValueToArrayList new key 5");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(valList.contains(50), "hashMapKeyValueToArrayList new val 50");
    }

    // 多条数据，验证 key-value 匹配
    {
        HashMap<int, int> m;
        m.add(7, 70);
        m.add(8, 80);
        m.add(9, 90);
        ArrayList<16, int> keyList;
        ArrayList<16, int> valList;
        hashMapKeyValueToArrayList(m, keyList, valList);
        // 验证每个 key 对应的 value
        for (int i = 0; i < keyList.size(); ++i)
        {
            const int key = keyList[i];
            const int val = valList[i];
            FrameUtilityExt2TestImpl_FUE2_ASSERT(val == key * 10, "hashMapKeyValueToArrayList key-val correspondence");
        }
    }

}

// ============================================================
// mapValueToVector
// ============================================================
static void testMapValueToVector1()
{
    // 基本用法
    {
        Map<int, int> m;
        m.add(1, 100);
        m.add(2, 200);
        m.add(3, 300);
        Vector<int> vals;
        mapValueToVector(m, vals);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals.size(), 3, "mapValueToVector size==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains(100), "mapValueToVector contains 100");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains(200), "mapValueToVector contains 200");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains(300), "mapValueToVector contains 300");
    }

    // 空 Map
    {
        Map<int, int> empty;
        Vector<int> vals;
        mapValueToVector(empty, vals);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals.size(), 0, "mapValueToVector empty map size==0");
    }

    // 追加模式（vals 本身已有数据）
    {
        Map<int, int> m;
        m.add(1, 10);
        m.add(2, 20);
        Vector<int> vals;
        vals.add(999);
        mapValueToVector(m, vals);
        // clearAndReserve 会 clear 并 reserve，所以 999 被清除
        // 注意：实现是 clearAndReserve(size+map.size())，即清空并预分配
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals.size(), 2, "mapValueToVector appends after clear+reserve");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(!vals.contains(999), "mapValueToVector old data cleared");
    }

    // string value
    {
        Map<int, string> m;
        m.add(1, "hello");
        m.add(2, "world");
        Vector<string> vals;
        mapValueToVector(m, vals);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals.size(), 2, "mapValueToVector string val size==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains("hello"), "mapValueToVector string val hello");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains("world"), "mapValueToVector string val world");
    }

    // 有序 Map，value 顺序与 key 顺序一致
    {
        Map<int, int> m;
        m.add(10, 1000);
        m.add(20, 2000);
        m.add(30, 3000);
        Vector<int> vals;
        mapValueToVector(m, vals);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals[0], 1000, "mapValueToVector ordered val[0]==1000");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals[1], 2000, "mapValueToVector ordered val[1]==2000");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals[2], 3000, "mapValueToVector ordered val[2]==3000");
    }

}

// ============================================================
// hashMapValueToVector
// ============================================================
static void testHashMapValueToVector2()
{
    // 基本用法
    {
        HashMap<int, int> m;
        m.add(1, 100);
        m.add(2, 200);
        m.add(3, 300);
        Vector<int> vals;
        hashMapValueToVector(m, vals);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals.size(), 3, "hashMapValueToVector size==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains(100), "hashMapValueToVector contains 100");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains(200), "hashMapValueToVector contains 200");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains(300), "hashMapValueToVector contains 300");
    }

    // 空 HashMap
    {
        HashMap<int, int> empty;
        Vector<int> vals;
        hashMapValueToVector(empty, vals);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals.size(), 0, "hashMapValueToVector empty size==0");
    }

    // string value
    {
        HashMap<string, string> m;
        m.add("k1", "v1");
        m.add("k2", "v2");
        Vector<string> vals;
        hashMapValueToVector(m, vals);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals.size(), 2, "hashMapValueToVector string val size==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains("v1"), "hashMapValueToVector string val v1");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains("v2"), "hashMapValueToVector string val v2");
    }

    // 大量数据
    {
        HashMap<int, int> m;
        for (int i = 0; i < 50; ++i)
        {
            m.add(i, i * 5);
        }
        Vector<int> vals;
        hashMapValueToVector(m, vals);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals.size(), 50, "hashMapValueToVector large size==50");
        for (int i = 0; i < 50; ++i)
        {
            FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains(i * 5), "hashMapValueToVector large contains i*5");
        }
    }

}

// ============================================================
// hashMapToVector (key-value pair)
// ============================================================
static void testHashMapToVector()
{
    // 基本用法
    {
        HashMap<int, int> m;
        m.add(1, 10);
        m.add(2, 20);
        m.add(3, 30);
        Vector<pair<int, int>> pairs;
        hashMapToVector(m, pairs);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(pairs.size(), 3, "hashMapToVector size==3");
        // 验证每对的 key-value 关系
        for (int i = 0; i < (int)pairs.size(); ++i)
        {
            FrameUtilityExt2TestImpl_FUE2_ASSERT(pairs[i].second == pairs[i].first * 10, "hashMapToVector pair value==key*10");
        }
    }

    // 空 HashMap
    {
        HashMap<int, int> empty;
        Vector<pair<int, int>> pairs;
        hashMapToVector(empty, pairs);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(pairs.size(), 0, "hashMapToVector empty size==0");
    }

    // string-int pair
    {
        HashMap<string, int> m;
        m.add("name", 42);
        m.add("count", 100);
        Vector<pair<string, int>> pairs;
        hashMapToVector(m, pairs);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(pairs.size(), 2, "hashMapToVector string-int size==2");
        bool foundName = false;
        bool foundCount = false;
        for (int i = 0; i < (int)pairs.size(); ++i)
        {
            if (pairs[i].first == "name" && pairs[i].second == 42)
            {
                foundName = true;
            }
            if (pairs[i].first == "count" && pairs[i].second == 100)
            {
                foundCount = true;
            }
        }
        FrameUtilityExt2TestImpl_FUE2_ASSERT(foundName, "hashMapToVector found name->42");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(foundCount, "hashMapToVector found count->100");
    }

}

// ============================================================
// mapValueToList (raw pointer)
// ============================================================
static void testMapValueToList()
{
    // 基本用法
    {
        Map<int, int> m;
        m.add(1, 10);
        m.add(2, 20);
        m.add(3, 30);
        m.add(4, 40);
        m.add(5, 50);
        int buf[8] = {};
        const int count = mapValueToList(m, buf, 8);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 5, "mapValueToList count==5");
        // 值应全部在 {10,20,30,40,50}
        int sum = 0;
        for (int i = 0; i < count; ++i)
        {
            sum += buf[i];
        }
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(sum, 150, "mapValueToList sum==150");
    }

    // startIndex
    {
        Map<int, int> m;
        m.add(1, 100);
        m.add(2, 200);
        m.add(3, 300);
        m.add(4, 400);
        int buf[4] = {};
        const int count = mapValueToList(m, buf, 4, 2);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 2, "mapValueToList startIndex=2 count==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(buf[0] == 300, "mapValueToList startIndex=2 buf[0]==300");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(buf[1] == 400, "mapValueToList startIndex=2 buf[1]==400");
    }

    // maxCount 限制
    {
        Map<int, int> m;
        for (int i = 1; i <= 10; ++i)
        {
            m.add(i, i * 100);
        }
        int buf[4] = {};
        const int count = mapValueToList(m, buf, 4);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 4, "mapValueToList maxCount=4 count==4");
    }

    // startIndex >= size -> count=0
    {
        Map<int, int> m;
        m.add(1, 10);
        m.add(2, 20);
        int buf[4] = {};
        const int count = mapValueToList(m, buf, 4, 10);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 0, "mapValueToList startIndex>=size count==0");
    }

    // 空 Map
    {
        Map<int, int> empty;
        int buf[4] = {};
        const int count = mapValueToList(empty, buf, 4);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 0, "mapValueToList empty map count==0");
    }

}

// ============================================================
// hashMapValueToList (raw pointer)
// ============================================================
static void testHashMapValueToList()
{
    // 基本用法
    {
        HashMap<int, int> m;
        m.add(1, 10);
        m.add(2, 20);
        m.add(3, 30);
        int buf[8] = {};
        const int count = hashMapValueToList(m, buf, 8);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 3, "hashMapValueToList count==3");
        int sum = 0;
        for (int i = 0; i < count; ++i)
        {
            sum += buf[i];
        }
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(sum, 60, "hashMapValueToList sum==60");
    }

    // startIndex
    {
        HashMap<int, int> m;
        for (int i = 1; i <= 5; ++i)
        {
            m.add(i, i * 10);
        }
        int buf[8] = {};
        const int count = hashMapValueToList(m, buf, 8, 3);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 2, "hashMapValueToList startIndex=3 count==2");
    }

    // maxCount 限制
    {
        HashMap<int, int> m;
        for (int i = 0; i < 20; ++i)
        {
            m.add(i, i * 100);
        }
        int buf[5] = {};
        const int count = hashMapValueToList(m, buf, 5);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 5, "hashMapValueToList maxCount=5 count==5");
    }

    // startIndex >= size -> 0
    {
        HashMap<int, int> m;
        m.add(1, 10);
        int buf[4] = {};
        const int count = hashMapValueToList(m, buf, 4, 100);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 0, "hashMapValueToList startIndex>=size count==0");
    }

    // 空 HashMap
    {
        HashMap<int, int> empty;
        int buf[4] = {};
        const int count = hashMapValueToList(empty, buf, 4);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 0, "hashMapValueToList empty count==0");
    }

}

// ============================================================
// mapKeyToList (raw pointer)
// ============================================================
static void testMapKeyToList()
{
    // 基本用法，所有 key 放得下
    {
        Map<int, int> m;
        m.add(5, 50);
        m.add(3, 30);
        m.add(1, 10);
        m.add(4, 40);
        m.add(2, 20);
        int buf[8] = {};
        bool r = mapKeyToList(m, buf, 8);
        FrameUtilityExt2TestImpl_FUE2_ASSERT(r, "mapKeyToList success");
        // Map 有序，key 应为 1,2,3,4,5
        FrameUtilityExt2TestImpl_FUE2_ASSERT(buf[0] == 1, "mapKeyToList ordered buf[0]==1");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(buf[4] == 5, "mapKeyToList ordered buf[4]==5");
    }

    // maxCount 不足，返回 false
    {
        Map<int, int> m;
        for (int i = 1; i <= 10; ++i)
        {
            m.add(i, i);
        }
        int buf[4] = {};
        bool r = mapKeyToList(m, buf, 4);
        FrameUtilityExt2TestImpl_FUE2_ASSERT(!r, "mapKeyToList insufficient maxCount false");
    }

    // 空 Map，返回 true
    {
        Map<int, int> empty;
        int buf[4] = {};
        bool r = mapKeyToList(empty, buf, 4);
        FrameUtilityExt2TestImpl_FUE2_ASSERT(r, "mapKeyToList empty map true");
    }

    // 恰好放下
    {
        Map<int, int> m;
        m.add(10, 100);
        m.add(20, 200);
        m.add(30, 300);
        int buf[3] = {};
        bool r = mapKeyToList(m, buf, 3);
        FrameUtilityExt2TestImpl_FUE2_ASSERT(r, "mapKeyToList exact capacity true");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(buf[0] == 10 && buf[1] == 20 && buf[2] == 30, "mapKeyToList exact values correct");
    }

}

// ============================================================
// hashMapKeyToList (raw pointer)
// ============================================================
static void testHashMapKeyToList()
{
    // 基本用法
    {
        HashMap<int, int> m;
        m.add(1, 10);
        m.add(2, 20);
        m.add(3, 30);
        int buf[8] = {};
        bool r = hashMapKeyToList(m, buf, 8);
        FrameUtilityExt2TestImpl_FUE2_ASSERT(r, "hashMapKeyToList success");
        // 检查所有 key 出现
        bool found1 = false;
        bool found2 = false;
        bool found3 = false;
        for (int i = 0; i < 3; ++i)
        {
            if (buf[i] == 1)
            {
                found1 = true;
            }
            if (buf[i] == 2)
            {
                found2 = true;
            }
            if (buf[i] == 3)
            {
                found3 = true;
            }
        }
        FrameUtilityExt2TestImpl_FUE2_ASSERT(found1 && found2 && found3, "hashMapKeyToList all keys found");
    }

    // maxCount 不足，返回 false
    {
        HashMap<int, int> m;
        FOR(10)
        {
            m.add(i, i * 10);
        }
        int buf[3] = {};
        bool r = hashMapKeyToList(m, buf, 3);
        FrameUtilityExt2TestImpl_FUE2_ASSERT(!r, "hashMapKeyToList insufficient maxCount false");
    }

    // 空 HashMap，返回 true
    {
        HashMap<int, int> empty;
        int buf[4] = {};
        bool r = hashMapKeyToList(empty, buf, 4);
        FrameUtilityExt2TestImpl_FUE2_ASSERT(r, "hashMapKeyToList empty map true");
    }

}

// ============================================================
// getListDiff (非 POD 类型，vector 版本)
// ============================================================
static void testGetListDiff()
{
    // 基本场景：两个数组各有唯一元素和共同元素
    {
        // array0 = [1, 2, 3, 4, 5], array1 = [3, 4, 5, 6, 7]
        // 共同：3,4,5  差异0：1,2  差异1：6,7
        Vector<int> arr0;
        arr0.add(1);
        arr0.add(2);
        arr0.add(3);
        arr0.add(4);
        arr0.add(5);
        Vector<int> arr1;
        arr1.add(3);
        arr1.add(4);
        arr1.add(5);
        arr1.add(6);
        arr1.add(7);
        Vector<int> diff0;
        Vector<int> diff1;
        getListDiffPod(arr0.data(), arr0.size(), arr1.data(), arr1.size(), diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 2, "getListDiffPod diff0 size==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 2, "getListDiffPod diff1 size==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff0.contains(1), "getListDiffPod diff0 contains 1");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff0.contains(2), "getListDiffPod diff0 contains 2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff1.contains(6), "getListDiffPod diff1 contains 6");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff1.contains(7), "getListDiffPod diff1 contains 7");
    }

    // 完全相同
    {
        int data[] = {10, 20, 30};
        Vector<int> diff0;
        Vector<int> diff1;
        getListDiffPod(data, 3, data, 3, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 0, "getListDiffPod identical diff0 empty");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 0, "getListDiffPod identical diff1 empty");
    }

    // 完全不同
    {
        int a0[] = {1, 2, 3};
        int a1[] = {4, 5, 6};
        Vector<int> diff0;
        Vector<int> diff1;
        getListDiffPod(a0, 3, a1, 3, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 3, "getListDiffPod all-diff diff0 size==3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 3, "getListDiffPod all-diff diff1 size==3");
    }

    // 空数组 vs 非空
    {
        int a1[] = {1, 2, 3};
        Vector<int> diff0;
        Vector<int> diff1;
        getListDiffPod((int*)nullptr, 0, a1, 3, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 0, "getListDiffPod empty vs nonempty diff0==0");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 3, "getListDiffPod empty vs nonempty diff1==3");
    }

    // 非空 vs 空
    {
        int a0[] = {10, 20};
        Vector<int> diff0;
        Vector<int> diff1;
        getListDiffPod(a0, 2, (int*)nullptr, 0, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 2, "getListDiffPod nonempty vs empty diff0==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 0, "getListDiffPod nonempty vs empty diff1==0");
    }

    // 含重复元素
    {
        int a0[] = {1, 1, 2, 3};
        int a1[] = {1, 3, 3};
        Vector<int> diff0;
        Vector<int> diff1;
        getListDiffPod(a0, 4, a1, 3, diff0, diff1);
        // 共同元素：1(one match), 3(one match)
        // diff0: 1(one more), 2
        // diff1: 3(one more)
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff0.contains(2), "getListDiffPod dup: diff0 contains 2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff1.contains(3), "getListDiffPod dup: diff1 contains 3");
    }

}

// ============================================================
// getListDiffPod ArrayList 版本
// ============================================================
static void testGetListDiffPodArrayList()
{
    // 基本场景
    {
        ArrayList<16, int> arr0;
        arr0.add(1);
        arr0.add(2);
        arr0.add(3);
        arr0.add(4);
        ArrayList<16, int> arr1;
        arr1.add(3);
        arr1.add(4);
        arr1.add(5);
        arr1.add(6);
        ArrayList<16, int> diff0;
        ArrayList<16, int> diff1;
        getListDiffPod(arr0, arr1, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 2, "getListDiffPod ArrayList diff0 size==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 2, "getListDiffPod ArrayList diff1 size==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff0.contains(1), "getListDiffPod ArrayList diff0 contains 1");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff0.contains(2), "getListDiffPod ArrayList diff0 contains 2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff1.contains(5), "getListDiffPod ArrayList diff1 contains 5");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff1.contains(6), "getListDiffPod ArrayList diff1 contains 6");
    }

    // 完全相同
    {
        ArrayList<16, int> arr0;
        arr0.add(10);
        arr0.add(20);
        ArrayList<16, int> arr1;
        arr1.add(10);
        arr1.add(20);
        ArrayList<16, int> diff0;
        ArrayList<16, int> diff1;
        getListDiffPod(arr0, arr1, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 0, "getListDiffPod ArrayList identical diff0==0");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 0, "getListDiffPod ArrayList identical diff1==0");
    }

    // 空 vs 非空
    {
        ArrayList<16, int> empty;
        ArrayList<16, int> arr1;
        arr1.add(100);
        arr1.add(200);
        ArrayList<16, int> diff0;
        ArrayList<16, int> diff1;
        getListDiffPod(empty, arr1, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 0, "getListDiffPod ArrayList empty vs diff0==0");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 2, "getListDiffPod ArrayList empty vs diff1==2");
    }

}

// ============================================================
// getListDiff (非 POD 类型，使用 string)
// ============================================================
static void testGetListDiffNonPod()
{
    // 使用 string 类型测试非 POD 版本
    {
        // array0 = ["a","b","c"], array1 = ["b","c","d"]
        // 共同: b,c  diff0: a  diff1: d
        string a0[] = {"a", "b", "c"};
        string a1[] = {"b", "c", "d"};
        Vector<string> diff0;
        Vector<string> diff1;
        getListDiff(a0, 3, a1, 3, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 1, "getListDiff nonpod diff0 size==1");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 1, "getListDiff nonpod diff1 size==1");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff0.contains("a"), "getListDiff nonpod diff0 contains a");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(diff1.contains("d"), "getListDiff nonpod diff1 contains d");
    }

    // 完全相同
    {
        string data[] = {"x", "y", "z"};
        Vector<string> diff0;
        Vector<string> diff1;
        getListDiff(data, 3, data, 3, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 0, "getListDiff nonpod identical diff0==0");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 0, "getListDiff nonpod identical diff1==0");
    }

    // 完全不同
    {
        string a0[] = {"apple", "banana"};
        string a1[] = {"cherry", "date"};
        Vector<string> diff0;
        Vector<string> diff1;
        getListDiff(a0, 2, a1, 2, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 2, "getListDiff nonpod all-diff diff0==2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 2, "getListDiff nonpod all-diff diff1==2");
    }

    // 空 vs 非空
    {
        string a1[] = {"hello", "world"};
        Vector<string> diff0;
        Vector<string> diff1;
        getListDiff((string*)nullptr, 0, a1, 2, diff0, diff1);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff0.size(), 0, "getListDiff nonpod empty vs diff0==0");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(diff1.size(), 2, "getListDiff nonpod empty vs diff1==2");
    }

}

// ============================================================
// 综合场景：map/hashmap 键值操作流水线
// ============================================================
static void testComprehensivePipeline()
{
    // 模拟业务：从 HashMap 中提取 key 和 value，分别验证
    {
        HashMap<int, string> playerMap;
        playerMap.add(101, "Alice");
        playerMap.add(102, "Bob");
        playerMap.add(103, "Charlie");
        playerMap.add(104, "Dave");
        playerMap.add(105, "Eve");

        // 提取所有 key
        Vector<int> ids = hashMapKeyToVector(playerMap);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(ids.size(), 5, "pipeline: 5 player ids");

        // 提取所有 value
        Vector<string> names;
        hashMapValueToVector(playerMap, names);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(names.size(), 5, "pipeline: 5 player names");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(names.contains("Alice"), "pipeline: Alice in names");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(names.contains("Eve"), "pipeline: Eve in names");

        // 同时提取 key+value 到 ArrayList
        ArrayList<16, int> keyList;
        ArrayList<16, string> valList;
        hashMapKeyValueToArrayList(playerMap, keyList, valList);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keyList.size(), 5, "pipeline: keyList size==5");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(valList.size(), 5, "pipeline: valList size==5");

        // 验证 key 和 value 在各自 list 中相同 index 对应
        for (int i = 0; i < keyList.size(); ++i)
        {
            const int key = keyList[i];
            const string& val = valList[i];
            const string* p = playerMap.getPtrConst(key);
            FrameUtilityExt2TestImpl_FUE2_ASSERT(p != nullptr && *p == val, "pipeline: key-val correspondence");
        }
    }

    // 模拟 diff：找出两个玩家列表的差异
    {
        int oldPlayers[] = {101, 102, 103, 104, 105};
        int newPlayers[] = {102, 103, 106, 107};
        // 离线玩家（old 有 new 没有）：101, 104, 105
        // 上线玩家（new 有 old 没有）：106, 107
        Vector<int> loggedOut;
        Vector<int> loggedIn;
        getListDiffPod(oldPlayers, 5, newPlayers, 4, loggedOut, loggedIn);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(loggedOut.size(), 3, "pipeline diff: logged out 3");
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(loggedIn.size(), 2, "pipeline diff: logged in 2");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(loggedOut.contains(101), "pipeline diff: 101 logged out");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(loggedOut.contains(104), "pipeline diff: 104 logged out");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(loggedOut.contains(105), "pipeline diff: 105 logged out");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(loggedIn.contains(106), "pipeline diff: 106 logged in");
        FrameUtilityExt2TestImpl_FUE2_ASSERT(loggedIn.contains(107), "pipeline diff: 107 logged in");
    }
}

// ============================================================
// 大量数据 stress
// ============================================================
static void testLargeScaleOperations()
{
    const int N = 500;

    // 大量 Map Key→Vector
    {
        Map<int, int> m;
        FOR(N)
        {
            m.add(i * 2, i);
        }
        Vector<int> keys;
        mapKeyToVector(m, keys);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(keys.size(), N, "largeScale mapKeyToVector size==N");
        FOR(N)
        {
            FrameUtilityExt2TestImpl_FUE2_ASSERT(keys.contains(i * 2), "largeScale mapKeyToVector contains i*2");
        }
    }

    // 大量 HashMap Value→Vector
    {
        HashMap<int, int> m;
        FOR(N)
        {
            m.add(i, i * 3);
        }
        Vector<int> vals;
        hashMapValueToVector(m, vals);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(vals.size(), N, "largeScale hashMapValueToVector size==N");
        FOR(N)
        {
            FrameUtilityExt2TestImpl_FUE2_ASSERT(vals.contains(i * 3), "largeScale hashMapValueToVector contains i*3");
        }
    }

    // 大量 mapValueToList
    {
        Map<int, int> m;
        FOR(100)
        {
            m.add(i, i + 1000);
        }
        int buf[50] = {};
        const int count = mapValueToList(m, buf, 50);
        FrameUtilityExt2TestImpl_FUE2_ASSERT_EQ(count, 50, "largeScale mapValueToList count==50");
    }

}

// ============================================================
// 主入口
// ============================================================
static void test()
{
    testMapKeyToVector1();
    testHashMapKeyToVector1();
    testHashMapKeyToArrayList1();
    testHashMapKeyValueToArrayList1();
    testMapValueToVector1();
    testHashMapValueToVector2();
    testHashMapToVector();
    testMapValueToList();
    testHashMapValueToList();
    testMapKeyToList();
    testHashMapKeyToList();
    testGetListDiff();
    testGetListDiffPodArrayList();
    testGetListDiffNonPod();
    testComprehensivePipeline();
    testLargeScaleOperations();
}
};

struct FrameUtilityExt3TestImpl
{

#define FrameUtilityExt3TestImpl_FUE3_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("FrameUtilityExt3Test FAILED: ") + (msg)); } } while(0)
#define FrameUtilityExt3TestImpl_FUE3_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("FrameUtilityExt3Test EQ FAILED: ") + (msg) \
        + " got=" + IToS((int)(a)) + " exp=" + IToS((int)(b))); } } while(0)

static void test()
{
    testHashMapValueToArray();
    testHashMapValueToArrayList();
    testMapValueToArrayFilter();
    testHashMapValueToArrayFilter();
    testMapValueToVectorFilter();
    testHashMapValueToVectorFilter();
    testVectorToSet();
    testRemoveDumplicate();
    testGetListDiffPodArrayList();
    testMapValueToList();
    testHashMapValueToList();
    testSetToArrayArrayList();
    testMapValueToArrayStartIndex();
    testHashMapValueToArrayStartIndex();
    testGetListDiffPodVector();
    testMapValueToArrayFilterEmpty();
    testHashMapValueToArrayFilterEmpty();
    testVectorToSetAppend();
    testRemoveDumplicateNoDup();
    testGetListDiffPodBothEmpty();
    testGetListDiffPodOneEmpty();
    testHashMapKeyToArrayList();
    testHashMapKeyValueToArrayList();
    testTickTimerLoop();
    testTickTimerLoopEnsure();
    testTickTimerOnce();
}

static void testHashMapValueToArray()
{
    HashMap<int, int> hm;
    hm.add(1, 10);
    hm.add(2, 20);
    hm.add(3, 30);
    Array<8, int> arr;
    int count = hashMapValueToArray(hm, arr, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(count, 3, "hashMapValueToArray count==3");
    int sum = 0;
    for (int i = 0; i < count; ++i)
    {
        sum += arr[i];
    }
    FrameUtilityExt3TestImpl_FUE3_EQ(sum, 60, "hashMapValueToArray sum==60");
}

static void testHashMapValueToArrayList()
{
    HashMap<int, int> hm;
    hm.add(10, 100);
    hm.add(20, 200);
    hm.add(30, 300);
    ArrayList<8, int> arr;
    hashMapValueToArray(hm, arr, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(arr.size(), 3, "hashMapValueToArray-ArrayList size==3");
    int sum = 0;
    for (int i = 0; i < arr.size(); ++i)
    {
        sum += arr[i];
    }
    FrameUtilityExt3TestImpl_FUE3_EQ(sum, 600, "hashMapValueToArray-ArrayList sum==600");
}

static void testMapValueToArrayFilter()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 0);
    m.add(3, 30);
    m.add(4, 0);
    m.add(5, 50);
    Array<8, int> arr;
    int count = mapValueToArrayFilter(m, arr, 0, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(count, 3, "mapValueToArrayFilter count==3");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(arr[0] == 10 || arr[1] == 10 || arr[2] == 10, "mapValueToArrayFilter has 10");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(arr[0] == 30 || arr[1] == 30 || arr[2] == 30, "mapValueToArrayFilter has 30");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(arr[0] == 50 || arr[1] == 50 || arr[2] == 50, "mapValueToArrayFilter has 50");
}

static void testHashMapValueToArrayFilter()
{
    HashMap<int, int> hm;
    hm.add(1, 111);
    hm.add(2, -1);
    hm.add(3, 333);
    hm.add(4, -1);
    Array<8, int> arr;
    int count = hashMapValueToArrayFilter(hm, arr, -1, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(count, 2, "hashMapValueToArrayFilter count==2");
    int sum = 0;
    for (int i = 0; i < count; ++i)
    {
        sum += arr[i];
    }
    FrameUtilityExt3TestImpl_FUE3_EQ(sum, 444, "hashMapValueToArrayFilter sum==444");
}

static void testMapValueToVectorFilter()
{
    Map<int, int> m;
    m.add(1, 5);
    m.add(2, 0);
    m.add(3, 7);
    m.add(4, 0);
    Vector<int> vec;
    mapValueToVectorFilter(m, vec, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(vec.size(), 2, "mapValueToVectorFilter size==2");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(vec.contains(5), "mapValueToVectorFilter has 5");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(vec.contains(7), "mapValueToVectorFilter has 7");
}

static void testHashMapValueToVectorFilter()
{
    HashMap<int, int> hm;
    hm.add(1, 100);
    hm.add(2, -999);
    hm.add(3, 200);
    hm.add(4, -999);
    hm.add(5, 300);
    Vector<int> vec;
    hashMapValueToVectorFilter(hm, vec, -999);
    FrameUtilityExt3TestImpl_FUE3_EQ(vec.size(), 3, "hashMapValueToVectorFilter size==3");
    int sum = 0;
    for (int i = 0; i < vec.size(); ++i)
    {
        sum += vec[i];
    }
    FrameUtilityExt3TestImpl_FUE3_EQ(sum, 600, "hashMapValueToVectorFilter sum==600");
}

static void testVectorToSet()
{
    Vector<int> vec;
    vec.add(1);
    vec.add(2);
    vec.add(3);
    Set<int> s;
    vectorToSet(vec, s, false);
    FrameUtilityExt3TestImpl_FUE3_EQ(s.size(), 3, "vectorToSet size==3");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(s.contains(1), "vectorToSet has 1");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(s.contains(2), "vectorToSet has 2");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(s.contains(3), "vectorToSet has 3");

    Vector<int> vec2;
    vec2.add(4);
    vec2.add(5);
    vectorToSet(vec2, s, true);
    FrameUtilityExt3TestImpl_FUE3_EQ(s.size(), 5, "vectorToSet append size==5");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(s.contains(4), "vectorToSet append has 4");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(s.contains(5), "vectorToSet append has 5");
}

static void testRemoveDumplicate()
{
    Vector<int> vec;
    vec.add(3);
    vec.add(1);
    vec.add(2);
    vec.add(1);
    vec.add(3);
    vec.add(4);
    removeDumplicate(vec);
    FrameUtilityExt3TestImpl_FUE3_EQ(vec.size(), 4, "removeDuplicate size==4");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(vec.contains(1), "removeDuplicate has 1");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(vec.contains(2), "removeDuplicate has 2");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(vec.contains(3), "removeDuplicate has 3");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(vec.contains(4), "removeDuplicate has 4");
}

static void testGetListDiffPodArrayList()
{
    int arr0[] = { 1, 2, 3, 4, 5 };
    int arr1[] = { 3, 4, 5, 6, 7 };
    ArrayList<10, int> diff0;
    ArrayList<10, int> diff1;
    getListDiffPod(arr0, 5, arr1, 5, diff0, diff1);
    FrameUtilityExt3TestImpl_FUE3_EQ(diff0.size(), 2, "getListDiffPod diff0 size==2");
    FrameUtilityExt3TestImpl_FUE3_EQ(diff1.size(), 2, "getListDiffPod diff1 size==2");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(diff0.contains(1), "getListDiffPod diff0 has 1");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(diff0.contains(2), "getListDiffPod diff0 has 2");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(diff1.contains(6), "getListDiffPod diff1 has 6");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(diff1.contains(7), "getListDiffPod diff1 has 7");
}

static void testMapValueToList()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    int buf[8] = {};
    int count = mapValueToList(m, buf, 8, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(count, 3, "mapValueToList count==3");
    int sum = 0;
    for (int i = 0; i < count; ++i)
    {
        sum += buf[i];
    }
    FrameUtilityExt3TestImpl_FUE3_EQ(sum, 60, "mapValueToList sum==60");
}

static void testHashMapValueToList()
{
    HashMap<int, int> hm;
    hm.add(1, 100);
    hm.add(2, 200);
    hm.add(3, 300);
    int buf[8] = {};
    int count = hashMapValueToList(hm, buf, 8, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(count, 3, "hashMapValueToList count==3");
    int sum = 0;
    for (int i = 0; i < count; ++i)
    {
        sum += buf[i];
    }
    FrameUtilityExt3TestImpl_FUE3_EQ(sum, 600, "hashMapValueToList sum==600");
}

static void testSetToArrayArrayList()
{
    Set<int> s;
    s.add(10);
    s.add(20);
    s.add(30);
    s.add(40);
    ArrayList<8, int> arr;
    setToArray(s, arr, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(arr.size(), 4, "setToArray-ArrayList size==4");
    int sum = 0;
    for (int i = 0; i < arr.size(); ++i)
    {
        sum += arr[i];
    }
    FrameUtilityExt3TestImpl_FUE3_EQ(sum, 100, "setToArray-ArrayList sum==100");
}

static void testMapValueToArrayStartIndex()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.add(4, 40);
    Array<8, int> arr;
    int count = mapValueToArray(m, arr, 2);
    FrameUtilityExt3TestImpl_FUE3_EQ(count, 2, "mapValueToArray startIndex=2 count==2");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(arr[0] == 30 || arr[0] == 40, "mapValueToArray startIndex=2 val ok");
}

static void testHashMapValueToArrayStartIndex()
{
    HashMap<int, int> hm;
    hm.add(1, 10);
    hm.add(2, 20);
    hm.add(3, 30);
    hm.add(4, 40);
    ArrayList<4, int> arr;
    hashMapValueToArray(hm, arr, 2);
    FrameUtilityExt3TestImpl_FUE3_EQ(arr.size(), 2, "hashMapValueToArray-ArrayList startIndex=2 size==2");
}

static void testGetListDiffPodVector()
{
    int a[] = { 5, 6, 7, 8 };
    int b[] = { 6, 8, 9 };
    Vector<int> d0;
    Vector<int> d1;
    getListDiffPod(a, 4, b, 3, d0, d1);
    FrameUtilityExt3TestImpl_FUE3_EQ(d0.size(), 2, "getListDiffPod vector d0 size==2");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(d0.contains(5), "getListDiffPod vector d0 has 5");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(d0.contains(7), "getListDiffPod vector d0 has 7");
    FrameUtilityExt3TestImpl_FUE3_EQ(d1.size(), 1, "getListDiffPod vector d1 size==1");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(d1.contains(9), "getListDiffPod vector d1 has 9");
}

static void testMapValueToArrayFilterEmpty()
{
    Map<int, int> m;
    Array<8, int> arr;
    int count = mapValueToArrayFilter(m, arr, 0, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(count, 0, "mapValueToArrayFilter empty count==0");
}

static void testHashMapValueToArrayFilterEmpty()
{
    HashMap<int, int> hm;
    Array<8, int> arr;
    int count = hashMapValueToArrayFilter(hm, arr, -1, 0);
    FrameUtilityExt3TestImpl_FUE3_EQ(count, 0, "hashMapValueToArrayFilter empty count==0");
}

static void testVectorToSetAppend()
{
    Vector<int> vec;
    vec.add(10);
    vec.add(20);
    Set<int> s;
    s.add(1);
    s.add(2);
    vectorToSet(vec, s, true);
    FrameUtilityExt3TestImpl_FUE3_EQ(s.size(), 4, "vectorToSet append size==4");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(s.contains(1), "has 1");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(s.contains(2), "has 2");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(s.contains(10), "has 10");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(s.contains(20), "has 20");
}

static void testRemoveDumplicateNoDup()
{
    Vector<int> vec;
    vec.add(100);
    vec.add(200);
    vec.add(300);
    removeDumplicate(vec);
    FrameUtilityExt3TestImpl_FUE3_EQ(vec.size(), 3, "removeDumplicate no-dup size==3");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(vec.contains(100), "has 100");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(vec.contains(200), "has 200");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(vec.contains(300), "has 300");
}

static void testGetListDiffPodBothEmpty()
{
    Vector<int> d0;
    Vector<int> d1;
    getListDiffPod((int*)nullptr, 0, (int*)nullptr, 0, d0, d1);
    FrameUtilityExt3TestImpl_FUE3_EQ(d0.size(), 0, "getListDiffPod both-empty d0==0");
    FrameUtilityExt3TestImpl_FUE3_EQ(d1.size(), 0, "getListDiffPod both-empty d1==0");
}

static void testGetListDiffPodOneEmpty()
{
    int a[] = { 1, 2, 3 };
    Vector<int> d0;
    Vector<int> d1;
    getListDiffPod(a, 3, (int*)nullptr, 0, d0, d1);
    FrameUtilityExt3TestImpl_FUE3_EQ(d0.size(), 3, "getListDiffPod one-empty d0==3");
    FrameUtilityExt3TestImpl_FUE3_EQ(d1.size(), 0, "getListDiffPod one-empty d1==0");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(d0.contains(1), "has 1");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(d0.contains(2), "has 2");
    FrameUtilityExt3TestImpl_FUE3_ASSERT(d0.contains(3), "has 3");
}

static void testHashMapKeyToArrayList()
{
    HashMap<int, string> hm;
    hm.add(11, "aa");
    hm.add(22, "bb");
    hm.add(33, "cc");
    ArrayList<8, int> keys;
    hashMapKeyToArrayList(hm, keys);
    FrameUtilityExt3TestImpl_FUE3_EQ(keys.size(), 3, "hashMapKeyToArrayList size==3");
    int sum = 0;
    for (int i = 0; i < keys.size(); ++i)
    {
        sum += keys[i];
    }
    FrameUtilityExt3TestImpl_FUE3_EQ(sum, 66, "hashMapKeyToArrayList sum==66");
}

static void testHashMapKeyValueToArrayList()
{
    HashMap<int, int> hm;
    hm.add(1, 100);
    hm.add(2, 200);
    hm.add(3, 300);
    ArrayList<8, int> keys;
    ArrayList<8, int> vals;
    hashMapKeyValueToArrayList(hm, keys, vals);
    FrameUtilityExt3TestImpl_FUE3_EQ(keys.size(), 3, "hashMapKeyValueToArrayList keys.size==3");
    FrameUtilityExt3TestImpl_FUE3_EQ(vals.size(), 3, "hashMapKeyValueToArrayList vals.size==3");
    int ksum = 0;
    int vsum = 0;
    for (int i = 0; i < keys.size(); ++i)
    {
        ksum += keys[i];
        vsum += vals[i];
    }
    FrameUtilityExt3TestImpl_FUE3_EQ(ksum, 6, "hashMapKeyValueToArrayList ksum==6");
    FrameUtilityExt3TestImpl_FUE3_EQ(vsum, 600, "hashMapKeyValueToArrayList vsum==600");
}

static void testTickTimerLoop()
{
    // tickTimerLoop 倒计时语义：time=interval 表示从头计时
    // timer=1.0，elapsed=0.5 → timer=0.5>0，不触发
    float timer = 1.0f;
    bool fired = tickTimerLoop(timer, 0.5f, 1.0f);
    FrameUtilityExt3TestImpl_FUE3_ASSERT(!fired, "tickTimerLoop 0.5/1.0 not fired");
    // timer=0.5，elapsed=0.5 → timer=0 ≤ 0，触发，timer += 1.0 = 1.0
    fired = tickTimerLoop(timer, 0.5f, 1.0f);
    FrameUtilityExt3TestImpl_FUE3_ASSERT(fired, "tickTimerLoop fires after 1.0 total");
    // timer=1.0，elapsed=0.01 → timer=0.99>0，不触发
    fired = tickTimerLoop(timer, 0.01f, 1.0f);
    FrameUtilityExt3TestImpl_FUE3_ASSERT(!fired, "tickTimerLoop not fired again so soon");
    // t2=0.0：timer=0，elapsed=2.0 → timer=-2≤0，触发，timer += 1.0=-1≤0，timer=1.0
    float t2 = 0.0f;
    bool fired2 = tickTimerLoop(t2, 2.0f, 1.0f);
    FrameUtilityExt3TestImpl_FUE3_ASSERT(fired2, "tickTimerLoop elapsedTime > interval fires");
}

static void testTickTimerLoopEnsure()
{
    // tickTimerLoopEnsure：触发后 time=interval（不像 Loop 用 +=）
    // timer=1.0，elapsed=0.3 → timer=0.7>0，不触发
    float timer = 1.0f;
    bool r1 = tickTimerLoopEnsure(timer, 0.3f, 1.0f);
    FrameUtilityExt3TestImpl_FUE3_ASSERT(!r1, "tickTimerLoopEnsure 0.3/1.0 not fired");
    // timer=0.7，elapsed=0.8 → timer=-0.1≤0，触发，timer=1.0
    bool r2 = tickTimerLoopEnsure(timer, 0.8f, 1.0f);
    FrameUtilityExt3TestImpl_FUE3_ASSERT(r2, "tickTimerLoopEnsure ensure fired at >=1.0");
}

static void testTickTimerOnce()
{
    // tickTimerOnce：触发后 time=-1，不再触发
    // timer=1.5，elapsed=0.5 → timer=1.0>0，不触发
    float timer = 1.5f;
    bool r1 = tickTimerOnce(timer, 0.5f);
    FrameUtilityExt3TestImpl_FUE3_ASSERT(!r1, "tickTimerOnce 1.5-0.5=1.0 not zero");
    // timer=1.0，elapsed=0.5 → timer=0.5>0，不触发
    bool r2 = tickTimerOnce(timer, 0.5f);
    FrameUtilityExt3TestImpl_FUE3_ASSERT(!r2, "tickTimerOnce 1.0-0.5=0.5 not zero");
    // timer=0.5，elapsed=0.6 → timer=-0.1≤0，触发，time=-1
    float timer2 = 0.5f;
    bool r3 = tickTimerOnce(timer2, 0.6f);
    FrameUtilityExt3TestImpl_FUE3_ASSERT(r3, "tickTimerOnce fires when reaches zero");
}
};

struct FrameUtilityMapKeyTestImpl
{

#define FrameUtilityMapKeyTestImpl_FUMK_ASSERT(expr) \
    if (!(expr)) { ERROR("FrameUtilityMapKeyTest FAILED: " #expr); }

static void testMapKeyToVector()
{
    Map<int, string> m;
    m.add(1, "one");
    m.add(2, "two");
    m.add(3, "three");
    Vector<int> keys;
    mapKeyToVector(m, keys);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys.size() == 3);
    bool has1 = false;
    bool has2 = false;
    bool has3 = false;
    for (int k : keys)
    {
        if (k == 1) { has1 = true; }
        if (k == 2) { has2 = true; }
        if (k == 3) { has3 = true; }
    }
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(has1 && has2 && has3);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys[0] == 1);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys[1] == 2);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys[2] == 3);
}

static void testMapKeyToVectorEmpty()
{
    Map<int, string> m;
    Vector<int> keys;
    keys.add(99);
    mapKeyToVector(m, keys);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys.size() == 1);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys[0] == 99);
}

static void testHashMapKeyToVector()
{
    HashMap<int, string> hm;
    hm.add(10, "ten");
    hm.add(20, "twenty");
    hm.add(30, "thirty");
    Vector<int> keys;
    hashMapKeyToVector(hm, keys);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys.size() == 3);
    bool has10 = false;
    bool has20 = false;
    bool has30 = false;
    for (int k : keys)
    {
        if (k == 10) { has10 = true; }
        if (k == 20) { has20 = true; }
        if (k == 30) { has30 = true; }
    }
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(has10 && has20 && has30);
}

static void testHashMapKeyToVectorEmpty()
{
    HashMap<int, string> hm;
    Vector<int> keys;
    keys.add(55);
    hashMapKeyToVector(hm, keys);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys.size() == 1);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys[0] == 55);
}

static void testHashMapKeyToVectorReturnValue()
{
    HashMap<string, int> hm;
    hm.add("a", 1);
    hm.add("b", 2);
    hm.add("c", 3);
    Vector<string> keys = hashMapKeyToVector(hm);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys.size() == 3);
    HashMap<string, int> emptyHm;
    Vector<string> emptyKeys = hashMapKeyToVector(emptyHm);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(emptyKeys.isEmpty());
}

static void testMapKeyToList()
{
    Map<int, int> m;
    m.add(5, 50);
    m.add(10, 100);
    m.add(15, 150);
    int keyArr[10];
    bool result = mapKeyToList(m, keyArr, 10);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(result);
    bool found5 = false;
    bool found10 = false;
    bool found15 = false;
    for (int i = 0; i < 3; ++i)
    {
        if (keyArr[i] == 5)
        {
            found5 = true;
        }
        if (keyArr[i] == 10)
        {
            found10 = true;
        }
        if (keyArr[i] == 15)
        {
            found15 = true;
        }
    }
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(found5);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(found10);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(found15);
}

static void testMapKeyToListEmpty()
{
    Map<int, int> m;
    int keyArr[10];
    bool result = mapKeyToList(m, keyArr, 10);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(result);
}

static void testMapKeyToListTruncate()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.add(4, 40);
    int keyArr[2];
    bool result = mapKeyToList(m, keyArr, 2);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(!result);
}

static void testHashMapKeyToList()
{
    HashMap<int, float> hm;
    hm.add(100, 1.0f);
    hm.add(200, 2.0f);
    hm.add(300, 3.0f);
    int keyArr[10];
    bool result = hashMapKeyToList(hm, keyArr, 10);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(result);
}

static void testHashMapKeyToListEmpty()
{
    HashMap<int, float> hm;
    int keyArr[10];
    bool result = hashMapKeyToList(hm, keyArr, 10);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(result);
}

static void testHashMapKeyToListTruncate()
{
    HashMap<int, int> hm;
    hm.add(1, 10);
    hm.add(2, 20);
    hm.add(3, 30);
    hm.add(4, 40);
    hm.add(5, 50);
    int keyArr[3];
    bool result = hashMapKeyToList(hm, keyArr, 3);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(!result);
}

static void testMapValueToVector()
{
    Map<int, int> m;
    m.add(1, 100);
    m.add(2, 200);
    m.add(3, 300);
    Vector<int> values;
    mapValueToVector(m, values);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(values.size() == 3);
    bool has100 = false;
    bool has200 = false;
    bool has300 = false;
    for (int v : values)
    {
        if (v == 100) 
        {
            has100 = true; 
        }
        if (v == 200) 
        {
            has200 = true; 
        }
        if (v == 300) 
        {
            has300 = true; 
        }
    }
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(has100 && has200 && has300);
}

static void testMapValueToVectorEmpty()
{
    Map<int, int> m;
    Vector<int> values;
    values.add(77);
    mapValueToVector(m, values);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(values.size() == 1);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(values[0] == 77);
}

static void testHashMapValueToVector()
{
    HashMap<string, int> hm;
    hm.add("x", 10);
    hm.add("y", 20);
    hm.add("z", 30);
    Vector<int> values;
    hashMapValueToVector(hm, values);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(values.size() == 3);
    bool has10 = false;
    bool has20 = false;
    bool has30 = false;
    for (int v : values)
    {
        if (v == 10) 
        {
            has10 = true; 
        }
        if (v == 20) 
        {
            has20 = true; 
        }
        if (v == 30) 
        {
            has30 = true; 
        }
    }
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(has10 && has20 && has30);
}

static void testHashMapValueToVectorEmpty()
{
    HashMap<string, int> hm;
    Vector<int> values;
    values.add(999);
    hashMapValueToVector(hm, values);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(values.size() == 1);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(values[0] == 999);
}

static void testHashMapToVector()
{
    HashMap<int, string> hm;
    hm.add(1, "one");
    hm.add(2, "two");
    hm.add(3, "three");
    Vector<pair<int, string>> pairs;
    hashMapToVector(hm, pairs);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(pairs.size() == 3);
    for (int i = 0; i < (int)pairs.size(); ++i)
    {
        FrameUtilityMapKeyTestImpl_FUMK_ASSERT(pairs[i].first >= 1 && pairs[i].first <= 3);
        FrameUtilityMapKeyTestImpl_FUMK_ASSERT(!pairs[i].second.empty());
    }
}

static void testHashMapToVectorEmpty()
{
    HashMap<int, string> hm;
    Vector<pair<int, string>> pairs;
    pairs.add({ 99, "pre" });
    hashMapToVector(hm, pairs);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(pairs.size() == 1);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(pairs[0].first == 99);
}

static void testMapKeyToVectorMultiTypes()
{
    Map<string, int> m;
    m.add("alpha", 1);
    m.add("beta", 2);
    m.add("gamma", 3);
    Vector<string> keys;
    mapKeyToVector(m, keys);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys.size() == 3);
    bool hasAlpha = false;
    bool hasBeta = false;
    bool hasGamma = false;
    for (const string& k : keys)
    {
        if (k == "alpha") 
        {
            hasAlpha = true; 
        }
        if (k == "beta") 
        {
            hasBeta = true; 
        }
        if (k == "gamma") 
        {
            hasGamma = true; 
        }
    }
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(hasAlpha && hasBeta && hasGamma);
}

static void testHashMapKeyToVectorMultiTypes()
{
    HashMap<string, float> hm;
    hm.add("pi", 3.14f);
    hm.add("e", 2.71f);
    Vector<string> keys;
    hashMapKeyToVector(hm, keys);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(keys.size() == 2);
    bool hasPi = false;
    bool hasE = false;
    for (const string& k : keys)
    {
        if (k == "pi")
        {
            hasPi = true; 
        }
        if (k == "e") 
        {
            hasE = true; 
        }
    }
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(hasPi && hasE);
}

static void testMapValueToVectorString()
{
    Map<int, string> m;
    m.add(1, "first");
    m.add(2, "second");
    m.add(3, "third");
    Vector<string> values;
    mapValueToVector(m, values);
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(values.size() == 3);
    bool hasFirst = false;
    bool hasSecond = false;
    bool hasThird = false;
    for (const string& v : values)
    {
        if (v == "first") { hasFirst = true; }
        if (v == "second") { hasSecond = true; }
        if (v == "third") { hasThird = true; }
    }
    FrameUtilityMapKeyTestImpl_FUMK_ASSERT(hasFirst && hasSecond && hasThird);
}

static void test()
{
    testMapKeyToVector();
    testMapKeyToVectorEmpty();
    testHashMapKeyToVector();
    testHashMapKeyToVectorEmpty();
    testHashMapKeyToVectorReturnValue();
    testMapKeyToList();
    testMapKeyToListEmpty();
    testMapKeyToListTruncate();
    testHashMapKeyToList();
    testHashMapKeyToListEmpty();
    testHashMapKeyToListTruncate();
    testMapValueToVector();
    testMapValueToVectorEmpty();
    testHashMapValueToVector();
    testHashMapValueToVectorEmpty();
    testHashMapToVector();
    testHashMapToVectorEmpty();
    testMapKeyToVectorMultiTypes();
    testHashMapKeyToVectorMultiTypes();
    testMapValueToVectorString();
}
};

struct FrameUtilityTimerTestImpl
{

#define FrameUtilityTimerTestImpl_FUTT_ASSERT(condition) if (!(condition)) { ERROR("FrameUtilityTimerTest failed: " #condition); }

static void test()
{
    testTickTimerLoopNotFired();
    testTickTimerLoopFired();
    testTickTimerLoopExact();
    testTickTimerLoopMultiple();
    testTickTimerLoopReset();
    testTickTimerLoopEnsureBasic();
    testTickTimerLoopEnsureAccumulate();
    testTickTimerOnceNotFired();
    testTickTimerOnceFired();
    testTickTimerOnceExact();
    testTickTimerOnceZeroTime();
    testTickTimerOnceMultipleFrames();
    testTickTimerLoopZeroInterval();
    testTickTimerLoopEnsureZeroInterval();
}

// tickTimerLoop 倒计时语义：time = 距离下次触发的剩余时间
// time=0 时首次调用会立即触发；正确初始值是 time=interval

static void testTickTimerLoopNotFired()
{
    // time=1.0（还剩1秒），elapsed=0.5 → time=0.5>0，不触发
    float time = 1.0f;
    bool result = tickTimerLoop(time, 0.5f, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(!result);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 0.5f) < 1e-4f);
}

static void testTickTimerLoopFired()
{
    // time=0（已到时），elapsed=1.0 → time=-1.0 ≤ 0，触发
    // time += 1.0 = 0，但 time <= 0 导致重置为 interval = 1.0
    float time = 0.0f;
    bool result = tickTimerLoop(time, 1.0f, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(result);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 1.0f) < 1e-4f);
}

static void testTickTimerLoopExact()
{
    // time=0.2f，elapsed=0.2f → time=0，不 > 0，触发；time += 1.0 = 1.0
    float time = 0.2f;
    bool result = tickTimerLoop(time, 0.2f, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(result);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 1.0f) < 1e-4f);
}

static void testTickTimerLoopMultiple()
{
    // time=1.0（从头开始），interval=1.0
    // step1: elapsed=0.3 → time=0.7>0，不触发
    // step2: elapsed=0.4 → time=0.3>0，不触发
    // step3: elapsed=0.4 → time=-0.1 ≤ 0，触发，time += 1.0 = 0.9
    float time = 1.0f;
    int fired = 0;
    fired += tickTimerLoop(time, 0.3f, 1.0f) ? 1 : 0;
    fired += tickTimerLoop(time, 0.4f, 1.0f) ? 1 : 0;
    fired += tickTimerLoop(time, 0.4f, 1.0f) ? 1 : 0;
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fired == 1);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 0.9f) < 1e-4f);
}

static void testTickTimerLoopReset()
{
    // time=0.5f，elapsed=0.5f → time=0 ≤ 0，触发，time += 1.0 = 1.0
    float time = 0.5f;
    bool r1 = tickTimerLoop(time, 0.5f, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(r1);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 1.0f) < 1e-4f);
    // 触发后 time=1.0，再 elapsed=0.5 → time=0.5>0，不触发
    bool result = tickTimerLoop(time, 0.5f, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(!result);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 0.5f) < 1e-4f);
}

// tickTimerLoopEnsure：触发后 time=interval（不加回，直接重置）

static void testTickTimerLoopEnsureBasic()
{
    // time=1.0，elapsed=0.5 → time=0.5>0，不触发
    float time = 1.0f;
    bool result = tickTimerLoopEnsure(time, 0.5f, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(!result);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 0.5f) < 1e-4f);

    // time=0.5，elapsed=0.5 → time=0 ≤ 0，触发，time=1.0
    bool result2 = tickTimerLoopEnsure(time, 0.5f, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(result2);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 1.0f) < 1e-4f);
}

static void testTickTimerLoopEnsureAccumulate()
{
    // time=1.0，elapsed=0.6 → time=0.4>0，不触发
    float time = 1.0f;
    bool r1 = tickTimerLoopEnsure(time, 0.6f, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(!r1);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 0.4f) < 1e-4f);
    // time=0.4，elapsed=0.6 → time=-0.2 ≤ 0，触发，time=1.0（EnsureLoop 直接重置为 interval）
    bool r2 = tickTimerLoopEnsure(time, 0.6f, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(r2);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 1.0f) < 1e-4f);
}

// tickTimerOnce：触发后 time=-1（不再触发）

static void testTickTimerOnceNotFired()
{
    // time=0.5f（剩余0.5），elapsed=0.3 → time=0.2>0，不触发
    float time = 0.5f;
    bool result = tickTimerOnce(time, 0.3f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(!result);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 0.2f) < 1e-4f);
}

static void testTickTimerOnceFired()
{
    // time=0.3f，elapsed=0.5 → time=-0.2 ≤ 0，触发，time=-1
    float time = 0.3f;
    bool result = tickTimerOnce(time, 0.5f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(result);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - (-1.0f)) < 1e-4f);
}

static void testTickTimerOnceExact()
{
    // time=1.0，elapsed=1.0 → time=0 ≤ 0，触发，time=-1
    float time = 1.0f;
    bool result = tickTimerOnce(time, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(result);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - (-1.0f)) < 1e-4f);
}

static void testTickTimerOnceZeroTime()
{
    // time=0.0f（已到时），elapsed=0.1 → time=-0.1 ≤ 0，触发，time=-1
    float time = 0.0f;
    bool result = tickTimerOnce(time, 0.1f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(result);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - (-1.0f)) < 1e-4f);
}

static void testTickTimerOnceMultipleFrames()
{
    // time=2.0（等待2秒触发）
    // frame1: elapsed=1.0 → time=1.0>0，不触发
    float time = 2.0f;
    bool r1 = tickTimerOnce(time, 1.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(!r1);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 1.0f) < 1e-4f);

    // frame2: elapsed=0.5 → time=0.5>0，不触发
    bool r2 = tickTimerOnce(time, 0.5f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(!r2);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - 0.5f) < 1e-4f);

    // frame3: elapsed=0.5 → time=0 ≤ 0，触发，time=-1
    bool r3 = tickTimerOnce(time, 0.5f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(r3);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(fabsf(time - (-1.0f)) < 1e-4f);
}

static void testTickTimerLoopZeroInterval()
{
    // interval=0：触发后 time += 0 = 仍然 ≤ 0，设为 interval=0
    // 即：time=0, elapsed=0.1 → time=-0.1 ≤ 0，触发，time+= 0=-0.1 ≤ 0，time=0
    float time = 0.0f;
    bool result = tickTimerLoop(time, 0.1f, 0.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(result);
}

static void testTickTimerLoopEnsureZeroInterval()
{
    // interval=0：time=0, elapsed=0.1 → time=-0.1 ≤ 0，触发，time=0
    float time = 0.0f;
    bool result = tickTimerLoopEnsure(time, 0.1f, 0.0f);
    FrameUtilityTimerTestImpl_FUTT_ASSERT(result);
}
};

struct FrameUtilityVectorFilterTestImpl
{

#define FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(expr) \
    do { if (!(expr)) { ERROR("FrameUtilityVectorFilterTest FAIL: " #expr); } } while(0)

static void test()
{
    testMapValueToVectorFilterBasic();
    testMapValueToVectorFilterNone();
    testMapValueToVectorFilterAll();
    testHashMapValueToVectorFilterBasic();
    testHashMapValueToVectorFilterNone();
    testHashMapValueToVectorFilterAll();
    testSetToVectorBasic();
    testSetToVectorEmpty();
    testSetToVectorIntType();
    testVectorToSetBasic();
    testVectorToSetDuplicates();
    testVectorToSetEmpty();
    testRemoveDumplicateIntVector();
    testRemoveDumplicateStringVector();
    testRemoveDumplicateNoDuplicates();
    testRemoveDumplicateAllSame();
    testGetListDiffBasic();
    testGetListDiffEmpty();
    testGetListDiffNoChange();
    testGetListDiffAddOnly();
    testGetListDiffRemoveOnly();
    testHashMapValueToArrayListFilter();
    testMapValueToArrayFilterStartIndex();
}

static void testMapValueToVectorFilterBasic()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 0);
    Vector<int> result;
    mapValueToVectorFilter(m, result, 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 2);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.contains(10));
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.contains(20));
}

static void testMapValueToVectorFilterNone()
{
    Map<int, int> m;
    m.add(1, 5);
    m.add(2, 8);
    Vector<int> result;
    mapValueToVectorFilter(m, result, 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 2);
}

static void testMapValueToVectorFilterAll()
{
    Map<int, int> m;
    m.add(1, 0);
    m.add(2, 0);
    m.add(3, 0);
    Vector<int> result;
    mapValueToVectorFilter(m, result, 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 0);
}

static void testHashMapValueToVectorFilterBasic()
{
    HashMap<int, int> m;
    m.add(1, 100);
    m.add(2, 200);
    m.add(3, 0);
    Vector<int> result;
    hashMapValueToVectorFilter(m, result, 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 2);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.contains(100));
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.contains(200));
}

static void testHashMapValueToVectorFilterNone()
{
    HashMap<int, int> m;
    m.add(1, 1);
    m.add(2, 2);
    Vector<int> result;
    hashMapValueToVectorFilter(m, result, 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 2);
}

static void testHashMapValueToVectorFilterAll()
{
    HashMap<int, int> m;
    m.add(10, 0);
    m.add(20, 0);
    m.add(30, 0);
    Vector<int> result;
    hashMapValueToVectorFilter(m, result, 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 0);
}

static void testSetToVectorBasic()
{
    Set<int> s;
    s.add(1);
    s.add(2);
    s.add(3);
    Vector<int> result;
    setToVector(s, result);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 3);
}

static void testSetToVectorEmpty()
{
    Set<int> s;
    Vector<int> result;
    setToVector(s, result);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 0);
}

static void testSetToVectorIntType()
{
    Set<int> s;
    s.add(10);
    s.add(20);
    Vector<int> result;
    setToVector(s, result);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 2);
    int sum = 0;
    FOR_VECTOR(result)
    {
        sum += result[i];
    }
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(sum == 30);
}

static void testVectorToSetBasic()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    v.add(3);
    Set<int> result;
    vectorToSet(v, result, false);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 3);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.find(1) != result.end());
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.find(2) != result.end());
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.find(3) != result.end());
}

static void testVectorToSetDuplicates()
{
    Vector<int> v;
    v.add(5);
    v.add(5);
    v.add(5);
    Set<int> result;
    vectorToSet(v, result, false);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 1);
}

static void testVectorToSetEmpty()
{
    Vector<int> v;
    Set<int> result;
    vectorToSet(v, result, false);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 0);
}

static void testRemoveDumplicateIntVector()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    v.add(1);
    v.add(3);
    v.add(2);
    removeDumplicate(v);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(v.size() == 3);
}

static void testRemoveDumplicateStringVector()
{
    Vector<string> v;
    v.add("a");
    v.add("b");
    v.add("a");
    removeDumplicate(v);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(v.size() == 2);
}

static void testRemoveDumplicateNoDuplicates()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    v.add(3);
    removeDumplicate(v);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(v.size() == 3);
}

static void testRemoveDumplicateAllSame()
{
    Vector<int> v;
    v.add(7);
    v.add(7);
    v.add(7);
    removeDumplicate(v);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(v.size() == 1);
}

static void testGetListDiffBasic()
{
    Vector<int> oldList;
    oldList.add(1);
    oldList.add(2);
    oldList.add(3);
    Vector<int> newList;
    newList.add(2);
    newList.add(3);
    newList.add(4);
    Vector<int> addList;
    Vector<int> removeList;
    getListDiffPod(oldList.data(), oldList.size(), newList.data(), newList.size(), removeList, addList);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(addList.size() == 1);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(addList[0] == 4);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(removeList.size() == 1);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(removeList[0] == 1);
}

static void testGetListDiffEmpty()
{
    Vector<int> empty1;
    Vector<int> empty2;
    Vector<int> addList;
    Vector<int> removeList;
    getListDiffPod(empty1.data(), empty1.size(), empty2.data(), empty2.size(), addList, removeList);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(addList.size() == 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(removeList.size() == 0);
}

static void testGetListDiffNoChange()
{
    Vector<int> oldList;
    oldList.add(1);
    oldList.add(2);
    Vector<int> newList;
    newList.add(1);
    newList.add(2);
    Vector<int> addList;
    Vector<int> removeList;
    getListDiffPod(oldList.data(), oldList.size(), newList.data(), newList.size(), addList, removeList);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(addList.size() == 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(removeList.size() == 0);
}

static void testGetListDiffAddOnly()
{
    Vector<int> oldList;
    oldList.add(1);
    Vector<int> newList;
    newList.add(1);
    newList.add(2);
    newList.add(3);
    Vector<int> addList;
    Vector<int> removeList;
    getListDiffPod(oldList.data(), oldList.size(), newList.data(), newList.size(), removeList, addList);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(addList.size() == 2);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(removeList.size() == 0);
}

static void testGetListDiffRemoveOnly()
{
    Vector<int> oldList;
    oldList.add(1);
    oldList.add(2);
    oldList.add(3);
    Vector<int> newList;
    newList.add(2);
    Vector<int> addList;
    Vector<int> removeList;
    getListDiffPod(oldList.data(), oldList.size(), newList.data(), newList.size(), removeList, addList);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(addList.size() == 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(removeList.size() == 2);
}

static void testHashMapValueToArrayListFilter()
{
    HashMap<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.add(4, 0);
    Vector<int> result;
    hashMapValueToVectorFilter(m, result, 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(result.size() == 3);
}

static void testMapValueToArrayFilterStartIndex()
{
    Map<int, int> m;
    m.add(1, 1);
    m.add(2, 2);
    m.add(3, 3);
    m.add(4, 0);
    Vector<int> arr;
    mapValueToVectorFilter(m, arr, 0);
    FrameUtilityVectorFilterTestImpl_FUVF_ASSERT(arr.size() == 3);
}
};

void FrameUtilityTest::test()
{
	FrameUtilityTestImpl::test();
	FrameUtilityArrayTestImpl::test();
	FrameUtilityDiffTestImpl::test();
	FrameUtilityExt2TestImpl::test();
	FrameUtilityExt3TestImpl::test();
	FrameUtilityMapKeyTestImpl::test();
	FrameUtilityTimerTestImpl::test();
	FrameUtilityVectorFilterTestImpl::test();
}
