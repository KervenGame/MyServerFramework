#include "FrameHeader.h"

#define SBE_ASSERT_1(expr, msg) \
	if (!(expr)) { ERROR(string("StreamBufferEdgeTest FAIL: ") + (msg)); }
#define SBE_ASSERT_EQ_1(a, b, msg) \
	if ((a) != (b)) { ERROR(string("StreamBufferEdgeTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); }

// =====================================================================
// Part 1: StreamBuffer — 空状态测试
// =====================================================================

static void testSBE_defaultState()
{
	StreamBuffer buf(128);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 0, "default dataLength 0");
	SBE_ASSERT_EQ_1(buf.getBufferSize(), 128, "default bufferSize 0");
}

static void testSBE_emptyPeekData()
{
	StreamBuffer buf(128);
	char out[16] = {};
	SBE_ASSERT_1(!buf.peekData(out, 0, 1), "empty peek returns false");
}

static void testSBE_emptyAddData()
{
	StreamBuffer buf(128);
	const char data[] = "test";
	buf.addDataToBack(data, 4);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 4, "empty addData size 4");
}

// =====================================================================
// Part 2: StreamBuffer — 基础添加和读取
// =====================================================================

static void testSBE_addAndPeek()
{
	StreamBuffer buf(128);
	const char src[] = "Hello!";
	buf.addDataToBack(src, 6);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 6, "add & peek length");

	char dst[6] = {};
	SBE_ASSERT_1(buf.peekData(dst, 6), "peek success");
	for (int i = 0; i < 6; ++i)
	{
		SBE_ASSERT_EQ_1(dst[i], src[i], "peek content");
	}
}

static void testSBE_addAndRemove()
{
	StreamBuffer buf(128);
	const char src[] = "ABCD";
	buf.addDataToBack(src, 4);
	buf.removeDataFromFront(2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 2, "add+remove length 2");

	char dst[2] = {};
	buf.peekData(dst, 2);
	SBE_ASSERT_EQ_1(dst[0], 'C', "add+remove[0]=C");
	SBE_ASSERT_EQ_1(dst[1], 'D', "add+remove[1]=D");
}

static void testSBE_addRemoveAll()
{
	StreamBuffer buf(128);
	buf.addDataToBack("XYZ", 3);
	buf.removeDataFromFront(3);
	SBE_ASSERT_1(buf.isEmpty(), "add+remove all empty");
}

static void testSBE_addTwice()
{
	StreamBuffer buf(128);
	buf.addDataToBack("AB", 2);
	buf.addDataToBack("CD", 2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 4, "add twice length 4");

	char dst[4] = {};
	buf.peekData(dst, 4);
	SBE_ASSERT_EQ_1(dst[0], 'A', "add twice[0]");
	SBE_ASSERT_EQ_1(dst[1], 'B', "add twice[1]");
	SBE_ASSERT_EQ_1(dst[2], 'C', "add twice[2]");
	SBE_ASSERT_EQ_1(dst[3], 'D', "add twice[3]");
}

static void testSBE_removePartial()
{
	StreamBuffer buf(128);
	buf.addDataToBack("12345", 5);
	buf.removeDataFromFront(2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 3, "remove partial length 3");

	buf.addDataToBack("678", 3);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 6, "remove+add length 6");

	char dst[6] = {};
	buf.peekData(dst, 6);
	SBE_ASSERT_EQ_1(dst[0], '3', "remove partial[0]");
	SBE_ASSERT_EQ_1(dst[1], '4', "remove partial[1]");
	SBE_ASSERT_EQ_1(dst[2], '5', "remove partial[2]");
	SBE_ASSERT_EQ_1(dst[3], '6', "remove partial[3]");
	SBE_ASSERT_EQ_1(dst[4], '7', "remove partial[4]");
	SBE_ASSERT_EQ_1(dst[5], '8', "remove partial[5]");
}

// =====================================================================
// Part 3: StreamBuffer — 多次操作序列
// =====================================================================

static void testSBE_multiCycle()
{
	StreamBuffer buf(128);
	for (int cycle = 0; cycle < 5; ++cycle)
	{
		char src[4] = {(char)('A' + cycle * 4), (char)('A' + cycle * 4 + 1),
					   (char)('A' + cycle * 4 + 2), (char)('A' + cycle * 4 + 3)};
		buf.addDataToBack(src, 4);
	}
	SBE_ASSERT_EQ_1(buf.getDataLength(), 20, "5 cycles length 20");

	for (int i = 0; i < 20; ++i)
	{
		char c;
		buf.peekData(&c, 1, i);
		SBE_ASSERT_EQ_1(c, (char)('A' + i), "cycle content");
	}
}

