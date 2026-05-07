#include "FrameHeader.h"

#define MT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MathTest FAIL: ") + (msg)); } } while(0)
#define MT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MathTest FAIL: ") + (msg)); } } while(0)
#define MT_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((a) - (b)) > 1e-4f) { ERROR(string("MathTest FAIL: ") + (msg) + \
		" | expected=" + FToS(b) + " actual=" + FToS(a)); } } while(0)

//------------------------------------------------------------------------------
// Vector2
//------------------------------------------------------------------------------
static void testVector2_Operators()
{
	const Vector2 a(1.0f, 2.0f);
	const Vector2 b(3.0f, 4.0f);

	// +
	const Vector2 add = a + b;
	MT_ASSERT_FLOAT(add.x, 4.0f, "V2 add x");
	MT_ASSERT_FLOAT(add.y, 6.0f, "V2 add y");

	// -
	const Vector2 sub = b - a;
	MT_ASSERT_FLOAT(sub.x, 2.0f, "V2 sub x");
	MT_ASSERT_FLOAT(sub.y, 2.0f, "V2 sub y");

	// * scalar
	const Vector2 scaled = a * 3.0f;
	MT_ASSERT_FLOAT(scaled.x, 3.0f, "V2 scale x");
	MT_ASSERT_FLOAT(scaled.y, 6.0f, "V2 scale y");

	// / scalar
	const Vector2 divided = b / 2.0f;
	MT_ASSERT_FLOAT(divided.x, 1.5f, "V2 div x");
	MT_ASSERT_FLOAT(divided.y, 2.0f, "V2 div y");

	// unary -
	const Vector2 neg = -a;
	MT_ASSERT_FLOAT(neg.x, -1.0f, "V2 neg x");
	MT_ASSERT_FLOAT(neg.y, -2.0f, "V2 neg y");

	// +=
	Vector2 acc(1.0f, 1.0f);
	acc += Vector2(2.0f, 3.0f);
	MT_ASSERT_FLOAT(acc.x, 3.0f, "V2 += x");
	MT_ASSERT_FLOAT(acc.y, 4.0f, "V2 += y");

	// -=
	acc -= Vector2(1.0f, 1.0f);
	MT_ASSERT_FLOAT(acc.x, 2.0f, "V2 -= x");
	MT_ASSERT_FLOAT(acc.y, 3.0f, "V2 -= y");

	// clear
	Vector2 v(5.0f, 6.0f);
	v.clear();
	MT_ASSERT_FLOAT(v.x, 0.0f, "V2 clear x");
	MT_ASSERT_FLOAT(v.y, 0.0f, "V2 clear y");

	// ZERO / ONE constants
	MT_ASSERT_FLOAT(Vector2::ZERO.x, 0.0f, "V2::ZERO x");
	MT_ASSERT_FLOAT(Vector2::ZERO.y, 0.0f, "V2::ZERO y");
	MT_ASSERT_FLOAT(Vector2::ONE.x,  1.0f, "V2::ONE x");
	MT_ASSERT_FLOAT(Vector2::ONE.y,  1.0f, "V2::ONE y");
}

