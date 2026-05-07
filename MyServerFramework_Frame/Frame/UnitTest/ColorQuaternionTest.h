#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API ColorQuaternionTest
{
public:
	static void test();
private:
	// Color tests
	static void testColorDefault();
	static void testColorConstruct();
	static void testColorAdd();
	static void testColorSub();
	static void testColorAddAssign();
	static void testColorSubAssign();
	static void testColorEqual();
	static void testColorNotEqual();
	static void testColorStaticConstants();
	static void testColorChain();
	static void testColorBoundary();
	static void testColorInVector();
	static void testColorWrap();
	static void testColorAlpha();
	static void testColorGrayscale();
	// Quaternion tests
	static void testQuaternionDefault();
	static void testQuaternionConstruct();
	static void testQuaternionClear();
	static void testQuaternionConjugate();
	static void testQuaternionLength();
	static void testQuaternionNormalize();
	static void testQuaternionMulScalar();
	static void testQuaternionDivScalar();
	static void testQuaternionAdd();
	static void testQuaternionNeg();
	static void testQuaternionMulAssign();
	static void testQuaternionDivAssign();
	static void testQuaternionAddAssign();
	static void testQuaternionDot();
	static void testQuaternionLerp();
	static void testQuaternionFromAxisAngle();
	static void testQuaternionEulerAngles();
	static void testQuaternionMul();
	static void testQuaternionCross();
	static void testQuaternionIdentity();
	static void testQuaternionStressNormalize();
};
