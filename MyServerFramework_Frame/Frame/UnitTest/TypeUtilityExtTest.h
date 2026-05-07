#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API TypeUtilityExtTest
{
public:
	static void test();
private:
	static void testIsTypeBasic();
	static void testIsTypeWithConst();
	static void testIsTypeWithRef();
	static void testIsTypeDifferent();
	static void testIsSubClassTrue();
	static void testIsSubClassFalse();
	static void testIsPODTypeTrue();
	static void testIsPODTypeFalse();
	static void testIsSignedIntegerTrue();
	static void testIsSignedIntegerFalse();
	static void testIsUnsignedIntegerTrue();
	static void testIsUnsignedIntegerFalse();
	static void testIsTypeMixedPrimitive();
};
