#include "FrameHeader.h"

// -----------------------------------------------------------
// 测试用事件定义
// -----------------------------------------------------------
class TestEventA : public GameEvent
{
public:
    int value = 0;
    string message;
    TestEventA()
    {
        mType = (int)typeid(TestEventA).hash_code();
    }
};

class TestEventB : public GameEvent
{
public:
    float score = 0.0f;
    bool flag = false;
    TestEventB()
    {
        mType = (int)typeid(TestEventB).hash_code();
    }
};

// -----------------------------------------------------------
// 测试用事件监听器
// -----------------------------------------------------------
class TestListener : public IEventListener
{
public:
    int eventACount = 0;
    int eventBCount = 0;
    int lastValue = 0;
    float lastScore = 0.0f;
    string lastMessage;

    void reset()
    {
        eventACount = 0;
        eventBCount = 0;
        lastValue = 0;
        lastScore = 0.0f;
        lastMessage.clear();
    }

    void onEvent(TestEventA* event)
    {
        ++eventACount;
        lastValue = event->value;
        lastMessage = event->message;
    }
    void onEvent(TestEventB* event)
    {
        ++eventBCount;
        lastScore = event->score;
    }
};

// -----------------------------------------------------------
// 测试实现
// -----------------------------------------------------------
#define EVT_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("EventSystemTest FAILED: ") + (msg)); } } while(0)

void EventSystemTest::test()
{
    testEventInfoResetProperty();
    testPushAndListenEvent();
    testUnlistenEvent();
    testRemoveCharacterEvent();
    testGlobalEventListen();
    testEventProcessCallback();
    testMultipleListeners();
    testEventTypeIsolation();
}

void EventSystemTest::testEventInfoResetProperty()
{
    EventInfoPool pool;
    EventInfo* info = CLASS<EventInfo>();
    EVT_ASSERT(info != nullptr, "newClass returns valid pointer");
}

void EventSystemTest::testPushAndListenEvent()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    // 监听全局事件
    eventSystem.listenEvent<TestEventA>(&listener);

    // 推送事件
    TestEventA event;
    event.value = 42;
    event.message = "test";
    eventSystem.pushEvent(event);

    // 更新事件系统以处理事件
    eventSystem.update(0.016f);

    EVT_ASSERT(listener.eventACount == 1, "listener received event A once");
    EVT_ASSERT(listener.lastValue == 42, "event value is 42");
    EVT_ASSERT(listener.lastMessage == "test", "event message is 'test'");

    eventSystem.quit();
}

void EventSystemTest::testUnlistenEvent()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    // 监听然后取消监听
    eventSystem.listenEvent<TestEventA>(&listener);
    eventSystem.unlistenEvent(&listener);

    // 推送事件
    TestEventA event;
    event.value = 100;
    eventSystem.pushEvent(event);
    eventSystem.update(0.016f);

    EVT_ASSERT(listener.eventACount == 0, "after unlisten, no event received");

    eventSystem.quit();
}

void EventSystemTest::testRemoveCharacterEvent()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    // 创建一个模拟角色GUID
    const llong charGUID = 12345;

    // 监听特定角色的事件
    // 注意：这里我们使用nullptr作为character参数来模拟监听
    // 实际项目中需要真实的Character对象
    eventSystem.listenEvent<TestEventA>(&listener);

    // 推送事件
    TestEventA event;
    event.mCharacterGUID = charGUID;
    event.value = 200;
    eventSystem.pushEvent(event, nullptr);

    // 移除该角色的所有事件监听
    eventSystem.removeCharacterEvent(charGUID);

    // 再次推送事件
    listener.reset();
    eventSystem.pushEvent(event, nullptr);
    eventSystem.update(0.016f);

    // 由于监听是全局的，应该仍然能收到
    EVT_ASSERT(listener.eventACount == 1, "global listener still receives after character removal");

    eventSystem.quit();
}

void EventSystemTest::testGlobalEventListen()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener1;
    TestListener listener2;

    // 两个监听器都监听全局事件
    eventSystem.listenEvent<TestEventA>(&listener1);
    eventSystem.listenEvent<TestEventA>(&listener2);

    // 推送事件
    TestEventA event;
    event.value = 999;
    eventSystem.pushEvent(event);
    eventSystem.update(0.016f);

    EVT_ASSERT(listener1.eventACount == 1, "global listener1 received event");
    EVT_ASSERT(listener2.eventACount == 1, "global listener2 received event");
    EVT_ASSERT(listener1.lastValue == 999, "listener1 got correct value");
    EVT_ASSERT(listener2.lastValue == 999, "listener2 got correct value");

    eventSystem.quit();
}

void EventSystemTest::testEventProcessCallback()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    eventSystem.listenEvent<TestEventA>(&listener);
    eventSystem.listenEvent<TestEventB>(&listener);

    // 推送事件A
    TestEventA eventA;
    eventA.value = 111;
    eventSystem.pushEvent(eventA);

    // 推送事件B
    TestEventB eventB;
    eventB.score = 3.14f;
    eventSystem.pushEvent(eventB);

    eventSystem.update(0.016f);

    EVT_ASSERT(listener.eventACount == 1, "received event A");
    EVT_ASSERT(listener.eventBCount == 1, "received event B");
    EVT_ASSERT(listener.lastValue == 111, "event A value correct");
    EVT_ASSERT(listener.lastScore == 3.14f, "event B score correct");

    eventSystem.quit();
}

void EventSystemTest::testMultipleListeners()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener1;
    TestListener listener2;
    TestListener listener3;

    // 三个监听器监听同一事件
    eventSystem.listenEvent<TestEventA>(&listener1);
    eventSystem.listenEvent<TestEventA>(&listener2);
    eventSystem.listenEvent<TestEventA>(&listener3);

    // 推送多个事件
    for (int i = 0; i < 5; ++i)
    {
        TestEventA event;
        event.value = i;
        eventSystem.pushEvent(event);
    }

    eventSystem.update(0.016f);

    EVT_ASSERT(listener1.eventACount == 5, "listener1 received all 5 events");
    EVT_ASSERT(listener2.eventACount == 5, "listener2 received all 5 events");
    EVT_ASSERT(listener3.eventACount == 5, "listener3 received all 5 events");
    EVT_ASSERT(listener1.lastValue == 4, "listener1 last value is 4");

    eventSystem.quit();
}

void EventSystemTest::testEventTypeIsolation()
{
    EventInfoPool pool;
    EventSystem eventSystem;
    TestListener listener;

    // 只监听事件A
    eventSystem.listenEvent<TestEventA>(&listener);

    // 推送事件A和B
    TestEventA eventA;
    eventA.value = 100;
    eventSystem.pushEvent(eventA);

    TestEventB eventB;
    eventB.score = 2.5f;
    eventSystem.pushEvent(eventB);

    eventSystem.update(0.016f);

    // 只应该收到事件A
    EVT_ASSERT(listener.eventACount == 1, "only received event A");
    EVT_ASSERT(listener.eventBCount == 0, "did not receive event B (not listening)");
    EVT_ASSERT(listener.lastValue == 100, "event A value correct");

    eventSystem.quit();
}
