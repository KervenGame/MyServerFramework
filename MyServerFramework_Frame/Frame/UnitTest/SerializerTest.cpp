#include "FrameHeader.h"
#include "SerializerRead.h"
#include "SerializerWrite.h"
#include "SerializerBitRead.h"
#include "SerializerBitWrite.h"

struct SerializerExtTestImpl
{

#define SXT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("SerializerExtTest FAIL: ") + (msg)); } } while(0)
#define SXT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SerializerExtTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SXT_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SerializerExtTest FAIL: ") + (msg) + \
		" | expected=" + LLToS((llong)(b)) + " actual=" + LLToS((llong)(a))); } } while(0)
#define SXT_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((float)(a) - (float)(b)) > 1e-4f) { ERROR(string("SerializerExtTest FAIL: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)

	// ─────────────────────────────────────────────────────────────────────────────
	// Helper: write T to buffer via SerializerWrite, read back via SerializerRead
	// ─────────────────────────────────────────────────────────────────────────────
	template<typename T>
	static bool sxtRoundTrip(const T& val, T& out)
	{
		SerializerWrite w;
		w.initCapacity(64);
		if (!w.write(val))
		{ 
			return false;
		}
		SerializerRead r(w.getBuffer(), w.getDataSize());
		return r.read(out);
	}

	//------------------------------------------------------------------------------
	// POD types: bool, byte, short, int, llong, float, double, uint, ushort, char
	//------------------------------------------------------------------------------
	static void testSxtPodTypes()
	{
		// bool
		{
			bool v1 = true, v2 = false, r1 = false, r2 = true;
			SXT_ASSERT(sxtRoundTrip(v1, r1), "sxt bool true write ok");
			SXT_ASSERT(r1 == true, "sxt bool true read");
			SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt bool false write ok");
			SXT_ASSERT(r2 == false, "sxt bool false read");
		}

		// byte
		{
			byte v = 255, r = 0;
			SXT_ASSERT(sxtRoundTrip(v, r), "sxt byte 255 ok");
			SXT_ASSERT_EQ(r, 255, "sxt byte 255");
			byte z = 0, rz = 99;
			SXT_ASSERT(sxtRoundTrip(z, rz), "sxt byte 0 ok");
			SXT_ASSERT_EQ(rz, 0, "sxt byte 0");
		}

		// short
		{
			short v = -32767, r = 0;
			SXT_ASSERT(sxtRoundTrip(v, r), "sxt short -32767 ok");
			SXT_ASSERT_EQ(r, -32767, "sxt short -32767");
			short v2 = 32767, r2 = 0;
			SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt short max ok");
			SXT_ASSERT_EQ(r2, 32767, "sxt short max=32767");
			short z = 0, rz = 99;
			SXT_ASSERT(sxtRoundTrip(z, rz), "sxt short 0 ok");
			SXT_ASSERT_EQ(rz, 0, "sxt short 0");
		}

		// int
		{
			int v = 2147483647, r = 0;
			SXT_ASSERT(sxtRoundTrip(v, r), "sxt int max ok");
			SXT_ASSERT_EQ(r, 2147483647, "sxt int max");
			int v2 = -2147483647, r2 = 0;
			SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt int min+1 ok");
			SXT_ASSERT_EQ(r2, -2147483647, "sxt int min+1");
			int z = 0, rz = 99;
			SXT_ASSERT(sxtRoundTrip(z, rz), "sxt int 0 ok");
			SXT_ASSERT_EQ(rz, 0, "sxt int 0");
		}

		// uint
		{
			uint v = 4294967295u, r = 0u;
			SXT_ASSERT(sxtRoundTrip(v, r), "sxt uint max ok");
			SXT_ASSERT_EQ_LL((llong)r, (llong)4294967295u, "sxt uint max=4294967295");
			uint v2 = 0u, r2 = 99u;
			SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt uint 0 ok");
			SXT_ASSERT_EQ(r2, 0u, "sxt uint 0");
		}

		// llong
		{
			llong v = 9223372036854775807LL, r = 0LL;
			SXT_ASSERT(sxtRoundTrip(v, r), "sxt llong max ok");
			SXT_ASSERT_EQ_LL(r, 9223372036854775807LL, "sxt llong max");
			llong v2 = -9223372036854775807LL, r2 = 0LL;
			SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt llong min+1 ok");
			SXT_ASSERT_EQ_LL(r2, -9223372036854775807LL, "sxt llong min+1");
		}

		// float
		{
			float v = 3.14159f, r = 0.0f;
			SXT_ASSERT(sxtRoundTrip(v, r), "sxt float pi ok");
			SXT_ASSERT_FLOAT(r, 3.14159f, "sxt float pi");
			float v2 = -1234.5f, r2 = 0.0f;
			SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt float neg ok");
			SXT_ASSERT_FLOAT(r2, -1234.5f, "sxt float -1234.5");
			float z = 0.0f, rz = 99.0f;
			SXT_ASSERT(sxtRoundTrip(z, rz), "sxt float 0 ok");
			SXT_ASSERT_FLOAT(rz, 0.0f, "sxt float 0");
		}

		// double
		{
			double v = 3.141592653589793, r = 0.0;
			SXT_ASSERT(sxtRoundTrip(v, r), "sxt double pi ok");
			SXT_ASSERT((fabsf((float)r - 3.14159f) < 1e-4f), "sxt double pi approx");
			double z = 0.0, rz = 99.0;
			SXT_ASSERT(sxtRoundTrip(z, rz), "sxt double 0 ok");
			SXT_ASSERT((fabs(rz) < 1e-10), "sxt double 0");
		}

		// ushort
		{
			ushort v = 65535, r = 0;
			SXT_ASSERT(sxtRoundTrip(v, r), "sxt ushort max ok");
			SXT_ASSERT_EQ(r, 65535, "sxt ushort max=65535");
		}

		// char
		{
			char v = 'A', r = 0;
			SXT_ASSERT(sxtRoundTrip(v, r), "sxt char A ok");
			SXT_ASSERT_EQ(r, (char)'A', "sxt char A");
			char v2 = -127, r2 = 0;
			SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt char -127 ok");
			SXT_ASSERT_EQ(r2, (char)-127, "sxt char -127");
		}
	}

	//------------------------------------------------------------------------------
	// String round-trip
	//------------------------------------------------------------------------------
	static void testSxtString()
	{
		auto roundTripStr = [](const char* src, string& dst) -> bool
		{
			SerializerWrite w;
			w.initCapacity(128);
			w.writeString(src);
			SerializerRead r(w.getBuffer(), w.getDataSize());
			return r.readString(dst);
		};

		string s;
		SXT_ASSERT(roundTripStr("hello", s), "sxt str hello ok");
		SXT_ASSERT(s == "hello", "sxt str hello value");

		string s2;
		SXT_ASSERT(roundTripStr("", s2), "sxt empty str ok");
		SXT_ASSERT(s2 == "", "sxt empty str value");

		string s3;
		SXT_ASSERT(roundTripStr("test_123_!@#", s3), "sxt special str ok");
		SXT_ASSERT(s3 == "test_123_!@#", "sxt special str value");

		// Long string
		string longStr(200, 'x');
		SerializerWrite w;
		w.initCapacity(256);
		w.writeString(longStr);
		SerializerRead r(w.getBuffer(), w.getDataSize());
		string s4;
		SXT_ASSERT(r.readString(s4), "sxt long str ok");
		SXT_ASSERT(s4 == longStr, "sxt long str value");

		// string overload
		SerializerWrite w2;
		w2.initCapacity(128);
		const string src2 = "overload_test";
		w2.writeString(src2);
		SerializerRead r2(w2.getBuffer(), w2.getDataSize());
		string s5;
		SXT_ASSERT(r2.readString(s5), "sxt string overload ok");
		SXT_ASSERT(s5 == "overload_test", "sxt string overload value");
	}

	//------------------------------------------------------------------------------
	// Vector2 round-trip
	//------------------------------------------------------------------------------
	static void testSxtVector2()
	{
		SerializerWrite w;
		w.initCapacity(64);
		SXT_ASSERT(w.writeVector2(Vector2(3.14f, 2.71f)), "sxt V2 write ok");

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector2 v;
		SXT_ASSERT(r.readVector2(v), "sxt V2 read ok");
		SXT_ASSERT_FLOAT(v.x, 3.14f, "sxt V2 x=3.14");
		SXT_ASSERT_FLOAT(v.y, 2.71f, "sxt V2 y=2.71");

		// Multiple V2
		SerializerWrite w2;
		w2.initCapacity(128);
		w2.writeVector2(Vector2(1.0f, 2.0f));
		w2.writeVector2(Vector2(3.0f, 4.0f));
		w2.writeVector2(Vector2(-1.0f, -2.0f));

		SerializerRead r2(w2.getBuffer(), w2.getDataSize());
		Vector2 va, vb, vc;
		r2.readVector2(va);
		r2.readVector2(vb);
		r2.readVector2(vc);
		SXT_ASSERT_FLOAT(va.x, 1.0f, "sxt V2 multi a.x=1");
		SXT_ASSERT_FLOAT(vb.x, 3.0f, "sxt V2 multi b.x=3");
		SXT_ASSERT_FLOAT(vc.x, -1.0f, "sxt V2 multi c.x=-1");
		SXT_ASSERT_FLOAT(vc.y, -2.0f, "sxt V2 multi c.y=-2");
	}

	//------------------------------------------------------------------------------
	// Vector2Int round-trip
	//------------------------------------------------------------------------------
	static void testSxtVector2Int()
	{
		SerializerWrite w;
		w.initCapacity(64);
		SXT_ASSERT(w.writeVector2Int(Vector2Int(100, -200)), "sxt V2I write ok");

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector2Int v;
		SXT_ASSERT(r.readVector2Int(v), "sxt V2I read ok");
		SXT_ASSERT_EQ(v.x, 100, "sxt V2I x=100");
		SXT_ASSERT_EQ(v.y, -200, "sxt V2I y=-200");

		// Zero
		SerializerWrite w2;
		w2.initCapacity(32);
		w2.writeVector2Int(Vector2Int(0, 0));
		SerializerRead r2(w2.getBuffer(), w2.getDataSize());
		Vector2Int z;
		r2.readVector2Int(z);
		SXT_ASSERT_EQ(z.x, 0, "sxt V2I zero x=0");
		SXT_ASSERT_EQ(z.y, 0, "sxt V2I zero y=0");
	}

	//------------------------------------------------------------------------------
	// Vector3 round-trip
	//------------------------------------------------------------------------------
	static void testSxtVector3()
	{
		SerializerWrite w;
		w.initCapacity(64);
		SXT_ASSERT(w.writeVector3(Vector3(1.1f, 2.2f, 3.3f)), "sxt V3 write ok");

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector3 v;
		SXT_ASSERT(r.readVector3(v), "sxt V3 read ok");
		SXT_ASSERT_FLOAT(v.x, 1.1f, "sxt V3 x=1.1");
		SXT_ASSERT_FLOAT(v.y, 2.2f, "sxt V3 y=2.2");
		SXT_ASSERT_FLOAT(v.z, 3.3f, "sxt V3 z=3.3");

		// Negative values
		SerializerWrite w2;
		w2.initCapacity(64);
		w2.writeVector3(Vector3(-5.0f, 0.0f, 5.0f));
		SerializerRead r2(w2.getBuffer(), w2.getDataSize());
		Vector3 v2;
		r2.readVector3(v2);
		SXT_ASSERT_FLOAT(v2.x, -5.0f, "sxt V3 neg x=-5");
		SXT_ASSERT_FLOAT(v2.z, 5.0f, "sxt V3 pos z=5");
	}

	//------------------------------------------------------------------------------
	// Vector4 round-trip
	//------------------------------------------------------------------------------
	static void testSxtVector4()
	{
		SerializerWrite w;
		w.initCapacity(64);
		SXT_ASSERT(w.writeVector4(Vector4(1.0f, 2.0f, 3.0f, 4.0f)), "sxt V4 write ok");

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector4 v;
		SXT_ASSERT(r.readVector4(v), "sxt V4 read ok");
		SXT_ASSERT_FLOAT(v.x, 1.0f, "sxt V4 x=1");
		SXT_ASSERT_FLOAT(v.y, 2.0f, "sxt V4 y=2");
		SXT_ASSERT_FLOAT(v.z, 3.0f, "sxt V4 z=3");
		SXT_ASSERT_FLOAT(v.w, 4.0f, "sxt V4 w=4");
	}

	//------------------------------------------------------------------------------
	// List round-trip: int, float, byte
	//------------------------------------------------------------------------------
	static void testSxtList()
	{
		// int list
		{
			Vector<int> src;
			src.add(10);
			src.add(-20);
			src.add(300);
			src.add(0);

			SerializerWrite w;
			w.initCapacity(128);
			SXT_ASSERT(w.writeList(src), "sxt int list write ok");

			SerializerRead r(w.getBuffer(), w.getDataSize());
			Vector<int> dst;
			SXT_ASSERT(r.readList(dst), "sxt int list read ok");
			SXT_ASSERT_EQ(dst.size(), 4, "sxt int list size=4");
			SXT_ASSERT_EQ(dst[0], 10, "sxt int list[0]=10");
			SXT_ASSERT_EQ(dst[1], -20, "sxt int list[1]=-20");
			SXT_ASSERT_EQ(dst[2], 300, "sxt int list[2]=300");
			SXT_ASSERT_EQ(dst[3], 0, "sxt int list[3]=0");
		}

		// float list
		{
			Vector<float> src;
			src.add(1.5f);
			src.add(-2.5f);
			src.add(3.14f);

			SerializerWrite w;
			w.initCapacity(128);
			SXT_ASSERT(w.writeList(src), "sxt float list write ok");

			SerializerRead r(w.getBuffer(), w.getDataSize());
			Vector<float> dst;
			SXT_ASSERT(r.readList(dst), "sxt float list read ok");
			SXT_ASSERT_EQ(dst.size(), 3, "sxt float list size=3");
			SXT_ASSERT_FLOAT(dst[0], 1.5f, "sxt float list[0]=1.5");
			SXT_ASSERT_FLOAT(dst[1], -2.5f, "sxt float list[1]=-2.5");
			SXT_ASSERT_FLOAT(dst[2], 3.14f, "sxt float list[2]=3.14");
		}

		// byte list
		{
			Vector<byte> src;
			for (byte i = 0; i < 10; ++i) 
			{ 
				src.add(i);
			}

			SerializerWrite w;
			w.initCapacity(128);
			SXT_ASSERT(w.writeList(src), "sxt byte list write ok");

			SerializerRead r(w.getBuffer(), w.getDataSize());
			Vector<byte> dst;
			SXT_ASSERT(r.readList(dst), "sxt byte list read ok");
			SXT_ASSERT_EQ(dst.size(), 10, "sxt byte list size=10");
			FOR(10)
			{
				SXT_ASSERT_EQ(dst[i], (byte)i, "sxt byte list[i]=i");
			}
		}

		// empty list
		{
			Vector<int> empty;
			SerializerWrite w;
			w.initCapacity(32);
			SXT_ASSERT(w.writeList(empty), "sxt empty list write ok");
			SerializerRead r(w.getBuffer(), w.getDataSize());
			Vector<int> dst;
			SXT_ASSERT(r.readList(dst), "sxt empty list read ok");
			SXT_ASSERT(dst.isEmpty(), "sxt empty list isEmpty");
		}
	}

	//------------------------------------------------------------------------------
	// Mixed types in single buffer
	//------------------------------------------------------------------------------
	static void testSxtMixed()
	{
		SerializerWrite w;
		w.initCapacity(256);
		w.write((int)42);
		w.write((float)3.14f);
		w.write((bool)true);
		w.write((byte)255);
		w.writeString("mixed_test");
		w.write((short)-100);
		w.write((llong)1234567890123LL);

		SerializerRead r(w.getBuffer(), w.getDataSize());
		int i;  
		r.read(i);
		float f;
		r.read(f);
		bool b; 
		r.read(b);
		byte by;
		r.read(by);
		string s;
		r.readString(s);
		short sh;
		r.read(sh);
		llong ll;
		r.read(ll);

		SXT_ASSERT_EQ(i, 42, "mixed int=42");
		SXT_ASSERT_FLOAT(f, 3.14f, "mixed float=3.14");
		SXT_ASSERT(b == true, "mixed bool=true");
		SXT_ASSERT_EQ(by, (byte)255, "mixed byte=255");
		SXT_ASSERT(s == "mixed_test", "mixed str=mixed_test");
		SXT_ASSERT_EQ(sh, -100, "mixed short=-100");
		SXT_ASSERT_EQ_LL(ll, 1234567890123LL, "mixed llong=1234567890123");
	}

	//------------------------------------------------------------------------------
	// getIndex / setIndex / clear
	//------------------------------------------------------------------------------
	static void testSxtIndex()
	{
		SerializerWrite w;
		w.initCapacity(64);
		w.write((int)1);
		w.write((int)2);
		SXT_ASSERT_EQ(w.getDataSize(), (int)(sizeof(int) * 2), "sxt write size=8");

		// reset and rewrite
		w.clear();
		SXT_ASSERT_EQ(w.getDataSize(), 0, "sxt clear size=0");
		w.write((int)99);

		SerializerRead r(w.getBuffer(), w.getDataSize());
		SXT_ASSERT_EQ(r.getIndex(), 0, "sxt read index=0 initially");
		int v = 0;
		r.read(v);
		SXT_ASSERT_EQ(v, 99, "sxt read after clear=99");
		SXT_ASSERT_EQ(r.getIndex(), (int)sizeof(int), "sxt read index=4 after int");

		// setIndex: rewind and re-read
		r.setIndex(0);
		SXT_ASSERT_EQ(r.getIndex(), 0, "sxt setIndex(0) ok");
		int v2 = 0;
		r.read(v2);
		SXT_ASSERT_EQ(v2, 99, "sxt rewind read=99");
	}

	//------------------------------------------------------------------------------
	// Buffer read (raw bytes)
	//------------------------------------------------------------------------------
	static void testSxtBuffer()
	{
		const char src[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
		SerializerWrite w;
		w.initCapacity(32);
		SXT_ASSERT(w.writeBuffer(src, 5), "sxt writeBuffer ok");
		SXT_ASSERT_EQ(w.getDataSize(), 5, "sxt writeBuffer size=5");

		char dst[5] = {};
		SerializerRead r(w.getBuffer(), w.getDataSize());
		SXT_ASSERT(r.readBuffer(dst, 5, 5), "sxt readBuffer ok");
		FOR(5)
		{
			SXT_ASSERT_EQ(dst[i], src[i], "sxt buffer[i] match");
		}
	}

	//------------------------------------------------------------------------------
	// String list
	//------------------------------------------------------------------------------
	static void testSxtStringList()
	{
		Vector<string> src;
		src.add("alpha");
		src.add("beta");
		src.add("gamma");
		src.add("");

		SerializerWrite w;
		w.initCapacity(256);
		SXT_ASSERT(w.writeStringList(src), "sxt string list write ok");

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<string> dst;
		SXT_ASSERT(r.readStringList(dst), "sxt string list read ok");
		SXT_ASSERT_EQ(dst.size(), 4, "sxt string list size=4");
		SXT_ASSERT(dst[0] == "alpha", "sxt strlist[0]=alpha");
		SXT_ASSERT(dst[1] == "beta", "sxt strlist[1]=beta");
		SXT_ASSERT(dst[2] == "gamma", "sxt strlist[2]=gamma");
		SXT_ASSERT(dst[3] == "", "sxt strlist[3]=empty");
	}

	//------------------------------------------------------------------------------
	// Vector2 list / Vector3 list
	//------------------------------------------------------------------------------
	static void testSxtVectorLists()
	{
		// Vector2 list
		{
			Vector<Vector2> src;
			src.add(Vector2(1.0f, 2.0f));
			src.add(Vector2(-3.0f, 4.0f));
			src.add(Vector2(0.0f, 0.0f));

			SerializerWrite w;
			w.initCapacity(128);
			// Use manual approach since writeList takes POD only
			// Write count then each vector
			w.write((int)src.size());
			for (const auto& v : src) 
			{
				w.writeVector2(v); 
			}

			SerializerRead r(w.getBuffer(), w.getDataSize());
			Vector<Vector2> dst;
			SXT_ASSERT(r.readVector2List(dst), "sxt V2 list read ok");
			SXT_ASSERT_EQ(dst.size(), 3, "sxt V2 list size=3");
			SXT_ASSERT_FLOAT(dst[0].x, 1.0f, "sxt V2 list[0].x=1");
			SXT_ASSERT_FLOAT(dst[1].x, -3.0f, "sxt V2 list[1].x=-3");
			SXT_ASSERT_FLOAT(dst[2].x, 0.0f, "sxt V2 list[2].x=0");
		}

		// Vector3 list
		{
			Vector<Vector3> src;
			src.add(Vector3(1.0f, 2.0f, 3.0f));
			src.add(Vector3(-1.0f, -2.0f, -3.0f));

			SerializerWrite w;
			w.initCapacity(128);
			w.write((int)src.size());
			for (const auto& v : src)
			{
				w.writeVector3(v); 
			}

			SerializerRead r(w.getBuffer(), w.getDataSize());
			Vector<Vector3> dst;
			SXT_ASSERT(r.readVector3List(dst), "sxt V3 list read ok");
			SXT_ASSERT_EQ(dst.size(), 2, "sxt V3 list size=2");
			SXT_ASSERT_FLOAT(dst[0].z, 3.0f, "sxt V3 list[0].z=3");
			SXT_ASSERT_FLOAT(dst[1].z, -3.0f, "sxt V3 list[1].z=-3");
		}
	}

	//------------------------------------------------------------------------------
	// Stress: write 1000 ints then read them back
	//------------------------------------------------------------------------------
	static void testSxtStress()
	{
		SerializerWrite w;
		w.initCapacity(4096);
		FOR(1000)
		{
			SXT_ASSERT(w.write(i), "sxt stress write ok");
		}
		SXT_ASSERT_EQ(w.getDataSize(), (int)(1000 * sizeof(int)), "sxt stress size=4000");

		SerializerRead r(w.getBuffer(), w.getDataSize());
		FOR(1000)
		{
			int v = -1;
			SXT_ASSERT(r.read(v), "sxt stress read ok");
			SXT_ASSERT_EQ(v, i, "sxt stress val=i");
		}

		// Alternating int/float
		SerializerWrite w2;
		w2.initCapacity(2048);
		FOR(100)
		{
			w2.write(i);
			w2.write((float)i * 0.5f);
		}
		SerializerRead r2(w2.getBuffer(), w2.getDataSize());
		FOR(100)
		{
			int iv = -1;
			float fv = -1.0f;
			r2.read(iv);
			r2.read(fv);
			SXT_ASSERT_EQ(iv, i, "sxt alt int");
			SXT_ASSERT_FLOAT(fv, i * 0.5f, "sxt alt float");
		}
	}

	//------------------------------------------------------------------------------
	// Round-trip: all POD boundary values
	//------------------------------------------------------------------------------
	static void testSxtBoundaryValues()
	{
		// bool both values
		{ bool a = true, b = false; SXT_ASSERT(sxtRoundTrip(a, b) && b, "boundary bool true"); }
		{ bool a = false, b = true; SXT_ASSERT(sxtRoundTrip(a, b) && !b, "boundary bool false"); }

		// byte 0 and 255
		{ byte a = 0, b = 1; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0, "boundary byte 0"); }
		{ byte a = 255, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 255, "boundary byte 255"); }
		{ byte a = 128, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 128, "boundary byte 128"); }
		{ byte a = 127, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 127, "boundary byte 127"); }

		// short: 0, max, min+1, -1
		{ short a = 0, b = 1; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0, "boundary short 0"); }
		{ short a = 32767, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 32767, "boundary short max"); }
		{ short a = -32767, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == -32767, "boundary short min+1"); }
		{ short a = -1, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == -1, "boundary short -1"); }

		// int: 0, max, min+1, -1, 1
		{ int a = 0, b = 1; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0, "boundary int 0"); }
		{ int a = 2147483647, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 2147483647, "boundary int max"); }
		{ int a = -2147483647, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == -2147483647, "boundary int min+1"); }
		{ int a = -1, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == -1, "boundary int -1"); }
		{ int a = 1, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 1, "boundary int 1"); }

		// llong
		{ llong a = 0LL, b = 1LL; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0LL, "boundary llong 0"); }
		{ llong a = 9223372036854775807LL, b = 0LL; SXT_ASSERT(sxtRoundTrip(a, b) && b == 9223372036854775807LL, "boundary llong max"); }
		{ llong a = -9223372036854775807LL, b = 0LL; SXT_ASSERT(sxtRoundTrip(a, b) && b == -9223372036854775807LL, "boundary llong min+1"); }
		{ llong a = -1LL, b = 0LL; SXT_ASSERT(sxtRoundTrip(a, b) && b == -1LL, "boundary llong -1"); }

		// uint
		{ uint a = 0u, b = 1u; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0u, "boundary uint 0"); }
		{ uint a = 4294967295u, b = 0u; SXT_ASSERT(sxtRoundTrip(a, b) && b == 4294967295u, "boundary uint max"); }

		// ushort
		{ ushort a = 0, b = 1; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0, "boundary ushort 0"); }
		{ ushort a = 65535, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 65535, "boundary ushort max"); }

		// float special values
		{ float a = 0.0f, b = 1.0f; SXT_ASSERT(sxtRoundTrip(a, b), "float 0 roundtrip"); SXT_ASSERT_FLOAT(b, 0.0f, "boundary float 0"); }
		{ float a = -0.0f, b = 1.0f; sxtRoundTrip(a, b); }  // just no crash
		{ float a = 1e38f, b = 0.0f; SXT_ASSERT(sxtRoundTrip(a, b), "float large roundtrip"); SXT_ASSERT_FLOAT(b, 1e38f, "boundary float large"); }
		{ float a = -1e38f, b = 0.0f; SXT_ASSERT(sxtRoundTrip(a, b), "float large neg roundtrip"); SXT_ASSERT_FLOAT(b, -1e38f, "boundary float large neg"); }

		// double
		{ double a = 0.0, b = 1.0; SXT_ASSERT(sxtRoundTrip(a, b), "double 0 roundtrip"); SXT_ASSERT((b >= -1e-12 && b <= 1e-12), "boundary double 0"); }
		{ double a = 3.141592653589793, b = 0.0; SXT_ASSERT(sxtRoundTrip(a, b), "double pi roundtrip"); SXT_ASSERT(fabsf((float)(b - 3.141592653589793)) < 1e-6f, "boundary double pi"); }
	}

	//------------------------------------------------------------------------------
	// Multiple sequential writes to same writer
	//------------------------------------------------------------------------------
	static void testSxtSequentialWrites()
	{
		SerializerWrite w;
		w.initCapacity(256);

		// Write a sequence of different types
		w.write((int)1001);
		w.write((float)3.14f);
		w.write((bool)true);
		w.write((short)(-500));
		w.write((llong)999999999LL);
		w.write((byte)200);
		w.write((ushort)60000);
		w.write((uint)123456789u);
		w.write((double)2.718281828);

		SerializerRead r(w.getBuffer(), w.getDataSize());
		int ri = 0;
		r.read(ri);
		SXT_ASSERT_EQ(ri, 1001, "seq int=1001");
		float rf = 0;
		r.read(rf);
		SXT_ASSERT_FLOAT(rf, 3.14f, "seq float=3.14");
		bool rb = false;
		r.read(rb);
		SXT_ASSERT(rb, "seq bool=true");
		short rs = 0;
		r.read(rs);
		SXT_ASSERT_EQ((int)rs, -500, "seq short=-500");
		llong rll = 0; 
		r.read(rll);  
		SXT_ASSERT_EQ_LL(rll, 999999999LL, "seq llong");
		byte rby = 0;  
		r.read(rby);  
		SXT_ASSERT_EQ((int)rby, 200, "seq byte=200");
		ushort rus = 0; 
		r.read(rus);  
		SXT_ASSERT_EQ((int)rus, 60000, "seq ushort=60000");
		uint rui = 0;  
		r.read(rui);  
		SXT_ASSERT(rui == 123456789u, "seq uint");
		double rd = 0; 
		r.read(rd);   
		SXT_ASSERT(fabsf((float)(rd - 2.718281828)) < 1e-6f, "seq double");
	}

	//------------------------------------------------------------------------------
	// Large string round-trip
	//------------------------------------------------------------------------------
	static void testSxtLargeString()
	{
		// Build a 500-char string
		string big;
		big.reserve(500);
		FOR(500)
		{
			big += (char)('a' + (i % 26));
		}

		// string 不是 POD，用 writeString/readString 专用接口
		{
			SerializerWrite w;
			w.initCapacity(512);
			SXT_ASSERT(w.writeString(big), "large string write ok");
			SerializerRead r(w.getBuffer(), w.getDataSize());
			string out;
			SXT_ASSERT(r.readString(out), "large string read ok");
			SXT_ASSERT_EQ((int)out.size(), 500, "large string size=500");
			SXT_ASSERT(out == big, "large string content match");
		}

		// Empty string
		{
			SerializerWrite w;
			w.initCapacity(16);
			string empty;
			SXT_ASSERT(w.writeString(empty), "empty string write ok");
			SerializerRead r(w.getBuffer(), w.getDataSize());
			string emptyOut;
			SXT_ASSERT(r.readString(emptyOut), "empty string read ok");
			SXT_ASSERT_EQ((int)emptyOut.size(), 0, "empty string size=0");
		}

		// Single char
		{
			SerializerWrite w;
			w.initCapacity(16);
			string sc = "X";
			SXT_ASSERT(w.writeString(sc), "single char string write ok");
			SerializerRead r(w.getBuffer(), w.getDataSize());
			string scOut;
			SXT_ASSERT(r.readString(scOut), "single char string read ok");
			SXT_ASSERT(scOut == "X", "single char string match");
		}
	}

	//------------------------------------------------------------------------------
	// Write/read many small ints sequentially to verify no byte misalignment
	//------------------------------------------------------------------------------
	static void testSxtManySmallInts()
	{
		const int COUNT = 50;
		SerializerWrite w;
		w.initCapacity(COUNT * 4 + 16);
		FOR(COUNT)
		{
			w.write(i * 3 - COUNT);  // mix of neg and pos
		}

		SerializerRead r(w.getBuffer(), w.getDataSize());
		FOR(COUNT)
		{
			int v = 0;
			r.read(v);
			SXT_ASSERT_EQ(v, i * 3 - COUNT, "many small ints");
		}
	}

	//------------------------------------------------------------------------------
	// Write/read boolean array
	//------------------------------------------------------------------------------
	static void testSxtBoolArray()
	{
		const bool src[] = { true, false, true, true, false, false, true, false };
		const int N = 8;
		SerializerWrite w;
		w.initCapacity(64);
		FOR(N)
		{
			w.write(src[i]);
		}

		SerializerRead r(w.getBuffer(), w.getDataSize());
		FOR(N)
		{
			bool v = !src[i];  // start different
			r.read(v);
			SXT_ASSERT(v == src[i], "bool array element");
		}
	}

	//------------------------------------------------------------------------------
	// Write/read mixed list of strings and ints interleaved
	//------------------------------------------------------------------------------
	static void testSxtStringIntInterleaved()
	{
		SerializerWrite w;
		w.initCapacity(256);
		const int PAIRS = 10;
		FOR(PAIRS)
		{
			string s = "item" + IToS(i);
			w.write(i * 100);
			w.writeString(s);
		}

		SerializerRead r(w.getBuffer(), w.getDataSize());
		FOR(PAIRS)
		{
			int iv = -1;
			string sv;
			r.read(iv);
			r.readString(sv);
			SXT_ASSERT_EQ(iv, i * 100, "interleaved int");
			string expected = "item" + IToS(i);
			SXT_ASSERT(sv == expected, "interleaved string");
		}
	}

	//------------------------------------------------------------------------------
	// SerializerExtTest entry point
	//------------------------------------------------------------------------------
	static void test()
	{
		testSxtPodTypes();
		testSxtString();
		testSxtVector2();
		testSxtVector2Int();
		testSxtVector3();
		testSxtVector4();
		testSxtList();
		testSxtMixed();
		testSxtIndex();
		testSxtBuffer();
		testSxtStringList();
		testSxtVectorLists();
		testSxtStress();
		testSxtBoundaryValues();
		testSxtSequentialWrites();
		testSxtLargeString();
		testSxtManySmallInts();
		testSxtBoolArray();
		testSxtStringIntInterleaved();
	}
};
struct SerializerByteTestImpl
{