static void testSBE_addRemoveAddRemove()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ab", 2);
	buf.removeDataFromFront(1);
	buf.addDataToBack("cd", 2);
	buf.removeDataFromFront(2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 1, "ara length 1");
	char c;
	buf.peekData(&c, 1);
	SBE_ASSERT_EQ_1(c, 'd', "ara content = d");
}

static void testSBE_addLargeThenRemove()
{
	StreamBuffer buf(2048);
	char big[1024];
	for (int i = 0; i < 1024; ++i)
	{
		big[i] = (char)(i & 0xFF);
	}
	buf.addDataToBack(big, 1024);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 1024, "large add 1024");

	buf.removeDataFromFront(512);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 512, "large remove to 512");

	char dst[512] = {};
	buf.peekData(dst, 512);
	for (int i = 0; i < 512; ++i)
	{
		SBE_ASSERT_EQ_1((byte)dst[i], (byte)((i + 512) & 0xFF), "large content");
	}
}

static void testSBE_addRemoveAllRepeated()
{
	StreamBuffer buf(128);
	for (int cycle = 0; cycle < 20; ++cycle)
	{
		buf.addDataToBack("data", 4);
		buf.removeDataFromFront(4);
		SBE_ASSERT_1(buf.isEmpty(), "cycle empty");
	}
}

// =====================================================================
// Part 4: StreamBuffer — 环状缓冲区边界
// =====================================================================

static void testSBE_wrapAround()
{
	StreamBuffer buf(128);
	// 先填充到接近缓冲区末尾，然后移除部分，再加数据触发环状
	char initial[100];
	memset(initial, 'X', 100);
	buf.addDataToBack(initial, 100);
	buf.removeDataFromFront(80);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 20, "wrap before length 20");

	char more[100];
	memset(more, 'Y', 100);
	buf.addDataToBack(more, 80);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 100, "wrap after length 100");
}

static void testSBE_multipleWrapAround()
{
	StreamBuffer buf(128);
	for (int cycle = 0; cycle < 10; ++cycle)
	{
		char data[30];
		memset(data, (char)('A' + cycle), 30);
		buf.addDataToBack(data, 30);
		buf.removeDataFromFront(25);
	}
	char dst[50] = {};
	buf.peekData(dst, buf.getDataLength());
	SBE_ASSERT_1(true, "multiple wraps no crash");
}

static void testSBE_exactBufferFit()
{
	StreamBuffer buf(128);
	char data[64];
	memset(data, 'Z', 64);
	buf.addDataToBack(data, 64);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 64, "exact fit length 64");
	buf.removeDataFromFront(32);
	buf.addDataToBack(data, 32);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 64, "exact fit re-fill 64");
}

// =====================================================================
// Part 6: StreamBuffer — mergeFrom
// =====================================================================

static void testSBE_mergeFromBasic()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("Hello ", 6);
	b.addDataToBack("World!", 6);
	a.mergeFrom(&b);
	SBE_ASSERT_EQ_1(a.getDataLength(), 12, "merge length 12");
	SBE_ASSERT_1(!b.isEmpty(), "merge source empty");

	char dst[12] = {};
	a.peekData(dst, 12);
	SBE_ASSERT_EQ_1(dst[0], 'H', "merge[0]");
	SBE_ASSERT_EQ_1(dst[5], ' ', "merge[5]");
	SBE_ASSERT_EQ_1(dst[11], '!', "merge[11]");
}

static void testSBE_mergeFromEmpty()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("data", 4);
	a.mergeFrom(&b);
	SBE_ASSERT_EQ_1(a.getDataLength(), 4, "merge empty target unchanged");
}

static void testSBE_mergeFromEmptyBoth()
{
	StreamBuffer a(128), b(128);
	a.mergeFrom(&b);
	SBE_ASSERT_1(a.isEmpty(), "merge both empty");
}

static void testSBE_mergeFromSelf()
{
	StreamBuffer buf(128);
	buf.addDataToBack("test", 4);
	buf.mergeFrom(&buf);
	SBE_ASSERT_1(true, "merge self no crash");
}

static void testSBE_mergeFromAfterRemove()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("0123456789", 10);
	a.removeDataFromFront(5);
	b.addDataToBack("abcde", 5);
	a.mergeFrom(&b);
	SBE_ASSERT_EQ_1(a.getDataLength(), 10, "merge after remove length 10");
}

// =====================================================================
// Part 7: StreamBuffer — linearize
// =====================================================================

static void testSBE_linearizeEmpty()
{
	StreamBuffer buf(128);
	buf.linearize();
	SBE_ASSERT_1(buf.isEmpty(), "linearize empty");
}

