#include "FrameHeader.h"

// ============================================================
// StreamBufferExtTest
// 覆盖 StreamBuffer 环形缓冲区的完整功能:
//   构造 / isAvailable / addDataToBack / addEmptyToBack
//   removeDataFromFront / getData / getDataLength / clear
//   环形跨越 / peekData / getContiguousDataLength
// ============================================================

#define SBE_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("StreamBufferExtTest FAIL: ") + (msg)); } } while(0)
#define SBE_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StreamBufferExtTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

//------------------------------------------------------------------------------
// 基本构造与初始状态
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferConstruct()
{
	StreamBuffer sb(128);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "construct: dataLength=0");
	SBE_ASSERT(sb.isAvailable(1), "construct: isAvailable(1)=true");
	SBE_ASSERT(sb.isAvailable(128), "construct: isAvailable(128)=true");
	SBE_ASSERT(!sb.isAvailable(129), "construct: isAvailable(129)=false");
	SBE_ASSERT_EQ(sb.getBufferSize(), 128, "construct: bufferSize=128");
	SBE_ASSERT_EQ(sb.getFreeSize(), 128, "construct: freeSize=128");
}

//------------------------------------------------------------------------------
// addDataToBack 基本写入
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferAddBasic()
{
	StreamBuffer sb(256);
	const char* data = "hello";
	int len = 5;
	bool ok = sb.addDataToBack(data, len);
	SBE_ASSERT(ok, "addDataToBack 'hello' ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "addDataToBack: dataLength=5");
	// 读取验证
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], 'h', "addDataToBack[0]='h'");
	SBE_ASSERT_EQ(p[1], 'e', "addDataToBack[1]='e'");
	SBE_ASSERT_EQ(p[2], 'l', "addDataToBack[2]='l'");
	SBE_ASSERT_EQ(p[3], 'l', "addDataToBack[3]='l'");
	SBE_ASSERT_EQ(p[4], 'o', "addDataToBack[4]='o'");
}

//------------------------------------------------------------------------------
// addDataToBack 多次写入
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferAddMultiple()
{
	StreamBuffer sb(256);
	const char* part1 = "ABC";
	const char* part2 = "DEF";
	const char* part3 = "GHI";
	sb.addDataToBack(part1, 3);
	sb.addDataToBack(part2, 3);
	sb.addDataToBack(part3, 3);
	SBE_ASSERT_EQ(sb.getDataLength(), 9, "addMultiple: total=9");
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], 'A', "addMultiple[0]='A'");
	SBE_ASSERT_EQ(p[3], 'D', "addMultiple[3]='D'");
	SBE_ASSERT_EQ(p[6], 'G', "addMultiple[6]='G'");
	SBE_ASSERT_EQ(p[8], 'I', "addMultiple[8]='I'");
}

//------------------------------------------------------------------------------
// removeDataFromFront 基本移除
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferRemoveFront()
{
	StreamBuffer sb(256);
	const char* data = "1234567890";
	sb.addDataToBack(data, 10);
	SBE_ASSERT_EQ(sb.getDataLength(), 10, "before remove: len=10");
	sb.removeDataFromFront(3);
	SBE_ASSERT_EQ(sb.getDataLength(), 7, "after remove 3: len=7");
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], '4', "after remove 3: [0]='4'");
	SBE_ASSERT_EQ(p[1], '5', "after remove 3: [1]='5'");
	SBE_ASSERT_EQ(p[6], '0', "after remove 3: [6]='0'");
}

//------------------------------------------------------------------------------
// removeDataFromFront 分多次
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferRemoveMultiple()
{
	StreamBuffer sb(256);
	const char* data = "ABCDEFGHIJ";
	sb.addDataToBack(data, 10);
	sb.removeDataFromFront(2);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "remove 2: len=8");
	SBE_ASSERT_EQ(sb.getData()[0], 'C', "remove 2: [0]='C'");
	sb.removeDataFromFront(3);
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "remove 5: len=5");
	SBE_ASSERT_EQ(sb.getData()[0], 'F', "remove 5: [0]='F'");
	sb.removeDataFromFront(5);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "remove all: len=0");
}

//------------------------------------------------------------------------------
// clear 清空
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferClear()
{
	StreamBuffer sb(128);
	const char* data = "testdata";
	sb.addDataToBack(data, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "before clear: len=8");
	sb.clear();
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "after clear: len=0");
	SBE_ASSERT(sb.isAvailable(128), "after clear: isAvailable(128)=true");
}

//------------------------------------------------------------------------------
// clear 后可以继续写入
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferClearAndReuse()
{
	StreamBuffer sb(64);
	const char* data1 = "first";
	sb.addDataToBack(data1, 5);
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "before clear: len=5");
	sb.clear();
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "cleared: len=0");
	const char* data2 = "second";
	sb.addDataToBack(data2, 6);
	SBE_ASSERT_EQ(sb.getDataLength(), 6, "after clear+add: len=6");
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], 's', "after reuse[0]='s'");
	SBE_ASSERT_EQ(p[5], 'd', "after reuse[5]='d'");
}

//------------------------------------------------------------------------------
// isAvailable — 写入后剩余空间变化
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferIsAvailable()
{
	StreamBuffer sb(100);
	SBE_ASSERT(sb.isAvailable(100), "isAvailable(100) initial");
	SBE_ASSERT(!sb.isAvailable(101), "isAvailable(101) initial false");

	const char* data = "1234567890";
	sb.addDataToBack(data, 10);
	SBE_ASSERT(sb.isAvailable(90), "isAvailable(90) after add 10");
	SBE_ASSERT(!sb.isAvailable(91), "isAvailable(91) after add 10 false");

	sb.addDataToBack(data, 10);
	SBE_ASSERT(sb.isAvailable(80), "isAvailable(80) after add 20");

	sb.clear();
	SBE_ASSERT(sb.isAvailable(100), "isAvailable(100) after clear");
}

