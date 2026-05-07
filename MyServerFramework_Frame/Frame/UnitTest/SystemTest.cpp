#include "FrameHeader.h"

#define SYS_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("SystemTest FAIL: ") + (msg)); } } while(0)
#define SYS_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SystemTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SYS_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SystemTest FAIL: ") + (msg) + \
		" | expected=" + LLToS((llong)(b)) + " actual=" + LLToS((llong)(a))); } } while(0)

//------------------------------------------------------------------------------
// isLeapYear
//------------------------------------------------------------------------------
static void testIsLeapYear()
{
	// standard leap: divisible by 4, not 100
	SYS_ASSERT(SystemUtility::isLeapYear(2000), "2000 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(2004), "2004 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(2024), "2024 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(1600), "1600 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(400),  "400 is leap");

	// divisible by 100 but not 400: not leap
	SYS_ASSERT(!SystemUtility::isLeapYear(1900), "1900 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1800), "1800 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1700), "1700 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(2100), "2100 not leap");

	// not divisible by 4: not leap
	SYS_ASSERT(!SystemUtility::isLeapYear(2001), "2001 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(2003), "2003 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(2023), "2023 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1999), "1999 not leap");
}

//------------------------------------------------------------------------------
// daysToSeconds
//------------------------------------------------------------------------------
static void testDaysToSeconds()
{
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(0),   0LL,     "0 days = 0 secs");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(1),   86400LL, "1 day = 86400 secs");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(7),   604800LL,"7 days = 604800 secs");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(30),  2592000LL,"30 days=2592000 secs");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(365), 31536000LL,"365 days=31536000 secs");
}

//------------------------------------------------------------------------------
// getDaysInMonth (year, month)
//------------------------------------------------------------------------------
static void testGetDaysInMonth()
{
	// January (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 1),  31, "Jan=31");
	// February normal year (28)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 2),  28, "Feb 2023=28");
	// February leap year (29)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2024, 2),  29, "Feb 2024=29");
	// March (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 3),  31, "Mar=31");
	// April (30)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 4),  30, "Apr=30");
	// May (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 5),  31, "May=31");
	// June (30)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 6),  30, "Jun=30");
	// July (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 7),  31, "Jul=31");
	// August (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 8),  31, "Aug=31");
	// September (30)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 9),  30, "Sep=30");
	// October (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 10), 31, "Oct=31");
	// November (30)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 11), 30, "Nov=30");
	// December (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 12), 31, "Dec=31");

	// Feb boundary: year 1900 (not leap)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(1900, 2),  28, "Feb 1900=28 (not leap)");
	// Feb boundary: year 2000 (leap)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2000, 2),  29, "Feb 2000=29 (leap)");
}

//------------------------------------------------------------------------------
// convertToTimestamp + getTimeStruct (round-trip)
//------------------------------------------------------------------------------
static void testConvertToTimestamp()
{
	// Known timestamp: 2024-01-15 12:30:45
	const time_t ts = SystemUtility::convertToTimestamp(2024, 1, 15, 12, 30, 45);
	SYS_ASSERT(ts > 0, "convertToTimestamp > 0");

	// Round-trip: convert back via getTimeStruct
	// NOTE: framework's getTimeStruct already applies tm_year+=1900 and tm_mon+=1
	// so tm_year is absolute year (e.g. 2024) and tm_mon is 1-based (1..12)
	const struct tm tmVal = SystemUtility::getTimeStruct(ts);
	SYS_ASSERT_EQ(tmVal.tm_year, 2024, "round-trip year=2024");
	SYS_ASSERT_EQ(tmVal.tm_mon,  1,    "round-trip month=1");
	SYS_ASSERT_EQ(tmVal.tm_mday, 15,   "round-trip day=15");
	SYS_ASSERT_EQ(tmVal.tm_hour, 12,   "round-trip hour=12");
	SYS_ASSERT_EQ(tmVal.tm_min,  30,   "round-trip min=30");
	SYS_ASSERT_EQ(tmVal.tm_sec,  45,   "round-trip sec=45");

	// 2000-02-29 (leap day)
	const time_t leapTs = SystemUtility::convertToTimestamp(2000, 2, 29, 0, 0, 0);
	const struct tm leapTm = SystemUtility::getTimeStruct(leapTs);
	SYS_ASSERT_EQ(leapTm.tm_year, 2000, "leap day year=2000");
	SYS_ASSERT_EQ(leapTm.tm_mon,  2,    "leap day month=2");
	SYS_ASSERT_EQ(leapTm.tm_mday, 29,   "leap day day=29");

	// 2000-06-15 08:00:00 — safely away from epoch boundary on any timezone
	// (1970-01-01 local time in UTC+8 yields negative timestamp, mktime returns -1)
	const time_t ts2000 = SystemUtility::convertToTimestamp(2000, 6, 15, 8, 0, 0);
	SYS_ASSERT(ts2000 > 0, "convertToTimestamp 2000-06-15 > 0");
	const struct tm tm2000 = SystemUtility::getTimeStruct(ts2000);
	SYS_ASSERT_EQ(tm2000.tm_year, 2000, "2000 round-trip year=2000");
	SYS_ASSERT_EQ(tm2000.tm_mon,  6,    "2000 round-trip month=6");
	SYS_ASSERT_EQ(tm2000.tm_mday, 15,   "2000 round-trip day=15");
}