//------------------------------------------------------------------------------
// Vector3
//------------------------------------------------------------------------------
static void testVector3_Operators()
{
	const Vector3 a(1.0f, 2.0f, 3.0f);
	const Vector3 b(4.0f, 5.0f, 6.0f);

	// +
	const Vector3 add = a + b;
	MT_ASSERT_FLOAT(add.x, 5.0f, "V3 add x");
	MT_ASSERT_FLOAT(add.y, 7.0f, "V3 add y");
	MT_ASSERT_FLOAT(add.z, 9.0f, "V3 add z");

	// -
	const Vector3 sub = b - a;
	MT_ASSERT_FLOAT(sub.x, 3.0f, "V3 sub x");
	MT_ASSERT_FLOAT(sub.y, 3.0f, "V3 sub y");
	MT_ASSERT_FLOAT(sub.z, 3.0f, "V3 sub z");

	// * scalar
	const Vector3 scaled = a * 2.0f;
	MT_ASSERT_FLOAT(scaled.x, 2.0f, "V3 scale x");
	MT_ASSERT_FLOAT(scaled.y, 4.0f, "V3 scale y");
	MT_ASSERT_FLOAT(scaled.z, 6.0f, "V3 scale z");

	// / scalar
	const Vector3 divided = b / 2.0f;
	MT_ASSERT_FLOAT(divided.x, 2.0f, "V3 div x");
	MT_ASSERT_FLOAT(divided.y, 2.5f, "V3 div y");
	MT_ASSERT_FLOAT(divided.z, 3.0f, "V3 div z");

	// unary -
	const Vector3 neg = -a;
	MT_ASSERT_FLOAT(neg.x, -1.0f, "V3 neg x");
	MT_ASSERT_FLOAT(neg.y, -2.0f, "V3 neg y");
	MT_ASSERT_FLOAT(neg.z, -3.0f, "V3 neg z");

	// +=
	Vector3 acc(0.0f, 0.0f, 0.0f);
	acc += a;
	MT_ASSERT_FLOAT(acc.x, 1.0f, "V3 += x");
	MT_ASSERT_FLOAT(acc.y, 2.0f, "V3 += y");
	MT_ASSERT_FLOAT(acc.z, 3.0f, "V3 += z");

	// -=
	acc -= a;
	MT_ASSERT_FLOAT(acc.x, 0.0f, "V3 -= x");
	MT_ASSERT_FLOAT(acc.y, 0.0f, "V3 -= y");
	MT_ASSERT_FLOAT(acc.z, 0.0f, "V3 -= z");

	// clear
	Vector3 v(1.0f, 2.0f, 3.0f);
	v.clear();
	MT_ASSERT_FLOAT(v.x, 0.0f, "V3 clear x");
	MT_ASSERT_FLOAT(v.y, 0.0f, "V3 clear y");
	MT_ASSERT_FLOAT(v.z, 0.0f, "V3 clear z");

	// constants
	MT_ASSERT_FLOAT(Vector3::ZERO.x, 0.0f, "V3::ZERO x");
	MT_ASSERT_FLOAT(Vector3::UP.y,   1.0f, "V3::UP y");
}

//------------------------------------------------------------------------------
// Vector2Int
//------------------------------------------------------------------------------
static void testVector2Int_Operators()
{
	const Vector2Int a(2, 3);
	const Vector2Int b(5, 7);

	// +
	const Vector2Int add = a + b;
	MT_ASSERT_EQ(add.x, 7, "V2Int add x");
	MT_ASSERT_EQ(add.y, 10, "V2Int add y");

	// -
	const Vector2Int sub = b - a;
	MT_ASSERT_EQ(sub.x, 3, "V2Int sub x");
	MT_ASSERT_EQ(sub.y, 4, "V2Int sub y");

	// * scalar
	const Vector2Int scaled = a * 3;
	MT_ASSERT_EQ(scaled.x, 6,  "V2Int scale x");
	MT_ASSERT_EQ(scaled.y, 9,  "V2Int scale y");

	// unary -
	const Vector2Int neg = -a;
	MT_ASSERT_EQ(neg.x, -2, "V2Int neg x");
	MT_ASSERT_EQ(neg.y, -3, "V2Int neg y");

	// ==, !=
	MT_ASSERT(a == Vector2Int(2, 3), "V2Int ==");
	MT_ASSERT(a != b,                "V2Int !=");

	// +=, -=
	Vector2Int acc(0, 0);
	acc += a;
	MT_ASSERT_EQ(acc.x, 2, "V2Int += x");
	MT_ASSERT_EQ(acc.y, 3, "V2Int += y");
	acc -= a;
	MT_ASSERT_EQ(acc.x, 0, "V2Int -= x");
	MT_ASSERT_EQ(acc.y, 0, "V2Int -= y");

	// clear
	Vector2Int v(4, 5);
	v.clear();
	MT_ASSERT_EQ(v.x, 0, "V2Int clear x");
	MT_ASSERT_EQ(v.y, 0, "V2Int clear y");

	// ZERO
	MT_ASSERT_EQ(Vector2Int::ZERO.x, 0, "V2Int::ZERO x");
	MT_ASSERT_EQ(Vector2Int::ZERO.y, 0, "V2Int::ZERO y");
}

