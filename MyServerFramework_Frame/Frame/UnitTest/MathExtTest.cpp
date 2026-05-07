#include "FrameHeader.h"

#define MATH_ET_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MathExtTest FAIL: ") + (msg)); } } while(0)
#define MET_ASSERT_EQ_INT(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MathExtTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define MET_ASSERT_EQ_UINT(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MathExtTest FAIL: ") + (msg) + \
		" | expected=" + UIToS((uint)(b)) + " actual=" + UIToS((uint)(a))); } } while(0)
#define MET_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((float)(a) - (float)(b)) > 1e-4f) { ERROR(string("MathExtTest FAIL: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)

//------------------------------------------------------------------------------
// Vector2Short
//------------------------------------------------------------------------------
static void testVector2Short()
{
	// constructor + member access
	const Vector2Short a((short)3, (short)4);
	MET_ASSERT_EQ_INT(a.x, 3, "V2S ctor x");
	MET_ASSERT_EQ_INT(a.y, 4, "V2S ctor y");

	// default ctor == 0
	const Vector2Short zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V2S default x=0");
	MET_ASSERT_EQ_INT(zero.y, 0, "V2S default y=0");

	// operator+
	const Vector2Short b((short)1, (short)2);
	const Vector2Short add = a + b;
	MET_ASSERT_EQ_INT(add.x, 4, "V2S add x");
	MET_ASSERT_EQ_INT(add.y, 6, "V2S add y");

	// operator-
	const Vector2Short sub = a - b;
	MET_ASSERT_EQ_INT(sub.x, 2, "V2S sub x");
	MET_ASSERT_EQ_INT(sub.y, 2, "V2S sub y");

	// operator* scalar
	const Vector2Short scaled = a * (short)2;
	MET_ASSERT_EQ_INT(scaled.x, 6, "V2S scale x");
	MET_ASSERT_EQ_INT(scaled.y, 8, "V2S scale y");

	// unary -
	const Vector2Short neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -3, "V2S neg x");
	MET_ASSERT_EQ_INT(neg.y, -4, "V2S neg y");

	// operator+=
	Vector2Short acc((short)5, (short)6);
	acc += Vector2Short((short)1, (short)2);
	MET_ASSERT_EQ_INT(acc.x, 6, "V2S += x");
	MET_ASSERT_EQ_INT(acc.y, 8, "V2S += y");

	// operator-=
	acc -= Vector2Short((short)3, (short)3);
	MET_ASSERT_EQ_INT(acc.x, 3, "V2S -= x");
	MET_ASSERT_EQ_INT(acc.y, 5, "V2S -= y");

	// operator==
	const Vector2Short c((short)3, (short)4);
	MATH_ET_ASSERT(a == c, "V2S == true");
	MATH_ET_ASSERT(!(a == b), "V2S == false");

	// operator!=
	MATH_ET_ASSERT(a != b, "V2S != true");
	MATH_ET_ASSERT(!(a != c), "V2S != false");

	// operator< (compares sum)
	MATH_ET_ASSERT(b < a, "V2S < : (1+2)<(3+4)");
	MATH_ET_ASSERT(!(a < b), "V2S < false: (3+4)>=(1+2)");

	// clear
	Vector2Short d((short)10, (short)20);
	d.clear();
	MET_ASSERT_EQ_INT(d.x, 0, "V2S clear x=0");
	MET_ASSERT_EQ_INT(d.y, 0, "V2S clear y=0");
}

//------------------------------------------------------------------------------
// Vector2UInt
//------------------------------------------------------------------------------
static void testVector2UInt()
{
	// constructor
	const Vector2UInt a(10u, 20u);
	MET_ASSERT_EQ_UINT(a.x, 10u, "V2UI ctor x");
	MET_ASSERT_EQ_UINT(a.y, 20u, "V2UI ctor y");

	// default ctor
	const Vector2UInt zero;
	MET_ASSERT_EQ_UINT(zero.x, 0u, "V2UI default x=0");
	MET_ASSERT_EQ_UINT(zero.y, 0u, "V2UI default y=0");

	// operator+
	const Vector2UInt b(5u, 3u);
	const Vector2UInt add = a + b;
	MET_ASSERT_EQ_UINT(add.x, 15u, "V2UI add x");
	MET_ASSERT_EQ_UINT(add.y, 23u, "V2UI add y");

	// operator-
	const Vector2UInt sub = a - b;
	MET_ASSERT_EQ_UINT(sub.x, 5u, "V2UI sub x");
	MET_ASSERT_EQ_UINT(sub.y, 17u, "V2UI sub y");

	// operator+=
	Vector2UInt acc(100u, 200u);
	acc += Vector2UInt(50u, 75u);
	MET_ASSERT_EQ_UINT(acc.x, 150u, "V2UI += x");
	MET_ASSERT_EQ_UINT(acc.y, 275u, "V2UI += y");

	// operator-=
	acc -= Vector2UInt(30u, 25u);
	MET_ASSERT_EQ_UINT(acc.x, 120u, "V2UI -= x");
	MET_ASSERT_EQ_UINT(acc.y, 250u, "V2UI -= y");

	// operator==
	const Vector2UInt c(10u, 20u);
	MATH_ET_ASSERT(a == c, "V2UI == true");
	MATH_ET_ASSERT(!(a == b), "V2UI == false");

	// operator!=
	MATH_ET_ASSERT(a != b, "V2UI != true");
	MATH_ET_ASSERT(!(a != c), "V2UI != false");

	// operator< (sum comparison)
	MATH_ET_ASSERT(b < a, "V2UI < : (5+3)<(10+20)");

	// clear
	Vector2UInt d(9u, 8u);
	d.clear();
	MET_ASSERT_EQ_UINT(d.x, 0u, "V2UI clear x=0");
	MET_ASSERT_EQ_UINT(d.y, 0u, "V2UI clear y=0");
}

