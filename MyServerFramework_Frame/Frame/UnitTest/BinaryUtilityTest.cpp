#include "FrameHeader.h"
struct BinaryUtilityExtTestImpl
{

    // BinaryUtilityExtTestImpl_BUE_ASSERT: BinaryUtilityExtTest 专用断言宏
#define BinaryUtilityExtTestImpl_BUE_ASSERT(expr) \
	if (!(expr)) { ERROR("BinaryUtilityExtTest FAILED: " #expr); }

// ========== getByte / setByte ==========
    static void testGetSetByte()
    {
        // int 的字节操作
        int val = 0x04030201;  // [0]=0x01 [1]=0x02 [2]=0x03 [3]=0x04
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(val, 0) == 0x01);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(val, 1) == 0x02);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(val, 2) == 0x03);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(val, 3) == 0x04);

        // setByte
        int v = 0;
        BinaryUtility::setByte(v, (byte)0xAB, 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 0) == 0xAB);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 1) == 0x00);

        BinaryUtility::setByte(v, (byte)0xCD, 2);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 2) == 0xCD);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 0) == 0xAB);  // 其他位不变

        // short
        short sv = 0x1234;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(sv, 0) == 0x34);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(sv, 1) == 0x12);

        // ushort
        ushort usv = 0xABCD;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(usv, 0) == 0xCD);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(usv, 1) == 0xAB);

        // llong
        llong lv = 0x0807060504030201LL;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(lv, 0) == 0x01);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(lv, 7) == 0x08);

        // setByte on llong
        llong lv2 = 0;
        BinaryUtility::setByte(lv2, (byte)0xFF, 3);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(lv2, 3) == 0xFF);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(lv2, 0) == 0x00);

    }

    // ========== hasBit / getBit / setBit ==========
    static void testBitGetSet()
    {
        int v = 0;
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(v, 0));

        BinaryUtility::setBit(v, 0, 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(v, 0));
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(v, 0) == 1);

        BinaryUtility::setBit(v, 0, 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(v, 0));
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(v, 0) == 0);

        // 多位
        int v2 = 0;
        BinaryUtility::setBit(v2, 3, 1);
        BinaryUtility::setBit(v2, 7, 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(v2, 3));
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(v2, 7));
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(v2, 0));
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(v2, 1));
        BinaryUtilityExtTestImpl_BUE_ASSERT(v2 == ((1 << 3) | (1 << 7)));

        // setBitOne / setBitZero
        int v3 = 0;
        BinaryUtility::setBitOne(v3, 5);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(v3, 5));
        BinaryUtility::setBitZero(v3, 5);
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(v3, 5));

        // 所有 int 位
        int v4 = 0;
        for (int i = 0; i < 32; ++i)
        {
            BinaryUtility::setBitOne(v4, i);
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(v4, i));
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(v4, i) == 1);
        }
        BinaryUtilityExtTestImpl_BUE_ASSERT(v4 == -1);  // 所有位都是1

        for (int i = 0; i < 32; ++i)
        {
            BinaryUtility::setBitZero(v4, i);
            BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(v4, i));
        }
        BinaryUtilityExtTestImpl_BUE_ASSERT(v4 == 0);

    }

    // ========== setBit 不同整数类型 ==========
    static void testBitTypeVariants()
    {
        // byte
        byte bv = 0;
        BinaryUtility::setBitOne(bv, 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(bv, 0));
        BinaryUtility::setBitZero(bv, 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(bv, 0));
        BinaryUtility::setBitOne(bv, 7);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(bv, 7));

        // short
        short sv = 0;
        BinaryUtility::setBitOne(sv, 15);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(sv, 15));
        BinaryUtility::setBitZero(sv, 15);
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(sv, 15));

        // ushort
        ushort usv = 0;
        BinaryUtility::setBitOne(usv, 8);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(usv, 8));

        // llong
        llong lv = 0;
        BinaryUtility::setBitOne(lv, 63);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(lv, 63));
        BinaryUtility::setBitZero(lv, 63);
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(lv, 63));

        // ullong
        ullong ulv = 0;
        BinaryUtility::setBitOne(ulv, 63);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(ulv, 63));

    }

    // ========== getHighestBit / setHighestBit ==========
    static void testHighestBit()
    {
        // 正数最高位是0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((int)0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((int)1) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(INT_MAX) == 0);

        // 负数/最高位为1
        int neg = -1;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(neg) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(INT_MIN) == 1);

        // setHighestBit 置1
        int v = 0;
        BinaryUtility::setHighestBit(v, true);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(v) == 1);
        BinaryUtility::setHighestBit(v, false);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(v) == 0);

        // byte 最高位
        byte bv = 0x80;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(bv) == 1);
        bv = 0x7F;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(bv) == 0);

        // llong 最高位
        llong lv = LLONG_MIN;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(lv) == 1);
        lv = LLONG_MAX;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(lv) == 0);

    }

    // ========== getLowestBit / setLowestBit ==========
    static void testLowestBit()
    {
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(1) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(2) == 0);  // 10 最低位是0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(3) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(-1) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(-2) == 0);  // -2 = ...11111110，最低位是0

        int v = 0;
        BinaryUtility::setLowestBit(v, true);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(v) == 1);
        BinaryUtility::setLowestBit(v, false);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(v) == 0);

        // byte
        byte bv = 0xFE;  // 11111110
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(bv) == 0);
        BinaryUtility::setLowestBit(bv, true);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(bv) == 1);

    }

    // ========== getBits / bitCountToByteCount ==========
    static void testGetBitsBitCount()
    {
        // getBits(value, start, count)
        // 0b10110101 = 0xB5, 从bit0开始取3位 = 101 = 5
        byte v = 0b10110101;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 0, 3) == 5);
        // 从bit4开始取3位: bit4=1,bit5=1,bit6=0 低到高 = 0b011 = 3
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 4, 3) == 0b011);
        // 从bit2开始取4位: bit2=1,bit3=0,bit4=1,bit5=1 低到高 = 0b1101 = 13
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 2, 4) == 0b1101);
        // 取1位
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 0, 1) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 1, 1) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 7, 1) == 1);

        // bitCountToByteCount
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::bitCountToByteCount(0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::bitCountToByteCount(1) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::bitCountToByteCount(8) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::bitCountToByteCount(9) == 2);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::bitCountToByteCount(16) == 2);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::bitCountToByteCount(17) == 3);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::bitCountToByteCount(24) == 3);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::bitCountToByteCount(25) == 4);

    }

    // ========== copyBits ==========
    static void testCopyBits()
    {
        // copyBits(destValue, destStart, sourceValue, sourceStart, writeBitCount)
        byte src = 0b11010110;
        byte dst = 0;
        // 将 src 从 bit2 开始的 3 位（101）复制到 dst 从 bit0 开始
        BinaryUtility::copyBits(dst, 0, src, 2, 3);
        // 0b101 = 5
        BinaryUtilityExtTestImpl_BUE_ASSERT(dst == 5);

        byte dst2 = 0;
        // 将 src bit0 开始 2位（10）复制到 dst2 bit4 开始
        BinaryUtility::copyBits(dst2, 4, src, 0, 2);
        // 0b00100000 | (10 << 4) = 0b00100000 = 32
        // src bit0-1 = 10, 即 0b10 = 2，放到 dst2 bit4 = 0b00100000 = 32
        BinaryUtilityExtTestImpl_BUE_ASSERT(dst2 == 0b00100000);

        // 简化版 copyBits(byte&, destStart, sourceValue)
        byte dst3 = 0;
        BinaryUtility::copyBits(dst3, (byte)0, (byte)0b1010);
        BinaryUtilityExtTestImpl_BUE_ASSERT(dst3 == 0b1010);

        byte dst4 = 0;
        BinaryUtility::copyBits(dst4, (byte)4, (byte)0b0011);
        BinaryUtilityExtTestImpl_BUE_ASSERT(dst4 == 0b00110000);

    }

    // ========== generateBitCount ==========
    static void testGenerateBitCount()
    {
        // byte: generateBitCount(0) = 0 (no bits needed)
        // generateBitCount(1) = 1, generateBitCount(2) = 2, generateBitCount(3) = 2
        // generateBitCount(127) = 7, generateBitCount(128) = 8 (for byte: 0~255)
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)1) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)2) == 2);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)3) == 2);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)4) == 3);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)7) == 3);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)8) == 4);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)127) == 7);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)128) == 8);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((byte)255) == 8);

        // ushort
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)1) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)256) == 9);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)65535) == 16);

        // uint
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)65536) == 17);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)0xFFFFFFFF) == 32);

    }

    // ========== getBufferBit / setBufferBitOne ==========
    static void testBufferBit()
    {
        char buf[4] = { 0 };
        // bitIndex 0-31 对应 buf[0-3] 的各位
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::getBufferBit(buf, 0));

        BinaryUtility::setBufferBitOne(buf, 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBufferBit(buf, 0));
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::getBufferBit(buf, 1));

        BinaryUtility::setBufferBitOne(buf, 7);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBufferBit(buf, 7));
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::getBufferBit(buf, 8));

        BinaryUtility::setBufferBitOne(buf, 8);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBufferBit(buf, 8));

        BinaryUtility::setBufferBitOne(buf, 31);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBufferBit(buf, 31));

        // 跨字节
        char buf2[2] = { 0 };
        for (int i = 0; i < 16; ++i)
        {
            BinaryUtility::setBufferBitOne(buf2, i);
        }
        BinaryUtilityExtTestImpl_BUE_ASSERT(buf2[0] == (char)0xFF && buf2[1] == (char)0xFF);

    }

    // ========== getListMax / getListMaxAbs ==========
    static void testGetListMax()
    {
        // getListMax（无符号整数）
        Vector<uint> ulist;
        ulist.add(3);
        ulist.add(7);
        ulist.add(1);
        ulist.add(5);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getListMax(ulist) == 7);

        // 单元素
        Vector<uint> single;
        single.add(42);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getListMax(single) == 42);

        // 数组重载
        uint arr[] = { 10, 3, 8, 1 };
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getListMax(arr, 4) == 10);

        // getListMaxAbs（有符号整数）
        Vector<int> slist;
        slist.add(-10);
        slist.add(5);
        slist.add(-3);
        slist.add(8);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getListMaxAbs(slist) == 10);

        // 数组重载
        int sarr[] = { -100, 50, -30 };
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getListMaxAbs(sarr, 3) == 100);

        // 全正
        Vector<int> allPos;
        allPos.add(1); allPos.add(2); allPos.add(3);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getListMaxAbs(allPos) == 3);

        // 全负
        Vector<int> allNeg;
        allNeg.add(-5); allNeg.add(-3); allNeg.add(-7);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getListMaxAbs(allNeg) == 7);

    }

    // ========== setBit 批量翻转循环 ==========
    static void testBitFlipCycle()
    {
        // 对 int 的每个 bit 进行 set/clear 循环验证
        for (int i = 0; i < 32; ++i)
        {
            int v = 0;
            BinaryUtility::setBit(v, i, 1);
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(v, i) == 1);
            BinaryUtilityExtTestImpl_BUE_ASSERT(v == (1 << i));
            BinaryUtility::setBit(v, i, 0);
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(v, i) == 0);
            BinaryUtilityExtTestImpl_BUE_ASSERT(v == 0);
        }

        // hasBit 批量
        int pattern = 0b10101010;
        for (int i = 0; i < 8; ++i)
        {
            if (i % 2 == 1)
            {
                BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(pattern, i));
            }
            else
            {
                BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(pattern, i));
            }
        }

    }

    // ========== setByte 保护其他字节 ==========
    static void testSetByteIsolation()
    {
        // 确保 setByte 不影响其他字节
        int v = 0x01020304;
        // 修改 byte[0] 为 0xFF
        BinaryUtility::setByte(v, (byte)0xFF, 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 0) == 0xFF);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 1) == 0x03);  // 不变
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 2) == 0x02);  // 不变
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 3) == 0x01);  // 不变

        // 修改 byte[3]
        BinaryUtility::setByte(v, (byte)0x00, 3);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 3) == 0x00);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 0) == 0xFF);  // 不变
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 1) == 0x03);  // 不变
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(v, 2) == 0x02);  // 不变

    }

    // ========== bit 操作和 getHighestBit 综合 ==========
    static void testBitComprehensive()
    {
        // 验证 getHighestBit 对各种有符号类型
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((char)0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((char)-1) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((short)0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((short)-1) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((llong)0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((llong)-1) == 1);

        // setHighestBit on llong
        llong lv = 0;
        BinaryUtility::setHighestBit(lv, true);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(lv) == 1);
        BinaryUtility::setHighestBit(lv, false);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(lv) == 0);

        // getLowestBit for llong
        llong lv2 = LLONG_MIN;  // 最低位是0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(lv2) == 0);
        llong lv3 = -1LL;  // 所有位是1
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(lv3) == 1);

    }

    // ========== getBits 边界 ==========
    static void testGetBitsBoundary()
    {
        // 获取全部8位
        byte v = 0b11110000;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 0, 8) == 0b11110000);

        // 获取0位
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 0, 0) == 0);

        // 各种 start+count 组合
        // 0b11110000，bit4=1, bit5=1, bit6=1, bit7=1
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 4, 4) == 0b1111);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 0, 4) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBits(v, 3, 2) == 0b10);  // bit3=0, bit4=1 → 从低到高 0b10

    }

    // ========== SIGNED_LENGTH_MAX_BIT / UNSIGNED_LENGTH_MAX_BIT 常量 ==========
    static void testBitConstArrays()
    {
        // sizeof(char)=1 → 3, sizeof(short)=2 → 4, sizeof(int)=4 → 5, sizeof(llong)=8 → 6
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::SIGNED_LENGTH_MAX_BIT[1] == 3);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::SIGNED_LENGTH_MAX_BIT[2] == 4);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::SIGNED_LENGTH_MAX_BIT[4] == 5);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::SIGNED_LENGTH_MAX_BIT[8] == 6);

        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[1] == 3);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[2] == 4);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[4] == 5);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[8] == 6);

    }

    // ========== generateBitCount stress ==========
    static void testGenerateBitCountStress()
    {
        // 验证 2^n 需要 n+1 个bit
        for (int i = 0; i < 8; ++i)
        {
            byte v = (byte)(1 << i);
            byte expected = (byte)(i + 1);
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount(v) == expected);
        }

        // 验证 2^n - 1 需要 n 个bit（全1）
        for (int i = 1; i < 8; ++i)
        {
            byte v = (byte)((1 << i) - 1);
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount(v) == (byte)i);
        }

        // ushort 大值
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)32768) == 16);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)16384) == 15);

    }

    // ========== hasBit 大量边界 ==========
    static void testHasBitStress()
    {
        // 测试每个 bit 位的独立性
        for (int i = 0; i < 32; ++i)
        {
            int v = 1 << i;
            for (int j = 0; j < 32; ++j)
            {
                if (j == i)
                {
                    BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(v, j));
                }
                else
                {
                    BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(v, j));
                }
            }
        }

        // llong 63位
        llong lv = (llong)1 << 62;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(lv, 62));
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(lv, 61));
        BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(lv, 63));

    }

    // ========== setBit / clearBit 循环测试 ==========
    static void testBitSetClearCycle()
    {
        // 对 int 的每一位都做 set/clear/check
        for (int bit = 0; bit < 31; ++bit)
        {
            int v = 0;
            BinaryUtility::setBit(v, bit, true);
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(v, bit) == 1);
            BinaryUtility::setBit(v, bit, false);
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(v, bit) == 0);
        }
        // 同时设置多位
        int mv = 0;
        BinaryUtility::setBit(mv, 0, true);
        BinaryUtility::setBit(mv, 4, true);
        BinaryUtility::setBit(mv, 8, true);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(mv, 0) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(mv, 4) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(mv, 8) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(mv, 1) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(mv, 5) == 0);
        // 清除一位后其他不变
        BinaryUtility::setBit(mv, 4, false);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(mv, 0) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(mv, 4) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(mv, 8) == 1);
    }

    // ========== hasBit 与 getBit 一致性测试 ==========
    static void testHasBitConsistency()
    {
        int v = 0b10110101;  // bits 0,2,4,5,7 set
        int setBits[] = { 0, 2, 4, 5, 7 };
        int clearBits[] = { 1, 3, 6 };
        for (int b : setBits)
        {
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(v, b));
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(v, b) == 1);
        }
        for (int b : clearBits)
        {
            BinaryUtilityExtTestImpl_BUE_ASSERT(!BinaryUtility::hasBit(v, b));
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(v, b) == 0);
        }
        // llong 一致性
        llong lv = 0LL;
        for (int bit = 0; bit < 62; bit += 5)
        {
            BinaryUtility::setBit(lv, bit, true);
        }
        for (int bit = 0; bit < 62; bit += 5)
        {
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::hasBit(lv, bit));
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getBit(lv, bit) == 1);
        }
    }

    // ========== getHighestBit 扩展测试 ==========
    static void testHighestBitExtended()
    {
        // 框架 getHighestBit 取的是"类型最高位的值(0或1)"，即符号位
        // uint: 最高位只有值 >= 0x80000000 时为 1
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(0u) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(1u) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(0x7FFFFFFFu) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(0x80000000u) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit(0xFFFFFFFFu) == 1);
        // byte: 最高位为1 => >= 128
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((byte)0) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((byte)127) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((byte)128) == 1);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((byte)255) == 1);
        // ushort
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((ushort)0x7FFF) == 0);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getHighestBit((ushort)0x8000) == 1);
    }

    // ========== getLowestBit 扩展测试 ==========
    // getLowestBit: 取 bit0（最低位）的值，返回 0 或 1，与 getHighestBit 语义对称
    static void testLowestBitExtended()
    {
        // 奇数：bit0 = 1
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(1u) == 1);    // 0b0001 → bit0=1
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(3u) == 1);    // 0b0011 → bit0=1
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(255u) == 1);  // 0xFF → bit0=1
        // 偶数：bit0 = 0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(2u) == 0);    // 0b0010 → bit0=0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(4u) == 0);    // 0b0100 → bit0=0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(8u) == 0);    // 0b1000 → bit0=0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(6u) == 0);    // 0b110  → bit0=0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(12u) == 0);   // 0b1100 → bit0=0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(24u) == 0);   // 0b11000 → bit0=0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(0b10100u) == 0);   // bit0=0
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getLowestBit(0b1000000u) == 0); // bit0=0
    }

    // ========== generateBitCount 扩展测试 ==========
    // generateBitCount: 返回"最高置1的bit下标+1"，即表示该值所需的最小bit位数
    // 例：0→0, 1→1, 2→2, 3→2, 4→3, 7→3, 8→4, 15→4, 16→5, 32→6
    static void testGenerateBitCountExtended()
    {
        // 边界值
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)0) == 0u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)1) == 1u);
        // 2的幂：恰好多用一位
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)2) == 2u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)4) == 3u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)8) == 4u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)16) == 5u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)32) == 6u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)256) == 9u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)65536) == 17u);
        // 2的幂-1：与上一个2的幂用相同位数
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)3) == 2u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)7) == 3u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)15) == 4u);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::generateBitCount((uint)31) == 5u);
        // 验证返回值就是"可表示该值的最小bit数"
        for (int bits = 1; bits <= 16; ++bits)
        {
            const uint value = (uint)bits;
            const byte bc = BinaryUtility::generateBitCount(value);
            // bc 位能表示 value：value < (1u << bc)
            BinaryUtilityExtTestImpl_BUE_ASSERT((1u << bc) > value);
            // bc-1 位不够（value >= 1u << (bc-1)，当 value 为2的幂时等号成立）
            if (bc > 0)
            {
                BinaryUtilityExtTestImpl_BUE_ASSERT((1u << (bc - 1)) <= value);
            }
        }
    }

    // ========== getByte 边界和多类型测试 ==========
    static void testGetSetByteExtended()
    {
        // llong 字节操作
        llong lv = 0x0807060504030201LL;
        for (int i = 0; i < 8; ++i)
        {
            BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(lv, i) == (byte)(i + 1));
        }

        // ushort
        ushort us = 0xABCD;
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(us, 0) == 0xCD);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(us, 1) == 0xAB);

        // setByte on llong
        llong lset = 0LL;
        BinaryUtility::setByte(lset, (byte)0xFF, 7);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(lset, 7) == 0xFF);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(lset, 0) == 0x00);

        // setByte 多次叠加
        int multi = 0;
        for (int i = 0; i < 4; ++i)
        {
            BinaryUtility::setByte(multi, (byte)(i * 0x11), i);
        }
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(multi, 0) == 0x00);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(multi, 1) == 0x11);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(multi, 2) == 0x22);
        BinaryUtilityExtTestImpl_BUE_ASSERT(BinaryUtility::getByte(multi, 3) == 0x33);
    }

    // ========== 主入口 ==========
    static void test()
    {
        testGetSetByte();
        testBitGetSet();
        testBitTypeVariants();
        testHighestBit();
        testLowestBit();
        testGetBitsBitCount();
        testCopyBits();
        testGenerateBitCount();
        testBufferBit();
        testGetListMax();
        testBitFlipCycle();
        testSetByteIsolation();
        testBitComprehensive();
        testGetBitsBoundary();
        testBitConstArrays();
        testGenerateBitCountStress();
        testHasBitStress();
        testBitSetClearCycle();
        testHasBitConsistency();
        testHighestBitExtended();
        testLowestBitExtended();
        testGenerateBitCountExtended();
        testGetSetByteExtended();
    }
};

