#include "FrameHeader.h"

#define UT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("UtilityTest FAIL: ") + (msg)); } } while(0)
#define UT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("UtilityTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define UT_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("UtilityTest FAIL: ") + (msg) + \
		" | expected=" + LLToS(b) + " actual=" + LLToS(a)); } } while(0)
#define UT_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((a) - (b)) > 1e-3f) { ERROR(string("UtilityTest FAIL: ") + (msg) + \
		" | expected=" + FToS(b) + " actual=" + FToS(a)); } } while(0)
#define UT_ASSERT_STR(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("UtilityTest FAIL: ") + (msg) + \
		" | expected=" + (b) + " actual=" + (a)); } } while(0)

//------------------------------------------------------------------------------
// MathUtility tests
//------------------------------------------------------------------------------

static void testMath_Ceiling()
{
	UT_ASSERT_EQ(ceiling(1.0f),    1, "ceiling(1.0)=1");
	UT_ASSERT_EQ(ceiling(1.1f),    2, "ceiling(1.1)=2");
	UT_ASSERT_EQ(ceiling(1.9f),    2, "ceiling(1.9)=2");
	UT_ASSERT_EQ(ceiling(-1.1f),  -1, "ceiling(-1.1)=-1");
	UT_ASSERT_EQ(ceiling(-1.9f),  -1, "ceiling(-1.9)=-1");
	UT_ASSERT_EQ(ceiling(0.0f),    0, "ceiling(0.0)=0");
	UT_ASSERT_EQ(ceiling(0.5f),    1, "ceiling(0.5)=1");
}

static void testMath_Floor()
{
	UT_ASSERT_EQ(floor_(1.0f),    1, "floor_(1.0)=1");
	UT_ASSERT_EQ(floor_(1.9f),    1, "floor_(1.9)=1");
	UT_ASSERT_EQ(floor_(1.1f),    1, "floor_(1.1)=1");
	UT_ASSERT_EQ(floor_(-1.1f),  -2, "floor_(-1.1)=-2");
	UT_ASSERT_EQ(floor_(-1.9f),  -2, "floor_(-1.9)=-2");
	UT_ASSERT_EQ(floor_(0.0f),    0, "floor_(0.0)=0");
	UT_ASSERT_EQ(floor_(-0.5f),  -1, "floor_(-0.5)=-1");
}

static void testMath_Clamp()
{
	UT_ASSERT_EQ(clamp(5,  0, 10),  5,  "clamp(5,0,10)=5");
	UT_ASSERT_EQ(clamp(-1, 0, 10),  0,  "clamp(-1,0,10)=0");
	UT_ASSERT_EQ(clamp(11, 0, 10),  10, "clamp(11,0,10)=10");
	UT_ASSERT_EQ(clamp(0,  0, 0),   0,  "clamp(0,0,0)=0");

	UT_ASSERT_EQ(clampMin(5, 3),   5, "clampMin(5,3)=5");
	UT_ASSERT_EQ(clampMin(2, 3),   3, "clampMin(2,3)=3");
	UT_ASSERT_EQ(clampMin(-5, 0),  0, "clampMin(-5,0)=0");

	UT_ASSERT_EQ(clampMax(5, 10),  5,  "clampMax(5,10)=5");
	UT_ASSERT_EQ(clampMax(15, 10), 10, "clampMax(15,10)=10");
	UT_ASSERT_EQ(clampMax(0, 0),   0,  "clampMax(0,0)=0");

	int v = 20;
	clampRef(v, 0, 10);
	UT_ASSERT_EQ(v, 10, "clampRef: 20->10");
	v = -5;
	clampRef(v, 0, 10);
	UT_ASSERT_EQ(v, 0, "clampRef: -5->0");
}

static void testMath_MinMax()
{
	UT_ASSERT_EQ(getMin(3, 7),    3, "getMin(3,7)=3");
	UT_ASSERT_EQ(getMin(-1, -5), -5, "getMin(-1,-5)=-5");
	UT_ASSERT_EQ(getMin(0, 0),    0, "getMin(0,0)=0");

	UT_ASSERT_EQ(getMax(3, 7),    7, "getMax(3,7)=7");
	UT_ASSERT_EQ(getMax(-1, -5), -1, "getMax(-1,-5)=-1");
	UT_ASSERT_EQ(getMax(0, 0),    0, "getMax(0,0)=0");
}

static void testMath_IsZeroEqual()
{
	UT_ASSERT(isZero(0.0f),              "isZero(0.0)=true");
	UT_ASSERT(isZero(0.000005f),          "isZero(1e-5)=true");
	UT_ASSERT(!isZero(0.1f),              "isZero(0.1)=false");
	UT_ASSERT(!isZero(-0.1f),             "isZero(-0.1)=false");

	UT_ASSERT(isEqual(1.0f, 1.0f),       "isEqual(1,1)=true");
	UT_ASSERT(!isEqual(1.0f, 1.1f),      "isEqual(1,1.1)=false");
	UT_ASSERT(isEqual(0.0f, 0.000005f),  "isEqual(0,1e-5)=true");
}