//------------------------------------------------------------------------------
// Color
//------------------------------------------------------------------------------
static void testColor_Operators()
{
	const Color a((byte)10u, (byte)20u, (byte)30u, (byte)255u);
	const Color b((byte)5u,  (byte)10u, (byte)15u, (byte)100u);

	// +
	const Color added = a + b;
	MT_ASSERT_EQ((int)added.r, 15,  "Color add r");
	MT_ASSERT_EQ((int)added.g, 30,  "Color add g");
	MT_ASSERT_EQ((int)added.b, 45,  "Color add b");
	MT_ASSERT_EQ((int)added.a, 355 & 0xFF, "Color add a wrap");

	// -
	const Color subbed = a - b;
	MT_ASSERT_EQ((int)subbed.r, 5,  "Color sub r");
	MT_ASSERT_EQ((int)subbed.g, 10, "Color sub g");
	MT_ASSERT_EQ((int)subbed.b, 15, "Color sub b");

	// +=
	Color acc((byte)1u, (byte)2u, (byte)3u, (byte)4u);
	acc += Color((byte)10u, (byte)20u, (byte)30u, (byte)40u);
	MT_ASSERT_EQ((int)acc.r, 11, "Color += r");
	MT_ASSERT_EQ((int)acc.g, 22, "Color += g");
	MT_ASSERT_EQ((int)acc.b, 33, "Color += b");
	MT_ASSERT_EQ((int)acc.a, 44, "Color += a");

	// static constants
	MT_ASSERT_EQ((int)Color::WHITE.r, 255, "Color::WHITE r");
	MT_ASSERT_EQ((int)Color::WHITE.g, 255, "Color::WHITE g");
	MT_ASSERT_EQ((int)Color::WHITE.b, 255, "Color::WHITE b");
	MT_ASSERT_EQ((int)Color::WHITE.a, 255, "Color::WHITE a");

	MT_ASSERT_EQ((int)Color::BLACK.r, 0,   "Color::BLACK r");
	MT_ASSERT_EQ((int)Color::BLACK.a, 255, "Color::BLACK a");

	MT_ASSERT_EQ((int)Color::RED.r, 255,   "Color::RED r");
	MT_ASSERT_EQ((int)Color::RED.g, 0,     "Color::RED g");
}

//------------------------------------------------------------------------------
// Quaternion
//------------------------------------------------------------------------------
static void testQuaternion_Basic()
{
	// identity
	const Quaternion identity;
	MT_ASSERT_FLOAT(identity.x, 0.0f, "Quat identity x");
	MT_ASSERT_FLOAT(identity.y, 0.0f, "Quat identity y");
	MT_ASSERT_FLOAT(identity.z, 0.0f, "Quat identity z");
	MT_ASSERT_FLOAT(identity.w, 1.0f, "Quat identity w");

	// dot with itself = 1
	MT_ASSERT_FLOAT(Quaternion::dot(identity, identity), 1.0f, "Quat dot(id,id)=1");

	// length of identity = 1
	MT_ASSERT_FLOAT(identity.length(), 1.0f, "Quat length(id)=1");

	// conjugate of identity
	const Quaternion conj = identity.conjugate();
	MT_ASSERT_FLOAT(conj.x,  0.0f, "Quat conj x");
	MT_ASSERT_FLOAT(conj.y,  0.0f, "Quat conj y");
	MT_ASSERT_FLOAT(conj.z,  0.0f, "Quat conj z");
	MT_ASSERT_FLOAT(conj.w,  1.0f, "Quat conj w");

	// normalize of identity = identity
	const Quaternion norm = identity.normalize();
	MT_ASSERT_FLOAT(norm.w, 1.0f, "Quat normalize(id) w");
	MT_ASSERT_FLOAT(norm.length(), 1.0f, "Quat normalize length=1");

	// * scalar
	const Quaternion scaled = identity * 2.0f;
	MT_ASSERT_FLOAT(scaled.w, 2.0f, "Quat scale w");

	// +
	const Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
	const Quaternion q2(0.1f, 0.2f, 0.3f, 0.9f);
	const Quaternion sum = q1 + q2;
	MT_ASSERT_FLOAT(sum.x, 0.1f, "Quat + x");
	MT_ASSERT_FLOAT(sum.w, 1.9f, "Quat + w");

	// clear
	Quaternion q = q2;
	q.clear();
	MT_ASSERT_FLOAT(q.x, 0.0f, "Quat clear x");
	MT_ASSERT_FLOAT(q.w, 1.0f, "Quat clear w");

	// axis-angle construction -> angle should round-trip
	const Vector3 axis(0.0f, 1.0f, 0.0f);
	const float angleRad = 1.0f; // ~57 deg
	const Quaternion fromAxis(axis, angleRad);
	MT_ASSERT_FLOAT(fromAxis.length(), 1.0f, "Quat axis-angle length=1");
	MT_ASSERT_FLOAT(fromAxis.angle(), angleRad, "Quat axis-angle round-trip");
}

