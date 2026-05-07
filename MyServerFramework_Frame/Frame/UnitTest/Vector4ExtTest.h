#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API Vector4ExtTest
{
public:
	static void test();
private:
	// Vector3Int tests
	static void testVector3IntDefault();
	static void testVector3IntConstruct();
	static void testVector3IntClear();
	static void testVector3IntAdd();
	static void testVector3IntSub();
	static void testVector3IntMulScalar();
	static void testVector3IntAddAssign();
	static void testVector3IntSubAssign();
	static void testVector3IntNegate();
	static void testVector3IntEqual();
	static void testVector3IntNotEqual();
	static void testVector3IntLess();
	// Vector4 tests
	static void testVector4Default();
	static void testVector4Construct();
	static void testVector4Clear();
	static void testVector4Add();
	static void testVector4Sub();
	static void testVector4AddAssign();
	static void testVector4SubAssign();
	static void testVector4Negate();
	// Vector4Int tests
	static void testVector4IntDefault();
	static void testVector4IntConstruct();
	static void testVector4IntClear();
	static void testVector4IntAdd();
	static void testVector4IntSub();
	static void testVector4IntAddAssign();
	static void testVector4IntSubAssign();
	static void testVector4IntNegate();
	static void testVector4IntMulScalar();
	static void testVector4IntEqual();
	static void testVector4IntNotEqual();
	static void testVector4IntLess();
	static void testVector4IntChain();
};
