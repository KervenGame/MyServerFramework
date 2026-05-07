#include "FrameHeader.h"

#define V2ET_ASSERT(condition) if (!(condition)) { ERROR("Vector2ExtTest failed: " #condition); }
#define V2ET_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)

void Vector2ExtTest::test()
{
    testVector2DefaultConstruct();
    testVector2FloatConstruct();
    testVector2IntConstruct();
    testVector2Clear();
    testVector2Arithmetic();
    testVector2AssignOps();
    testVector2Negate();
    testVector2Equality();
    testVector2StaticConstants();
    testVector2IntDefaultConstruct();
    testVector2IntArithmetic();
    testVector2IntAssignOps();
    testVector2IntNegate();
    testVector2IntEquality();
    testVector2IntLess();
    testVector2IntClear();
    testVector2ShortDefaultConstruct();
    testVector2ShortArithmetic();
    testVector2ShortEquality();
    testVector2UShortBasic();
    testVector2UIntBasic();
}

void Vector2ExtTest::testVector2DefaultConstruct()
{
    Vector2 v;
    V2ET_ASSERT(V2ET_NEAR(v.x, 0.0f));
    V2ET_ASSERT(V2ET_NEAR(v.y, 0.0f));
}

void Vector2ExtTest::testVector2FloatConstruct()
{
    Vector2 v(3.5f, 7.5f);
    V2ET_ASSERT(V2ET_NEAR(v.x, 3.5f));
    V2ET_ASSERT(V2ET_NEAR(v.y, 7.5f));
}

void Vector2ExtTest::testVector2IntConstruct()
{
    Vector2 v(3, 7);
    V2ET_ASSERT(V2ET_NEAR(v.x, 3.0f));
    V2ET_ASSERT(V2ET_NEAR(v.y, 7.0f));
}

void Vector2ExtTest::testVector2Clear()
{
    Vector2 v(5.0f, 10.0f);
    v.clear();
    V2ET_ASSERT(V2ET_NEAR(v.x, 0.0f));
    V2ET_ASSERT(V2ET_NEAR(v.y, 0.0f));
}

void Vector2ExtTest::testVector2Arithmetic()
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);

    Vector2 sum = a + b;
    V2ET_ASSERT(V2ET_NEAR(sum.x, 4.0f));
    V2ET_ASSERT(V2ET_NEAR(sum.y, 6.0f));

    Vector2 diff = b - a;
    V2ET_ASSERT(V2ET_NEAR(diff.x, 2.0f));
    V2ET_ASSERT(V2ET_NEAR(diff.y, 2.0f));

    Vector2 scaled = a * 3.0f;
    V2ET_ASSERT(V2ET_NEAR(scaled.x, 3.0f));
    V2ET_ASSERT(V2ET_NEAR(scaled.y, 6.0f));

    Vector2 divided = b / 2.0f;
    V2ET_ASSERT(V2ET_NEAR(divided.x, 1.5f));
    V2ET_ASSERT(V2ET_NEAR(divided.y, 2.0f));
}

void Vector2ExtTest::testVector2AssignOps()
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);

    a += b;
    V2ET_ASSERT(V2ET_NEAR(a.x, 4.0f));
    V2ET_ASSERT(V2ET_NEAR(a.y, 6.0f));

    a -= b;
    V2ET_ASSERT(V2ET_NEAR(a.x, 1.0f));
    V2ET_ASSERT(V2ET_NEAR(a.y, 2.0f));
}

void Vector2ExtTest::testVector2Negate()
{
    Vector2 a(1.0f, -2.0f);
    Vector2 b = -a;
    V2ET_ASSERT(V2ET_NEAR(b.x, -1.0f));
    V2ET_ASSERT(V2ET_NEAR(b.y, 2.0f));
}

void Vector2ExtTest::testVector2Equality()
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(1.0f, 2.0f);
    Vector2 c(1.0f, 3.0f);
    V2ET_ASSERT(a == b);
    V2ET_ASSERT(!(a == c));
    V2ET_ASSERT(a != c);
    V2ET_ASSERT(!(a != b));
}

void Vector2ExtTest::testVector2StaticConstants()
{
    Vector2 zero = Vector2::ZERO;
    V2ET_ASSERT(V2ET_NEAR(zero.x, 0.0f));
    V2ET_ASSERT(V2ET_NEAR(zero.y, 0.0f));

    Vector2 one = Vector2::ONE;
    V2ET_ASSERT(V2ET_NEAR(one.x, 1.0f));
    V2ET_ASSERT(V2ET_NEAR(one.y, 1.0f));

    Vector2 up = Vector2::UP;
    V2ET_ASSERT(V2ET_NEAR(up.y, 1.0f));

    Vector2 down = Vector2::DOWN;
    V2ET_ASSERT(V2ET_NEAR(down.y, -1.0f));

    Vector2 right = Vector2::RIGHT;
    V2ET_ASSERT(V2ET_NEAR(right.x, 1.0f));

    Vector2 left = Vector2::LEFT;
    V2ET_ASSERT(V2ET_NEAR(left.x, -1.0f));
}