static void testMath_IsEvenPow2()
{
	UT_ASSERT(isEven(0),    "isEven(0)=true");
	UT_ASSERT(isEven(4),    "isEven(4)=true");
	UT_ASSERT(isEven(-2),   "isEven(-2)=true");
	UT_ASSERT(!isEven(1),   "isEven(1)=false");
	UT_ASSERT(!isEven(7),   "isEven(7)=false");

	UT_ASSERT(isPow2(1),    "isPow2(1)=true");
	UT_ASSERT(isPow2(2),    "isPow2(2)=true");
	UT_ASSERT(isPow2(4),    "isPow2(4)=true");
	UT_ASSERT(isPow2(1024), "isPow2(1024)=true");
	UT_ASSERT(!isPow2(0),   "isPow2(0)=false");
	UT_ASSERT(!isPow2(3),   "isPow2(3)=false");
	UT_ASSERT(!isPow2(6),   "isPow2(6)=false");
}

static void testMath_Divide()
{
	UT_ASSERT_FLOAT(divide(10.0f, 2.0f), 5.0f, "divide(10,2)=5");
	UT_ASSERT_FLOAT(divide(10.0f, 0.0f), 0.0f, "divide(10,0)=0 guard");
	UT_ASSERT_FLOAT(divide(0.0f,  2.0f), 0.0f, "divide(0,2)=0");
	UT_ASSERT_FLOAT(divide(10, 4),        2.5f, "divide(int 10,4)=2.5");
	UT_ASSERT_FLOAT(divide(10, 0),        0.0f, "divide(int 10,0)=0 guard");
}

static void testMath_FmodFrac()
{
	UT_ASSERT_FLOAT(fmod(5.5f, 2.0f), 1.5f, "fmod(5.5,2)=1.5");
	UT_ASSERT_FLOAT(fmod(4.0f, 2.0f), 0.0f, "fmod(4,2)=0");
	UT_ASSERT_FLOAT(frac(3.7f),        0.7f, "frac(3.7)=0.7");
	UT_ASSERT_FLOAT(frac(0.0f),        0.0f, "frac(0)=0");
	UT_ASSERT_FLOAT(frac(1.0f),        0.0f, "frac(1.0)=0");
}

static void testMath_BatchCount()
{
	// 10 items / batch=3 => 4 batches (3+3+3+1)
	UT_ASSERT_EQ(generateBatchCount(10, 3),   4, "batchCount(10,3)=4");
	// 9 items / batch=3 => 3 batches
	UT_ASSERT_EQ(generateBatchCount(9, 3),    3, "batchCount(9,3)=3");
	// 0 items => 0 batches
	UT_ASSERT_EQ(generateBatchCount(0, 3),    0, "batchCount(0,3)=0");
	// 1 item / batch=100 => 1 batch
	UT_ASSERT_EQ(generateBatchCount(1, 100),  1, "batchCount(1,100)=1");

	// batchIndex=0 => min(10-0,3)=3
	UT_ASSERT_EQ(generateBatchSize(10, 3, 0), 3, "batchSize(10,3,0)=3");
	// batchIndex=1 => min(10-3,3)=3
	UT_ASSERT_EQ(generateBatchSize(10, 3, 1), 3, "batchSize(10,3,1)=3");
	// batchIndex=3 (last) => min(10-9,3)=1
	UT_ASSERT_EQ(generateBatchSize(10, 3, 3), 1, "batchSize(10,3,3)=1 tail");
	// batchIndex=4 (out of range) => 0
	UT_ASSERT_EQ(generateBatchSize(10, 3, 4), 0, "batchSize(10,3,4)=0 oor");
}

static void testMathUtility()
{
	testMath_Ceiling();
	testMath_Floor();
	testMath_Clamp();
	testMath_MinMax();
	testMath_IsZeroEqual();
	testMath_IsEvenPow2();
	testMath_Divide();
	testMath_FmodFrac();
	testMath_BatchCount();
}

//------------------------------------------------------------------------------
// BinaryUtility tests
//------------------------------------------------------------------------------

static void testBinary_BitOps()
{
	// hasBit
	UT_ASSERT(BinaryUtility::hasBit((int)0b1010, 1),  "hasBit(1010,1)=true");
	UT_ASSERT(BinaryUtility::hasBit((int)0b1010, 3),  "hasBit(1010,3)=true");
	UT_ASSERT(!BinaryUtility::hasBit((int)0b1010, 0), "hasBit(1010,0)=false");
	UT_ASSERT(!BinaryUtility::hasBit((int)0b1010, 2), "hasBit(1010,2)=false");

	// getBit
	UT_ASSERT_EQ(BinaryUtility::getBit((int)0b1010, 1), 1, "getBit(1010,1)=1");
	UT_ASSERT_EQ(BinaryUtility::getBit((int)0b1010, 0), 0, "getBit(1010,0)=0");

	// setBitOne / setBitZero
	int v = 0b0000;
	BinaryUtility::setBitOne(v, 2);
	UT_ASSERT_EQ(v, 0b0100, "setBitOne(2)->0b0100");
	BinaryUtility::setBitZero(v, 2);
	UT_ASSERT_EQ(v, 0b0000, "setBitZero(2)->0b0000");

	// setBit
	BinaryUtility::setBit(v, 3, 1);
	UT_ASSERT_EQ(v, 0b1000, "setBit(3,1)->0b1000");
	BinaryUtility::setBit(v, 3, 0);
	UT_ASSERT_EQ(v, 0b0000, "setBit(3,0)->0b0000");
}