struct BinaryUtilityExt2TestImpl
{

    // ============================================================
    // BinaryUtilityExt2Test — BinaryUtility 扩展第二批测试
    // 覆盖：read/readInverse/write/writeInverse（基础类型）
    //        readArray/readArrayInverse/writeArray/writeArrayInverse
    //        readBuffer/writeBuffer
    //        readBoolBit/writeBoolBit
    //        readSignedIntegerBit/writeSignedIntegerBit（int/short/char）
    //        readUnsignedIntegerBit/writeUnsignedIntegerBit（uint/ushort/byte）
    //        readSignedIntegerListBit/writeSignedIntegerListBit（Vector<int>）
    //        readUnsignedIntegerListBit/writeUnsignedIntegerListBit（Vector<ushort>）
    //        readFloatBit/writeFloatListBit（Vector<float>）
    //        readDoubleBit/writeDoubleListBit（Vector<double>）
    //        readVector2/writeVector2/readVector2Inverse/writeVector2Inverse
    //        readVector2Int/writeVector2Int
    //        readVector3/writeVector3/readVector3Inverse/writeVector3Inverse
    //        readVector4/writeVector4
    //        readColor/writeColor/readColorInverse/writeColorInverse
    //        crc16/crc16Byte
    //        generateBitCount（byte/ushort/uint/ullong）
    //        getBits/bitCountToByteCount
    //        setBufferBitOne/getBufferBit
    //        copyBits
    //        getListMax/getListMaxAbs/getFloatListMaxAbs/getDoubleListMaxAbs
    //        inverseByte/swapByte
    //        zeroArray
    //        fillZeroToByteEnd
    // ============================================================

#define BinaryUtilityExt2TestImpl_BUE2_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("BinaryUtilityExt2Test FAILED: ") + (msg)); }

#define BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(a, b) (fabsf((a) - (b)) < 1e-4f)
#define BinaryUtilityExt2TestImpl_BUE2_DOUBLE_EQ(a, b) (fabs((a) - (b)) < 1e-3)

// ===========================================================
// read / write — 基础 POD 类型往返
// ===========================================================
    static void testBUE2_ReadWriteInt()
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 64, idx, (int)12345678), "write int ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 4, "write int advances 4 bytes");

        int readVal = 0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 64, idx, readVal), "read int ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(readVal == 12345678, "read int value match");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 4, "read int advances 4 bytes");

    }

    static void testBUE2_ReadWriteShort()
    {
        char buf[16];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 16, idx, (short)-500), "write short ok");

        short sv = 0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 16, idx, sv), "read short ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(sv == -500, "read short value match");
    }

    static void testBUE2_ReadWriteLlong()
    {
        char buf[32];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        llong writeVal = 0x0102030405060708LL;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 32, idx, writeVal), "write llong ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 8, "write llong advances 8 bytes");

        llong readVal = 0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 32, idx, readVal), "read llong ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(readVal == writeVal, "read llong value match");
    }

    static void testBUE2_ReadWriteFloat()
    {
        char buf[16];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        float fv = 3.14159f;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 16, idx, fv), "write float ok");

        float rfv = 0.0f;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 16, idx, rfv), "read float ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rfv, fv), "read float value match");
    }

    static void testBUE2_ReadWriteDouble()
    {
        char buf[16];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        double dv = 2.718281828;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 16, idx, dv), "write double ok");

        double rdv = 0.0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 16, idx, rdv), "read double ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_DOUBLE_EQ(rdv, dv), "read double value match");
    }

    static void testBUE2_ReadWriteByte()
    {
        char buf[8];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, (byte)0xBE), "write byte ok");

        byte rb = 0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rb), "read byte ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb == 0xBE, "read byte value match");
    }

    static void testBUE2_ReadWriteChar()
    {
        char buf[8];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, (char)-100), "write char ok");

        char rc = 0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rc), "read char ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rc == -100, "read char value match");
    }

    static void testBUE2_ReadWriteUInt()
    {
        char buf[8];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        uint uv = 0xDEADBEEFu;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, uv), "write uint ok");

        uint ruv = 0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, ruv), "read uint ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(ruv == uv, "read uint value match");
    }

    static void testBUE2_ReadWriteUShort()
    {
        char buf[8];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        ushort usv = 60000u;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, usv), "write ushort ok");

        ushort rusv = 0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rusv), "read ushort ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rusv == usv, "read ushort value match");
    }

    static void testBUE2_ReadWriteBool()
    {
        char buf[8];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, true), "write bool true ok");

        bool rb = false;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rb), "read bool ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb == true, "read bool true match");

        memset(buf, 0, sizeof(buf));
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, false), "write bool false ok");
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rb), "read bool false ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb == false, "read bool false match");
    }

    // ===========================================================
    // readInverse / writeInverse
    // ===========================================================
    static void testBUE2_ReadWriteInverseInt()
    {
        char buf[16];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        // 正向写入
        int val = 0x01020304;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeInverse(buf, 16, idx, val), "writeInverse int ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 4, "writeInverse advances 4");
        // writeInverse 用 FOR_INVERSE 从 ptr[size-1] 到 ptr[0] 顺序写入 buf
        // 小端机器 0x01020304 内存布局为 ptr={0x04,0x03,0x02,0x01}
        // 逆序（ptr[3]→ptr[2]→ptr[1]→ptr[0]）写入后 buf={0x01,0x02,0x03,0x04}
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((byte)buf[0] == 0x01, "writeInverse byte[0]=0x01");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((byte)buf[1] == 0x02, "writeInverse byte[1]=0x02");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((byte)buf[2] == 0x03, "writeInverse byte[2]=0x03");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((byte)buf[3] == 0x04, "writeInverse byte[3]=0x04");

        int rval = 0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readInverse(buf, 16, idx, rval), "readInverse int ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rval == val, "readInverse int value match");
    }

    static void testBUE2_ReadWriteInverseFloat()
    {
        char buf[16];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        float fv = 1234.5f;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeInverse(buf, 16, idx, fv), "writeInverse float ok");

        float rfv = 0.0f;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readInverse(buf, 16, idx, rfv), "readInverse float ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rfv, fv), "readInverse float value match");
    }

    static void testBUE2_ReadWriteInverseLlong()
    {
        char buf[16];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        llong val = 0x0102030405060708LL;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeInverse(buf, 16, idx, val), "writeInverse llong ok");
        // 小端存储 ptr={0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01}
        // FOR_INVERSE 从 ptr[7]=0x01 到 ptr[0]=0x08 写入
        // buf[0]=0x01（最高字节），buf[7]=0x08（最低字节）
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((byte)buf[0] == 0x01, "writeInverse llong byte[0]");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((byte)buf[7] == 0x08, "writeInverse llong byte[7]");

        llong rv = 0;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readInverse(buf, 16, idx, rv), "readInverse llong ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == val, "readInverse llong value match");
    }

    // ===========================================================
    // readArray / readArrayInverse / writeArray / writeArrayInverse
    // ===========================================================
    static void testBUE2_ReadWriteArray()
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        int srcArr[5] = { 10, 20, 30, 40, 50 };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeArray(buf, 64, idx, srcArr, 5), "writeArray ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 20, "writeArray advances 5*4=20 bytes");

        int dstArr[5] = { 0 };
        idx = 0;
        BinaryUtility::readArray(buf, 64, idx, dstArr, 5);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(dstArr[0] == 10, "readArray [0]");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(dstArr[1] == 20, "readArray [1]");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(dstArr[2] == 30, "readArray [2]");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(dstArr[3] == 40, "readArray [3]");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(dstArr[4] == 50, "readArray [4]");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 20, "readArray advances 20 bytes");
    }

    static void testBUE2_ReadWriteArrayInverse()
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        int srcArr[3] = { 0x01020304, 0x05060708, 0x090A0B0C };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeArrayInverse(buf, 64, idx, srcArr, 3), "writeArrayInverse ok");

        int dstArr[3] = { 0 };
        idx = 0;
        BinaryUtility::readArrayInverse(buf, 64, idx, dstArr, 3);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(dstArr[0] == srcArr[0], "readArrayInverse [0]");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(dstArr[1] == srcArr[1], "readArrayInverse [1]");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(dstArr[2] == srcArr[2], "readArrayInverse [2]");
    }

    // ===========================================================
    // readBuffer / writeBuffer
    // ===========================================================
    static void testBUE2_ReadWriteBuffer()
    {
        char src[16] = "HelloWorld12345";
        char writeDst[32];
        memset(writeDst, 0, sizeof(writeDst));
        int writeIdx = 4;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeBuffer(writeDst, 32, writeIdx, src, 10), "writeBuffer ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(writeIdx == 14, "writeBuffer advances 10 bytes");

        char readDst[16];
        memset(readDst, 0, sizeof(readDst));
        int readIdx = 4;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readBuffer(writeDst, 32, readIdx, readDst, 16, 10), "readBuffer ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(readIdx == 14, "readBuffer advances 10 bytes");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(memcmp(readDst, src, 10) == 0, "readBuffer data match");
    }

    static void testBUE2_ReadWriteBufferBoundary()
    {
        char buf[8];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        char src[4] = { 1, 2, 3, 4 };
        // 写到缓冲区末尾
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeBuffer(buf, 8, idx, src, 4), "writeBuffer boundary ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 4, "writeBuffer idx=4");
        // 再写4字节刚好满
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeBuffer(buf, 8, idx, src, 4), "writeBuffer fill to end ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 8, "writeBuffer idx=8");
        // 写入失败——超出缓冲区
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(!BinaryUtility::writeBuffer(buf, 8, idx, src, 1), "writeBuffer overflow false");
    }

    static void testBUE2_ReadBufferBoundary()
    {
        char buf[4] = { 0x0A, 0x0B, 0x0C, 0x0D };
        char dst[4];
        int idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readBuffer(buf, 4, idx, dst, 4, 4), "readBuffer full ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 4, "readBuffer full idx=4");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((byte)dst[0] == 0x0A, "readBuffer dst[0]");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((byte)dst[3] == 0x0D, "readBuffer dst[3]");

        // 超出范围
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(!BinaryUtility::readBuffer(buf, 4, idx, dst, 4, 1), "readBuffer overflow false");
    }

    // ===========================================================
    // 读写时缓冲区不足返回 false
    // ===========================================================
    static void testBUE2_ReadWriteOverflow()
    {
        char buf[3];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        // int 需要4字节，缓冲区只有3字节，写入应失败
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(!BinaryUtility::write(buf, 3, idx, (int)100), "write int overflow false");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 0, "write int overflow idx unchanged");

        int rv = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(!BinaryUtility::read(buf, 3, idx, rv), "read int overflow false");
    }

    static void testBUE2_MultipleReadWrite()
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        BinaryUtility::write(buf, 64, idx, (int)1001);
        BinaryUtility::write(buf, 64, idx, (short)-200);
        BinaryUtility::write(buf, 64, idx, (float)9.99f);
        BinaryUtility::write(buf, 64, idx, (byte)0xFF);
        BinaryUtility::write(buf, 64, idx, (llong)987654321LL);

        int total = idx;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(total == 4 + 2 + 4 + 1 + 8, "multiple write total bytes");

        idx = 0;
        int ri = 0;
        short rs = 0;
        float rf = 0.0f;
        byte rb = 0;
        llong rl = 0;
        BinaryUtility::read(buf, 64, idx, ri);
        BinaryUtility::read(buf, 64, idx, rs);
        BinaryUtility::read(buf, 64, idx, rf);
        BinaryUtility::read(buf, 64, idx, rb);
        BinaryUtility::read(buf, 64, idx, rl);

        BinaryUtilityExt2TestImpl_BUE2_ASSERT(ri == 1001, "multi read int");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rs == -200, "multi read short");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rf, 9.99f), "multi read float");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb == 0xFF, "multi read byte");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rl == 987654321LL, "multi read llong");
    }

    // ===========================================================
    // readBoolBit / writeBoolBit
    // ===========================================================
    static void testBUE2_BoolBit()
    {
        char buf[8];
        memset(buf, 0, sizeof(buf));
        int bitIdx = 0;

        // 写入 true
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeBoolBit(buf, 8, bitIdx, true), "writeBoolBit true ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(bitIdx == 1, "writeBoolBit bitIdx=1");

        // 写入 false
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeBoolBit(buf, 8, bitIdx, false), "writeBoolBit false ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(bitIdx == 2, "writeBoolBit bitIdx=2");

        // 写入 true
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeBoolBit(buf, 8, bitIdx, true), "writeBoolBit true2 ok");

        // 读取
        bool rb = false;
        bitIdx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readBoolBit(buf, 8, bitIdx, rb), "readBoolBit ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb == true, "readBoolBit [0]=true");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(bitIdx == 1, "readBoolBit bitIdx=1");

        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readBoolBit(buf, 8, bitIdx, rb), "readBoolBit [1] ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb == false, "readBoolBit [1]=false");

        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readBoolBit(buf, 8, bitIdx, rb), "readBoolBit [2] ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb == true, "readBoolBit [2]=true");
    }

    static void testBUE2_BoolBitMultiple()
    {
        char buf[4];
        memset(buf, 0, sizeof(buf));
        int bitIdx = 0;
        // 写入8个交替的 true/false
        for (int i = 0; i < 8; i++)
        {
            BinaryUtility::writeBoolBit(buf, 4, bitIdx, (i % 2 == 0));
        }
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(bitIdx == 8, "8 boolBit write bitIdx=8");

        bitIdx = 0;
        for (int i = 0; i < 8; i++)
        {
            bool rb = false;
            BinaryUtility::readBoolBit(buf, 4, bitIdx, rb);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb == (i % 2 == 0), "boolBit alternating read");
        }
    }

    // ===========================================================
    // writeSignedIntegerBit / readSignedIntegerBit（int）
    // ===========================================================
    static void testBUE2_SignedIntBit_Int()
    {
        // 场景1：正数
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)12345, true), "write int 12345 ok");
            int rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, true), "read int 12345 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 12345, "int 12345 roundtrip");
        }
        // 场景2：负数
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)-9876, true), "write int -9876 ok");
            int rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, true), "read int -9876 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == -9876, "int -9876 roundtrip");
        }
        // 场景3：零
        {
            char buf[16];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 16, bitIdx, (int)0, true), "write int 0 ok");
            int rv = -1;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 16, bitIdx, rv, true), "read int 0 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 0, "int 0 roundtrip");
        }
        // 场景4：大正值
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)2000000000, true), "write int 2e9 ok");
            int rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, true), "read int 2e9 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 2000000000, "int 2e9 roundtrip");
        }
        // 场景5：大负值（注意不能用 INT_MIN，用 -2147483647）
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)-2147483647, true), "write int INT_MIN+1 ok");
            int rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, true), "read int INT_MIN+1 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == -2147483647, "int INT_MIN+1 roundtrip");
        }
        // 场景6：无符号模式（needWriteSign=false），负值会被当作正值取反
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)777, false), "write int 777 no-sign ok");
            int rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, false), "read int 777 no-sign ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 777, "int 777 no-sign roundtrip");
        }

    }

    static void testBUE2_SignedIntBit_Short()
    {
        // 正数
        {
            char buf[16];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 16, bitIdx, (short)1000, true), "write short 1000 ok");
            short rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 16, bitIdx, rv, true), "read short 1000 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 1000, "short 1000 roundtrip");
        }
        // 负数
        {
            char buf[16];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 16, bitIdx, (short)-32767, true), "write short -32767 ok");
            short rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 16, bitIdx, rv, true), "read short -32767 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == -32767, "short -32767 roundtrip");
        }
        // 零
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            short rv = -1;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (short)0, true), "write short 0 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read short 0 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 0, "short 0 roundtrip");
        }
        // 中间值
        {
            char buf[16];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 16, bitIdx, (short)-128, true), "write short -128 ok");
            short rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 16, bitIdx, rv, true), "read short -128 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == -128, "short -128 roundtrip");
        }

    }

    static void testBUE2_SignedIntBit_Char()
    {
        // 正数
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)100, true), "write char 100 ok");
            char rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char 100 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 100, "char 100 roundtrip");
        }
        // 负数
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)-127, true), "write char -127 ok");
            char rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char -127 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == -127, "char -127 roundtrip");
        }
        // 零
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            char rv = -1;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)0, true), "write char 0 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char 0 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 0, "char 0 roundtrip");
        }
        // 1 和 -1
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)1, true), "write char 1 ok");
            char rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char 1 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 1, "char 1 roundtrip");
        }
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)-1, true), "write char -1 ok");
            char rv = 0;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char -1 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == -1, "char -1 roundtrip");
        }

    }

    // ===========================================================
    // writeUnsignedIntegerBit / readUnsignedIntegerBit
    // ===========================================================
    static void testBUE2_UnsignedIntBit_UInt()
    {
        // 零
        {
            char buf[16];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            uint rv = 1;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 16, bitIdx, (uint)0), "write uint 0 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 16, bitIdx, rv), "read uint 0 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 0u, "uint 0 roundtrip");
        }
        // 小值
        {
            char buf[16];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            uint rv = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 16, bitIdx, (uint)7), "write uint 7 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 16, bitIdx, rv), "read uint 7 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 7u, "uint 7 roundtrip");
        }
        // 大值
        {
            char buf[16];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            uint rv = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 16, bitIdx, (uint)4000000000u), "write uint 4e9 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 16, bitIdx, rv), "read uint 4e9 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 4000000000u, "uint 4e9 roundtrip");
        }
        // 255
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            uint rv = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 8, bitIdx, (uint)255), "write uint 255 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 8, bitIdx, rv), "read uint 255 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 255u, "uint 255 roundtrip");
        }

    }

    static void testBUE2_UnsignedIntBit_UShort()
    {
        // 零
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            ushort rv = 1;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 8, bitIdx, (ushort)0), "write ushort 0 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 8, bitIdx, rv), "read ushort 0 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 0u, "ushort 0 roundtrip");
        }
        // 最大值
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            ushort rv = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 8, bitIdx, (ushort)65535), "write ushort 65535 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 8, bitIdx, rv), "read ushort 65535 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 65535u, "ushort 65535 roundtrip");
        }
        // 中间值
        {
            char buf[8];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            ushort rv = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 8, bitIdx, (ushort)1000), "write ushort 1000 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 8, bitIdx, rv), "read ushort 1000 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 1000u, "ushort 1000 roundtrip");
        }

    }

    static void testBUE2_UnsignedIntBit_Byte()
    {
        // 零
        {
            char buf[4];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            byte rv = 1;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 4, bitIdx, (byte)0), "write byte 0 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 4, bitIdx, rv), "read byte 0 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 0u, "byte 0 roundtrip");
        }
        // 255（最大）
        {
            char buf[4];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            byte rv = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 4, bitIdx, (byte)255), "write byte 255 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 4, bitIdx, rv), "read byte 255 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 255u, "byte 255 roundtrip");
        }
        // 1
        {
            char buf[4];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            byte rv = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 4, bitIdx, (byte)1), "write byte 1 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 4, bitIdx, rv), "read byte 1 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 1u, "byte 1 roundtrip");
        }
        // 128
        {
            char buf[4];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            byte rv = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 4, bitIdx, (byte)128), "write byte 128 ok");
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 4, bitIdx, rv), "read byte 128 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == 128u, "byte 128 roundtrip");
        }

    }

    // ===========================================================
    // writeSignedIntegerListBit / readSignedIntegerListBit（Vector<int>）
    // ===========================================================
    static void testBUE2_SignedIntListBit()
    {
        // 场景1：多个正数
        {
            char buf[128];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<int> src;
            src.add(100);
            src.add(200);
            src.add(300);
            src.add(400);
            src.add(500);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 128, bitIdx, src, true), "writeSignedIntListBit positive ok");
            Vector<int> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 128, bitIdx, dst, true), "readSignedIntListBit positive ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 5, "signed int list size=5");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[0] == 100 && dst[4] == 500, "signed int list values");
        }
        // 场景2：混合正负数
        {
            char buf[128];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<int> src;
            src.add(-100);
            src.add(200);
            src.add(-300);
            src.add(400);
            src.add(-500);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 128, bitIdx, src, true), "writeSignedIntListBit mixed ok");
            Vector<int> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 128, bitIdx, dst, true), "readSignedIntListBit mixed ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 5, "signed int list mixed size=5");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[0] == -100, "signed int list [0]=-100");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[1] == 200, "signed int list [1]=200");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[2] == -300, "signed int list [2]=-300");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[3] == 400, "signed int list [3]=400");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[4] == -500, "signed int list [4]=-500");
        }
        // 场景3：空列表
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<int> src;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 32, bitIdx, src, true), "writeSignedIntListBit empty ok");
            Vector<int> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 32, bitIdx, dst, true), "readSignedIntListBit empty ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 0, "signed int list empty size=0");
        }
        // 场景4：全零
        {
            char buf[64];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<int> src;
            src.add(0);
            src.add(0);
            src.add(0);
            src.add(0);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 64, bitIdx, src, true), "writeSignedIntListBit zeros ok");
            Vector<int> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 64, bitIdx, dst, true), "readSignedIntListBit zeros ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 4, "signed int list zero size=4");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[0] == 0 && dst[3] == 0, "signed int list zero values");
        }
        // 场景5：单元素
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<int> src;
            src.add(-7777);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 32, bitIdx, src, true), "writeSignedIntListBit single ok");
            Vector<int> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 32, bitIdx, dst, true), "readSignedIntListBit single ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 1, "signed int list single size=1");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[0] == -7777, "signed int list single value");
        }
        // 场景6：较大数量（20个）
        {
            char buf[512];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<int> src;
            for (int i = 0; i < 20; i++)
            {
                src.add(i % 2 == 0 ? i * 1000 : -(i * 1000));
            }
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 512, bitIdx, src, true), "writeSignedIntListBit 20 ok");
            Vector<int> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 512, bitIdx, dst, true), "readSignedIntListBit 20 ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 20, "signed int list 20 size");
            for (int i = 0; i < 20; i++)
            {
                BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[i] == src[i], "signed int list 20 value match");
            }
        }

    }

    // ===========================================================
    // writeUnsignedIntegerListBit / readUnsignedIntegerListBit（Vector<ushort>）
    // ===========================================================
    static void testBUE2_UnsignedShortListBit()
    {
        // 场景1：多个值
        {
            char buf[128];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<ushort> src;
            src.add(10);
            src.add(20);
            src.add(30);
            src.add(40);
            src.add(50);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerListBit(buf, 128, bitIdx, src), "writeUnsignedShortListBit ok");
            Vector<ushort> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerListBit(buf, 128, bitIdx, dst), "readUnsignedShortListBit ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 5, "ushort list size=5");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[0] == 10 && dst[4] == 50, "ushort list values");
        }
        // 场景2：空列表
        {
            char buf[16];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<ushort> src;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerListBit(buf, 16, bitIdx, src), "writeUnsignedShortListBit empty ok");
            Vector<ushort> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerListBit(buf, 16, bitIdx, dst), "readUnsignedShortListBit empty ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 0, "ushort list empty");
        }
        // 场景3：含零
        {
            char buf[64];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<ushort> src;
            src.add(0);
            src.add(100);
            src.add(0);
            src.add(200);
            src.add(0);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerListBit(buf, 64, bitIdx, src), "writeUnsignedShortListBit withZero ok");
            Vector<ushort> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerListBit(buf, 64, bitIdx, dst), "readUnsignedShortListBit withZero ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 5, "ushort list with-zero size=5");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[0] == 0 && dst[1] == 100 && dst[3] == 200, "ushort list with-zero values");
        }
        // 场景4：最大值
        {
            char buf[64];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<ushort> src;
            src.add(65535);
            src.add(65535);
            src.add(65535);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerListBit(buf, 64, bitIdx, src), "writeUnsignedShortListBit max ok");
            Vector<ushort> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerListBit(buf, 64, bitIdx, dst), "readUnsignedShortListBit max ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 3, "ushort list max size=3");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst[0] == 65535 && dst[2] == 65535, "ushort list max values");
        }

    }

    // ===========================================================
    // writeFloatListBit / readFloatListBit（Vector<float>）
    // ===========================================================
    static void testBUE2_FloatListBit()
    {
        // 场景1：正数列表
        {
            char buf[256];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<float> src;
            src.add(1.5f);
            src.add(2.5f);
            src.add(3.5f);
            src.add(4.5f);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeFloatListBit(buf, 256, bitIdx, src, true, 3), "writeFloatListBit positive ok");
            Vector<float> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readFloatListBit(buf, 256, bitIdx, dst, true, 3), "readFloatListBit positive ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 4, "float list size=4");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(dst[0], 1.5f), "float list [0]=1.5");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(dst[1], 2.5f), "float list [1]=2.5");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(dst[2], 3.5f), "float list [2]=3.5");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(dst[3], 4.5f), "float list [3]=4.5");
        }
        // 场景2：含负数
        {
            char buf[256];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<float> src;
            src.add(-1.0f);
            src.add(2.0f);
            src.add(-3.0f);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeFloatListBit(buf, 256, bitIdx, src, true, 3), "writeFloatListBit neg ok");
            Vector<float> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readFloatListBit(buf, 256, bitIdx, dst, true, 3), "readFloatListBit neg ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 3, "float neg list size=3");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(dst[0], -1.0f), "float neg [0]");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(dst[1], 2.0f), "float neg [1]");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(dst[2], -3.0f), "float neg [2]");
        }
        // 场景3：空列表
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<float> src;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeFloatListBit(buf, 32, bitIdx, src, true, 3), "writeFloatListBit empty ok");
            Vector<float> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readFloatListBit(buf, 32, bitIdx, dst, true, 3), "readFloatListBit empty ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 0, "float list empty size=0");
        }

    }

    // ===========================================================
    // readDoubleBit / writeDoubleListBit（Vector<double>）
    // ===========================================================
    static void testBUE2_DoubleListBit()
    {
        // 场景1：正数列表
        {
            char buf[512];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<double> src;
            src.add(1.23456);
            src.add(2.34567);
            src.add(3.45678);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeDoubleListBit(buf, 512, bitIdx, src, true, 4), "writeDoubleListBit ok");
            Vector<double> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readDoubleListBit(buf, 512, bitIdx, dst, true, 4), "readDoubleListBit ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 3, "double list size=3");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_DOUBLE_EQ(dst[0], 1.23456), "double list [0]");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_DOUBLE_EQ(dst[1], 2.34567), "double list [1]");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_DOUBLE_EQ(dst[2], 3.45678), "double list [2]");
        }
        // 场景2：空列表
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            int bitIdx = 0;
            Vector<double> src;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeDoubleListBit(buf, 32, bitIdx, src, true, 4), "writeDoubleListBit empty ok");
            Vector<double> dst;
            bitIdx = 0;
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readDoubleListBit(buf, 32, bitIdx, dst, true, 4), "readDoubleListBit empty ok");
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(dst.size() == 0, "double list empty size=0");
        }

    }

    // ===========================================================
    // writeVector2 / readVector2 / readVector2Inverse / writeVector2Inverse
    // ===========================================================
    static void testBUE2_Vector2ReadWrite()
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        Vector2 v = { 1.5f, -2.5f };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeVector2(buf, 64, idx, v), "writeVector2 ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 8, "writeVector2 advances 8 bytes");

        Vector2 rv = { 0.0f, 0.0f };
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readVector2(buf, 64, idx, rv), "readVector2 ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rv.x, 1.5f), "readVector2 x");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rv.y, -2.5f), "readVector2 y");

        // Inverse
        memset(buf, 0, sizeof(buf));
        idx = 0;
        Vector2 vi = { 3.14f, 2.71f };
        // writeInverse for vector2 via writeVector2Inverse not defined, use manual
        BinaryUtility::writeInverse(buf, 64, idx, vi.x);
        BinaryUtility::writeInverse(buf, 64, idx, vi.y);
        Vector2 rvi = { 0.0f, 0.0f };
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readVector2Inverse(buf, 64, idx, rvi), "readVector2Inverse ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rvi.x, vi.x), "readVector2Inverse x");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rvi.y, vi.y), "readVector2Inverse y");

    }

    static void testBUE2_Vector2IntReadWrite()
    {
        char buf[32];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        Vector2Int v = { 100, -200 };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeVector2Int(buf, 32, idx, v), "writeVector2Int ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 8, "writeVector2Int advances 8 bytes");

        Vector2Int rv = { 0, 0 };
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readVector2Int(buf, 32, idx, rv), "readVector2Int ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv.x == 100, "readVector2Int x");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv.y == -200, "readVector2Int y");

        // InverseWrite/Read
        memset(buf, 0, sizeof(buf));
        idx = 0;
        Vector2Int vi = { 300, -400 };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeVector2IntInverse(buf, 32, idx, vi), "writeVector2IntInverse ok");
        Vector2Int rvi = { 0, 0 };
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readVector2IntInverse(buf, 32, idx, rvi), "readVector2IntInverse ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rvi.x == 300, "readVector2IntInverse x");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rvi.y == -400, "readVector2IntInverse y");

    }

    // ===========================================================
    // writeVector3 / readVector3 / readVector3Inverse / writeVector3Inverse
    // ===========================================================
    static void testBUE2_Vector3ReadWrite()
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        Vector3 v = { 1.0f, 2.0f, 3.0f };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeVector3(buf, 64, idx, v), "writeVector3 ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 12, "writeVector3 advances 12 bytes");

        Vector3 rv = { 0.0f, 0.0f, 0.0f };
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readVector3(buf, 64, idx, rv), "readVector3 ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rv.x, 1.0f), "readVector3 x");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rv.y, 2.0f), "readVector3 y");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rv.z, 3.0f), "readVector3 z");

        // Inverse
        memset(buf, 0, sizeof(buf));
        idx = 0;
        Vector3 vi = { -1.0f, -2.0f, -3.0f };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeVector3Inverse(buf, 64, idx, vi), "writeVector3Inverse ok");
        Vector3 rvi = { 0.0f, 0.0f, 0.0f };
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readVector3Inverse(buf, 64, idx, rvi), "readVector3Inverse ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rvi.x, -1.0f), "readVector3Inverse x");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rvi.y, -2.0f), "readVector3Inverse y");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rvi.z, -3.0f), "readVector3Inverse z");

    }

    static void testBUE2_Vector3IntReadWrite()
    {
        char buf[32];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        Vector3Int vi3 = { 10, 20, 30 };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 32, idx, vi3.x), "writeVector3Int.x ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 32, idx, vi3.y), "writeVector3Int.y ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::write(buf, 32, idx, vi3.z), "writeVector3Int.z ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 12, "write Vector3Int 12 bytes");

        Vector3Int rv = { 0, 0, 0 };
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readVector3Int(buf, 32, idx, rv), "readVector3Int ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv.x == 10, "readVector3Int x");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv.y == 20, "readVector3Int y");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv.z == 30, "readVector3Int z");

    }

    // ===========================================================
    // writeVector4 / readVector4 / Inverse
    // ===========================================================
    static void testBUE2_Vector4ReadWrite()
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        Vector4 v = { 1.0f, 2.0f, 3.0f, 4.0f };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeVector4(buf, 64, idx, v), "writeVector4 ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 16, "writeVector4 advances 16 bytes");

        Vector4 rv = { 0.0f, 0.0f, 0.0f, 0.0f };
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readVector4(buf, 64, idx, rv), "readVector4 ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rv.x, 1.0f), "readVector4 x");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rv.y, 2.0f), "readVector4 y");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rv.z, 3.0f), "readVector4 z");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rv.w, 4.0f), "readVector4 w");

        // Inverse
        memset(buf, 0, sizeof(buf));
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeVector4Inverse(buf, 64, idx, v), "writeVector4Inverse ok");
        Vector4 rvi = { 0.0f, 0.0f, 0.0f, 0.0f };
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readVector4Inverse(buf, 64, idx, rvi), "readVector4Inverse ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rvi.x, 1.0f), "readVector4Inverse x");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rvi.w, 4.0f), "readVector4Inverse w");

    }

    // ===========================================================
    // writeColor / readColor / readColorInverse / writeColorInverse
    // Color 成员均为 byte (uint8)，write/read 以 byte 为单位
    // ===========================================================
    static void testBUE2_ColorReadWrite()
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        Color c(25, 127, 229, 255);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeColor(buf, 64, idx, c), "writeColor ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == 4, "writeColor advances 4 bytes (4 bytes)");

        Color rc;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readColor(buf, 64, idx, rc), "readColor ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rc.r == 25, "readColor r==25");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rc.g == 127, "readColor g==127");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rc.b == 229, "readColor b==229");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rc.a == 255, "readColor a==255");

        // Inverse（字节序反转后读出应相等）
        memset(buf, 0, sizeof(buf));
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::writeColorInverse(buf, 64, idx, c), "writeColorInverse ok");
        Color rci;
        idx = 0;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::readColorInverse(buf, 64, idx, rci), "readColorInverse ok");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rci.r == c.r, "readColorInverse r match");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rci.g == c.g, "readColorInverse g match");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rci.b == c.b, "readColorInverse b match");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rci.a == c.a, "readColorInverse a match");

    }

    static void testBUE2_ColorEdge()
    {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        int idx = 0;
        // 边界颜色：全黑
        Color black(0, 0, 0, 0);
        BinaryUtility::writeColor(buf, 64, idx, black);
        Color rb(255, 255, 255, 255);
        idx = 0;
        BinaryUtility::readColor(buf, 64, idx, rb);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb.r == 0, "black r==0");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb.a == 0, "black a==0");

        // 全白
        memset(buf, 0, sizeof(buf));
        idx = 0;
        Color white(255, 255, 255, 255);
        BinaryUtility::writeColor(buf, 64, idx, white);
        Color rw(0, 0, 0, 0);
        idx = 0;
        BinaryUtility::readColor(buf, 64, idx, rw);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rw.r == 255, "white r==255");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rw.a == 255, "white a==255");

    }

    // ===========================================================
    // crc16 / crc16Byte
    // ===========================================================
    static void testBUE2_CRC16()
    {
        // 场景1：空数据
        ushort crc0 = BinaryUtility::crc16(0, nullptr, 0);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(crc0 == 0, "crc16 empty = 0");

        // 场景2：已知数据验证
        const char data1[] = { 0x31, 0x32, 0x33, 0x34, 0x35 };
        ushort crc1a = BinaryUtility::crc16(0, data1, 5);
        ushort crc1b = BinaryUtility::crc16(0, data1, 5);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(crc1a == crc1b, "crc16 deterministic");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(crc1a != 0, "crc16 non-empty != 0");

        // 场景3：不同数据产生不同CRC
        const char data2[] = { 0x41, 0x42, 0x43, 0x44, 0x45 };
        ushort crc2 = BinaryUtility::crc16(0, data2, 5);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(crc1a != crc2, "crc16 different data different result");

        // 场景4：crc16Byte 逐字节计算
        ushort crcByByte = 0;
        for (int i = 0; i < 5; i++)
        {
            crcByByte = BinaryUtility::crc16Byte(crcByByte, (byte)data1[i]);
        }
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(crcByByte == crc1a, "crc16Byte same as crc16");

        // 场景5：单字节
        const char single[1] = { 0x01 };
        ushort crcSingle = BinaryUtility::crc16(0, single, 1);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(crcSingle != 0, "crc16 single byte != 0");

        // 场景6：bufferOffset
        const char padded[] = { 0x00, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35 };
        ushort crcOffset = BinaryUtility::crc16(0, padded, 5, 2);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(crcOffset == crc1a, "crc16 with offset same as without");

        // 场景7：累积CRC
        ushort crcPart1 = BinaryUtility::crc16(0, data1, 2);
        ushort crcFull = BinaryUtility::crc16(crcPart1, data1, 3, 2);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(crcFull == crc1a, "crc16 incremental same as full");

    }

    // ===========================================================
    // generateBitCount（byte/ushort/uint/ullong）
    // ===========================================================
    static void testBUE2_GenerateBitCount()
    {
        // byte
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)0) == 0, "generateBitCount byte 0 = 0");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)1) == 1, "generateBitCount byte 1 = 1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)2) == 2, "generateBitCount byte 2 = 2");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)3) == 2, "generateBitCount byte 3 = 2");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)4) == 3, "generateBitCount byte 4 = 3");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)7) == 3, "generateBitCount byte 7 = 3");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)8) == 4, "generateBitCount byte 8 = 4");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)127) == 7, "generateBitCount byte 127 = 7");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)128) == 8, "generateBitCount byte 128 = 8");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)255) == 8, "generateBitCount byte 255 = 8");

        // ushort
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)0) == 0, "generateBitCount ushort 0 = 0");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)1) == 1, "generateBitCount ushort 1 = 1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)255) == 8, "generateBitCount ushort 255 = 8");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)256) == 9, "generateBitCount ushort 256 = 9");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)1000) == 10, "generateBitCount ushort 1000 = 10");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)65535) == 16, "generateBitCount ushort 65535 = 16");

        // uint
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)0) == 0, "generateBitCount uint 0 = 0");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)1) == 1, "generateBitCount uint 1 = 1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)65535) == 16, "generateBitCount uint 65535 = 16");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)65536) == 17, "generateBitCount uint 65536 = 17");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)0xFFFFFFFFu) == 32, "generateBitCount uint max = 32");

        // ullong
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)0) == 0, "generateBitCount ullong 0 = 0");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)1) == 1, "generateBitCount ullong 1 = 1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)0xFFFFFFFFull) == 32, "generateBitCount ullong 32bit max = 32");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)0x100000000ull) == 33, "generateBitCount ullong 2^32 = 33");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)0xFFFFFFFFFFFFFFFFull) == 64, "generateBitCount ullong max = 64");

    }

    // ===========================================================
    // getBits / bitCountToByteCount
    // ===========================================================
    static void testBUE2_GetBits()
    {
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits(0b11111111, 0, 4) == 0b00001111, "getBits [0,4) of 0xFF");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits(0b11111111, 4, 4) == 0b00001111, "getBits [4,4) of 0xFF");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits(0b10101010, 1, 4) == 0b00000101, "getBits [1,4) of 0xAA");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits(0b00000001, 0, 1) == 1, "getBits single bit=1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits(0b00000000, 0, 8) == 0, "getBits all zero");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits(0b11110000, 4, 4) == 0b00001111, "getBits upper nibble");

    }

    static void testBUE2_BitCountToByteCount()
    {
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(0) == 0, "bitCountToByteCount 0 = 0");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(1) == 1, "bitCountToByteCount 1 = 1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(8) == 1, "bitCountToByteCount 8 = 1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(9) == 2, "bitCountToByteCount 9 = 2");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(16) == 2, "bitCountToByteCount 16 = 2");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(17) == 3, "bitCountToByteCount 17 = 3");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(24) == 3, "bitCountToByteCount 24 = 3");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(25) == 4, "bitCountToByteCount 25 = 4");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(32) == 4, "bitCountToByteCount 32 = 4");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(64) == 8, "bitCountToByteCount 64 = 8");

    }

    // ===========================================================
    // setBufferBitOne / getBufferBit
    // ===========================================================
    static void testBUE2_BufferBit()
    {
        char buf[8];
        memset(buf, 0, sizeof(buf));

        // 设置第0位
        BinaryUtility::setBufferBitOne(buf, 0);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 0) == true, "bit[0] = 1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 1) == false, "bit[1] = 0");

        // 设置第7位
        BinaryUtility::setBufferBitOne(buf, 7);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 7) == true, "bit[7] = 1");

        // 设置第8位（第二字节的第0位）
        BinaryUtility::setBufferBitOne(buf, 8);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 8) == true, "bit[8] = 1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 9) == false, "bit[9] = 0");

        // 设置第15位
        BinaryUtility::setBufferBitOne(buf, 15);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 15) == true, "bit[15] = 1");

        // 检查第1~6位仍然为0
        for (int i = 1; i < 7; i++)
        {
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, i) == false, "bit[1~6] = 0");
        }

        // 验证连续设置
        memset(buf, 0, sizeof(buf));
        for (int i = 0; i < 32; i += 2)
        {
            BinaryUtility::setBufferBitOne(buf, i);
        }
        for (int i = 0; i < 32; i++)
        {
            bool expected = (i % 2 == 0);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, i) == expected, "alternating bits");
        }

    }

    // ===========================================================
    // copyBits
    // ===========================================================
    static void testBUE2_CopyBits()
    {
        // 基本测试：将source的低3位复制到dest的高3位（从位5开始）
        byte dest = 0;
        byte src = 0b00000101;
        BinaryUtility::copyBits(dest, (byte)5, src, (byte)0, (byte)3);
        // 期望：dest的[5..7]被设置为101
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits(dest, 5, 3) == 0b101, "copyBits dest[5..7]=101");

        // 复制到位0
        byte dest2 = 0;
        BinaryUtility::copyBits(dest2, (byte)0, (byte)0b11001010, (byte)2, (byte)4);
        // 从src的位2开始取4位 = 0b0010
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits(dest2, 0, 4) == 0b0010, "copyBits to pos 0");

        // 覆盖测试：从1开始复制2位
        // copyBits 是 or 操作，先用0初始化再设置
        byte dest3clean = 0;
        BinaryUtility::copyBits(dest3clean, (byte)1, (byte)0b10101010, (byte)1, (byte)2);
        // 从src位1取2位 = 0b01
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits(dest3clean, 1, 2) == 0b01, "copyBits 2 bits from pos 1");

        // char 版本
        char cdest = 0;
        BinaryUtility::copyBits(cdest, (byte)0, (byte)0b11110000, (byte)4, (byte)4);
        // 从src位4取4位 = 0b1111
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getBits((byte)cdest, 0, 4) == 0b1111, "copyBits char version");
    }

    // ===========================================================
    // getListMax / getListMaxAbs / getFloatListMaxAbs / getDoubleListMaxAbs
    // ===========================================================
    static void testBUE2_GetListMax()
    {
        // getListMax（无符号整数）
        Vector<uint> ulist;
        ulist.add(10u);
        ulist.add(50u);
        ulist.add(30u);
        ulist.add(20u);
        ulist.add(40u);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getListMax(ulist) == 50u, "getListMax uint vector");

        uint uArr[5] = { 5u, 15u, 3u, 100u, 7u };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getListMax(uArr, 5) == 100u, "getListMax uint array");

        // 单元素
        Vector<ushort> single;
        single.add((ushort)42);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getListMax(single) == 42u, "getListMax single element");

        // 空列表返回0
        Vector<uint> empty;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getListMax(empty) == 0u, "getListMax empty = 0");
    }

    static void testBUE2_GetListMaxAbs()
    {
        // getListMaxAbs（有符号整数）
        Vector<int> ilist;
        ilist.add(-100);
        ilist.add(50);
        ilist.add(-30);
        ilist.add(200);
        ilist.add(-10);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getListMaxAbs(ilist) == 200, "getListMaxAbs int vector = 200");

        int iArr[4] = { -500, 100, -300, 200 };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getListMaxAbs(iArr, 4) == 500, "getListMaxAbs int array = 500");

        // 全正
        Vector<int> allPos;
        allPos.add(1);
        allPos.add(2);
        allPos.add(3);
        allPos.add(4);
        allPos.add(5);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getListMaxAbs(allPos) == 5, "getListMaxAbs all positive");

        // 全负
        Vector<int> allNeg;
        allNeg.add(-1);
        allNeg.add(-2);
        allNeg.add(-3);
        allNeg.add(-4);
        allNeg.add(-5);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getListMaxAbs(allNeg) == 5, "getListMaxAbs all negative");

        // 空列表
        Vector<int> empty;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getListMaxAbs(empty) == 0, "getListMaxAbs empty = 0");
    }

    static void testBUE2_GetFloatDoubleListMaxAbs()
    {
        // getFloatListMaxAbs
        float farr[5] = { 1.5f, -3.5f, 2.5f, -5.5f, 4.5f };
        float fmax = BinaryUtility::getFloatListMaxAbs(farr, 5);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(fmax, 5.5f), "getFloatListMaxAbs = 5.5");

        float farr2[3] = { 0.0f, 0.0f, 0.0f };
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(BinaryUtility::getFloatListMaxAbs(farr2, 3), 0.0f), "getFloatListMaxAbs zero");

        // getDoubleListMaxAbs
        double darr[4] = { 1.1, -2.2, 3.3, -4.4 };
        double dmax = BinaryUtility::getDoubleListMaxAbs(darr, 4);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_DOUBLE_EQ(dmax, 4.4), "getDoubleListMaxAbs = 4.4");
    }

    // ===========================================================
    // inverseByte / swapByte
    // ===========================================================
    static void testBUE2_InverseSwapByte()
    {
        // inverseByte — int
        int v = 0x01020304;
        BinaryUtility::inverseByte(v);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(v == 0x04030201, "inverseByte int");

        // inverseByte — short
        short sv = (short)0x0102;
        BinaryUtility::inverseByte(sv);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((ushort)sv == (ushort)0x0201, "inverseByte short");

        // inverseByte — llong
        llong lv = 0x0102030405060708LL;
        BinaryUtility::inverseByte(lv);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(lv == (llong)0x0807060504030201LL, "inverseByte llong");

        // swapByte
        // 0x01020304 小端内存布局: byte[0]=0x04, byte[1]=0x03, byte[2]=0x02, byte[3]=0x01
        // swapByte(0, 3) 后: byte[0]=0x01, byte[3]=0x04, byte[1]/byte[2] 不变(分别是 0x03/0x02)
        int sv2 = 0x01020304;
        BinaryUtility::swapByte(sv2, 0, 3);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getByte(sv2, 0) == 0x01, "swapByte: [0] should be 0x01");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getByte(sv2, 3) == 0x04, "swapByte: [3] should be 0x04");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getByte(sv2, 1) == 0x03, "swapByte: [1] unchanged");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtility::getByte(sv2, 2) == 0x02, "swapByte: [2] unchanged");
    }

    // ===========================================================
    // zeroArray
    // ===========================================================
    static void testBUE2_ZeroArray()
    {
        int arr[10];
        for (int i = 0; i < 10; i++)
        {
            arr[i] = i + 1;
        }
        BinaryUtility::zeroArray(arr, 10);
        for (int i = 0; i < 10; i++)
        {
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(arr[i] == 0, "zeroArray int[i] = 0");
        }

        float farr[5] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
        BinaryUtility::zeroArray(farr, 5);
        for (int i = 0; i < 5; i++)
        {
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(farr[i], 0.0f), "zeroArray float[i] = 0");
        }

        llong larr[4] = { 100LL, 200LL, 300LL, 400LL };
        BinaryUtility::zeroArray(larr, 4);
        for (int i = 0; i < 4; i++)
        {
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(larr[i] == 0LL, "zeroArray llong[i] = 0");
        }
    }

    // ===========================================================
    // fillZeroToByteEnd
    // ===========================================================
    static void testBUE2_FillZeroToByteEnd()
    {
        char buf[16];
        memset(buf, (char)0xFF, sizeof(buf));

        // bitIdx = 5 → 当前字节的[5..7]应被清零
        int bitIdx = 5;
        BinaryUtility::fillZeroToByteEnd(buf, bitIdx);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(bitIdx == 8, "fillZeroToByteEnd bitIdx advances to 8");
        // 验证buf[0]的高3位已清零
        byte firstByte = (byte)buf[0];
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((firstByte & 0b11100000) == 0, "fillZeroToByteEnd clears high bits");
        // 低5位不变（仍为0xFF的低5位 = 0b11111）
        BinaryUtilityExt2TestImpl_BUE2_ASSERT((firstByte & 0b00011111) == 0b00011111, "fillZeroToByteEnd keeps low bits");

        // bitIdx已对齐时不做任何事
        memset(buf, (char)0xFF, sizeof(buf));
        bitIdx = 8;
        BinaryUtility::fillZeroToByteEnd(buf, bitIdx);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(bitIdx == 8, "fillZeroToByteEnd aligned bitIdx unchanged");

        // bitIdx = 0时
        memset(buf, (char)0xFF, sizeof(buf));
        bitIdx = 0;
        BinaryUtility::fillZeroToByteEnd(buf, bitIdx);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(bitIdx == 0, "fillZeroToByteEnd bitIdx=0 unchanged");
    }

    // ===========================================================
    // 混合场景：write多种类型然后全部读回
    // ===========================================================
    static void testBUE2_MixedWriteRead()
    {
        char buf[256];
        memset(buf, 0, sizeof(buf));
        int idx = 0;

        // 写入各种类型
        BinaryUtility::write(buf, 256, idx, (int)1000);
        BinaryUtility::write(buf, 256, idx, (short)-500);
        BinaryUtility::write(buf, 256, idx, (float)3.14f);
        BinaryUtility::write(buf, 256, idx, (double)2.718281828);
        BinaryUtility::write(buf, 256, idx, (byte)0xAB);
        BinaryUtility::write(buf, 256, idx, (llong)-9876543210LL);
        BinaryUtility::write(buf, 256, idx, (bool)true);
        BinaryUtility::write(buf, 256, idx, (uint)3000000000u);
        BinaryUtility::write(buf, 256, idx, (ushort)50000u);

        int expectedSize = 4 + 2 + 4 + 8 + 1 + 8 + 1 + 4 + 2;
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == expectedSize, "mixed write total size");

        // 全部读回
        idx = 0;
        int ri = 0;
        short rs = 0;
        float rf = 0.0f;
        double rd = 0.0;
        byte rb = 0;
        llong rl = 0LL;
        bool rbool = false;
        uint rui = 0;
        ushort rus = 0;

        BinaryUtility::read(buf, 256, idx, ri);
        BinaryUtility::read(buf, 256, idx, rs);
        BinaryUtility::read(buf, 256, idx, rf);
        BinaryUtility::read(buf, 256, idx, rd);
        BinaryUtility::read(buf, 256, idx, rb);
        BinaryUtility::read(buf, 256, idx, rl);
        BinaryUtility::read(buf, 256, idx, rbool);
        BinaryUtility::read(buf, 256, idx, rui);
        BinaryUtility::read(buf, 256, idx, rus);

        BinaryUtilityExt2TestImpl_BUE2_ASSERT(ri == 1000, "mixed: int");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rs == -500, "mixed: short");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_FLOAT_EQ(rf, 3.14f), "mixed: float");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(BinaryUtilityExt2TestImpl_BUE2_DOUBLE_EQ(rd, 2.718281828), "mixed: double");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb == 0xAB, "mixed: byte");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rl == -9876543210LL, "mixed: llong");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rbool == true, "mixed: bool");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rui == 3000000000u, "mixed: uint");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rus == 50000u, "mixed: ushort");
    }

    // ===========================================================
    // 混合bit级读写场景
    // ===========================================================
    static void testBUE2_MixedBitReadWrite()
    {
        char buf[256];
        memset(buf, 0, sizeof(buf));
        int bitIdx = 0;

        // 写入多种bit类型
        BinaryUtility::writeBoolBit(buf, 256, bitIdx, true);
        BinaryUtility::writeSignedIntegerBit(buf, 256, bitIdx, (int)12345, true);
        BinaryUtility::writeBoolBit(buf, 256, bitIdx, false);
        BinaryUtility::writeUnsignedIntegerBit(buf, 256, bitIdx, (ushort)9999);
        BinaryUtility::writeSignedIntegerBit(buf, 256, bitIdx, (char)-50, true);
        BinaryUtility::writeBoolBit(buf, 256, bitIdx, true);
        BinaryUtility::writeUnsignedIntegerBit(buf, 256, bitIdx, (byte)200);

        // 读回
        bool rb1 = false;
        int ri = 0;
        bool rb2 = true;
        ushort rus = 0;
        char rc = 0;
        bool rb3 = false;
        byte rby = 0;

        bitIdx = 0;
        BinaryUtility::readBoolBit(buf, 256, bitIdx, rb1);
        BinaryUtility::readSignedIntegerBit(buf, 256, bitIdx, ri, true);
        BinaryUtility::readBoolBit(buf, 256, bitIdx, rb2);
        BinaryUtility::readUnsignedIntegerBit(buf, 256, bitIdx, rus);
        BinaryUtility::readSignedIntegerBit(buf, 256, bitIdx, rc, true);
        BinaryUtility::readBoolBit(buf, 256, bitIdx, rb3);
        BinaryUtility::readUnsignedIntegerBit(buf, 256, bitIdx, rby);

        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb1 == true, "mixed bit: bool1");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(ri == 12345, "mixed bit: int");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb2 == false, "mixed bit: bool2");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rus == 9999, "mixed bit: ushort");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rc == -50, "mixed bit: char");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rb3 == true, "mixed bit: bool3");
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rby == 200, "mixed bit: byte");
    }

    // ===========================================================
    // 连续多次读写不同起始偏移
    // ===========================================================
    static void testBUE2_OffsetReadWrite()
    {
        char buf[128];
        memset(buf, 0, sizeof(buf));
        int idx = 0;

        // 在偏移0写入int
        BinaryUtility::write(buf, 128, idx, (int)111);
        // 在偏移4写入float
        BinaryUtility::write(buf, 128, idx, (float)2.2f);
        // 在偏移8写入short
        BinaryUtility::write(buf, 128, idx, (short)-99);
        // 在偏移10写入llong
        BinaryUtility::write(buf, 128, idx, (llong)999999999LL);

        // 非顺序读取（跳过float直接读short）
        idx = 8;
        short rs = 0;
        BinaryUtility::read(buf, 128, idx, rs);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rs == -99, "offset read short at 8");

        // 读取第一个int
        idx = 0;
        int ri = 0;
        BinaryUtility::read(buf, 128, idx, ri);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(ri == 111, "offset read int at 0");

        // 读取llong
        idx = 10;
        llong rl = 0;
        BinaryUtility::read(buf, 128, idx, rl);
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(rl == 999999999LL, "offset read llong at 10");
    }

    // ===========================================================
    // 压力测试：大量读写
    // ===========================================================
    static void testBUE2_StressReadWrite()
    {
        const int count = 100;
        char buf[count * 4 + 16];
        memset(buf, 0, sizeof(buf));
        int idx = 0;

        // 写入100个int
        for (int i = 0; i < count; i++)
        {
            BinaryUtility::write(buf, (int)sizeof(buf), idx, (int)(i * 13 - 500));
        }
        BinaryUtilityExt2TestImpl_BUE2_ASSERT(idx == count * 4, "stress write 100 ints total bytes");

        // 读回
        idx = 0;
        for (int i = 0; i < count; i++)
        {
            int rv = 0;
            BinaryUtility::read(buf, (int)sizeof(buf), idx, rv);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == i * 13 - 500, "stress read int value");
        }
    }

    static void testBUE2_StressBitReadWrite()
    {
        // 压力测试：写入50个int的bit编码，再全部读回
        const int count = 50;
        char buf[512];
        memset(buf, 0, sizeof(buf));
        int bitIdx = 0;

        for (int i = 0; i < count; i++)
        {
            int val = (i % 2 == 0) ? i * 100 : -(i * 100);
            BinaryUtility::writeSignedIntegerBit(buf, 512, bitIdx, val, true);
        }

        bitIdx = 0;
        for (int i = 0; i < count; i++)
        {
            int rv = 0;
            BinaryUtility::readSignedIntegerBit(buf, 512, bitIdx, rv, true);
            int expected = (i % 2 == 0) ? i * 100 : -(i * 100);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == expected, "stress bit read int value");
        }
    }

    static void testBUE2_StressUnsignedBit()
    {
        // 写入30个ushort的bit编码
        const int count = 30;
        char buf[256];
        memset(buf, 0, sizeof(buf));
        int bitIdx = 0;

        for (int i = 0; i < count; i++)
        {
            BinaryUtility::writeUnsignedIntegerBit(buf, 256, bitIdx, (ushort)(i * 300));
        }

        bitIdx = 0;
        for (int i = 0; i < count; i++)
        {
            ushort rv = 0;
            BinaryUtility::readUnsignedIntegerBit(buf, 256, bitIdx, rv);
            BinaryUtilityExt2TestImpl_BUE2_ASSERT(rv == (ushort)(i * 300), "stress unsigned bit ushort value");
        }
    }

    // ===========================================================
    // test()
    // ===========================================================
    static void test()
    {
        // --- 基础类型读写 ---
        testBUE2_ReadWriteInt();
        testBUE2_ReadWriteShort();
        testBUE2_ReadWriteLlong();
        testBUE2_ReadWriteFloat();
        testBUE2_ReadWriteDouble();
        testBUE2_ReadWriteByte();
        testBUE2_ReadWriteChar();
        testBUE2_ReadWriteUInt();
        testBUE2_ReadWriteUShort();
        testBUE2_ReadWriteBool();

        // --- Inverse 读写 ---
        testBUE2_ReadWriteInverseInt();
        testBUE2_ReadWriteInverseFloat();
        testBUE2_ReadWriteInverseLlong();

        // --- 数组读写 ---
        testBUE2_ReadWriteArray();
        testBUE2_ReadWriteArrayInverse();

        // --- Buffer 读写 ---
        testBUE2_ReadWriteBuffer();
        testBUE2_ReadWriteBufferBoundary();
        testBUE2_ReadBufferBoundary();
        testBUE2_ReadWriteOverflow();
        testBUE2_MultipleReadWrite();

        // --- Bool Bit ---
        testBUE2_BoolBit();
        testBUE2_BoolBitMultiple();

        // --- Signed Integer Bit ---
        testBUE2_SignedIntBit_Int();
        testBUE2_SignedIntBit_Short();
        testBUE2_SignedIntBit_Char();

        // --- Unsigned Integer Bit ---
        testBUE2_UnsignedIntBit_UInt();
        testBUE2_UnsignedIntBit_UShort();
        testBUE2_UnsignedIntBit_Byte();

        // --- List Bit ---
        testBUE2_SignedIntListBit();
        testBUE2_UnsignedShortListBit();
        testBUE2_FloatListBit();
        testBUE2_DoubleListBit();

        // --- Vector/Color 读写 ---
        testBUE2_Vector2ReadWrite();
        testBUE2_Vector2IntReadWrite();
        testBUE2_Vector3ReadWrite();
        testBUE2_Vector3IntReadWrite();
        testBUE2_Vector4ReadWrite();
        testBUE2_ColorReadWrite();
        testBUE2_ColorEdge();

        // --- CRC ---
        testBUE2_CRC16();

        // --- generateBitCount / getBits / bitCountToByteCount ---
        testBUE2_GenerateBitCount();
        testBUE2_GetBits();
        testBUE2_BitCountToByteCount();

        // --- Buffer Bit ---
        testBUE2_BufferBit();

        // --- copyBits ---
        testBUE2_CopyBits();

        // --- getListMax ---
        testBUE2_GetListMax();
        testBUE2_GetListMaxAbs();
        testBUE2_GetFloatDoubleListMaxAbs();

        // --- inverseByte / swapByte ---
        testBUE2_InverseSwapByte();

        // --- zeroArray ---
        testBUE2_ZeroArray();

        // --- fillZeroToByteEnd ---
        testBUE2_FillZeroToByteEnd();

        // --- 综合场景 ---
        testBUE2_MixedWriteRead();
        testBUE2_MixedBitReadWrite();
        testBUE2_OffsetReadWrite();
        testBUE2_StressReadWrite();
        testBUE2_StressBitReadWrite();
        testBUE2_StressUnsignedBit();
    }
};