//------------------------------------------------------------------------------
// Vector2UShort
//------------------------------------------------------------------------------
static void testVector2UShort()
{
	// constructor
	const Vector2UShort a((ushort)7, (ushort)9);
	MET_ASSERT_EQ_INT(a.x, 7, "V2US ctor x");
	MET_ASSERT_EQ_INT(a.y, 9, "V2US ctor y");

	// default ctor
	const Vector2UShort zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V2US default x=0");
	MET_ASSERT_EQ_INT(zero.y, 0, "V2US default y=0");

	// operator+
	const Vector2UShort b((ushort)3, (ushort)1);
	const Vector2UShort add = a + b;
	MET_ASSERT_EQ_INT(add.x, 10, "V2US add x");
	MET_ASSERT_EQ_INT(add.y, 10, "V2US add y");

	// operator-
	const Vector2UShort sub = a - b;
	MET_ASSERT_EQ_INT(sub.x, 4, "V2US sub x");
	MET_ASSERT_EQ_INT(sub.y, 8, "V2US sub y");

	// operator* scalar
	const Vector2UShort scaled = a * (short)3;
	MET_ASSERT_EQ_INT(scaled.x, 21, "V2US scale x");
	MET_ASSERT_EQ_INT(scaled.y, 27, "V2US scale y");

	// operator+=
	Vector2UShort acc((ushort)10, (ushort)20);
	acc += Vector2UShort((ushort)5, (ushort)5);
	MET_ASSERT_EQ_INT(acc.x, 15, "V2US += x");
	MET_ASSERT_EQ_INT(acc.y, 25, "V2US += y");

	// operator-=
	acc -= Vector2UShort((ushort)5, (ushort)5);
	MET_ASSERT_EQ_INT(acc.x, 10, "V2US -= x");
	MET_ASSERT_EQ_INT(acc.y, 20, "V2US -= y");

	// operator==
	const Vector2UShort c((ushort)7, (ushort)9);
	MATH_ET_ASSERT(a == c, "V2US == true");
	MATH_ET_ASSERT(!(a == b), "V2US == false");

	// operator!=
	MATH_ET_ASSERT(a != b, "V2US != true");
	MATH_ET_ASSERT(!(a != c), "V2US != false");

	// operator< (sum comparison)
	MATH_ET_ASSERT(b < a, "V2US < : (3+1)<(7+9)");

	// clear
	Vector2UShort d((ushort)50, (ushort)60);
	d.clear();
	MET_ASSERT_EQ_INT(d.x, 0, "V2US clear x=0");
	MET_ASSERT_EQ_INT(d.y, 0, "V2US clear y=0");
}

//------------------------------------------------------------------------------
// Vector3Int
//------------------------------------------------------------------------------
static void testVector3Int()
{
	// constructor
	const Vector3Int a(1, 2, 3);
	MET_ASSERT_EQ_INT(a.x, 1, "V3I ctor x");
	MET_ASSERT_EQ_INT(a.y, 2, "V3I ctor y");
	MET_ASSERT_EQ_INT(a.z, 3, "V3I ctor z");

	// default ctor
	const Vector3Int zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V3I default x=0");
	MET_ASSERT_EQ_INT(zero.y, 0, "V3I default y=0");
	MET_ASSERT_EQ_INT(zero.z, 0, "V3I default z=0");

	// operator+
	const Vector3Int b(4, 5, 6);
	const Vector3Int add = a + b;
	MET_ASSERT_EQ_INT(add.x, 5, "V3I add x");
	MET_ASSERT_EQ_INT(add.y, 7, "V3I add y");
	MET_ASSERT_EQ_INT(add.z, 9, "V3I add z");

	// operator-
	const Vector3Int sub = b - a;
	MET_ASSERT_EQ_INT(sub.x, 3, "V3I sub x");
	MET_ASSERT_EQ_INT(sub.y, 3, "V3I sub y");
	MET_ASSERT_EQ_INT(sub.z, 3, "V3I sub z");

	// operator* scalar
	const Vector3Int scaled = a * 3;
	MET_ASSERT_EQ_INT(scaled.x, 3, "V3I scale x");
	MET_ASSERT_EQ_INT(scaled.y, 6, "V3I scale y");
	MET_ASSERT_EQ_INT(scaled.z, 9, "V3I scale z");

	// unary -
	const Vector3Int neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -1, "V3I neg x");
	MET_ASSERT_EQ_INT(neg.y, -2, "V3I neg y");
	MET_ASSERT_EQ_INT(neg.z, -3, "V3I neg z");

	// operator+=
	Vector3Int acc(10, 20, 30);
	acc += Vector3Int(5, 5, 5);
	MET_ASSERT_EQ_INT(acc.x, 15, "V3I += x");
	MET_ASSERT_EQ_INT(acc.y, 25, "V3I += y");
	MET_ASSERT_EQ_INT(acc.z, 35, "V3I += z");

	// operator-=
	acc -= Vector3Int(5, 5, 5);
	MET_ASSERT_EQ_INT(acc.x, 10, "V3I -= x");
	MET_ASSERT_EQ_INT(acc.y, 20, "V3I -= y");
	MET_ASSERT_EQ_INT(acc.z, 30, "V3I -= z");

	// operator==
	const Vector3Int c(1, 2, 3);
	MATH_ET_ASSERT(a == c, "V3I == true");
	MATH_ET_ASSERT(!(a == b), "V3I == false");

	// operator!=
	MATH_ET_ASSERT(a != b, "V3I != true");
	MATH_ET_ASSERT(!(a != c), "V3I != false");

	// operator< (sum comparison)
	MATH_ET_ASSERT(a < b, "V3I < : 1+2+3 < 4+5+6");
	MATH_ET_ASSERT(!(b < a), "V3I < false");

	// clear
	Vector3Int d(7, 8, 9);
	d.clear();
	MET_ASSERT_EQ_INT(d.x, 0, "V3I clear x=0");
	MET_ASSERT_EQ_INT(d.y, 0, "V3I clear y=0");
	MET_ASSERT_EQ_INT(d.z, 0, "V3I clear z=0");
}

