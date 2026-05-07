#pragma once
#include "FrameComponent.h"
#include "Utility.h"

class MICRO_LEGEND_FRAME_API IsPODTypeTraitsTest
{
public:
	static void test();
private:
	static void testIsPodPrimitives();
	static void testIsPodNonPrimitive();
	static void testIsPodTypeWithEnum();
	static void testIsPodTypeWithConst();
	static void testIsPointerPrimitive();
	static void testIsPointerNonPointer();
	static void testIsPointerNull();
	static void testIsPodOrPointerType();
	static void testIsNotPodAndPointerType();
	static void testIsPodIntegerIntegers();
	static void testIsPodIntegerFloats();
	static void testIsPodIntegerBool();
	static void testIsPodSignedIntegers();
	static void testIsPodUnsignedIntegers();
	static void testToUnsignedIntegerMapping();
	static void testIsPositiveNonNegative();
	static void testIsSubClassOf();
	static void testCompileTimeEnableIf();
	static void testMixedTypeChecks();
};
