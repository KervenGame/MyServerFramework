#include "FrameHeader.h"

namespace
{
	static void assertStreamBuffer(bool condition, const char* message)
	{
		if (!condition)
		{
			ERROR(string("StreamBufferTest failed: ") + message);
		}
	}

	static void testBasicAddRemove()
	{
		StreamBuffer buffer(32);
		assertStreamBuffer(buffer.getDataLength() == 0, "initial length");
		assertStreamBuffer(buffer.getDataStart() == 0, "initial start");
		assertStreamBuffer(buffer.getBufferSize() == 32, "initial buffer size");
		assertStreamBuffer(buffer.isAvailable(8), "available initially");

		const char data[] = "abcd";
		assertStreamBuffer(buffer.addDataToBack(data, 4), "add data");
		assertStreamBuffer(buffer.getDataLength() == 4, "length after add");
		assertStreamBuffer(buffer.getData()[0] == 'a', "data a");
		assertStreamBuffer(buffer.getData()[3] == 'd', "data d");

		buffer.removeDataFromFront(2);
		assertStreamBuffer(buffer.getDataLength() == 2, "length after remove");
		assertStreamBuffer(buffer.getData()[0] == 'c', "data c");
		assertStreamBuffer(buffer.getDataStart() == 2, "start after remove");
	}

	static void testEmptyAndClear()
	{
		StreamBuffer buffer(16);
		assertStreamBuffer(buffer.addEmptyToBack(6), "add empty");
		assertStreamBuffer(buffer.getDataLength() == 6, "empty length");
		buffer.clear();
		assertStreamBuffer(buffer.getDataLength() == 0, "clear length");
		assertStreamBuffer(buffer.getDataStart() == 0, "clear start");
	}

	static void testResizePath()
	{
		StreamBuffer buffer(8);
		const char first[] = "abcd";
		const char second[] = "efgh";
		assertStreamBuffer(buffer.addDataToBack(first, 4), "add first");
		buffer.removeDataFromFront(2);
		// ring buffer: no compaction needed, data starts from mHead=2
		assertStreamBuffer(buffer.addDataToBack(second, 4), "ring add");
		assertStreamBuffer(buffer.getDataLength() == 6, "ring length");
		assertStreamBuffer(buffer.getData()[0] == 'c', "ring c");
		// wraparound check: use peekData to verify all data
		char peekBuf[6] = {};
		buffer.peekData(peekBuf, 6);
		assertStreamBuffer(peekBuf[4] == 'g', "ring g via peek");
		assertStreamBuffer(peekBuf[5] == 'h', "ring h via peek");
	}

	static void testFullClearFallback()
	{
		StreamBuffer buffer(4);
		const char data[] = "abcd";
		assertStreamBuffer(buffer.addDataToBack(data, 4), "fill buffer");
		assertStreamBuffer(buffer.addDataToBack(data, 4, true), "clear if full");
		assertStreamBuffer(buffer.getDataLength() == 4, "clear if full length");
		assertStreamBuffer(buffer.getData()[0] == 'a', "clear if full data");
	}
}

void StreamBufferTest::test()
{
	testBasicAddRemove();
	testEmptyAndClear();
	testResizePath();
	testFullClearFallback();
}