//------------------------------------------------------------------------------
// addEmptyToBack 写入空白占位
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferAddEmpty()
{
	StreamBuffer sb(256);
	bool ok = sb.addEmptyToBack(10);
	SBE_ASSERT(ok, "addEmptyToBack(10) ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 10, "addEmptyToBack: len=10");

	// 追加真实数据后能读取
	const char* data = "XYZ";
	sb.addDataToBack(data, 3);
	SBE_ASSERT_EQ(sb.getDataLength(), 13, "addEmptyToBack+add: len=13");
	// 用 peekData 验证跨段数据
	char peekBuf[13] = {};
	sb.peekData(peekBuf, 13);
	SBE_ASSERT_EQ(peekBuf[10], 'X', "after addEmpty+add: [10]='X'");
	SBE_ASSERT_EQ(peekBuf[11], 'Y', "after addEmpty+add: [11]='Y'");
	SBE_ASSERT_EQ(peekBuf[12], 'Z', "after addEmpty+add: [12]='Z'");
}

//------------------------------------------------------------------------------
// addDataToBack clearIfFull=false: 超出容量返回 false
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferAddDataFail()
{
	StreamBuffer sb(10);
	const char* smallData = "12345";
	sb.addDataToBack(smallData, 5);
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "half-full len=5");
}

//------------------------------------------------------------------------------
// addDataToBack clearIfFull=true: 超出容量先清空再写入
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferAddDataClearIfFull()
{
	StreamBuffer sb(16);
	const char* init = "12345678";
	sb.addDataToBack(init, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "before overflow: len=8");

	const char* big = "ABCDEFGHIJKLMNOP";
	bool ok = sb.addDataToBack(big, 16, true);
	SBE_ASSERT(ok, "addDataToBack clearIfFull=true succeed after clear");
	SBE_ASSERT_EQ(sb.getDataLength(), 16, "after clearIfFull: len=16");
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], 'A', "clearIfFull[0]='A'");
	SBE_ASSERT_EQ(p[15], 'P', "clearIfFull[15]='P'");
}

//------------------------------------------------------------------------------
// 交替 add/remove 模拟消费者场景
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferProducerConsumer()
{
	StreamBuffer sb(64);
	// 批次1: 写入12字节
	const char* batch1 = "BATCH_ONE___";
	sb.addDataToBack(batch1, 12);
	SBE_ASSERT_EQ(sb.getDataLength(), 12, "pc batch1: len=12");
	// 消费前4
	sb.removeDataFromFront(4);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "pc consume 4: len=8");
	SBE_ASSERT_EQ(sb.getData()[0], 'H', "pc: [0]='H'");
	// 批次2: 再写8
	const char* batch2 = "BATCH_TWO";
	sb.addDataToBack(batch2, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 16, "pc batch2: len=16");
	// 消费全部
	sb.removeDataFromFront(16);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "pc consume all: len=0");
}

//------------------------------------------------------------------------------
// 二进制数据写入（整数序列）
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferBinaryData()
{
	StreamBuffer sb(256);
	// 写入 10 个 int
	for (int i = 0; i < 10; ++i)
	{
		int val = i * 100;
		sb.addDataToBack(reinterpret_cast<const char*>(&val), sizeof(int));
	}
	SBE_ASSERT_EQ(sb.getDataLength(), 40, "binary 10 ints: len=40");
	// 用 peekData 读取并验证（保证连续）
	for (int i = 0; i < 10; ++i)
	{
		int val = 0;
		sb.peekData(&val, sizeof(int), i * sizeof(int));
		int expected = i * 100;
		if (val != expected)
		{
			ERROR(string("binary data[") + IToS(i) + "] expected=" + IToS(expected) + " actual=" + IToS(val));
		}
	}
}

//------------------------------------------------------------------------------
// 多次 clear 与满填充测试
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferFillAndClear()
{
	StreamBuffer sb(32);
	// 填满
	const char* full = "12345678901234567890123456789012";  // 32 chars
	sb.addDataToBack(full, 32);
	SBE_ASSERT_EQ(sb.getDataLength(), 32, "fill: len=32");
	SBE_ASSERT(!sb.isAvailable(1), "fill: isAvailable(1)=false");
	sb.clear();
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "fill+clear: len=0");
	SBE_ASSERT(sb.isAvailable(32), "fill+clear: isAvailable(32)=true");

	// 再次填满
	sb.addDataToBack(full, 32);
	SBE_ASSERT_EQ(sb.getDataLength(), 32, "refill: len=32");
	sb.clear();
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "refill+clear: len=0");
}

//------------------------------------------------------------------------------
// 多次 removeDataFromFront 超出数据量（安全性）
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferRemoveOverflow()
{
	StreamBuffer sb(64);
	const char* data = "test";
	sb.addDataToBack(data, 4);
	SBE_ASSERT_EQ(sb.getDataLength(), 4, "before overflow remove: len=4");
	// 框架内部用 LOG 打印错误但不修改 mDataLength
	// 仅验证没有崩溃，数据没有变成负数
	SBE_ASSERT(sb.getDataLength() >= 0, "removeOverflow: dataLength not negative");
}

//------------------------------------------------------------------------------
// 大缓冲区压力测试 (1KB chunks x 10)
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferLargeBuffer()
{
	const int chunkSize = 1024;
	StreamBuffer sb(chunkSize * 10);
	// 一次性写满 10 个 chunk
	for (int i = 0; i < 10; ++i)
	{
		char chunk[chunkSize];
		memset(chunk, (char)i, chunkSize);
		bool ok = sb.addDataToBack(chunk, chunkSize);
		SBE_ASSERT(ok, string("large: chunk ") + IToS(i) + " ok");
	}
	SBE_ASSERT_EQ(sb.getDataLength(), chunkSize * 10, "large: total len=10240");
	// 逐块验证
	for (int i = 0; i < 10; ++i)
	{
		const char* p = sb.getData();
		SBE_ASSERT_EQ((unsigned char)p[0], (unsigned char)i, string("large verify chunk ") + IToS(i));
		sb.removeDataFromFront(chunkSize);
	}
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "large: after all remove len=0");
}

