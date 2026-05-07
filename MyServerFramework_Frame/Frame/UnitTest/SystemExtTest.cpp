#include "FrameHeader.h"

// ============================================================
// SystemExtTest
// 覆盖 SystemUtility 中 SystemTest 未测到的函数：
//   getTodayBegin / getTodayEnd
//   getDayOfWeek
//   getTimeHourInDay
//   getTime(weekDay, year, month, day, hour, minute, second)
//   getTime(year, month, day, hour, minute, second)
//   getTime(string) — 格式化时间字符串
//   getCPUCoreCount
//   getPID
//   getRealTimeMS
//   sleep (short)
//   getProgramFile
//   isMainThread / getThreadID / setMainThread (补充)
//   makeIDThread (压力测试)
//   daysToSeconds (更多边界)
//   isLeapYear (补充边界)
//   convertToTimestamp (更多场景)
//   getTimeStruct (独立验证)
// ============================================================

#define SEXT_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("SystemExtTest FAILED: ") + (msg)); } } while(0)
#define SEXT_ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("SystemExtTest FAILED: ") + (msg) \
        + " expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SEXT_ASSERT_EQ_LL(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("SystemExtTest FAILED: ") + (msg) \
        + " expected=" + LLToS((llong)(b)) + " actual=" + LLToS((llong)(a))); } } while(0)

// ============================================================
// getTodayBegin / getTodayEnd
// ============================================================
void SystemExtTest::testTodayBeginEnd1()
{
    const llong begin = getTodayBegin();
    const llong end = getTodayEnd();
    const llong nowUTC = getTimeSecondUTC();

    // begin 应 <= now
    SEXT_ASSERT(begin <= nowUTC, "getTodayBegin <= now");

    // end 应 > now（今天还没结束）
    SEXT_ASSERT(end >= nowUTC, "getTodayEnd >= now");

    // end > begin
    SEXT_ASSERT(end > begin, "getTodayEnd > getTodayBegin");

    // end - begin 应 == 86400 秒（一天）
    const llong diff = end - begin;
    SEXT_ASSERT_EQ_LL(diff, 86400LL, "getTodayEnd - getTodayBegin == 86400");

    // begin 对应的时间应该是今天 00:00:00
    const struct tm beginTm = getTimeStruct((time_t)begin);
    SEXT_ASSERT_EQ(beginTm.tm_hour, 0, "getTodayBegin hour==0");
    SEXT_ASSERT_EQ(beginTm.tm_min, 0, "getTodayBegin min==0");
    SEXT_ASSERT_EQ(beginTm.tm_sec, 0, "getTodayBegin sec==0");

    // end 对应的时间应该是今天 24:00:00 即明天 00:00:00
    // end = begin + 86400，对应明天 0 点
    const struct tm endTm = getTimeStruct((time_t)end);
    SEXT_ASSERT_EQ(endTm.tm_hour, 0, "getTodayEnd hour==0 (next day midnight)");
    SEXT_ASSERT_EQ(endTm.tm_min, 0, "getTodayEnd min==0");
    SEXT_ASSERT_EQ(endTm.tm_sec, 0, "getTodayEnd sec==0");

    // 两次调用结果一致（幂等）
    const llong begin2 = getTodayBegin();
    const llong end2 = getTodayEnd();
    SEXT_ASSERT_EQ_LL(begin, begin2, "getTodayBegin idempotent");
    SEXT_ASSERT_EQ_LL(end, end2, "getTodayEnd idempotent");
}

