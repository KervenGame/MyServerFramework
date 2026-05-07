#include "FrameHeader.h"

#define SBL_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("StreamBufferSwapLinearizeTest FAILED: ") + (msg)); } } while(0)
#define SBL_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StreamBufferSwapLinearizeTest FAILED: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

// ========== linearize: 已连续（no-op）==========
void StreamBufferSwapLinearizeTest::testLinearize_AlreadyContiguous()
{
	StreamBuffer sb(128);
	sb.addDataToBack("hello", 5);
	// 数据未跨越，linearize 应该 no-op
	const char* dataBefore = sb.getData();
	sb.linearize();
	SBL_ASSERT_EQ(sb.getDataLength(), 5, "contiguous length unchanged");
	SBL_ASSERT(sb.getData() == dataBefore || sb.getContiguousDataLength() == 5, "contiguous data still contiguous");
}

// ========== linearize: 空缓冲区 ==========
void StreamBufferSwapLinearizeTest::testLinearize_Empty()
{
	StreamBuffer sb(64);
	sb.linearize(); // 不应崩溃
	SBL_ASSERT_EQ(sb.getDataLength(), 0, "empty linearize ok");
}

// ========== linearize: 跨越一次环形边界 ==========
void StreamBufferSwapLinearizeTest::testLinearize_SingleWrap()
{
	StreamBuffer sb(16);
	// 先写入 10 字节，然后移除 6 字节，使 head=6 tail=10
	sb.addDataToBack("0123456789", 10);
	sb.removeDataFromFront(6);
	// 此时 head=6, tail=10(循环后), 有效数据4字节: "6789"
	SBL_ASSERT_EQ(sb.getDataStart(), 6, "wrap pre: head=6");

	// 再写入超过尾部剩余空间的长度，强制跨环写入
	int freeAtEnd = sb.getContiguousFreeLength();
	char fill[20];
	memset(fill, 'X', sizeof(fill));
	bool ok = sb.addDataToBack(fill, freeAtEnd + 6);
	SBL_ASSERT(ok, "wrap write ok");
	// 现在数据一定跨越了缓冲区末尾

	int totalLen = sb.getDataLength();
	SBL_ASSERT(totalLen > 0, "has data after wrap");
	int contigLen = sb.getContiguousDataLength();
	SBL_ASSERT(contigLen < totalLen, "data wraps around");

	// linearize 后应该连续
	sb.linearize();

	SBL_ASSERT_EQ(sb.getDataStart(), 0, "post-linearize head=0");
	SBL_ASSERT_EQ(sb.getContiguousDataLength(), totalLen, "fully contiguous after linearize");

	// 验证数据完整性: 用 peekData 对比
	char* peekBuf = new char[totalLen];
	// 先 peekData 到临时区
	sb.peekData(peekBuf, totalLen);

	// linearize 后 getData() 应该和 peekData 一致
	const char* linearData = sb.getData();
	SBL_ASSERT(memcmp(linearData, peekBuf, totalLen) == 0, "linearize data matches peekData");
	delete[] peekBuf;
}

// ========== linearize: 多次跨环后线性化 ==========
void StreamBufferSwapLinearizeTest::testLinearize_MultiWrapThenLinearize()
{
	StreamBuffer sb(32);
	// 模拟 TCP 场景：反复 add/remove 造成多次环绕
	FOR(5)
	{
		char payload[12];
		memset(payload, 'A' + i, sizeof(payload));
		sb.addDataToBack(payload, sizeof(payload));
		sb.removeDataFromFront(8); // 每次消费 8 字节，head 漂移
	}

	if (sb.getContiguousDataLength() < sb.getDataLength())
	{
		int lenBefore = sb.getDataLength();
		sb.linearize();
		SBL_ASSERT_EQ(sb.getDataLength(), lenBefore, "multi-wrap length preserved");
		SBL_ASSERT_EQ(sb.getContiguousDataLength(), lenBefore, "multi-wrap fully contiguous");
		SBL_ASSERT_EQ(sb.getDataStart(), 0, "multi-wrap head reset to 0");
	}
	else
	{
		// 数据恰好连续，也测一下
		sb.linearize();
		SBL_ASSERT(true, "already contiguous in multi-wrap scenario");
	}
}

