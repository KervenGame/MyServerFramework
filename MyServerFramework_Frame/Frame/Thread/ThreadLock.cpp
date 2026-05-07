#include "FrameHeader.h"

atomic<uint64_t> ThreadLock::mLockTicks = 0;
atomic<uint64_t> ThreadLock::mLockTimes = 0;
uint64_t ThreadLock::mStartTimeTick = 0;

void ThreadLock::waitForUnlock(const char* file, const int line)
{
	if (!mEnable)
	{
		return;
	}
#ifdef WINDOWS
	llong start = readTSC();
#endif
	++mLockTimes;
	// 原子自旋操作
	// 如果mLock当前为true,则设置为true,并继续等待,如果mLock为false,则设置为true,不再循环等待
	while (mLock.test_and_set(memory_order_acquire))
	{
		SystemUtility::sleep(0);
	}
#ifdef WINDOWS
	const uint64_t end = readTSC();
	mLockTicks.fetch_add(end - start, memory_order_relaxed);
	double ms = Profiler::ticksToMS(end - start);
	if (ms > 5)
	{
		LOG_NO_WRITE("锁等待时间:" + FToS((float)ms, 1) +
			"毫秒,file:" + getFileName(file) + ":" + IToS(line) + 
			", 当前锁文件:" + getFileName(mFile) + ":" + IToS(mLine));
	}
	mFile = file;
#endif
	mLine = line;
	mThreadID = getThreadID();
}

void ThreadLock::unlock()
{
	if (!mEnable)
	{
		return;
	}
	mLock.clear(memory_order_release);
}

void ThreadLock::dump()
{
#ifdef STRESS_TEST
	const uint64_t ticks = mLockTicks.exchange(0);
	if (mStartTimeTick > 0)
	{
		const uint64_t end = readTSC();
		const double elapsedSecond = Profiler::ticksToMS(end - mStartTimeTick) / 1000.0;
		if (elapsedSecond > 1e-6)
		{
			const float lockTimePerSecond = (float)(Profiler::ticksToMS(ticks) / elapsedSecond);
			LOG("平均每秒锁等待总时间:" + FToS(lockTimePerSecond) + "毫秒");
		}
		LOG("平均每秒加锁次数:" + LLToS(mLockTimes));
	}
#else
	mLockTicks.exchange(0);
#endif
	mLockTimes = 0;
	mStartTimeTick = readTSC();
}