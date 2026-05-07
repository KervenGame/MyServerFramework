#pragma once

#include "ThreadLock.h"
#include "CommandPool.h"
#include "FrameComponent.h"
#include "GameCommand.h"

class MICRO_LEGEND_FRAME_API DelayCallSystem : public FrameComponent
{
	BASE(DelayCallSystem, FrameComponent);
public:
	void lateUpdate(float elapsedTime) override;
	// 从主线程添加延迟调用,在主线程下一次调用update时调用
	void addCall(VoidCallback callback);
	// 从子线程添加延迟调用,并且会在主线程被调用,如果想要在子线程执行调用,则直接使用CustomThread的addCall即可
	void addCallCrossThread(VoidCallback callback);
protected:
	Vector<VoidCallback> mDelayCallList;					// 延迟命令列表,只允许主线程调用
	Vector<VoidCallback> mCacheDelayCallList;				// 延迟命令列表,用于每帧复制一份来执行,因为执行列表时可能还会添加命令
	Vector<VoidCallback> mDelayCallCrossMainList;			// 延迟命令列表,在子线程添加,主线程执行调用
	Vector<VoidCallback> mCacheDelayCallCrossMainList;		// 延迟命令列表,用于每帧复制一份来执行,减少加锁时间
	ThreadLock mDelayCallCrossMainListLock;					// mDelayCallCrossMainList的线程锁
};