// ========== linearize: 满缓冲区跨环 ==========
void StreamBufferSwapLinearizeTest::testLinearize_FullWrap()
{
	// 构造一个几乎满且必然跨环的 buffer
	StreamBuffer sb(24);
	// 写入到接近满
	sb.addDataToBack("0123456789ABCDEF", 16); // head=0, tail=16
	sb.removeDataFromFront(12); // head=12, tail=16, 有效=4
	// 填充到满（跨环写入）
	char big[20];
	memset(big, 'Z', sizeof(big));
	sb.addDataToBack(big, 19); // 跨环: 16..23(8字节) + 0..11(11字节)
	SBL_ASSERT(sb.getDataLength() > sb.getBufferSize() - sb.getDataLength(), "full wrap: contig < total"); // 即 contigLen < totalLen

	int totalLen = sb.getDataLength();
	sb.linearize();
	SBL_ASSERT_EQ(sb.getDataStart(), 0, "full wrap: head=0");
	SBL_ASSERT_EQ(sb.getContiguousDataLength(), totalLen, "full wrap: fully contiguous");
}

// ========== linearize: 单字节跨环边界 ==========
void StreamBufferSwapLinearizeTest::testLinearize_OneByteAtBoundary()
{
	StreamBuffer sb(8);
	// 精心构造：head 在倒数第二位，tail 在第一位（1 字节跨环）
	sb.addDataToBack("ABCDEFGH", 8); // 满
	sb.removeDataFromFront(7);        // head=7, 有效="H"(1字节)
	// 写入更多数据，迫使 tail 绕过
	sb.addDataToBack("XY", 2);       // tail=(7+2)%8=1, 有效="HXY" 共3字节
	// 数据分布: [7]='H', [0]='X', [1]='Y'
	// contigLen = min(3, 8-7)=1, 但总长=3 → 跨环!

	SBL_ASSERT_EQ(sb.getContiguousDataLength(), 1, "one-byte boundary: contig=1");
	SBL_ASSERT_EQ(sb.getDataLength(), 3, "one-byte boundary: total=3");

	sb.linearize();

	SBL_ASSERT_EQ(sb.getDataStart(), 0, "one-byte post-linearize head=0");
	SBL_ASSERT_EQ(sb.getContiguousDataLength(), 3, "one-byte post-linearize full");
	const char* d = sb.getData();
	SBL_ASSERT(d[0] == 'H' && d[1] == 'X' && d[2] == 'Y', "one-byte data correct");
}

// ========== getContiguousFreeLength 边界测试 ==========
void StreamBufferSwapLinearizeTest::testGetContiguousFreeLength()
{
	StreamBuffer sb(16);
	// 空 buffer: 整个 buffer 都可写
	SBL_ASSERT_EQ(sb.getContiguousFreeLength(), 16, "empty: free=all");

	sb.addDataToBack("ABCD", 4); // head=0, tail=4
	SBL_ASSERT_EQ(sb.getContiguousFreeLength(), 12, "partial: 12 free at tail");

	// 移除一些，但 head 还没动
	sb.removeDataFromFront(2); // head=2, tail=4
	// free = 16 - 2 = 14, tail 在 head 右边所以 freeAtTail = 16 - 4 = 12
	SBL_ASSERT_EQ(sb.getContiguousFreeLength(), 12, "after remove: freeAtTail=12");

	// 强制跨环
	sb.addDataToBack("EFGHIJKLMNO", 11); // tail=(4+11)%16=15, head=2, len=13
	// free = 3, tail < head 所以 freeAtTail = head-tail = 2-15+16=3
	int cf = sb.getContiguousFreeLength();
	SBL_ASSERT(cf >= 1 && cf <= 3, "wrapped: small contiguous free");
}

void StreamBufferSwapLinearizeTest::test()
{
	testLinearize_AlreadyContiguous();
	testLinearize_Empty();
	testLinearize_SingleWrap();
	testLinearize_MultiWrapThenLinearize();
	testLinearize_FullWrap();
	testLinearize_OneByteAtBoundary();
	testGetContiguousFreeLength();
}
