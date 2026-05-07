#include "FrameHeader.h"

void ClassPoolBase::init()
{
	mServerFramework->registeHour(this, [this] {onHour(); });
	mServerFramework->registeSecond(this, [this] {onSecond(); });
}

void ClassPoolBase::quit()
{
	mServerFramework->unregisteHour(this);
	mServerFramework->unregisteSecond(this);
}