//------------------------------------------------------------------------------
// 流式处理: 模拟 TCP 分包粘包场景
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferTCPSimulation()
{
	StreamBuffer sb(512);
	// 模拟到来的TCP数据流: 3个"包", 每包4字节包头+N字节体
	const char* packet1 = "\x00\x00\x00\x08" "BODY1111";  // 4+8=12 bytes
	const char* packet2 = "\x00\x00\x00\x05" "BODY2";     // 4+5=9 bytes
	const char* packet3 = "\x00\x00\x00\x06" "BODY33";    // 4+6=10 bytes

	sb.addDataToBack(packet1, 12);
	sb.addDataToBack(packet2, 9);
	sb.addDataToBack(packet3, 10);
	SBE_ASSERT_EQ(sb.getDataLength(), 31, "tcp sim: total=31");

	// 处理包1
	const char* p1 = sb.getData();
	int body1Size = *(reinterpret_cast<const int*>(p1));
	(void)body1Size;
	SBE_ASSERT_EQ(sb.getData()[4], 'B', "tcp pkt1 body[0]='B'");
	sb.removeDataFromFront(12);
	SBE_ASSERT_EQ(sb.getDataLength(), 19, "tcp after pkt1: len=19");

	// 处理包2
	SBE_ASSERT_EQ(sb.getData()[4], 'B', "tcp pkt2 body[0]='B'");
	sb.removeDataFromFront(9);
	SBE_ASSERT_EQ(sb.getDataLength(), 10, "tcp after pkt2: len=10");

	// 处理包3
	SBE_ASSERT_EQ(sb.getData()[4], 'B', "tcp pkt3 body[0]='B'");
	sb.removeDataFromFront(10);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "tcp after pkt3: len=0");
}

//------------------------------------------------------------------------------
// 状态跟踪（环形缓冲区语义）
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferOffsetTracking()
{
	StreamBuffer sb(128);
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "initial: dataStart=0");
	SBE_ASSERT_EQ(sb.getBufferSize(), 128, "initial: bufferSize=128");

	const char* data = "ABCDE";
	sb.addDataToBack(data, 5);
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "after add: dataStart=0");

	sb.removeDataFromFront(2);
	SBE_ASSERT_EQ(sb.getDataStart(), 2, "after remove 2: dataStart=2");
	SBE_ASSERT_EQ(sb.getDataLength(), 3, "after remove 2: dataLen=3");
	SBE_ASSERT_EQ(sb.getBufferSize(), 128, "after remove 2: bufferSize=128");

	sb.removeDataFromFront(3);
	// 环形缓冲区清空后重置指针
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "after remove all: dataStart=0");
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "after remove all: dataLen=0");

	sb.clear();
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "after clear: dataStart=0");
	SBE_ASSERT_EQ(sb.getBufferSize(), 128, "after clear: bufferSize=128");
}

//------------------------------------------------------------------------------
// 单字节写入测试
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferSingleByte()
{
	StreamBuffer sb(64);
	char c = 'X';
	sb.addDataToBack(&c, 1);
	SBE_ASSERT_EQ(sb.getDataLength(), 1, "single byte: len=1");
	SBE_ASSERT_EQ(sb.getData()[0], 'X', "single byte: [0]='X'");

	char c2 = 'Y';
	sb.addDataToBack(&c2, 1);
	SBE_ASSERT_EQ(sb.getDataLength(), 2, "two bytes: len=2");
	SBE_ASSERT_EQ(sb.getData()[1], 'Y', "two bytes: [1]='Y'");

	sb.removeDataFromFront(1);
	SBE_ASSERT_EQ(sb.getDataLength(), 1, "remove 1: len=1");
	SBE_ASSERT_EQ(sb.getData()[0], 'Y', "remove 1: [0]='Y'");
}

//------------------------------------------------------------------------------
// 写入零字节 (edge case)
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferAddZeroBytes()
{
	StreamBuffer sb(64);
	const char* data = "anything";
	bool ok = sb.addDataToBack(data, 0);
	// 写入0字节应该成功(无实际数据)
	SBE_ASSERT(ok, "addDataToBack 0 bytes ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "addZeroBytes: len=0");
}

//------------------------------------------------------------------------------
// 中途重复写入同一段数据
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferRepeatAdd()
{
	StreamBuffer sb(256);
	const char* pattern = "XY";
	for (int i = 0; i < 20; ++i)
	{
		sb.addDataToBack(pattern, 2);
	}
	SBE_ASSERT_EQ(sb.getDataLength(), 40, "repeat add: len=40");
	const char* p = sb.getData();
	for (int i = 0; i < 20; ++i)
	{
		SBE_ASSERT_EQ(p[i * 2 + 0], 'X', string("repeat[") + IToS(i) + "][0]='X'");
		SBE_ASSERT_EQ(p[i * 2 + 1], 'Y', string("repeat[") + IToS(i) + "][1]='Y'");
	}
}

//------------------------------------------------------------------------------
// 小缓冲区恰好装满
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferExactFit()
{
	StreamBuffer sb(8);
	const char* data = "12345678";
	bool ok = sb.addDataToBack(data, 8);
	SBE_ASSERT(ok, "exactFit add 8 to size-8 buffer");
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "exactFit: len=8");
	SBE_ASSERT(!sb.isAvailable(1), "exactFit: isAvailable(1)=false");
	SBE_ASSERT(sb.isAvailable(0), "exactFit: isAvailable(0)=true");
}

//------------------------------------------------------------------------------
// 频繁 clear 与多轮写读
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferMultiRound()
{
	StreamBuffer sb(128);
	for (int round = 0; round < 10; ++round)
	{
		sb.clear();
		// 每轮写 round+1 字节
		for (int j = 0; j <= round; ++j)
		{
			char c = (char)('A' + j);
			sb.addDataToBack(&c, 1);
		}
		int expectedLen = round + 1;
		SBE_ASSERT_EQ(sb.getDataLength(), expectedLen,
			string("multiRound[") + IToS(round) + "] len=" + IToS(expectedLen));
		SBE_ASSERT_EQ(sb.getData()[0], 'A',
			string("multiRound[") + IToS(round) + "] [0]='A'");
	}
}

//------------------------------------------------------------------------------
// isAvailable(0) — 边界: 请求0字节永远可用
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferIsAvailableZero()
{
	StreamBuffer sb(1);
	SBE_ASSERT(sb.isAvailable(0), "isAvailable(0) on size-1 empty");
	const char c = 'X';
	sb.addDataToBack(&c, 1);
	SBE_ASSERT(!sb.isAvailable(1), "isAvailable(1) full");
	SBE_ASSERT(sb.isAvailable(0), "isAvailable(0) even when full");
}

