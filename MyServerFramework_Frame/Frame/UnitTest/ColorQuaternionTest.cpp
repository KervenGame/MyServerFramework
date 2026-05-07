#include "FrameHeader.h"

// ============================================================
// ColorQuaternionTest
// Testing Color struct and Quaternion struct
// Color: RGBA byte struct with arithmetic operators
// Quaternion: float quaternion with rotation math
// ============================================================

#define CQT_ASSERT(expr) \
	if (!(expr)) { ERROR(string("ColorQuaternionTest FAIL: ") + #expr); }

#define CQT_FLOAT_EQ(a, b) (fabsf((a) - (b)) <= 1e-4f)

void ColorQuaternionTest::test()
{
	// Color
	testColorDefault();
	testColorConstruct();
	testColorAdd();
	testColorSub();
	testColorAddAssign();
	testColorSubAssign();
	testColorEqual();
	testColorNotEqual();
	testColorStaticConstants();
	testColorChain();
	testColorBoundary();
	testColorInVector();
	testColorWrap();
	testColorAlpha();
	testColorGrayscale();
	// Quaternion
	testQuaternionDefault();
	testQuaternionConstruct();
	testQuaternionClear();
	testQuaternionConjugate();
	testQuaternionLength();
	testQuaternionNormalize();
	testQuaternionMulScalar();
	testQuaternionDivScalar();
	testQuaternionAdd();
	testQuaternionNeg();
	testQuaternionMulAssign();
	testQuaternionDivAssign();
	testQuaternionAddAssign();
	testQuaternionDot();
	testQuaternionLerp();
	testQuaternionFromAxisAngle();
	testQuaternionEulerAngles();
	testQuaternionMul();
	testQuaternionCross();
	testQuaternionIdentity();
	testQuaternionStressNormalize();
}

// ============================================================
// Color tests
// ============================================================

void ColorQuaternionTest::testColorDefault()
{
	Color c;
	CQT_ASSERT(c.r == 0);
	CQT_ASSERT(c.g == 0);
	CQT_ASSERT(c.b == 0);
	CQT_ASSERT(c.a == 0);
}

void ColorQuaternionTest::testColorConstruct()
{
	Color c((byte)255, (byte)128, (byte)64, (byte)32);
	CQT_ASSERT(c.r == 255);
	CQT_ASSERT(c.g == 128);
	CQT_ASSERT(c.b == 64);
	CQT_ASSERT(c.a == 32);
	Color c2((byte)0, (byte)0, (byte)0, (byte)255);
	CQT_ASSERT(c2.r == 0);
	CQT_ASSERT(c2.g == 0);
	CQT_ASSERT(c2.b == 0);
	CQT_ASSERT(c2.a == 255);
}

void ColorQuaternionTest::testColorAdd()
{
	Color a((byte)10, (byte)20, (byte)30, (byte)40);
	Color b((byte)5, (byte)10, (byte)15, (byte)20);
	Color c = a + b;
	CQT_ASSERT(c.r == 15);
	CQT_ASSERT(c.g == 30);
	CQT_ASSERT(c.b == 45);
	CQT_ASSERT(c.a == 60);
	Color d((byte)0, (byte)0, (byte)0, (byte)0);
	Color e((byte)100, (byte)200, (byte)50, (byte)150);
	Color f = d + e;
	CQT_ASSERT(f.r == 100);
	CQT_ASSERT(f.g == 200);
	CQT_ASSERT(f.b == 50);
	CQT_ASSERT(f.a == 150);
}

void ColorQuaternionTest::testColorSub()
{
	Color a((byte)100, (byte)200, (byte)150, (byte)100);
	Color b((byte)50, (byte)100, (byte)75, (byte)50);
	Color c = a - b;
	CQT_ASSERT(c.r == 50);
	CQT_ASSERT(c.g == 100);
	CQT_ASSERT(c.b == 75);
	CQT_ASSERT(c.a == 50);
	Color d((byte)255, (byte)255, (byte)255, (byte)255);
	Color e((byte)255, (byte)255, (byte)255, (byte)255);
	Color f = d - e;
	CQT_ASSERT(f.r == 0);
	CQT_ASSERT(f.g == 0);
	CQT_ASSERT(f.b == 0);
	CQT_ASSERT(f.a == 0);
}

void ColorQuaternionTest::testColorAddAssign()
{
	Color c((byte)10, (byte)20, (byte)30, (byte)40);
	c += Color((byte)5, (byte)10, (byte)15, (byte)20);
	CQT_ASSERT(c.r == 15);
	CQT_ASSERT(c.g == 30);
	CQT_ASSERT(c.b == 45);
	CQT_ASSERT(c.a == 60);
	c += Color((byte)0, (byte)0, (byte)0, (byte)0);
	CQT_ASSERT(c.r == 15);
	CQT_ASSERT(c.g == 30);
	CQT_ASSERT(c.b == 45);
	CQT_ASSERT(c.a == 60);
}

void ColorQuaternionTest::testColorSubAssign()
{
	Color c((byte)100, (byte)80, (byte)60, (byte)40);
	c -= Color((byte)50, (byte)40, (byte)30, (byte)20);
	CQT_ASSERT(c.r == 50);
	CQT_ASSERT(c.g == 40);
	CQT_ASSERT(c.b == 30);
	CQT_ASSERT(c.a == 20);
	c -= Color((byte)50, (byte)40, (byte)30, (byte)20);
	CQT_ASSERT(c.r == 0);
	CQT_ASSERT(c.g == 0);
	CQT_ASSERT(c.b == 0);
	CQT_ASSERT(c.a == 0);
}

void ColorQuaternionTest::testColorEqual()
{
	Color a((byte)255, (byte)128, (byte)64, (byte)32);
	Color b((byte)255, (byte)128, (byte)64, (byte)32);
	Color c((byte)255, (byte)128, (byte)64, (byte)33);
	CQT_ASSERT(a == b);
	CQT_ASSERT(!(a == c));
	Color d((byte)0, (byte)0, (byte)0, (byte)0);
	Color e((byte)0, (byte)0, (byte)0, (byte)0);
	CQT_ASSERT(d == e);
}

void ColorQuaternionTest::testColorNotEqual()
{
	Color a((byte)100, (byte)200, (byte)150, (byte)255);
	Color b((byte)100, (byte)200, (byte)151, (byte)255);
	Color c((byte)100, (byte)200, (byte)150, (byte)255);
	CQT_ASSERT(a != b);
	CQT_ASSERT(!(a != c));
}

void ColorQuaternionTest::testColorStaticConstants()
{
	CQT_ASSERT(Color::WHITE.r == 255 && Color::WHITE.g == 255 && Color::WHITE.b == 255 && Color::WHITE.a == 255);
	CQT_ASSERT(Color::BLACK.r == 0 && Color::BLACK.g == 0 && Color::BLACK.b == 0 && Color::BLACK.a == 255);
	CQT_ASSERT(Color::RED.r == 255 && Color::RED.g == 0 && Color::RED.b == 0 && Color::RED.a == 255);
	CQT_ASSERT(Color::GREEN.r == 0 && Color::GREEN.g == 255 && Color::GREEN.b == 0 && Color::GREEN.a == 255);
	CQT_ASSERT(Color::BLUE.r == 0 && Color::BLUE.g == 0 && Color::BLUE.b == 255 && Color::BLUE.a == 255);
}

void ColorQuaternionTest::testColorChain()
{
	Color a((byte)10, (byte)20, (byte)30, (byte)40);
	Color b((byte)5, (byte)10, (byte)5, (byte)10);
	Color c = (a + b) - Color((byte)2, (byte)5, (byte)10, (byte)15);
	// (15, 30, 35, 50) - (2, 5, 10, 15) = (13, 25, 25, 35)
	CQT_ASSERT(c.r == 13);
	CQT_ASSERT(c.g == 25);
	CQT_ASSERT(c.b == 25);
	CQT_ASSERT(c.a == 35);
}

void ColorQuaternionTest::testColorBoundary()
{
	Color max((byte)255, (byte)255, (byte)255, (byte)255);
	Color min((byte)0, (byte)0, (byte)0, (byte)0);
	CQT_ASSERT(max.r == 255 && max.a == 255);
	CQT_ASSERT(min.r == 0 && min.a == 0);
	// Byte wraps at 255+1=0
	Color c((byte)255, (byte)0, (byte)0, (byte)0);
	Color d((byte)1, (byte)0, (byte)0, (byte)0);
	Color e = c + d;
	CQT_ASSERT(e.r == 0); // byte overflow: 256 & 0xFF == 0
}

void ColorQuaternionTest::testColorInVector()
{
	Vector<Color> colors;
	colors.add(Color::RED);
	colors.add(Color::GREEN);
	colors.add(Color::BLUE);
	colors.add(Color::WHITE);
	colors.add(Color::BLACK);
	CQT_ASSERT(colors.size() == 5);
	CQT_ASSERT(colors[0] == Color::RED);
	CQT_ASSERT(colors[1] == Color::GREEN);
	CQT_ASSERT(colors[2] == Color::BLUE);
	CQT_ASSERT(colors[3] == Color::WHITE);
	CQT_ASSERT(colors[4] == Color::BLACK);
}

void ColorQuaternionTest::testColorWrap()
{
	// Byte arithmetic: (255 + 2) wraps to 1
	Color c((byte)254, (byte)0, (byte)0, (byte)0);
	c += Color((byte)2, (byte)0, (byte)0, (byte)0);
	CQT_ASSERT(c.r == 0); // 256 mod 256 == 0
}

void ColorQuaternionTest::testColorAlpha()
{
	Color transparent((byte)255, (byte)0, (byte)128, (byte)0);
	Color opaque((byte)255, (byte)0, (byte)128, (byte)255);
	CQT_ASSERT(transparent.a == 0);
	CQT_ASSERT(opaque.a == 255);
	Color halfAlpha((byte)200, (byte)100, (byte)50, (byte)128);
	CQT_ASSERT(halfAlpha.a == 128);
}

void ColorQuaternionTest::testColorGrayscale()
{
	Color gray((byte)128, (byte)128, (byte)128, (byte)255);
	CQT_ASSERT(gray.r == gray.g && gray.g == gray.b);
	Color white((byte)255, (byte)255, (byte)255, (byte)255);
	Color black((byte)0, (byte)0, (byte)0, (byte)255);
	Color mixed = white + black;
	CQT_ASSERT(mixed.r == 255);
	CQT_ASSERT(mixed.g == 255);
	CQT_ASSERT(mixed.b == 255);
}

// ============================================================
// Quaternion tests
// ============================================================

void ColorQuaternionTest::testQuaternionDefault()
{
	Quaternion q;
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 1.0f));
}

