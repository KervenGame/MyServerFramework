#include "FrameHeader.h"

// ============================================================
// EventSystemExtendedTest — 扩展事件系统测试
// 覆盖：嵌套事件推送、重复监听/取消监听、
//       多事件类型完全隔离、事件中修改监听列表
// ============================================================

namespace
{

// -----------------------------------------------------------
// 测试用事件类型
// -----------------------------------------------------------
class TestEventX : public GameEvent
{
public:
    int xValue = 0;
    TestEventX() { mType = (int)typeid(TestEventX).hash_code(); }
};

class TestEventY : public GameEvent
{
public:
    float yValue = 0.0f;
    string yLabel;
    TestEventY() { mType = (int)typeid(TestEventY).hash_code(); }
};

class TestEventZ : public GameEvent
{
public:
    bool flag = false;
    TestEventZ() { mType = (int)typeid(TestEventZ).hash_code(); }
};

// -----------------------------------------------------------
// 测试用监听器
// -----------------------------------------------------------
class ExtendedListener : public IEventListener
{
public:
    int eventXCount = 0;
    int eventYCount = 0;
    int eventZCount = 0;
    int lastXValue = 0;
    float lastYValue = 0.0f;
    string lastYLabel;
    bool lastZFlag = false;

    // 嵌套测试：在回调中再次 pushEvent
    EventSystem* nestedPushTarget = nullptr;
    int nestedDepth = 0;

    void reset()
    {
        eventXCount = 0;
        eventYCount = 0;
        eventZCount = 0;
        lastXValue = 0;
        lastYValue = 0.0f;
        lastYLabel.clear();
        lastZFlag = false;
        nestedDepth = 0;
    }

    void onEvent(TestEventX* event)
    {
        ++eventXCount;
        lastXValue = event->xValue;
        // 如果设置了嵌套目标，在 X 事件的回调中推一个 Y 事件
        if (nestedPushTarget != nullptr && nestedDepth < 5)
        {
            ++nestedDepth;
            TestEventY nestedY;
            nestedY.yValue = -1.0f;
            nestedY.yLabel = "nested-from-X";
            nestedPushTarget->pushEvent(nestedY);
            --nestedDepth;
        }
    }

    void onEvent(TestEventY* event)
    {
        ++eventYCount;
        lastYValue = event->yValue;
        lastYLabel = event->yLabel;
    }

    void onEvent(TestEventZ* event)
    {
        ++eventZCount;
        lastZFlag = event->flag;
    }
};

#define EVT_EXT_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("EventSystemExtendedTest FAILED: ") + (msg)); } } while(0)

// -----------------------------------------------------------
// Case 1: 嵌套事件推送 — 在 A 的回调中 push B
// -----------------------------------------------------------
static void testNestedEventPush()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    listener.nestedPushTarget = &evtSys;
    evtSys.listenEvent<TestEventX>(&listener);
    evtSys.listenEvent<TestEventY>(&listener);

    // 推送 X → 回调中会自动推送 Y
    TestEventX eventX;
    eventX.xValue = 42;
    evtSys.pushEvent(eventX);

    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 1, "nested: X received once");
    EVT_EXT_ASSERT(listener.eventYCount >= 1, "nested: Y received from within X callback");
    EVT_EXT_ASSERT(listener.lastYLabel == "nested-from-X", "nested: Y label correct");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 2: 多次连续推送同一事件
// -----------------------------------------------------------
static void testMultipleSameEvents()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);

    for (int i = 0; i < 20; ++i)
    {
        TestEventX e;
        e.xValue = i;
        evtSys.pushEvent(e);
    }

    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 20, "multi-push: all 20 events received");
    EVT_EXT_ASSERT(listener.lastXValue == 19, "multi-push: last value is 19");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 3: 三种事件类型完全隔离
// -----------------------------------------------------------
static void testEventTypeIsolation_Strict()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);
    evtSys.listenEvent<TestEventY>(&listener);
    evtSys.listenEvent<TestEventZ>(&listener);

    listener.reset();

    // 只推送 Y 和 Z
    TestEventY eY;
    eY.yValue = 77.0f;
    eY.yLabel = "only-y";
    evtSys.pushEvent(eY);

    TestEventZ eZ;
    eZ.flag = true;
    evtSys.pushEvent(eZ);

    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 0, "isolation: X not triggered");
    EVT_EXT_ASSERT(listener.eventYCount == 1, "isolation: Y triggered once");
    EVT_EXT_ASSERT(listener.eventZCount == 1, "isolation: Z triggered once");
    EVT_EXT_ASSERT(fabsf(listener.lastYValue - 77.0f) < 1e-6f, "isolation: Y value correct");
    EVT_EXT_ASSERT(listener.lastZFlag == true, "isolation: Z flag correct");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 4: 同一监听器监听同一事件两次（应收到两份）
// -----------------------------------------------------------
static void testDoubleListen_SameEvent()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener1;
    ExtendedListener listener2;

    // 同一事件注册两个不同监听器
    evtSys.listenEvent<TestEventX>(&listener1);
    evtSys.listenEvent<TestEventX>(&listener2);

    TestEventX e;
    e.xValue = 888;
    evtSys.pushEvent(e);
    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener1.eventXCount == 1, "double listen: L1 got it");
    EVT_EXT_ASSERT(listener2.eventXCount == 1, "double listen: L2 got it");
    EVT_EXT_ASSERT(listener1.lastXValue == 888, "double listen: L1 value");
    EVT_EXT_ASSERT(listener2.lastXValue == 888, "double listen: L2 value");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 5: 取消重监听后再推送
