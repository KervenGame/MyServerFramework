#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API Vector3ExtTest
{
public:
    static void test();
private:
    static void testDefaultConstruct();
    static void testFloatConstruct();
    static void testIntConstruct();
    static void testClear();
    static void testOperatorAdd();
    static void testOperatorSub();
    static void testOperatorMulScalar();
    static void testOperatorDivScalar();
    static void testOperatorAddAssign();
    static void testOperatorSubAssign();
    static void testOperatorNegate();
    static void testOperatorEqual();
    static void testOperatorNotEqual();
    static void testStaticZero();
    static void testStaticOne();
    static void testStaticForward();
    static void testStaticBack();
    static void testStaticLeft();
    static void testStaticRight();
    static void testStaticUp();
    static void testStaticDown();
    static void testChainOps();
    static void testFloatPrecision();
};
