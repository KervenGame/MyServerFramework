#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API SafeMapTest
{
public:
	static void test();
private:
	static void testSafeMapEmpty();
	static void testSafeMapInsert();
	static void testSafeMapInsertDuplicate();
	static void testSafeMapErase();
	static void testSafeMapEraseNotExist();
	static void testSafeMapContains();
	static void testSafeMapTryGet();
	static void testSafeMapGetPtr();
	static void testSafeMapSize();
	static void testSafeMapClear();
	static void testSafeMapForeachEmpty();
	static void testSafeMapForeachBasic();
	static void testSafeMapInsertDuringForeach();
	static void testSafeMapEraseDuringForeach();
	static void testSafeMapModifyBetweenForeach();
	static void testSafeMapStringKey();
	static void testSafeMapIntKey();
	static void testSafeMapLargeCount();
	static void testSafeMapMultiRound();
	static void testSafeMapGetMainList();
	static void testSafeMapNotForeachingAfterEnd();
	static void testSafeMapForeachAllRead();
	static void testSafeMapAddThenEraseInForeach();
	static void testSafeMapChainInsertErase();
};
