#include "FrameHeader.h"

// ============================================================
// QueueAndModifyTest
// 覆盖：
//   Queue<T>: add / addRange / pop / popOnly / front / size / isEmpty / clear
//   MapModify<K,V>: 构造函数（erase版/insert版）+ 成员访问
//   ValueModify<T>: 构造函数 + 成员访问
// ============================================================

#define QMT_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("QueueAndModifyTest FAILED: ") + (msg)); } } while(0)
#define QMT_ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("QueueAndModifyTest FAILED: ") + (msg) \
        + " expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

// ============================================================
// Queue<int> 基础操作
// ============================================================
void QueueAndModifyTest::testQueueBasicInt()
{
    Queue<int> q;

    // 初始状态
    QMT_ASSERT(q.isEmpty(), "queue initial isEmpty");
    QMT_ASSERT_EQ(q.size(), 0, "queue initial size==0");

    // add
    q.add(10);
    QMT_ASSERT(!q.isEmpty(), "queue after add not empty");
    QMT_ASSERT_EQ(q.size(), 1, "queue size==1 after add");

    q.add(20);
    q.add(30);
    QMT_ASSERT_EQ(q.size(), 3, "queue size==3");

    // front
    QMT_ASSERT_EQ(q.front(), 10, "queue front==10");

    // pop
    int v1 = q.pop(0);
    QMT_ASSERT_EQ(v1, 10, "queue pop first==10");
    QMT_ASSERT_EQ(q.size(), 2, "queue size==2 after pop");

    int v2 = q.pop(0);
    QMT_ASSERT_EQ(v2, 20, "queue pop second==20");

    int v3 = q.pop(0);
    QMT_ASSERT_EQ(v3, 30, "queue pop third==30");

    QMT_ASSERT(q.isEmpty(), "queue empty after all pops");
    QMT_ASSERT_EQ(q.size(), 0, "queue size==0 after all pops");

}

// ============================================================
// Queue pop on empty (returns defaultValue)
// ============================================================
void QueueAndModifyTest::testQueuePopEmpty()
{
    Queue<int> q;

    // pop 空队列返回默认值
    int v = q.pop(-99);
    QMT_ASSERT_EQ(v, -99, "queue pop empty returns defaultValue");
    QMT_ASSERT(q.isEmpty(), "queue still empty after pop-empty");

    // 多次 pop 空队列
    int v2 = q.pop(0);
    int v3 = q.pop(0);
    QMT_ASSERT_EQ(v2, 0, "queue pop empty 2");
    QMT_ASSERT_EQ(v3, 0, "queue pop empty 3");

}

// ============================================================
// Queue popOnly
// ============================================================
void QueueAndModifyTest::testQueuePopOnly()
{
    Queue<int> q;
    q.add(1);
    q.add(2);
    q.add(3);

    // popOnly 只弹出不返回
    q.popOnly();
    QMT_ASSERT_EQ(q.size(), 2, "queue size==2 after popOnly");
    QMT_ASSERT_EQ(q.front(), 2, "queue front==2 after popOnly");

    q.popOnly();
    QMT_ASSERT_EQ(q.front(), 3, "queue front==3 after second popOnly");

    q.popOnly();
    QMT_ASSERT(q.isEmpty(), "queue empty after 3 popOnly");

}

// ============================================================
// Queue clear
// ============================================================
void QueueAndModifyTest::testQueueClear()
{
    Queue<int> q;
    for (int i = 0; i < 10; ++i)
    {
        q.add(i * 100);
    }
    QMT_ASSERT_EQ(q.size(), 10, "queue size==10 before clear");

    q.clear();
    QMT_ASSERT(q.isEmpty(), "queue isEmpty after clear");
    QMT_ASSERT_EQ(q.size(), 0, "queue size==0 after clear");

    // 清空后可继续使用
    q.add(42);
    QMT_ASSERT_EQ(q.size(), 1, "queue add after clear size==1");
    QMT_ASSERT_EQ(q.pop(0), 42, "queue pop after clear==42");

}

// ============================================================
// Queue addRange
// ============================================================
void QueueAndModifyTest::testQueueAddRange()
{
    Queue<int*> q;

    // 构造测试数据（逐个 add 验证 FIFO）
    int a = 10, b = 20, c = 30;
    q.add(&a);
    q.add(&b);
    q.add(&c);

    QMT_ASSERT_EQ(q.size(), 3, "queue add x3 size==3");

    int* p1 = q.pop(nullptr);
    QMT_ASSERT(p1 == &a, "queue add pop p1==&a");
    int* p2 = q.pop(nullptr);
    QMT_ASSERT(p2 == &b, "queue add pop p2==&b");
    int* p3 = q.pop(nullptr);
    QMT_ASSERT(p3 == &c, "queue add pop p3==&c");

    QMT_ASSERT(q.isEmpty(), "queue empty after pops");

    // 空队列不影响
    QMT_ASSERT(q.isEmpty(), "queue still empty");

}