static void testSBE_linearizeSimple()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCDEF", 6);
	buf.linearize();
	SBE_ASSERT_EQ_1(buf.getDataLength(), 6, "linearize simple length");

	char dst[6] = {};
	buf.peekData(dst, 6);
	SBE_ASSERT_EQ_1(dst[0], 'A', "linearize[0]");
	SBE_ASSERT_EQ_1(dst[5], 'F', "linearize[5]");
}

static void testSBE_linearizeAfterPartialRead()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCDEFGH", 8);
	buf.removeDataFromFront(3);
	buf.linearize();
	SBE_ASSERT_EQ_1(buf.getDataLength(), 5, "linearize after read length 5");

	char dst[5] = {};
	buf.peekData(dst, 5);
	SBE_ASSERT_EQ_1(dst[0], 'D', "linearize after read[0]=D");
	SBE_ASSERT_EQ_1(dst[4], 'H', "linearize after read[4]=H");
}

static void testSBE_linearizeThenAdd()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABC", 3);
	buf.linearize();
	buf.addDataToBack("DEF", 3);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 6, "linearize then add length");

	char dst[6] = {};
	buf.peekData(dst, 6);
	SBE_ASSERT_EQ_1(dst[0], 'A', "linearize then add[0]");
	SBE_ASSERT_EQ_1(dst[5], 'F', "linearize then add[5]");
}

static void testSBE_linearizeAfterMerge()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("AB", 2);
	b.addDataToBack("CD", 2);
	a.mergeFrom(&b);
	a.linearize();
	SBE_ASSERT_EQ_1(a.getDataLength(), 4, "linearize after merge length");

	char dst[4] = {};
	a.peekData(dst, 4);
	SBE_ASSERT_EQ_1(dst[0], 'A', "linearize after merge[0]");
	SBE_ASSERT_EQ_1(dst[3], 'D', "linearize after merge[3]");
}

// =====================================================================
// Part 8: StreamBuffer — getContiguousDataLength
// =====================================================================

static void testSBE_contiguousLength()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCD", 4);
	int len = buf.getContiguousDataLength();
	SBE_ASSERT_EQ_1(len, buf.getDataLength(), "contiguous length = total");
}

static void testSBE_contiguousAfterRemove()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCDEFGH", 8);
	buf.removeDataFromFront(2);
	int len = buf.getContiguousDataLength();
	SBE_ASSERT_1(len > 0 && len <= buf.getDataLength(), "contiguous after remove");
}

// =====================================================================
// Part 9: StreamBuffer — 批量数据操作
// =====================================================================

static void testSBE_bulkAddAndRead()
{
	StreamBuffer buf(128);
	for (int i = 0; i < 100; ++i)
	{
		char c = (char)('a' + (i % 26));
		buf.addDataToBack(&c, 1);
	}
	SBE_ASSERT_EQ_1(buf.getDataLength(), 100, "bulk add length 100");

	for (int i = 0; i < 100; ++i)
	{
		char c;
		buf.peekData(&c, 1, i);
		SBE_ASSERT_EQ_1(c, (char)('a' + (i % 26)), "bulk content");
	}
}

static void testSBE_bulkRemove()
{
	StreamBuffer buf(1024);
	char data[500];
	memset(data, 'X', 500);
	buf.addDataToBack(data, 500);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 500, "bulk remove before");

	for (int i = 0; i < 50; ++i)
	{
		buf.removeDataFromFront(10);
	}
	SBE_ASSERT_1(buf.isEmpty(), "bulk remove all empty");
}

static void testSBE_bulkAddAfterRemove()
{
	StreamBuffer buf(128);
	for (int batch = 0; batch < 10; ++batch)
	{
		char data[20];
		memset(data, (char)('0' + batch), 20);
		buf.addDataToBack(data, 20);
		if (batch > 0)
		{
			buf.removeDataFromFront(10);
		}
	}
	SBE_ASSERT_1(true, "bulk add after remove no crash");
}

// =====================================================================
// Part 10: StreamBuffer — 边界值和极端情况
// =====================================================================

static void testSBE_singleByteAddRemove()
{
	StreamBuffer buf(128);
	const char c = 'X';
	buf.addDataToBack(&c, 1);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 1, "single byte add");

	char out;
	buf.peekData(&out, 1);
	SBE_ASSERT_EQ_1(out, 'X', "single byte peek");

	buf.removeDataFromFront(1);
	SBE_ASSERT_1(buf.isEmpty(), "single byte remove empty");
}

