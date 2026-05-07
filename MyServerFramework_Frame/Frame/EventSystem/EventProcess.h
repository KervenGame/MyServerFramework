#pragma once

#include "ClassObject.h"
#include "GameEvent.h"

class EventProcessBase
{
public:
	virtual void onEventInternal(GameEvent* event, IEventListener* listener) = 0;
};

// 监听事件需要使用EventProcess对象来处理事件,确保事件类型不会出错,这里别换行,不然代码自动生成的时候会误以为此类不是模板类
template<typename T0, typename T1, typename TypeCheck0 = typename IsSubClassOf<GameEvent, T0>::mType, typename TypeCheck1 = typename IsSubClassOf<IEventListener, T1>::mType>
class EventProcess : public EventProcessBase
{
	BASE(EventProcess, EventProcessBase);
public:
    static void call(GameEvent* event, IEventListener* listener)
    {
        static_cast<T1*>(listener)->onEvent(static_cast<T0*>(event));
    }
};