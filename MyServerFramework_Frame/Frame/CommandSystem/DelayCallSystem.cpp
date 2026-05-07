#include "FrameHeader.h"

void DelayCallSystem::lateUpdate(const float elapsedTime)
{
	// 从主线程发起的延迟调用
	mCacheDelayCallList = move(mDelayCallList);
	for (VoidCallback call : mCacheDelayCallList)
	{
		CALL(call);
	}
	mCacheDelayCallList.clear();

	// 从子线程发起的延迟调用
	if (!mDelayCallCrossMainList.isEmpty())
	{
		THREAD_LOCK(mDelayCallCrossMainListLock);
		mCacheDelayCallCrossMainList = move(mDelayCallCrossMainList);
	}
	for (VoidCallback call : mCacheDelayCallCrossMainList)
	{
		CALL(call);
	}
	mCacheDelayCallCrossMainList.clear();
}

void DelayCallSystem::addCall(VoidCallback callback)
{
	checkMainThread();
	mDelayCallList.add(callback);
}

void DelayCallSystem::addCallCrossThread(VoidCallback callback)
{
	THREAD_LOCK(mDelayCallCrossMainListLock);
	mDelayCallCrossMainList.add(callback);
}