	// ============================================================
	// SerializerByteTest — SerializerWrite/SerializerRead 全方位测试
	// 覆盖：write<T>/read<T>/writeVector2/3/4/readVector2/3/4/
	//        writeString/readString/writeList/readList/
	//        writeCustom/readCustom/writeCustomList/readCustomList/
	//        writeStringList/readStringList/writeBuffer/readBuffer/
	//        readBufferNoCopy/setIndex/getIndex/getBuffer/getBufferSize/
	//        initCapacity/clear/getDataSize
	// ============================================================

#define SBT_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("SerializerByteTest FAILED: ") + (msg)); }

// ------------------------------------------------------------------
// 辅助：自定义可序列化结构体（继承 SerializableData）
// ------------------------------------------------------------------
	struct TestPoint : public SerializableData
	{
		int x = 0;
		int y = 0;
		int z = 0;

		bool readFromBuffer(SerializerRead* reader) override
		{
			return reader->read(x) && reader->read(y) && reader->read(z);
		}
		bool writeToBuffer(SerializerWrite* serializer) const override
		{
			return serializer->write(x) && serializer->write(y) && serializer->write(z);
		}
		void resetProperty() override { x = 0; y = 0; z = 0; }
		bool operator==(const TestPoint& o) const { return x == o.x && y == o.y && z == o.z; }
	};

