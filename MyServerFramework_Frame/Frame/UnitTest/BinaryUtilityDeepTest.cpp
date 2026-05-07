#include "FrameHeader.h"

namespace
{
	static void assertBinary(bool condition, const char* message)
	{
		if (!condition)
		{
			ERROR(string("BinaryUtilityDeepTest failed: ") + message);
		}
	}

	static bool approxEqualFloat(const float a, const float b, const float eps = 0.01f)
	{
		return fabs(a - b) <= eps;
	}

	static bool approxEqualDouble(const double a, const double b, const double eps = 0.01)
	{
		return fabs(a - b) <= eps;
	}

	static void testBitCountAndFlags()
	{
		assertBinary(generateBitCount((byte)0) == 0, "bitcount byte 0");
		assertBinary(generateBitCount((byte)1) == 1, "bitcount byte 1");
		assertBinary(generateBitCount((byte)2) == 2, "bitcount byte 2");
		assertBinary(generateBitCount((byte)3) == 2, "bitcount byte 3");
		assertBinary(generateBitCount((ushort)255) == 8, "bitcount ushort 255");
		assertBinary(generateBitCount((ushort)256) == 9, "bitcount ushort 256");
		assertBinary(generateBitCount((uint)65535) == 16, "bitcount uint 65535");
		assertBinary(generateBitCount((ullong)4294967296ull) == 33, "bitcount ullong");
		assertBinary(getBits((byte)0b11010110, (byte)1, (byte)3) == 0b011, "getBits");
		assertBinary(bitCountToByteCount(0) == 0, "bitcount bytes 0");
		assertBinary(bitCountToByteCount(1) == 1, "bitcount bytes 1");
		assertBinary(bitCountToByteCount(8) == 1, "bitcount bytes 8");
		assertBinary(bitCountToByteCount(9) == 2, "bitcount bytes 9");
	}

	static void testBitManipulation()
	{
		uint value = 0;
		setByte(value, (byte)0x12, 0);
		setByte(value, (byte)0x34, 1);
		setByte(value, (byte)0x56, 2);
		assertBinary(getByte(value, 0) == 0x12, "getByte 0");
		assertBinary(getByte(value, 1) == 0x34, "getByte 1");
		assertBinary(getByte(value, 2) == 0x56, "getByte 2");

		setBit(value, 0, 1);
		setBitOne(value, 3);
		setBitZero(value, 3);
		assertBinary(hasBit(value, 0), "hasBit 0");
		assertBinary(!hasBit(value, 3), "hasBit 3");
		assertBinary(getBit(value, 0) == 1, "getBit 0");
		assertBinary(getHighestBit((byte)0x80) == 1, "highest bit");
		assertBinary(getLowestBit((byte)0x81) == 1, "lowest bit");

		byte copy = 0;
		copyBits(copy, 2, (byte)0b11110000, 4, 4);
		assertBinary(copy == 0b00001111 << 2, "copyBits");

		ushort swapValue = 0x1234;
		swapByte(swapValue, 0, 1);
		assertBinary(swapValue == 0x3412, "swapByte");
		uint inverseValue = 0x11223344;
		inverseByte(inverseValue);
		assertBinary(inverseValue == 0x44332211, "inverseByte");

		char buffer[32]{};
		setString(buffer, 32, string("abc"));
		assertBinary(string(buffer) == "abc", "setString");
	}

	static void testPrimitiveReadWrite()
	{
		char buffer[64]{};
		int index = 0;
		assertBinary(write(buffer, 64, index, (int)123456), "write int");
		assertBinary(writeInverse(buffer, 64, index, (short)-321), "write inverse short");
		assertBinary(write(buffer, 64, index, (float)12.5f), "write float");
		index = 0;
		int intValue = 0;
		short shortValue = 0;
		float floatValue = 0.0f;
		assertBinary(read(buffer, 64, index, intValue), "read int");
		assertBinary(readInverse(buffer, 64, index, shortValue), "read inverse short");
		assertBinary(read(buffer, 64, index, floatValue), "read float");
		assertBinary(intValue == 123456, "int roundtrip");
		assertBinary(shortValue == -321, "short roundtrip");
		assertBinary(floatValue == 12.5f, "float roundtrip");
	}