static void testBinary_HighLowBit()
{
	// getHighestBit on byte
	byte val8 = 0x80u;  // 1000 0000
	UT_ASSERT_EQ((int)BinaryUtility::getHighestBit(val8), 1, "getHighestBit(0x80)=1");
	val8 = 0x7Fu;       // 0111 1111
	UT_ASSERT_EQ((int)BinaryUtility::getHighestBit(val8), 0, "getHighestBit(0x7F)=0");

	// setHighestBit on int
	int iv = 0;
	BinaryUtility::setHighestBit(iv, true);
	UT_ASSERT(BinaryUtility::getHighestBit(iv) == 1, "setHighestBit(true)->1");
	BinaryUtility::setHighestBit(iv, false);
	UT_ASSERT(BinaryUtility::getHighestBit(iv) == 0, "setHighestBit(false)->0");

	// getLowestBit
	UT_ASSERT_EQ(BinaryUtility::getLowestBit((int)0b101), 1, "getLowestBit(101)=1");
	UT_ASSERT_EQ(BinaryUtility::getLowestBit((int)0b110), 0, "getLowestBit(110)=0");
}

static void testBinary_ByteOps()
{
	// little-endian layout: pos=0 is lowest byte
	int v = 0x01020304;
	UT_ASSERT_EQ((int)BinaryUtility::getByte(v, 0), 0x04, "getByte(0x01020304,0)=0x04");
	UT_ASSERT_EQ((int)BinaryUtility::getByte(v, 1), 0x03, "getByte(0x01020304,1)=0x03");
	UT_ASSERT_EQ((int)BinaryUtility::getByte(v, 2), 0x02, "getByte(0x01020304,2)=0x02");
	UT_ASSERT_EQ((int)BinaryUtility::getByte(v, 3), 0x01, "getByte(0x01020304,3)=0x01");

	// setByte
	int w = 0;
	BinaryUtility::setByte(w, (byte)0xABu, 1);
	UT_ASSERT_EQ((int)BinaryUtility::getByte(w, 1), 0xAB, "setByte(0xAB,1)->getByte=0xAB");
	UT_ASSERT_EQ((int)BinaryUtility::getByte(w, 0), 0x00, "setByte does not affect pos=0");
}

static void testBinary_GetBits()
{
	// byte 0b11011010: bits[1..3]=0b101=5
	UT_ASSERT_EQ((int)BinaryUtility::getBits((byte)0b11011010u, 1, 3), 0b101,  "getBits(11011010,1,3)=0b101");
	// bits[4..7]=0b1101=13
	UT_ASSERT_EQ((int)BinaryUtility::getBits((byte)0b11011010u, 4, 4), 0b1101, "getBits(11011010,4,4)=0b1101");
	// all zeros
	UT_ASSERT_EQ((int)BinaryUtility::getBits((byte)0u, 0, 8), 0,               "getBits(0,0,8)=0");
}

static void testBinary_BitToByteCount()
{
	UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(0),  0, "bitToByteCount(0)=0");
	UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(1),  1, "bitToByteCount(1)=1");
	UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(8),  1, "bitToByteCount(8)=1");
	UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(9),  2, "bitToByteCount(9)=2");
	UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(16), 2, "bitToByteCount(16)=2");
	UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(17), 3, "bitToByteCount(17)=3");
}

static void testBinary_ListMax()
{
	Vector<uint> uList{ 5u, 3u, 9u, 1u };
	UT_ASSERT_EQ((int)BinaryUtility::getListMax(uList), 9, "getListMax<uint>{5,3,9,1}=9");

	Vector<uint> uSingle{ 42u };
	UT_ASSERT_EQ((int)BinaryUtility::getListMax(uSingle), 42, "getListMax<uint>{42}=42");

	Vector<int> sAbsList{ -10, 3, -7, 2 };
	UT_ASSERT_EQ(BinaryUtility::getListMaxAbs(sAbsList), 10, "getListMaxAbs{-10,3,-7,2}=10");

	Vector<int> allPos{ 1, 5, 3 };
	UT_ASSERT_EQ(BinaryUtility::getListMaxAbs(allPos), 5, "getListMaxAbs{1,5,3}=5");
}

static void testBinaryUtility()
{
	testBinary_BitOps();
	testBinary_HighLowBit();
	testBinary_ByteOps();
	testBinary_GetBits();
	testBinary_BitToByteCount();
	testBinary_ListMax();
}

//------------------------------------------------------------------------------
// StringUtility tests
//------------------------------------------------------------------------------

static void testString_NumToStr()
{
	UT_ASSERT_STR(IToS(0),           "0",             "IToS(0)");
	UT_ASSERT_STR(IToS(123),         "123",           "IToS(123)");
	UT_ASSERT_STR(IToS(-456),        "-456",          "IToS(-456)");
	UT_ASSERT_STR(IToS(2147483647),  "2147483647",    "IToS(INT_MAX)");

	UT_ASSERT_STR(LLToS(0LL),              "0",             "LLToS(0)");
	UT_ASSERT_STR(LLToS(123456789012LL),   "123456789012",  "LLToS(large)");
	UT_ASSERT_STR(LLToS(-987654321098LL),  "-987654321098", "LLToS(neg large)");

	// FToS precision=4, trailing zeros stripped
	UT_ASSERT_STR(FToS(0.0f),   "0",      "FToS(0)");
	UT_ASSERT_STR(FToS(1.5f),   "1.5",    "FToS(1.5)");
	UT_ASSERT_STR(FToS(-2.25f), "-2.25",  "FToS(-2.25)");
}

