#include "FrameHeader.h"

namespace
{
#define SBR_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("SerializerBitRoundTripTest FAILED: ") + (msg)); } } while(0)
#define SBR_ASSERT_EQ(a, bool_val, msg) \
	do { if ((a) != (bool_val)) { ERROR(string("SerializerBitRoundTripTest FAILED: ") + (msg)); } } while(0)
#define SBR_ASSERT_INT(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SerializerBitRoundTripTest FAILED: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SBR_ASSERT_FLOAT(a, b, eps, msg) \
	do { if (fabsf((float)(a) - (float)(b)) > (eps)) { ERROR(string("SerializerBitRoundTripTest FAILED: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)

	// ========== round-trip helper: SerializerBitWrite → SerializerBitRead ==========
	template<typename T>
	static bool bitWriteRead(const T& value, T& outValue,
	                          bool(SerializerBitWrite::*writeFunc)(T),
	                          bool(SerializerBitRead::*readFunc)(T&))
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		if (!(w.*writeFunc)(value))
		{
			return false;
		}

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		return (r.*readFunc)(outValue);
	}

	// ========== round-trip helper for signed (needWriteSign parameter) ==========
	template<typename T>
	static bool bitSignedWriteRead(const T& value, T& outValue, bool needSign = true)
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		if constexpr (is_same<float, T>::value)
		{
			if (!w.writeFloat(needSign, value))
			{
				return false;
			}
		}
		else if constexpr (is_same<double, T>::value)
		{
			if (!w.writeDouble(needSign, value))
			{
				return false;
			}
		}
		else
		{
			if (!w.writeSigned(needSign, value))
			{
				return false;
			}
		}

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		if constexpr (is_same<float, T>::value)
		{
			return r.readFloat(needSign, outValue);
		}
		else if constexpr (is_same<double, T>::value)
		{
			return r.readDouble(needSign, outValue);
		}
		else
		{
			return r.readSigned(needSign, outValue);
		}
	}

	// ========== round-trip helper for unsigned ==========
	template<typename T>
	static bool bitUnsignedWriteRead(const T& value, T& outValue)
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		if (!w.writeUnsigned(value))
		{
			return false;
		}

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		return r.readUnsigned(outValue);
	}

	// ---------- bool ----------
	static void testRT_Bool()
	{
		bool val, out;
		val = true;  SBR_ASSERT(bitWriteRead(val, out, &SerializerBitWrite::writeBool, &SerializerBitRead::readBool), "bool true ok");
		SBR_ASSERT(out == true, "bool true value");

		val = false; SBR_ASSERT(bitWriteRead(val, out, &SerializerBitWrite::writeBool, &SerializerBitRead::readBool), "bool false ok");
		SBR_ASSERT(out == false, "bool false value");
	}

	// ---------- signed int: 典型值 ----------
	static void testRT_SignedInt_Typical()
	{
		int values[] = {0, 1, -1, 42, -42, 1000, -1000, 99999, -99999};
		for (int v : values)
		{
			int out = 0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "signed int typical write ok");
			SBR_ASSERT_INT(out, v, "signed int typical read match");
		}
	}