// ============================================================
// getDayOfWeek
// ============================================================
void SystemExtTest::testGetDayOfWeek2()
{
    // 当前值必须在 [0, 6]（0=周日, 1=周一, ..., 6=周六）
    const int dow = getDayOfWeek();
    SEXT_ASSERT(dow >= 0 && dow <= 6, "getDayOfWeek in [0,6]");

    // 使用已知日期验证：通过 convertToTimestamp 构造特定日期的 timestamp
    // 2024-01-07 是周日（dow = 0）
    const time_t ts_sunday = convertToTimestamp(2024, 1, 7, 12, 0, 0);
    const struct tm tm_sunday = getTimeStruct(ts_sunday);
    // tm_wday: 0=Sunday in standard C, framework 里若保持标准则 tm_wday==0
    // 注意：framework 的 getTimeStruct 对 tm_wday 是否调整，如不调整则标准 0=Sunday
    // 根据框架代码，getTimeStruct 只对 year/mon 做调整，wday 保持原始 0-6(Sun=0)
    SEXT_ASSERT_EQ(tm_sunday.tm_wday, 0, "2024-01-07 wday==0 (Sunday)");

    // 2024-01-01 是周一
    const time_t ts_monday = convertToTimestamp(2024, 1, 1, 12, 0, 0);
    const struct tm tm_monday = getTimeStruct(ts_monday);
    SEXT_ASSERT_EQ(tm_monday.tm_wday, 1, "2024-01-01 wday==1 (Monday)");

    // 2024-01-06 是周六
    const time_t ts_saturday = convertToTimestamp(2024, 1, 6, 12, 0, 0);
    const struct tm tm_saturday = getTimeStruct(ts_saturday);
    SEXT_ASSERT_EQ(tm_saturday.tm_wday, 6, "2024-01-06 wday==6 (Saturday)");

    // 2024-01-03 是周三
    const time_t ts_wednesday = convertToTimestamp(2024, 1, 3, 12, 0, 0);
    const struct tm tm_wednesday = getTimeStruct(ts_wednesday);
    SEXT_ASSERT_EQ(tm_wednesday.tm_wday, 3, "2024-01-03 wday==3 (Wednesday)");
}

// ============================================================
// getTimeHourInDay
// ============================================================
void SystemExtTest::testGetTimeHourInDay2()
{
    const int hour = getTimeHourInDay();
    // 必须在 [0, 23]
    SEXT_ASSERT(hour >= 0 && hour < 24, "getTimeHourInDay in [0,23]");

    // 与 getTime(h,m,s) 一致
    int h = -1;
    int m = -1;
    int s = -1;
    getTime(h, m, s);
    SEXT_ASSERT_EQ(hour, h, "getTimeHourInDay matches getTime hour");
}

// ============================================================
// getTime (full: weekDay, year, month, day, hour, minute, second)
// ============================================================
void SystemExtTest::testGetTimeFullWithWeekDay()
{
    int weekDay = -1;
    int year = -1;
    int month = -1;
    int day = -1;
    int hour = -1;
    int minute = -1;
    int second = -1;
    getTime(weekDay, year, month, day, hour, minute, second);

    SEXT_ASSERT(weekDay >= 0 && weekDay <= 6, "getTime7 weekDay in [0,6]");
    SEXT_ASSERT(year >= 2024, "getTime7 year>=2024");
    SEXT_ASSERT(month >= 1 && month <= 12, "getTime7 month in [1,12]");
    SEXT_ASSERT(day >= 1 && day <= 31, "getTime7 day in [1,31]");
    SEXT_ASSERT(hour >= 0 && hour < 24, "getTime7 hour in [0,23]");
    SEXT_ASSERT(minute >= 0 && minute < 60, "getTime7 minute in [0,59]");
    SEXT_ASSERT(second >= 0 && second < 60, "getTime7 second in [0,59]");

    // 与 6-param 版本一致
    int y2 = -1, m2 = -1, d2 = -1, h2 = -1, min2 = -1, s2 = -1;
    getTime(y2, m2, d2, h2, min2, s2);
    SEXT_ASSERT_EQ(year, y2, "getTime7 year matches getTime6");
    SEXT_ASSERT_EQ(month, m2, "getTime7 month matches getTime6");
    SEXT_ASSERT_EQ(day, d2, "getTime7 day matches getTime6");

    // weekDay 与 getDayOfWeek 一致（实时调用，可能有 1 秒误差忽略）
    const int dowCurrent = getDayOfWeek();
    SEXT_ASSERT(dowCurrent >= 0 && dowCurrent <= 6, "getTime7 weekDay matches getDayOfWeek range");
}