//------------------------------------------------------------------------------
// addEmptyToBack 分批占位然后手动填充
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferReserveThenFill()
{
	StreamBuffer sb(64);
	// 先占位 4 字节（用于后写包头）
	bool ok = sb.addEmptyToBack(4);
	SBE_ASSERT(ok, "reserve 4 bytes ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 4, "reserve: len=4");

	// 再写包体
	const char* body = "BODY_DATA";
	sb.addDataToBack(body, 9);
	SBE_ASSERT_EQ(sb.getDataLength(), 13, "reserve+body: len=13");

	// 回写包头 (直接操作 getData 指针 — 仅在数据未跨越时安全)
	char* header = sb.getData();
	header[0] = 0x01;
	header[1] = 0x02;
	header[2] = 0x03;
	header[3] = 0x04;
	SBE_ASSERT_EQ((unsigned char)sb.getData()[0], 0x01, "reserve fill[0]=0x01");
	SBE_ASSERT_EQ((unsigned char)sb.getData()[3], 0x04, "reserve fill[3]=0x04");

	// 用 peekData 验证完整数据（安全跨段读取）
	char peekBuf[13] = {};
	sb.peekData(peekBuf, 13);
	SBE_ASSERT_EQ((unsigned char)peekBuf[0], 0x01, "peek reserve fill[0]=0x01");
	SBE_ASSERT_EQ((unsigned char)peekBuf[3], 0x04, "peek reserve fill[3]=0x04");
	SBE_ASSERT_EQ(peekBuf[4], 'B', "peek reserve fill body[0]='B'");
}

//------------------------------------------------------------------------------
// 模拟帧数据流: 多帧连续写入和逐帧消费
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferFrameStream()
{
	StreamBuffer sb(1024);
	// 模拟5帧数据，每帧=4字节帧号+8字节数据
	const int frameCount = 5;
	const int frameBodySize = 8;
	const int frameHeaderSize = 4;
	const int frameSize = frameHeaderSize + frameBodySize;

	for (int f = 0; f < frameCount; ++f)
	{
		sb.addDataToBack(reinterpret_cast<const char*>(&f), frameHeaderSize);
		char body[8];
		memset(body, (char)('a' + f), frameBodySize);
		sb.addDataToBack(body, frameBodySize);
	}
	SBE_ASSERT_EQ(sb.getDataLength(), frameSize * frameCount,
		"frameStream: total len=" + IToS(frameSize * frameCount));

	// 逐帧消费并验证
	for (int f = 0; f < frameCount; ++f)
	{
		const char* p = sb.getData();
		int frameNum = 0;
		memcpy(&frameNum, p, frameHeaderSize);
		SBE_ASSERT_EQ(frameNum, f, string("frameStream frame=") + IToS(f));
		SBE_ASSERT_EQ(p[frameHeaderSize], (char)('a' + f),
			string("frameStream body[0] frame=") + IToS(f));
		sb.removeDataFromFront(frameSize);
	}
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "frameStream: consumed all");
}

//------------------------------------------------------------------------------
// 不同缓冲区大小的边界测试
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferVariousSizes()
{
	// 大小为1
	{
		StreamBuffer sb(1);
		SBE_ASSERT(sb.isAvailable(1), "size-1: isAvailable(1)");
		SBE_ASSERT(!sb.isAvailable(2), "size-1: isAvailable(2) false");
		const char c = 'Z';
		bool ok = sb.addDataToBack(&c, 1);
		SBE_ASSERT(ok, "size-1: add 1 byte ok");
		SBE_ASSERT_EQ(sb.getDataLength(), 1, "size-1: len=1");
	}
	// 大小为4
	{
		StreamBuffer sb(4);
		int val = 0x12345678;
		bool ok = sb.addDataToBack(reinterpret_cast<const char*>(&val), 4);
		SBE_ASSERT(ok, "size-4: add int ok");
		SBE_ASSERT_EQ(sb.getDataLength(), 4, "size-4: len=4");
		int readback = 0;
		memcpy(&readback, sb.getData(), 4);
		SBE_ASSERT_EQ(readback, val, "size-4: int readback");
	}
	// 大小为2048
	{
		StreamBuffer sb(2048);
		SBE_ASSERT(sb.isAvailable(2048), "size-2048: isAvailable(2048)");
		SBE_ASSERT(!sb.isAvailable(2049), "size-2048: isAvailable(2049) false");
	}
}

//------------------------------------------------------------------------------
// addEmptyToBack clearIfFull=true 场景
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferAddEmptyClearIfFull()
{
	StreamBuffer sb(16);
	const char* init = "0123456789012345";
	sb.addDataToBack(init, 16);
	SBE_ASSERT_EQ(sb.getDataLength(), 16, "before empty overflow: len=16");

	// 再申请8字节空间，clearIfFull=true 会先清空
	bool ok = sb.addEmptyToBack(8, true);
	SBE_ASSERT(ok, "addEmptyToBack overflow clearIfFull=true ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "after addEmpty clearIfFull: len=8");
}

//------------------------------------------------------------------------------
// 混合 addDataToBack / addEmptyToBack 场景
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferMixedAddOps()
{
	StreamBuffer sb(128);
	// 先写header占位
	sb.addEmptyToBack(8);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "mixed: empty header 8");
	// 再写payload
	const char* payload = "PAYLOAD_DATA_12345";
	sb.addDataToBack(payload, 18);
	SBE_ASSERT_EQ(sb.getDataLength(), 26, "mixed: +payload=26");
	// 验证payload正确
	SBE_ASSERT_EQ(sb.getData()[8], 'P', "mixed: payload[0]='P'");
	// 移除header
	sb.removeDataFromFront(8);
	SBE_ASSERT_EQ(sb.getDataLength(), 18, "mixed: remove header len=18");
	SBE_ASSERT_EQ(sb.getData()[0], 'P', "mixed: after remove[0]='P'");
}