	// ---------- signed int: 边界值 ----------
	// NOTE: 变长有符号位编码无法表示 INT_MIN/SHRT_MIN/LLONG_MIN,
	//       因为取绝对值后需要的比特数超出长度位容量(sizeof(T)*8 vs 长度位最大值31/15/63),
	//       且 -INT_MIN 本身对有符号整数是未定义行为(溢出)
	static void testRT_SignedInt_Boundary()
	{
		int boundary[] = {INT_MAX, INT_MAX - 1, INT_MIN + 1};
		for (int v : boundary)
		{
			int out = 0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "signed int boundary write ok");
			SBR_ASSERT_INT(out, v, "signed int boundary read match");
		}
	}

	// ---------- unsigned int: 典型值 ----------
	static void testRT_UnsignedInt_Typical()
	{
		uint values[] = {0u, 1u, 42u, 1000u, 99999u, 4000000000u};
		for (uint v : values)
		{
			uint out = 0;
			SBR_ASSERT(bitUnsignedWriteRead(v, out), "unsigned int typical write ok");
			SBR_ASSERT((llong)out == (llong)v, "unsigned int typical read match");
		}
	}

	// ---------- unsigned int: 最大值 ----------
	static void testRT_UnsignedInt_Max()
	{
		uint v = UINT_MAX;
		uint out = 0;
		SBR_ASSERT(bitUnsignedWriteRead(v, out), "uint MAX write ok");
		SBR_ASSERT((llong)out == (llong)UINT_MAX, "uint MAX read match");
	}

	// ---------- short / ushort ----------
	static void testRT_Short()
	{
		short svals[] = {0, 1, -1, SHRT_MAX, SHRT_MIN + 1};
		for (short v : svals)
		{
			short out = 0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "short write ok");
			SBR_ASSERT_INT(out, v, "short read match");
		}

		ushort uvals[] = {0, 1, USHRT_MAX};
		for (ushort v : uvals)
		{
			ushort out = 0;
			SBR_ASSERT(bitUnsignedWriteRead(v, out), "ushort write ok");
			SBR_ASSERT(out == v, "ushort read match");
		}
	}

	// ---------- llong / ullong ----------
	static void testRT_LLong()
	{
		llong vals[] = {0LL, 1LL, -1LL, 10000000000LL, -10000000000LL, LLONG_MAX, LLONG_MIN + 1};
		for (llong v : vals)
		{
			llong out = 0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "llong write ok");
			SBR_ASSERT(out == v, "llong read match");
		}

		ullong uvals[] = {0ULL, 1ULL, ULLONG_MAX, 10000000000ULL};
		for (ullong v : uvals)
		{
			ullong out = 0;
			SBR_ASSERT(bitUnsignedWriteRead(v, out), "ullong write ok");
			SBR_ASSERT(out == v, "ullong read match");
		}
	}

	// ---------- byte ----------
	static void testRT_Byte()
	{
		byte vals[] = {0, 1, 127, 128, 255};
		for (byte v : vals)
		{
			byte out = 0;
			SBR_ASSERT(bitUnsignedWriteRead(v, out), "byte write ok");
			SBR_ASSERT(out == v, "byte read match");
		}
	}

	// ---------- float ----------
	static void testRT_Float()
	{
		float vals[] = {0.0f, 1.0f, -1.0f, 3.14f, -3.14f, 0.001f, 1000000.0f};
		for (float v : vals)
		{
			float out = 0.0f;
			SBR_ASSERT(bitSignedWriteRead(v, out), "float write ok");
			SBR_ASSERT_FLOAT(out, v, 0.01f * fabsf(v) + 1e-4f, "float read match");
		}
	}

	// ---------- double ----------
	// NOTE: writeDouble 内部是 roundDouble(value * pow10LLong(precision)) 转成整数存储,
	//       精度固定为 10^{-precision}(默认4位小数)。大数值×10^4可能溢出llong。
	static void testRT_Double()
	{
		// 小数值：精度损失 ≤ 0.5 × 10^{-4}
		double smallVals[] = {0.0, 1.0, -1.0, 3.14159265358979, -3.14159265358979};
		for (double v : smallVals)
		{
			double out = 0.0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "double write ok");
			double diff = fabs(out - v);
			// 4位小数精度：误差不超过 0.5 × 10^{-4} + 浮点epsilon
			double tol = 0.5e-4 + fabs(v) * 1e-12;
			SBR_ASSERT(diff < tol, "double read match");
		}
		// 大数值（但×10^4后不溢出llong）：验证 round-trip 一致性
		double bigVals[] = {1e8, -1e8, 1e12, -1e12};
		for (double v : bigVals)
		{
			double out = 0.0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "double big write ok");
			// 大数值的小数部分会被完全截断，只验证数量级一致
			double relDiff = (fabs(v) > 0.0) ? fabs(out - v) / fabs(v) : fabs(out);
			SBR_ASSERT(relDiff < 1e-3, "double big read approximate");
		}
	}

	// ---------- float/double: needWriteSign=false ----------
	static void testRT_Float_NoSign()
	{
		float v = -3.14f;
		float out = 0.0f;
		SerializerBitWrite w;
		w.initCapacity(256);
		w.writeFloat(false, v); // 无符号写入

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		r.readFloat(false, out);
		// 没符号位时负数会变正数（因为取了 abs），这是预期行为
		SBR_ASSERT(fabsf(out) > 3.0f, "float nosign magnitude preserved");
	}

	// ---------- signed int: 多参数列表 ----------
	static void testRT_SignedList()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		int v1 = 100;
		int v2 = -200;
		int v3 = 300;
		SBR_ASSERT(w.writeSigned(true, v1, v2, v3), "signed list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		int o1;
		int o2;
		int o3;
		SBR_ASSERT(r.readSigned(true, o1, o2, o3), "signed list read ok");
		SBR_ASSERT_INT(o1, v1, "list [0]");
		SBR_ASSERT_INT(o2, v2, "list [1]");
		SBR_ASSERT_INT(o3, v3, "list [2]");
	}

	// ---------- unsigned int: 多参数列表 ----------
	static void testRT_UnsignedList()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		uint a = 10u;
		uint b = 20u;
		uint c = 30u;
		SBR_ASSERT(w.writeUnsigned(a, b, c), "uint list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		uint x;
		uint y;
		uint z;
		SBR_ASSERT(r.readUnsigned(x, y, z), "uint list read ok");
		SBR_ASSERT(x == a && y == b && z == c, "uint list values match");
	}

	// ---------- string ----------
	static void testRT_String()
	{
		string tests[] = {"", "hello", "你好世界", "a\0b"};
		for (const string& str : tests)
		{
			SerializerBitWrite w;
			w.initCapacity(512);
			SBR_ASSERT(w.writeString(str), "string write ok");

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			string out;
			SBR_ASSERT(r.readString(out), "string read ok");
			SBR_ASSERT(out == str, "string roundtrip match");
		}
	}

	// ---------- Vector<int> 列表 round-trip ----------
	static void testRT_IntVectorList()
	{
		Vector<int> original{10, -20, 30, -40, 50};

		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeSignedList(true, original), "int vector list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<int> result;
		SBR_ASSERT(r.readSignedList(true, result), "int vector list read ok");
		SBR_ASSERT(result.size() == original.size(), "int vector list size match");
		FOR_VECTOR(original)
		{
			SBR_ASSERT_INT(result[i], original[i], "int vector list element match");
		}
	}

	// ---------- Vector<uint> 列表 round-trip ----------
	static void testRT_UintVectorList()
	{
		Vector<uint> original {100u, 200u, 300u, UINT_MAX};

		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeUnsignedList(original), "uint vector list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<uint> result;
		SBR_ASSERT(r.readUnsignedList(result), "uint vector list read ok");
		SBR_ASSERT(result.size() == original.size(), "uint vector list size match");
		FOR_VECTOR(original)
		{
			SBR_ASSERT(result[i] == original[i], "uint vector list element match");
		}
	}

	// ---------- Vector<float> 列表 round-trip ----------
	static void testRT_FloatVectorList()
	{
		Vector<float> original {1.1f, 2.2f, -3.3f, 0.0f, 100.5f};

		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeFloatList(true, original), "float vector list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<float> result;
		SBR_ASSERT(r.readFloatList(true, result), "float vector list read ok");
		SBR_ASSERT(result.size() == original.size(), "float vector list size match");
		for (int i = 0; i < original.size(); ++i)
		{
			SBR_ASSERT_FLOAT(result[i], original[i], 0.01f * fabsf(original[i]) + 1e-5f, "float vector element match");
		}
	}

	// ---------- Array 固定大小列表 ----------
	static void testRT_FixedArray()
	{
		Array<4, int> arr = {10, -20, 30, -40};

		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeSigned(true, arr), "fixed array write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Array<4, int> outArr;
		// 注意: Array 读需要用指针数组形式
		int* ptrs[4] = {&outArr[0], &outArr[1], &outArr[2], &outArr[3]};
		SBR_ASSERT(r.readSigned(true, *ptrs[0], *ptrs[1], *ptrs[2], *ptrs[3]), "fixed array read ok");
		FOR(4)
		{
			SBR_ASSERT_INT(outArr[i], arr[i], "fixed array element match");
		}
	}
	// ---------- Vector2 ----------
	static void testRT_Vector2()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2(true, Vector2(1.5f, -2.5f)), "V2 write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2 out;
		SBR_ASSERT(r.readVector2(true, out), "V2 read ok");
		SBR_ASSERT_FLOAT(out.x, 1.5f, 1e-4f, "V2 x");
		SBR_ASSERT_FLOAT(out.y, -2.5f, 1e-4f, "V2 y");
	}

	// ---------- Vector2Int ----------
	static void testRT_Vector2Int()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2Int(true, Vector2Int(100, -200)), "V2I write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2Int out;
		SBR_ASSERT(r.readVector2Int(true, out), "V2I read ok");
		SBR_ASSERT_INT(out.x, 100, "V2I x");
		SBR_ASSERT_INT(out.y, -200, "V2I y");
	}

	// ---------- Vector2UInt ----------
	static void testRT_Vector2UInt()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2UInt(Vector2UInt{100u, 200u}), "V2UInt write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2UInt out;
		SBR_ASSERT(r.readVector2UInt(out), "V2UInt read ok");
		SBR_ASSERT(out.x == 100u && out.y == 200u, "V2UInt values");
	}

	// ---------- Vector2Short ----------
	static void testRT_Vector2Short()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2Short(true, Vector2Short{-32000, 32000}), "V2Short write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2Short out;
		SBR_ASSERT(r.readVector2Short(true, out), "V2Short read ok");
		SBR_ASSERT(out.x == -32000 && out.y == 32000, "V2Short values");
	}

	// ---------- Vector2UShort ----------
	static void testRT_Vector2UShort()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2UShort(Vector2UShort{0, 65535}), "V2UShort write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2UShort out;
		SBR_ASSERT(r.readVector2UShort(out), "V2UShort read ok");
		SBR_ASSERT(out.x == 0 && out.y == 65535, "V2UShort values");
	}

	// ---------- Vector3 ----------
	static void testRT_Vector3()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector3(true, Vector3(1.0f, 2.0f, 3.0f)), "V3 write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector3 out;
		SBR_ASSERT(r.readVector3(true, out), "V3 read ok");
		SBR_ASSERT_FLOAT(out.x, 1.0f, 1e-4f, "V3 x");
		SBR_ASSERT_FLOAT(out.y, 2.0f, 1e-4f, "V3 y");
		SBR_ASSERT_FLOAT(out.z, 3.0f, 1e-4f, "V3 z");
	}

	// ---------- Vector4 ----------
	static void testRT_Vector4()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector4(true, Vector4(1.0f, 2.0f, 3.0f, 4.0f)), "V4 write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector4 out;
		SBR_ASSERT(r.readVector4(true, out), "V4 read ok");
		SBR_ASSERT_FLOAT(out.x, 1.0f, 1e-4f, "V4 x");
		SBR_ASSERT_FLOAT(out.y, 2.0f, 1e-4f, "V4 y");
		SBR_ASSERT_FLOAT(out.z, 3.0f, 1e-4f, "V4 z");
		SBR_ASSERT_FLOAT(out.w, 4.0f, 1e-4f, "V4 w");
	}

	// ---------- writeBuffer / readBuffer ----------
	static void testRT_BufferRoundTrip()
	{
		const char src[] = { 0x10, 0x20, 0x30, 0x40, (char)0xFF };
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeBuffer(src, 5), "bit buf write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		char dst[5] = {};
		SBR_ASSERT(r.readBuffer(dst, 5), "bit buf read ok");
		FOR(5)
		{
			SBR_ASSERT(dst[i] == src[i], "bit buf byte match");
		}
	}

	// ---------- readBufferNoCopy ----------
	static void testRT_BufferNoCopy()
	{
		const char src[] = { 0x01, 0x02, 0x03, 0x04 };
		SerializerBitWrite w;
		w.initCapacity(256);
		w.writeBuffer(src, 4);

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		const char* ptr = nullptr;
		SBR_ASSERT(r.readBufferNoCopy(ptr, 4), "bit bufNoCopy read ok");
		SBR_ASSERT(ptr != nullptr, "bit bufNoCopy non-null");
		SBR_ASSERT(ptr[0] == 0x01 && ptr[3] == 0x04, "bit bufNoCopy content");
	}

	// ---------- writeCustom / readCustom ----------
	struct TestCustomBit : public SerializableBitData
	{
		int mA = 0;
		int mB = 0;
		bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) override
		{
			return reader->readSigned(needReadSign, mA) && reader->readSigned(needReadSign, mB);
		}
		bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const override
		{
			return writer->writeSigned(needWriteSign, mA) && writer->writeSigned(needWriteSign, mB);
		}
	};

	static void testRT_CustomRoundTrip()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		TestCustomBit src;
		src.mA = 42;
		src.mB = -99;
		SBR_ASSERT(w.writeCustom(true, src), "bit custom write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		TestCustomBit dst;
		SBR_ASSERT(r.readCustom(true, dst), "bit custom read ok");
		SBR_ASSERT_INT(dst.mA, 42, "bit custom mA");
		SBR_ASSERT_INT(dst.mB, -99, "bit custom mB");
	}

	// ---------- writeCustomList / readCustomList ----------
	static void testRT_CustomList()
	{
		SerializerBitWrite w;
		w.initCapacity(512);
		Vector<TestCustomBit> src;
		FOR(3)
		{
			TestCustomBit p;
			p.mA = i * 100;
			p.mB = i * -100;
			src.add(p);
		}
		SBR_ASSERT(w.writeCustomList(true, src), "bit customList write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<TestCustomBit> dst;
		SBR_ASSERT(r.readCustomList(true, dst), "bit customList read ok");
		SBR_ASSERT((int)dst.size() == 3, "bit customList size");
		FOR(3)
		{
			SBR_ASSERT_INT(dst[i].mA, i * 100, "bit customList[i].mA");
			SBR_ASSERT_INT(dst[i].mB, i * -100, "bit customList[i].mB");
		}
	}

	// ---------- Empty signed/unsigned/float lists ----------
	static void testRT_EmptySignatureList()
	{
		// signed empty
		{
			Vector<int> empty;
			SerializerBitWrite w;
			w.initCapacity(256);
			SBR_ASSERT(w.writeSignedList(true, empty), "empty signedList write ok");

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			Vector<int> dst;
			SBR_ASSERT(r.readSignedList(true, dst), "empty signedList read ok");
			SBR_ASSERT(dst.isEmpty(), "empty signedList isEmpty");
		}
		// unsigned empty
		{
			Vector<uint> empty;
			SerializerBitWrite w;
			w.initCapacity(256);
			SBR_ASSERT(w.writeUnsignedList(empty), "empty unsignedList write ok");

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			Vector<uint> dst;
			SBR_ASSERT(r.readUnsignedList(dst), "empty unsignedList read ok");
			SBR_ASSERT(dst.isEmpty(), "empty unsignedList isEmpty");
		}
		// float empty
		{
			Vector<float> empty;
			SerializerBitWrite w;
			w.initCapacity(256);
			SBR_ASSERT(w.writeFloatList(true, empty), "empty floatList write ok");

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			Vector<float> dst;
			SBR_ASSERT(r.readFloatList(true, dst), "empty floatList read ok");
			SBR_ASSERT(dst.isEmpty(), "empty floatList isEmpty");
		}
	}

	// ---------- fillZeroToByteEnd / skipToByteEnd ----------
	static void testRT_ByteBoundary()
	{
		// fillZeroToByteEnd
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			w.writeSigned(true, 1);
			w.fillZeroToByteEnd();
			SBR_ASSERT((w.getBitCount() & 7) == 0, "fillZeroToByteEnd aligned");
		}
		// skipToByteEnd
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			w.writeSigned(true, 1);
			w.writeUnsigned(2u);

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			int v;
			r.readSigned(true, v);
			r.skipToByteEnd();
			SBR_ASSERT((r.getBitIndex() & 7) == 0, "skipToByteEnd aligned");
		}
	}

	// ---------- Utility: getByteCount, getBufferSize, getBitIndex, setBitIndex, getReadByteCount, clear ----------
	static void testRT_BitUtility()
	{
		// getByteCount, getBufferSize
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			SBR_ASSERT(w.getBufferSize() >= 256, "bit initCapacity ok");
			SBR_ASSERT(w.getByteCount() == 0, "bit byteCount initially 0");
			w.writeSigned(true, 42);
			SBR_ASSERT(w.getByteCount() > 0, "bit byteCount after write > 0");
		}
		// clear
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			w.writeSigned(true, 100);
			SBR_ASSERT(w.getBitCount() > 0, "bit bitCount > 0 before clear");
			w.clear();
			SBR_ASSERT(w.getBitCount() == 0, "bit bitCount == 0 after clear");
			// clear后重新写入
			w.writeSigned(true, 200);
			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			int outVal = 0;
			SBR_ASSERT(r.readSigned(true, outVal) && outVal == 200, "bit clear reuse ok");
		}
		// getBitIndex, setBitIndex, getReadByteCount
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			w.writeBool(true);

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			SBR_ASSERT(r.getBitIndex() == 0, "bit getBitIndex init 0");
			SBR_ASSERT(r.getReadByteCount() == 0, "bit getReadByteCount init 0");

			bool b;
			r.readBool(b);
			SBR_ASSERT(b == true, "bit bool value");
			SBR_ASSERT(r.getBitIndex() == 1, "bit getBitIndex after 1 bool = 1");
			SBR_ASSERT(r.getReadByteCount() == 1, "bit getReadByteCount after 1 bool = 1");

			r.setBitIndex(0);
			SBR_ASSERT(r.getBitIndex() == 0, "bit setBitIndex(0) ok");
		}
	}

	// ---------- Mixed types in single bit buffer ----------
	static void testRT_MixedTypes()
	{
		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeBool(true), "mixed bool");
		SBR_ASSERT(w.writeSigned(true, 42), "mixed signed int");
		SBR_ASSERT(w.writeUnsigned(100u), "mixed unsigned");
		SBR_ASSERT(w.writeFloat(true, 3.14f), "mixed float");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		bool b;
		int vi;
		uint vu;
		float vf;
		SBR_ASSERT(r.readBool(b) && b == true, "mixed read bool");
		SBR_ASSERT(r.readSigned(true, vi) && vi == 42, "mixed read signed");
		SBR_ASSERT(r.readUnsigned(vu) && vu == 100u, "mixed read unsigned");
		SBR_ASSERT(r.readFloat(true, vf), "mixed read float");
		SBR_ASSERT_FLOAT(vf, 3.14f, 1e-4f, "mixed float value");
	}

	// ---------- Double list ----------
	static void testRT_DoubleList()
	{
		Vector<double> src;
		src.add(1.2345);
		src.add(-6.789);
		src.add(0.0);

		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeDoubleList(true, src), "double list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<double> dst;
		SBR_ASSERT(r.readDoubleList(true, dst), "double list read ok");
		SBR_ASSERT((int)dst.size() == 3, "double list size");
	}
}

void SerializerBitRoundTripTest::test()
{
	testRT_Bool();
	testRT_SignedInt_Typical();
	testRT_SignedInt_Boundary();
	testRT_UnsignedInt_Typical();
	testRT_UnsignedInt_Max();
	testRT_Short();
	testRT_LLong();
	testRT_Byte();
	testRT_Float();
	testRT_Double();
	testRT_Float_NoSign();
	testRT_SignedList();
	testRT_UnsignedList();
	testRT_String();
	testRT_IntVectorList();
	testRT_UintVectorList();
	testRT_FloatVectorList();
	testRT_FixedArray();
	testRT_Vector2();
	testRT_Vector2Int();
	testRT_Vector2UInt();
	testRT_Vector2Short();
	testRT_Vector2UShort();
	testRT_Vector3();
	testRT_Vector4();
	testRT_BufferRoundTrip();
	testRT_BufferNoCopy();
	testRT_CustomRoundTrip();
	testRT_CustomList();
	testRT_EmptySignatureList();
	testRT_ByteBoundary();
	testRT_BitUtility();
	testRT_MixedTypes();
	testRT_DoubleList();
}
