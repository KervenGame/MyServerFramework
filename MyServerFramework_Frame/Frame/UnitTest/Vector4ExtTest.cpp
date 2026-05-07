#include "FrameHeader.h"

#define V4ET_ASSERT(expr) \
	if (!(expr)) { ERROR(string("Vector4ExtTest FAILED: ") + (#expr)); }

#define V4ET_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)

// ========== Vector3Int ==========

static void testV4ET_Vector3IntDefault()
{
	Vector3Int v;
	V4ET_ASSERT(v.x == 0);
	V4ET_ASSERT(v.y == 0);
	V4ET_ASSERT(v.z == 0);
}

static void testV4ET_Vector3IntConstruct()
{
	Vector3Int v(3, -5, 7);
	V4ET_ASSERT(v.x == 3);
	V4ET_ASSERT(v.y == -5);
	V4ET_ASSERT(v.z == 7);
}

static void testV4ET_Vector3IntClear()
{
	Vector3Int v(10, 20, 30);
	v.clear();
	V4ET_ASSERT(v.x == 0);
	V4ET_ASSERT(v.y == 0);
	V4ET_ASSERT(v.z == 0);
}

static void testV4ET_Vector3IntAdd()
{
	Vector3Int a(1, 2, 3);
	Vector3Int b(4, 5, 6);
	Vector3Int c = a + b;
	V4ET_ASSERT(c.x == 5);
	V4ET_ASSERT(c.y == 7);
	V4ET_ASSERT(c.z == 9);
}

static void testV4ET_Vector3IntSub()
{
	Vector3Int a(10, 8, 6);
	Vector3Int b(1, 3, 2);
	Vector3Int c = a - b;
	V4ET_ASSERT(c.x == 9);
	V4ET_ASSERT(c.y == 5);
	V4ET_ASSERT(c.z == 4);
}

static void testV4ET_Vector3IntMulScalar()
{
	Vector3Int v(2, -3, 4);
	Vector3Int r = v * 3;
	V4ET_ASSERT(r.x == 6);
	V4ET_ASSERT(r.y == -9);
	V4ET_ASSERT(r.z == 12);
}

static void testV4ET_Vector3IntAddAssign()
{
	Vector3Int v(1, 2, 3);
	Vector3Int d(10, 20, 30);
	v += d;
	V4ET_ASSERT(v.x == 11);
	V4ET_ASSERT(v.y == 22);
	V4ET_ASSERT(v.z == 33);
}

static void testV4ET_Vector3IntSubAssign()
{
	Vector3Int v(10, 20, 30);
	Vector3Int d(1, 2, 3);
	v -= d;
	V4ET_ASSERT(v.x == 9);
	V4ET_ASSERT(v.y == 18);
	V4ET_ASSERT(v.z == 27);
}

static void testV4ET_Vector3IntNegate()
{
	Vector3Int v(5, -3, 7);
	Vector3Int neg = -v;
	V4ET_ASSERT(neg.x == -5);
	V4ET_ASSERT(neg.y == 3);
	V4ET_ASSERT(neg.z == -7);
}

static void testV4ET_Vector3IntEqual()
{
	Vector3Int a(1, 2, 3);
	Vector3Int b(1, 2, 3);
	V4ET_ASSERT(a == b);
	Vector3Int c(1, 2, 4);
	V4ET_ASSERT(!(a == c));
}

static void testV4ET_Vector3IntNotEqual()
{
	Vector3Int a(1, 2, 3);
	Vector3Int b(1, 2, 4);
	V4ET_ASSERT(a != b);
	Vector3Int c(1, 2, 3);
	V4ET_ASSERT(!(a != c));
}

static void testV4ET_Vector3IntLess()
{
	// operator< compares sum
	Vector3Int a(1, 1, 1); // sum=3
	Vector3Int b(2, 2, 2); // sum=6
	V4ET_ASSERT(a < b);
	V4ET_ASSERT(!(b < a));
	Vector3Int c(1, 1, 1); // sum=3
	V4ET_ASSERT(!(a < c));
}

// ========== Vector4 ==========

static void testV4ET_Vector4Default()
{
	Vector4 v;
	V4ET_ASSERT(V4ET_NEAR(v.x, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 0.0f));
}

static void testV4ET_Vector4Construct()
{
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	V4ET_ASSERT(V4ET_NEAR(v.x, 1.0f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 2.0f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 3.0f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 4.0f));
}

static void testV4ET_Vector4Clear()
{
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	v.clear();
	V4ET_ASSERT(V4ET_NEAR(v.x, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 0.0f));
}

static void testV4ET_Vector4Add()
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(0.5f, 1.5f, 2.5f, 3.5f);
	Vector4 c = a + b;
	V4ET_ASSERT(V4ET_NEAR(c.x, 1.5f));
	V4ET_ASSERT(V4ET_NEAR(c.y, 3.5f));
	V4ET_ASSERT(V4ET_NEAR(c.z, 5.5f));
	V4ET_ASSERT(V4ET_NEAR(c.w, 7.5f));
}

static void testV4ET_Vector4Sub()
{
	Vector4 a(5.0f, 4.0f, 3.0f, 2.0f);
	Vector4 b(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 c = a - b;
	V4ET_ASSERT(V4ET_NEAR(c.x, 4.0f));
	V4ET_ASSERT(V4ET_NEAR(c.y, 3.0f));
	V4ET_ASSERT(V4ET_NEAR(c.z, 2.0f));
	V4ET_ASSERT(V4ET_NEAR(c.w, 1.0f));
}

static void testV4ET_Vector4AddAssign()
{
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 d(0.1f, 0.2f, 0.3f, 0.4f);
	v += d;
	V4ET_ASSERT(V4ET_NEAR(v.x, 1.1f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 2.2f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 3.3f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 4.4f));
}

static void testV4ET_Vector4SubAssign()
{
	Vector4 v(2.0f, 4.0f, 6.0f, 8.0f);
	Vector4 d(1.0f, 1.0f, 1.0f, 1.0f);
	v -= d;
	V4ET_ASSERT(V4ET_NEAR(v.x, 1.0f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 3.0f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 5.0f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 7.0f));
}

static void testV4ET_Vector4Negate()
{
	Vector4 v(1.0f, -2.0f, 3.0f, -4.0f);
	Vector4 neg = -v;
	V4ET_ASSERT(V4ET_NEAR(neg.x, -1.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.y, 2.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.z, -3.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.w, 4.0f));
}

static void testV4ET_Vector4ZeroComponents()
{
	Vector4 a(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 c = a + b;
	V4ET_ASSERT(V4ET_NEAR(c.x, 1.0f));
	V4ET_ASSERT(V4ET_NEAR(c.w, 4.0f));
}

static void testV4ET_Vector4NegativeComponents()
{
	Vector4 v(-1.0f, -2.0f, -3.0f, -4.0f);
	Vector4 neg = -v;
	V4ET_ASSERT(V4ET_NEAR(neg.x, 1.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.y, 2.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.z, 3.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.w, 4.0f));
}

// ========== Vector4Int ==========

static void testV4ET_Vector4IntDefault()
{
	Vector4Int v;
	V4ET_ASSERT(v.x == 0);
	V4ET_ASSERT(v.y == 0);
	V4ET_ASSERT(v.z == 0);
	V4ET_ASSERT(v.w == 0);
}

static void testV4ET_Vector4IntConstruct()
{
	Vector4Int v(1, 2, 3, 4);
	V4ET_ASSERT(v.x == 1);
	V4ET_ASSERT(v.y == 2);
	V4ET_ASSERT(v.z == 3);
	V4ET_ASSERT(v.w == 4);
}

static void testV4ET_Vector4IntClear()
{
	Vector4Int v(10, 20, 30, 40);
	v.clear();
	V4ET_ASSERT(v.x == 0);
	V4ET_ASSERT(v.y == 0);
	V4ET_ASSERT(v.z == 0);
	V4ET_ASSERT(v.w == 0);
}

static void testV4ET_Vector4IntAdd()
{
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(10, 20, 30, 40);
	Vector4Int c = a + b;
	V4ET_ASSERT(c.x == 11);
	V4ET_ASSERT(c.y == 22);
	V4ET_ASSERT(c.z == 33);
	V4ET_ASSERT(c.w == 44);
}

static void testV4ET_Vector4IntSub()
{
	Vector4Int a(10, 20, 30, 40);
	Vector4Int b(1, 2, 3, 4);
	Vector4Int c = a - b;
	V4ET_ASSERT(c.x == 9);
	V4ET_ASSERT(c.y == 18);
	V4ET_ASSERT(c.z == 27);
	V4ET_ASSERT(c.w == 36);
}

static void testV4ET_Vector4IntAddAssign()
{
	Vector4Int v(1, 2, 3, 4);
	Vector4Int d(5, 6, 7, 8);
	v += d;
	V4ET_ASSERT(v.x == 6);
	V4ET_ASSERT(v.y == 8);
	V4ET_ASSERT(v.z == 10);
	V4ET_ASSERT(v.w == 12);
}

static void testV4ET_Vector4IntSubAssign()
{
	Vector4Int v(10, 20, 30, 40);
	Vector4Int d(1, 2, 3, 4);
	v -= d;
	V4ET_ASSERT(v.x == 9);
	V4ET_ASSERT(v.y == 18);
	V4ET_ASSERT(v.z == 27);
	V4ET_ASSERT(v.w == 36);
}

static void testV4ET_Vector4IntNegate()
{
	Vector4Int v(1, -2, 3, -4);
	Vector4Int neg = -v;
	V4ET_ASSERT(neg.x == -1);
	V4ET_ASSERT(neg.y == 2);
	V4ET_ASSERT(neg.z == -3);
	V4ET_ASSERT(neg.w == 4);
}

static void testV4ET_Vector4IntMulScalar()
{
	Vector4Int v(1, 2, 3, 4);
	Vector4Int r = v * 5;
	V4ET_ASSERT(r.x == 5);
	V4ET_ASSERT(r.y == 10);
	V4ET_ASSERT(r.z == 15);
	V4ET_ASSERT(r.w == 20);
}

static void testV4ET_Vector4IntEqual()
{
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(1, 2, 3, 4);
	V4ET_ASSERT(a == b);
	Vector4Int c(1, 2, 3, 5);
	V4ET_ASSERT(!(a == c));
}

static void testV4ET_Vector4IntNotEqual()
{
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(1, 2, 3, 5);
	V4ET_ASSERT(a != b);
	Vector4Int c(1, 2, 3, 4);
	V4ET_ASSERT(!(a != c));
}

static void testV4ET_Vector4IntLess()
{
	// operator< compares x+y+z+w
	Vector4Int a(1, 1, 1, 1); // sum=4
	Vector4Int b(2, 2, 2, 2); // sum=8
	V4ET_ASSERT(a < b);
	V4ET_ASSERT(!(b < a));
	Vector4Int c(1, 1, 1, 1); // sum=4
	V4ET_ASSERT(!(a < c));
}

static void testV4ET_Vector4IntChain()
{
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(5, 6, 7, 8);
	Vector4Int c(1, 1, 1, 1);
	// (a+b)-c
	Vector4Int r = (a + b) - c;
	V4ET_ASSERT(r.x == 5);
	V4ET_ASSERT(r.y == 7);
	V4ET_ASSERT(r.z == 9);
	V4ET_ASSERT(r.w == 11);
}

static void testV4ET_Vector4IntNegateChain()
{
	Vector4Int v(3, 4, 5, 6);
	Vector4Int neg = -v;
	Vector4Int back = -neg;
	V4ET_ASSERT(back == v);
}

static void testV4ET_Vector3IntChain()
{
	Vector3Int a(1, 2, 3);
	Vector3Int b(4, 5, 6);
	Vector3Int c(1, 1, 1);
	Vector3Int r = (a + b) - c;
	V4ET_ASSERT(r.x == 4);
	V4ET_ASSERT(r.y == 6);
	V4ET_ASSERT(r.z == 8);
}

static void testV4ET_Vector3IntMulZero()
{
	Vector3Int v(5, 10, 15);
	Vector3Int r = v * 0;
	V4ET_ASSERT(r.x == 0);
	V4ET_ASSERT(r.y == 0);
	V4ET_ASSERT(r.z == 0);
}

static void testV4ET_Vector4SubSelf()
{
	Vector4 v(3.0f, 3.0f, 3.0f, 3.0f);
	Vector4 r = v - v;
	V4ET_ASSERT(V4ET_NEAR(r.x, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(r.y, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(r.z, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(r.w, 0.0f));
}

void Vector4ExtTest::test()
{
	testV4ET_Vector3IntDefault();
	testV4ET_Vector3IntConstruct();
	testV4ET_Vector3IntClear();
	testV4ET_Vector3IntAdd();
	testV4ET_Vector3IntSub();
	testV4ET_Vector3IntMulScalar();
	testV4ET_Vector3IntAddAssign();
	testV4ET_Vector3IntSubAssign();
	testV4ET_Vector3IntNegate();
	testV4ET_Vector3IntEqual();
	testV4ET_Vector3IntNotEqual();
	testV4ET_Vector3IntLess();
	testV4ET_Vector4Default();
	testV4ET_Vector4Construct();
	testV4ET_Vector4Clear();
	testV4ET_Vector4Add();
	testV4ET_Vector4Sub();
	testV4ET_Vector4AddAssign();
	testV4ET_Vector4SubAssign();
	testV4ET_Vector4Negate();
	testV4ET_Vector4ZeroComponents();
	testV4ET_Vector4NegativeComponents();
	testV4ET_Vector4IntDefault();
	testV4ET_Vector4IntConstruct();
	testV4ET_Vector4IntClear();
	testV4ET_Vector4IntAdd();
	testV4ET_Vector4IntSub();
	testV4ET_Vector4IntAddAssign();
	testV4ET_Vector4IntSubAssign();
	testV4ET_Vector4IntNegate();
	testV4ET_Vector4IntMulScalar();
	testV4ET_Vector4IntEqual();
	testV4ET_Vector4IntNotEqual();
	testV4ET_Vector4IntLess();
	testV4ET_Vector4IntChain();
	testV4ET_Vector4IntNegateChain();
	testV4ET_Vector3IntChain();
	testV4ET_Vector3IntMulZero();
	testV4ET_Vector4SubSelf();
}