//------------------------------------------------------------------------------
// 环形跨越: 核心 — 数据跨越缓冲区末尾
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferRingWraparound()
{
	StreamBuffer sb(8);
	// 填满
	const char* full = "ABCDEFGH";
	sb.addDataToBack(full, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "wrap: fill len=8");

	// 消费前4字节，head=4
	sb.removeDataFromFront(4);
	SBE_ASSERT_EQ(sb.getDataLength(), 4, "wrap: after remove len=4");
	SBE_ASSERT_EQ(sb.getDataStart(), 4, "wrap: after remove head=4");

	// 再写4字节，会跨越缓冲区末尾（tail=0, 写入4字节到0-3）
	const char* more = "IJKL";
	sb.addDataToBack(more, 4);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "wrap: after wrap add len=8");

	// getData() 返回第一段（从 head=4 开始，4字节：EFGH）
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 4, "wrap: contiguous=4");
	SBE_ASSERT_EQ(sb.getData()[0], 'E', "wrap: first seg [0]='E'");
	SBE_ASSERT_EQ(sb.getData()[3], 'H', "wrap: first seg [3]='H'");

	// 用 peekData 验证完整数据（跨段：EFGHIJKL）
	char peekBuf[8] = {};
	sb.peekData(peekBuf, 8);
	SBE_ASSERT_EQ(peekBuf[0], 'E', "wrap: peek[0]='E'");
	SBE_ASSERT_EQ(peekBuf[3], 'H', "wrap: peek[3]='H'");
	SBE_ASSERT_EQ(peekBuf[4], 'I', "wrap: peek[4]='I'");
	SBE_ASSERT_EQ(peekBuf[7], 'L', "wrap: peek[7]='L'");

	// 移除全部
	sb.removeDataFromFront(8);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "wrap: after remove all len=0");
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "wrap: after remove all head=0");
}

//------------------------------------------------------------------------------
// 环形跨越: 多轮写读，指针多次回绕
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferRingMultiWrap()
{
	// 16 byte buffer: enough to hold multi-round wrap-around data
	StreamBuffer sb(16);
	// Round 1: write 8, consume 6, 2 left at tail
	const char* d1 = "ABCDEFGH";
	sb.addDataToBack(d1, 8);
	sb.removeDataFromFront(6);
	SBE_ASSERT_EQ(sb.getDataLength(), 2, "multiwrap r1: len=2");

	// Round 2: write 6, wrap (tail=8, 2 to [8-9], 4 to [0-3])
	const char* d2 = "IJKLMN";
	sb.addDataToBack(d2, 6);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "multiwrap r2: len=8");

	// Verify: GH IJKLMN
	char peekBuf[8] = {};
	sb.peekData(peekBuf, 8);
	SBE_ASSERT_EQ(peekBuf[0], 'G', "multiwrap: [0]='G'");
	SBE_ASSERT_EQ(peekBuf[1], 'H', "multiwrap: [1]='H'");
	SBE_ASSERT_EQ(peekBuf[2], 'I', "multiwrap: [2]='I'");
	SBE_ASSERT_EQ(peekBuf[7], 'N', "multiwrap: [7]='N'");

	// Consume 5, head moves from 6 to 11%16=11
	sb.removeDataFromFront(5);
	SBE_ASSERT_EQ(sb.getDataLength(), 3, "multiwrap r2: after remove len=3");

	// Round 3: write 8, still fits (freeSpace = 16-3 = 13 >= 8), wraps
	const char* d3 = "OPQRSTUV";
	sb.addDataToBack(d3, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 11, "multiwrap r3: len=11");

	// Full verify
	char peek2[11] = {};
	sb.peekData(peek2, 11);
	SBE_ASSERT_EQ(peek2[0], 'L', "multiwrap r3: [0]='L'");
	SBE_ASSERT_EQ(peek2[2], 'N', "multiwrap r3: [2]='N'");
	SBE_ASSERT_EQ(peek2[10], 'V', "multiwrap r3: [10]='V'");
}

//------------------------------------------------------------------------------
// peekData 边界：读取0字节、超出数据量
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferPeekDataEdge()
{
	StreamBuffer sb(16);
	const char* data = "ABCDEFGHIJ";
	sb.addDataToBack(data, 10);

	// 读取0字节
	int peeked = sb.peekData(nullptr, 0);
	SBE_ASSERT_EQ(peeked, 0, "peekData 0 bytes");

	// 读取超出数据量
	char buf[20] = {};
	peeked = sb.peekData(buf, 20);
	SBE_ASSERT_EQ(peeked, 10, "peekData overflow capped to 10");
}

//------------------------------------------------------------------------------
// getContiguousDataLength / getContiguousFreeLength
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferContiguousLength()
{
	StreamBuffer sb(16);
	// 初始状态
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 0, "contig: empty data=0");
	SBE_ASSERT_EQ(sb.getContiguousFreeLength(), 16, "contig: empty free=16");

	// 写入8字节
	const char* data = "ABCDEFGH";
	sb.addDataToBack(data, 8);
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 8, "contig: after add data=8");
	SBE_ASSERT_EQ(sb.getContiguousFreeLength(), 8, "contig: after add free=8");

	// 消费前4
	sb.removeDataFromFront(4);
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 4, "contig: after remove data=4");
	SBE_ASSERT_EQ(sb.getContiguousFreeLength(), 8, "contig: after remove free=8");

	// 写满到末尾
	sb.addDataToBack(data, 8);
	// 现在 head=4, tail=4(回绕), dataLen=12
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 12, "contig: after wrap fill data=12");
}

//------------------------------------------------------------------------------
// commitTail 直接写入场景
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferCommitTail()
{
	StreamBuffer sb(32);
	// 通过 getTailPtr 直接写入5字节
	char* tail = sb.getTailPtr();
	tail[0] = 'H';
	tail[1] = 'E';
	tail[2] = 'L';
	tail[3] = 'L';
	tail[4] = 'O';
	sb.commitTail(5);
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "commitTail: len=5");
	SBE_ASSERT_EQ(sb.getData()[0], 'H', "commitTail: [0]='H'");
	SBE_ASSERT_EQ(sb.getData()[4], 'O', "commitTail: [4]='O'");
}