	struct TestPlayerInfo : public SerializableData
	{
		int id = 0;
		float hp = 0.0f;
		string name;

		bool readFromBuffer(SerializerRead* reader) override
		{
			return reader->read(id) && reader->read(hp) && reader->readString(name);
		}
		bool writeToBuffer(SerializerWrite* serializer) const override
		{
			return serializer->write(id) && serializer->write(hp) && serializer->writeString(name);
		}
		void resetProperty() override { id = 0; hp = 0.0f; name.clear(); }
	};

	// ------------------------------------------------------------------
	// 基础 POD 类型往返测试
	// ------------------------------------------------------------------
	static void testSB_BoolRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<bool>(true);
		sw.write<bool>(false);
		sw.write<bool>(true);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		bool v1;
		bool v2;
		bool v3;
		SBT_ASSERT(sr.read(v1) && v1 == true, "bool true");
		SBT_ASSERT(sr.read(v2) && v2 == false, "bool false");
		SBT_ASSERT(sr.read(v3) && v3 == true, "bool true again");
	}

	static void testSB_ByteRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<byte>((byte)0);
		sw.write<byte>((byte)127);
		sw.write<byte>((byte)255);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		byte v1;
		byte v2;
		byte v3;
		SBT_ASSERT(sr.read(v1) && v1 == 0, "byte 0");
		SBT_ASSERT(sr.read(v2) && v2 == 127, "byte 127");
		SBT_ASSERT(sr.read(v3) && v3 == 255, "byte 255");
	}

	static void testSB_ShortRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<short>((short)-32767);
		sw.write<short>((short)0);
		sw.write<short>((short)32767);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		short v1;
		short v2;
		short v3;
		SBT_ASSERT(sr.read(v1) && v1 == -32767, "short min");
		SBT_ASSERT(sr.read(v2) && v2 == 0, "short 0");
		SBT_ASSERT(sr.read(v3) && v3 == 32767, "short max");
	}

	static void testSB_UShortRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<ushort>((ushort)0);
		sw.write<ushort>((ushort)65535);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		ushort v1;
		ushort v2;
		SBT_ASSERT(sr.read(v1) && v1 == 0, "ushort 0");
		SBT_ASSERT(sr.read(v2) && v2 == 65535, "ushort 65535");
	}

	static void testSB_IntRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<int>(-2147483647);
		sw.write<int>(0);
		sw.write<int>(2147483647);
		sw.write<int>(12345678);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		int v1;
		int v2;
		int v3;
		int v4;
		SBT_ASSERT(sr.read(v1) && v1 == -2147483647, "int min");
		SBT_ASSERT(sr.read(v2) && v2 == 0, "int 0");
		SBT_ASSERT(sr.read(v3) && v3 == 2147483647, "int max");
		SBT_ASSERT(sr.read(v4) && v4 == 12345678, "int mid");
	}

	static void testSB_UIntRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<uint>(0u);
		sw.write<uint>(4294967295u);
		sw.write<uint>(99999u);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		uint v1;
		uint v2;
		uint v3;
		SBT_ASSERT(sr.read(v1) && v1 == 0u, "uint 0");
		SBT_ASSERT(sr.read(v2) && v2 == 4294967295u, "uint max");
		SBT_ASSERT(sr.read(v3) && v3 == 99999u, "uint mid");
	}

	static void testSB_LLongRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(128);
		sw.write<llong>(-9223372036854775807LL);
		sw.write<llong>(0LL);
		sw.write<llong>(9223372036854775807LL);
		sw.write<llong>(1234567890123456LL);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		llong v1;
		llong v2;
		llong v3;
		llong v4;
		SBT_ASSERT(sr.read(v1) && v1 == -9223372036854775807LL, "llong min");
		SBT_ASSERT(sr.read(v2) && v2 == 0LL, "llong 0");
		SBT_ASSERT(sr.read(v3) && v3 == 9223372036854775807LL, "llong max");
		SBT_ASSERT(sr.read(v4) && v4 == 1234567890123456LL, "llong mid");
	}

	static void testSB_FloatRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(128);
		sw.write<float>(0.0f);
		sw.write<float>(3.14159f);
		sw.write<float>(-99.9f);
		sw.write<float>(1234567.0f);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		float v1;
		float v2;
		float v3;
		float v4;
		SBT_ASSERT(sr.read(v1) && fabsf(v1) < 1e-6f, "float 0");
		SBT_ASSERT(sr.read(v2) && fabsf(v2 - 3.14159f) < 1e-4f, "float pi");
		SBT_ASSERT(sr.read(v3) && fabsf(v3 + 99.9f) < 1e-3f, "float neg");
		SBT_ASSERT(sr.read(v4) && fabsf(v4 - 1234567.0f) < 1.0f, "float large");
	}

	static void testSB_DoubleRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(128);
		sw.write<double>(0.0);
		sw.write<double>(3.14159265358979);
		sw.write<double>(-1.23456789e10);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		double v1;
		double v2;
		double v3;
		SBT_ASSERT(sr.read(v1) && fabs(v1) < 1e-15, "double 0");
		SBT_ASSERT(sr.read(v2) && fabs(v2 - 3.14159265358979) < 1e-10, "double pi");
		SBT_ASSERT(sr.read(v3) && fabs(v3 + 1.23456789e10) < 1.0, "double large neg");
	}

	// ------------------------------------------------------------------
	// 读取重载（不传参数直接返回值）
	// ------------------------------------------------------------------
	static void testSB_ReadReturnValue()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<int>(42);
		sw.write<float>(1.5f);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		int iv = sr.read<int>();
		float fv = sr.read<float>();
		SBT_ASSERT(iv == 42, "readReturn int==42");
		SBT_ASSERT(fabsf(fv - 1.5f) < 1e-5f, "readReturn float==1.5");
	}

	// ------------------------------------------------------------------
	// Vector2 / Vector3 / Vector4
	// ------------------------------------------------------------------
	static void testSB_Vector2RoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		Vector2 in = { 1.0f, 2.5f };
		sw.writeVector2(in);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector2 out;
		SBT_ASSERT(sr.readVector2(out), "readVector2 true");
		SBT_ASSERT(fabsf(out.x - 1.0f) < 1e-5f && fabsf(out.y - 2.5f) < 1e-5f, "vector2 values");
	}

	static void testSB_Vector2IntRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		Vector2Int in = { -100, 200 };
		sw.writeVector2Int(in);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector2Int out;
		SBT_ASSERT(sr.readVector2Int(out), "readVector2Int true");
		SBT_ASSERT(out.x == -100 && out.y == 200, "vector2int values");
	}

	static void testSB_Vector3RoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		Vector3 in = { 1.0f, 2.0f, 3.0f };
		sw.writeVector3(in);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector3 out;
		SBT_ASSERT(sr.readVector3(out), "readVector3 true");
		SBT_ASSERT(fabsf(out.x - 1.0f) < 1e-5f, "vector3 x");
		SBT_ASSERT(fabsf(out.y - 2.0f) < 1e-5f, "vector3 y");
		SBT_ASSERT(fabsf(out.z - 3.0f) < 1e-5f, "vector3 z");
	}

	static void testSB_Vector4RoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		Vector4 in = { 1.0f, 2.0f, 3.0f, 4.0f };
		sw.writeVector4(in);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector4 out;
		SBT_ASSERT(sr.readVector4(out), "readVector4 true");
		SBT_ASSERT(fabsf(out.x - 1.0f) < 1e-5f, "vector4 x");
		SBT_ASSERT(fabsf(out.y - 2.0f) < 1e-5f, "vector4 y");
		SBT_ASSERT(fabsf(out.z - 3.0f) < 1e-5f, "vector4 z");
		SBT_ASSERT(fabsf(out.w - 4.0f) < 1e-5f, "vector4 w");
	}

	static void testSB_MultipleVector3()
	{
		SerializerWrite sw;
		sw.initCapacity(128);
		FOR(5)
		{
			Vector3 v = { (float)i, (float)(i * 2), (float)(i * 3) };
			sw.writeVector3(v);
		}
		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		FOR(5)
		{
			Vector3 out;
			SBT_ASSERT(sr.readVector3(out), "multiVec3 read true");
			SBT_ASSERT(fabsf(out.x - i) < 1e-5f, "multiVec3 x");
			SBT_ASSERT(fabsf(out.y - i * 2) < 1e-5f, "multiVec3 y");
			SBT_ASSERT(fabsf(out.z - i * 3) < 1e-5f, "multiVec3 z");
		}
	}

	// ------------------------------------------------------------------
	// String
	// ------------------------------------------------------------------
	static void testSB_StringRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(256);
		sw.writeString("hello world");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		string s;
		SBT_ASSERT(sr.readString(s), "readString true");
		SBT_ASSERT(s == "hello world", "string value");
	}

	static void testSB_EmptyStringRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.writeString("");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		string s;
		SBT_ASSERT(sr.readString(s), "readString empty true");
		SBT_ASSERT(s.empty(), "empty string");
	}

	static void testSB_MultipleStrings()
	{
		SerializerWrite sw;
		sw.initCapacity(512);
		const char* strs[] = { "alpha", "beta", "gamma", "delta", "epsilon" };
		FOR(5)
		{ 
			sw.writeString(strs[i]);
		}

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		FOR(5)
		{
			string out;
			SBT_ASSERT(sr.readString(out), "multiStr read true");
			SBT_ASSERT(out == strs[i], "multiStr value");
		}
	}

	static void testSB_LongString()
	{
		string longStr(1000, 'X');
		SerializerWrite sw;
		sw.initCapacity(1200);
		sw.writeString(longStr);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		string out;
		SBT_ASSERT(sr.readString(out), "longStr read true");
		SBT_ASSERT(out.size() == 1000, "longStr size");
		SBT_ASSERT(out == longStr, "longStr equal");
	}

	static void testSB_StringWithStdString()
	{
		SerializerWrite sw;
		sw.initCapacity(128);
		string in = "test_std_string";
		sw.writeString(in);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		string out;
		SBT_ASSERT(sr.readString(out) && out == in, "stdString roundtrip");
	}

	// ------------------------------------------------------------------
	// List<POD>
	// ------------------------------------------------------------------
	static void testSB_IntListRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(256);
		Vector<int> src;
		FOR(10)
		{
			src.add(i * 10); 
		}
		SBT_ASSERT(sw.writeList(src), "writeIntList true");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<int> dst;
		SBT_ASSERT(sr.readList(dst), "readIntList true");
		SBT_ASSERT(dst.size() == 10, "intList size==10");
		FOR(10)
		{
			SBT_ASSERT(dst[i] == i * 10, "intList[i]==i*10");
		}
	}

	static void testSB_EmptyListRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		Vector<int> empty;
		SBT_ASSERT(sw.writeList(empty), "writeEmptyList true");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<int> dst;
		SBT_ASSERT(sr.readList(dst), "readEmptyList true");
		SBT_ASSERT(dst.isEmpty(), "emptyList empty");
	}

	static void testSB_FloatListRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(256);
		Vector<float> src;
		FOR(5) { src.add(i * 1.5f); }
		sw.writeList(src);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<float> dst;
		SBT_ASSERT(sr.readList(dst), "floatList read true");
		SBT_ASSERT(dst.size() == 5, "floatList size==5");
		FOR(5)
		{
			SBT_ASSERT(fabsf(dst[i] - i * 1.5f) < 1e-4f, "floatList[i] correct");
		}
	}

	static void testSB_LLongListRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(256);
		Vector<llong> src;
		src.add(1LL); 
		src.add(-9999999999LL); 
		src.add(0LL);
		sw.writeList(src);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<llong> dst;
		SBT_ASSERT(sr.readList(dst), "llongList read true");
		SBT_ASSERT(dst.size() == 3, "llongList size==3");
		SBT_ASSERT(dst[0] == 1LL && dst[1] == -9999999999LL && dst[2] == 0LL, "llongList values");
	}

	// ------------------------------------------------------------------
	// StringList
	// ------------------------------------------------------------------
	static void testSB_StringListRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(512);
		Vector<string> src;
		src.add("one"); src.add("two"); src.add("three");
		SBT_ASSERT(sw.writeStringList(src), "writeStringList true");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<string> dst;
		SBT_ASSERT(sr.readStringList(dst), "readStringList true");
		SBT_ASSERT(dst.size() == 3, "stringList size==3");
		SBT_ASSERT(dst[0] == "one" && dst[1] == "two" && dst[2] == "three", "stringList values");
	}

	static void testSB_EmptyStringList()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		Vector<string> empty;
		sw.writeStringList(empty);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<string> dst;
		SBT_ASSERT(sr.readStringList(dst), "emptyStringList read true");
		SBT_ASSERT(dst.isEmpty(), "emptyStringList empty");
	}

	// ------------------------------------------------------------------
	// writeCustom / readCustom
	// ------------------------------------------------------------------
	static void testSB_CustomRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		TestPoint p1;
		p1.x = 10; 
		p1.y = 20; 
		p1.z = 30;
		SBT_ASSERT(sw.writeCustom(p1), "writeCustom true");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		TestPoint p2;
		SBT_ASSERT(sr.readCustom(p2), "readCustom true");
		SBT_ASSERT(p2.x == 10 && p2.y == 20 && p2.z == 30, "custom values");
	}

	static void testSB_PlayerInfoRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(256);
		TestPlayerInfo pi;
		pi.id = 12345;
		pi.hp = 98.5f; 
		pi.name = "Alice";
		SBT_ASSERT(sw.writeCustom(pi), "writePlayerInfo true");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		TestPlayerInfo out;
		SBT_ASSERT(sr.readCustom(out), "readPlayerInfo true");
		SBT_ASSERT(out.id == 12345, "playerInfo id");
		SBT_ASSERT(fabsf(out.hp - 98.5f) < 1e-4f, "playerInfo hp");
		SBT_ASSERT(out.name == "Alice", "playerInfo name");
	}

	// ------------------------------------------------------------------
	// writeCustomList / readCustomList
	// ------------------------------------------------------------------
	static void testSB_CustomListRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(512);
		Vector<TestPoint> src;
		FOR(5)
		{
			TestPoint p; p.x = i; p.y = i * 2; p.z = i * 3;
			src.add(p);
		}
		SBT_ASSERT(sw.writeCustomList(src), "writeCustomList true");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<TestPoint> dst;
		SBT_ASSERT(sr.readCustomList(dst), "readCustomList true");
		SBT_ASSERT(dst.size() == 5, "customList size==5");
		FOR(5)
		{
			SBT_ASSERT(dst[i].x == i && dst[i].y == i * 2 && dst[i].z == i * 3, "customList[i]");
		}
	}

	static void testSB_EmptyCustomList()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		Vector<TestPoint> empty;
		sw.writeCustomList(empty);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<TestPoint> dst;
		SBT_ASSERT(sr.readCustomList(dst), "emptyCustomList read true");
		SBT_ASSERT(dst.isEmpty(), "emptyCustomList empty");
	}

	// ------------------------------------------------------------------
	// writeBuffer / readBuffer / readBufferNoCopy
	// ------------------------------------------------------------------
	static void testSB_BufferRoundTrip()
	{
		const char raw[] = "Hello, Buffer!";
		const int rawLen = (int)strlen(raw);

		SerializerWrite sw;
		sw.initCapacity(256);
		SBT_ASSERT(sw.writeBuffer(raw, rawLen), "writeBuffer true");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		char out[64] = {};
		SBT_ASSERT(sr.readBuffer(out, 64, rawLen), "readBuffer true");
		SBT_ASSERT(memcmp(out, raw, rawLen) == 0, "buffer content match");
	}

	static void testSB_BufferNoCopy()
	{
		const char raw[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.writeBuffer(raw, 5);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		const char* ptr = nullptr;
		SBT_ASSERT(sr.readBufferNoCopy(ptr, 5), "readBufferNoCopy true");
		SBT_ASSERT(ptr != nullptr, "readBufferNoCopy non-null");
		SBT_ASSERT(memcmp(ptr, raw, 5) == 0, "bufferNoCopy content");
	}

	static void testSB_LargeBuffer()
	{
		const int N = 4096;
		Vector<byte> src;
		FOR(N) { src.add((byte)(i & 0xFF)); }

		SerializerWrite sw;
		sw.initCapacity(N + 16);
		sw.writeBuffer((const char*)src.data(), N);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<byte> dst;
		dst.resize(N);
		SBT_ASSERT(sr.readBuffer((char*)dst.data(), N, N), "largeBuffer read true");
		FOR(N)
		{
			SBT_ASSERT(dst[i] == (byte)(i & 0xFF), "largeBuffer byte[i]");
		}
	}

	// ------------------------------------------------------------------
	// setIndex / getIndex
	// ------------------------------------------------------------------
	static void testSB_SetIndex()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<int>(10);
		sw.write<int>(20);
		sw.write<int>(30);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		// 跳过第一个 int
		sr.setIndex(sizeof(int));
		int v;
		SBT_ASSERT(sr.read(v) && v == 20, "setIndex read at 1==20");
		SBT_ASSERT(sr.getIndex() == 2 * (int)sizeof(int), "setIndex after read");
	}

	static void testSB_GetIndex()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<int>(1);
		sw.write<int>(2); 
		sw.write<int>(3);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		SBT_ASSERT(sr.getIndex() == 0, "getIndex init==0");
		int v;
		sr.read(v);
		SBT_ASSERT(sr.getIndex() == (int)sizeof(int), "getIndex after 1 read");
		sr.read(v); 
		sr.read(v);
		SBT_ASSERT(sr.getIndex() == 3 * (int)sizeof(int), "getIndex after 3 reads");
	}

	static void testSW_SetIndex()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<int>(100);
		sw.write<int>(200);
		// 倒回重写第一个
		sw.setIndex(0);
		sw.write<int>(999);
		sw.setIndex(2 * (int)sizeof(int));

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		int v1;
		int v2;
		SBT_ASSERT(sr.read(v1) && v1 == 999, "sw.setIndex overwrite v1==999");
		SBT_ASSERT(sr.read(v2) && v2 == 200, "sw.setIndex v2==200");
	}

	// ------------------------------------------------------------------
	// getBufferSize / getDataSize / clear / initCapacity
	// ------------------------------------------------------------------
	static void testSB_InitCapacity()
	{
		SerializerWrite sw;
		sw.initCapacity(512);
		SBT_ASSERT(sw.getBufferSize() >= 512, "initCapacity bufferSize>=512");
		SBT_ASSERT(sw.getDataSize() == 0, "initCapacity dataSize==0");
	}

	static void testSB_Clear()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<int>(1); sw.write<int>(2); sw.write<int>(3);
		SBT_ASSERT(sw.getDataSize() == 3 * (int)sizeof(int), "beforeClear dataSize");
		sw.clear();
		SBT_ASSERT(sw.getDataSize() == 0, "afterClear dataSize==0");
	}

	static void testSB_GetBuffer()
	{
		SerializerWrite sw;
		sw.initCapacity(64);
		sw.write<int>(42);
		const char* buf = sw.getBuffer();
		SBT_ASSERT(buf != nullptr, "getBuffer non-null");

		SerializerRead sr(buf, sw.getDataSize());
		SBT_ASSERT(sr.getBuffer() == buf, "sr.getBuffer == sw.getBuffer");
		SBT_ASSERT(sr.getBufferSize() == sw.getDataSize(), "sr.getBufferSize == sw.getDataSize");
	}

	// ------------------------------------------------------------------
	// 混合序列化场景
	// ------------------------------------------------------------------
	static void testSB_MixedTypes()
	{
		SerializerWrite sw;
		sw.initCapacity(512);
		sw.write<int>(42);
		sw.write<float>(3.14f);
		sw.writeString("mixed");
		sw.write<bool>(true);
		sw.write<llong>(9876543210LL);
		Vector2 v2 = { 1.0f, 2.0f };
		sw.writeVector2(v2);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		int i; 
		float f; 
		string s; 
		bool b; 
		llong ll; 
		Vector2 rv2;
		SBT_ASSERT(sr.read(i) && i == 42, "mixed int");
		SBT_ASSERT(sr.read(f) && fabsf(f - 3.14f) < 1e-4f, "mixed float");
		SBT_ASSERT(sr.readString(s) && s == "mixed", "mixed string");
		SBT_ASSERT(sr.read(b) && b == true, "mixed bool");
		SBT_ASSERT(sr.read(ll) && ll == 9876543210LL, "mixed llong");
		SBT_ASSERT(sr.readVector2(rv2), "mixed vec2 read");
		SBT_ASSERT(fabsf(rv2.x - 1.0f) < 1e-5f && fabsf(rv2.y - 2.0f) < 1e-5f, "mixed vec2 vals");
	}

	static void testSB_NestedCustomObjects()
	{
		SerializerWrite sw;
		sw.initCapacity(512);
		TestPoint p1; 
		p1.x = 100; 
		p1.y = 200; 
		p1.z = 300;
		TestPoint p2; 
		p2.x = -1;  
		p2.y = -2;  
		p2.z = -3;
		TestPlayerInfo pi;
		pi.id = 1;
		pi.hp = 50.0f;
		pi.name = "Bob";

		sw.writeCustom(p1);
		sw.writeCustom(p2);
		sw.writeCustom(pi);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		TestPoint op1, op2;
		TestPlayerInfo opi;
		SBT_ASSERT(sr.readCustom(op1), "nested p1 read");
		SBT_ASSERT(op1.x == 100 && op1.y == 200 && op1.z == 300, "nested p1 vals");
		SBT_ASSERT(sr.readCustom(op2), "nested p2 read");
		SBT_ASSERT(op2.x == -1 && op2.y == -2 && op2.z == -3, "nested p2 vals");
		SBT_ASSERT(sr.readCustom(opi), "nested pi read");
		SBT_ASSERT(opi.id == 1 && fabsf(opi.hp - 50.0f) < 1e-4f && opi.name == "Bob", "nested pi vals");
	}

	static void testSB_MultipleListsInterleaved()
	{
		SerializerWrite sw;
		sw.initCapacity(1024);
		Vector<int> ints;
		ints.add(1, 2, 3); 
		Vector<float> floats;
		floats.add(1.1f, 2.2f);
		Vector<string> strs;
		strs.add("a", "b", "c", "d");

		sw.writeList(ints);
		sw.writeList(floats);
		sw.writeStringList(strs);

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<int> ri;
		Vector<float> rf;
		Vector<string> rs;
		SBT_ASSERT(sr.readList(ri) && ri.size() == 3, "interleaved intList");
		SBT_ASSERT(sr.readList(rf) && rf.size() == 2, "interleaved floatList");
		SBT_ASSERT(sr.readStringList(rs) && rs.size() == 4, "interleaved strList");
		SBT_ASSERT(ri[2] == 3, "interleaved int val");
		SBT_ASSERT(fabsf(rf[1] - 2.2f) < 1e-4f, "interleaved float val");
		SBT_ASSERT(rs[3] == "d", "interleaved str val");
	}

	static void testSB_ClearAndReuse()
	{
		SerializerWrite sw;
		sw.initCapacity(256);
		sw.write<int>(111);
		sw.write<int>(222);
		sw.clear();

		sw.write<int>(999);
		SBT_ASSERT(sw.getDataSize() == (int)sizeof(int), "clearReuse dataSize==4");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		int v;
		SBT_ASSERT(sr.read(v) && v == 999, "clearReuse value==999");
	}

	static void testSB_LargeVolume()
	{
		const int N = 2000;
		SerializerWrite sw;
		sw.initCapacity(N * 4 + 64);
		FOR(N) { sw.write<int>(i); }
		SBT_ASSERT(sw.getDataSize() == N * (int)sizeof(int), "largeVolume dataSize");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		FOR(N)
		{
			int v;
			SBT_ASSERT(sr.read(v) && v == i, "largeVolume val[i]");
		}
	}

	static void testSB_StringCharBuffer()
	{
		SerializerWrite sw;
		sw.initCapacity(256);
		sw.writeString("char_buf_test");

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		char buf[64] = {};
		SBT_ASSERT(sr.readString(buf, 64), "readStringCharBuf true");
		SBT_ASSERT(strcmp(buf, "char_buf_test") == 0, "charBuf value match");
	}

	static void testSB_Vector2ListRoundTrip()
	{
		SerializerWrite sw;
		sw.initCapacity(256);
		Vector<Vector2> src;
		FOR(5)
		{
			src.add({ (float)i, (float)(i * 2) });
		}
		// 手动写 count + 数据
		sw.write<int>(src.size());
		for (const Vector2& v : src) 
		{
			sw.writeVector2(v); 
		}

		SerializerRead sr(sw.getBuffer(), sw.getDataSize());
		Vector<Vector2> dst;
		SBT_ASSERT(sr.readVector2List(dst), "readVec2List true");
		SBT_ASSERT(dst.size() == 5, "vec2List size==5");
		FOR(5)
		{
			SBT_ASSERT(fabsf(dst[i].x - i) < 1e-5f, "vec2List x");
			SBT_ASSERT(fabsf(dst[i].y - i * 2) < 1e-5f, "vec2List y");
		}
	}

	// ============================================================
	// 主入口
	// ============================================================
	static void test()
	{
		testSB_BoolRoundTrip();
		testSB_ByteRoundTrip();
		testSB_ShortRoundTrip();
		testSB_UShortRoundTrip();
		testSB_IntRoundTrip();
		testSB_UIntRoundTrip();
		testSB_LLongRoundTrip();
		testSB_FloatRoundTrip();
		testSB_DoubleRoundTrip();
		testSB_ReadReturnValue();
		testSB_Vector2RoundTrip();
		testSB_Vector2IntRoundTrip();
		testSB_Vector3RoundTrip();
		testSB_Vector4RoundTrip();
		testSB_MultipleVector3();
		testSB_StringRoundTrip();
		testSB_EmptyStringRoundTrip();
		testSB_MultipleStrings();
		testSB_LongString();
		testSB_StringWithStdString();
		testSB_IntListRoundTrip();
		testSB_EmptyListRoundTrip();
		testSB_FloatListRoundTrip();
		testSB_LLongListRoundTrip();
		testSB_StringListRoundTrip();
		testSB_EmptyStringList();
		testSB_CustomRoundTrip();
		testSB_PlayerInfoRoundTrip();
		testSB_CustomListRoundTrip();
		testSB_EmptyCustomList();
		testSB_BufferRoundTrip();
		testSB_BufferNoCopy();
		testSB_LargeBuffer();
		testSB_SetIndex();
		testSB_GetIndex();
		testSW_SetIndex();
		testSB_InitCapacity();
		testSB_Clear();
		testSB_GetBuffer();
		testSB_MixedTypes();
		testSB_NestedCustomObjects();
		testSB_MultipleListsInterleaved();
		testSB_ClearAndReuse();
		testSB_LargeVolume();
		testSB_StringCharBuffer();
		testSB_Vector2ListRoundTrip();

	}
};
struct SerializerByteExt2TestImpl
{

#define SBE2_ASSERT(expr) if (!(expr)) { ERROR(string("SerializerByteExt2Test FAILED: ") + (#expr)); }
#define SBE2_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)

