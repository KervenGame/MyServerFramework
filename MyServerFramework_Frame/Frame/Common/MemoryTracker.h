#pragma once

#include <cstdio>
#include <cstdlib>

// 需要检查内存时就打开这个宏
//#define _MEMORY_TRACK

// 只能在MemoryOperation中调用
class MICRO_LEGEND_FRAME_API MemoryTracker
{
public:
    static void NewInstance()
    {
        if (mInstance == nullptr)
        {
            mInstance = new MemoryTracker();
            mInstance->mMainThreadId = getThreadID();
        }
    }
    static MemoryTracker* Get() { return mInstance; }
    void RegisterAlloc(void* ptr, size_t size, const char* file, int line);
    void RegisterFree(void* ptr);
    void DumpReport(const char* filename);
    bool IsMainThread() const { return getThreadID() == mMainThreadId; }
    static int getThreadID()
    {
#ifdef WINDOWS
        return (int)GetCurrentThreadId();
#elif defined LINUX
        return (int)pthread_self();
#endif
    }
    size_t Hash(void* p) const { return ((size_t)p >> 3) & (HASH_SIZE - 1); }
private:
    struct AllocNode
    {
        void* ptr;
        size_t size;
        const char* file;
        int line;
        AllocNode* next;
    };
    MemoryTracker() { memset(mBuckets, 0, sizeof(mBuckets)); }
    bool mIsSelfAlloc = false;
    int mMainThreadId = 0;
    unsigned long long mTotalAllocated = 0;
    static const size_t HASH_SIZE = 1 << 22; // 4M桶
    AllocNode* mBuckets[HASH_SIZE];
    static MemoryTracker* mInstance;
};