	static void testVectorReadWrite()
	{
		char buffer[128]{};
		int index = 0;
		assertBinary(writeVector2(buffer, 128, index, Vector2(1.5f, -2.5f)), "write vec2");
		assertBinary(writeVector3(buffer, 128, index, Vector3(3.25f, 4.5f, -5.75f)), "write vec3");
		assertBinary(writeVector4(buffer, 128, index, Vector4(6.0f, 7.0f, 8.0f, 9.0f)), "write vec4");
		index = 0;
		Vector2 v2;
		Vector3 v3;
		Vector4 v4;
		assertBinary(readVector2(buffer, 128, index, v2), "read vec2");
		assertBinary(readVector3(buffer, 128, index, v3), "read vec3");
		assertBinary(readVector4(buffer, 128, index, v4), "read vec4");
		assertBinary(approxEqualFloat(v2.x, 1.5f) && approxEqualFloat(v2.y, -2.5f), "vec2 roundtrip");
		assertBinary(approxEqualFloat(v3.z, -5.75f), "vec3 roundtrip");
		assertBinary(approxEqualFloat(v4.w, 9.0f), "vec4 roundtrip");
	}

	static void testColorAndBuffer()
	{
		char buffer[128]{};
		int index = 0;
		Color color(11, 22, 33, 44);
		assertBinary(writeColor(buffer, 128, index, color), "write color");
		assertBinary(writeBuffer(buffer, 128, index, "XYZ", 3), "write buffer");
		index = 0;
		Color loaded;
		char text[4]{};
		assertBinary(readColor(buffer, 128, index, loaded), "read color");
		assertBinary(readBuffer(buffer, 128, index, text, 4, 3), "read buffer");
		assertBinary(loaded.r == 11, "color r");
		assertBinary(string(text, 3) == "XYZ", "buffer text");
	}

	static void testUnsignedIntegerBits()
	{
		char buffer[64]{};
		int bitIndex = 0;
		assertBinary(writeBoolBit(buffer, 64, bitIndex, true), "write bool");
		assertBinary(writeUnsignedIntegerBit(buffer, 64, bitIndex, (ushort)511), "write ushort");
		assertBinary(writeUnsignedIntegerBit(buffer, 64, bitIndex, (uint)65535), "write uint");
		assertBinary(writeUnsignedIntegerBit(buffer, 64, bitIndex, (ullong)123456789ull), "write ullong");
		bitIndex = 0;
		bool boolValue = false;
		ushort us = 0;
		uint ui = 0;
		ullong ul = 0;
		assertBinary(readBoolBit(buffer, 64, bitIndex, boolValue), "read bool");
		assertBinary(readUnsignedIntegerBit(buffer, 64, bitIndex, us), "read ushort");
		assertBinary(readUnsignedIntegerBit(buffer, 64, bitIndex, ui), "read uint");
		assertBinary(readUnsignedIntegerBit(buffer, 64, bitIndex, ul), "read ullong");
		assertBinary(boolValue, "bool roundtrip");
		assertBinary(us == 511, "ushort roundtrip");
		assertBinary(ui == 65535, "uint roundtrip");
		assertBinary(ul == 123456789ull, "ullong roundtrip");
	}