// ============================================================
// Queue FIFO 顺序验证
// ============================================================
void QueueAndModifyTest::testQueueFIFOOrder()
{
    Queue<int> q;
    const int N = 100;
    for (int i = 0; i < N; ++i)
    {
        q.add(i);
    }
    QMT_ASSERT_EQ(q.size(), N, "queue FIFO size==N");

    // 按照先进先出顺序弹出
    for (int i = 0; i < N; ++i)
    {
        const int v = q.pop(0);
        QMT_ASSERT_EQ(v, i, "queue FIFO pop order");
    }

    QMT_ASSERT(q.isEmpty(), "queue empty after FIFO drain");

}

// ============================================================
// Queue<string> 字符串类型
// ============================================================
void QueueAndModifyTest::testQueueString()
{
    Queue<string> q;

    q.add("alpha");
    q.add("beta");
    q.add("gamma");
    QMT_ASSERT_EQ(q.size(), 3, "queue string size==3");
    QMT_ASSERT(q.front() == "alpha", "queue string front==alpha");

    const string v1 = q.pop("");
    QMT_ASSERT(v1 == "alpha", "queue string pop 1==alpha");

    const string v2 = q.pop("");
    QMT_ASSERT(v2 == "beta", "queue string pop 2==beta");

    const string v3 = q.pop("");
    QMT_ASSERT(v3 == "gamma", "queue string pop 3==gamma");

    QMT_ASSERT(q.isEmpty(), "queue string empty");

}

// ============================================================
// Queue 混合 add / pop 交替
// ============================================================
void QueueAndModifyTest::testQueueInterleavedAddPop()
{
    Queue<int> q;

    // add 3, pop 2, add 3, pop 4
    q.add(1);
    q.add(2);
    q.add(3);
    QMT_ASSERT_EQ(q.size(), 3, "interleaved: size==3");

    QMT_ASSERT_EQ(q.pop(0), 1, "interleaved: pop 1");
    QMT_ASSERT_EQ(q.pop(0), 2, "interleaved: pop 2");
    QMT_ASSERT_EQ(q.size(), 1, "interleaved: size==1");

    q.add(4);
    q.add(5);
    q.add(6);
    QMT_ASSERT_EQ(q.size(), 4, "interleaved: size==4 after add 3 more");

    QMT_ASSERT_EQ(q.pop(0), 3, "interleaved: pop 3");
    QMT_ASSERT_EQ(q.pop(0), 4, "interleaved: pop 4");
    QMT_ASSERT_EQ(q.pop(0), 5, "interleaved: pop 5");
    QMT_ASSERT_EQ(q.pop(0), 6, "interleaved: pop 6");
    QMT_ASSERT(q.isEmpty(), "interleaved: empty at end");

}

// ============================================================
// Queue 压力测试
// ============================================================
void QueueAndModifyTest::testQueueStress()
{
    Queue<int> q;
    const int N = 1000;

    // 1000 个元素入队
    for (int i = 0; i < N; ++i)
    {
        q.add(i * 2);
    }
    QMT_ASSERT_EQ(q.size(), N, "stress: size==1000");

    // 验证前 10 个
    for (int i = 0; i < 10; ++i)
    {
        const int v = q.pop(0);
        QMT_ASSERT_EQ(v, i * 2, "stress: FIFO correct");
    }

    // 清空剩余
    q.clear();
    QMT_ASSERT(q.isEmpty(), "stress: clear successful");

}

// ============================================================
// MapModify<K,V> 测试
// ============================================================
void QueueAndModifyTest::testMapModify()
{
    // erase 构造（只有 key，mAdd=false）
    {
        MapModify<int, int> m(42);
        QMT_ASSERT_EQ(m.mKey, 42, "MapModify erase ctor mKey==42");
        QMT_ASSERT(!m.mAdd, "MapModify erase ctor mAdd==false");
    }

    // insert 构造（key + value，mAdd=true）
    {
        MapModify<int, int> m(10, 100);
        QMT_ASSERT_EQ(m.mKey, 10, "MapModify insert ctor mKey==10");
        QMT_ASSERT_EQ(m.mValue, 100, "MapModify insert ctor mValue==100");
        QMT_ASSERT(m.mAdd, "MapModify insert ctor mAdd==true");
    }

    // string key
    {
        MapModify<string, int> m(string("name"), 999);
        QMT_ASSERT(m.mKey == "name", "MapModify string key");
        QMT_ASSERT_EQ(m.mValue, 999, "MapModify string value");
        QMT_ASSERT(m.mAdd, "MapModify string mAdd==true");
    }

    // erase 构造 string key
    {
        MapModify<string, float> m(string("erase_key"));
        QMT_ASSERT(m.mKey == "erase_key", "MapModify string erase key");
        QMT_ASSERT(!m.mAdd, "MapModify string erase mAdd==false");
    }

    // 批量验证，模拟修改列表
    {
        Vector<MapModify<int, string>> modifyList;
        modifyList.emplace(MapModify<int, string>(1, "one"));
        modifyList.emplace(MapModify<int, string>(2, "two"));
        modifyList.emplace(MapModify<int, string>(3));     // erase 3
        modifyList.emplace(MapModify<int, string>(4, "four"));

        QMT_ASSERT_EQ(modifyList.size(), 4, "modifyList size==4");
        QMT_ASSERT(modifyList[0].mAdd, "modifyList[0].mAdd==true");
        QMT_ASSERT(!modifyList[2].mAdd, "modifyList[2].mAdd==false");
        QMT_ASSERT(modifyList[1].mKey == 2, "modifyList[1].mKey==2");
        QMT_ASSERT(modifyList[1].mValue == "two", "modifyList[1].mValue==two");
        QMT_ASSERT(modifyList[2].mKey == 3, "modifyList[2].mKey==3");
    }

}

