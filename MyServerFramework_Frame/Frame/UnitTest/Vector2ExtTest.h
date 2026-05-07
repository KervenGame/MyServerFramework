#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API Vector2ExtTest
{
public:
    static void test();
private:
    static void testVector2DefaultConstruct();
    static void testVector2FloatConstruct();
    static void testVector2IntConstruct();
    static void testVector2Clear();
    static void testVector2Arithmetic();
    static void testVector2AssignOps();
    static void testVector2Negate();
    static void testVector2Equality();
    static void testVector2StaticConstants();
    static void testVector2IntDefaultConstruct();
    static void testVector2IntArithmetic();
    static void testVector2IntAssignOps();
    static void testVector2IntNegate();
    static void testVector2IntEquality();
    static void testVector2IntLess();
    static void testVector2IntClear();
    static void testVector2ShortDefaultConstruct();
    static void testVector2ShortArithmetic();
    static void testVector2ShortEquality();
    static void testVector2UShortBasic();
    static void testVector2UIntBasic();
};