//------------------------------------------------------------------------------
// isSameDay
//------------------------------------------------------------------------------
static void testIsSameDay()
{
	// Two timestamps in the same calendar day
	const time_t ts1 = SystemUtility::convertToTimestamp(2024, 3, 15, 8, 0, 0);
	const time_t ts2 = SystemUtility::convertToTimestamp(2024, 3, 15, 23, 59, 59);
	SYS_ASSERT(SystemUtility::isSameDay(ts1, ts2), "isSameDay: same day true");

	// Two timestamps in different calendar days
	const time_t ts3 = SystemUtility::convertToTimestamp(2024, 3, 15, 23, 59, 59);
	const time_t ts4 = SystemUtility::convertToTimestamp(2024, 3, 16, 0, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(ts3, ts4), "isSameDay: diff day false");

	// Same timestamp with itself
	SYS_ASSERT(SystemUtility::isSameDay(ts1, ts1), "isSameDay: ts with itself");

	// Different months
	const time_t ts5 = SystemUtility::convertToTimestamp(2024, 3, 31, 12, 0, 0);
	const time_t ts6 = SystemUtility::convertToTimestamp(2024, 4, 1, 12, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(ts5, ts6), "isSameDay: diff month false");

	// Different years
	const time_t ts7 = SystemUtility::convertToTimestamp(2023, 12, 31, 12, 0, 0);
	const time_t ts8 = SystemUtility::convertToTimestamp(2024, 1, 1, 12, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(ts7, ts8), "isSameDay: diff year false");
}

//------------------------------------------------------------------------------
// mTimeMSecondUTC / mTimeSecondUTC set-get
//------------------------------------------------------------------------------
static void testTimeSeed()
{
	// set and get mTimeMSecondUTC
	const llong origMS = SystemUtility::getTimeMSecondUTC();
	SystemUtility::setTimeMSecondUTC(999999LL);
	SYS_ASSERT_EQ_LL(SystemUtility::getTimeMSecondUTC(), 999999LL, "setTimeMSecond round-trip");
	// restore
	SystemUtility::setTimeMSecondUTC(origMS);

	// set and get mTimeSecondUTC
	const llong origS = SystemUtility::getTimeSecondUTC();
	SystemUtility::setTimeSecondUTC(888888LL);
	SYS_ASSERT_EQ_LL(SystemUtility::getTimeSecondUTC(), 888888LL, "setTimeSecond round-trip");
	// restore
	SystemUtility::setTimeSecondUTC(origS);
}

//------------------------------------------------------------------------------
// makeIDMain - generates monotonically increasing IDs on main thread
//------------------------------------------------------------------------------
static void testMakeID()
{
	// Ensure IDs are strictly increasing
	const llong id0 = SystemUtility::makeIDMain();
	const llong id1 = SystemUtility::makeIDMain();
	const llong id2 = SystemUtility::makeIDMain();
	SYS_ASSERT(id1 > id0, "makeIDMain id1>id0");
	SYS_ASSERT(id2 > id1, "makeIDMain id2>id1");
	SYS_ASSERT(id0 > 0,   "makeIDMain id>0");

	// makeIDThread must also be increasing
	const llong tid0 = SystemUtility::makeIDThread();
	const llong tid1 = SystemUtility::makeIDThread();
	SYS_ASSERT(tid1 > tid0, "makeIDThread tid1>tid0");
}