//------------------------------------------------------------------------------
// Vector2 — extended tests
//------------------------------------------------------------------------------
static void testVector2_StaticConstants()
{
	MT_ASSERT_FLOAT(Vector2::UP.x,    0.0f, "V2::UP x=0");
	MT_ASSERT_FLOAT(Vector2::UP.y,    1.0f, "V2::UP y=1");
	MT_ASSERT_FLOAT(Vector2::DOWN.x,  0.0f, "V2::DOWN x=0");
	MT_ASSERT_FLOAT(Vector2::DOWN.y, -1.0f, "V2::DOWN y=-1");
	MT_ASSERT_FLOAT(Vector2::LEFT.x, -1.0f, "V2::LEFT x=-1");
	MT_ASSERT_FLOAT(Vector2::LEFT.y,  0.0f, "V2::LEFT y=0");
	MT_ASSERT_FLOAT(Vector2::RIGHT.x, 1.0f, "V2::RIGHT x=1");
	MT_ASSERT_FLOAT(Vector2::RIGHT.y, 0.0f, "V2::RIGHT y=0");
}

static void testVector2_IntConstructor()
{
	Vector2 v(3, 4);
	MT_ASSERT_FLOAT(v.x, 3.0f, "V2 int ctor x");
	MT_ASSERT_FLOAT(v.y, 4.0f, "V2 int ctor y");
}

static void testVector2_ChainedOps()
{
	Vector2 v(1.0f, 1.0f);
	v += Vector2(1.0f, 0.0f);
	v += Vector2(0.0f, 2.0f);
	v -= Vector2(0.5f, 0.5f);
	// v = (1.5, 2.5)
	MT_ASSERT_FLOAT(v.x, 1.5f, "V2 chained x");
	MT_ASSERT_FLOAT(v.y, 2.5f, "V2 chained y");

	Vector2 scaled = v * 4.0f;
	MT_ASSERT_FLOAT(scaled.x, 6.0f, "V2 chained scale x");
	MT_ASSERT_FLOAT(scaled.y, 10.0f, "V2 chained scale y");

	Vector2 divided = scaled / 2.0f;
	MT_ASSERT_FLOAT(divided.x, 3.0f, "V2 chained div x");
	MT_ASSERT_FLOAT(divided.y, 5.0f, "V2 chained div y");
}

static void testVector2_NegativeValues()
{
	Vector2 a(-3.0f, -4.0f);
	Vector2 b(-1.0f, 2.0f);
	Vector2 sum = a + b;
	MT_ASSERT_FLOAT(sum.x, -4.0f, "V2 neg add x");
	MT_ASSERT_FLOAT(sum.y, -2.0f, "V2 neg add y");

	Vector2 neg = -a;
	MT_ASSERT_FLOAT(neg.x, 3.0f, "V2 double neg x");
	MT_ASSERT_FLOAT(neg.y, 4.0f, "V2 double neg y");
}

static void testVector2_SmallValues()
{
	Vector2 tiny(0.001f, -0.002f);
	MT_ASSERT_FLOAT(tiny.x * 1000.0f, 1.0f, "V2 tiny scale x");
	MT_ASSERT_FLOAT(tiny.y * (-500.0f), 1.0f, "V2 tiny scale y");
}

static void testVector2_LargeValues()
{
	Vector2 big(100000.0f, -200000.0f);
	Vector2 scaled = big * 0.00001f;
	MT_ASSERT_FLOAT(scaled.x, 1.0f, "V2 large scale x");
	MT_ASSERT_FLOAT(scaled.y, -2.0f, "V2 large scale y");
}

//------------------------------------------------------------------------------
// Vector3 — extended tests
//------------------------------------------------------------------------------
static void testVector3_StaticConstants()
{
	MT_ASSERT_FLOAT(Vector3::ONE.x,     1.0f,  "V3::ONE x");
	MT_ASSERT_FLOAT(Vector3::ONE.y,     1.0f,  "V3::ONE y");
	MT_ASSERT_FLOAT(Vector3::ONE.z,     1.0f,  "V3::ONE z");
	MT_ASSERT_FLOAT(Vector3::DOWN.y,   -1.0f,  "V3::DOWN y");
	MT_ASSERT_FLOAT(Vector3::LEFT.x,   -1.0f,  "V3::LEFT x");
	MT_ASSERT_FLOAT(Vector3::RIGHT.x,   1.0f,  "V3::RIGHT x");
	MT_ASSERT_FLOAT(Vector3::FORWARD.z, 1.0f,  "V3::FORWARD z");
	MT_ASSERT_FLOAT(Vector3::BACK.z,   -1.0f,  "V3::BACK z");
}

