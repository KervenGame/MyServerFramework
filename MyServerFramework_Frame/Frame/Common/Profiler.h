#pragma once

#include "FrameDefine.h"
#include "ProfilerData.h"

#if defined(_MSC_VER)
#include <intrin.h>
inline uint64_t readTSC() { return __rdtsc(); }
#elif defined(__x86_64__)
inline uint64_t readTSC() 
{
    unsigned hi, lo;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}
#else
inline uint64_t readTSC() 
{
    return (uint64_t)high_resolution_clock::now().time_since_epoch().count();
}
#endif

// 需要搭配PROFILE宏来使用
class MICRO_LEGEND_FRAME_API Profiler
{
public:
    static constexpr int MAX_POINTS = 8192;
    static ProfilerData mData[MAX_POINTS];
    static double mTickToNS;
    static double mTickToMS;
    static ullong mTickPerMS;
    static ullong mTickPerNS;
public:
    static void initTicks()
    {
        const auto t1 = readTSC();
        sleep_for(milliseconds(100));
        const auto t2 = readTSC();
        mTickPerMS = (t2 - t1) / 100;
        mTickToNS = 1e8 / (t2 - t1);
        mTickToMS = 1e2 / (t2 - t1);
    }
    static double getTickToNS()
    {
        if (mTickToNS > 0.0)
        {
            return mTickToNS;
        }
        initTicks();
        return mTickToNS;
    }
    static double getTickToMS()
    {
        if (mTickToMS > 0.0)
        {
            return mTickToMS;
        }
        initTicks();
        return mTickToMS;
    }
    static double ticksToMS(ullong ticks)
    {
        return ticks * getTickToMS();
    }
    static double ticksToMS(ullong ticks, double tickToMS)
    {
        return ticks * tickToMS;
    }
    static inline ProfilerData& get(int id, const char* file, const char* func)
    {
        ProfilerData& data = mData[id];
        if (data.mFileName != nullptr && data.mFileName != file)
        {
            ERROR("有重复的行号:" + IToS(id));
        }
        data.mFileName = file;
        data.mFunction = func;
        return data;
    }
    static void resetFrame() 
    {
        FOR(MAX_POINTS)
        {
            ProfilerData& data = mData[i];
            data.mTotalTicks = 0;
            data.mCount = 0;
            data.mFileName = nullptr;
            data.mFunction = nullptr;
        }
    }
    static void dump()
    {
#ifdef WINDOWS
        bool hasData = false;
        FOR(MAX_POINTS)
        {
            ProfilerData& data = mData[i];
            if (data.mCount > 0)
            {
                hasData = true;
                break;
            }
        }
        if (!hasData)
        {
            return;
        }

        const double tickToMS = getTickToMS();
        LOG("profile start--------------------------------------------------------------------");
        FOR(MAX_POINTS)
        {
            ProfilerData& data = mData[i];
            if (data.mCount > 0)
            {
                string funcName = string(data.mFunction);
                const int firstColonPos = (int)funcName.find_first_of(':');
                const int firstBracePos = (int)funcName.find_first_of('(', firstColonPos);
                funcName = funcName.substr(firstColonPos + 2, firstBracePos - firstColonPos - 2);
                string preStr = getFileName(data.mFileName) + "." + funcName + ":" + IToS(i);
                string endStr = " time:" + FToS((float)ticksToMS(data.mTotalTicks, tickToMS)) + " ms, count:" + LLToS(data.mCount);
                appendWithAlign(preStr, endStr, 56);
                LOG(preStr);
            }
        }
        LOG("profile end----------------------------------------------------------------------");
#endif
    }
};