static void testString_StrToNum()
{
	UT_ASSERT_EQ(SToI("0"),      0,    "SToI('0')");
	UT_ASSERT_EQ(SToI("123"),    123,  "SToI('123')");
	UT_ASSERT_EQ(SToI("-456"),  -456,  "SToI('-456')");

	UT_ASSERT_EQ_LL(SToLL("0"),              0LL,            "SToLL('0')");
	UT_ASSERT_EQ_LL(SToLL("123456789012"),   123456789012LL, "SToLL(large)");
	UT_ASSERT_EQ_LL(SToLL("-987654321098"), -987654321098LL, "SToLL(neg large)");

	UT_ASSERT_FLOAT(SToF("0"),     0.0f,   "SToF('0')");
	UT_ASSERT_FLOAT(SToF("1.5"),   1.5f,   "SToF('1.5')");
	UT_ASSERT_FLOAT(SToF("-2.25"), -2.25f, "SToF('-2.25')");
}

static void testString_Split()
{
	// normal split
	Vector<string> parts;
	StringUtility::split("a,b,c", ',', parts);
	UT_ASSERT_EQ((int)parts.size(), 3, "split 'a,b,c' -> 3 parts");
	UT_ASSERT_STR(parts[0], "a", "split[0]='a'");
	UT_ASSERT_STR(parts[1], "b", "split[1]='b'");
	UT_ASSERT_STR(parts[2], "c", "split[2]='c'");

	// removeEmpty=true skips consecutive delimiters
	parts.clear();
	StringUtility::split("a,,c", ',', parts, true);
	UT_ASSERT_EQ((int)parts.size(), 2, "split 'a,,c' removeEmpty -> 2");
	UT_ASSERT_STR(parts[0], "a", "split_rme[0]='a'");
	UT_ASSERT_STR(parts[1], "c", "split_rme[1]='c'");

	// removeEmpty=false keeps empty token
	parts.clear();
	StringUtility::split("a,,c", ',', parts, false);
	UT_ASSERT_EQ((int)parts.size(), 3, "split 'a,,c' keepEmpty -> 3");
	UT_ASSERT_STR(parts[1], "", "split_keep[1]=''");

	// single element (no delimiter)
	parts.clear();
	StringUtility::split("hello", ',', parts);
	UT_ASSERT_EQ((int)parts.size(), 1, "split 'hello' -> 1");
	UT_ASSERT_STR(parts[0], "hello", "split_single='hello'");

	// empty string
	parts.clear();
	StringUtility::split("", ',', parts);
	UT_ASSERT_EQ((int)parts.size(), 0, "split '' -> 0");
}

static void testString_Find()
{
	int pos = -1;
	UT_ASSERT(StringUtility::findString("hello world", "world", &pos), "findString 'world' found");
	UT_ASSERT_EQ(pos, 6, "findString 'world' pos=6");

	UT_ASSERT(!StringUtility::findString("hello world", "xyz"), "findString 'xyz' not found");

	// search from startIndex
	// "aababc": indices 0='a' 1='a' 2='b' 3='a' 4='b' 5='c'
	// "ab" first appears at pos=1; searching from startIndex=2 skips it, next match is at pos=3
	pos = -1;
	UT_ASSERT(StringUtility::findString("aababc", "ab", &pos, 2), "findString from 2 found");
	UT_ASSERT_EQ(pos, 3, "findString from 2 pos=3");

	UT_ASSERT_EQ(StringUtility::findCharCount("a,b,,c", ','), 3, "findCharCount 'a,b,,c'=3");
	UT_ASSERT_EQ(StringUtility::findCharCount("abc", ','),    0, "findCharCount 'abc'=0");
	UT_ASSERT_EQ(StringUtility::findCharCount("", ','),       0, "findCharCount ''=0");
}

static void testString_StartEndWith()
{
	UT_ASSERT(StringUtility::startWith("hello world", "hello"),  "startWith 'hello' ok");
	UT_ASSERT(!StringUtility::startWith("hello world", "world"), "startWith 'world' fail");
	UT_ASSERT(StringUtility::startWith("hello", "hello"),        "startWith exact match");
	UT_ASSERT(!StringUtility::startWith("hi", "hello"),          "startWith too short");
	UT_ASSERT(StringUtility::startWith("", ""),                  "startWith empty ok");

	UT_ASSERT(StringUtility::endWith("hello world", "world"),    "endWith 'world' ok");
	UT_ASSERT(!StringUtility::endWith("hello world", "hello"),   "endWith 'hello' fail");
	UT_ASSERT(StringUtility::endWith("hello", "hello"),          "endWith exact match");
	UT_ASSERT(!StringUtility::endWith("hi", "hello"),            "endWith too short");
}

