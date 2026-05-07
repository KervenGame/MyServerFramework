#pragma once

#include "Singleton.h"
#include "ThreadLock.h"
#include "Map.h"
#include "Vector.h"
#include "Set.h"
#include "Utility.h"

class MICRO_LEGEND_FRAME_API ServerFramework : public Singleton<ServerFramework>
{
	BASE(ServerFramework, Singleton<ServerFramework>);
public:
	ServerFramework();
	virtual ~ServerFramework() { destroy(); }
	virtual void init();
	void destroy();
	virtual void update(float elapsedTime);
	virtual void launch();
	bool isStop() const { return mStop; }
	void stop() { mStop = true; }
	llong getFrameID() const { return mFrameID; }
	void addFrameTime(llong frameTimeMS)
	{
		mAllFrameMS += frameTimeMS; 
		++mAllFrameCount;
	}
	// 获得成员变量
#ifdef LINUX
	const ulong & getStartMiliTime() const { return mStartMiliTime; }
#endif
	template<typename T, typename TypeCheck = typename IsSubClassOf<FrameComponent, T>::mType>
	T* getSystem(const string& name) const
	{
		return static_cast<T*>(mFrameComponentMap.get(name));
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<FrameComponent, T>::mType>
	void getSystem(const string& name, T*& component) const
	{
		component = static_cast<T*>(mFrameComponentMap.get(name));
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<FrameComponent, T>::mType>
	void registeSystem(const char* name)
	{
		T* component = new T();
		component->setName(name);
		mFrameComponentVector.add(component);
		mFrameComponentMap.add(name, component);
		if (component->isClassPool())
		{
			mPoolSystemList.add(component);
		}
		else if (component->isFactory())
		{
			mFactorySystemList.add(component);
		}
	}
	void registeSecond(void* obj, VoidCallback callback)	{ mSecondCallback.add(obj, callback); }
	void unregisteSecond(void* obj)							{ mSecondCallback.remove(obj); }
	void registeMinute(void* obj, VoidCallback callback)	{ mMinuteCallback.add(obj, callback); }
	void unregisteMinute(void* obj)							{ mMinuteCallback.remove(obj); }
	void registeHour(void* obj, VoidCallback callback)		{ mHourCallback.add(obj, callback); }
	void unregisteHour(void* obj)							{ mHourCallback.remove(obj); }
	void registeDay(void* obj, VoidCallback callback)		{ mDayCallback.add(obj, callback); }
	void unregisteDay(void* obj)							{ mDayCallback.remove(obj); }
	void registeWeek(void* obj, VoidCallback callback)		{ mWeekCallback.add(obj, callback); }
	void unregisteWeek(void* obj)							{ mWeekCallback.remove(obj); }
protected:
	virtual void registeComponent();
	virtual void constructDone();
	virtual void clearSystem();
	virtual void registe();
protected:
	Vector<FrameComponent*> mFrameComponentVector;
	HashMap<string, FrameComponent*> mFrameComponentMap;
	HashMap<void*, VoidCallback> mSecondCallback;
	HashMap<void*, VoidCallback> mMinuteCallback;
	HashMap<void*, VoidCallback> mHourCallback;
	HashMap<void*, VoidCallback> mDayCallback;
	HashMap<void*, VoidCallback> mWeekCallback;
	Vector<FrameComponent*> mPoolSystemList;
	Vector<FrameComponent*> mFactorySystemList;
#ifdef LINUX
	ulong mStartMiliTime;
#endif
	atomic<bool> mStop;
	ThreadLock mLock;
	llong mFrameID = 0;
	int mFPS = 0;
	int mCurFrameCount = 0;
	int mLastDayOfWeek = 0;
	llong mAllFrameMS = 0;			// 用于计算每帧的平均耗时
	llong mAllFrameCount = 0;		// 用于计算每帧的平均耗时
};