	static const int SBE2_BUF = 2048;

	// ========== Vector3Int round-trip ==========

	static void testVector3IntRoundTrip()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		Vector3Int src(10, -20, 30);
		SBE2_ASSERT(w.write(src.x));
		SBE2_ASSERT(w.write(src.y));
		SBE2_ASSERT(w.write(src.z));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector3Int dst;
		SBE2_ASSERT(r.readVector3Int(dst));
		SBE2_ASSERT(dst.x == 10);
		SBE2_ASSERT(dst.y == -20);
		SBE2_ASSERT(dst.z == 30);
	}

	static void testVector3IntZero()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		Vector3Int src(0, 0, 0);
		SBE2_ASSERT(w.write(src.x));
		SBE2_ASSERT(w.write(src.y));
		SBE2_ASSERT(w.write(src.z));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector3Int dst;
		SBE2_ASSERT(r.readVector3Int(dst));
		SBE2_ASSERT(dst.x == 0);
		SBE2_ASSERT(dst.y == 0);
		SBE2_ASSERT(dst.z == 0);
	}

	static void testVector3IntListRoundTrip()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		Vector<Vector3Int> src;
		src.add(Vector3Int(1, 2, 3));
		src.add(Vector3Int(-1, -2, -3));
		src.add(Vector3Int(100, 200, 300));
		int srcCount = (int)src.size();
		SBE2_ASSERT(w.write(srcCount));
		FOR(srcCount)
		{
			SBE2_ASSERT(w.writeVector3(src[i]));
		}
		SerializerRead r(w.getBuffer(), w.getDataSize());
		int dstCount = 0;
		SBE2_ASSERT(r.read(dstCount));
		SBE2_ASSERT(dstCount == 3);
		Vector<Vector3Int> dst;
		FOR(dstCount)
		{
			Vector3Int v;
			SBE2_ASSERT(r.readVector3Int(v));
			dst.add(v);
		}
		SBE2_ASSERT(dst[0].x == 1 && dst[0].y == 2 && dst[0].z == 3);
		SBE2_ASSERT(dst[1].x == -1 && dst[1].y == -2 && dst[1].z == -3);
		SBE2_ASSERT(dst[2].x == 100 && dst[2].y == 200 && dst[2].z == 300);
	}

	static void testVector3IntListEmpty()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		int srcCount = 0;
		SBE2_ASSERT(w.write(srcCount));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		int dstCount = 1;
		SBE2_ASSERT(r.read(dstCount));
		SBE2_ASSERT(dstCount == 0);
	}

	// ========== readBuffer ==========

	static void testReadBufferFixed()
	{
		const char data[] = "HELLO";
		int dataLen = 5;
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		SBE2_ASSERT(w.writeBuffer(data, dataLen));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		char outBuf[16] = {};
		SBE2_ASSERT(r.readBuffer(outBuf, (int)sizeof(outBuf), dataLen));
		SBE2_ASSERT(outBuf[0] == 'H');
		SBE2_ASSERT(outBuf[1] == 'E');
		SBE2_ASSERT(outBuf[4] == 'O');
	}

	static void testReadBufferNoCopy()
	{
		const char data[] = { 0x01, 0x02, 0x03, 0x04 };
		int dataLen = 4;
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		SBE2_ASSERT(w.writeBuffer(data, dataLen));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		const char* ptr = nullptr;
		SBE2_ASSERT(r.readBufferNoCopy(ptr, dataLen));
		SBE2_ASSERT(ptr != nullptr);
		SBE2_ASSERT(ptr[0] == 0x01);
		SBE2_ASSERT(ptr[1] == 0x02);
		SBE2_ASSERT(ptr[3] == 0x04);
	}

	// ========== setIndex / getIndex ==========

	static void testSetIndexGetIndex()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		SBE2_ASSERT(w.write((int)42));
		SBE2_ASSERT(w.write((int)99));
		// getDataSize should be 8
		int dataSize = w.getDataSize();
		SBE2_ASSERT(dataSize == 8);
		// reset index and re-read first int
		SerializerRead r(w.getBuffer(), dataSize);
		int v0 = 0;
		SBE2_ASSERT(r.read(v0));
		SBE2_ASSERT(v0 == 42);
		SBE2_ASSERT(r.getIndex() == 4);
		// setIndex back to 0 and re-read
		r.setIndex(0);
		SBE2_ASSERT(r.getIndex() == 0);
		int v1 = 0;
		SBE2_ASSERT(r.read(v1));
		SBE2_ASSERT(v1 == 42);
	}

	// ========== getBufferSize ==========

	static void testGetBufferSize()
	{
		SerializerWrite w;
		w.initCapacity(512);
		SBE2_ASSERT(w.getBufferSize() == 512);
		SBE2_ASSERT(w.write((int)10));
		SBE2_ASSERT(w.getBufferSize() == 512);
		SerializerRead r(w.getBuffer(), 512);
		SBE2_ASSERT(r.getBufferSize() == 512);
	}

	// ========== read<T>() return value form ==========

	static void testReadReturnValueTemplate()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		SBE2_ASSERT(w.write((float)3.14f));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		float v = r.read<float>();
		SBE2_ASSERT(SBE2_NEAR(v, 3.14f));
	}

	// ========== multiple types sequence ==========

	static void testMultipleTypes()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		SBE2_ASSERT(w.write((int)100));
		SBE2_ASSERT(w.write((float)1.5f));
		SBE2_ASSERT(w.write((bool)true));
		SBE2_ASSERT(w.write((llong)1234567890LL));
		SBE2_ASSERT(w.writeString("abc"));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		int vi = 0;
		float vf = 0.0f;
		bool vb = false;
		llong vl = 0;
		string vs;
		SBE2_ASSERT(r.read(vi));
		SBE2_ASSERT(r.read(vf));
		SBE2_ASSERT(r.read(vb));
		SBE2_ASSERT(r.read(vl));
		SBE2_ASSERT(r.readString(vs));
		SBE2_ASSERT(vi == 100);
		SBE2_ASSERT(SBE2_NEAR(vf, 1.5f));
		SBE2_ASSERT(vb == true);
		SBE2_ASSERT(vl == 1234567890LL);
		SBE2_ASSERT(vs == "abc");
	}

	// ========== ushort round-trip ==========

	static void testUShortRoundTrip()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		SBE2_ASSERT(w.write((ushort)65535));
		SBE2_ASSERT(w.write((ushort)0));
		SBE2_ASSERT(w.write((ushort)1000));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		ushort v0 = 0;
		ushort v1 = 1;
		ushort v2 = 2;
		SBE2_ASSERT(r.read(v0));
		SBE2_ASSERT(r.read(v1));
		SBE2_ASSERT(r.read(v2));
		SBE2_ASSERT(v0 == 65535);
		SBE2_ASSERT(v1 == 0);
		SBE2_ASSERT(v2 == 1000);
	}

	// ========== uint round-trip ==========

	static void testUIntRoundTrip()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		SBE2_ASSERT(w.write((uint)4294967295u));
		SBE2_ASSERT(w.write((uint)0));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		uint v0 = 0;
		uint v1 = 1;
		SBE2_ASSERT(r.read(v0));
		SBE2_ASSERT(r.read(v1));
		SBE2_ASSERT(v0 == 4294967295u);
		SBE2_ASSERT(v1 == 0);
	}

	// ========== Vector2UInt, Vector2Short, Vector2UShort ==========

	static void testVector2UIntRoundTrip()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		// use generic write since no dedicated writeVector2UInt
		Vector2UInt src;
		src.x = 100;
		src.y = 200;
		SBE2_ASSERT(w.write(src.x));
		SBE2_ASSERT(w.write(src.y));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector2UInt dst;
		SBE2_ASSERT(r.read(dst.x));
		SBE2_ASSERT(r.read(dst.y));
		SBE2_ASSERT(dst.x == 100);
		SBE2_ASSERT(dst.y == 200);
	}

	static void testVector2ShortRoundTrip()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		Vector2Short src;
		src.x = -32000;
		src.y = 32000;
		SBE2_ASSERT(w.write(src.x));
		SBE2_ASSERT(w.write(src.y));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector2Short dst;
		SBE2_ASSERT(r.read(dst.x));
		SBE2_ASSERT(r.read(dst.y));
		SBE2_ASSERT(dst.x == -32000);
		SBE2_ASSERT(dst.y == 32000);
	}

	static void testVector2UShortRoundTrip()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		Vector2UShort src;
		src.x = 0;
		src.y = 65535;
		SBE2_ASSERT(w.write(src.x));
		SBE2_ASSERT(w.write(src.y));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector2UShort dst;
		SBE2_ASSERT(r.read(dst.x));
		SBE2_ASSERT(r.read(dst.y));
		SBE2_ASSERT(dst.x == 0);
		SBE2_ASSERT(dst.y == 65535);
	}

	// ========== int list with boundary values ==========

	static void testIntListMixed()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		Vector<int> src;
		src.add(0);
		src.add(-1);
		src.add(2147483647);
		src.add(-2147483647);
		src.add(1);
		SBE2_ASSERT(w.writeList(src));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<int> dst;
		SBE2_ASSERT(r.readList(dst));
		SBE2_ASSERT((int)dst.size() == 5);
		SBE2_ASSERT(dst[0] == 0);
		SBE2_ASSERT(dst[1] == -1);
		SBE2_ASSERT(dst[2] == 2147483647);
		SBE2_ASSERT(dst[3] == -2147483647);
		SBE2_ASSERT(dst[4] == 1);
	}

	// ========== float list boundary ==========

	static void testFloatListBoundary()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		Vector<float> src;
		src.add(0.0f);
		src.add(-0.0f);
		src.add(1e10f);
		src.add(-1e10f);
		SBE2_ASSERT(w.writeList(src));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<float> dst;
		SBE2_ASSERT(r.readList(dst));
		SBE2_ASSERT((int)dst.size() == 4);
		SBE2_ASSERT(SBE2_NEAR(dst[2], 1e10f));
		SBE2_ASSERT(SBE2_NEAR(dst[3], -1e10f));
	}

	// ========== double round-trip ==========

	static void testDoubleRoundTrip()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		SBE2_ASSERT(w.write((double)3.14159265358979));
		SBE2_ASSERT(w.write((double)0.0));
		SBE2_ASSERT(w.write((double)-1.0));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		double v0 = 0.0;
		double v1 = 0.0;
		double v2 = 0.0;
		SBE2_ASSERT(r.read(v0));
		SBE2_ASSERT(r.read(v1));
		SBE2_ASSERT(r.read(v2));
		SBE2_ASSERT(fabsf((float)(v0 - 3.14159265358979)) < 1e-6f);
		SBE2_ASSERT(v1 == 0.0);
		SBE2_ASSERT(v2 == -1.0);
	}

	static void testDoubleListRoundTrip()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		Vector<double> src;
		src.add(1.1);
		src.add(2.2);
		src.add(3.3);
		SBE2_ASSERT(w.writeList(src));
		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<double> dst;
		SBE2_ASSERT(r.readList(dst));
		SBE2_ASSERT((int)dst.size() == 3);
		SBE2_ASSERT(fabsf((float)(dst[0] - 1.1)) < 1e-6f);
		SBE2_ASSERT(fabsf((float)(dst[1] - 2.2)) < 1e-6f);
		SBE2_ASSERT(fabsf((float)(dst[2] - 3.3)) < 1e-6f);
	}

	// ========== bool list ==========

	static void testBoolList()
	{
		SerializerWrite w;
		w.initCapacity(SBE2_BUF);
		Vector<bool> src;
		src.add(true);
		src.add(false);
		src.add(true);
		SBE2_ASSERT(w.writeList(src));
		SerializerRead r(w.getBuffer(), w.getDataSize());
	}

	static void test()
	{
		testVector3IntRoundTrip();
		testVector3IntZero();
		testVector3IntListRoundTrip();
		testVector3IntListEmpty();
		testReadBufferFixed();
		testReadBufferNoCopy();
		testSetIndexGetIndex();
		testGetBufferSize();
		testReadReturnValueTemplate();
		testMultipleTypes();
		testUShortRoundTrip();
		testUIntRoundTrip();
		testVector2UIntRoundTrip();
		testVector2ShortRoundTrip();
		testVector2UShortRoundTrip();
		testIntListMixed();
		testFloatListBoundary();
		testDoubleRoundTrip();
		testDoubleListRoundTrip();
		testBoolList();
	}
};
struct SerializerBitExt2TestImpl
{
#define SBE2_MSG_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("SerializerBitExt2Test FAILED: ") + (msg)); } } while(0)

#define SBE2_F(a, b, msg) \
    do { if (fabsf((a) - (b)) > 1e-2f) { ERROR(string("SerializerBitExt2Test FLOAT: ") + (msg) \
        + " exp=" + FToS(b) + " got=" + FToS(a)); } } while(0)

