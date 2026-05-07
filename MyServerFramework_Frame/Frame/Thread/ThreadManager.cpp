#include "FrameHeader.h"

ThreadManager::~ThreadManager()
{
	if (!mThreadList.isEmpty())
	{
		print(("有" + UIToS(mThreadList.size()) + "个线程未正常关闭").c_str());
	}
	// 强制关闭所有线程
	DELETE_MAP(mThreadList);
}

CustomThread* ThreadManager::createThread(const string& name, VoidCallback callback, VoidCallback preCmdCallback, VoidCallback endCmdCallback)
{
	CustomThread* thread = new CustomThread(name, callback, preCmdCallback, endCmdCallback);
	mThreadList.add(thread->getThreadHandle(), thread);
	return thread;
}

void ThreadManager::destroyThread(CustomThread*& thread)
{
	if (thread == nullptr)
	{
		return;
	}
	mThreadList.remove(thread->getThreadHandle());
	DELETE(thread);
}