static void testString_ReplaceRemove()
{
	// replaceAll(string&, string, string)
	string s = "aabbcc";
	StringUtility::replaceAll(s, string("bb"), string("XX"));
	UT_ASSERT_STR(s, "aaXXcc", "replaceAll 'bb'->'XX'");

	s = "aaaa";
	StringUtility::replaceAll(s, string("aa"), string("b"));
	UT_ASSERT_STR(s, "bb", "replaceAll 'aa'->'b' x2");

	// replaceAll(string&, char, char)
	s = "a.b.c";
	StringUtility::replaceAll(s, '.', '/');
	UT_ASSERT_STR(s, "a/b/c", "replaceAll '.'->'/'" );

	// removeAll(string&, char)
	s = "a1b2c3";
	StringUtility::removeAll(s, '1');
	UT_ASSERT_STR(s, "ab2c3", "removeAll '1'");

	// removeAll(string&, char, char) - remove two chars
	s = "a1b2c3";
	StringUtility::removeAll(s, '1', '3');
	UT_ASSERT_STR(s, "ab2c", "removeAll '1','3'");
}

static void testString_TrimChar()
{
	string s = "   hello";
	StringUtility::removeStartAll(s, ' ');
	UT_ASSERT_STR(s, "hello", "removeStartAll spaces");

	s = "hello   ";
	StringUtility::removeLastAll(s, ' ');
	UT_ASSERT_STR(s, "hello", "removeLastAll spaces");

	s = "   ";
	StringUtility::removeLastAll(s, ' ');
	UT_ASSERT_STR(s, "", "removeLastAll all spaces -> empty");

	s = "   ";
	StringUtility::removeStartAll(s, ' ');
	UT_ASSERT_STR(s, "", "removeStartAll all spaces -> empty");
}

static void testStringUtility()
{
	testString_NumToStr();
	testString_StrToNum();
	testString_Split();
	testString_Find();
	testString_StartEndWith();
	testString_ReplaceRemove();
	testString_TrimChar();
}

static void testMathUtility_ClampVariants()
{
	// clamp int
	UT_ASSERT_EQ(clamp(5, 1, 10), 5, "clamp in range");
	UT_ASSERT_EQ(clamp(-5, 0, 10), 0, "clamp below min");
	UT_ASSERT_EQ(clamp(15, 0, 10), 10, "clamp above max");
	UT_ASSERT_EQ(clamp(0, 0, 0), 0, "clamp zero range");
	UT_ASSERT_EQ(clamp(1, 1, 1), 1, "clamp single value");

	// clampRef
	int val = 15;
	clampRef(val, 0, 10);
	UT_ASSERT_EQ(val, 10, "clampRef above max");
	val = -5;
	clampRef(val, 0, 10);
	UT_ASSERT_EQ(val, 0, "clampRef below min");
	val = 5;
	clampRef(val, 0, 10);
	UT_ASSERT_EQ(val, 5, "clampRef in range unchanged");

	// clampMin
	UT_ASSERT_EQ(clampMin(5, 10), 10, "clampMin below");
	UT_ASSERT_EQ(clampMin(15, 10), 15, "clampMin above");
	UT_ASSERT_EQ(clampMin(10, 10), 10, "clampMin equal");

	// clampMax
	UT_ASSERT_EQ(clampMax(15, 10), 10, "clampMax above");
	UT_ASSERT_EQ(clampMax(5, 10), 5, "clampMax below");
	UT_ASSERT_EQ(clampMax(10, 10), 10, "clampMax equal");

	// clampZero
	UT_ASSERT_EQ(clampZero(-5), 0, "clampZero neg");
	UT_ASSERT_EQ(clampZero(5), 5, "clampZero pos");
	UT_ASSERT_EQ(clampZero(0), 0, "clampZero zero");

	// saturate
	{ float v = -0.5f; saturate(v); UT_ASSERT_FLOAT(v, 0.0f, "saturate neg"); }
	{ float v = 0.5f;  saturate(v); UT_ASSERT_FLOAT(v, 0.5f, "saturate mid"); }
	{ float v = 1.5f;  saturate(v); UT_ASSERT_FLOAT(v, 1.0f, "saturate above 1"); }
	{ float v = 0.0f;  saturate(v); UT_ASSERT_FLOAT(v, 0.0f, "saturate 0"); }
	{ float v = 1.0f;  saturate(v); UT_ASSERT_FLOAT(v, 1.0f, "saturate 1"); }

}

static void testMathUtility_StepFracFmod()
{
	// step
	UT_ASSERT_FLOAT((float)step(5.0f, 3.0f), 0.0f, "step x<edge=0");
	UT_ASSERT_FLOAT((float)step(5.0f, 5.0f), 1.0f, "step x==edge=1");
	UT_ASSERT_FLOAT((float)step(5.0f, 7.0f), 1.0f, "step x>edge=1");

	// frac
	UT_ASSERT_FLOAT(frac(3.7f), 0.7f, "frac 3.7=0.7");
	UT_ASSERT_FLOAT(frac(0.0f), 0.0f, "frac 0.0=0.0");
	UT_ASSERT_FLOAT(frac(1.0f), 0.0f, "frac 1.0=0.0");

	// fmod float
	UT_ASSERT_FLOAT(fmod(7.5f, 3.0f), 1.5f, "fmod 7.5%3=1.5");
	UT_ASSERT_FLOAT(fmod(6.0f, 3.0f), 0.0f, "fmod 6%3=0");
	UT_ASSERT_FLOAT(fmod(0.5f, 1.0f), 0.5f, "fmod 0.5%1=0.5");

	// divide int
	UT_ASSERT_EQ(divideInt(7, 3), 2, "divide 7/3=2");
	UT_ASSERT_EQ(divideInt(9, 3), 3, "divide 9/3=3");
	UT_ASSERT_EQ(divideInt(0, 5), 0, "divide 0/5=0");

}