	static void test()
	{
		testWriteReadVector2();
		testWriteReadVector2Int();
		testWriteReadVector2UInt();
		testWriteReadVector2Short();
		testWriteReadVector2UShort();
		testWriteReadVector3();
		testWriteReadVector4();
		testWriteReadBoolTrue();
		testWriteReadBoolFalse();
		testWriteReadBoolSequence();
		testWriteReadString();
		testWriteReadStringEmpty();
		testWriteReadStringList();
		testWriteReadStringListEmpty();
		testSkipToByteEnd();
		testGetBitIndex();
		testGetReadByteCount();
		testWriteReadBuffer();
		testWriteReadFloatList();
		testWriteReadDoubleList();
		testWriteReadMixedTypes();
		testWriteReadSignedSingle();
		testWriteReadUnsignedSingle();
		testWriteReadUnsignedList();
		testWriteReadSignedList();
		testWriteReadDouble();
		testWriteReadDoublePrecision();
		testMultipleStrings();
		testVector3CloseToZero();
		testVector2IntNegative();
	}

	static void testWriteReadVector2()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector2 v(3.14f, -2.71f);
		SBE2_MSG_ASSERT(w.writeVector2(true, v), "writeVector2 ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector2 vOut;
		SBE2_MSG_ASSERT(r.readVector2(true, vOut), "readVector2 ok");
		SBE2_F(vOut.x, v.x, "Vector2 x");
		SBE2_F(vOut.y, v.y, "Vector2 y");
	}

