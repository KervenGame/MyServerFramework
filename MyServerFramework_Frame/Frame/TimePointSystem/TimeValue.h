#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API TimeValue
{
public:
	void clear();
	void refreshTime() { getTime(mWeekDay, mYear, mMonth, mDay, mHour, mMinute, mSecond); }
	static bool isTimeInRangeInHour(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	static bool isTimeInRangeInDay(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	static bool isTimeInRangeInWeek(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	static bool isTimeInRangeInMonth(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	static bool isTimeInRangeInYear(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	bool operator>(const TimeValue& other) const
	{
		if (mYear != other.mYear)
		{
			return mYear > other.mYear;
		}
		if (mMonth != other.mMonth)
		{
			return mMonth > other.mMonth;
		}
		if (mDay != other.mDay)
		{
			return mDay > other.mDay;
		}
		if (mHour != other.mHour)
		{
			return mHour > other.mHour;
		}
		if (mMinute != other.mMinute)
		{
			return mMinute > other.mMinute;
		}
		return mSecond > other.mSecond;
	}
public:
	int mWeekDay = 0;		// 0到6分别表示周日到周六,比如周一就是1,周二就是2,周日是0
	int mYear = 0;
	int mMonth = 0;
	int mDay = 0;
	int mHour = 0;
	int mMinute = 0;
	int mSecond = 0;
};