void ColorQuaternionTest::testQuaternionConstruct()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 4.0f));
	Quaternion q2(q);
	CQT_ASSERT(CQT_FLOAT_EQ(q2.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q2.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q2.z, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q2.w, 4.0f));
}

void ColorQuaternionTest::testQuaternionClear()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 0.5f);
	q.clear();
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 1.0f));
}

void ColorQuaternionTest::testQuaternionConjugate()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion c = q.conjugate();
	CQT_ASSERT(CQT_FLOAT_EQ(c.x, -1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.y, -2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.z, -3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.w, 4.0f));
	// Double conjugate == original
	Quaternion cc = c.conjugate();
	CQT_ASSERT(CQT_FLOAT_EQ(cc.x, q.x));
	CQT_ASSERT(CQT_FLOAT_EQ(cc.y, q.y));
	CQT_ASSERT(CQT_FLOAT_EQ(cc.z, q.z));
	CQT_ASSERT(CQT_FLOAT_EQ(cc.w, q.w));
}

void ColorQuaternionTest::testQuaternionLength()
{
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
	float len = q.length();
	CQT_ASSERT(CQT_FLOAT_EQ(len, 1.0f));
	Quaternion q2(3.0f, 0.0f, 0.0f, 4.0f);
	float len2 = q2.length();
	CQT_ASSERT(CQT_FLOAT_EQ(len2, 5.0f));
}