static void testVector3_IntConstructor()
{
	Vector3 v(1, 2, 3);
	MT_ASSERT_FLOAT(v.x, 1.0f, "V3 int ctor x");
	MT_ASSERT_FLOAT(v.y, 2.0f, "V3 int ctor y");
	MT_ASSERT_FLOAT(v.z, 3.0f, "V3 int ctor z");
}

static void testVector3_MultiplyScalarBothSides()
{
	Vector3 a(2.0f, 3.0f, 4.0f);
	Vector3 b = a * 5.0f;
	MT_ASSERT_FLOAT(b.x, 10.0f, "V3 *5 x");
	MT_ASSERT_FLOAT(b.y, 15.0f, "V3 *5 y");
	MT_ASSERT_FLOAT(b.z, 20.0f, "V3 *5 z");

	Vector3 c = b / 5.0f;
	MT_ASSERT_FLOAT(c.x, a.x, "V3 /5 = original x");
	MT_ASSERT_FLOAT(c.y, a.y, "V3 /5 = original y");
	MT_ASSERT_FLOAT(c.z, a.z, "V3 /5 = original z");
}

static void testVector3_NegativeAndClear()
{
	Vector3 v(-1.0f, -2.0f, -3.0f);
	Vector3 pos = -v;
	MT_ASSERT_FLOAT(pos.x, 1.0f, "V3 neg x");
	MT_ASSERT_FLOAT(pos.y, 2.0f, "V3 neg y");
	MT_ASSERT_FLOAT(pos.z, 3.0f, "V3 neg z");

	pos.clear();
	MT_ASSERT_FLOAT(pos.x, 0.0f, "V3 clear x");
	MT_ASSERT_FLOAT(pos.y, 0.0f, "V3 clear y");
	MT_ASSERT_FLOAT(pos.z, 0.0f, "V3 clear z");
}

static void testVector3_ChainedArithmetic()
{
	Vector3 acc(0.0f, 0.0f, 0.0f);
	for (int i = 1; i <= 10; ++i)
	{
		acc += Vector3((float)i, (float)i, (float)i);
	}
	// sum of 1..10 = 55
	MT_ASSERT_FLOAT(acc.x, 55.0f, "V3 chain sum x");
	MT_ASSERT_FLOAT(acc.y, 55.0f, "V3 chain sum y");
	MT_ASSERT_FLOAT(acc.z, 55.0f, "V3 chain sum z");

	for (int i = 1; i <= 10; ++i)
	{
		acc -= Vector3((float)i, (float)i, (float)i);
	}
	MT_ASSERT_FLOAT(acc.x, 0.0f, "V3 chain sub back to 0 x");
	MT_ASSERT_FLOAT(acc.y, 0.0f, "V3 chain sub back to 0 y");
	MT_ASSERT_FLOAT(acc.z, 0.0f, "V3 chain sub back to 0 z");
}

//------------------------------------------------------------------------------
// Vector2Int — extended
//------------------------------------------------------------------------------
static void testVector2Int_Extended()
{
	// Zero operations
	Vector2Int zero = Vector2Int::ZERO;
	MT_ASSERT_EQ(zero.x, 0, "V2Int::ZERO x=0");
	MT_ASSERT_EQ(zero.y, 0, "V2Int::ZERO y=0");

	// Large values
	Vector2Int big(10000, -10000);
	Vector2Int doubled = big * 2;
	MT_ASSERT_EQ(doubled.x, 20000, "V2Int big *2 x");
	MT_ASSERT_EQ(doubled.y, -20000, "V2Int big *2 y");

	// Chain add/subtract
	Vector2Int a(5, 10);
	Vector2Int b(3, 4);
	Vector2Int c = a + b;
	MT_ASSERT_EQ(c.x, 8, "V2Int + x");
	MT_ASSERT_EQ(c.y, 14, "V2Int + y");

	Vector2Int d = c - a;
	MT_ASSERT_EQ(d.x, b.x, "V2Int - x = b.x");
	MT_ASSERT_EQ(d.y, b.y, "V2Int - y = b.y");

	// Equality / inequality
	MT_ASSERT(c != a, "V2Int != a");
	MT_ASSERT(d == b, "V2Int d == b");

	// Negative
	Vector2Int neg = -a;
	MT_ASSERT_EQ(neg.x, -5, "V2Int neg x");
	MT_ASSERT_EQ(neg.y, -10, "V2Int neg y");

	// += and -=
	Vector2Int acc(0, 0);
	for (int i = 1; i <= 5; ++i)
	{
		acc += Vector2Int(i, i * 2);
	}
	// x: 1+2+3+4+5=15, y: 2+4+6+8+10=30
	MT_ASSERT_EQ(acc.x, 15, "V2Int chain += x");
	MT_ASSERT_EQ(acc.y, 30, "V2Int chain += y");
}