//------------------------------------------------------------------------------
// Vector4
//------------------------------------------------------------------------------
static void testVector4()
{
	// constructor
	const Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	MET_ASSERT_FLOAT(a.x, 1.0f, "V4 ctor x");
	MET_ASSERT_FLOAT(a.y, 2.0f, "V4 ctor y");
	MET_ASSERT_FLOAT(a.z, 3.0f, "V4 ctor z");
	MET_ASSERT_FLOAT(a.w, 4.0f, "V4 ctor w");

	// default ctor
	const Vector4 zero;
	MET_ASSERT_FLOAT(zero.x, 0.0f, "V4 default x=0");
	MET_ASSERT_FLOAT(zero.y, 0.0f, "V4 default y=0");
	MET_ASSERT_FLOAT(zero.z, 0.0f, "V4 default z=0");
	MET_ASSERT_FLOAT(zero.w, 0.0f, "V4 default w=0");

	// operator+
	const Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
	const Vector4 add = a + b;
	MET_ASSERT_FLOAT(add.x, 6.0f, "V4 add x");
	MET_ASSERT_FLOAT(add.y, 8.0f, "V4 add y");
	MET_ASSERT_FLOAT(add.z, 10.0f, "V4 add z");
	MET_ASSERT_FLOAT(add.w, 12.0f, "V4 add w");

	// operator-
	const Vector4 sub = b - a;
	MET_ASSERT_FLOAT(sub.x, 4.0f, "V4 sub x");
	MET_ASSERT_FLOAT(sub.y, 4.0f, "V4 sub y");
	MET_ASSERT_FLOAT(sub.z, 4.0f, "V4 sub z");
	MET_ASSERT_FLOAT(sub.w, 4.0f, "V4 sub w");

	// unary -
	const Vector4 neg = -a;
	MET_ASSERT_FLOAT(neg.x, -1.0f, "V4 neg x");
	MET_ASSERT_FLOAT(neg.y, -2.0f, "V4 neg y");
	MET_ASSERT_FLOAT(neg.z, -3.0f, "V4 neg z");
	MET_ASSERT_FLOAT(neg.w, -4.0f, "V4 neg w");

	// operator+= (fixed bug: was using -= before)
	Vector4 acc(1.0f, 1.0f, 1.0f, 1.0f);
	acc += Vector4(2.0f, 3.0f, 4.0f, 5.0f);
	MET_ASSERT_FLOAT(acc.x, 3.0f, "V4 += x");
	MET_ASSERT_FLOAT(acc.y, 4.0f, "V4 += y");
	MET_ASSERT_FLOAT(acc.z, 5.0f, "V4 += z");
	MET_ASSERT_FLOAT(acc.w, 6.0f, "V4 += w");

	// operator-=
	acc -= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	MET_ASSERT_FLOAT(acc.x, 2.0f, "V4 -= x");
	MET_ASSERT_FLOAT(acc.y, 3.0f, "V4 -= y");
	MET_ASSERT_FLOAT(acc.z, 4.0f, "V4 -= z");
	MET_ASSERT_FLOAT(acc.w, 5.0f, "V4 -= w");

	// clear
	Vector4 d(9.0f, 8.0f, 7.0f, 6.0f);
	d.clear();
	MET_ASSERT_FLOAT(d.x, 0.0f, "V4 clear x=0");
	MET_ASSERT_FLOAT(d.y, 0.0f, "V4 clear y=0");
	MET_ASSERT_FLOAT(d.z, 0.0f, "V4 clear z=0");
	MET_ASSERT_FLOAT(d.w, 0.0f, "V4 clear w=0");

	// -= semantics are preserved
	Vector4 e(10.0f, 10.0f, 10.0f, 10.0f);
	e -= Vector4(3.0f, 4.0f, 5.0f, 6.0f);
	MET_ASSERT_FLOAT(e.x, 7.0f, "V4 -= semantics x");
	MET_ASSERT_FLOAT(e.y, 6.0f, "V4 -= semantics y");
	MET_ASSERT_FLOAT(e.z, 5.0f, "V4 -= semantics z");
	MET_ASSERT_FLOAT(e.w, 4.0f, "V4 -= semantics w");
}