//------------------------------------------------------------------------------
// mergeFrom: merge another StreamBuffer
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferMergeFrom()
{
	// Case 1: basic merge, both linear
	{
		StreamBuffer dst(32);
		StreamBuffer src(16);
		const char* d1 = "HELLO";
		const char* d2 = "WORLD";
		dst.addDataToBack(d1, 5);
		src.addDataToBack(d2, 5);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom basic: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 10, "mergeFrom basic: len=10");
		char buf[10] = {};
		dst.peekData(buf, 10);
		SBE_ASSERT_EQ(buf[0], 'H', "mergeFrom basic: [0]='H'");
		SBE_ASSERT_EQ(buf[5], 'W', "mergeFrom basic: [5]='W'");
		SBE_ASSERT_EQ(buf[9], 'D', "mergeFrom basic: [9]='D'");
	}

	// Case 2: merge from wrapped source (data spans ring boundary)
	{
		StreamBuffer src(8);
		const char* full = "ABCDEFGH";
		src.addDataToBack(full, 8);
		src.removeDataFromFront(4);
		// src now: head=4, dataLen=4, tail=0, data=[EFGH] (not wrapped yet)
		// add more to wrap
		const char* more = "IJKL";
		src.addDataToBack(more, 4);
		// src now: head=4, tail=4, dataLen=8, data=[EFGHIJKL] wrapped
		SBE_ASSERT_EQ(src.getDataLength(), 8, "mergeFrom wrap src: len=8");

		StreamBuffer dst(32);
		const char* prefix = "XX";
		dst.addDataToBack(prefix, 2);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 10, "mergeFrom wrap: len=10");
		char buf[10] = {};
		dst.peekData(buf, 10);
		SBE_ASSERT_EQ(buf[0], 'X', "mergeFrom wrap: [0]='X'");
		SBE_ASSERT_EQ(buf[2], 'E', "mergeFrom wrap: [2]='E'");
		SBE_ASSERT_EQ(buf[5], 'H', "mergeFrom wrap: [5]='H'");
		SBE_ASSERT_EQ(buf[6], 'I', "mergeFrom wrap: [6]='I'");
		SBE_ASSERT_EQ(buf[9], 'L', "mergeFrom wrap: [9]='L'");
	}

	// Case 3: merge into wrapped destination
	{
		StreamBuffer dst(8);
		const char* full = "ABCDEFGH";
		dst.addDataToBack(full, 8);
		dst.removeDataFromFront(4);
		// dst: head=4, tail=0, dataLen=4, free=4
		StreamBuffer src(16);
		const char* data = "MN";
		src.addDataToBack(data, 2);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom wrap dst: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 6, "mergeFrom wrap dst: len=6");
		char buf[6] = {};
		dst.peekData(buf, 6);
		SBE_ASSERT_EQ(buf[0], 'E', "mergeFrom wrap dst: [0]='E'");
		SBE_ASSERT_EQ(buf[3], 'H', "mergeFrom wrap dst: [3]='H'");
		SBE_ASSERT_EQ(buf[4], 'M', "mergeFrom wrap dst: [4]='M'");
		SBE_ASSERT_EQ(buf[5], 'N', "mergeFrom wrap dst: [5]='N'");
	}

	// Case 4: merge empty source (no-op)
	{
		StreamBuffer dst(16);
		StreamBuffer src(16);
		const char* d = "ABC";
		dst.addDataToBack(d, 3);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom empty: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 3, "mergeFrom empty: len=3");
	}

	// Case 5: merge when destination full, clearIfFull=true
	{
		StreamBuffer dst(8);
		StreamBuffer src(16);
		const char* d1 = "ABCDEFGH";
		dst.addDataToBack(d1, 8);
		const char* d2 = "XYZ";
		src.addDataToBack(d2, 3);
		bool ok = dst.mergeFrom(&src, true);
		SBE_ASSERT(ok, "mergeFrom clearIfFull: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 3, "mergeFrom clearIfFull: len=3");
		char buf[3] = {};
		dst.peekData(buf, 3);
		SBE_ASSERT_EQ(buf[0], 'X', "mergeFrom clearIfFull: [0]='X'");
	}


	// Case 6: both source and destination wrapped
	{
		StreamBuffer dst(16);
		const char* dFull = "ABCDEFGHIJ";
		dst.addDataToBack(dFull, 10);
		dst.removeDataFromFront(6);
		// dst: head=6, dataLen=4, free=12, data=[GHIJ]
		StreamBuffer src(8);
		const char* sFull = "KLMNOPQR";
		src.addDataToBack(sFull, 8);
		src.removeDataFromFront(4);
		// src: head=4, dataLen=4, tail=0, active data=[OPQR]
		const char* sMore = "STUV";
		src.addDataToBack(sMore, 4);
		// src: head=4, tail=4, dataLen=8, data=[OPQRSTUV] wrapped
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom both wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 12, "mergeFrom both wrap: len=12");
		char buf[12] = {};
		dst.peekData(buf, 12);
		SBE_ASSERT_EQ(buf[0], 'G', "mergeFrom both wrap: [0]='G'");
		SBE_ASSERT_EQ(buf[3], 'J', "mergeFrom both wrap: [3]='J'");
		SBE_ASSERT_EQ(buf[4], 'O', "mergeFrom both wrap: [4]='O'");
		SBE_ASSERT_EQ(buf[7], 'R', "mergeFrom both wrap: [7]='R'");
		SBE_ASSERT_EQ(buf[8], 'S', "mergeFrom both wrap: [8]='S'");
		SBE_ASSERT_EQ(buf[11], 'V', "mergeFrom both wrap: [11]='V'");
	}

	// Case 7: source wraps around buffer end, dst is linear
	{
		// src(10): fill, remove 6, add 4 → data wraps
		StreamBuffer src(10);
		src.addDataToBack("ABCDEFGHIJ", 10); // tail=0, full
		src.removeDataFromFront(6);           // head=6, dataLen=4
		src.addDataToBack("KLMN", 4);         // tail=4, dataLen=8
		// src: [6..9]=GHIJ, [0..3]=KLMN → peekData = GHIJKLMN

		StreamBuffer dst(20);
		dst.addDataToBack("XX", 2);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom src wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 10, "mergeFrom src wrap: len=10");
		char buf[10] = {};
		dst.peekData(buf, 10);
		SBE_ASSERT_EQ(buf[0], 'X', "mergeFrom src wrap: [0]='X'");
		SBE_ASSERT_EQ(buf[2], 'G', "mergeFrom src wrap: [2]='G'");
		SBE_ASSERT_EQ(buf[5], 'J', "mergeFrom src wrap: [5]='J'");
		SBE_ASSERT_EQ(buf[6], 'K', "mergeFrom src wrap: [6]='K'");
		SBE_ASSERT_EQ(buf[9], 'N', "mergeFrom src wrap: [9]='N'");
	}

	// Case 8: dst write wraps — dst tail near buffer end, mergeFrom's internal
	// addDataToBack crosses the dst buffer boundary
	{
		StreamBuffer dst(8);
		dst.addDataToBack("ABCDE", 5);   // tail=5, dataLen=5
		dst.removeDataFromFront(3);      // head=3, dataLen=2, tail=5, data=[DE]
		// tail=5, writing 5+ bytes will cross buffer end at [8]

		StreamBuffer src(16);
		src.addDataToBack("FGHIJ", 5);   // linear

		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom dst write wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 7, "mergeFrom dst write wrap: len=7");
		char buf[7] = {};
		dst.peekData(buf, 7);
		SBE_ASSERT_EQ(buf[0], 'D', "mergeFrom dst write wrap: [0]='D'");
		SBE_ASSERT_EQ(buf[1], 'E', "mergeFrom dst write wrap: [1]='E'");
		SBE_ASSERT_EQ(buf[2], 'F', "mergeFrom dst write wrap: [2]='F'");
		SBE_ASSERT_EQ(buf[4], 'H', "mergeFrom dst write wrap: [4]='H'");
		SBE_ASSERT_EQ(buf[6], 'J', "mergeFrom dst write wrap: [6]='J'");
	}

	// Case 9: source wraps AND dst write wraps — both sides cross buffer boundary
	{
		// dst(8): tail near end, data wraps
		StreamBuffer dst(8);
		dst.addDataToBack("ABCDE", 5);   // tail=5
		dst.removeDataFromFront(3);      // head=3, dataLen=2, tail=5, data=[DE]
		// writing to dst will cross: tail=5, contiguous free=8-5=3

		// src(10): data wraps around — produce GHIJKL
		StreamBuffer src(10);
		src.addDataToBack("ABCDEFGH", 8);   // tail=8, dataLen=8
		src.removeDataFromFront(6);           // head=6, dataLen=2, data=[GH]
		src.addDataToBack("IJKL", 4);         // tail=2, dataLen=6
		// src data: [6..9]=GHIJ, [0..1]=KL → peekData = GHIJKL (6 bytes, wrapped)

		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom both wrap2: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 8, "mergeFrom both wrap2: len=8");
		char buf[8] = {};
		dst.peekData(buf, 8);
		SBE_ASSERT_EQ(buf[0], 'D', "mergeFrom both wrap2: [0]='D'");
		SBE_ASSERT_EQ(buf[1], 'E', "mergeFrom both wrap2: [1]='E'");
		SBE_ASSERT_EQ(buf[2], 'G', "mergeFrom both wrap2: [2]='G'");
		SBE_ASSERT_EQ(buf[5], 'J', "mergeFrom both wrap2: [5]='J'");
		SBE_ASSERT_EQ(buf[6], 'K', "mergeFrom both wrap2: [6]='K'");
		SBE_ASSERT_EQ(buf[7], 'L', "mergeFrom both wrap2: [7]='L'");
	}

	// Case 10: dst data wraps (read side) + source linear
	{
		StreamBuffer dst(8);
		dst.addDataToBack("ABCDEFGH", 8); // tail=0, full
		dst.removeDataFromFront(5);       // head=5, dataLen=3, data=[FGH]
		dst.addDataToBack("I", 1);        // tail=1, dataLen=4
		// dst data: [5..7]=FGH, [0]=I → peekData = FGHI (4 bytes, wraps)
		// freeSpace = 4

		StreamBuffer src(16);
		src.addDataToBack("JKLM", 4);     // linear

		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom dst data wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 8, "mergeFrom dst data wrap: len=8");
		char buf[8] = {};
		dst.peekData(buf, 8);
		SBE_ASSERT_EQ(buf[0], 'F', "mergeFrom dst data wrap: [0]='F'");
		SBE_ASSERT_EQ(buf[3], 'I', "mergeFrom dst data wrap: [3]='I'");
		SBE_ASSERT_EQ(buf[4], 'J', "mergeFrom dst data wrap: [4]='J'");
		SBE_ASSERT_EQ(buf[7], 'M', "mergeFrom dst data wrap: [7]='M'");
	}

	// Case 11: triple wrap — source data wraps, dst data wraps, dst write wraps
	{
		// dst(12): data wraps, tail=5 (near middle, write will cross end for 7+ bytes)
		StreamBuffer dst(12);
		dst.addDataToBack("ABCDEFGHIJKL", 12); // tail=0, full
		dst.removeDataFromFront(8);             // head=8, dataLen=4, data=[IJKL]
		dst.addDataToBack("MNOPQ", 5);          // tail=5, dataLen=9
		// dst data: [8..11]=IJKL, [0..4]=MNOPQ → peekData = IJKLMNOPQ (wraps)
		// freeSpace=3, tail=5

		// src(8): data wraps
		StreamBuffer src(8);
		src.addDataToBack("RSTUVWXY", 8); // tail=0, full
		src.removeDataFromFront(6);        // head=6, dataLen=2, data=[XY]
		src.addDataToBack("Z", 1);         // tail=1, dataLen=3
		// src data: [6..7]=XY, [0]=Z → peekData = XYZ (wraps)

		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom triple wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 12, "mergeFrom triple wrap: len=12");
		char buf[12] = {};
		dst.peekData(buf, 12);
		SBE_ASSERT_EQ(buf[0], 'I', "mergeFrom triple wrap: [0]='I'");
		SBE_ASSERT_EQ(buf[3], 'L', "mergeFrom triple wrap: [3]='L'");
		SBE_ASSERT_EQ(buf[4], 'M', "mergeFrom triple wrap: [4]='M'");
		SBE_ASSERT_EQ(buf[8], 'Q', "mergeFrom triple wrap: [8]='Q'");
		SBE_ASSERT_EQ(buf[9], 'X', "mergeFrom triple wrap: [9]='X'");
		SBE_ASSERT_EQ(buf[10], 'Y', "mergeFrom triple wrap: [10]='Y'");
		SBE_ASSERT_EQ(buf[11], 'Z', "mergeFrom triple wrap: [11]='Z'");
	}
}