//------------------------------------------------------------------------------
// Color — extended
//------------------------------------------------------------------------------
static void testColor_Extended()
{
	// Default constructor (should be (0,0,0,0) or (0,0,0,255)?)
	Color c;
	(void)c; // just ensure it compiles

	// GREY
	MT_ASSERT_EQ((int)Color::GREY.r, 128, "Color::GREY r");
	MT_ASSERT_EQ((int)Color::GREY.g, 128, "Color::GREY g");
	MT_ASSERT_EQ((int)Color::GREY.b, 128, "Color::GREY b");

	// overflow wrap behavior (byte arithmetic)
	Color cMax((byte)255u, (byte)255u, (byte)255u, (byte)255u);
	Color cOne((byte)1u, (byte)1u, (byte)1u, (byte)1u);
	Color overflow = cMax + cOne;
	MT_ASSERT_EQ((int)overflow.r, 0, "Color overflow r=0");
	MT_ASSERT_EQ((int)overflow.g, 0, "Color overflow g=0");
	MT_ASSERT_EQ((int)overflow.b, 0, "Color overflow b=0");

	// underflow
	Color cZero((byte)0u, (byte)0u, (byte)0u, (byte)0u);
	Color underflow = cZero - cOne;
	MT_ASSERT_EQ((int)underflow.r, 255, "Color underflow r=255");

	// -= operator
	Color a((byte)50u, (byte)60u, (byte)70u, (byte)80u);
	Color b((byte)10u, (byte)20u, (byte)30u, (byte)40u);
	a -= b;
	MT_ASSERT_EQ((int)a.r, 40, "Color -= r");
	MT_ASSERT_EQ((int)a.g, 40, "Color -= g");
	MT_ASSERT_EQ((int)a.b, 40, "Color -= b");
	MT_ASSERT_EQ((int)a.a, 40, "Color -= a");
}

//------------------------------------------------------------------------------
// Quaternion — extended
//------------------------------------------------------------------------------
static void testQuaternion_Conjugate()
{
	Quaternion q(0.1f, 0.2f, 0.3f, 0.9f);
	Quaternion conj = q.conjugate();
	MT_ASSERT_FLOAT(conj.x, -0.1f, "Quat conj x");
	MT_ASSERT_FLOAT(conj.y, -0.2f, "Quat conj y");
	MT_ASSERT_FLOAT(conj.z, -0.3f, "Quat conj z");
	MT_ASSERT_FLOAT(conj.w,  0.9f, "Quat conj w");
}

static void testQuaternion_Normalize()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion n = q.normalize();
	MT_ASSERT_FLOAT(n.length(), 1.0f, "Quat normalize length=1");
}

static void testQuaternion_AxisAngle()
{
	// Y axis rotation by π/2
	const Vector3 axisY(0.0f, 1.0f, 0.0f);
	const float halfPi = 1.5707963f;
	Quaternion q(axisY, halfPi);
	MT_ASSERT_FLOAT(q.length(), 1.0f, "Quat Y-axis pi/2 len=1");
	MT_ASSERT_FLOAT(q.angle(), halfPi, "Quat Y-axis pi/2 angle round-trip");

	// Z axis rotation by π/4
	const Vector3 axisZ(0.0f, 0.0f, 1.0f);
	const float quarterPi = 0.7853982f;
	Quaternion qz(axisZ, quarterPi);
	MT_ASSERT_FLOAT(qz.length(), 1.0f, "Quat Z-axis pi/4 len=1");
	MT_ASSERT_FLOAT(qz.angle(), quarterPi, "Quat Z-axis pi/4 angle round-trip");
}

