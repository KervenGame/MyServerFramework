#include "FrameHeader.h"

void StateParam::resetProperty()
{
	base::resetProperty();
	mTarget = nullptr;
	mSource = nullptr;
	mSourceCharacterGUID = 0;
	mType = 0;
	mStateTime = -1.0f;
	// mParamSet构造中填充的,不重置
	//mParamSet.resetProperty();
}