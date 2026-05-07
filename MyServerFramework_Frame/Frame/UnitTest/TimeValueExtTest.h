#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API TimeValueExtTest
{
public:
    static void test();
private:
    static void testTimeValueClear();
    static void testTimeValueOperatorGreater();
    static void testIsTimeInRangeInHourBasic();
    static void testIsTimeInRangeInHourEdge();
    static void testIsTimeInRangeInHourOutside();
    static void testIsTimeInRangeInDayBasic();
    static void testIsTimeInRangeInDayEdge();
    static void testIsTimeInRangeInDayOutside();
    static void testIsTimeInRangeInWeekBasic();
    static void testIsTimeInRangeInWeekEdge();
    static void testIsTimeInRangeInWeekOutside();
    static void testIsTimeInRangeInMonthBasic();
    static void testIsTimeInRangeInMonthEdge();
    static void testIsTimeInRangeInMonthOutside();
    static void testIsTimeInRangeInYearBasic();
    static void testIsTimeInRangeInYearEdge();
    static void testIsTimeInRangeInYearOutside();
    static void testTimeValueDefaultInit();
    static void testOperatorGreaterEdge();
    static void testOperatorGreaterSameValues();
};