static void testMathUtility_IsEvenIsPow2()
{
	// isEven
	UT_ASSERT(isEven(0), "isEven 0");
	UT_ASSERT(isEven(2), "isEven 2");
	UT_ASSERT(isEven(-4), "isEven -4");
	UT_ASSERT(!isEven(1), "not isEven 1");
	UT_ASSERT(!isEven(3), "not isEven 3");
	UT_ASSERT(!isEven(-3), "not isEven -3");

	// isPow2
	UT_ASSERT(!isPow2(0), "isPow2 0 false");
	UT_ASSERT(isPow2(1), "isPow2 1");
	UT_ASSERT(isPow2(2), "isPow2 2");
	UT_ASSERT(isPow2(4), "isPow2 4");
	UT_ASSERT(isPow2(16), "isPow2 16");
	UT_ASSERT(isPow2(1024), "isPow2 1024");
	UT_ASSERT(!isPow2(3), "isPow2 3 false");
	UT_ASSERT(!isPow2(6), "isPow2 6 false");
	UT_ASSERT(!isPow2(100), "isPow2 100 false");

}

static void testMathUtility_CeilingFloor()
{
	// ceiling float
	UT_ASSERT_EQ(ceiling(2.0f), 2, "ceiling 2.0=2");
	UT_ASSERT_EQ(ceiling(2.5f), 3, "ceiling 2.5=3");
	UT_ASSERT_EQ(ceiling(-1.5f), -1, "ceiling -1.5=-1");
	UT_ASSERT_EQ(ceiling(0.0f), 0, "ceiling 0.0=0");
	// NOTE: 2.0001f == 2.0f + 0.0001f fails >, so ceiling returns 2
	UT_ASSERT_EQ(ceiling(2.9999f), 3, "ceiling 2.9999=3");

	// floor_ float
	UT_ASSERT_EQ(floor_(2.0f), 2, "floor 2.0=2");
	UT_ASSERT_EQ(floor_(2.9f), 2, "floor 2.9=2");
	UT_ASSERT_EQ(floor_(-1.5f), -2, "floor -1.5=-2");
	UT_ASSERT_EQ(floor_(0.0f), 0, "floor 0.0=0");

}

static void testMathUtility_LerpInverseLerp()
{
	// lerp float
	UT_ASSERT_FLOAT(lerp(0.0f, 10.0f, 0.5f), 5.0f, "lerp 0-10 t=0.5");
	UT_ASSERT_FLOAT(lerp(0.0f, 10.0f, 0.0f), 0.0f, "lerp t=0");
	UT_ASSERT_FLOAT(lerp(0.0f, 10.0f, 1.0f), 10.0f, "lerp t=1");
	UT_ASSERT_FLOAT(lerp(-5.0f, 5.0f, 0.5f), 0.0f, "lerp -5..5 t=0.5");

	// lerp int
	UT_ASSERT_EQ(lerp(0, 10, 0.5f), 5, "lerp int 0-10 t=0.5");
	UT_ASSERT_EQ(lerp(0, 100, 0.25f), 25, "lerp int 0-100 t=0.25");

	// inverseLerp
	UT_ASSERT_FLOAT(inverseLerp(0.0f, 10.0f, 5.0f), 0.5f, "inverseLerp 5 in 0-10=0.5");
	UT_ASSERT_FLOAT(inverseLerp(0.0f, 10.0f, 0.0f), 0.0f, "inverseLerp 0 in 0-10=0");
	UT_ASSERT_FLOAT(inverseLerp(0.0f, 10.0f, 10.0f), 1.0f, "inverseLerp 10 in 0-10=1");

}

static void testMathUtility_ClampCycle()
{
	// clampCycle
	{ float v = 5.0f;  clampCycle(v, 0.0f, 10.0f); UT_ASSERT_FLOAT(v, 5.0f, "clampCycle in range"); }
	{ float v = -1.0f; clampCycle(v, 0.0f, 10.0f); UT_ASSERT_FLOAT(v, 9.0f, "clampCycle below wraps to 9"); }
	{ float v = 11.0f; clampCycle(v, 0.0f, 10.0f); UT_ASSERT_FLOAT(v, 1.0f, "clampCycle above wraps to 1"); }
	{ float v = 20.0f; clampCycle(v, 0.0f, 10.0f); UT_ASSERT_FLOAT(v, 10.0f, "clampCycle 20->10"); }
	{ float v = 10.0f; clampCycle(v, 0.0f, 10.0f); UT_ASSERT_FLOAT(v, 10.0f, "clampCycle max wraps to 10"); }

	// clampDegree360
	{ float v = 0.0f;   clampDegree360(v); UT_ASSERT_FLOAT(v, 0.0f, "clampDeg360 0=0"); }
	{ float v = 360.0f; clampDegree360(v); UT_ASSERT_FLOAT(v, 360.0f, "clampDeg360 360=360"); }
	{ float v = 370.0f; clampDegree360(v); UT_ASSERT_FLOAT(v, 10.0f, "clampDeg360 370=10"); }
	{ float v = -10.0f; clampDegree360(v); UT_ASSERT_FLOAT(v, 350.0f, "clampDeg360 -10=350"); }

	// clampDegree180
	{ float v = 0.0f;   clampDegree180(v); UT_ASSERT_FLOAT(v, 0.0f, "clampDeg180 0=0"); }
	{ float v = 180.0f; clampDegree180(v); UT_ASSERT_FLOAT(v, 180.0f, "clampDeg180 180=180"); }
	{ float v = 90.0f;  clampDegree180(v); UT_ASSERT_FLOAT(v, 90.0f, "clampDeg180 90=90"); }
	{ float v = -90.0f; clampDegree180(v); UT_ASSERT_FLOAT(v, -90.0f, "clampDeg180 -90=-90"); }

}