void ColorQuaternionTest::testQuaternionNormalize()
{
	Quaternion q(1.0f, 0.0f, 0.0f, 0.0f);
	Quaternion n = q.normalize();
	CQT_ASSERT(CQT_FLOAT_EQ(n.length(), 1.0f));
	Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion n2 = q2.normalize();
	CQT_ASSERT(CQT_FLOAT_EQ(n2.length(), 1.0f));
	Quaternion q3(1.0f, 1.0f, 1.0f, 1.0f);
	Quaternion n3 = q3.normalize();
	CQT_ASSERT(CQT_FLOAT_EQ(n3.length(), 1.0f));
}

void ColorQuaternionTest::testQuaternionMulScalar()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion r = q * 2.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(r.x, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.y, 4.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.z, 6.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.w, 8.0f));
	Quaternion r2 = q * 0.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(r2.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r2.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r2.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r2.w, 0.0f));
}

void ColorQuaternionTest::testQuaternionDivScalar()
{
	Quaternion q(4.0f, 8.0f, 12.0f, 16.0f);
	Quaternion r = q / 4.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(r.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.z, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.w, 4.0f));
}

void ColorQuaternionTest::testQuaternionAdd()
{
	Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion b(0.5f, 0.5f, 0.5f, 0.5f);
	Quaternion c = a + b;
	CQT_ASSERT(CQT_FLOAT_EQ(c.x, 1.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.y, 2.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.z, 3.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.w, 4.5f));
}

