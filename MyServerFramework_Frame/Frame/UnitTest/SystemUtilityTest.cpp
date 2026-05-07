#include "FrameHeader.h"

namespace
{
	static void assertSystemUtility(bool condition, const char* message)
	{
		if (!condition)
		{
			ERROR(string("SystemUtilityTest failed: ") + message);
		}
	}

	static void testDateMath()
	{
		assertSystemUtility(isLeapYear(2000), "2000 leap");
		assertSystemUtility(!isLeapYear(1900), "1900 not leap");
		assertSystemUtility(isLeapYear(2024), "2024 leap");
		assertSystemUtility(!isLeapYear(2023), "2023 not leap");
		assertSystemUtility(daysToSeconds(1) == 86400, "days to seconds");
		assertSystemUtility(getDaysInMonth(2024, 2) == 29, "feb leap");
		assertSystemUtility(getDaysInMonth(2023, 2) == 28, "feb normal");
		assertSystemUtility(getDaysInMonth(2023, 4) == 30, "april days");
		assertSystemUtility(getDaysInMonth(2023, 1) == 31, "jan days");
	}

	static void testTimestampRoundTrip()
	{
		const time_t stamp = convertToTimestamp(2024, 3, 31, 12, 34, 56);
		struct tm timeStruct = getTimeStruct(stamp);
		assertSystemUtility(timeStruct.tm_year == 2024, "year roundtrip");
		assertSystemUtility(timeStruct.tm_mon == 3, "month roundtrip");
		assertSystemUtility(timeStruct.tm_mday == 31, "day roundtrip");
		assertSystemUtility(timeStruct.tm_hour == 12, "hour roundtrip");
		assertSystemUtility(timeStruct.tm_min == 34, "minute roundtrip");
		assertSystemUtility(timeStruct.tm_sec == 56, "second roundtrip");
		assertSystemUtility(isSameDay(stamp, stamp + 3600), "same day");
		assertSystemUtility(!isSameDay(stamp, stamp + daysToSeconds(1)), "different day");
	}

	static void testTodayBoundaries()
	{
		const llong begin = getTodayBegin();
		const llong end = getTodayEnd();
		assertSystemUtility(end - begin == daysToSeconds(1), "today interval");
		struct tm beginStruct = getTimeStruct((time_t)begin);
		assertSystemUtility(beginStruct.tm_hour == 0, "today begin hour");
		assertSystemUtility(beginStruct.tm_min == 0, "today begin minute");
		assertSystemUtility(beginStruct.tm_sec == 0, "today begin second");
	}

	static void testMutableGlobalState()
	{
		setTimeMSecondUTC(123456);
		setTimeSecondUTC(654321);
		assertSystemUtility(getTimeMSecondUTC() == 123456, "ms utc set");
		assertSystemUtility(getTimeSecondUTC() == 654321, "sec utc set");

		setMainThread(getThreadID());
		assertSystemUtility(isMainThread(), "main thread flag");
		const llong id0 = makeIDMain();
		const llong id1 = makeIDMain();
		const llong tid0 = makeIDThread();
		const llong tid1 = makeIDThread();
		assertSystemUtility(id1 == id0 + 1, "main id increment");
		assertSystemUtility(tid1 == tid0 + 1, "thread id increment");
	}

	static void testTimeQueries()
	{
		const int year = getYear();
		const int month = getMonth();
		const int day = getDay();
		const int hour = getTimeHourInDay();
		const int cpu = getCPUCoreCount();
		const llong realTime = getRealTimeMS();

		assertSystemUtility(year >= 1970, "year valid");
		assertSystemUtility(month >= 1 && month <= 12, "month valid");
		assertSystemUtility(day >= 1 && day <= 31, "day valid");
		assertSystemUtility(hour >= 0 && hour < 24, "hour valid");
		assertSystemUtility(cpu > 0, "cpu count");
		assertSystemUtility(realTime >= 0, "real time");
	}
}

void SystemUtilityTest::test()
{
	testDateMath();
	testTimestampRoundTrip();
	testTodayBoundaries();
	testMutableGlobalState();
	testTimeQueries();
}
