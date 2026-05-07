#pragma once

#include "ClassObject.h"
#include "ClassPool.h"
#include "FrameCallback.h"

class MICRO_LEGEND_FRAME_API EventInfo : public ClassObject
{
	BASE(EventInfo, ClassObject);
public:
	void resetProperty() override
	{
		base::resetProperty();
		mType = 0;
		mCharacterGUID = 0;
		mCallback = nullptr;
		mListener = nullptr;
	}
public:
	int mType = 0;
	llong mCharacterGUID = 0;
	EventCallback mCallback;
	IEventListener* mListener = nullptr;
};

CLASS_POOL(EventInfo);