//------------------------------------------------------------------------------
// Vector4Int
//------------------------------------------------------------------------------
static void testVector4Int()
{
	// constructor
	const Vector4Int a(1, 2, 3, 4);
	MET_ASSERT_EQ_INT(a.x, 1, "V4I ctor x");
	MET_ASSERT_EQ_INT(a.y, 2, "V4I ctor y");
	MET_ASSERT_EQ_INT(a.z, 3, "V4I ctor z");
	MET_ASSERT_EQ_INT(a.w, 4, "V4I ctor w");

	// default ctor
	const Vector4Int zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V4I default x=0");
	MET_ASSERT_EQ_INT(zero.y, 0, "V4I default y=0");
	MET_ASSERT_EQ_INT(zero.z, 0, "V4I default z=0");
	MET_ASSERT_EQ_INT(zero.w, 0, "V4I default w=0");

	// operator+
	const Vector4Int b(10, 20, 30, 40);
	const Vector4Int add = a + b;
	MET_ASSERT_EQ_INT(add.x, 11, "V4I add x");
	MET_ASSERT_EQ_INT(add.y, 22, "V4I add y");
	MET_ASSERT_EQ_INT(add.z, 33, "V4I add z");
	MET_ASSERT_EQ_INT(add.w, 44, "V4I add w");

	// operator-
	const Vector4Int sub = b - a;
	MET_ASSERT_EQ_INT(sub.x, 9,  "V4I sub x");
	MET_ASSERT_EQ_INT(sub.y, 18, "V4I sub y");
	MET_ASSERT_EQ_INT(sub.z, 27, "V4I sub z");
	MET_ASSERT_EQ_INT(sub.w, 36, "V4I sub w");

	// unary -
	const Vector4Int neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -1, "V4I neg x");
	MET_ASSERT_EQ_INT(neg.y, -2, "V4I neg y");
	MET_ASSERT_EQ_INT(neg.z, -3, "V4I neg z");
	MET_ASSERT_EQ_INT(neg.w, -4, "V4I neg w");

	// operator+=
	Vector4Int acc(5, 5, 5, 5);
	acc += Vector4Int(1, 2, 3, 4);
	MET_ASSERT_EQ_INT(acc.x, 6,  "V4I += x");
	MET_ASSERT_EQ_INT(acc.y, 7,  "V4I += y");
	MET_ASSERT_EQ_INT(acc.z, 8,  "V4I += z");
	MET_ASSERT_EQ_INT(acc.w, 9,  "V4I += w");

	// operator-=
	acc -= Vector4Int(1, 1, 1, 1);
	MET_ASSERT_EQ_INT(acc.x, 5, "V4I -= x");
	MET_ASSERT_EQ_INT(acc.y, 6, "V4I -= y");
	MET_ASSERT_EQ_INT(acc.z, 7, "V4I -= z");
	MET_ASSERT_EQ_INT(acc.w, 8, "V4I -= w");

	// clear
	Vector4Int d(9, 8, 7, 6);
	d.clear();
	MET_ASSERT_EQ_INT(d.x, 0, "V4I clear x=0");
	MET_ASSERT_EQ_INT(d.y, 0, "V4I clear y=0");
	MET_ASSERT_EQ_INT(d.z, 0, "V4I clear z=0");
	MET_ASSERT_EQ_INT(d.w, 0, "V4I clear w=0");

	// large positive values
	const Vector4Int big(1000, 2000, 3000, 4000);
	const Vector4Int bigAdd = big + Vector4Int(1, 2, 3, 4);
	MET_ASSERT_EQ_INT(bigAdd.x, 1001, "V4I big add x");
	MET_ASSERT_EQ_INT(bigAdd.w, 4004, "V4I big add w");

	// negative values
	const Vector4Int neg2(-10, -20, -30, -40);
	const Vector4Int negAdd = neg2 + Vector4Int(10, 20, 30, 40);
	MET_ASSERT_EQ_INT(negAdd.x, 0, "V4I neg add x=0");
	MET_ASSERT_EQ_INT(negAdd.y, 0, "V4I neg add y=0");
	MET_ASSERT_EQ_INT(negAdd.z, 0, "V4I neg add z=0");
	MET_ASSERT_EQ_INT(negAdd.w, 0, "V4I neg add w=0");
}

