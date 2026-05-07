#pragma once
#include "FrameComponent.h"
#include "Utility.h"

class MICRO_LEGEND_FRAME_API SafeContainerTest
{
public:
	static void test();
private:
	// SafeHashMap
	static void testSafeHashMapBasicInsert();
	static void testSafeHashMapBasicErase();
	static void testSafeHashMapContains();
	static void testSafeHashMapTryGet();
	static void testSafeHashMapGetPtr();
	static void testSafeHashMapClear();
	static void testSafeHashMapInsertDuringForeach();
	static void testSafeHashMapEraseDuringForeach();
	static void testSafeHashMapInsertAndEraseDuringForeach();
	static void testSafeHashMapLargeModifyDuringForeach();
	static void testSafeHashMapForeachEmpty();
	static void testSafeHashMapSizeAndEmpty();
	static void testSafeHashMapMainListAccess();
	static void testSafeHashMapEraseWithValue();
	// SafeMap
	static void testSafeMapBasicInsert();
	static void testSafeMapBasicErase();
	static void testSafeMapContains();
	static void testSafeMapTryGet();
	static void testSafeMapClear();
	static void testSafeMapInsertDuringForeach();
	static void testSafeMapEraseDuringForeach();
	static void testSafeMapSizeAndEmpty();
	// SafeSet
	static void testSafeSetBasicInsert();
	static void testSafeSetBasicErase();
	static void testSafeSetContains();
	static void testSafeSetClear();
	static void testSafeSetInsertDuringForeach();
	static void testSafeSetEraseDuringForeach();
	static void testSafeSetSizeAndEmpty();
	static void testSafeSetInsertAndEraseDuringForeach();
};