static void testQuaternion_DotProduct()
{
	Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
	MT_ASSERT_FLOAT(Quaternion::dot(q1, q2), 1.0f, "Quat dot identity=1");

	Quaternion q3(1.0f, 0.0f, 0.0f, 0.0f);
	MT_ASSERT_FLOAT(Quaternion::dot(q1, q3), 0.0f, "Quat dot perp=0");
}

static void testQuaternion_Lerp()
{
	Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
	// lerp between identical quaternions = same quaternion
	Quaternion lerped = Quaternion::lerp(q1, q2, 0.5f);
	MT_ASSERT_FLOAT(lerped.x, 0.0f, "Quat lerp same x");
	MT_ASSERT_FLOAT(lerped.w, 1.0f, "Quat lerp same w");

	// lerp at t=0 -> q1
	Quaternion at0 = Quaternion::lerp(q1, q2, 0.0f);
	MT_ASSERT_FLOAT(at0.w, q1.w, "Quat lerp t=0 w=q1.w");

	// lerp at t=1 -> q2
	Quaternion at1 = Quaternion::lerp(q1, q2, 1.0f);
	MT_ASSERT_FLOAT(at1.w, q2.w, "Quat lerp t=1 w=q2.w");
}

static void testQuaternion_AddAndScale()
{
	Quaternion q1(0.1f, 0.2f, 0.3f, 0.4f);
	Quaternion q2(0.5f, 0.6f, 0.7f, 0.8f);
	Quaternion sum = q1 + q2;
	MT_ASSERT_FLOAT(sum.x, 0.6f, "Quat + x");
	MT_ASSERT_FLOAT(sum.y, 0.8f, "Quat + y");
	MT_ASSERT_FLOAT(sum.z, 1.0f, "Quat + z");
	MT_ASSERT_FLOAT(sum.w, 1.2f, "Quat + w");

	Quaternion scaled = q1 * 2.0f;
	MT_ASSERT_FLOAT(scaled.x, 0.2f, "Quat *2 x");
	MT_ASSERT_FLOAT(scaled.y, 0.4f, "Quat *2 y");
	MT_ASSERT_FLOAT(scaled.z, 0.6f, "Quat *2 z");
	MT_ASSERT_FLOAT(scaled.w, 0.8f, "Quat *2 w");
}

static void testQuaternion_CopyConstructor()
{
	Quaternion q1(0.1f, 0.2f, 0.3f, 0.4f);
	Quaternion q2(q1);
	MT_ASSERT_FLOAT(q2.x, q1.x, "Quat copy x");
	MT_ASSERT_FLOAT(q2.y, q1.y, "Quat copy y");
	MT_ASSERT_FLOAT(q2.z, q1.z, "Quat copy z");
	MT_ASSERT_FLOAT(q2.w, q1.w, "Quat copy w");
}

static void testQuaternion_Negate()
{
	Quaternion q(0.1f, -0.2f, 0.3f, 0.9f);
	Quaternion neg = -q;
	MT_ASSERT_FLOAT(neg.x, -0.1f, "Quat neg x");
	MT_ASSERT_FLOAT(neg.y,  0.2f, "Quat neg y");
	MT_ASSERT_FLOAT(neg.z, -0.3f, "Quat neg z");
	MT_ASSERT_FLOAT(neg.w, -0.9f, "Quat neg w");
}

static void testQuaternion_MultiplyAssignScale()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	q *= 0.5f;
	MT_ASSERT_FLOAT(q.x, 0.5f, "Quat *=0.5 x");
	MT_ASSERT_FLOAT(q.y, 1.0f, "Quat *=0.5 y");
	MT_ASSERT_FLOAT(q.z, 1.5f, "Quat *=0.5 z");
	MT_ASSERT_FLOAT(q.w, 2.0f, "Quat *=0.5 w");
}

static void testQuaternion_DivideAssignScale()
{
	Quaternion q(2.0f, 4.0f, 6.0f, 8.0f);
	q /= 2.0f;
	MT_ASSERT_FLOAT(q.x, 1.0f, "Quat /=2 x");
	MT_ASSERT_FLOAT(q.y, 2.0f, "Quat /=2 y");
	MT_ASSERT_FLOAT(q.z, 3.0f, "Quat /=2 z");
	MT_ASSERT_FLOAT(q.w, 4.0f, "Quat /=2 w");
}

