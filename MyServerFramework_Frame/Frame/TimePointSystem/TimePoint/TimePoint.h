#pragma once

#include "ClassObject.h"
#include "TimeValue.h"

class MICRO_LEGEND_FRAME_API TimePoint : public ClassObject
{
	BASE(TimePoint, ClassObject);
public:
	virtual void init(){}
	virtual void update(float elapsedTime);
	virtual void lateUpdate(float elapsedTime);
	void resetProperty() override;
	// 一般是lambda
	void setCallback(VoidCallback callback) { mCallback = callback; }
protected:
	void arrivalTime();
protected:
	VoidCallback mCallback = nullptr;		// 到时间的回调,两个回调设置一个即可,如果两个都设置了,优先调用mCallback
	TimeValue mTime;
	float mArrivalCD = -1.0f;				// 因为时间只能精确到秒,所以需要加一个2秒内只能触发一次mArrivalTime为true的限制
	bool mArrivalTime = false;
};