	static void testWriteReadVector2Int()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector2Int v(100, -200);
		SBE2_MSG_ASSERT(w.writeVector2Int(true, v), "writeVector2Int ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector2Int vOut;
		SBE2_MSG_ASSERT(r.readVector2Int(true, vOut), "readVector2Int ok");
		SBE2_MSG_ASSERT(vOut.x == 100, "Vector2Int x");
		SBE2_MSG_ASSERT(vOut.y == -200, "Vector2Int y");
	}

	static void testWriteReadVector2UInt()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector2UInt v;
		v.x = 12345;
		v.y = 67890;
		SBE2_MSG_ASSERT(w.writeVector2UInt(v), "writeVector2UInt ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector2UInt vOut;
		SBE2_MSG_ASSERT(r.readVector2UInt(vOut), "readVector2UInt ok");
		SBE2_MSG_ASSERT(vOut.x == 12345, "Vector2UInt x");
		SBE2_MSG_ASSERT(vOut.y == 67890, "Vector2UInt y");
	}

	static void testWriteReadVector2Short()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector2Short v((short)500, (short)-300);
		SBE2_MSG_ASSERT(w.writeVector2Short(true, v), "writeVector2Short ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector2Short vOut;
		SBE2_MSG_ASSERT(r.readVector2Short(true, vOut), "readVector2Short ok");
		SBE2_MSG_ASSERT(vOut.x == 500, "Vector2Short x");
		SBE2_MSG_ASSERT(vOut.y == -300, "Vector2Short y");
	}

	static void testWriteReadVector2UShort()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector2UShort v;
		v.x = (ushort)1000;
		v.y = (ushort)2000;
		SBE2_MSG_ASSERT(w.writeVector2UShort(v), "writeVector2UShort ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector2UShort vOut;
		SBE2_MSG_ASSERT(r.readVector2UShort(vOut), "readVector2UShort ok");
		SBE2_MSG_ASSERT(vOut.x == 1000, "Vector2UShort x");
		SBE2_MSG_ASSERT(vOut.y == 2000, "Vector2UShort y");
	}

