#include "FrameHeader.h"

#define V3ET_ASSERT(condition) if (!(condition)) { ERROR("Vector3ExtTest failed: " #condition); }
#define V3ET_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)

void Vector3ExtTest::test()
{
    testDefaultConstruct();
    testFloatConstruct();
    testIntConstruct();
    testClear();
    testOperatorAdd();
    testOperatorSub();
    testOperatorMulScalar();
    testOperatorDivScalar();
    testOperatorAddAssign();
    testOperatorSubAssign();
    testOperatorNegate();
    testOperatorEqual();
    testOperatorNotEqual();
    testStaticZero();
    testStaticOne();
    testStaticForward();
    testStaticBack();
    testStaticLeft();
    testStaticRight();
    testStaticUp();
    testStaticDown();
    testChainOps();
    testFloatPrecision();
}

void Vector3ExtTest::testDefaultConstruct()
{
    Vector3 v;
    V3ET_ASSERT(V3ET_NEAR(v.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(v.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(v.z, 0.0f));
}

void Vector3ExtTest::testFloatConstruct()
{
    Vector3 v(1.5f, 2.5f, 3.5f);
    V3ET_ASSERT(V3ET_NEAR(v.x, 1.5f));
    V3ET_ASSERT(V3ET_NEAR(v.y, 2.5f));
    V3ET_ASSERT(V3ET_NEAR(v.z, 3.5f));
}

void Vector3ExtTest::testIntConstruct()
{
    Vector3 v(1, 2, 3);
    V3ET_ASSERT(V3ET_NEAR(v.x, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(v.y, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(v.z, 3.0f));
}

void Vector3ExtTest::testClear()
{
    Vector3 v(5.0f, 5.0f, 5.0f);
    v.clear();
    V3ET_ASSERT(V3ET_NEAR(v.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(v.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(v.z, 0.0f));
}

void Vector3ExtTest::testOperatorAdd()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 c = a + b;
    V3ET_ASSERT(V3ET_NEAR(c.x, 5.0f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 7.0f));
    V3ET_ASSERT(V3ET_NEAR(c.z, 9.0f));

    Vector3 d(0.0f, 0.0f, 0.0f);
    Vector3 e = a + d;
    V3ET_ASSERT(V3ET_NEAR(e.x, 1.0f));

    Vector3 neg(-1.0f, -2.0f, -3.0f);
    Vector3 f = a + neg;
    V3ET_ASSERT(V3ET_NEAR(f.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(f.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(f.z, 0.0f));
}

void Vector3ExtTest::testOperatorSub()
{
    Vector3 a(5.0f, 7.0f, 9.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 c = a - b;
    V3ET_ASSERT(V3ET_NEAR(c.x, 4.0f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 5.0f));
    V3ET_ASSERT(V3ET_NEAR(c.z, 6.0f));

    Vector3 d = a - a;
    V3ET_ASSERT(V3ET_NEAR(d.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(d.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(d.z, 0.0f));
}

void Vector3ExtTest::testOperatorMulScalar()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b = a * 2.0f;
    V3ET_ASSERT(V3ET_NEAR(b.x, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(b.y, 4.0f));
    V3ET_ASSERT(V3ET_NEAR(b.z, 6.0f));

    Vector3 c = a * 0.0f;
    V3ET_ASSERT(V3ET_NEAR(c.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(c.z, 0.0f));

    Vector3 d = a * -1.0f;
    V3ET_ASSERT(V3ET_NEAR(d.x, -1.0f));
    V3ET_ASSERT(V3ET_NEAR(d.y, -2.0f));
    V3ET_ASSERT(V3ET_NEAR(d.z, -3.0f));
}

void Vector3ExtTest::testOperatorDivScalar()
{
    Vector3 a(4.0f, 6.0f, 8.0f);
    Vector3 b = a / 2.0f;
    V3ET_ASSERT(V3ET_NEAR(b.x, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(b.y, 3.0f));
    V3ET_ASSERT(V3ET_NEAR(b.z, 4.0f));

    Vector3 c = a / 4.0f;
    V3ET_ASSERT(V3ET_NEAR(c.x, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 1.5f));
    V3ET_ASSERT(V3ET_NEAR(c.z, 2.0f));
}

void Vector3ExtTest::testOperatorAddAssign()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(10.0f, 20.0f, 30.0f);
    a += b;
    V3ET_ASSERT(V3ET_NEAR(a.x, 11.0f));
    V3ET_ASSERT(V3ET_NEAR(a.y, 22.0f));
    V3ET_ASSERT(V3ET_NEAR(a.z, 33.0f));
}

void Vector3ExtTest::testOperatorSubAssign()
{
    Vector3 a(10.0f, 20.0f, 30.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    a -= b;
    V3ET_ASSERT(V3ET_NEAR(a.x, 9.0f));
    V3ET_ASSERT(V3ET_NEAR(a.y, 18.0f));
    V3ET_ASSERT(V3ET_NEAR(a.z, 27.0f));
}

void Vector3ExtTest::testOperatorNegate()
{
    Vector3 a(1.0f, -2.0f, 3.0f);
    Vector3 b = -a;
    V3ET_ASSERT(V3ET_NEAR(b.x, -1.0f));
    V3ET_ASSERT(V3ET_NEAR(b.y, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(b.z, -3.0f));

    Vector3 zero(0.0f, 0.0f, 0.0f);
    Vector3 negZero = -zero;
    V3ET_ASSERT(V3ET_NEAR(negZero.x, 0.0f));
}

void Vector3ExtTest::testOperatorEqual()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 c(1.0f, 2.0f, 4.0f);
    V3ET_ASSERT(a == b);
    V3ET_ASSERT(!(a == c));
}

void Vector3ExtTest::testOperatorNotEqual()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(1.0f, 2.0f, 4.0f);
    Vector3 c(1.0f, 2.0f, 3.0f);
    V3ET_ASSERT(a != b);
    V3ET_ASSERT(!(a != c));
}

void Vector3ExtTest::testStaticZero()
{
    Vector3 z = Vector3::ZERO;
    V3ET_ASSERT(V3ET_NEAR(z.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(z.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(z.z, 0.0f));
}

void Vector3ExtTest::testStaticOne()
{
    Vector3 o = Vector3::ONE;
    V3ET_ASSERT(V3ET_NEAR(o.x, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(o.y, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(o.z, 1.0f));
}

void Vector3ExtTest::testStaticForward()
{
    Vector3 f = Vector3::FORWARD;
    V3ET_ASSERT(V3ET_NEAR(f.z, 1.0f) || V3ET_NEAR(f.z, -1.0f));
}

void Vector3ExtTest::testStaticBack()
{
    Vector3 b = Vector3::BACK;
    Vector3 f = Vector3::FORWARD;
    Vector3 sum = b + f;
    V3ET_ASSERT(V3ET_NEAR(sum.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(sum.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(sum.z, 0.0f));
}

void Vector3ExtTest::testStaticLeft()
{
    Vector3 l = Vector3::LEFT;
    V3ET_ASSERT(V3ET_NEAR(l.x, -1.0f) || V3ET_NEAR(l.x, 1.0f));
}

void Vector3ExtTest::testStaticRight()
{
    Vector3 r = Vector3::RIGHT;
    Vector3 l = Vector3::LEFT;
    Vector3 sum = r + l;
    V3ET_ASSERT(V3ET_NEAR(sum.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(sum.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(sum.z, 0.0f));
}

void Vector3ExtTest::testStaticUp()
{
    Vector3 u = Vector3::UP;
    V3ET_ASSERT(V3ET_NEAR(u.y, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(u.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(u.z, 0.0f));
}

void Vector3ExtTest::testStaticDown()
{
    Vector3 d = Vector3::DOWN;
    V3ET_ASSERT(V3ET_NEAR(d.y, -1.0f));
    V3ET_ASSERT(V3ET_NEAR(d.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(d.z, 0.0f));
}

void Vector3ExtTest::testChainOps()
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(0.0f, 1.0f, 0.0f);
    Vector3 c(0.0f, 0.0f, 1.0f);
    Vector3 result = (a + b + c) * 2.0f;
    V3ET_ASSERT(V3ET_NEAR(result.x, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(result.y, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(result.z, 2.0f));
}

void Vector3ExtTest::testFloatPrecision()
{
    Vector3 a(0.1f, 0.2f, 0.3f);
    Vector3 b(0.4f, 0.5f, 0.6f);
    Vector3 c = a + b;
    V3ET_ASSERT(V3ET_NEAR(c.x, 0.5f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 0.7f));
    V3ET_ASSERT(fabsf(c.z - 0.9f) < 1e-3f);
}