static void testQuaternion_EulerAngles()
{
	// Identity quaternion should give (0,0,0) euler angles
	Quaternion identity;
	Vector3 euler = identity.eulerAngles();
	MT_ASSERT_FLOAT(euler.x, 0.0f, "Quat identity euler x=0");
	MT_ASSERT_FLOAT(euler.y, 0.0f, "Quat identity euler y=0");
	MT_ASSERT_FLOAT(euler.z, 0.0f, "Quat identity euler z=0");

	MT_ASSERT_FLOAT(identity.pitch(), 0.0f, "Quat identity pitch=0");
	MT_ASSERT_FLOAT(identity.yaw(), 0.0f, "Quat identity yaw=0");
	MT_ASSERT_FLOAT(identity.roll(), 0.0f, "Quat identity roll=0");
}

static void testQuaternion_Inverse()
{
	// For a unit quaternion, inverse = conjugate
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion inv = q.inverse();
	MT_ASSERT_FLOAT(inv.x, 0.0f, "Quat inv identity x");
	MT_ASSERT_FLOAT(inv.y, 0.0f, "Quat inv identity y");
	MT_ASSERT_FLOAT(inv.z, 0.0f, "Quat inv identity z");
	MT_ASSERT_FLOAT(inv.w, 1.0f, "Quat inv identity w");
}

//------------------------------------------------------------------------------
// Cross-type interactions
//------------------------------------------------------------------------------
static void testCrossType_Vector2ToVec3()
{
	// A Vector3 can be constructed from three floats like a Vector2 + z
	Vector2 v2(1.0f, 2.0f);
	Vector3 v3(v2.x, v2.y, 0.0f);
	MT_ASSERT_FLOAT(v3.x, v2.x, "V2->V3 x");
	MT_ASSERT_FLOAT(v3.y, v2.y, "V2->V3 y");
	MT_ASSERT_FLOAT(v3.z, 0.0f, "V2->V3 z=0");
}

static void testCrossType_V3AsQuatAxis()
{
	// Standard rotation axes should produce unit quaternions
	const float angle = 1.0f;
	Quaternion qx(Vector3(1.0f, 0.0f, 0.0f), angle);
	Quaternion qy(Vector3(0.0f, 1.0f, 0.0f), angle);
	Quaternion qz(Vector3(0.0f, 0.0f, 1.0f), angle);
	MT_ASSERT_FLOAT(qx.length(), 1.0f, "qx length=1");
	MT_ASSERT_FLOAT(qy.length(), 1.0f, "qy length=1");
	MT_ASSERT_FLOAT(qz.length(), 1.0f, "qz length=1");
}

static void testCrossType_ColorArithmetic()
{
	// Multiple additions
	Color c((byte)0u, (byte)0u, (byte)0u, (byte)0u);
	FOR(10)
	{
		c += Color((byte)10u, (byte)5u, (byte)3u, (byte)1u);
	}
	MT_ASSERT_EQ((int)c.r, 100, "Color accum r");
	MT_ASSERT_EQ((int)c.g, 50, "Color accum g");
	MT_ASSERT_EQ((int)c.b, 30, "Color accum b");
	MT_ASSERT_EQ((int)c.a, 10, "Color accum a");
}

//------------------------------------------------------------------------------
// Entry point
//------------------------------------------------------------------------------
void MathTest::test()
{
	testVector2_Operators();
	testVector3_Operators();
	testVector2Int_Operators();
	testColor_Operators();
	testQuaternion_Basic();
	testVector2_StaticConstants();
	testVector2_IntConstructor();
	testVector2_ChainedOps();
	testVector2_NegativeValues();
	testVector2_SmallValues();
	testVector2_LargeValues();
	testVector3_StaticConstants();
	testVector3_IntConstructor();
	testVector3_MultiplyScalarBothSides();
	testVector3_NegativeAndClear();
	testVector3_ChainedArithmetic();
	testVector2Int_Extended();
	testColor_Extended();
	testQuaternion_Conjugate();
	testQuaternion_Normalize();
	testQuaternion_AxisAngle();
	testQuaternion_DotProduct();
	testQuaternion_Lerp();
	testQuaternion_AddAndScale();
	testQuaternion_CopyConstructor();
	testQuaternion_Negate();
	testQuaternion_MultiplyAssignScale();
	testQuaternion_DivideAssignScale();
	testQuaternion_EulerAngles();
	testQuaternion_Inverse();
	testCrossType_Vector2ToVec3();
	testCrossType_V3AsQuatAxis();
	testCrossType_ColorArithmetic();
}