//------------------------------------------------------------------------------
// getThreadID / isMainThread
//------------------------------------------------------------------------------
static void testThreadID()
{
	const int tid = SystemUtility::getThreadID();
	SYS_ASSERT(tid != 0, "getThreadID != 0");

	// isMainThread: depends on whether setMainThread was called
	// Since main.cpp calls setMainThread before UnitTest::test(),
	// this should be true in our test context
	SYS_ASSERT(SystemUtility::isMainThread(), "isMainThread() true in test");
}

//------------------------------------------------------------------------------
// getYear / getMonth / getDay consistency
//------------------------------------------------------------------------------
static void testGetYearMonthDay()
{
	int year = SystemUtility::getYear();
	int month = SystemUtility::getMonth();
	int day = SystemUtility::getDay();

	SYS_ASSERT(year >= 2024, "getYear >= 2024");
	SYS_ASSERT(month >= 1 && month <= 12, "getMonth in [1,12]");
	SYS_ASSERT(day >= 1 && day <= 31, "getDay in [1,31]");

	int y2 = 0, m2 = 0;
	SystemUtility::getYearMonth(y2, m2);
	SYS_ASSERT_EQ(y2, year, "getYearMonth year matches");
	SYS_ASSERT_EQ(m2, month, "getYearMonth month matches");
}

//------------------------------------------------------------------------------
// getTime (hour, minute, second)
//------------------------------------------------------------------------------
static void testGetTimeHMS()
{
	int h = -1, m = -1, s = -1;
	SystemUtility::getTime(h, m, s);
	SYS_ASSERT(h >= 0 && h < 24, "getTime hour in [0,23]");
	SYS_ASSERT(m >= 0 && m < 60, "getTime min in [0,59]");
	SYS_ASSERT(s >= 0 && s < 60, "getTime sec in [0,59]");
}

//------------------------------------------------------------------------------
// getTime (year, month, day, hour, minute, second)
//------------------------------------------------------------------------------
static void testGetTimeYMDHMS()
{
	int y = 0, mo = 0, d = 0, h = 0, mi = 0, s = 0;
	SystemUtility::getTime(y, mo, d, h, mi, s);
	SYS_ASSERT(y >= 2024, "getTime YMDHMS year >= 2024");
	SYS_ASSERT(mo >= 1 && mo <= 12, "getTime YMDHMS month valid");
	SYS_ASSERT(d >= 1 && d <= 31, "getTime YMDHMS day valid");
	SYS_ASSERT(h >= 0 && h < 24, "getTime YMDHMS hour valid");
	SYS_ASSERT(mi >= 0 && mi < 60, "getTime YMDHMS min valid");
	SYS_ASSERT(s >= 0 && s < 60, "getTime YMDHMS sec valid");
}

//------------------------------------------------------------------------------
// getTime (weekDay, year, month, day, hour, minute, second)
//------------------------------------------------------------------------------
static void testGetTimeWithWeekDay()
{
	int wd = -1, y = 0, mo = 0, d = 0, h = 0, mi = 0, s = 0;
	SystemUtility::getTime(wd, y, mo, d, h, mi, s);
	SYS_ASSERT(wd >= 0 && wd <= 6, "getTime weekDay in [1,7]");
	SYS_ASSERT(y >= 2024, "getTime weekDay year >= 2024");
}

//------------------------------------------------------------------------------
// getTimeHourInDay
//------------------------------------------------------------------------------
static void testGetTimeHourInDay()
{
	int h = SystemUtility::getTimeHourInDay();
	SYS_ASSERT(h >= 0 && h < 24, "getTimeHourInDay in [0,23]");
}

//------------------------------------------------------------------------------
// getDayOfWeek
//------------------------------------------------------------------------------
static void testGetDayOfWeek()
{
	int dow = SystemUtility::getDayOfWeek();
	SYS_ASSERT(dow >= 0 && dow <= 6, "getDayOfWeek in [0,6]");
}

//------------------------------------------------------------------------------
// getTime (string)
//------------------------------------------------------------------------------
static void testGetTimeString()
{
	string t = SystemUtility::getTime();
	SYS_ASSERT(!t.empty(), "getTime() string non-empty");

	string ts = SystemUtility::getTime(true);
	SYS_ASSERT(!ts.empty(), "getTime(timestamp) string non-empty");
}

