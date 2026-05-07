#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API StreamBufferSwapLinearizeTest
{
public:
	static void test();
private:
	static void testLinearize_AlreadyContiguous();
	static void testLinearize_Empty();
	static void testLinearize_SingleWrap();
	static void testLinearize_MultiWrapThenLinearize();
	static void testLinearize_FullWrap();
	static void testLinearize_OneByteAtBoundary();
	static void testGetContiguousFreeLength();
};
