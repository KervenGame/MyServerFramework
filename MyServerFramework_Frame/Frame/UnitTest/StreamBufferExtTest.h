#pragma once
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API StreamBufferExtTest
{
public:
	static void test();
private:
    static void testStreamBufferConstruct();
    static void testStreamBufferAddBasic();
    static void testStreamBufferAddMultiple();
    static void testStreamBufferRemoveFront();
    static void testStreamBufferRemoveMultiple();
    static void testStreamBufferClear();
    static void testStreamBufferClearAndReuse();
    static void testStreamBufferIsAvailable();
    static void testStreamBufferAddEmpty();
    static void testStreamBufferAddDataFail();
    static void testStreamBufferAddDataClearIfFull();
    static void testStreamBufferProducerConsumer();
    static void testStreamBufferBinaryData();
    static void testStreamBufferFillAndClear();
    static void testStreamBufferRemoveOverflow();
    static void testStreamBufferLargeBuffer();
    static void testStreamBufferTCPSimulation();
    static void testStreamBufferOffsetTracking();
    static void testStreamBufferSingleByte();
    static void testStreamBufferAddZeroBytes();
    static void testStreamBufferRepeatAdd();
    static void testStreamBufferExactFit();
    static void testStreamBufferMultiRound();
    static void testStreamBufferIsAvailableZero();
    static void testStreamBufferReserveThenFill();
    static void testStreamBufferFrameStream();
    static void testStreamBufferVariousSizes();
    static void testStreamBufferAddEmptyClearIfFull();
    static void testStreamBufferMixedAddOps();
    // 环形缓冲区新增测试
    static void testStreamBufferRingWraparound();
    static void testStreamBufferRingMultiWrap();
    static void testStreamBufferPeekDataEdge();
    static void testStreamBufferContiguousLength();
    static void testStreamBufferCommitTail();
	static void testStreamBufferMergeFrom();
	static void testStreamBufferLinearize();
};