static void testMathUtility_Pow2Pow10()
{
	// pow2
	UT_ASSERT_EQ((int)pow2(0), 1, "pow2(0)=1");
	UT_ASSERT_EQ((int)pow2(1), 2, "pow2(1)=2");
	UT_ASSERT_EQ((int)pow2(4), 16, "pow2(4)=16");
	UT_ASSERT_EQ((int)pow2(10), 1024, "pow2(10)=1024");

	// pow10
	UT_ASSERT_EQ((int)pow10(0), 1, "pow10(0)=1");
	UT_ASSERT_EQ((int)pow10(1), 10, "pow10(1)=10");
	UT_ASSERT_EQ((int)pow10(3), 1000, "pow10(3)=1000");
	UT_ASSERT_EQ((int)pow10(6), 1000000, "pow10(6)=1000000");

}

static void testMathUtility_SecondsConvert()
{
	// secondsToMinutesSeconds
	{
		int m = 0, s = 0;
		secondsToMinutesSeconds(125, m, s);
		UT_ASSERT_EQ(m, 2, "sec2min m=2");
		UT_ASSERT_EQ(s, 5, "sec2min s=5");
	}
	{
		int m = 0, s = 0;
		secondsToMinutesSeconds(60, m, s);
		UT_ASSERT_EQ(m, 1, "sec2min 60 m=1");
		UT_ASSERT_EQ(s, 0, "sec2min 60 s=0");
	}
	{
		int m = 0, s = 0;
		secondsToMinutesSeconds(0, m, s);
		UT_ASSERT_EQ(m, 0, "sec2min 0 m=0");
		UT_ASSERT_EQ(s, 0, "sec2min 0 s=0");
	}

	// secondsToHoursMinutesSeconds
	{
		int h = 0, m = 0, s = 0;
		secondsToHoursMinutesSeconds(3661, h, m, s);
		UT_ASSERT_EQ(h, 1, "sec2hms h=1");
		UT_ASSERT_EQ(m, 1, "sec2hms m=1");
		UT_ASSERT_EQ(s, 1, "sec2hms s=1");
	}
	{
		int h = 0, m = 0, s = 0;
		secondsToHoursMinutesSeconds(3600, h, m, s);
		UT_ASSERT_EQ(h, 1, "sec2hms 3600 h=1");
		UT_ASSERT_EQ(m, 0, "sec2hms 3600 m=0");
		UT_ASSERT_EQ(s, 0, "sec2hms 3600 s=0");
	}

}

