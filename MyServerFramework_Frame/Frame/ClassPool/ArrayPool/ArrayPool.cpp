#include "FrameHeader.h"

ArrayPoolSingle* ArrayPool::mArrayPoolSingle = new ArrayPoolSingle();
ArrayPoolSingleChar* ArrayPool::mArrayPoolSingleChar = new ArrayPoolSingleChar();

void ArrayPool::init()
{
	mServerFramework->registeHour(this, [this] { onHour(); });
}

void ArrayPool::quit()
{
	mServerFramework->unregisteHour(this);
}

void ArrayPool::onHour()
{
	mArrayPoolSingle->dump();
	mArrayPoolSingleChar->dump();
}

ArrayPool::~ArrayPool()
{
	DELETE(mArrayPoolSingle);
	DELETE(mArrayPoolSingleChar);
}