	static void testSignedIntegerBits()
	{
		char buffer[128]{};
		int bitIndex = 0;
		assertBinary(writeSignedIntegerBit(buffer, 128, bitIndex, (int)-12345, true), "write signed int");
		assertBinary(writeSignedIntegerBit(buffer, 128, bitIndex, (short)321, true), "write signed short");
		assertBinary(writeSignedIntegerBit(buffer, 128, bitIndex, (llong)-9876543210ll, true), "write signed llong");
		bitIndex = 0;
		int i = 0;
		short s = 0;
		llong l = 0;
		assertBinary(readSignedIntegerBit(buffer, 128, bitIndex, i, true), "read signed int");
		assertBinary(readSignedIntegerBit(buffer, 128, bitIndex, s, true), "read signed short");
		assertBinary(readSignedIntegerBit(buffer, 128, bitIndex, l, true), "read signed llong");
		assertBinary(i == -12345, "signed int roundtrip");
		assertBinary(s == 321, "signed short roundtrip");
		assertBinary(l == -9876543210ll, "signed llong roundtrip");
	}

	static void testListMaxAndBitLists()
	{
		Vector<int> signedList;
		signedList.add(-3);
		signedList.add(7);
		signedList.add(-12);
		signedList.add(5);
		assertBinary(BinaryUtility::getListMaxAbs(signedList.data(), signedList.size()) == 12, "list max abs");

		Vector<uint> unsignedList;
		unsignedList.add(1u);
		unsignedList.add(255u);
		unsignedList.add(512u);
		assertBinary(BinaryUtility::getListMax(unsignedList.data(), unsignedList.size()) == 512, "list max");

		char buffer[256]{};
		int bitIndex = 0;
		assertBinary(writeUnsignedIntegerListBit(buffer, 256, bitIndex, unsignedList), "write unsigned list");
		bitIndex = 0;
		Vector<uint> unsignedRead;
		assertBinary(readUnsignedIntegerListBit(buffer, 256, bitIndex, unsignedRead), "read unsigned list");
		assertBinary(unsignedRead.size() == 3, "unsigned list size");
		assertBinary(unsignedRead[1] == 255u, "unsigned list value");

		bitIndex = 0;
		Vector<int> signedRead;
		assertBinary(writeSignedIntegerListBit(buffer, 256, bitIndex, signedList, true), "write signed list");
		bitIndex = 0;
		assertBinary(readSignedIntegerListBit(buffer, 256, bitIndex, signedRead, true), "read signed list");
		assertBinary(signedRead.size() == 4, "signed list size");
		assertBinary(signedRead[2] == -12, "signed list value");
	}

	static void testFloatDoubleLists()
	{
		Vector<float> floats;
		floats.add(1.25f);
		floats.add(-2.5f);
		floats.add(3.75f);
		Vector<double> doubles;
		doubles.add(10.125);
		doubles.add(-20.25);
		doubles.add(30.5);

		char buffer[512]{};
		int bitIndex = 0;
		assertBinary(writeFloatListBit(buffer, 512, bitIndex, floats, true, 2), "write float list");
		assertBinary(writeDoubleListBit(buffer, 512, bitIndex, doubles, true, 3), "write double list");
		bitIndex = 0;
		Vector<float> floatsRead;
		Vector<double> doublesRead;
		assertBinary(readFloatListBit(buffer, 512, bitIndex, floatsRead, true, 2), "read float list");
		assertBinary(readDoubleListBit(buffer, 512, bitIndex, doublesRead, true, 3), "read double list");
		assertBinary(floatsRead.size() == 3, "float list size");
		assertBinary(doublesRead.size() == 3, "double list size");
		assertBinary(approxEqualFloat(floatsRead[0], 1.25f, 0.05f), "float list value");
		assertBinary(approxEqualDouble(doublesRead[2], 30.5, 0.05), "double list value");
	}
}

void BinaryUtilityDeepTest::test()
{
	testBitCountAndFlags();
	testBitManipulation();
	testPrimitiveReadWrite();
	testVectorReadWrite();
	testColorAndBuffer();
	testUnsignedIntegerBits();
	testSignedIntegerBits();
	testListMaxAndBitLists();
	testFloatDoubleLists();
}