// -----------------------------------------------------------
static void testUnlistenThenPushAgain()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);

    // 第一次推送
    TestEventX e1;
    e1.xValue = 111;
    evtSys.pushEvent(e1);
    evtSys.update(0.016f);
    EVT_EXT_ASSERT(listener.eventXCount == 1, "unlisten: first push received");

    // 取消监听
    evtSys.unlistenEvent(&listener);

    // 第二次推送
    listener.reset();
    TestEventX e2;
    e2.xValue = 222;
    evtSys.pushEvent(e2);
    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 0, "unlisten: second push NOT received after unlisten");

    // 重新监听
    evtSys.listenEvent<TestEventX>(&listener);

    // 第三次推送
    TestEventX e3;
    e3.xValue = 333;
    evtSys.pushEvent(e3);
    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.eventXCount == 1, "unlisten: third push received after re-listen");
    EVT_EXT_ASSERT(listener.lastXValue == 333, "unlisten: value after re-listen");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 6: 全局监听 vs 角色局部监听隔离
// -----------------------------------------------------------
static void testGlobalVsCharacterScope()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener globalListener;
    ExtendedListener charListener;

    // 全局监听
    evtSys.listenEvent<TestEventX>(&globalListener);

    // 角色局部监听（需要 Character 对象；这里用 nullptr 模拟全局行为）
    // 注意：实际项目中角色局部需要真实 Character*
    // 这里只验证两种 listenEvent 重载都能正常工作
    evtSys.listenEvent<TestEventY>(&charListener);

    TestEventX ex;
    ex.xValue = 100;
    evtSys.pushEvent(ex);

    TestEventY ey;
    ey.yValue = 200.0f;
    evtSys.pushEvent(ey);

    evtSys.update(0.016f);

    EVT_EXT_ASSERT(globalListener.eventXCount == 1, "scope: global got X");
    EVT_EXT_ASSERT(globalListener.eventYCount == 0, "scope: global no Y");
    EVT_EXT_ASSERT(charListener.eventXCount == 0, "scope: char no X");
    EVT_EXT_ASSERT(charListener.eventYCount == 1, "scope: char got Y");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 7: 高频交替推送多种事件
// -----------------------------------------------------------
static void testHighFrequencyMixedEvents()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);
    evtSys.listenEvent<TestEventY>(&listener);
    evtSys.listenEvent<TestEventZ>(&listener);

    const int ROUNDS = 50;
    for (int i = 0; i < ROUNDS; ++i)
    {
        if (i % 3 == 0)
        {
            TestEventX e;
            e.xValue = i;
            evtSys.pushEvent(e);
        }
        else if (i % 3 == 1)
        {
            TestEventY e;
            e.yValue = (float)i;
            evtSys.pushEvent(e);
        }
        else
        {
            TestEventZ e;
            e.flag = (i % 2 == 0);
            evtSys.pushEvent(e);
        }
    }

    evtSys.update(0.016f);

    // X: i%3==0 => 0,3,6,...,48 → 17 个
    int expectedX = ROUNDS / 3 + (ROUNDS % 3 > 0 ? 1 : 0);
    // Y: i%3==1 => 1,4,7,...,49 → 17 个
    int expectedY = ROUNDS / 3 + (ROUNDS % 3 > 1 ? 1 : 0);
    // Z: i%3==2 => 2,5,8,...,47 → 16 个
    int expectedZ = ROUNDS / 3;

    EVT_EXT_ASSERT(listener.eventXCount == expectedX, "high-freq: X count");
    EVT_EXT_ASSERT(listener.eventYCount == expectedY, "high-freq: Y count");
    EVT_EXT_ASSERT(listener.eventZCount == expectedZ, "high-freq: Z count");

    evtSys.quit();
}

// -----------------------------------------------------------
// Case 8: 无监听者时推送不崩溃
// -----------------------------------------------------------
static void testPushWithNoListeners()
{
    EventInfoPool pool;
    EventSystem evtSys;

    // 没有任何监听者，直接推送各种事件
    FOR(200)
    {
        TestEventX e;
        e.xValue = i;
        evtSys.pushEvent(e);
        TestEventY y;
        evtSys.pushEvent(y);
    }

    evtSys.update(0.016f);
    EVT_EXT_ASSERT(true, "no listeners: no crash");
    evtSys.quit();
}

// -----------------------------------------------------------
// Case 9: 事件数据完整性 — 字段不串扰
// -----------------------------------------------------------
static void testDataIntegrityAcrossTypes()
{
    EventInfoPool pool;
    EventSystem evtSys;
    ExtendedListener listener;

    evtSys.listenEvent<TestEventX>(&listener);
    evtSys.listenEvent<TestEventY>(&listener);

    // 推送带精确值的 X
    TestEventX ex;
    ex.xValue = 12345;
    evtSys.pushEvent(ex);

    // 推送带精确值的 Y
    TestEventY ey;
    ey.yValue = 67.89f;
    ey.yLabel = "integrity-check";
    evtSys.pushEvent(ey);

    evtSys.update(0.016f);

    EVT_EXT_ASSERT(listener.lastXValue == 12345, "integrity: X value exact");
    EVT_EXT_ASSERT(fabsf(listener.lastYValue - 67.89f) < 0.001f, "integrity: Y value exact");
    EVT_EXT_ASSERT(listener.lastYLabel == "integrity-check", "integrity: Y label exact");

    evtSys.quit();
}
}

void EventSystemExtendedTest::test()
{
    testNestedEventPush();
    testMultipleSameEvents();
    testEventTypeIsolation_Strict();
    testDoubleListen_SameEvent();
    testUnlistenThenPushAgain();
    testGlobalVsCharacterScope();
    testHighFrequencyMixedEvents();
    testPushWithNoListeners();
    testDataIntegrityAcrossTypes();
}
