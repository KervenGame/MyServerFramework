#include "FrameHeader.h"

void Ticker::update(const float elapsedTime)
{
	if (mCallback != nullptr && !mFinish)
	{
		mFinish = !mCallback();
	}
}

void Ticker::resetProperty()
{
	base::resetProperty();
	mCallback = nullptr;
	mFinish = false;
}