//------------------------------------------------------------------------------
// Vector2Short extended tests
//------------------------------------------------------------------------------
static void testVector2Short_Extended()
{
	// equality/inequality
	Vector2Short a(3, 4);
	Vector2Short b(3, 4);
	Vector2Short c(1, 2);
	MATH_ET_ASSERT(a == b, "V2S eq same");
	MATH_ET_ASSERT(!(a == c), "V2S eq diff");
	MATH_ET_ASSERT(a != c, "V2S neq");
	MATH_ET_ASSERT(!(a != b), "V2S neq same");

	// unary minus
	Vector2Short neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -3, "V2S unary- x");
	MET_ASSERT_EQ_INT(neg.y, -4, "V2S unary- y");

	// less-than (sum comparison)
	Vector2Short v2s_small(1, 1);
	Vector2Short v2s_large(5, 5);
	MATH_ET_ASSERT(v2s_small < v2s_large, "V2S small < large");
	MATH_ET_ASSERT(!(v2s_large < v2s_small), "V2S not large < small");

	// operator* (scale)
	Vector2Short sc(2, 3);
	Vector2Short scaled = sc * (short)4;
	MET_ASSERT_EQ_INT(scaled.x, 8, "V2S scale x");
	MET_ASSERT_EQ_INT(scaled.y, 12, "V2S scale y");

	// operator+=
	Vector2Short acc(1, 2);
	acc += Vector2Short(10, 20);
	MET_ASSERT_EQ_INT(acc.x, 11, "V2S += x");
	MET_ASSERT_EQ_INT(acc.y, 22, "V2S += y");

	// operator-=
	acc -= Vector2Short(1, 2);
	MET_ASSERT_EQ_INT(acc.x, 10, "V2S -= x");
	MET_ASSERT_EQ_INT(acc.y, 20, "V2S -= y");

	// clear then check
	Vector2Short q(99, 88);
	q.clear();
	MET_ASSERT_EQ_INT(q.x, 0, "V2S clear x");
	MET_ASSERT_EQ_INT(q.y, 0, "V2S clear y");

	// zero default
	Vector2Short zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V2S default x");
	MET_ASSERT_EQ_INT(zero.y, 0, "V2S default y");

	// boundary short max/min
	Vector2Short maxv((short)32767, (short)-32767);
	MET_ASSERT_EQ_INT(maxv.x, 32767, "V2S max x");
	MET_ASSERT_EQ_INT(maxv.y, -32767, "V2S min y");

	// chained add/sub
	Vector2Short ch(5, 5);
	Vector2Short r = (ch + Vector2Short(3, 3)) - Vector2Short(2, 2);
	MET_ASSERT_EQ_INT(r.x, 6, "V2S chain x");
	MET_ASSERT_EQ_INT(r.y, 6, "V2S chain y");
}

//------------------------------------------------------------------------------
// Vector3Int extended tests
//------------------------------------------------------------------------------
static void testVector3Int_Extended()
{
	// default constructor
	Vector3Int def;
	MET_ASSERT_EQ_INT(def.x, 0, "V3I def x");
	MET_ASSERT_EQ_INT(def.y, 0, "V3I def y");
	MET_ASSERT_EQ_INT(def.z, 0, "V3I def z");

	// parameterized constructor
	Vector3Int v(1, 2, 3);
	MET_ASSERT_EQ_INT(v.x, 1, "V3I ctor x");
	MET_ASSERT_EQ_INT(v.y, 2, "V3I ctor y");
	MET_ASSERT_EQ_INT(v.z, 3, "V3I ctor z");

	// add
	Vector3Int a(10, 20, 30);
	Vector3Int b(1, 2, 3);
	Vector3Int add = a + b;
	MET_ASSERT_EQ_INT(add.x, 11, "V3I add x");
	MET_ASSERT_EQ_INT(add.y, 22, "V3I add y");
	MET_ASSERT_EQ_INT(add.z, 33, "V3I add z");

	// sub
	Vector3Int sub = a - b;
	MET_ASSERT_EQ_INT(sub.x, 9, "V3I sub x");
	MET_ASSERT_EQ_INT(sub.y, 18, "V3I sub y");
	MET_ASSERT_EQ_INT(sub.z, 27, "V3I sub z");

	// multiply scalar
	Vector3Int mul = b * 5;
	MET_ASSERT_EQ_INT(mul.x, 5, "V3I mul x");
	MET_ASSERT_EQ_INT(mul.y, 10, "V3I mul y");
	MET_ASSERT_EQ_INT(mul.z, 15, "V3I mul z");

	// unary minus
	Vector3Int neg = -v;
	MET_ASSERT_EQ_INT(neg.x, -1, "V3I neg x");
	MET_ASSERT_EQ_INT(neg.y, -2, "V3I neg y");
	MET_ASSERT_EQ_INT(neg.z, -3, "V3I neg z");

	// +=
	Vector3Int acc(5, 5, 5);
	acc += Vector3Int(1, 2, 3);
	MET_ASSERT_EQ_INT(acc.x, 6, "V3I += x");
	MET_ASSERT_EQ_INT(acc.y, 7, "V3I += y");
	MET_ASSERT_EQ_INT(acc.z, 8, "V3I += z");

	// -=
	acc -= Vector3Int(1, 1, 1);
	MET_ASSERT_EQ_INT(acc.x, 5, "V3I -= x");
	MET_ASSERT_EQ_INT(acc.y, 6, "V3I -= y");
	MET_ASSERT_EQ_INT(acc.z, 7, "V3I -= z");

	// equality
	Vector3Int e1(1, 2, 3), e2(1, 2, 3), e3(1, 2, 4);
	MATH_ET_ASSERT(e1 == e2, "V3I eq");
	MATH_ET_ASSERT(e1 != e3, "V3I neq");
	MATH_ET_ASSERT(!(e1 == e3), "V3I not eq diff");

	// less-than (sum)
	Vector3Int s1(1, 1, 1), s2(3, 3, 3);
	MATH_ET_ASSERT(s1 < s2, "V3I s1 < s2");
	MATH_ET_ASSERT(!(s2 < s1), "V3I not s2 < s1");

	// clear
	Vector3Int cl(7, 8, 9);
	cl.clear();
	MET_ASSERT_EQ_INT(cl.x, 0, "V3I clear x");
	MET_ASSERT_EQ_INT(cl.y, 0, "V3I clear y");
	MET_ASSERT_EQ_INT(cl.z, 0, "V3I clear z");

	// large values
	Vector3Int big(1000000, 2000000, 3000000);
	Vector3Int bigr = big + Vector3Int(1, 2, 3);
	MET_ASSERT_EQ_INT(bigr.x, 1000001, "V3I big x");
	MET_ASSERT_EQ_INT(bigr.y, 2000002, "V3I big y");
	MET_ASSERT_EQ_INT(bigr.z, 3000003, "V3I big z");

	// negative values
	Vector3Int nv(-5, -10, -15);
	Vector3Int nvr = nv + Vector3Int(5, 10, 15);
	MET_ASSERT_EQ_INT(nvr.x, 0, "V3I neg+pos x");
	MET_ASSERT_EQ_INT(nvr.y, 0, "V3I neg+pos y");
	MET_ASSERT_EQ_INT(nvr.z, 0, "V3I neg+pos z");
}