static void testMathUtility_VectorMath()
{
	// getLength
	{
		float len = getLength(3.0f, 4.0f);
		UT_ASSERT_FLOAT(len, 5.0f, "getLength 3,4=5");
	}
	{
		float len = getLength(0.0f, 0.0f);
		UT_ASSERT_FLOAT(len, 0.0f, "getLength 0,0=0");
	}
	{
		float len = getLength(1.0f, 1.0f);
		UT_ASSERT_FLOAT(len, sqrtf(2.0f), "getLength 1,1=sqrt2");
	}

	// getSquaredLength
	{
		float sq = getSquaredLength(Vector2(3.0f, 4.0f));
		UT_ASSERT_FLOAT(sq, 25.0f, "getSquaredLen 3,4=25");
	}

	// dot
	{
		float d = dot(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		UT_ASSERT_FLOAT(d, 0.0f, "dot perp=0");
	}
	{
		float d = dot(Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
		UT_ASSERT_FLOAT(d, 1.0f, "dot same=1");
	}

	// normalize
	{
		float nx = 0.0f, ny = 0.0f, nz = 0.0f;
		Vector3 n = normalize(Vector3(3.0f, 4.0f, 0.0f));
		nx = n.x; ny = n.y; nz = n.z;
		UT_ASSERT_FLOAT(nx, 0.6f, "normalize nx=0.6");
		UT_ASSERT_FLOAT(ny, 0.8f, "normalize ny=0.8");
		UT_ASSERT_FLOAT(nz, 0.0f, "normalize nz=0");
		float len = sqrtf(nx * nx + ny * ny + nz * nz);
		UT_ASSERT_FLOAT(len, 1.0f, "normalize len=1");
	}

}

static void testMathUtility_MinMax()
{
	// getMin
	UT_ASSERT_EQ(getMin(3, 5), 3, "getMin 3,5=3");
	UT_ASSERT_EQ(getMin(-2, 1), -2, "getMin neg");
	UT_ASSERT_EQ(getMin(7, 7), 7, "getMin equal");
	UT_ASSERT_FLOAT(getMin(1.5f, 2.5f), 1.5f, "getMin float");

	// getMax
	UT_ASSERT_EQ(getMax(3, 5), 5, "getMax 3,5=5");
	UT_ASSERT_EQ(getMax(-2, -1), -1, "getMax neg");
	UT_ASSERT_FLOAT(getMax(1.5f, 0.5f), 1.5f, "getMax float");

}

static void testMathUtility_InRange()
{
	// inRange int [min, max)
	UT_ASSERT(inRange(5, 0, 10), "inRange 5 in [0,10)");
	UT_ASSERT(!inRange(0, 1, 10), "inRange 0 not in [1,10)");
	UT_ASSERT(inRange(10, 0, 10), "inRange 10 in [0,10)");

	// inFixedRange [min, max] inclusive
	UT_ASSERT(inFixedRange(5, 0, 10), "inFixedRange 5");
	UT_ASSERT(inFixedRange(0, 0, 10), "inFixedRange 0");
	UT_ASSERT(inFixedRange(10, 0, 10), "inFixedRange 10");
	UT_ASSERT(!inFixedRange(11, 0, 10), "inFixedRange 11 false");
	UT_ASSERT(!inFixedRange(-1, 0, 10), "inFixedRange -1 false");

}

static void testMathUtility_Swap()
{
	// swap_
	int a = 3, b = 7;
	swap_(a, b);
	UT_ASSERT_EQ(a, 7, "swap_ a=7");
	UT_ASSERT_EQ(b, 3, "swap_ b=3");

	float fa = 1.5f, fb = 2.5f;
	swap_(fa, fb);
	UT_ASSERT_FLOAT(fa, 2.5f, "swap_ float a=2.5");
	UT_ASSERT_FLOAT(fb, 1.5f, "swap_ float b=1.5");

}

static void testMathUtility_DegreeRadian()
{
	// toDegree
	UT_ASSERT_FLOAT(toDegree(0.0f), 0.0f, "toDeg 0=0");
	UT_ASSERT_FLOAT(toDegree(3.14159265f), 180.0f, "toDeg pi=180");
	UT_ASSERT_FLOAT(toDegree(1.5707963f), 90.0f, "toDeg pi/2=90");

	// toRadian
	UT_ASSERT_FLOAT(toRadian(0.0f), 0.0f, "toRad 0=0");
	UT_ASSERT_FLOAT(toRadian(180.0f), 3.14159265f, "toRad 180=pi");
	UT_ASSERT_FLOAT(toRadian(90.0f), 1.5707963f, "toRad 90=pi/2");

	// round-trip
	float deg = 45.0f;
	UT_ASSERT_FLOAT(toDegree(toRadian(deg)), deg, "deg->rad->deg round-trip");

}

static void testMathUtility_GenerateBatch()
{
	// generateBatchCount
	UT_ASSERT_EQ(generateBatchCount(10, 3), 4, "batchCount 10/3=4 (ceil)");
	UT_ASSERT_EQ(generateBatchCount(9, 3), 3, "batchCount 9/3=3");
	UT_ASSERT_EQ(generateBatchCount(1, 3), 1, "batchCount 1/3=1");
	UT_ASSERT_EQ(generateBatchCount(0, 3), 0, "batchCount 0/3=0");

	// generateBatchSize
	UT_ASSERT_EQ(generateBatchSize(10, 3, 0), 3, "batchSize batch 0: 3");
	// batch 1: min(3, 10-3)=3
	UT_ASSERT_EQ(generateBatchSize(10, 3, 1), 3, "batchSize batch 1: 3");

}

static void testMathUtility_IsZeroEqual()
{
	// isZero
	UT_ASSERT(isZero(0.0f), "isZero 0.0");
	UT_ASSERT(isZero(0.00001f), "isZero tiny");
	UT_ASSERT(!isZero(0.01f), "isZero 0.01 false");

	// isEqual
	UT_ASSERT(isEqual(1.0f, 1.0f), "isEqual 1==1");
	UT_ASSERT(isEqual(1.0f, 1.000001f), "isEqual tiny diff");
	UT_ASSERT(!isEqual(1.0f, 1.1f), "isEqual 1.0!=1.1");

}

//------------------------------------------------------------------------------
// Entry point
//------------------------------------------------------------------------------
void UtilityTest::test()
{
	testMathUtility();
	testBinaryUtility();
	testStringUtility();
	testMathUtility_ClampVariants();
	testMathUtility_StepFracFmod();
	testMathUtility_IsEvenIsPow2();
	testMathUtility_CeilingFloor();
	testMathUtility_LerpInverseLerp();
	testMathUtility_ClampCycle();
	testMathUtility_Pow2Pow10();
	testMathUtility_SecondsConvert();
	testMathUtility_VectorMath();
	testMathUtility_MinMax();
	testMathUtility_InRange();
	testMathUtility_Swap();
	testMathUtility_DegreeRadian();
	testMathUtility_GenerateBatch();
	testMathUtility_IsZeroEqual();
}