//------------------------------------------------------------------------------
// daysToSeconds — extended
//------------------------------------------------------------------------------
static void testDaysToSecondsExt()
{
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(2),   172800LL,  "2 days");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(3),   259200LL,  "3 days");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(10),  864000LL,  "10 days");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(100), 8640000LL, "100 days");
}

//------------------------------------------------------------------------------
// isLeapYear — extended range
//------------------------------------------------------------------------------
static void testIsLeapYearExt()
{
	// every 4th year in range [2000, 2100]
	for (int y = 2000; y <= 2100; y += 4)
	{
		if (y == 2100)
		{
			SYS_ASSERT(!SystemUtility::isLeapYear(y), "2100 not leap (div100 not div400)");
		}
		else
		{
			SYS_ASSERT(SystemUtility::isLeapYear(y), "y%4==0 is leap");
		}
	}
	// non-leap years
	for (int y = 2001; y <= 2099; y += 4)
	{
		// 2001 2005 2009 ... are not leap
		SYS_ASSERT(!SystemUtility::isLeapYear(y), "non-leap year");
	}
}

//------------------------------------------------------------------------------
// getDaysInMonth — all months for leap and non-leap
//------------------------------------------------------------------------------
static void testGetDaysInMonthFull()
{
	const int daysNormal[13] = {0, 31,28,31,30,31,30,31,31,30,31,30,31};
	const int daysLeap[13]   = {0, 31,29,31,30,31,30,31,31,30,31,30,31};

	// non-leap 2023
	for (int m = 1; m <= 12; ++m)
	{
		SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, m), daysNormal[m], "2023 days month");
	}
	// leap 2024
	for (int m = 1; m <= 12; ++m)
	{
		SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2024, m), daysLeap[m], "2024 days month");
	}
	// leap 2000
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2000, 2), 29, "2000 Feb=29");
	// non-leap 1900
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(1900, 2), 28, "1900 Feb=28");
}

//------------------------------------------------------------------------------
// convertToTimestamp — multiple dates
//------------------------------------------------------------------------------
static void testConvertToTimestampMultiple()
{
	// 2020-03-01 midnight
	const time_t ts1 = SystemUtility::convertToTimestamp(2020, 3, 1, 0, 0, 0);
	SYS_ASSERT(ts1 > 0, "ts1 > 0");
	const struct tm tm1 = SystemUtility::getTimeStruct(ts1);
	SYS_ASSERT_EQ(tm1.tm_year, 2020, "ts1 year");
	SYS_ASSERT_EQ(tm1.tm_mon, 3, "ts1 month");
	SYS_ASSERT_EQ(tm1.tm_mday, 1, "ts1 day");

	// 2023-12-31 23:59:59
	const time_t ts2 = SystemUtility::convertToTimestamp(2023, 12, 31, 23, 59, 59);
	SYS_ASSERT(ts2 > 0, "ts2 > 0");
	const struct tm tm2 = SystemUtility::getTimeStruct(ts2);
	SYS_ASSERT_EQ(tm2.tm_year, 2023, "ts2 year");
	SYS_ASSERT_EQ(tm2.tm_mon, 12, "ts2 month");
	SYS_ASSERT_EQ(tm2.tm_mday, 31, "ts2 day");
	SYS_ASSERT_EQ(tm2.tm_hour, 23, "ts2 hour");
	SYS_ASSERT_EQ(tm2.tm_sec, 59, "ts2 sec");

	// 2024-02-29 (leap day)
	const time_t ts3 = SystemUtility::convertToTimestamp(2024, 2, 29, 12, 0, 0);
	SYS_ASSERT(ts3 > 0, "ts3 > 0");
	const struct tm tm3 = SystemUtility::getTimeStruct(ts3);
	SYS_ASSERT_EQ(tm3.tm_year, 2024, "ts3 year");
	SYS_ASSERT_EQ(tm3.tm_mon, 2, "ts3 month");
	SYS_ASSERT_EQ(tm3.tm_mday, 29, "ts3 day");

	// 2025-06-15 08:30:00
	const time_t ts4 = SystemUtility::convertToTimestamp(2025, 6, 15, 8, 30, 0);
	SYS_ASSERT(ts4 > 0, "ts4 > 0");
	const struct tm tm4 = SystemUtility::getTimeStruct(ts4);
	SYS_ASSERT_EQ(tm4.tm_year, 2025, "ts4 year");
	SYS_ASSERT_EQ(tm4.tm_mon, 6, "ts4 month");
	SYS_ASSERT_EQ(tm4.tm_mday, 15, "ts4 day");
	SYS_ASSERT_EQ(tm4.tm_hour, 8, "ts4 hour");
	SYS_ASSERT_EQ(tm4.tm_min, 30, "ts4 min");
}