//------------------------------------------------------------------------------
// Vector4 extended tests
//------------------------------------------------------------------------------
static void testVector4_Extended()
{
	// default
	Vector4 def;
	MET_ASSERT_FLOAT(def.x, 0.0f, "V4 def x");
	MET_ASSERT_FLOAT(def.y, 0.0f, "V4 def y");
	MET_ASSERT_FLOAT(def.z, 0.0f, "V4 def z");
	MET_ASSERT_FLOAT(def.w, 0.0f, "V4 def w");

	// parameterized
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	MET_ASSERT_FLOAT(v.x, 1.0f, "V4 ctor x");
	MET_ASSERT_FLOAT(v.y, 2.0f, "V4 ctor y");
	MET_ASSERT_FLOAT(v.z, 3.0f, "V4 ctor z");
	MET_ASSERT_FLOAT(v.w, 4.0f, "V4 ctor w");

	// add
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(0.5f, 0.5f, 0.5f, 0.5f);
	Vector4 add = a + b;
	MET_ASSERT_FLOAT(add.x, 1.5f, "V4 add x");
	MET_ASSERT_FLOAT(add.y, 2.5f, "V4 add y");
	MET_ASSERT_FLOAT(add.z, 3.5f, "V4 add z");
	MET_ASSERT_FLOAT(add.w, 4.5f, "V4 add w");

	// sub
	Vector4 sub = a - b;
	MET_ASSERT_FLOAT(sub.x, 0.5f, "V4 sub x");
	MET_ASSERT_FLOAT(sub.y, 1.5f, "V4 sub y");
	MET_ASSERT_FLOAT(sub.z, 2.5f, "V4 sub z");
	MET_ASSERT_FLOAT(sub.w, 3.5f, "V4 sub w");

	// unary minus
	Vector4 neg = -a;
	MET_ASSERT_FLOAT(neg.x, -1.0f, "V4 neg x");
	MET_ASSERT_FLOAT(neg.y, -2.0f, "V4 neg y");
	MET_ASSERT_FLOAT(neg.z, -3.0f, "V4 neg z");
	MET_ASSERT_FLOAT(neg.w, -4.0f, "V4 neg w");

	// +=
	Vector4 acc(1.0f, 2.0f, 3.0f, 4.0f);
	acc += Vector4(0.1f, 0.2f, 0.3f, 0.4f);
	MET_ASSERT_FLOAT(acc.x, 1.1f, "V4 += x");
	MET_ASSERT_FLOAT(acc.y, 2.2f, "V4 += y");
	MET_ASSERT_FLOAT(acc.z, 3.3f, "V4 += z");
	MET_ASSERT_FLOAT(acc.w, 4.4f, "V4 += w");

	// -=
	acc -= Vector4(0.1f, 0.2f, 0.3f, 0.4f);
	MET_ASSERT_FLOAT(acc.x, 1.0f, "V4 -= x");
	MET_ASSERT_FLOAT(acc.y, 2.0f, "V4 -= y");
	MET_ASSERT_FLOAT(acc.z, 3.0f, "V4 -= z");
	MET_ASSERT_FLOAT(acc.w, 4.0f, "V4 -= w");

	// clear
	Vector4 cl(9.9f, 8.8f, 7.7f, 6.6f);
	cl.clear();
	MET_ASSERT_FLOAT(cl.x, 0.0f, "V4 clear x");
	MET_ASSERT_FLOAT(cl.y, 0.0f, "V4 clear y");
	MET_ASSERT_FLOAT(cl.z, 0.0f, "V4 clear z");
	MET_ASSERT_FLOAT(cl.w, 0.0f, "V4 clear w");

	// chained ops
	Vector4 ch(2.0f, 2.0f, 2.0f, 2.0f);
	Vector4 r = (ch + Vector4(1.0f, 1.0f, 1.0f, 1.0f)) - Vector4(0.5f, 0.5f, 0.5f, 0.5f);
	MET_ASSERT_FLOAT(r.x, 2.5f, "V4 chain x");
	MET_ASSERT_FLOAT(r.y, 2.5f, "V4 chain y");
	MET_ASSERT_FLOAT(r.z, 2.5f, "V4 chain z");
	MET_ASSERT_FLOAT(r.w, 2.5f, "V4 chain w");

	// large values
	Vector4 big(1e5f, 2e5f, 3e5f, 4e5f);
	Vector4 bigr = big + Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	MET_ASSERT_FLOAT(bigr.x, 100001.0f, "V4 big x");
	MET_ASSERT_FLOAT(bigr.w, 400001.0f, "V4 big w");

	// small fractional
	Vector4 frac(0.001f, 0.002f, 0.003f, 0.004f);
	Vector4 fracr = frac + frac;
	MET_ASSERT_FLOAT(fracr.x, 0.002f, "V4 frac x");
	MET_ASSERT_FLOAT(fracr.w, 0.008f, "V4 frac w");
}