struct BinaryUtilityExt3TestImpl
{

#define BinaryUtilityExt3TestImpl_BUE3_ASSERT(condition) if (!(condition)) { ERROR("BinaryUtilityExt3Test failed: " #condition); }


    static void test()
    {
        testGetByteBasic();
        testGetByteAllPositions();
        testSetByteBasic();
        testSetByteAllPositions();
        testHasBitTrue();
        testHasBitFalse();
        testGetBitInt();
        testGetBitByte();
        testSetBitOne();
        testSetBitZero();
        testSetBitOneFunc();
        testSetBitZeroFunc();
        testGetHighestBitInt();
        testGetHighestBitByte();
        testSetHighestBit();
        testGetLowestBit();
        testSetLowestBit();
        testGetBits();
        testBitCountToByteCount();
        testGetListMaxUnsigned();
        testGetListMaxAbs();
        testGetListMax_ptr();
        testGenerateBitCount();
    }

    static void testGetByteBasic()
    {
        int v = 0x04030201;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 0) == 0x01);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 1) == 0x02);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 2) == 0x03);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 3) == 0x04);
    }

    static void testGetByteAllPositions()
    {
        llong v = 0x0807060504030201LL;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 0) == 0x01);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 7) == 0x08);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 3) == 0x04);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 4) == 0x05);
    }

    static void testSetByteBasic()
    {
        int v = 0;
        BinaryUtility::setByte(v, (byte)0xAB, 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 0) == 0xAB);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 1) == 0x00);
    }

    static void testSetByteAllPositions()
    {
        int v = 0;
        BinaryUtility::setByte(v, (byte)0x11, 0);
        BinaryUtility::setByte(v, (byte)0x22, 1);
        BinaryUtility::setByte(v, (byte)0x33, 2);
        BinaryUtility::setByte(v, (byte)0x44, 3);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 0) == 0x11);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 1) == 0x22);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 2) == 0x33);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getByte(v, 3) == 0x44);
    }

    static void testHasBitTrue()
    {
        int v = 0b1010;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::hasBit(v, 1));
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::hasBit(v, 3));
    }

    static void testHasBitFalse()
    {
        int v = 0b1010;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(!BinaryUtility::hasBit(v, 0));
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(!BinaryUtility::hasBit(v, 2));
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(!BinaryUtility::hasBit(v, 4));
    }

    static void testGetBitInt()
    {
        int v = 0b1010;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 0) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 1) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 2) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 3) == 1);

        int v2 = 0;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v2, 0) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v2, 31) == 0);

        int v3 = ~0;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v3, 0) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v3, 31) == 1);
    }

    static void testGetBitByte()
    {
        byte v = 0b11001010;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 0) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 1) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 6) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 7) == 1);
    }

    static void testSetBitOne()
    {
        int v = 0;
        BinaryUtility::setBit(v, 0, 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 0) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(v == 1);

        BinaryUtility::setBit(v, 3, 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 3) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(v == 9);
    }

    static void testSetBitZero()
    {
        int v = 0b1111;
        BinaryUtility::setBit(v, 1, 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 1) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(v == 0b1101);

        BinaryUtility::setBit(v, 0, 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBit(v, 0) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(v == 0b1100);
    }

    static void testSetBitOneFunc()
    {
        int v = 0;
        BinaryUtility::setBitOne(v, 5);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::hasBit(v, 5));
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(!BinaryUtility::hasBit(v, 4));
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(!BinaryUtility::hasBit(v, 6));

        BinaryUtility::setBitOne(v, 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::hasBit(v, 0));
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::hasBit(v, 5));
    }

    static void testSetBitZeroFunc()
    {
        int v = ~0;
        BinaryUtility::setBitZero(v, 4);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(!BinaryUtility::hasBit(v, 4));
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::hasBit(v, 3));
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::hasBit(v, 5));
    }

    static void testGetHighestBitInt()
    {
        int positive = 100;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getHighestBit(positive) == 0);

        int negative = -1;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getHighestBit(negative) == 1);

        int zero = 0;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getHighestBit(zero) == 0);
    }

    static void testGetHighestBitByte()
    {
        byte low = 0x7F;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getHighestBit(low) == 0);

        byte high = 0x80;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getHighestBit(high) == 1);

        byte full = 0xFF;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getHighestBit(full) == 1);
    }

    static void testSetHighestBit()
    {
        int v = 0;
        BinaryUtility::setHighestBit(v, true);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getHighestBit(v) == 1);

        BinaryUtility::setHighestBit(v, false);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getHighestBit(v) == 0);

        byte b = 0;
        BinaryUtility::setHighestBit(b, true);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getHighestBit(b) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(b == 0x80);
    }

    static void testGetLowestBit()
    {
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getLowestBit(0) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getLowestBit(1) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getLowestBit(2) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getLowestBit(3) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getLowestBit(0xFF) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getLowestBit(0xFE) == 0);
    }

    static void testSetLowestBit()
    {
        int v = 0;
        BinaryUtility::setLowestBit(v, true);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getLowestBit(v) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(v == 1);

        BinaryUtility::setLowestBit(v, false);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getLowestBit(v) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(v == 0);

        int v2 = 0xFE;
        BinaryUtility::setLowestBit(v2, true);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(v2 == 0xFF);
    }

    static void testGetBits()
    {
        byte v = 0b11011010;
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBits(v, 0, 3) == 0b010);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBits(v, 3, 3) == 0b011);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBits(v, 0, 1) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBits(v, 1, 1) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::getBits(v, 7, 1) == 1);
    }

    static void testBitCountToByteCount()
    {
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(0) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(1) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(7) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(8) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(9) == 2);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(16) == 2);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(17) == 3);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(24) == 3);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(32) == 4);
    }

    static void testGetListMaxUnsigned()
    {
        Vector<uint> vec;
        vec.add(5u);
        vec.add(3u);
        vec.add(8u);
        vec.add(1u);
        uint maxVal = BinaryUtility::getListMax(vec);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(maxVal == 8u);

        Vector<uint> empty;
        uint emptyMax = BinaryUtility::getListMax(empty);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(emptyMax == 0u);
    }

    static void testGetListMaxAbs()
    {
        Vector<int> vec;
        vec.add(5);
        vec.add(-10);
        vec.add(3);
        vec.add(-2);
        int maxAbs = BinaryUtility::getListMaxAbs(vec);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(maxAbs == 10);

        Vector<int> empty;
        int emptyMaxAbs = BinaryUtility::getListMaxAbs(empty);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(emptyMaxAbs == 0);
    }

    static void testGetListMax_ptr()
    {
        uint arr[] = { 7u, 2u, 9u, 4u };
        uint maxVal = BinaryUtility::getListMax(arr, 4);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(maxVal == 9u);

        int arr2[] = { -3, -7, -1, -5 };
        int maxAbs = BinaryUtility::getListMaxAbs(arr2, 4);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(maxAbs == 7);
    }

    static void testGenerateBitCount()
    {
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)0) == 0);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)1) == 1);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)2) == 2);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)3) == 2);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)4) == 3);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)7) == 3);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)8) == 4);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)127) == 7);
        BinaryUtilityExt3TestImpl_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)255) == 8);
    }
};

void BinaryUtilityTest::test()
{
    BinaryUtilityExtTestImpl::test();
    BinaryUtilityExt2TestImpl::test();
    BinaryUtilityExt3TestImpl::test();
}
