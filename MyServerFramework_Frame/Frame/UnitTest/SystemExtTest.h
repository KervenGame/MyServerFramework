#pragma once
#include "FrameBase.h"
class MICRO_LEGEND_FRAME_API SystemExtTest
{
public:
    static void test();
private:
    static void testTodayBeginEnd1();
    static void testGetDayOfWeek2();
    static void testGetTimeHourInDay2();
    static void testGetTimeFullWithWeekDay();
    static void testGetTimeSixParams();
    static void testGetTimeString2();
    static void testGetCPUCoreCount2();
    static void testGetPID2();
    static void testGetRealTimeMS2();
    static void testSleep();
    static void testGetProgramFile2();
    static void testMakeIDThreadExtended();
    static void testDaysToSecondsExtended();
    static void testIsLeapYearExtended();
    static void testConvertToTimestampExtended();
    static void testGetTimeStruct();
    static void testIsSameDayExtended();
    static void testGetDaysInMonthCurrent();
    static void testTimeUTCSeed();
    static void testMainThreadExtended();
    static void testTimePipeline();
    static void testMakeIDStress();
};