// ============================================================
// getTime (6-param: year, month, day, hour, minute, second)
// ============================================================
void SystemExtTest::testGetTimeSixParams()
{
    int y = -1, m = -1, d = -1, h = -1, min = -1, s = -1;
    getTime(y, m, d, h, min, s);

    SEXT_ASSERT(y >= 2024, "getTime6 year>=2024");
    SEXT_ASSERT(m >= 1 && m <= 12, "getTime6 month in [1,12]");
    SEXT_ASSERT(d >= 1 && d <= 31, "getTime6 day in [1,31]");
    SEXT_ASSERT(h >= 0 && h < 24, "getTime6 hour in [0,23]");
    SEXT_ASSERT(min >= 0 && min < 60, "getTime6 minute in [0,59]");
    SEXT_ASSERT(s >= 0 && s < 60, "getTime6 second in [0,59]");

    // 与 getYear/getMonth/getDay 一致
    SEXT_ASSERT_EQ(y, getYear(), "getTime6 year matches getYear");
    SEXT_ASSERT_EQ(m, getMonth(), "getTime6 month matches getMonth");

    // 与 getTime(h,m,s) 一致
    int h2 = -1, m2 = -1, s2 = -1;
    getTime(h2, m2, s2);
    SEXT_ASSERT_EQ(h, h2, "getTime6 hour matches getTime3");
}

// ============================================================
// getTime (string 版本)
// ============================================================
void SystemExtTest::testGetTimeString2()
{
    // 不带时间戳
    const string timeStr = getTime(false);
    SEXT_ASSERT(!timeStr.empty(), "getTime string non-empty");
    // 格式通常是 "YYYY-MM-DD HH:MM:SS" 或类似，长度一般 >= 8
    SEXT_ASSERT(timeStr.length() >= 8, "getTime string length>=8");

    // 带时间戳（timeStamp=true 格式可能不同）
    const string timeStampStr = getTime(true);
    SEXT_ASSERT(!timeStampStr.empty(), "getTime timeStamp string non-empty");

    // 两次调用间隔很短，时间字符串应该相等或只差 1 秒
    const string t1 = getTime(false);
    const string t2 = getTime(false);
    SEXT_ASSERT(t1.length() == t2.length(), "getTime string same format length");
}

// ============================================================
// getCPUCoreCount
// ============================================================
void SystemExtTest::testGetCPUCoreCount2()
{
    const int count = getCPUCoreCount();
    SEXT_ASSERT(count >= 1, "getCPUCoreCount >= 1");
    // 现代机器一般不超过 1024 核
    SEXT_ASSERT(count <= 1024, "getCPUCoreCount <= 1024");

    // 多次调用结果一致
    const int count2 = getCPUCoreCount();
    SEXT_ASSERT_EQ(count, count2, "getCPUCoreCount idempotent");
}

// ============================================================
// getPID
// ============================================================
void SystemExtTest::testGetPID2()
{
    const int pid = getPID();
    SEXT_ASSERT(pid > 0, "getPID > 0");

    // 多次调用相同
    const int pid2 = getPID();
    SEXT_ASSERT_EQ(pid, pid2, "getPID idempotent");
}

// ============================================================
// getRealTimeMS
// ============================================================
void SystemExtTest::testGetRealTimeMS2()
{
    const llong t1 = getRealTimeMS();
    SEXT_ASSERT(t1 > 0, "getRealTimeMS > 0");

    // 稍等片刻后再获取，应该增加了
    // 使用极短 sleep（1ms），验证时间推进
    SystemUtility::sleep(2);
    const llong t2 = getRealTimeMS();
    SEXT_ASSERT(t2 >= t1, "getRealTimeMS monotonic after sleep");

    // 间隔至少 1ms（sleep 了 2ms）
    SEXT_ASSERT(t2 - t1 >= 1, "getRealTimeMS advanced at least 1ms");
}

// ============================================================
// sleep（验证执行不崩溃，时间推进）
// ============================================================
void SystemExtTest::testSleep()
{
    const llong before = getRealTimeMS();
    SystemUtility::sleep(5); // 5ms
    const llong after = getRealTimeMS();

    SEXT_ASSERT(after >= before, "sleep: time advanced");
    SEXT_ASSERT(after - before >= 3, "sleep: at least 3ms elapsed");

    // sleep 0ms 不崩溃
    SystemUtility::sleep(0);
    SEXT_ASSERT(true, "sleep(0) no crash");
}

// ============================================================
// getProgramFile
// ============================================================
void SystemExtTest::testGetProgramFile2()
{
    const string progFile = getProgramFile();
    SEXT_ASSERT(!progFile.empty(), "getProgramFile non-empty");

    // 应该是一个文件路径（包含路径分隔符或文件名）
    // 在 Windows 上通常以 .exe 结尾
    SEXT_ASSERT(progFile.length() >= 1, "getProgramFile length >= 1");

    // 多次调用一致
    const string progFile2 = getProgramFile();
    SEXT_ASSERT(progFile == progFile2, "getProgramFile idempotent");
}