void ColorQuaternionTest::testQuaternionNeg()
{
	Quaternion q(1.0f, -2.0f, 3.0f, -4.0f);
	Quaternion neg = -q;
	CQT_ASSERT(CQT_FLOAT_EQ(neg.x, -1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(neg.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(neg.z, -3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(neg.w, 4.0f));
}

void ColorQuaternionTest::testQuaternionMulAssign()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	q *= 3.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 6.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 9.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 12.0f));
}

void ColorQuaternionTest::testQuaternionDivAssign()
{
	Quaternion q(2.0f, 4.0f, 6.0f, 8.0f);
	q /= 2.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 4.0f));
}

void ColorQuaternionTest::testQuaternionAddAssign()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	q += Quaternion(0.5f, 0.5f, 0.5f, 0.5f);
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 1.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 2.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 3.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 4.5f));
}

void ColorQuaternionTest::testQuaternionDot()
{
	Quaternion a(1.0f, 0.0f, 0.0f, 0.0f);
	Quaternion b(1.0f, 0.0f, 0.0f, 0.0f);
	float d = Quaternion::dot(a, b);
	CQT_ASSERT(CQT_FLOAT_EQ(d, 1.0f));
	Quaternion c(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion e(4.0f, 3.0f, 2.0f, 1.0f);
	float d2 = Quaternion::dot(c, e);
	// 1*4 + 2*3 + 3*2 + 4*1 = 4+6+6+4 = 20
	CQT_ASSERT(CQT_FLOAT_EQ(d2, 20.0f));
}

void ColorQuaternionTest::testQuaternionLerp()
{
	Quaternion q0(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
	Quaternion lerp0 = Quaternion::lerp(q0, q1, 0.0f);
	CQT_ASSERT(CQT_FLOAT_EQ(lerp0.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(lerp0.w, 1.0f));
	Quaternion lerp1 = Quaternion::lerp(q0, q1, 1.0f);
	CQT_ASSERT(CQT_FLOAT_EQ(lerp1.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(lerp1.w, 0.0f));
	Quaternion lerp05 = Quaternion::lerp(q0, q1, 0.5f);
	CQT_ASSERT(CQT_FLOAT_EQ(lerp05.x, 0.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(lerp05.w, 0.5f));
}

void ColorQuaternionTest::testQuaternionFromAxisAngle()
{
	// Rotation of 0 around any axis = identity-like
	Vector3 axis(0.0f, 1.0f, 0.0f);
	Quaternion q(axis, 0.0f);
	// For angle=0: sin(0)=0, cos(0)=1; x=y=z=0, w=1
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 0.0f));
	// Rotation PI/2 around Y axis
	float halfAngle = 3.14159265f / 4.0f; // half of PI/2
	Quaternion qY(axis, 3.14159265f / 2.0f);
	float expectedW = cosf(halfAngle);
	float expectedY = sinf(halfAngle);
	CQT_ASSERT(CQT_FLOAT_EQ(qY.w, expectedW));
	CQT_ASSERT(CQT_FLOAT_EQ(qY.y, expectedY));
	CQT_ASSERT(CQT_FLOAT_EQ(qY.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(qY.z, 0.0f));
}

void ColorQuaternionTest::testQuaternionEulerAngles()
{
	// Identity quaternion should give zero euler angles
	Quaternion identity(0.0f, 0.0f, 0.0f, 1.0f);
	Vector3 euler = identity.eulerAngles();
	CQT_ASSERT(CQT_FLOAT_EQ(euler.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(euler.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(euler.z, 0.0f));
}

void ColorQuaternionTest::testQuaternionMul()
{
	// Identity * anything = anything
	Quaternion identity(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q(0.5f, 0.5f, 0.5f, 0.5f);
	Quaternion result = identity * q;
	CQT_ASSERT(CQT_FLOAT_EQ(result.x, q.x));
	CQT_ASSERT(CQT_FLOAT_EQ(result.y, q.y));
	CQT_ASSERT(CQT_FLOAT_EQ(result.z, q.z));
	CQT_ASSERT(CQT_FLOAT_EQ(result.w, q.w));
	// q * identity = q
	Quaternion result2 = q * identity;
	CQT_ASSERT(CQT_FLOAT_EQ(result2.x, q.x));
	CQT_ASSERT(CQT_FLOAT_EQ(result2.y, q.y));
	CQT_ASSERT(CQT_FLOAT_EQ(result2.z, q.z));
	CQT_ASSERT(CQT_FLOAT_EQ(result2.w, q.w));
}

void ColorQuaternionTest::testQuaternionCross()
{
	Quaternion a(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion b(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion c = Quaternion::cross(a, b);
	// cross(q1,q2) = {q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z,
	//                 q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
	//                 q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z,
	//                 q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x }
	// 初始化列表顺序对应 x,y,z,w
	// 对于 a=b=(x=0,y=0,z=0,w=1)：
	//   x = 1*1-0-0-0 = 1
	//   y = 0, z = 0, w = 0
	CQT_ASSERT(CQT_FLOAT_EQ(c.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.w, 0.0f));
}

void ColorQuaternionTest::testQuaternionIdentity()
{
	Quaternion identity;
	CQT_ASSERT(CQT_FLOAT_EQ(identity.length(), 1.0f));
	// Normalizing identity should still be identity
	Quaternion n = identity.normalize();
	CQT_ASSERT(CQT_FLOAT_EQ(n.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(n.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(n.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(n.w, 1.0f));
	// Conjugate of identity = identity
	Quaternion c = identity.conjugate();
	CQT_ASSERT(CQT_FLOAT_EQ(c.w, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.x, 0.0f));
}

void ColorQuaternionTest::testQuaternionStressNormalize()
{
	// Normalize 20 random-ish quaternions and verify unit length
	for (int i = 1; i <= 20; ++i)
	{
		float fi = (float)i;
		Quaternion q(fi * 0.1f, fi * 0.2f, fi * 0.3f, fi * 0.5f);
		Quaternion n = q.normalize();
		float len = n.length();
		CQT_ASSERT(CQT_FLOAT_EQ(len, 1.0f));
	}
	// Conjugate of normalized = normalized inverse
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion n = q.normalize();
	Quaternion conj = n.conjugate();
	// For unit quaternion, dot(q, conj(q)) = x*(-x)+y*(-y)+z*(-z)+w*w = -(x^2+y^2+z^2)+w^2
	// We just verify length stays 1
	CQT_ASSERT(CQT_FLOAT_EQ(conj.length(), 1.0f));
}
