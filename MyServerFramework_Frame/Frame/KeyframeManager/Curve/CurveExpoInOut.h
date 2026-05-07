#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveExpoInOut : public AnimationCurve
{
	BASE(CurveExpoInOut, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		if (isZero(time))
		{
			return 0.0f;
		}
		if (isEqual(time, 1.0f))
		{
			return 1.0f;
		}
		if (time < 0.5f)
		{
			return 0.5f * pow(2.0f, 20.0f * time - 10.0f);
		}
		else
		{
			return 0.5f * (2.0f - pow(2.0f, -20.0f * time + 10.0f));
		}
	}
};