void Vector2ExtTest::testVector2IntDefaultConstruct()
{
    Vector2Int v;
    V2ET_ASSERT(v.x == 0);
    V2ET_ASSERT(v.y == 0);
}

void Vector2ExtTest::testVector2IntArithmetic()
{
    Vector2Int a(1, 2);
    Vector2Int b(3, 4);

    Vector2Int sum = a + b;
    V2ET_ASSERT(sum.x == 4);
    V2ET_ASSERT(sum.y == 6);

    Vector2Int diff = b - a;
    V2ET_ASSERT(diff.x == 2);
    V2ET_ASSERT(diff.y == 2);

    Vector2Int scaled = a * 5;
    V2ET_ASSERT(scaled.x == 5);
    V2ET_ASSERT(scaled.y == 10);
}

void Vector2ExtTest::testVector2IntAssignOps()
{
    Vector2Int a(2, 3);
    Vector2Int b(1, 1);
    a += b;
    V2ET_ASSERT(a.x == 3);
    V2ET_ASSERT(a.y == 4);
    a -= b;
    V2ET_ASSERT(a.x == 2);
    V2ET_ASSERT(a.y == 3);
}

void Vector2ExtTest::testVector2IntNegate()
{
    Vector2Int a(3, -5);
    Vector2Int b = -a;
    V2ET_ASSERT(b.x == -3);
    V2ET_ASSERT(b.y == 5);
}

void Vector2ExtTest::testVector2IntEquality()
{
    Vector2Int a(1, 2);
    Vector2Int b(1, 2);
    Vector2Int c(1, 3);
    V2ET_ASSERT(a == b);
    V2ET_ASSERT(!(a == c));
    V2ET_ASSERT(a != c);
    V2ET_ASSERT(!(a != b));
}

void Vector2ExtTest::testVector2IntLess()
{
    Vector2Int a(1, 2);
    Vector2Int b(2, 3);
    V2ET_ASSERT(a < b);
    V2ET_ASSERT(!(b < a));
}

void Vector2ExtTest::testVector2IntClear()
{
    Vector2Int v(5, 10);
    v.clear();
    V2ET_ASSERT(v.x == 0);
    V2ET_ASSERT(v.y == 0);
}

void Vector2ExtTest::testVector2ShortDefaultConstruct()
{
    Vector2Short v;
    V2ET_ASSERT(v.x == 0);
    V2ET_ASSERT(v.y == 0);
}

void Vector2ExtTest::testVector2ShortArithmetic()
{
    Vector2Short a((short)2, (short)3);
    Vector2Short b((short)4, (short)5);

    Vector2Short sum = a + b;
    V2ET_ASSERT(sum.x == 6);
    V2ET_ASSERT(sum.y == 8);

    Vector2Short diff = b - a;
    V2ET_ASSERT(diff.x == 2);
    V2ET_ASSERT(diff.y == 2);

    Vector2Short scaled = a * (short)3;
    V2ET_ASSERT(scaled.x == 6);
    V2ET_ASSERT(scaled.y == 9);

    Vector2Short neg = -a;
    V2ET_ASSERT(neg.x == -2);
    V2ET_ASSERT(neg.y == -3);
}

void Vector2ExtTest::testVector2ShortEquality()
{
    Vector2Short a((short)1, (short)2);
    Vector2Short b((short)1, (short)2);
    Vector2Short c((short)1, (short)3);
    V2ET_ASSERT(a == b);
    V2ET_ASSERT(a != c);
}

void Vector2ExtTest::testVector2UShortBasic()
{
    Vector2UShort a;
    V2ET_ASSERT(a.x == 0);
    V2ET_ASSERT(a.y == 0);

    Vector2UShort b((ushort)5, (ushort)10);
    V2ET_ASSERT(b.x == 5);
    V2ET_ASSERT(b.y == 10);

    b.clear();
    V2ET_ASSERT(b.x == 0);
    V2ET_ASSERT(b.y == 0);
}

void Vector2ExtTest::testVector2UIntBasic()
{
    Vector2UInt a;
    V2ET_ASSERT(a.x == 0u);
    V2ET_ASSERT(a.y == 0u);

    Vector2UInt b(7u, 11u);
    V2ET_ASSERT(b.x == 7u);
    V2ET_ASSERT(b.y == 11u);

    b.clear();
    V2ET_ASSERT(b.x == 0u);
    V2ET_ASSERT(b.y == 0u);
}
