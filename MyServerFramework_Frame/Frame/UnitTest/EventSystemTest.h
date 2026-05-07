#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API EventSystemTest
{
public:
    static void test();
private:
    static void testEventInfoResetProperty();
    static void testPushAndListenEvent();
    static void testUnlistenEvent();
    static void testRemoveCharacterEvent();
    static void testGlobalEventListen();
    static void testEventProcessCallback();
    static void testMultipleListeners();
    static void testEventTypeIsolation();
};
