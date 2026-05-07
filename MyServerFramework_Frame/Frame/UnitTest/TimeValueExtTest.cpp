#include "FrameHeader.h"

#define TVE_ASSERT(expr) \
    if (!(expr)) { ERROR("TimeValueExtTest FAILED: " #expr); }

static TimeValue makeTV(int weekDay, int year, int month, int day, int hour, int minute, int second)
{
    TimeValue tv;
    tv.mWeekDay = weekDay;
    tv.mYear = year;
    tv.mMonth = month;
    tv.mDay = day;
    tv.mHour = hour;
    tv.mMinute = minute;
    tv.mSecond = second;
    return tv;
}

void TimeValueExtTest::testTimeValueClear()
{
    TimeValue tv = makeTV(3, 2024, 6, 15, 10, 30, 45);
    tv.clear();
    TVE_ASSERT(tv.mWeekDay == 0);
    TVE_ASSERT(tv.mYear == 0);
    TVE_ASSERT(tv.mMonth == 0);
    TVE_ASSERT(tv.mDay == 0);
    TVE_ASSERT(tv.mHour == 0);
    TVE_ASSERT(tv.mMinute == 0);
    TVE_ASSERT(tv.mSecond == 0);
}

void TimeValueExtTest::testTimeValueDefaultInit()
{
    TimeValue tv;
    TVE_ASSERT(tv.mYear == 0);
    TVE_ASSERT(tv.mMonth == 0);
    TVE_ASSERT(tv.mDay == 0);
    TVE_ASSERT(tv.mHour == 0);
    TVE_ASSERT(tv.mMinute == 0);
    TVE_ASSERT(tv.mSecond == 0);
    TVE_ASSERT(tv.mWeekDay == 0);
}

void TimeValueExtTest::testTimeValueOperatorGreater()
{
    TimeValue t1 = makeTV(0, 2024, 6, 15, 10, 0, 0);
    TimeValue t2 = makeTV(0, 2024, 6, 15, 9, 0, 0);
    TVE_ASSERT(t1 > t2);
    TVE_ASSERT(!(t2 > t1));
    TimeValue t3 = makeTV(0, 2025, 1, 1, 0, 0, 0);
    TimeValue t4 = makeTV(0, 2024, 12, 31, 23, 59, 59);
    TVE_ASSERT(t3 > t4);
    TVE_ASSERT(!(t4 > t3));
    TimeValue t5 = makeTV(0, 2024, 6, 16, 0, 0, 0);
    TimeValue t6 = makeTV(0, 2024, 6, 15, 23, 59, 59);
    TVE_ASSERT(t5 > t6);
    TimeValue t7 = makeTV(0, 2024, 6, 15, 10, 31, 0);
    TimeValue t8 = makeTV(0, 2024, 6, 15, 10, 30, 0);
    TVE_ASSERT(t7 > t8);
    TimeValue t9 = makeTV(0, 2024, 6, 15, 10, 30, 46);
    TimeValue t10 = makeTV(0, 2024, 6, 15, 10, 30, 45);
    TVE_ASSERT(t9 > t10);
}

void TimeValueExtTest::testOperatorGreaterEdge()
{
    TimeValue t1 = makeTV(0, 2024, 1, 1, 0, 0, 1);
    TimeValue t2 = makeTV(0, 2024, 1, 1, 0, 0, 0);
    TVE_ASSERT(t1 > t2);
    TVE_ASSERT(!(t2 > t1));
}

void TimeValueExtTest::testOperatorGreaterSameValues()
{
    TimeValue t1 = makeTV(0, 2024, 6, 15, 10, 30, 0);
    TimeValue t2 = makeTV(0, 2024, 6, 15, 10, 30, 0);
    TVE_ASSERT(!(t1 > t2));
    TVE_ASSERT(!(t2 > t1));
}

void TimeValueExtTest::testIsTimeInRangeInHourBasic()
{
    TimeValue t = makeTV(0, 0, 0, 0, 0, 30, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 0, 59, 59);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(t, minT, maxT));
    TimeValue tAtMin = makeTV(0, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(0, 0, 0, 0, 0, 59, 59);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(tAtMax, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInHourEdge()
{
    TimeValue t = makeTV(0, 0, 0, 0, 5, 30, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 0, 30, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 0, 30, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(t, minT, maxT));
    TimeValue minOnly = makeTV(0, 0, 0, 0, 0, 15, 0);
    TimeValue maxOnly = makeTV(0, 0, 0, 0, 0, 45, 0);
    TimeValue inside = makeTV(0, 0, 0, 0, 0, 30, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(inside, minOnly, maxOnly));
}

void TimeValueExtTest::testIsTimeInRangeInHourOutside()
{
    TimeValue t = makeTV(0, 0, 0, 0, 0, 5, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 0, 10, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 0, 50, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInHour(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 0, 0, 55, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInHour(t2, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInDayBasic()
{
    TimeValue t = makeTV(0, 0, 0, 0, 12, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 9, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInDay(t, minT, maxT));
    TimeValue tAtMin = makeTV(0, 0, 0, 0, 9, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInDay(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(0, 0, 0, 0, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInDay(tAtMax, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInDayEdge()
{
    TimeValue t = makeTV(0, 2024, 3, 15, 12, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 23, 59, 59);
    TVE_ASSERT(TimeValue::isTimeInRangeInDay(t, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInDayOutside()
{
    TimeValue t = makeTV(0, 0, 0, 0, 7, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 9, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 18, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInDay(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 0, 20, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInDay(t2, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInWeekBasic()
{
    TimeValue t = makeTV(3, 0, 0, 0, 0, 0, 0);
    TimeValue minT = makeTV(1, 0, 0, 0, 0, 0, 0);
    TimeValue maxT = makeTV(5, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInWeek(t, minT, maxT));
    TimeValue tAtMin = makeTV(1, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInWeek(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(5, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInWeek(tAtMax, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInWeekEdge()
{
    TimeValue t = makeTV(1, 0, 0, 0, 12, 0, 0);
    TimeValue minT = makeTV(1, 0, 0, 0, 9, 0, 0);
    TimeValue maxT = makeTV(1, 0, 0, 0, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInWeek(t, minT, maxT));
    TimeValue t2 = makeTV(3, 0, 0, 0, 0, 0, 0);
    TimeValue minT2 = makeTV(3, 0, 0, 0, 10, 0, 0);
    TimeValue maxT2 = makeTV(3, 0, 0, 0, 20, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInWeek(t2, minT2, maxT2));
}

void TimeValueExtTest::testIsTimeInRangeInWeekOutside()
{
    TimeValue t = makeTV(6, 0, 0, 0, 0, 0, 0);
    TimeValue minT = makeTV(1, 0, 0, 0, 0, 0, 0);
    TimeValue maxT = makeTV(5, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInWeek(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInWeek(t2, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInMonthBasic()
{
    TimeValue t = makeTV(0, 0, 0, 15, 0, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 1, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 28, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInMonth(t, minT, maxT));
    TimeValue tAtMin = makeTV(0, 0, 0, 1, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInMonth(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(0, 0, 0, 28, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInMonth(tAtMax, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInMonthEdge()
{
    TimeValue t = makeTV(0, 0, 0, 15, 12, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 15, 9, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 15, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInMonth(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 15, 8, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInMonth(t2, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInMonthOutside()
{
    TimeValue t = makeTV(0, 0, 0, 5, 0, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 10, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 20, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInMonth(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 25, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInMonth(t2, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInYearBasic()
{
    TimeValue t = makeTV(0, 0, 6, 15, 0, 0, 0);
    TimeValue minT = makeTV(0, 0, 1, 1, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 12, 31, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInYear(t, minT, maxT));
    TimeValue tAtMin = makeTV(0, 0, 1, 1, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInYear(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(0, 0, 12, 31, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInYear(tAtMax, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInYearEdge()
{
    TimeValue t = makeTV(0, 0, 6, 15, 12, 0, 0);
    TimeValue minT = makeTV(0, 0, 6, 15, 9, 0, 0);
    TimeValue maxT = makeTV(0, 0, 6, 15, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInYear(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 6, 1, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInYear(t2, minT, maxT));
}

void TimeValueExtTest::testIsTimeInRangeInYearOutside()
{
    TimeValue t = makeTV(0, 0, 3, 15, 0, 0, 0);
    TimeValue minT = makeTV(0, 0, 6, 1, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 9, 30, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInYear(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 11, 1, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInYear(t2, minT, maxT));
}

void TimeValueExtTest::test()
{
    testTimeValueClear();
    testTimeValueDefaultInit();
    testTimeValueOperatorGreater();
    testOperatorGreaterEdge();
    testOperatorGreaterSameValues();
    testIsTimeInRangeInHourBasic();
    testIsTimeInRangeInHourEdge();
    testIsTimeInRangeInHourOutside();
    testIsTimeInRangeInDayBasic();
    testIsTimeInRangeInDayEdge();
    testIsTimeInRangeInDayOutside();
    testIsTimeInRangeInWeekBasic();
    testIsTimeInRangeInWeekEdge();
    testIsTimeInRangeInWeekOutside();
    testIsTimeInRangeInMonthBasic();
    testIsTimeInRangeInMonthEdge();
    testIsTimeInRangeInMonthOutside();
    testIsTimeInRangeInYearBasic();
    testIsTimeInRangeInYearEdge();
    testIsTimeInRangeInYearOutside();
}
