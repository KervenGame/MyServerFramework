#include "FrameHeader.h"

void TickerSystem::quit()
{
	UN_CLASS(mTickerList);
}

void TickerSystem::update(const float elapsedTime)
{
	SAFE_VECTOR_SCOPE(mTickerList, readList);
	for (Ticker* cor : readList)
	{
		cor->update(elapsedTime);
		if (cor->isFinish())
		{
			mTickerList.remove(cor);
			UN_CLASS(cor);
		}
	}
}

void TickerSystem::createTicker(BoolFunction callback)
{
	Ticker* cor = CLASS<Ticker>();
	cor->setCallback(callback);
	mTickerList.add(cor);
}