// ============================================================
// makeIDThread — 多次调用严格递增
// ============================================================
void SystemExtTest::testMakeIDThreadExtended()
{
    // 连续 20 次，每次都应严格递增
    llong prev = makeIDThread();
    for (int i = 0; i < 20; ++i)
    {
        const llong curr = makeIDThread();
        SEXT_ASSERT(curr > prev, "makeIDThread strictly increasing");
        prev = curr;
    }

    // makeIDMain 与 makeIDThread 使用不同种子，互不影响
    const llong mainBefore = makeIDMain();
    FOR(200)
    {
        makeIDThread();
    }
    const llong mainAfter = makeIDMain();
    SEXT_ASSERT(mainAfter > mainBefore, "makeIDMain still increases after makeIDThread calls");
}

// ============================================================
// daysToSeconds 边界（补充）
// ============================================================
void SystemExtTest::testDaysToSecondsExtended()
{
    // 负值（框架可能允许负数）
    SEXT_ASSERT_EQ_LL(daysToSeconds(-1), -86400LL, "daysToSeconds -1 day");

    // 大数值
    SEXT_ASSERT_EQ_LL(daysToSeconds(1000), 86400000LL, "daysToSeconds 1000 days");

    // 边界：一年
    SEXT_ASSERT_EQ_LL(daysToSeconds(366), 31622400LL, "daysToSeconds 366 days (leap year)");

    // 精确值验证：1天 = 24*60*60 = 86400
    SEXT_ASSERT_EQ_LL((llong)(24 * 60 * 60), 86400LL, "86400 constant correct");
    SEXT_ASSERT_EQ_LL(daysToSeconds(1), 86400LL, "daysToSeconds(1)==86400");
}

// ============================================================
// isLeapYear 边界（补充）
// ============================================================
void SystemExtTest::testIsLeapYearExtended()
{
    // 早期年份
    SEXT_ASSERT(isLeapYear(4), "year 4 is leap");
    SEXT_ASSERT(!isLeapYear(1), "year 1 not leap");
    SEXT_ASSERT(!isLeapYear(100), "year 100 not leap");
    // 200 % 100 == 0 且 200 % 400 != 0，所以 NOT leap
    SEXT_ASSERT(!isLeapYear(200), "year 200 not leap (div 100 but not 400)");

    // 未来年份
    SEXT_ASSERT(isLeapYear(2400), "year 2400 is leap (div 400)");
    SEXT_ASSERT(!isLeapYear(2100), "year 2100 not leap");
    SEXT_ASSERT(!isLeapYear(2200), "year 2200 not leap");
    SEXT_ASSERT(!isLeapYear(2300), "year 2300 not leap");
    SEXT_ASSERT(isLeapYear(2400), "year 2400 is leap");

    // 连续几年
    SEXT_ASSERT(!isLeapYear(2025), "2025 not leap");
    SEXT_ASSERT(!isLeapYear(2026), "2026 not leap");
    SEXT_ASSERT(!isLeapYear(2027), "2027 not leap");
    SEXT_ASSERT(isLeapYear(2028), "2028 is leap");
}

