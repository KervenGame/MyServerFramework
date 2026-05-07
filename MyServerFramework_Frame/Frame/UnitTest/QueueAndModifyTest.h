#pragma once
#include "FrameBase.h"
class MICRO_LEGEND_FRAME_API QueueAndModifyTest { public: static void test();
private:
    static void testQueueBasicInt();
    static void testQueuePopEmpty();
    static void testQueuePopOnly();
    static void testQueueClear();
    static void testQueueAddRange();
    static void testQueueFIFOOrder();
    static void testQueueString();
    static void testQueueInterleavedAddPop();
    static void testQueueStress();
    static void testMapModify();
    static void testValueModify();
    static void testQueueWithModifyScenario();
};