//------------------------------------------------------------------------------
// isSameDay — extended
//------------------------------------------------------------------------------
static void testIsSameDayExt()
{
	// Year boundary: Dec 31 vs Jan 1
	const time_t end2022 = SystemUtility::convertToTimestamp(2022, 12, 31, 23, 0, 0);
	const time_t start2023 = SystemUtility::convertToTimestamp(2023, 1, 1, 0, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(end2022, start2023), "year boundary not same day");

	// Same timestamp
	const time_t t = SystemUtility::convertToTimestamp(2024, 5, 20, 12, 0, 0);
	SYS_ASSERT(SystemUtility::isSameDay(t, t), "same ts is same day");

	// Early morning vs late night same day
	const time_t early = SystemUtility::convertToTimestamp(2024, 7, 4, 0, 0, 1);
	const time_t late  = SystemUtility::convertToTimestamp(2024, 7, 4, 23, 59, 58);
	SYS_ASSERT(SystemUtility::isSameDay(early, late), "early vs late same day");

	// Leap day vs day after
	const time_t leapDay  = SystemUtility::convertToTimestamp(2024, 2, 29, 12, 0, 0);
	const time_t march1   = SystemUtility::convertToTimestamp(2024, 3, 1, 0, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(leapDay, march1), "leap day vs march1 not same");
}

//------------------------------------------------------------------------------
// getTodayBegin / getTodayEnd consistency
//------------------------------------------------------------------------------
static void testTodayBeginEnd2()
{
	const llong begin = SystemUtility::getTodayBegin();
	const llong end   = SystemUtility::getTodayEnd();
	SYS_ASSERT(begin > 0, "getTodayBegin > 0");
	SYS_ASSERT(end > begin, "getTodayEnd > getTodayBegin");
	// end - begin should be exactly 86400 seconds (24h in seconds)
	SYS_ASSERT_EQ_LL(end - begin, 86400LL, "today end-begin=86400");
}

//------------------------------------------------------------------------------
// getRealTimeMS — monotonically increasing
//------------------------------------------------------------------------------
static void testGetRealTimeMS()
{
	const llong t1 = SystemUtility::getRealTimeMS();
	SYS_ASSERT(t1 > 0, "getRealTimeMS > 0");
	// getRealTimeMS is monotone (we can't guarantee in test, but it should be >= t1)
	const llong t2 = SystemUtility::getRealTimeMS();
	SYS_ASSERT(t2 >= t1, "getRealTimeMS t2 >= t1");
}

//------------------------------------------------------------------------------
// makeIDMain — bulk IDs are strictly increasing
//------------------------------------------------------------------------------
static void testMakeIDMainBulk()
{
	const int N = 100;
	Vector<llong> ids;
	FOR(200)
	{
		ids.add(SystemUtility::makeIDMain());
	}
	for (int i = 1; i < N; ++i)
	{
		SYS_ASSERT(ids[i] > ids[i - 1], "makeIDMain bulk strictly increasing");
	}
}

//------------------------------------------------------------------------------
// makeIDThread — bulk
//------------------------------------------------------------------------------
static void testMakeIDThreadBulk()
{
	const int N = 50;
	Vector<llong> ids;
	FOR(200)
	{
		ids.add(SystemUtility::makeIDThread());
	}
	for (int i = 1; i < N; ++i)
	{
		SYS_ASSERT(ids[i] > ids[i - 1], "makeIDThread bulk strictly increasing");
	}
}

//------------------------------------------------------------------------------
// getPID
//------------------------------------------------------------------------------
static void testGetPID()
{
	const int pid = SystemUtility::getPID();
	SYS_ASSERT(pid > 0, "getPID > 0");
}