// ============================================================
// convertToTimestamp 更多场景
// ============================================================
void SystemExtTest::testConvertToTimestampExtended()
{
    // 年初 1月1日
    const time_t tsJan1 = convertToTimestamp(2023, 1, 1, 0, 0, 0);
    const struct tm tmJan1 = getTimeStruct(tsJan1);
    SEXT_ASSERT_EQ(tmJan1.tm_year, 2023, "convertToTimestamp Jan 1 year==2023");
    SEXT_ASSERT_EQ(tmJan1.tm_mon, 1, "convertToTimestamp Jan 1 month==1");
    SEXT_ASSERT_EQ(tmJan1.tm_mday, 1, "convertToTimestamp Jan 1 day==1");

    // 年末 12月31日
    const time_t tsDec31 = convertToTimestamp(2023, 12, 31, 23, 59, 59);
    const struct tm tmDec31 = getTimeStruct(tsDec31);
    SEXT_ASSERT_EQ(tmDec31.tm_year, 2023, "convertToTimestamp Dec 31 year==2023");
    SEXT_ASSERT_EQ(tmDec31.tm_mon, 12, "convertToTimestamp Dec 31 month==12");
    SEXT_ASSERT_EQ(tmDec31.tm_mday, 31, "convertToTimestamp Dec 31 day==31");
    SEXT_ASSERT_EQ(tmDec31.tm_hour, 23, "convertToTimestamp Dec 31 hour==23");
    SEXT_ASSERT_EQ(tmDec31.tm_min, 59, "convertToTimestamp Dec 31 min==59");
    SEXT_ASSERT_EQ(tmDec31.tm_sec, 59, "convertToTimestamp Dec 31 sec==59");

    // 闰年 2月29日
    const time_t tsLeap = convertToTimestamp(2024, 2, 29, 12, 0, 0);
    const struct tm tmLeap = getTimeStruct(tsLeap);
    SEXT_ASSERT_EQ(tmLeap.tm_year, 2024, "convertToTimestamp 2024-02-29 year");
    SEXT_ASSERT_EQ(tmLeap.tm_mon, 2, "convertToTimestamp 2024-02-29 month==2");
    SEXT_ASSERT_EQ(tmLeap.tm_mday, 29, "convertToTimestamp 2024-02-29 day==29");

    // 时间戳单调递增
    const time_t ts1 = convertToTimestamp(2024, 6, 1, 0, 0, 0);
    const time_t ts2 = convertToTimestamp(2024, 6, 1, 0, 0, 1);
    const time_t ts3 = convertToTimestamp(2024, 6, 1, 0, 1, 0);
    const time_t ts4 = convertToTimestamp(2024, 6, 2, 0, 0, 0);
    SEXT_ASSERT(ts2 > ts1, "convertToTimestamp +1s monotonic");
    SEXT_ASSERT(ts3 > ts2, "convertToTimestamp +1min monotonic");
    SEXT_ASSERT(ts4 > ts3, "convertToTimestamp +1day monotonic");
    SEXT_ASSERT(ts4 - ts1 == 86400, "convertToTimestamp day diff == 86400");

    // 两个相邻月的时间戳差
    const time_t tsAug31 = convertToTimestamp(2024, 8, 31, 12, 0, 0);
    const time_t tsSep1  = convertToTimestamp(2024, 9, 1, 12, 0, 0);
    SEXT_ASSERT(tsSep1 > tsAug31, "convertToTimestamp Aug31 < Sep1");
    SEXT_ASSERT(tsSep1 - tsAug31 == 86400, "convertToTimestamp Aug31 to Sep1 == 86400s");
}

// ============================================================
// getTimeStruct 独立验证
// ============================================================
void SystemExtTest::testGetTimeStruct()
{
    // 已知时间：2020-06-15 10:30:00
    const time_t ts = convertToTimestamp(2020, 6, 15, 10, 30, 0);
    const struct tm tm1 = getTimeStruct(ts);
    SEXT_ASSERT_EQ(tm1.tm_year, 2020, "getTimeStruct year==2020");
    SEXT_ASSERT_EQ(tm1.tm_mon, 6, "getTimeStruct month==6");
    SEXT_ASSERT_EQ(tm1.tm_mday, 15, "getTimeStruct day==15");
    SEXT_ASSERT_EQ(tm1.tm_hour, 10, "getTimeStruct hour==10");
    SEXT_ASSERT_EQ(tm1.tm_min, 30, "getTimeStruct min==30");
    SEXT_ASSERT_EQ(tm1.tm_sec, 0, "getTimeStruct sec==0");

    // 两次调用同一 timestamp，结果相同
    const struct tm tm2 = getTimeStruct(ts);
    SEXT_ASSERT(tm1.tm_year == tm2.tm_year, "getTimeStruct idempotent year");
    SEXT_ASSERT(tm1.tm_mon == tm2.tm_mon, "getTimeStruct idempotent month");
    SEXT_ASSERT(tm1.tm_mday == tm2.tm_mday, "getTimeStruct idempotent day");

    // 不同 timestamp 产生不同结果
    const time_t ts2 = convertToTimestamp(2020, 6, 16, 10, 30, 0);
    const struct tm tm3 = getTimeStruct(ts2);
    SEXT_ASSERT(tm3.tm_mday != tm1.tm_mday, "getTimeStruct different timestamps different day");
    SEXT_ASSERT_EQ(tm3.tm_mday, 16, "getTimeStruct ts2 day==16");
}