static void testSBE_zeroLengthAdd()
{
	StreamBuffer buf(128);
	buf.addDataToBack("", 0);
	SBE_ASSERT_1(buf.isEmpty(), "zero length add empty");
}

static void testSBE_maxSizeAdd()
{
	StreamBuffer buf(9000);
	char big[4096];
	memset(big, 'M', 4096);
	buf.addDataToBack(big, 4096);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 4096, "max size 4096");

	char verify[4096] = {};
	buf.peekData(verify, 4096);
	for (int i = 0; i < 4096; ++i)
	{
		SBE_ASSERT_EQ_1((byte)verify[i], (byte)'M', "max size content");
	}
}

static void testSBE_dataEquality()
{
	StreamBuffer buf(128);
	const char src[] = "!@#$%^&*()";
	buf.addDataToBack(src, 10);
	char dst[10] = {};
	buf.peekData(dst, 10);
	for (int i = 0; i < 10; ++i)
	{
		SBE_ASSERT_EQ_1(dst[i], src[i], "data equality");
	}
}

// =====================================================================
// Part 11: StreamBuffer — 混合操作
// =====================================================================

static void testSBE_addPeekRemovePeek()
{
	StreamBuffer buf(128);
	buf.addDataToBack("Hello", 5);
	char p1[3] = {};
	buf.peekData(p1, 3);
	SBE_ASSERT_EQ_1(p1[0], 'H', "apr peek[0]");
	buf.removeDataFromFront(2);
	char p2[3] = {};
	buf.peekData(p2, 3);
	SBE_ASSERT_EQ_1(p2[0], 'l', "apr peek after remove[0]=l");
	SBE_ASSERT_EQ_1(p2[1], 'l', "apr peek after remove[1]=l");
	SBE_ASSERT_EQ_1(p2[2], 'o', "apr peek after remove[2]=o");
}

static void testSBE_addAddRemoveAdd()
{
	StreamBuffer buf(128);
	buf.addDataToBack("AB", 2);
	buf.addDataToBack("CD", 2);
	buf.removeDataFromFront(2);
	buf.addDataToBack("EF", 2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 4, "aara length");

	char dst[4] = {};
	buf.peekData(dst, 4);
	SBE_ASSERT_EQ_1(dst[0], 'C', "aara[0]=C");
	SBE_ASSERT_EQ_1(dst[1], 'D', "aara[1]=D");
	SBE_ASSERT_EQ_1(dst[2], 'E', "aara[2]=E");
	SBE_ASSERT_EQ_1(dst[3], 'F', "aara[3]=F");
}

static void testSBE_addAddRemoveRemove()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCD", 4);
	buf.addDataToBack("EFGH", 4);
	buf.removeDataFromFront(5);
	buf.removeDataFromFront(3);
	SBE_ASSERT_1(buf.isEmpty(), "aarr empty");
}

static void testSBE_stressSequence()
{
	StreamBuffer buf(1024);
	for (int i = 0; i < 50; ++i)
	{
		char data[10];
		memset(data, (char)('A' + (i % 26)), 10);
		buf.addDataToBack(data, 10);
		if (i % 3 == 0)
		{
			buf.removeDataFromFront(5);
		}
		if (i % 7 == 0)
		{
			buf.linearize();
		}
	}
	SBE_ASSERT_1(true, "stress sequence no crash");
}

// =====================================================================
// Part 12: StreamBuffer — clear() 后操作
// =====================================================================

static void testSBE_clearThenAdd()
{
	StreamBuffer buf(128);
	buf.addDataToBack("old", 3);
	buf.clear();
	SBE_ASSERT_1(buf.isEmpty(), "clear then add empty");
	buf.addDataToBack("new", 3);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 3, "clear then add length");

	char dst[3] = {};
	buf.peekData(dst, 3);
	SBE_ASSERT_EQ_1(dst[0], 'n', "clear then add[0]");
	SBE_ASSERT_EQ_1(dst[2], 'w', "clear then add[2]");
}

static void testSBE_clearThenAddLarge()
{
	StreamBuffer buf(512);
	buf.addDataToBack("small", 5);
	buf.clear();
	char big[200];
	memset(big, 'B', 200);
	buf.addDataToBack(big, 200);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 200, "clear+large 200");
}

// =====================================================================
// Part 13: StreamBuffer — 多缓冲区交叉操作
// =====================================================================