// ============================================================
// ValueModify<T> 测试
// ============================================================
void QueueAndModifyTest::testValueModify()
{
    // add=true
    {
        ValueModify<int> m(42, true);
        QMT_ASSERT_EQ(m.mValue, 42, "ValueModify value==42");
        QMT_ASSERT(m.mAdd, "ValueModify add==true");
    }

    // add=false（erase）
    {
        ValueModify<int> m(99, false);
        QMT_ASSERT_EQ(m.mValue, 99, "ValueModify value==99");
        QMT_ASSERT(!m.mAdd, "ValueModify add==false");
    }

    // float 类型
    {
        ValueModify<float> m(3.14f, true);
        QMT_ASSERT(fabsf(m.mValue - 3.14f) < 1e-4f, "ValueModify float value");
        QMT_ASSERT(m.mAdd, "ValueModify float add==true");
    }

    // string 类型
    {
        ValueModify<string> m(string("hello"), true);
        QMT_ASSERT(m.mValue == "hello", "ValueModify string value");
        QMT_ASSERT(m.mAdd, "ValueModify string add==true");
    }

    // 批量验证
    {
        Vector<ValueModify<int>> list;
        list.emplace(ValueModify<int>(10, true));
        list.emplace(ValueModify<int>(20, true));
        list.emplace(ValueModify<int>(10, false)); // erase 10
        list.emplace(ValueModify<int>(30, true));

        QMT_ASSERT_EQ(list.size(), 4, "ValueModify batch size==4");
        QMT_ASSERT(list[0].mAdd, "batch[0].mAdd==true");
        QMT_ASSERT(list[1].mAdd, "batch[1].mAdd==true");
        QMT_ASSERT(!list[2].mAdd, "batch[2].mAdd==false");
        QMT_ASSERT_EQ(list[2].mValue, 10, "batch[2].mValue==10");
    }

}

// ============================================================
// 综合场景：Queue + MapModify 模拟 SafeMap 操作
// ============================================================
void QueueAndModifyTest::testQueueWithModifyScenario()
{
    // 模拟：一边遍历（startForeach），一边将修改操作记录到队列
    Queue<MapModify<int, string>*> pendingQueue;

    // 创建一些操作
    MapModify<int, string> op1(1, "Alice");
    MapModify<int, string> op2(2, "Bob");
    MapModify<int, string> op3(3);            // erase 3
    MapModify<int, string> op4(4, "Dave");

    pendingQueue.add(&op1);
    pendingQueue.add(&op2);
    pendingQueue.add(&op3);
    pendingQueue.add(&op4);

    QMT_ASSERT_EQ(pendingQueue.size(), 4, "scenario queue size==4");

    // 消费队列，重建 Map
    Map<int, string> target;
    target.add(3, "Charlie"); // 已有数据

    while (!pendingQueue.isEmpty())
    {
        MapModify<int, string>* op = pendingQueue.pop(nullptr);
        if (op->mAdd)
        {
            target.add(op->mKey, op->mValue);
        }
        else
        {
            target.remove(op->mKey);
        }
    }

    // 验证结果：3 被删除，1/2/4 被插入
    QMT_ASSERT(!target.contains(3), "scenario: 3 erased");
    QMT_ASSERT(target.contains(1), "scenario: 1 inserted");
    QMT_ASSERT(target.contains(2), "scenario: 2 inserted");
    QMT_ASSERT(target.contains(4), "scenario: 4 inserted");
    QMT_ASSERT(target.get(1, string("")) == "Alice", "scenario: 1==Alice");
    QMT_ASSERT(target.get(2, string("")) == "Bob", "scenario: 2==Bob");
    QMT_ASSERT(pendingQueue.isEmpty(), "scenario: queue empty after processing");

}

// ============================================================
// 主入口
// ============================================================
void QueueAndModifyTest::test()
{
    testQueueBasicInt();
    testQueuePopEmpty();
    testQueuePopOnly();
    testQueueClear();
    testQueueAddRange();
    testQueueFIFOOrder();
    testQueueString();
    testQueueInterleavedAddPop();
    testQueueStress();
    testMapModify();
    testValueModify();
    testQueueWithModifyScenario();
}