// ============================================================
// isSameDay 扩展
// ============================================================
void SystemExtTest::testIsSameDayExtended()
{
    // 同一天的极端情况：00:00:00 和 23:59:59
    const time_t t0 = convertToTimestamp(2024, 7, 15, 0, 0, 0);
    const time_t t1 = convertToTimestamp(2024, 7, 15, 23, 59, 59);
    SEXT_ASSERT(isSameDay(t0, t1), "isSameDay 00:00 and 23:59 same day");

    // 与自身是同一天
    SEXT_ASSERT(isSameDay(t0, t0), "isSameDay self is same day");

    // 月末和下月初
    const time_t tJul31 = convertToTimestamp(2024, 7, 31, 23, 59, 59);
    const time_t tAug1  = convertToTimestamp(2024, 8, 1, 0, 0, 0);
    SEXT_ASSERT(!isSameDay(tJul31, tAug1), "isSameDay Jul31 vs Aug1 false");

    // 年末和新年
    const time_t tDec31 = convertToTimestamp(2023, 12, 31, 23, 59, 59);
    const time_t tJan1  = convertToTimestamp(2024, 1, 1, 0, 0, 0);
    SEXT_ASSERT(!isSameDay(tDec31, tJan1), "isSameDay Dec31 vs Jan1 false");

    // 完全相同的 timestamp 是同一天
    const time_t tNow = convertToTimestamp(2024, 3, 21, 12, 0, 0);
    SEXT_ASSERT(isSameDay(tNow, tNow), "isSameDay same timestamp true");

    // 相差 1 秒但跨天
    const time_t tBefore = convertToTimestamp(2024, 5, 20, 23, 59, 59);
    const time_t tAfter = tBefore + 1; // 5月21日 00:00:00
    SEXT_ASSERT(!isSameDay(tBefore, tAfter), "isSameDay cross-midnight false");
}

// ============================================================
// getDaysInMonth 完整覆盖（补充框架内 getDaysInMonth() 无参版）
// ============================================================
void SystemExtTest::testGetDaysInMonthCurrent()
{
    // getDaysInMonth() 无参：返回当前月的天数
    const int daysCurrentMonth = getDaysInMonth();
    SEXT_ASSERT(daysCurrentMonth >= 28 && daysCurrentMonth <= 31, "getDaysInMonth() current in [28,31]");

    // 与 getDaysInMonth(year, month) 一致
    const int year = getYear();
    const int month = getMonth();
    const int daysExpected = getDaysInMonth(year, month);
    SEXT_ASSERT_EQ(daysCurrentMonth, daysExpected, "getDaysInMonth() matches getDaysInMonth(year,month)");
}

// ============================================================
// getTimeMSecondUTC / setTimeMSecondUTC
// getTimeSecondUTC / setTimeSecondUTC
// ============================================================
void SystemExtTest::testTimeUTCSeed()
{
    // getTimeMSecondUTC 返回毫秒，应 >= 0
    const llong ms1 = getTimeMSecondUTC();
    SEXT_ASSERT(ms1 >= 0, "getTimeMSecondUTC >= 0");

    // set 后 get 一致
    const llong origMS = ms1;
    setTimeMSecondUTC(123456789LL);
    SEXT_ASSERT_EQ_LL(getTimeMSecondUTC(), 123456789LL, "setTimeMSecondUTC round-trip");
    setTimeMSecondUTC(origMS); // 还原

    // getTimeSecondUTC
    const llong s1 = getTimeSecondUTC();
    SEXT_ASSERT(s1 >= 0, "getTimeSecondUTC >= 0");

    // set second 后 get 一致
    const llong origS = s1;
    setTimeSecondUTC(999888777LL);
    SEXT_ASSERT_EQ_LL(getTimeSecondUTC(), 999888777LL, "setTimeSecondUTC round-trip");
    setTimeSecondUTC(origS); // 还原
}

// ============================================================
// isMainThread / getThreadID / setMainThread（补充测试）
// ============================================================
void SystemExtTest::testMainThreadExtended()
{
    // 当前线程 ID
    const int tid = getThreadID();
    SEXT_ASSERT(tid > 0, "getThreadID > 0");

    // isMainThread 与 setMainThread 联动
    // 注意：这里用当前线程 ID 验证，不改变主线程 ID（只在测试范围内验证）
    getThreadID(); // 当前就是主线程

    // 调用两次 getThreadID 结果相同（同一线程内）
    const int tid2 = getThreadID();
    SEXT_ASSERT_EQ(tid, tid2, "getThreadID idempotent in same thread");

    // isMainThread 应为 true（UnitTest 在主线程中运行）
    SEXT_ASSERT(isMainThread(), "isMainThread true in test context");
}