//------------------------------------------------------------------------------
// Vector4Int extended tests
//------------------------------------------------------------------------------
static void testVector4Int_Extended()
{
	// default
	Vector4Int def;
	MET_ASSERT_EQ_INT(def.x, 0, "V4I-ext def x");
	MET_ASSERT_EQ_INT(def.y, 0, "V4I-ext def y");
	MET_ASSERT_EQ_INT(def.z, 0, "V4I-ext def z");
	MET_ASSERT_EQ_INT(def.w, 0, "V4I-ext def w");

	// equality/inequality
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(1, 2, 3, 4);
	Vector4Int c(1, 2, 3, 5);
	MATH_ET_ASSERT(a == b, "V4I-ext eq");
	MATH_ET_ASSERT(a != c, "V4I-ext neq");

	// unary minus
	Vector4Int neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -1, "V4I-ext neg x");
	MET_ASSERT_EQ_INT(neg.y, -2, "V4I-ext neg y");
	MET_ASSERT_EQ_INT(neg.z, -3, "V4I-ext neg z");
	MET_ASSERT_EQ_INT(neg.w, -4, "V4I-ext neg w");

	// less-than (sum)
	Vector4Int s1(1, 1, 1, 1);
	Vector4Int s2(2, 2, 2, 2);
	MATH_ET_ASSERT(s1 < s2, "V4I-ext s1 < s2");
	MATH_ET_ASSERT(!(s2 < s1), "V4I-ext not s2 < s1");

	// scale
	Vector4Int sc(2, 3, 4, 5);
	Vector4Int scaled = sc * 3;
	MET_ASSERT_EQ_INT(scaled.x, 6, "V4I-ext scale x");
	MET_ASSERT_EQ_INT(scaled.y, 9, "V4I-ext scale y");
	MET_ASSERT_EQ_INT(scaled.z, 12, "V4I-ext scale z");
	MET_ASSERT_EQ_INT(scaled.w, 15, "V4I-ext scale w");

	// large positive
	Vector4Int big(100000, 200000, 300000, 400000);
	Vector4Int bigr = big + Vector4Int(1, 2, 3, 4);
	MET_ASSERT_EQ_INT(bigr.x, 100001, "V4I-ext big x");
	MET_ASSERT_EQ_INT(bigr.w, 400004, "V4I-ext big w");

	// negative add/sub
	Vector4Int nv(-10, -20, -30, -40);
	Vector4Int pv(10, 20, 30, 40);
	Vector4Int zero = nv + pv;
	MET_ASSERT_EQ_INT(zero.x, 0, "V4I-ext neg+pos x");
	MET_ASSERT_EQ_INT(zero.y, 0, "V4I-ext neg+pos y");
	MET_ASSERT_EQ_INT(zero.z, 0, "V4I-ext neg+pos z");
	MET_ASSERT_EQ_INT(zero.w, 0, "V4I-ext neg+pos w");

	// chained ops
	Vector4Int ch(5, 5, 5, 5);
	Vector4Int r = (ch + Vector4Int(1, 2, 3, 4)) - Vector4Int(2, 3, 4, 5);
	MET_ASSERT_EQ_INT(r.x, 4, "V4I-ext chain x");
	MET_ASSERT_EQ_INT(r.y, 4, "V4I-ext chain y");
	MET_ASSERT_EQ_INT(r.z, 4, "V4I-ext chain z");
	MET_ASSERT_EQ_INT(r.w, 4, "V4I-ext chain w");

	// multiple clear cycles
	Vector4Int mc(1, 2, 3, 4);
	FOR(200)
	{
		mc.clear();
		MET_ASSERT_EQ_INT(mc.x, 0, "V4I-ext multiclear x");
		mc = Vector4Int(i, i+1, i+2, i+3);
	}
}

//------------------------------------------------------------------------------
// Vector2UInt extended tests
//------------------------------------------------------------------------------
static void testVector2UInt_Extended()
{
	// default
	Vector2UInt def;
	MET_ASSERT_EQ_UINT(def.x, 0, "V2UI-ext def x");
	MET_ASSERT_EQ_UINT(def.y, 0, "V2UI-ext def y");

	// parameterized
	Vector2UInt v(100u, 200u);
	MET_ASSERT_EQ_UINT(v.x, 100u, "V2UI-ext ctor x");
	MET_ASSERT_EQ_UINT(v.y, 200u, "V2UI-ext ctor y");

	// add
	Vector2UInt a(10u, 20u), b(5u, 8u);
	Vector2UInt add = a + b;
	MET_ASSERT_EQ_UINT(add.x, 15u, "V2UI-ext add x");
	MET_ASSERT_EQ_UINT(add.y, 28u, "V2UI-ext add y");

	// sub
	Vector2UInt sub = a - b;
	MET_ASSERT_EQ_UINT(sub.x, 5u, "V2UI-ext sub x");
	MET_ASSERT_EQ_UINT(sub.y, 12u, "V2UI-ext sub y");

	// scale
	Vector2UInt sc(3u, 4u);
	Vector2UInt scaled = sc * 6u;
	MET_ASSERT_EQ_UINT(scaled.x, 18u, "V2UI-ext scale x");
	MET_ASSERT_EQ_UINT(scaled.y, 24u, "V2UI-ext scale y");

	// +=
	Vector2UInt acc(1u, 2u);
	acc += Vector2UInt(9u, 8u);
	MET_ASSERT_EQ_UINT(acc.x, 10u, "V2UI-ext += x");
	MET_ASSERT_EQ_UINT(acc.y, 10u, "V2UI-ext += y");

	// -=
	acc -= Vector2UInt(5u, 5u);
	MET_ASSERT_EQ_UINT(acc.x, 5u, "V2UI-ext -= x");
	MET_ASSERT_EQ_UINT(acc.y, 5u, "V2UI-ext -= y");

	// equality
	Vector2UInt e1(10u, 20u), e2(10u, 20u), e3(10u, 21u);
	MATH_ET_ASSERT(e1 == e2, "V2UI-ext eq");
	MATH_ET_ASSERT(e1 != e3, "V2UI-ext neq");

	// clear
	Vector2UInt cl(99u, 88u);
	cl.clear();
	MET_ASSERT_EQ_UINT(cl.x, 0u, "V2UI-ext clear x");
	MET_ASSERT_EQ_UINT(cl.y, 0u, "V2UI-ext clear y");

	// large values
	Vector2UInt big(1000000u, 2000000u);
	Vector2UInt bigr = big + Vector2UInt(1u, 2u);
	MET_ASSERT_EQ_UINT(bigr.x, 1000001u, "V2UI-ext big x");
	MET_ASSERT_EQ_UINT(bigr.y, 2000002u, "V2UI-ext big y");
}