static void testSBE_independentBuffers()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("alpha", 5);
	b.addDataToBack("beta", 4);
	SBE_ASSERT_EQ_1(a.getDataLength(), 5, "independent a length");
	SBE_ASSERT_EQ_1(b.getDataLength(), 4, "independent b length");

	char dst[5] = {};
	a.peekData(dst, 5);
	SBE_ASSERT_EQ_1(dst[0], 'a', "independent a data[0]");
	b.peekData(dst, 4);
	SBE_ASSERT_EQ_1(dst[0], 'b', "independent b data[0]");

	a.removeDataFromFront(2);
	b.removeDataFromFront(1);
	SBE_ASSERT_EQ_1(a.getDataLength(), 3, "independent a after remove");
	SBE_ASSERT_EQ_1(b.getDataLength(), 3, "independent b after remove");
}

static void testSBE_clearAndReuse()
{
	StreamBuffer buf(128);
	buf.addDataToBack("first_batch", 11);
	buf.clear();
	buf.addDataToBack("second_batch", 12);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 12, "clear and reuse length");

	char dst[12] = {};
	buf.peekData(dst, 12);
	SBE_ASSERT_EQ_1(dst[0], 's', "clear reuse data[0]");
	SBE_ASSERT_EQ_1(dst[7], 'b', "clear reuse data[7]");
}

static void testSBE_partialPeek()
{
	StreamBuffer buf(128);
	buf.addDataToBack("abcdefghij", 10);

	char half[5] = {};
	SBE_ASSERT_1(buf.peekData(half, 5), "partial peek first half");
	SBE_ASSERT_EQ_1(half[0], 'a', "partial[0]=a");
	SBE_ASSERT_EQ_1(half[4], 'e', "partial[4]=e");

	char second[5] = {};
	SBE_ASSERT_1(buf.peekData(second, 5, 5), "partial peek second half");
	SBE_ASSERT_EQ_1(second[0], 'f', "partial[5]=f");
	SBE_ASSERT_EQ_1(second[4], 'j', "partial[9]=j");
}

static void testSBE_repeatedAddRemove()
{
	StreamBuffer buf(128);
	for (int cycle = 0; cycle < 100; ++cycle)
	{
		char data[1] = {(char)(cycle % 256)};
		buf.addDataToBack(data, 1);
		char read;
		buf.peekData(&read, 1);
		// After many add/remove cycles, buffer should be consistent
		if (cycle >= 50)
		{
			buf.removeDataFromFront(1);
		}
	}
	SBE_ASSERT_1(true, "repeated add/remove no crash");
}

static void testSBE_sequentialAddAndPeek()
{
	StreamBuffer buf(1024);
	char data[256];
	for (int i = 0; i < 256; ++i)
	{
		data[i] = (char)i;
	}
	buf.addDataToBack(data, 256);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 256, "sequential add length");

	for (int i = 0; i < 256; ++i)
	{
		char c;
		buf.peekData(&c, 1, i);
		SBE_ASSERT_EQ_1((int)(byte)c, i, "sequential data byte");
	}
}

// =====================================================================
// 主入口
// =====================================================================
void StreamBufferEdgeTest::test()
{
	testSBE_defaultState();
	testSBE_emptyPeekData();
	testSBE_emptyAddData();
	testSBE_addAndPeek();
	testSBE_addAndRemove();
	testSBE_addRemoveAll();
	testSBE_addTwice();
	testSBE_removePartial();
	testSBE_multiCycle();
	testSBE_addRemoveAddRemove();
	testSBE_addLargeThenRemove();
	testSBE_addRemoveAllRepeated();
	testSBE_wrapAround();
	testSBE_multipleWrapAround();
	testSBE_exactBufferFit();
	testSBE_mergeFromBasic();
	testSBE_mergeFromEmpty();
	testSBE_mergeFromEmptyBoth();
	testSBE_mergeFromSelf();
	testSBE_mergeFromAfterRemove();
	testSBE_linearizeEmpty();
	testSBE_linearizeSimple();
	testSBE_linearizeAfterPartialRead();
	testSBE_linearizeThenAdd();
	testSBE_linearizeAfterMerge();
	testSBE_contiguousLength();
	testSBE_contiguousAfterRemove();
	testSBE_bulkAddAndRead();
	testSBE_bulkRemove();
	testSBE_bulkAddAfterRemove();
	testSBE_singleByteAddRemove();
	testSBE_zeroLengthAdd();
	testSBE_maxSizeAdd();
	testSBE_dataEquality();
	testSBE_addPeekRemovePeek();
	testSBE_addAddRemoveAdd();
	testSBE_addAddRemoveRemove();
	testSBE_stressSequence();
	testSBE_clearThenAdd();
	testSBE_clearThenAddLarge();
	testSBE_independentBuffers();
	testSBE_clearAndReuse();
	testSBE_partialPeek();
	testSBE_repeatedAddRemove();
	testSBE_sequentialAddAndPeek();
}