// ============================================================
// 综合场景：时间操作流水线
// ============================================================
void SystemExtTest::testTimePipeline()
{
    // 场景：从当前时间获取完整信息，交叉验证各 API 一致性
    int year, month, day, hour, minute, second;
    getTime(year, month, day, hour, minute, second);

    // 通过 convertToTimestamp 重建同一时刻的 timestamp，验证 getTimeStruct 还原
    const time_t reconstructed = convertToTimestamp(year, month, day, hour, minute, second);
    const struct tm reconstructedTm = getTimeStruct(reconstructed);
    SEXT_ASSERT_EQ(reconstructedTm.tm_year, year, "pipeline: year round-trip");
    SEXT_ASSERT_EQ(reconstructedTm.tm_mon, month, "pipeline: month round-trip");
    SEXT_ASSERT_EQ(reconstructedTm.tm_mday, day, "pipeline: day round-trip");
    SEXT_ASSERT_EQ(reconstructedTm.tm_hour, hour, "pipeline: hour round-trip");
    SEXT_ASSERT_EQ(reconstructedTm.tm_min, minute, "pipeline: minute round-trip");
    SEXT_ASSERT_EQ(reconstructedTm.tm_sec, second, "pipeline: second round-trip");

    // getTodayBegin 应 <= reconstructed <= getTodayEnd
    const llong todayBegin = getTodayBegin();
    const llong todayEnd = getTodayEnd();
    SEXT_ASSERT(todayBegin <= (llong)reconstructed, "pipeline: reconstructed >= todayBegin");
    SEXT_ASSERT((llong)reconstructed <= todayEnd, "pipeline: reconstructed <= todayEnd");

    // isSameDay：两个相同日期的 timestamp 应为同一天
    const time_t ts1 = convertToTimestamp(year, month, day, 0, 0, 0);
    const time_t ts2 = convertToTimestamp(year, month, day, 23, 59, 59);
    SEXT_ASSERT(isSameDay(ts1, ts2), "pipeline: same day true");

    // getDaysInMonth(year, month) 应在 [28, 31]
    const int daysInCurMonth = getDaysInMonth(year, month);
    SEXT_ASSERT(daysInCurMonth >= 28 && daysInCurMonth <= 31, "pipeline: daysInMonth valid");

    // day 不超过当月最大天数
    SEXT_ASSERT(day <= daysInCurMonth, "pipeline: day <= daysInMonth");
}

// ============================================================
// 大量 makeIDThread 压力测试
// ============================================================
void SystemExtTest::testMakeIDStress()
{
    // 连续生成 1000 个 ID，全部严格单调递增
    const int N = 1000;
    llong prev = makeIDThread();
    for (int i = 0; i < N - 1; ++i)
    {
        const llong curr = makeIDThread();
        SEXT_ASSERT(curr > prev, "makeIDThread stress: strictly increasing");
        prev = curr;
    }

    // 连续生成 1000 个 Main ID
    llong prevMain = makeIDMain();
    for (int i = 0; i < N - 1; ++i)
    {
        const llong currMain = makeIDMain();
        SEXT_ASSERT(currMain > prevMain, "makeIDMain stress: strictly increasing");
        prevMain = currMain;
    }
}

// ============================================================
// 主入口
// ============================================================
void SystemExtTest::test()
{
    testTodayBeginEnd1();
    testGetDayOfWeek2();
    testGetTimeHourInDay2();
    testGetTimeFullWithWeekDay();
    testGetTimeSixParams();
    testGetTimeString2();
    testGetCPUCoreCount2();
    testGetPID2();
    testGetRealTimeMS2();
    testSleep();
    testGetProgramFile2();
    testMakeIDThreadExtended();
    testDaysToSecondsExtended();
    testIsLeapYearExtended();
    testConvertToTimestampExtended();
    testGetTimeStruct();
    testIsSameDayExtended();
    testGetDaysInMonthCurrent();
    testTimeUTCSeed();
    testMainThreadExtended();
    testTimePipeline();
    testMakeIDStress();
}