//------------------------------------------------------------------------------
// getTimeStruct consistency with convertToTimestamp
//------------------------------------------------------------------------------
static void testGetTimeStructConsistency()
{
	// Test multiple dates
	struct TestDate { int y, mo, d, h, mi, s; };
	TestDate dates[] = {
		{2020, 6, 15, 10, 30, 0},
		{2021, 11, 30, 23, 59, 59},
		{2022, 1, 1, 0, 0, 1},
		{2023, 8, 15, 12, 0, 0},
		{2024, 3, 10, 18, 45, 30},
		{2025, 9, 22, 6, 15, 45},
	};
	for (int i = 0; i < 6; ++i)
	{
		const TestDate& dt = dates[i];
		const time_t ts = SystemUtility::convertToTimestamp(dt.y, dt.mo, dt.d, dt.h, dt.mi, dt.s);
		SYS_ASSERT(ts > 0, "date ts > 0");
		const struct tm tmVal = SystemUtility::getTimeStruct(ts);
		SYS_ASSERT_EQ(tmVal.tm_year, dt.y,  "round-trip year");
		SYS_ASSERT_EQ(tmVal.tm_mon,  dt.mo, "round-trip month");
		SYS_ASSERT_EQ(tmVal.tm_mday, dt.d,  "round-trip day");
		SYS_ASSERT_EQ(tmVal.tm_hour, dt.h,  "round-trip hour");
		SYS_ASSERT_EQ(tmVal.tm_min,  dt.mi, "round-trip min");
		SYS_ASSERT_EQ(tmVal.tm_sec,  dt.s,  "round-trip sec");
	}
}

//------------------------------------------------------------------------------
// isSameDay — large batch of known pairs
//------------------------------------------------------------------------------
static void testIsSameDayBatch()
{
	// All pairs within the same year
	for (int m = 1; m <= 12; ++m)
	{
		int days = SystemUtility::getDaysInMonth(2023, m);
		for (int d = 1; d < days; ++d)
		{
			const time_t t1 = SystemUtility::convertToTimestamp(2023, m, d, 0, 0, 0);
			const time_t t2 = SystemUtility::convertToTimestamp(2023, m, d, 23, 59, 59);
			const time_t t3 = SystemUtility::convertToTimestamp(2023, m, d + 1, 0, 0, 0);
			SYS_ASSERT(SystemUtility::isSameDay(t1, t2), "batch same day");
			SYS_ASSERT(!SystemUtility::isSameDay(t1, t3), "batch diff day");
		}
	}
}

//------------------------------------------------------------------------------
// isLeapYear — boundary values
//------------------------------------------------------------------------------
static void testIsLeapYearBoundary()
{
	// Historical leap years
	SYS_ASSERT(SystemUtility::isLeapYear(4), "4 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(8), "8 is leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(100), "100 not leap");
	SYS_ASSERT(SystemUtility::isLeapYear(400), "400 is leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(200), "200 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(300), "300 not leap");
	SYS_ASSERT(SystemUtility::isLeapYear(800), "800 is leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(900), "900 not leap");
	SYS_ASSERT(SystemUtility::isLeapYear(1200), "1200 is leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1300), "1300 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1), "1 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(2), "2 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(3), "3 not leap");
}

//------------------------------------------------------------------------------
// getProgramFile — string non-empty
//------------------------------------------------------------------------------
static void testGetProgramFile()
{
	string progFile = SystemUtility::getProgramFile();
	SYS_ASSERT(!progFile.empty(), "getProgramFile non-empty");
}

//------------------------------------------------------------------------------
// getCPUCoreCount
//------------------------------------------------------------------------------
static void testGetCPUCoreCount()
{
	int cores = SystemUtility::getCPUCoreCount();
	SYS_ASSERT(cores >= 1, "getCPUCoreCount >= 1");
}

//------------------------------------------------------------------------------
// SystemTest entry point
//------------------------------------------------------------------------------
void SystemTest::test()
{
	testIsLeapYear();
	testDaysToSeconds();
	testGetDaysInMonth();
	testConvertToTimestamp();
	testIsSameDay();
	testTimeSeed();
	testMakeID();
	testThreadID();
	testGetYearMonthDay();
	testGetTimeHMS();
	testGetTimeYMDHMS();
	testGetTimeWithWeekDay();
	testGetTimeHourInDay();
	testGetDayOfWeek();
	testGetTimeString();
	testDaysToSecondsExt();
	testIsLeapYearExt();
	testGetDaysInMonthFull();
	testConvertToTimestampMultiple();
	testIsSameDayExt();
	testTodayBeginEnd2();
	testGetRealTimeMS();
	testMakeIDMainBulk();
	testMakeIDThreadBulk();
	testGetPID();
	testGetTimeStructConsistency();
	testIsSameDayBatch();
	testIsLeapYearBoundary();
	testGetProgramFile();
	testGetCPUCoreCount();
}