	static void testWriteReadVector3()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector3 v(1.5f, -2.5f, 3.5f);
		SBE2_MSG_ASSERT(w.writeVector3(true, v), "writeVector3 ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector3 vOut;
		SBE2_MSG_ASSERT(r.readVector3(true, vOut), "readVector3 ok");
		SBE2_F(vOut.x, v.x, "Vector3 x");
		SBE2_F(vOut.y, v.y, "Vector3 y");
		SBE2_F(vOut.z, v.z, "Vector3 z");
	}

	static void testWriteReadVector4()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector4 v;
		v.x = 1.0f;
		v.y = -1.0f;
		v.z = 2.0f;
		v.w = -2.0f;
		SBE2_MSG_ASSERT(w.writeVector4(true, v), "writeVector4 ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector4 vOut;
		SBE2_MSG_ASSERT(r.readVector4(true, vOut), "readVector4 ok");
		SBE2_F(vOut.x, v.x, "Vector4 x");
		SBE2_F(vOut.y, v.y, "Vector4 y");
		SBE2_F(vOut.z, v.z, "Vector4 z");
		SBE2_F(vOut.w, v.w, "Vector4 w");
	}

	static void testWriteReadBoolTrue()
	{
		SerializerBitWrite w;
		w.initCapacity(16);
		SBE2_MSG_ASSERT(w.writeBool(true), "writeBool true");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		bool val = false;
		SBE2_MSG_ASSERT(r.readBool(val), "readBool ok");
		SBE2_MSG_ASSERT(val == true, "bool == true");
	}

	static void testWriteReadBoolFalse()
	{
		SerializerBitWrite w;
		w.initCapacity(16);
		SBE2_MSG_ASSERT(w.writeBool(false), "writeBool false");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		bool val = true;
		SBE2_MSG_ASSERT(r.readBool(val), "readBool ok");
		SBE2_MSG_ASSERT(val == false, "bool == false");
	}

	static void testWriteReadBoolSequence()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		bool seq[] = { true, false, true, true, false, false, true };
		FOR(7)
		{
			w.writeBool(seq[i]);
		}
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		FOR(7)
		{
			bool v = !seq[i];
			SBE2_MSG_ASSERT(r.readBool(v), "readBool seq ok");
			SBE2_MSG_ASSERT(v == seq[i], "bool seq value");
		}
	}

	static void testWriteReadString()
	{
		SerializerBitWrite w;
		w.initCapacity(128);
		SBE2_MSG_ASSERT(w.writeString("HelloBitStr"), "writeString ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		string s;
		SBE2_MSG_ASSERT(r.readString(s), "readString ok");
		SBE2_MSG_ASSERT(s == "HelloBitStr", "string value");
	}

	static void testWriteReadStringEmpty()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		SBE2_MSG_ASSERT(w.writeString(""), "writeString empty ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		string s = "notempty";
		SBE2_MSG_ASSERT(r.readString(s), "readString empty ok");
		SBE2_MSG_ASSERT(s == "", "empty string");
	}

	static void testWriteReadStringList()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		Vector<string> wList;
		wList.add("alpha");
		wList.add("beta");
		wList.add("gamma");
		SBE2_MSG_ASSERT(w.writeStringList(wList), "writeStringList ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<string> rList;
		SBE2_MSG_ASSERT(r.readStringList(rList), "readStringList ok");
		SBE2_MSG_ASSERT(rList.size() == 3, "stringList size==3");
		SBE2_MSG_ASSERT(rList[0] == "alpha", "stringList[0]");
		SBE2_MSG_ASSERT(rList[1] == "beta", "stringList[1]");
		SBE2_MSG_ASSERT(rList[2] == "gamma", "stringList[2]");
	}

	static void testWriteReadStringListEmpty()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector<string> wList;
		SBE2_MSG_ASSERT(w.writeStringList(wList), "writeStringList empty ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<string> rList;
		SBE2_MSG_ASSERT(r.readStringList(rList), "readStringList empty ok");
		SBE2_MSG_ASSERT(rList.size() == 0, "empty stringList size==0");
	}

	static void testSkipToByteEnd()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		w.writeBool(true);
		w.writeBool(false);
		w.writeBool(true);
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		bool v;
		r.readBool(v);
		r.skipToByteEnd();
		SBE2_MSG_ASSERT((r.getBitIndex() % 8) == 0, "skipToByteEnd aligned");
	}

	static void testGetBitIndex()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		SBE2_MSG_ASSERT(r.getBitIndex() == 0, "initial bitIndex==0");
		w.writeBool(true);
		SerializerBitRead r2(w.getBuffer(), w.getByteCount());
		bool v;
		r2.readBool(v);
		SBE2_MSG_ASSERT(r2.getBitIndex() == 1, "after readBool bitIndex==1");
	}

	static void testGetReadByteCount()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		w.writeSigned(false, (int)12345);
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		int v = 0;
		r.readSigned(false, v);
		SBE2_MSG_ASSERT(r.getReadByteCount() >= 1, "getReadByteCount >= 1");
		SBE2_MSG_ASSERT(v == 12345, "readSigned value ok");
	}

	static void testWriteReadBuffer()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		const char src[] = { 0x41, 0x42, 0x43, 0x44 };
		SBE2_MSG_ASSERT(w.writeBuffer(src, 4), "writeBuffer ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		char dst[4] = {};
		SBE2_MSG_ASSERT(r.readBuffer(dst, 4), "readBuffer ok");
		SBE2_MSG_ASSERT(dst[0] == 0x41, "buf[0]");
		SBE2_MSG_ASSERT(dst[1] == 0x42, "buf[1]");
		SBE2_MSG_ASSERT(dst[2] == 0x43, "buf[2]");
		SBE2_MSG_ASSERT(dst[3] == 0x44, "buf[3]");
	}

	static void testWriteReadFloatList()
	{
		SerializerBitWrite w;
		w.initCapacity(128);
		Vector<float> wList;
		wList.add(1.5f);
		wList.add(-2.5f);
		wList.add(3.0f);
		SBE2_MSG_ASSERT(w.writeFloatList(true, wList), "writeFloatList ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<float> rList;
		SBE2_MSG_ASSERT(r.readFloatList(true, rList), "readFloatList ok");
		SBE2_MSG_ASSERT(rList.size() == 3, "floatList size==3");
		SBE2_F(rList[0], 1.5f, "floatList[0]");
		SBE2_F(rList[1], -2.5f, "floatList[1]");
		SBE2_F(rList[2], 3.0f, "floatList[2]");
	}

	static void testWriteReadDoubleList()
	{
		SerializerBitWrite w;
		w.initCapacity(128);
		Vector<double> wList;
		wList.add(1.1234);
		wList.add(-5.6789);
		SBE2_MSG_ASSERT(w.writeDoubleList(true, wList), "writeDoubleList ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<double> rList;
		SBE2_MSG_ASSERT(r.readDoubleList(true, rList), "readDoubleList ok");
		SBE2_MSG_ASSERT(rList.size() == 2, "doubleList size==2");
		SBE2_MSG_ASSERT(fabs(rList[0] - 1.1234) < 1e-3, "doubleList[0]");
		SBE2_MSG_ASSERT(fabs(rList[1] - (-5.6789)) < 1e-3, "doubleList[1]");
	}

	static void testWriteReadMixedTypes()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		w.writeBool(true);
		w.writeSigned(true, (int)42);
		w.writeUnsigned((uint)100u);
		w.writeFloat(true, 3.14f);
		w.writeString("test");

		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		bool b = false;
		SBE2_MSG_ASSERT(r.readBool(b), "mixed readBool");
		SBE2_MSG_ASSERT(b, "mixed bool==true");
		int vi = 0;
		SBE2_MSG_ASSERT(r.readSigned(true, vi), "mixed readSigned");
		SBE2_MSG_ASSERT(vi == 42, "mixed int==42");
		uint vu = 0;
		SBE2_MSG_ASSERT(r.readUnsigned(vu), "mixed readUnsigned");
		SBE2_MSG_ASSERT(vu == 100u, "mixed uint==100");
		float vf = 0.0f;
		SBE2_MSG_ASSERT(r.readFloat(true, vf), "mixed readFloat");
		SBE2_F(vf, 3.14f, "mixed float");
		string vs;
		SBE2_MSG_ASSERT(r.readString(vs), "mixed readString");
		SBE2_MSG_ASSERT(vs == "test", "mixed string");
	}

	static void testWriteReadSignedSingle()
	{
		for (int val : { -1000, -1, 0, 1, 999, 32767 })
		{
			SerializerBitWrite w;
			w.initCapacity(32);
			w.writeSigned(true, val);
			SerializerBitRead r(w.getBuffer(), w.getByteCount());
			int out = 0;
			SBE2_MSG_ASSERT(r.readSigned(true, out), "readSigned single ok");
			SBE2_MSG_ASSERT(out == val, "readSigned single value");
		}
	}

	static void testWriteReadUnsignedSingle()
	{
		for (uint val : { 0u, 1u, 255u, 65535u, 1000000u })
		{
			SerializerBitWrite w;
			w.initCapacity(32);
			w.writeUnsigned(val);
			SerializerBitRead r(w.getBuffer(), w.getByteCount());
			uint out = 0;
			SBE2_MSG_ASSERT(r.readUnsigned(out), "readUnsigned single ok");
			SBE2_MSG_ASSERT(out == val, "readUnsigned single value");
		}
	}

	static void testWriteReadUnsignedList()
	{
		SerializerBitWrite w;
		w.initCapacity(128);
		Vector<uint> wList;
		wList.add(10u);
		wList.add(200u);
		wList.add(3000u);
		wList.add(0u);
		SBE2_MSG_ASSERT(w.writeUnsignedList(wList), "writeUnsignedList ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<uint> rList;
		SBE2_MSG_ASSERT(r.readUnsignedList(rList), "readUnsignedList ok");
		SBE2_MSG_ASSERT(rList.size() == 4, "unsignedList size==4");
		SBE2_MSG_ASSERT(rList[0] == 10u, "uList[0]");
		SBE2_MSG_ASSERT(rList[1] == 200u, "uList[1]");
		SBE2_MSG_ASSERT(rList[2] == 3000u, "uList[2]");
		SBE2_MSG_ASSERT(rList[3] == 0u, "uList[3]");
	}

	static void testWriteReadSignedList()
	{
		SerializerBitWrite w;
		w.initCapacity(256);
		Vector<int> wList;
		wList.add(-100);
		wList.add(0);
		wList.add(100);
		wList.add(-32767);
		wList.add(32767);
		SBE2_MSG_ASSERT(w.writeSignedList(true, wList), "writeSignedList ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<int> rList;
		SBE2_MSG_ASSERT(r.readSignedList(true, rList), "readSignedList ok");
		SBE2_MSG_ASSERT(rList.size() == 5, "signedList size==5");
		SBE2_MSG_ASSERT(rList[0] == -100, "sList[0]");
		SBE2_MSG_ASSERT(rList[1] == 0, "sList[1]");
		SBE2_MSG_ASSERT(rList[2] == 100, "sList[2]");
		SBE2_MSG_ASSERT(rList[3] == -32767, "sList[3]");
		SBE2_MSG_ASSERT(rList[4] == 32767, "sList[4]");
	}

	static void testWriteReadDouble()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		SBE2_MSG_ASSERT(w.writeDouble(true, 3.1415926), "writeDouble ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		double v = 0.0;
		SBE2_MSG_ASSERT(r.readDouble(true, v), "readDouble ok");
		SBE2_MSG_ASSERT(fabs(v - 3.1415926) < 1e-3, "double value");
	}

	static void testWriteReadDoublePrecision()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		double d = -99.9999;
		SBE2_MSG_ASSERT(w.writeDouble(true, d), "writeDouble neg ok");
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		double v = 0.0;
		SBE2_MSG_ASSERT(r.readDouble(true, v), "readDouble neg ok");
		SBE2_MSG_ASSERT(fabs(v - d) < 1e-2, "double neg value");
	}

	static void testMultipleStrings()
	{
		SerializerBitWrite w;
		w.initCapacity(512);
		const char* strs[] = { "first", "second", "third", "fourth", "fifth" };
		FOR(5)
		{
			SBE2_MSG_ASSERT(w.writeString(strs[i]), "write multi string");
		}
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		FOR(5)
		{
			string s;
			SBE2_MSG_ASSERT(r.readString(s), "read multi string");
			SBE2_MSG_ASSERT(s == strs[i], "multi string value");
		}
	}

	static void testVector3CloseToZero()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector3 v(0.001f, 0.0f, -0.001f);
		w.writeVector3(true, v);
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector3 vOut;
		SBE2_MSG_ASSERT(r.readVector3(true, vOut), "readVector3 near zero ok");
		SBE2_F(vOut.y, 0.0f, "Vector3 near-zero y");
	}

	static void testVector2IntNegative()
	{
		SerializerBitWrite w;
		w.initCapacity(64);
		Vector2Int v(-999, -12345);
		w.writeVector2Int(true, v);
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector2Int vOut;
		SBE2_MSG_ASSERT(r.readVector2Int(true, vOut), "readVector2Int neg ok");
		SBE2_MSG_ASSERT(vOut.x == -999, "Vector2Int neg x");
		SBE2_MSG_ASSERT(vOut.y == -12345, "Vector2Int neg y");
	}
};
struct SerializerBitExt3TestImpl
{

#define SBE3_ASSERT(expr) \
	if (!(expr)) { ERROR(string("SerializerBitExt3Test FAILED: ") + (#expr)); }

#define SBE3_NEAR(a, b) (fabsf((float)((a) - (b))) < 1e-3f)

	static const int SBE3_BUF = 4096;

	// ========== writeUnsigned single ==========

	static void testWriteReadUnsignedSingle()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.writeUnsigned((uint)12345));
		SBE3_ASSERT(w.writeUnsigned((ushort)60000));
		SBE3_ASSERT(w.writeUnsigned((byte)255));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		uint v0 = 0;
		ushort v1 = 0;
		byte v2 = 0;
		SBE3_ASSERT(r.readUnsigned(v0));
		SBE3_ASSERT(r.readUnsigned(v1));
		SBE3_ASSERT(r.readUnsigned(v2));
		SBE3_ASSERT(v0 == 12345);
		SBE3_ASSERT(v1 == 60000);
		SBE3_ASSERT(v2 == 255);
	}

	// ========== writeUnsignedList ==========

	static void testWriteReadUnsignedList()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		Vector<uint> src;
		src.add(0);
		src.add(100);
		src.add(4294967295u);
		SBE3_ASSERT(w.writeUnsignedList(src));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<uint> dst;
		SBE3_ASSERT(r.readUnsignedList(dst));
		SBE3_ASSERT((int)dst.size() == 3);
		SBE3_ASSERT(dst[0] == 0);
		SBE3_ASSERT(dst[1] == 100);
		SBE3_ASSERT(dst[2] == 4294967295u);
	}