//------------------------------------------------------------------------------
// Vector2UShort extended tests
//------------------------------------------------------------------------------
static void testVector2UShort_Extended()
{
	// default
	Vector2UShort def;
	MET_ASSERT_EQ_UINT(def.x, 0u, "V2US-ext def x");
	MET_ASSERT_EQ_UINT(def.y, 0u, "V2US-ext def y");

	// parameterized
	Vector2UShort v((ushort)100, (ushort)200);
	MET_ASSERT_EQ_UINT(v.x, 100u, "V2US-ext ctor x");
	MET_ASSERT_EQ_UINT(v.y, 200u, "V2US-ext ctor y");

	// add
	Vector2UShort a((ushort)50, (ushort)60);
	Vector2UShort b((ushort)10, (ushort)20);
	Vector2UShort add = a + b;
	MET_ASSERT_EQ_UINT(add.x, 60u, "V2US-ext add x");
	MET_ASSERT_EQ_UINT(add.y, 80u, "V2US-ext add y");

	// sub
	Vector2UShort sub = a - b;
	MET_ASSERT_EQ_UINT(sub.x, 40u, "V2US-ext sub x");
	MET_ASSERT_EQ_UINT(sub.y, 40u, "V2US-ext sub y");

	// scale
	Vector2UShort sc((ushort)5, (ushort)7);
	Vector2UShort scaled = sc * (ushort)3;
	MET_ASSERT_EQ_UINT(scaled.x, 15u, "V2US-ext scale x");
	MET_ASSERT_EQ_UINT(scaled.y, 21u, "V2US-ext scale y");

	// +=
	Vector2UShort acc((ushort)1, (ushort)2);
	acc += Vector2UShort((ushort)9, (ushort)8);
	MET_ASSERT_EQ_UINT(acc.x, 10u, "V2US-ext += x");
	MET_ASSERT_EQ_UINT(acc.y, 10u, "V2US-ext += y");

	// -=
	acc -= Vector2UShort((ushort)5, (ushort)5);
	MET_ASSERT_EQ_UINT(acc.x, 5u, "V2US-ext -= x");
	MET_ASSERT_EQ_UINT(acc.y, 5u, "V2US-ext -= y");

	// equality
	Vector2UShort e1((ushort)10, (ushort)20);
	Vector2UShort e2((ushort)10, (ushort)20);
	Vector2UShort e3((ushort)10, (ushort)21);
	MATH_ET_ASSERT(e1 == e2, "V2US-ext eq");
	MATH_ET_ASSERT(e1 != e3, "V2US-ext neq");

	// clear
	Vector2UShort cl((ushort)99, (ushort)88);
	cl.clear();
	MET_ASSERT_EQ_UINT(cl.x, 0u, "V2US-ext clear x");
	MET_ASSERT_EQ_UINT(cl.y, 0u, "V2US-ext clear y");

	// max ushort value
	Vector2UShort maxv((ushort)65535, (ushort)65535);
	MET_ASSERT_EQ_UINT(maxv.x, 65535u, "V2US-ext maxval x");
	MET_ASSERT_EQ_UINT(maxv.y, 65535u, "V2US-ext maxval y");

	// less-than (sum)
	Vector2UShort s1((ushort)1, (ushort)1);
	Vector2UShort s2((ushort)5, (ushort)5);
	MATH_ET_ASSERT(s1 < s2, "V2US-ext s1 < s2");
	MATH_ET_ASSERT(!(s2 < s1), "V2US-ext not s2 < s1");
}

//------------------------------------------------------------------------------
// MathExtTest entry point
//------------------------------------------------------------------------------
void MathExtTest::test()
{
	testVector2Short();
	testVector2UInt();
	testVector2UShort();
	testVector3Int();
	testVector4();
	testVector4Int();
	testVector2Short_Extended();
	testVector3Int_Extended();
	testVector4_Extended();
	testVector4Int_Extended();
	testVector2UInt_Extended();
	testVector2UShort_Extended();
}