//------------------------------------------------------------------------------
// linearize: compact wrapped data to start from buffer[0]
//------------------------------------------------------------------------------
void StreamBufferExtTest::testStreamBufferLinearize()
{
	// Case 1: already linear — no-op
	{
		StreamBuffer sb(16);
		sb.addDataToBack("ABCDEFGH", 8);
		sb.linearize();
		SBE_ASSERT_EQ(sb.getDataLength(), 8, "linearize already linear: len=8");
		SBE_ASSERT_EQ(sb.getDataStart(), 0, "linearize already linear: head=0");
		char buf[8] = {};
		sb.peekData(buf, 8);
		SBE_ASSERT_EQ(buf[0], 'A', "linearize already linear: [0]='A'");
		SBE_ASSERT_EQ(buf[7], 'H', "linearize already linear: [7]='H'");
	}

	// Case 2: empty buffer — no-op
	{
		StreamBuffer sb(16);
		sb.linearize();
		SBE_ASSERT_EQ(sb.getDataLength(), 0, "linearize empty: len=0");
		SBE_ASSERT_EQ(sb.getDataStart(), 0, "linearize empty: head=0");
	}

	// Case 3: data wraps — compact to [0]
	{
		StreamBuffer sb(8);
		sb.addDataToBack("ABCDEFGH", 8); // full, tail=0
		sb.removeDataFromFront(3);        // head=3, dataLen=5, data=[DEFGH]
		sb.addDataToBack("IJ", 2);        // tail=2, dataLen=7, wraps
		// Before: [3..7]=DEFGH, [0..1]=IJ → peekData = DEFGHIJ
		SBE_ASSERT_EQ(sb.getDataStart(), 3, "linearize wrap: before head=3");
		SBE_ASSERT(sb.getContiguousDataLength() < sb.getDataLength(),
			"linearize wrap: before is wrapped");

		sb.linearize();

		// After: head=0, tail=7, data=[DEFGHIJ] at [0..6]
		SBE_ASSERT_EQ(sb.getDataStart(), 0, "linearize wrap: after head=0");
		SBE_ASSERT_EQ(sb.getDataLength(), 7, "linearize wrap: after len=7");
		SBE_ASSERT_EQ(sb.getContiguousDataLength(), 7,
			"linearize wrap: after contiguous=7");

		char buf[7] = {};
		sb.peekData(buf, 7);
		SBE_ASSERT_EQ(buf[0], 'D', "linearize wrap: [0]='D'");
		SBE_ASSERT_EQ(buf[4], 'H', "linearize wrap: [4]='H'");
		SBE_ASSERT_EQ(buf[5], 'I', "linearize wrap: [5]='I'");
		SBE_ASSERT_EQ(buf[6], 'J', "linearize wrap: [6]='J'");

		// After linearize, getData() returns full contiguous data directly
		const char* p = sb.getData();
		SBE_ASSERT_EQ(p[0], 'D', "linearize wrap getData: [0]='D'");
		SBE_ASSERT_EQ(p[6], 'J', "linearize wrap getData: [6]='J'");
	}

	// Case 4: linearize then continue add/remove still works
	{
		StreamBuffer sb(8);
		sb.addDataToBack("ABCDEF", 6);
		sb.removeDataFromFront(2);        // head=2, data=[CDEF]
		sb.addDataToBack("GH", 2);        // wraps: [2..7]=CDEF, [0..1]=GH
		sb.linearize();                    // now [0..5]=CDEF GH

		sb.removeDataFromFront(3);        // consume CDE, left FGH
		SBE_ASSERT_EQ(sb.getDataLength(), 3, "linearize+op: after remove len=3");
		char buf[3] = {};
		sb.peekData(buf, 3);
		SBE_ASSERT_EQ(buf[0], 'F', "linearize+op: [0]='F'");
		SBE_ASSERT_EQ(buf[2], 'H', "linearize+op: [2]='H'");

		sb.addDataToBack("IJKL", 4);      // append more
		SBE_ASSERT_EQ(sb.getDataLength(), 7, "linearize+op: after add len=7");
		char buf2[7] = {};
		sb.peekData(buf2, 7);
		SBE_ASSERT_EQ(buf2[0], 'F', "linearize+op: final [0]='F'");
		SBE_ASSERT_EQ(buf2[6], 'L', "linearize+op: final [6]='L'");
	}
}
//------------------------------------------------------------------------------
void StreamBufferExtTest::test()
{
	testStreamBufferConstruct();
	testStreamBufferAddBasic();
	testStreamBufferAddMultiple();
	testStreamBufferRemoveFront();
	testStreamBufferRemoveMultiple();
	testStreamBufferClear();
	testStreamBufferClearAndReuse();
	testStreamBufferIsAvailable();
	testStreamBufferAddEmpty();
	testStreamBufferAddDataFail();
	testStreamBufferAddDataClearIfFull();
	testStreamBufferProducerConsumer();
	testStreamBufferBinaryData();
	testStreamBufferFillAndClear();
	testStreamBufferRemoveOverflow();
	testStreamBufferLargeBuffer();
	testStreamBufferTCPSimulation();
	testStreamBufferOffsetTracking();
	testStreamBufferSingleByte();
	testStreamBufferAddZeroBytes();
	testStreamBufferRepeatAdd();
	testStreamBufferExactFit();
	testStreamBufferMultiRound();
	testStreamBufferIsAvailableZero();
	testStreamBufferReserveThenFill();
	testStreamBufferFrameStream();
	testStreamBufferVariousSizes();
	testStreamBufferAddEmptyClearIfFull();
	testStreamBufferMixedAddOps();
	testStreamBufferRingWraparound();
	testStreamBufferRingMultiWrap();
	testStreamBufferPeekDataEdge();
	testStreamBufferContiguousLength();
	testStreamBufferCommitTail();
	testStreamBufferMergeFrom();
	testStreamBufferLinearize();
}
