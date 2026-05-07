#include "FrameHeader.h"

HashMap<int, ArrayPoolSingle*> ArrayPoolThread::mArrayPoolSingleList;
HashMap<int, ArrayPoolSingleChar*> ArrayPoolThread::mArrayPoolSingleCharList;
ThreadLock ArrayPoolThread::mLock;
ThreadLock ArrayPoolThread::mCharLock;

void ArrayPoolThread::init()
{
	mServerFramework->registeHour(this, [this] {onHour(); });
}

void ArrayPoolThread::quit()
{
	mServerFramework->unregisteHour(this);
}

ArrayPoolThread::~ArrayPoolThread()
{
	DELETE_MAP(mArrayPoolSingleList);
}

char* ArrayPoolThread::newCharArray(const int count, const bool zeroMemory)
{
	ArrayPoolSingleChar* threadArrayMemory = nullptr;
	// 找到指定线程的内存列表
	const int threadID = getThreadID();
	{
		THREAD_LOCK(mCharLock);
		threadArrayMemory = mArrayPoolSingleCharList.get(threadID);
		if (threadArrayMemory == nullptr)
		{
			threadArrayMemory = new ArrayPoolSingleChar();
			mArrayPoolSingleCharList.add(threadID, threadArrayMemory);
		}
	}
	// 在线程内存列表中找到指定类型的内存列表
	return threadArrayMemory->get(count, zeroMemory);
}

void ArrayPoolThread::deleteCharArray(char*& data, const int size)
{
	ArrayPoolSingleChar* threadArrayMemory = nullptr;
	{
		THREAD_LOCK(mCharLock);
		threadArrayMemory = mArrayPoolSingleCharList.get(getThreadID());
	}
	if (threadArrayMemory != nullptr)
	{
		threadArrayMemory->release(data, size);
	}
	data = nullptr;
}

void ArrayPoolThread::onHour()
{
	LOG("开始dumpArrayPoolThread");
	for (const auto& item : mArrayPoolSingleList)
	{
		LOG("ThreadID:" + IToS(item.first));
		item.second->dump();
	}
	for (const auto& item : mArrayPoolSingleCharList)
	{
		LOG("ThreadID:" + IToS(item.first));
		item.second->dump();
	}
	LOG("结束dumpArrayPoolThread");
}