	// ========== writeSignedList ==========

	static void testWriteReadSignedList()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		Vector<int> src;
		src.add(0);
		src.add(100);
		src.add(-100);
		src.add(2147483647);
		src.add(-2147483647);
		SBE3_ASSERT(w.writeSignedList(true, src));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<int> dst;
		SBE3_ASSERT(r.readSignedList(true, dst));
		SBE3_ASSERT((int)dst.size() == 5);
		SBE3_ASSERT(dst[0] == 0);
		SBE3_ASSERT(dst[1] == 100);
		SBE3_ASSERT(dst[2] == -100);
		SBE3_ASSERT(dst[3] == 2147483647);
		SBE3_ASSERT(dst[4] == -2147483647);
	}

	// ========== writeDoubleList ==========

	static void testWriteReadDoubleList()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		Vector<double> src;
		src.add(0.0);
		src.add(1.2345);
		src.add(-9.876);
		SBE3_ASSERT(w.writeDoubleList(true, src));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<double> dst;
		SBE3_ASSERT(r.readDoubleList(true, dst));
		SBE3_ASSERT((int)dst.size() == 3);
		SBE3_ASSERT(SBE3_NEAR(dst[0], 0.0));
		SBE3_ASSERT(SBE3_NEAR(dst[1], 1.2345));
		SBE3_ASSERT(SBE3_NEAR(dst[2], -9.876));
	}

	// ========== writeBuffer ==========

	static void testWriteReadBuffer()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		const char data[] = { 0x10, 0x20, 0x30, 0x40 };
		SBE3_ASSERT(w.writeBuffer(data, 4));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		char dst[8] = {};
		SBE3_ASSERT(r.readBuffer(dst, 4));
		SBE3_ASSERT(dst[0] == 0x10);
		SBE3_ASSERT(dst[1] == 0x20);
		SBE3_ASSERT(dst[2] == 0x30);
		SBE3_ASSERT(dst[3] == 0x40);
	}

	// ========== writeUnsigned multi-arg ==========

	static void testWriteReadUnsignedMultiArg()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.writeUnsigned((uint)1, (uint)2));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		uint v0 = 0;
		uint v1 = 0;
		SBE3_ASSERT(r.readUnsigned(v0, v1));
		SBE3_ASSERT(v0 == 1);
		SBE3_ASSERT(v1 == 2);
	}

	// ========== writeSigned multi-arg ==========

	static void testWriteReadSignedMultiArg()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.writeSigned(true, (int)10, (int)-20));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		int v0 = 0;
		int v1 = 0;
		SBE3_ASSERT(r.readSigned(true, v0, v1));
		SBE3_ASSERT(v0 == 10);
		SBE3_ASSERT(v1 == -20);
	}

	// ========== writeFloatList ==========

	static void testWriteReadFloatList()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		Vector<float> src;
		src.add(0.0f);
		src.add(1.5f);
		src.add(-2.25f);
		src.add(100.0f);
		SBE3_ASSERT(w.writeFloatList(true, src));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<float> dst;
		SBE3_ASSERT(r.readFloatList(true, dst));
		SBE3_ASSERT((int)dst.size() == 4);
		SBE3_ASSERT(SBE3_NEAR(dst[0], 0.0f));
		SBE3_ASSERT(SBE3_NEAR(dst[1], 1.5f));
		SBE3_ASSERT(SBE3_NEAR(dst[2], -2.25f));
		SBE3_ASSERT(SBE3_NEAR(dst[3], 100.0f));
	}

	// ========== writeStringList ==========

	static void testWriteReadStringList()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		Vector<string> src;
		src.add("alpha");
		src.add("beta");
		src.add("");
		src.add("gamma");
		SBE3_ASSERT(w.writeStringList(src));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector<string> dst;
		SBE3_ASSERT(r.readStringList(dst));
		SBE3_ASSERT((int)dst.size() == 4);
		SBE3_ASSERT(dst[0] == "alpha");
		SBE3_ASSERT(dst[1] == "beta");
		SBE3_ASSERT(dst[2] == "");
		SBE3_ASSERT(dst[3] == "gamma");
	}

	// ========== writeBool sequence ==========

	static void testWriteReadBoolSequence()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.writeBool(true));
		SBE3_ASSERT(w.writeBool(false));
		SBE3_ASSERT(w.writeBool(true));
		SBE3_ASSERT(w.writeBool(true));
		SBE3_ASSERT(w.writeBool(false));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		bool b0 = false;
		bool b1 = true;
		bool b2 = false;
		bool b3 = false;
		bool b4 = true;
		SBE3_ASSERT(r.readBool(b0));
		SBE3_ASSERT(r.readBool(b1));
		SBE3_ASSERT(r.readBool(b2));
		SBE3_ASSERT(r.readBool(b3));
		SBE3_ASSERT(r.readBool(b4));
		SBE3_ASSERT(b0 == true);
		SBE3_ASSERT(b1 == false);
		SBE3_ASSERT(b2 == true);
		SBE3_ASSERT(b3 == true);
		SBE3_ASSERT(b4 == false);
	}

	// ========== getBitCount / getByteCount ==========

	static void testGetBitCount()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.getBitCount() == 0);
		SBE3_ASSERT(w.writeBool(true));
		SBE3_ASSERT(w.getBitCount() == 1);
		SBE3_ASSERT(w.writeBool(false));
		SBE3_ASSERT(w.getBitCount() == 2);
	}

	static void testGetByteCount()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.getByteCount() == 0);
		SBE3_ASSERT(w.writeBool(true));
		// 1 bit => 1 byte
		SBE3_ASSERT(w.getByteCount() == 1);
		SBE3_ASSERT(w.writeSigned(true, (int)100));
		int bc = w.getByteCount();
		SBE3_ASSERT(bc > 0);
	}

	// ========== Vector2UInt / Vector2Short / Vector2UShort ==========

	static void testWriteReadVector2UInt()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		Vector2UInt src;
		src.x = 999;
		src.y = 12345;
		SBE3_ASSERT(w.writeVector2UInt(src));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector2UInt dst;
		SBE3_ASSERT(r.readVector2UInt(dst));
		SBE3_ASSERT(dst.x == 999);
		SBE3_ASSERT(dst.y == 12345);
	}

	static void testWriteReadVector2Short()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		Vector2Short src;
		src.x = -1000;
		src.y = 1000;
		SBE3_ASSERT(w.writeVector2Short(true, src));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector2Short dst;
		SBE3_ASSERT(r.readVector2Short(true, dst));
		SBE3_ASSERT(dst.x == -1000);
		SBE3_ASSERT(dst.y == 1000);
	}

	static void testWriteReadVector2UShort()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		Vector2UShort src;
		src.x = 0;
		src.y = 65535;
		SBE3_ASSERT(w.writeVector2UShort(src));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		Vector2UShort dst;
		SBE3_ASSERT(r.readVector2UShort(dst));
		SBE3_ASSERT(dst.x == 0);
		SBE3_ASSERT(dst.y == 65535);
	}

	// ========== boundary values ==========

	static void testWriteReadUIntBoundary()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.writeUnsigned((uint)0));
		SBE3_ASSERT(w.writeUnsigned((uint)4294967295u));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		uint v0 = 1;
		uint v1 = 0;
		SBE3_ASSERT(r.readUnsigned(v0));
		SBE3_ASSERT(r.readUnsigned(v1));
		SBE3_ASSERT(v0 == 0);
		SBE3_ASSERT(v1 == 4294967295u);
	}

	static void testWriteReadUShortBoundary()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.writeUnsigned((ushort)0));
		SBE3_ASSERT(w.writeUnsigned((ushort)65535));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		ushort v0 = 1;
		ushort v1 = 0;
		SBE3_ASSERT(r.readUnsigned(v0));
		SBE3_ASSERT(r.readUnsigned(v1));
		SBE3_ASSERT(v0 == 0);
		SBE3_ASSERT(v1 == 65535);
	}

	static void testWriteReadShortSigned()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.writeSigned(true, (short)0));
		SBE3_ASSERT(w.writeSigned(true, (short)32767));
		SBE3_ASSERT(w.writeSigned(true, (short)-32767));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		short v0 = 1;
		short v1 = 0;
		short v2 = 0;
		SBE3_ASSERT(r.readSigned(true, v0));
		SBE3_ASSERT(r.readSigned(true, v1));
		SBE3_ASSERT(r.readSigned(true, v2));
		SBE3_ASSERT(v0 == 0);
		SBE3_ASSERT(v1 == 32767);
		SBE3_ASSERT(v2 == -32767);
	}

	static void testWriteReadNegativeLlong()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.writeSigned(true, (llong)-999999999LL));
		SBE3_ASSERT(w.writeSigned(true, (llong)999999999LL));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		llong v0 = 0;
		llong v1 = 0;
		SBE3_ASSERT(r.readSigned(true, v0));
		SBE3_ASSERT(r.readSigned(true, v1));
		SBE3_ASSERT(v0 == -999999999LL);
		SBE3_ASSERT(v1 == 999999999LL);
	}

	static void testWriteReadMixedUnsignedSigned()
	{
		SerializerBitWrite w;
		w.initCapacity(SBE3_BUF);
		SBE3_ASSERT(w.writeUnsigned((uint)42));
		SBE3_ASSERT(w.writeSigned(true, (int)-7));
		SBE3_ASSERT(w.writeBool(true));
		SBE3_ASSERT(w.writeUnsigned((ushort)500));
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		uint vu = 0;
		int vi = 0;
		bool vb = false;
		ushort vs = 0;
		SBE3_ASSERT(r.readUnsigned(vu));
		SBE3_ASSERT(r.readSigned(true, vi));
		SBE3_ASSERT(r.readBool(vb));
		SBE3_ASSERT(r.readUnsigned(vs));
		SBE3_ASSERT(vu == 42);
		SBE3_ASSERT(vi == -7);
		SBE3_ASSERT(vb == true);
		SBE3_ASSERT(vs == 500);
	}

	static void test()
	{
		testWriteReadUnsignedSingle();
		testWriteReadUnsignedList();
		testWriteReadSignedList();
		testWriteReadDoubleList();
		testWriteReadBuffer();
		testWriteReadUnsignedMultiArg();
		testWriteReadSignedMultiArg();
		testWriteReadFloatList();
		testWriteReadStringList();
		testWriteReadBoolSequence();
		testGetBitCount();
		testGetByteCount();
		testWriteReadVector2UInt();
		testWriteReadVector2Short();
		testWriteReadVector2UShort();
		testWriteReadUIntBoundary();
		testWriteReadUShortBoundary();
		testWriteReadShortSigned();
		testWriteReadNegativeLlong();
		testWriteReadMixedUnsignedSigned();
	}
};
void SerializerTest::test()
{
	SerializerExtTestImpl::test();
	SerializerByteTestImpl::test();
	SerializerByteExt2TestImpl::test();
	SerializerBitExt2TestImpl::test();
	SerializerBitExt3TestImpl::test();
}
