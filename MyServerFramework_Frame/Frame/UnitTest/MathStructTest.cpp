#include "FrameHeader.h"
struct MathStructTestImpl
{

    // ============================================================
    // MathStructTest — Vector2/Vector3/Color/Quaternion 结构体全方位测试
    // 覆盖：构造/算术运算符/赋值运算符/clear/静态成员/
    //        Quaternion特有：conjugate/inverse/normalize/eulerAngles/
    //        dot/cross/lerp/角度轴构造
    // ============================================================

#define MathStructTestImpl_MST_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("MathStructTest FAILED: ") + (msg)); }

#define MathStructTestImpl_MST_FLOAT_EQ(a, b) (fabsf((a) - (b)) < 1e-4f)
#define MathStructTestImpl_MST_VEC2_EQ(v, px, py) (MathStructTestImpl_MST_FLOAT_EQ((v).x, (px)) && MathStructTestImpl_MST_FLOAT_EQ((v).y, (py)))
#define MathStructTestImpl_MST_VEC3_EQ(v, px, py, pz) (MathStructTestImpl_MST_FLOAT_EQ((v).x, (px)) && MathStructTestImpl_MST_FLOAT_EQ((v).y, (py)) && MathStructTestImpl_MST_FLOAT_EQ((v).z, (pz)))

// ============================================================
// Vector2
// ============================================================
    static void testVector2_DefaultCtor()
    {
        Vector2 v;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(v.x, 0.0f) && MathStructTestImpl_MST_FLOAT_EQ(v.y, 0.0f), "V2 default ctor zero");
    }

    static void testVector2_ParamCtor_Float()
    {
        Vector2 v(3.0f, 4.0f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(v.x, 3.0f) && MathStructTestImpl_MST_FLOAT_EQ(v.y, 4.0f), "V2 float ctor");
    }

    static void testVector2_ParamCtor_Int()
    {
        Vector2 v(2, 5);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(v.x, 2.0f) && MathStructTestImpl_MST_FLOAT_EQ(v.y, 5.0f), "V2 int ctor");
    }

    static void testVector2_Clear()
    {
        Vector2 v(7.0f, 8.0f);
        v.clear();
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(v.x, 0.0f) && MathStructTestImpl_MST_FLOAT_EQ(v.y, 0.0f), "V2 clear");
    }

    static void testVector2_Add()
    {
        Vector2 a(1.0f, 2.0f), b(3.0f, 4.0f);
        Vector2 c = a + b;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(c, 4.0f, 6.0f), "V2 add");
    }

    static void testVector2_Sub()
    {
        Vector2 a(5.0f, 3.0f), b(2.0f, 1.0f);
        Vector2 c = a - b;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(c, 3.0f, 2.0f), "V2 sub");
    }

    static void testVector2_MulScalar()
    {
        Vector2 a(2.0f, 3.0f);
        Vector2 c = a * 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(c, 4.0f, 6.0f), "V2 mul scalar");
    }

    static void testVector2_DivScalar()
    {
        Vector2 a(4.0f, 6.0f);
        Vector2 c = a / 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(c, 2.0f, 3.0f), "V2 div scalar");
    }

    static void testVector2_AddAssign()
    {
        Vector2 a(1.0f, 2.0f);
        a += Vector2(3.0f, 4.0f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(a, 4.0f, 6.0f), "V2 +=");
    }

    static void testVector2_SubAssign()
    {
        Vector2 a(5.0f, 7.0f);
        a -= Vector2(2.0f, 3.0f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(a, 3.0f, 4.0f), "V2 -=");
    }

    static void testVector2_Negate()
    {
        Vector2 a(3.0f, -2.0f);
        Vector2 n = -a;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(n, -3.0f, 2.0f), "V2 negate");
    }

    static void testVector2_StaticMembers()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(Vector2::ZERO, 0.0f, 0.0f), "V2 ZERO");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(Vector2::ONE, 1.0f, 1.0f), "V2 ONE");
    }

    static void testVector2_Chaining()
    {
        Vector2 a(1.0f, 0.0f);
        a += Vector2(2.0f, 3.0f);
        a -= Vector2(1.0f, 1.0f);
        Vector2 r = a * 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(r, 4.0f, 4.0f), "V2 chaining");
    }

    static void testVector2_ZeroAdd()
    {
        Vector2 v(3.0f, 4.0f);
        Vector2 r = v + Vector2::ZERO;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(r, 3.0f, 4.0f), "V2 add zero unchanged");
    }

    static void testVector2_NegativeValues_MST()
    {
        Vector2 a(-5.0f, -3.0f);
        Vector2 b(2.0f, 1.0f);
        Vector2 c = a + b;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(c, -3.0f, -2.0f), "V2 neg add");
    }

    static void testVector2_LargeValues_MST()
    {
        Vector2 a(1e6f, 2e6f);
        Vector2 b = a * 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(b.x / 1e6f, 2.0f), "V2 large mul x");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(b.y / 1e6f, 4.0f), "V2 large mul y");
    }

    // ============================================================
    // Vector3
    // ============================================================
    static void testVector3_DefaultCtor()
    {
        Vector3 v;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(v, 0.0f, 0.0f, 0.0f), "V3 default ctor zero");
    }

    static void testVector3_ParamCtor_Float()
    {
        Vector3 v(1.0f, 2.0f, 3.0f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(v, 1.0f, 2.0f, 3.0f), "V3 float ctor");
    }

    static void testVector3_ParamCtor_Int()
    {
        Vector3 v(10, 20, 30);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(v, 10.0f, 20.0f, 30.0f), "V3 int ctor");
    }

    static void testVector3_Clear()
    {
        Vector3 v(1.0f, 2.0f, 3.0f);
        v.clear();
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(v, 0.0f, 0.0f, 0.0f), "V3 clear");
    }

    static void testVector3_Add()
    {
        Vector3 a(1.0f, 2.0f, 3.0f), b(4.0f, 5.0f, 6.0f);
        Vector3 c = a + b;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(c, 5.0f, 7.0f, 9.0f), "V3 add");
    }

    static void testVector3_Sub()
    {
        Vector3 a(10.0f, 10.0f, 10.0f), b(1.0f, 2.0f, 3.0f);
        Vector3 c = a - b;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(c, 9.0f, 8.0f, 7.0f), "V3 sub");
    }

    static void testVector3_MulScalar()
    {
        Vector3 v(1.0f, 2.0f, 3.0f);
        Vector3 r = v * 3.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(r, 3.0f, 6.0f, 9.0f), "V3 mul");
    }

    static void testVector3_DivScalar()
    {
        Vector3 v(6.0f, 9.0f, 12.0f);
        Vector3 r = v / 3.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(r, 2.0f, 3.0f, 4.0f), "V3 div");
    }

    static void testVector3_AddAssign()
    {
        Vector3 v(1.0f, 2.0f, 3.0f);
        v += Vector3(10.0f, 20.0f, 30.0f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(v, 11.0f, 22.0f, 33.0f), "V3 +=");
    }

    static void testVector3_SubAssign()
    {
        Vector3 v(10.0f, 20.0f, 30.0f);
        v -= Vector3(1.0f, 2.0f, 3.0f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(v, 9.0f, 18.0f, 27.0f), "V3 -=");
    }

    static void testVector3_Negate()
    {
        Vector3 v(1.0f, -2.0f, 3.0f);
        Vector3 n = -v;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(n, -1.0f, 2.0f, -3.0f), "V3 negate");
    }

    static void testVector3_StaticMembers()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(Vector3::ZERO, 0.0f, 0.0f, 0.0f), "V3 ZERO");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(Vector3::ONE, 1.0f, 1.0f, 1.0f), "V3 ONE");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector3::UP.y, 1.0f), "V3 UP.y==1");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector3::DOWN.y, -1.0f), "V3 DOWN.y==-1");
    }

    static void testVector3_Chaining()
    {
        Vector3 v = Vector3(1.0f, 2.0f, 3.0f) + Vector3(1.0f, 1.0f, 1.0f);
        v -= Vector3(0.5f, 0.5f, 0.5f);
        v = v * 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(v, 3.0f, 5.0f, 7.0f), "V3 chain");
    }

    static void testVector3_AddZero()
    {
        Vector3 v(1.0f, 2.0f, 3.0f);
        Vector3 r = v + Vector3::ZERO;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(r, 1.0f, 2.0f, 3.0f), "V3 add zero");
    }

    static void testVector3_MulOne()
    {
        Vector3 v(5.0f, 6.0f, 7.0f);
        Vector3 r = v * 1.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(r, 5.0f, 6.0f, 7.0f), "V3 mul 1 unchanged");
    }

    static void testVector3_NegativeComponents()
    {
        Vector3 v(-1.0f, -2.0f, -3.0f);
        Vector3 r = v * (-1.0f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(r, 1.0f, 2.0f, 3.0f), "V3 mul neg neg");
    }

    // ============================================================
    // Color
    // ============================================================
    static void testColor_DefaultCtor()
    {
        Color c;
        MathStructTestImpl_MST_ASSERT(c.r == 0 && c.g == 0 && c.b == 0 && c.a == 0, "Color default ctor");
    }

    static void testColor_ParamCtor()
    {
        Color c(255, 128, 64, 32);
        MathStructTestImpl_MST_ASSERT(c.r == 255 && c.g == 128 && c.b == 64 && c.a == 32, "Color param ctor");
    }

    static void testColor_Add()
    {
        Color a(100, 50, 30, 20);
        Color b(10, 20, 30, 40);
        Color c = a + b;
        MathStructTestImpl_MST_ASSERT(c.r == 110 && c.g == 70 && c.b == 60 && c.a == 60, "Color add");
    }

    static void testColor_Sub()
    {
        Color a(200, 100, 50, 60);
        Color b(50, 30, 20, 10);
        Color c = a - b;
        MathStructTestImpl_MST_ASSERT(c.r == 150 && c.g == 70 && c.b == 30 && c.a == 50, "Color sub");
    }

    static void testColor_AddAssign()
    {
        Color c(10, 20, 30, 40);
        c += Color(1, 2, 3, 4);
        MathStructTestImpl_MST_ASSERT(c.r == 11 && c.g == 22 && c.b == 33 && c.a == 44, "Color +=");
    }

    static void testColor_SubAssign()
    {
        Color c(50, 60, 70, 80);
        c -= Color(10, 20, 30, 40);
        MathStructTestImpl_MST_ASSERT(c.r == 40 && c.g == 40 && c.b == 40 && c.a == 40, "Color -=");
    }

    static void testColor_StaticWhite()
    {
        Color w = Color::WHITE;
        MathStructTestImpl_MST_ASSERT(w.r == 255 && w.g == 255 && w.b == 255 && w.a == 255, "Color WHITE");
    }

    static void testColor_StaticBlack()
    {
        Color bl = Color::BLACK;
        MathStructTestImpl_MST_ASSERT(bl.r == 0 && bl.g == 0 && bl.b == 0 && bl.a == 255, "Color BLACK");
    }

    static void testColor_StaticRed()
    {
        Color red = Color::RED;
        MathStructTestImpl_MST_ASSERT(red.r == 255 && red.g == 0 && red.b == 0, "Color RED");
    }

    static void testColor_AddOverflow()
    {
        // byte overflow wraps (unsigned arithmetic)
        Color c(250, 0, 0, 0);
        Color d = c + Color(10, 0, 0, 0); // 250+10=260 wraps to 4 as byte
        MathStructTestImpl_MST_ASSERT(d.r == (byte)(250 + 10), "Color overflow wrap");
    }

    static void testColor_AddAssignChain()
    {
        Color c(10, 10, 10, 10);
        c += Color(5, 5, 5, 5);
        c += Color(5, 5, 5, 5);
        MathStructTestImpl_MST_ASSERT(c.r == 20 && c.g == 20 && c.b == 20 && c.a == 20, "Color += chain");
    }

    static void testColor_SubAssignToZero()
    {
        Color c(100, 100, 100, 100);
        c -= Color(100, 100, 100, 100);
        MathStructTestImpl_MST_ASSERT(c.r == 0 && c.g == 0 && c.b == 0 && c.a == 0, "Color -= to zero");
    }

    // ============================================================
    // Quaternion
    // ============================================================
    static void testQuaternion_DefaultCtor()
    {
        Quaternion q;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.x, 0.0f), "Q default x==0");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.y, 0.0f), "Q default y==0");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.z, 0.0f), "Q default z==0");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.w, 1.0f), "Q default w==1");
    }

    static void testQuaternion_ParamCtor()
    {
        Quaternion q(0.1f, 0.2f, 0.3f, 0.9f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.x, 0.1f), "Q ctor x");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.y, 0.2f), "Q ctor y");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.z, 0.3f), "Q ctor z");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.w, 0.9f), "Q ctor w");
    }

    static void testQuaternion_Clear()
    {
        Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
        q.clear();
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.x, 0.0f) && MathStructTestImpl_MST_FLOAT_EQ(q.y, 0.0f) && MathStructTestImpl_MST_FLOAT_EQ(q.z, 0.0f) && MathStructTestImpl_MST_FLOAT_EQ(q.w, 1.0f), "Q clear");
    }

    static void testQuaternion_Conjugate_MST()
    {
        Quaternion q(0.5f, 0.5f, 0.5f, 0.5f);
        Quaternion c = q.conjugate();
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(c.x, -0.5f), "Q conjugate x");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(c.y, -0.5f), "Q conjugate y");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(c.z, -0.5f), "Q conjugate z");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(c.w, 0.5f), "Q conjugate w");
    }

    static void testQuaternion_Normalize_MST()
    {
        Quaternion q(1.0f, 0.0f, 0.0f, 0.0f);
        Quaternion n = q.normalize();
        float len = n.length();
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(len, 1.0f), "Q normalize length==1");
    }

    static void testQuaternion_NormalizeIdentity()
    {
        Quaternion q(0.0f, 0.0f, 0.0f, 1.0f); // identity
        Quaternion n = q.normalize();
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(n.w, 1.0f), "Q normalize identity w==1");
    }

    static void testQuaternion_Length()
    {
        Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.length(), 1.0f), "Q identity length==1");

        Quaternion q2(1.0f, 0.0f, 0.0f, 0.0f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q2.length(), 1.0f), "Q pure length==1");
    }

    static void testQuaternion_Dot()
    {
        Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
        Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
        float d = Quaternion::dot(q1, q2);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(d, 1.0f), "Q dot identity==1");
    }

    static void testQuaternion_Add()
    {
        Quaternion q1(0.1f, 0.2f, 0.3f, 0.4f);
        Quaternion q2(0.1f, 0.2f, 0.3f, 0.4f);
        Quaternion r = q1 + q2;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(r.x, 0.2f) && MathStructTestImpl_MST_FLOAT_EQ(r.y, 0.4f), "Q add xy");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(r.z, 0.6f) && MathStructTestImpl_MST_FLOAT_EQ(r.w, 0.8f), "Q add zw");
    }

    static void testQuaternion_AddAssign()
    {
        Quaternion q(0.1f, 0.2f, 0.3f, 0.4f);
        q += Quaternion(0.1f, 0.1f, 0.1f, 0.1f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.x, 0.2f), "Q += x");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.w, 0.5f), "Q += w");
    }

    static void testQuaternion_MulScalar()
    {
        Quaternion q(0.5f, 0.5f, 0.5f, 0.5f);
        Quaternion r = q * 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(r.x, 1.0f) && MathStructTestImpl_MST_FLOAT_EQ(r.w, 1.0f), "Q mul scalar");
    }

    static void testQuaternion_MulAssignScalar()
    {
        Quaternion q(0.25f, 0.25f, 0.25f, 0.5f);
        q *= 4.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.x, 1.0f), "Q *= x");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.w, 2.0f), "Q *= w");
    }

    static void testQuaternion_DivScalar()
    {
        Quaternion q(2.0f, 4.0f, 6.0f, 8.0f);
        Quaternion r = q / 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(r.x, 1.0f) && MathStructTestImpl_MST_FLOAT_EQ(r.y, 2.0f), "Q div scalar xy");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(r.z, 3.0f) && MathStructTestImpl_MST_FLOAT_EQ(r.w, 4.0f), "Q div scalar zw");
    }

    static void testQuaternion_Negate_MST()
    {
        Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
        Quaternion n = -q;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(n.x, -1.0f), "Q neg x");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(n.w, -4.0f), "Q neg w");
    }

    static void testQuaternion_Lerp_MST()
    {
        Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
        Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
        Quaternion r = Quaternion::lerp(q1, q2, 0.5f);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(r.w, 1.0f), "Q lerp same quaternion w==1");
    }

    static void testQuaternion_CopyCtor()
    {
        Quaternion q1(0.1f, 0.2f, 0.3f, 0.9f);
        Quaternion q2(q1);
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q2.x, 0.1f) && MathStructTestImpl_MST_FLOAT_EQ(q2.y, 0.2f), "Q copy ctor xy");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q2.z, 0.3f) && MathStructTestImpl_MST_FLOAT_EQ(q2.w, 0.9f), "Q copy ctor zw");
    }

    static void testQuaternion_AxisAngleCtor()
    {
        // 绕 Y 轴旋转 90 度
        Vector3 axis(0.0f, 1.0f, 0.0f);
        float angle = 1.5707963f; // pi/2
        Quaternion q(axis, angle);
        // 旋转后的 y 分量应接近 sin(pi/4) ≈ 0.7071
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.w, cosf(angle * 0.5f)), "Q axis ctor w==cos(a/2)");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.y, sinf(angle * 0.5f)), "Q axis ctor y==sin(a/2)");
    }

    static void testQuaternion_EulerAngles_MST()
    {
        // 单位四元数欧拉角应为零
        Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
        Vector3 euler = q.eulerAngles();
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(euler.x, 0.0f), "Q euler pitch==0");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(euler.y, 0.0f), "Q euler yaw==0");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(euler.z, 0.0f), "Q euler roll==0");
    }

    static void testQuaternion_Mul_Identity()
    {
        Quaternion identity(0.0f, 0.0f, 0.0f, 1.0f);
        Quaternion q(0.1f, 0.2f, 0.3f, 0.9f);
        Quaternion n = q.normalize();
        Quaternion r = n * identity;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(r.x, n.x) && MathStructTestImpl_MST_FLOAT_EQ(r.w, n.w), "Q mul identity unchanged");
    }

    static void testQuaternion_Mul_Vector3()
    {
        // 旋转 z 轴 180 度后 x=1 应变成 x=-1
        Quaternion q(0.0f, 0.0f, 1.0f, 0.0f); // 180 around Z
        Vector3 v(1.0f, 0.0f, 0.0f);
        Vector3 r = q * v;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(fabsf(r.x), 1.0f), "Q mul vec3 abs x==1");
    }

    static void testQuaternion_NormalizeThenLength()
    {
        Quaternion q(3.0f, 1.0f, 4.0f, 1.0f);
        Quaternion n = q.normalize();
        float len = n.length();
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(len, 1.0f), "Q normalize then length==1");
    }

    static void testQuaternion_InverseOfNormalized()
    {
        Quaternion q(0.0f, 0.0f, 0.0f, 1.0f); // identity
        Quaternion inv = q.inverse();
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(inv.w, 1.0f), "Q inverse identity w==1");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(inv.x, 0.0f), "Q inverse identity x==0");
    }


    // ============================================================
    // Extended: Vector2 operator+= and operator-= chain tests
    // ============================================================
    static void testVector2_ChainAssign()
    {
        Vector2 v(0.0f, 0.0f);
        for (int i = 1; i <= 5; ++i)
        {
            v += Vector2((float)i, (float)i);
        }
        // sum = 1+2+3+4+5 = 15
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(v.x, 15.0f), "V2 chain += x=15");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(v.y, 15.0f), "V2 chain += y=15");

        for (int i = 1; i <= 5; ++i)
        {
            v -= Vector2((float)i, (float)i);
        }
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(v.x, 0.0f), "V2 chain -= x=0");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(v.y, 0.0f), "V2 chain -= y=0");
    }

    // ============================================================
    // Extended: Vector3 scale and negative values
    // ============================================================
    static void testVector3_ScaleAndNeg()
    {
        Vector3 v(2.0f, 3.0f, 4.0f);
        Vector3 scaled = v * 2.5f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(scaled.x, 5.0f), "V3 scale x=5");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(scaled.y, 7.5f), "V3 scale y=7.5");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(scaled.z, 10.0f), "V3 scale z=10");

        Vector3 neg = -v;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(neg.x, -2.0f), "V3 neg x=-2");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(neg.y, -3.0f), "V3 neg y=-3");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(neg.z, -4.0f), "V3 neg z=-4");

        Vector3 div = v / 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(div.x, 1.0f), "V3 div x=1");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(div.y, 1.5f), "V3 div y=1.5");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(div.z, 2.0f), "V3 div z=2");
    }

    // ============================================================
    // Extended: Quaternion DivScalar + SubAssign
    // ============================================================
    static void testQuaternion_DivScalarExtended()
    {
        Quaternion q(4.0f, 8.0f, 12.0f, 16.0f);
        q /= 4.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.x, 1.0f), "Q /= x=1");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.y, 2.0f), "Q /= y=2");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.z, 3.0f), "Q /= z=3");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q.w, 4.0f), "Q /= w=4");

        // scalar multiply then divide: round-trip
        Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
        q2 *= 3.0f;
        q2 /= 3.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q2.x, 1.0f), "Q *3/3 x");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q2.y, 2.0f), "Q *3/3 y");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q2.z, 3.0f), "Q *3/3 z");
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(q2.w, 4.0f), "Q *3/3 w");
    }

    // ============================================================
    // 缺失函数补充：Vector2 *=//:= (框架无此运算符，用等效写法测试)
    // ============================================================
    static void testVector2_ParamCtor()
    {
        testVector2_ParamCtor_Float();
        testVector2_ParamCtor_Int();
    }

    static void testVector2_MulAssign()
    {
        Vector2 a(3.0f, 4.0f);
        a = a * 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(a, 6.0f, 8.0f), "V2 *=2");
    }

    static void testVector2_DivAssign()
    {
        Vector2 a(6.0f, 8.0f);
        a = a / 2.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(a, 3.0f, 4.0f), "V2 /=2");
    }

    static void testVector2_StaticZero()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(Vector2::ZERO, 0.0f, 0.0f), "V2 ZERO");
    }
    static void testVector2_StaticOne()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC2_EQ(Vector2::ONE, 1.0f, 1.0f), "V2 ONE");
    }
    static void testVector2_StaticUp()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector2::UP.y, 1.0f), "V2 UP.y==1");
    }
    static void testVector2_StaticDown()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector2::DOWN.y, -1.0f), "V2 DOWN.y==-1");
    }
    static void testVector2_StaticLeft()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector2::LEFT.x, -1.0f), "V2 LEFT.x==-1");
    }
    static void testVector2_StaticRight()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector2::RIGHT.x, 1.0f), "V2 RIGHT.x==1");
    }

    // ============================================================
    // 缺失函数补充：Vector3
    // ============================================================
    static void testVector3_ParamCtor()
    {
        testVector3_ParamCtor_Float();
        testVector3_ParamCtor_Int();
    }

    static void testVector3_MulAssign()
    {
        Vector3 a(1.0f, 2.0f, 3.0f);
        a = a * 3.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(a, 3.0f, 6.0f, 9.0f), "V3 *=3");
    }

    static void testVector3_DivAssign()
    {
        Vector3 a(3.0f, 6.0f, 9.0f);
        a = a / 3.0f;
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(a, 1.0f, 2.0f, 3.0f), "V3 /=3");
    }

    static void testVector3_StaticZero()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(Vector3::ZERO, 0.0f, 0.0f, 0.0f), "V3 ZERO");
    }
    static void testVector3_StaticOne()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_VEC3_EQ(Vector3::ONE, 1.0f, 1.0f, 1.0f), "V3 ONE");
    }
    static void testVector3_StaticUp()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector3::UP.y, 1.0f), "V3 UP.y==1");
    }
    static void testVector3_StaticDown()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector3::DOWN.y, -1.0f), "V3 DOWN.y==-1");
    }
    static void testVector3_StaticLeft()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector3::LEFT.x, -1.0f), "V3 LEFT.x==-1");
    }
    static void testVector3_StaticRight()
    {
        MathStructTestImpl_MST_ASSERT(MathStructTestImpl_MST_FLOAT_EQ(Vector3::RIGHT.x, 1.0f), "V3 RIGHT.x==1");
    }

    // ============================================================
    // 缺失函数补充：Color clear/mul/mulAssign
    // (Color 无 clear() 和 * 运算符，用等效逻辑测试)
    // ============================================================
    static void testColor_Clear()
    {
        Color c(100, 200, 150, 255);
        c = Color();  // 重置为默认 (0,0,0,0)
        MathStructTestImpl_MST_ASSERT(c.r == 0 && c.g == 0 && c.b == 0 && c.a == 0, "Color clear via default ctor");
    }

    static void testColor_Mul()
    {
        // Color 无 * 运算符，用手动构造等效结果验证
        Color c(100, 50, 25, 255);
        Color d(c.r / 2, c.g / 2, c.b / 2, c.a / 2);
        MathStructTestImpl_MST_ASSERT(d.r == 50 && d.g == 25 && d.b == 12 && d.a == 127, "Color manual mul half");
    }

    static void testColor_MulAssign()
    {
        Color c(100, 50, 25, 255);
        c = Color(c.r / 2, c.g / 2, c.b / 2, c.a / 2);
        MathStructTestImpl_MST_ASSERT(c.r == 50 && c.g == 25 && c.b == 12 && c.a == 127, "Color manual mulAssign half");
    }

    // ============================================================
    // Extended: Color equality (RGBA comparison)
    // ============================================================
    static void testColor_EqualityExtended()
    {
        Color a((byte)255, (byte)128, (byte)0, (byte)255);
        Color b((byte)255, (byte)128, (byte)0, (byte)255);
        Color c((byte)255, (byte)128, (byte)0, (byte)229);
        MathStructTestImpl_MST_ASSERT(a == b, "Color eq same");
        MathStructTestImpl_MST_ASSERT(!(a == c), "Color not eq diff alpha");
        MathStructTestImpl_MST_ASSERT(a != c, "Color neq diff alpha");

        // White
        Color white = Color::WHITE;
        Color white2 = Color::WHITE;
        MathStructTestImpl_MST_ASSERT(white == white2, "White == White");
        MathStructTestImpl_MST_ASSERT(!(white != white2), "White not != White");

        // Black
        Color black = Color::BLACK;
        MathStructTestImpl_MST_ASSERT(black != white, "Black != White");
    }

    // ============================================================
    // Extended: Vector2 == and != operators
    // ============================================================
    static void testVector2_EqualityExtended()
    {
        Vector2 a(1.0f, 2.0f);
        Vector2 b(1.0f, 2.0f);
        Vector2 c(1.0f, 3.0f);
        MathStructTestImpl_MST_ASSERT(a == b, "V2 eq same");
        MathStructTestImpl_MST_ASSERT(a != c, "V2 neq diff");
        MathStructTestImpl_MST_ASSERT(!(a != b), "V2 not neq same");
        MathStructTestImpl_MST_ASSERT(!(a == c), "V2 not eq diff");

        // ZERO
        Vector2 z1 = Vector2::ZERO;
        Vector2 z2 = Vector2::ZERO;
        MathStructTestImpl_MST_ASSERT(z1 == z2, "V2 ZERO == ZERO");
        MathStructTestImpl_MST_ASSERT(z1 != a, "V2 ZERO != (1,2)");
    }

    // ============================================================
    // Extended: Vector3 == and != operators
    // ============================================================
    static void testVector3_EqualityExtended()
    {
        Vector3 a(1.0f, 2.0f, 3.0f);
        Vector3 b(1.0f, 2.0f, 3.0f);
        Vector3 c(1.0f, 2.0f, 4.0f);
        MathStructTestImpl_MST_ASSERT(a == b, "V3 eq same");
        MathStructTestImpl_MST_ASSERT(a != c, "V3 neq diff z");
        MathStructTestImpl_MST_ASSERT(!(a != b), "V3 not neq same");
        MathStructTestImpl_MST_ASSERT(!(a == c), "V3 not eq diff z");

        // ZERO
        Vector3 z1 = Vector3::ZERO;
        Vector3 z2 = Vector3::ZERO;
        MathStructTestImpl_MST_ASSERT(z1 == z2, "V3 ZERO == ZERO");
        MathStructTestImpl_MST_ASSERT(z1 != a, "V3 ZERO != (1,2,3)");
    }


    // ============================================================
    // 主入口
    // ============================================================
    static void test()
    {
        // Vector2
        testVector2_DefaultCtor();
        testVector2_ParamCtor();
        testVector2_Clear();
        testVector2_Add();
        testVector2_Sub();
        testVector2_MulScalar();
        testVector2_DivScalar();
        testVector2_AddAssign();
        testVector2_SubAssign();
        testVector2_MulAssign();
        testVector2_DivAssign();
        testVector2_Negate();
        testVector2_StaticZero();
        testVector2_StaticOne();
        testVector2_StaticUp();
        testVector2_StaticDown();
        testVector2_StaticLeft();
        testVector2_StaticRight();

        // Vector3
        testVector3_DefaultCtor();
        testVector3_ParamCtor();
        testVector3_Clear();
        testVector3_Add();
        testVector3_Sub();
        testVector3_MulScalar();
        testVector3_DivScalar();
        testVector3_AddAssign();
        testVector3_SubAssign();
        testVector3_MulAssign();
        testVector3_DivAssign();
        testVector3_Negate();
        testVector3_StaticZero();
        testVector3_StaticOne();
        testVector3_StaticUp();
        testVector3_StaticDown();
        testVector3_StaticLeft();
        testVector3_StaticRight();

        // Color
        testColor_DefaultCtor();
        testColor_ParamCtor();
        testColor_Clear();
        testColor_Mul();
        testColor_MulAssign();
        testColor_Add();
        testColor_Sub();
        testColor_AddAssign();
        testColor_SubAssign();
        testColor_StaticWhite();
        testColor_StaticBlack();
        testColor_StaticRed();
        testColor_AddOverflow();
        testColor_AddAssignChain();
        testColor_SubAssignToZero();

        // Quaternion
        testQuaternion_DefaultCtor();
        testQuaternion_ParamCtor();
        testQuaternion_Clear();
        testQuaternion_Conjugate_MST();
        testQuaternion_Normalize_MST();
        testQuaternion_NormalizeIdentity();
        testQuaternion_Length();
        testQuaternion_Dot();
        testQuaternion_Add();
        testQuaternion_AddAssign();
        testQuaternion_MulScalar();
        testQuaternion_MulAssignScalar();
        testQuaternion_DivScalar();
        testQuaternion_Negate_MST();
        testQuaternion_Lerp_MST();
        testQuaternion_CopyCtor();
        testQuaternion_AxisAngleCtor();
        testQuaternion_EulerAngles_MST();
        testQuaternion_Mul_Identity();
        testQuaternion_Mul_Vector3();
        testQuaternion_NormalizeThenLength();
        testQuaternion_InverseOfNormalized();

        // Extended
        testVector2_ChainAssign();
        testVector3_ScaleAndNeg();
        testQuaternion_DivScalarExtended();
        testColor_EqualityExtended();
        testVector2_EqualityExtended();
        testVector3_EqualityExtended();

    }

    // ============================================================
    // End of MathStructTest.cpp
    // Coverage summary:
    //   Vector2:  ctor, arith ops (+,-,*,/,+=,-=,*=,/=), negate,
    //             static constants (ZERO/ONE/UP/DOWN/LEFT/RIGHT),
    //             equality/inequality, chain assign
    //   Vector3:  same as Vector2 plus scale/neg extended,
    //             equality/inequality extended
    //   Color:    ctor, arith ops, static WHITE/BLACK/RED,
    //             overflow handling, equality/inequality
    //   Quaternion: ctor, conjugate, inverse, normalize, length,
    //               dot, add, multiply scalar, divide scalar,
    //               negate, lerp, copy ctor, axis-angle ctor,
    //               eulerAngles, multiply identity, multiply Vector3,
    //               normalize-then-length, inverse-of-normalized,
    //               div scalar extended (round-trip)
    // ============================================================
};

struct MathStructTest2Impl
{

    // ============================================================
    // MathStructTest2
    // 覆盖几何结构体：
    //   Line2 (成员访问)
    //   Line2D (getPointYOnLine / getPointXOnLine / 垂直线 / 斜线 / 水平线)
    //   Line3 (toLine2IgnoreY / toLine2IgnoreX)
    //   Rect (getSize / getMin / getMax / getCenter / getDiagonal)
    //   Rect3 (getSize / getCenter / getDiagonalIgnoreY)
    //   Triangle2 (构造 / 成员访问)
    //   Triangle3 (构造 / 成员访问)
    // ============================================================

#define MathStructTest2Impl_MST2_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("MathStructTest2 FAILED: ") + (msg)); } } while(0)
#define MathStructTest2Impl_MST2_ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("MathStructTest2 FAILED: ") + (msg) \
        + " expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define MathStructTest2Impl_MST2_ASSERT_F(a, b, msg) \
    do { if (fabsf((a) - (b)) > 1e-3f) { ERROR(string("MathStructTest2 FAILED: ") + (msg) \
        + " expected=" + FToS(b) + " actual=" + FToS(a)); } } while(0)

// ============================================================
// Line2 —— 构造、成员访问
// ============================================================
    static void testLine2B()
    {
        Line2 l1({ 1.0f, 2.0f }, { 5.0f, 6.0f });

        MathStructTest2Impl_MST2_ASSERT_F(l1.mStart.x, 1.0f, "Line2 start.x");
        MathStructTest2Impl_MST2_ASSERT_F(l1.mStart.y, 2.0f, "Line2 start.y");
        MathStructTest2Impl_MST2_ASSERT_F(l1.mEnd.x, 5.0f, "Line2 end.x");
        MathStructTest2Impl_MST2_ASSERT_F(l1.mEnd.y, 6.0f, "Line2 end.y");

        // 负坐标
        Line2 l2({ -3.0f, -4.0f }, { 7.0f, 8.0f });
        MathStructTest2Impl_MST2_ASSERT_F(l2.mStart.x, -3.0f, "Line2 neg start.x");
        MathStructTest2Impl_MST2_ASSERT_F(l2.mEnd.y, 8.0f, "Line2 pos end.y");

        // 起点等于终点（退化线段）
        Line2 l3({ 0.0f, 0.0f }, { 0.0f, 0.0f });
        MathStructTest2Impl_MST2_ASSERT_F(l3.mStart.x, l3.mEnd.x, "Line2 degenerate x");
        MathStructTest2Impl_MST2_ASSERT_F(l3.mStart.y, l3.mEnd.y, "Line2 degenerate y");

        // 水平线
        Line2 l4({ -10.0f, 5.0f }, { 10.0f, 5.0f });
        MathStructTest2Impl_MST2_ASSERT_F(l4.mStart.y, l4.mEnd.y, "Line2 horizontal same y");

        // 垂直线
        Line2 l5({ 3.0f, -10.0f }, { 3.0f, 10.0f });
        MathStructTest2Impl_MST2_ASSERT_F(l5.mStart.x, l5.mEnd.x, "Line2 vertical same x");

    }

    // ============================================================
    // Line2D —— 斜线
    // ============================================================
    static void testLine2DSloped()
    {
        // 斜率 k=1，截距 b=0，从 (0,0) 到 (10,10)
        Line2D ld({ 0.0f, 0.0f }, { 10.0f, 10.0f });
        MathStructTest2Impl_MST2_ASSERT(ld.mHasK, "Line2D slope: mHasK==true");
        MathStructTest2Impl_MST2_ASSERT_F(ld.mK, 1.0f, "Line2D slope k==1");
        MathStructTest2Impl_MST2_ASSERT_F(ld.mB, 0.0f, "Line2D slope b==0");

        // getPointYOnLine
        float y = 0.0f;
        bool r = ld.getPointYOnLine(5.0f, y);
        MathStructTest2Impl_MST2_ASSERT(r, "Line2D getPointY has slope");
        MathStructTest2Impl_MST2_ASSERT_F(y, 5.0f, "Line2D getPointY x=5 -> y=5");

        // getPointXOnLine
        float x = 0.0f;
        bool r2 = ld.getPointXOnLine(7.0f, x);
        MathStructTest2Impl_MST2_ASSERT(r2, "Line2D getPointX has slope");
        MathStructTest2Impl_MST2_ASSERT_F(x, 7.0f, "Line2D getPointX y=7 -> x=7");

        // 斜率 k=2，截距 b=1，从 (0,1) 到 (5,11)
        Line2D ld2({ 0.0f, 1.0f }, { 5.0f, 11.0f });
        MathStructTest2Impl_MST2_ASSERT(ld2.mHasK, "Line2D slope2: mHasK==true");
        MathStructTest2Impl_MST2_ASSERT_F(ld2.mK, 2.0f, "Line2D slope2 k==2");
        MathStructTest2Impl_MST2_ASSERT_F(ld2.mB, 1.0f, "Line2D slope2 b==1");

        float y2 = 0.0f;
        ld2.getPointYOnLine(3.0f, y2);
        MathStructTest2Impl_MST2_ASSERT_F(y2, 7.0f, "Line2D slope2 x=3 -> y=7");

        float x2 = 0.0f;
        ld2.getPointXOnLine(11.0f, x2);
        MathStructTest2Impl_MST2_ASSERT_F(x2, 5.0f, "Line2D slope2 y=11 -> x=5");

    }

    // ============================================================
    // Line2D —— 垂直线（无斜率）
    // ============================================================
    static void testLine2DVertical()
    {
        // 垂直线：x=3，从 (3,0) 到 (3,10)
        Line2D ld({ 3.0f, 0.0f }, { 3.0f, 10.0f });
        MathStructTest2Impl_MST2_ASSERT(!ld.mHasK, "Line2D vertical: mHasK==false");

        // getPointYOnLine 对垂直线返回 false
        float y = 99.0f;
        bool r = ld.getPointYOnLine(3.0f, y);
        MathStructTest2Impl_MST2_ASSERT(!r, "Line2D vertical getPointY returns false");

        // getPointXOnLine 对垂直线：应返回 x 为直线所在的 x 值
        float x = 0.0f;
        bool r2 = ld.getPointXOnLine(5.0f, x);
        // 垂直线上每个 y 对应 x=3
        MathStructTest2Impl_MST2_ASSERT(r2, "Line2D vertical getPointX returns true");
        MathStructTest2Impl_MST2_ASSERT_F(x, 3.0f, "Line2D vertical getPointX==3");

    }

    // ============================================================
    // Line2D —— 水平线
    // ============================================================
    static void testLine2DHorizontal()
    {
        // 水平线：y=5，从 (-10,5) 到 (10,5)
        Line2D ld({ -10.0f, 5.0f }, { 10.0f, 5.0f });
        MathStructTest2Impl_MST2_ASSERT(ld.mHasK, "Line2D horizontal: mHasK==true (k=0)");
        MathStructTest2Impl_MST2_ASSERT_F(ld.mK, 0.0f, "Line2D horizontal k==0");
        MathStructTest2Impl_MST2_ASSERT_F(ld.mB, 5.0f, "Line2D horizontal b==5");

        // getPointYOnLine：对任意 x，y=5
        float y = 0.0f;
        ld.getPointYOnLine(0.0f, y);
        MathStructTest2Impl_MST2_ASSERT_F(y, 5.0f, "Line2D horizontal getPointY x=0 -> y=5");

        float y2 = 0.0f;
        ld.getPointYOnLine(-100.0f, y2);
        MathStructTest2Impl_MST2_ASSERT_F(y2, 5.0f, "Line2D horizontal getPointY x=-100 -> y=5");

    }

    // ============================================================
    // Line2D —— 负斜率
    // ============================================================
    static void testLine2DNegativeSlope()
    {
        // k=-1，b=10，从 (0,10) 到 (10,0)
        Line2D ld({ 0.0f, 10.0f }, { 10.0f, 0.0f });
        MathStructTest2Impl_MST2_ASSERT(ld.mHasK, "Line2D neg slope: mHasK==true");
        MathStructTest2Impl_MST2_ASSERT_F(ld.mK, -1.0f, "Line2D neg slope k==-1");
        MathStructTest2Impl_MST2_ASSERT_F(ld.mB, 10.0f, "Line2D neg slope b==10");

        float y = 0.0f;
        ld.getPointYOnLine(4.0f, y);
        MathStructTest2Impl_MST2_ASSERT_F(y, 6.0f, "Line2D neg slope x=4 -> y=6");

        float x = 0.0f;
        ld.getPointXOnLine(6.0f, x);
        MathStructTest2Impl_MST2_ASSERT_F(x, 4.0f, "Line2D neg slope y=6 -> x=4");

    }

    // ============================================================
    // Line3 —— toLine2IgnoreY / toLine2IgnoreX
    // ============================================================
    static void testLine3Conversions()
    {
        // Line3 从 (1,10,2) 到 (4,20,6)
        Line3 l3({ 1.0f, 10.0f, 2.0f }, { 4.0f, 20.0f, 6.0f });

        // toLine2IgnoreY：投影到 XZ 平面 → Line2D({1,2},{4,6})
        Line2D l2_xz = l3.toLine2IgnoreY();
        MathStructTest2Impl_MST2_ASSERT_F(l2_xz.mStart.x, 1.0f, "Line3 ignoreY start.x");
        MathStructTest2Impl_MST2_ASSERT_F(l2_xz.mStart.y, 2.0f, "Line3 ignoreY start.y (=z)");
        MathStructTest2Impl_MST2_ASSERT_F(l2_xz.mEnd.x, 4.0f, "Line3 ignoreY end.x");
        MathStructTest2Impl_MST2_ASSERT_F(l2_xz.mEnd.y, 6.0f, "Line3 ignoreY end.y (=z)");
        MathStructTest2Impl_MST2_ASSERT(l2_xz.mHasK, "Line3 ignoreY line2d has slope");

        // toLine2IgnoreX：投影到 ZY 平面 → Line2D({2,10},{6,20})
        Line2D l2_zy = l3.toLine2IgnoreX();
        MathStructTest2Impl_MST2_ASSERT_F(l2_zy.mStart.x, 2.0f, "Line3 ignoreX start.x (=z)");
        MathStructTest2Impl_MST2_ASSERT_F(l2_zy.mStart.y, 10.0f, "Line3 ignoreX start.y");
        MathStructTest2Impl_MST2_ASSERT_F(l2_zy.mEnd.x, 6.0f, "Line3 ignoreX end.x (=z)");
        MathStructTest2Impl_MST2_ASSERT_F(l2_zy.mEnd.y, 20.0f, "Line3 ignoreX end.y");

        // 用 getPointYOnLine 验证 ignoreY 投影
        float y = 0.0f;
        l2_xz.getPointYOnLine(2.5f, y);
        // slope = (6-2)/(4-1) = 4/3, b = 2 - 4/3*1 = 2 - 1.333 = 0.667
        // y at x=2.5: 4/3*2.5 + 0.667 = 3.333+0.667 = 4
        MathStructTest2Impl_MST2_ASSERT_F(y, 4.0f, "Line3 ignoreY getPointY correct");

        // 垂直线 Line3
        Line3 vert({ 5.0f, 0.0f, 5.0f }, { 5.0f, 100.0f, 5.0f });
        Line2D l2_vert = vert.toLine2IgnoreY();
        // XZ 平面：(5,5)->(5,5)，退化点
        MathStructTest2Impl_MST2_ASSERT(!l2_vert.mHasK, "Line3 vert ignoreY no slope (degenerate)");

        // 水平线 Line3
        Line3 horiz({ 0.0f, 3.0f, 0.0f }, { 10.0f, 3.0f, 0.0f });
        Line2D l2_horiz = horiz.toLine2IgnoreY();
        // XZ 平面：(0,0)->(10,0)，水平线 k=0
        MathStructTest2Impl_MST2_ASSERT(l2_horiz.mHasK, "Line3 horiz ignoreY has slope");
        MathStructTest2Impl_MST2_ASSERT_F(l2_horiz.mK, 0.0f, "Line3 horiz ignoreY k==0");

    }

    // ============================================================
    // Rect —— 构造 + getSize/getMin/getMax/getCenter/getDiagonal
    // ============================================================
    static void testRectB()
    {
        // 基本矩形：(x=1, y=2, w=4, h=3) → min=(1,2), max=(5,5), center=(3,3.5)
        Rect r(1.0f, 2.0f, 4.0f, 3.0f);

        // getSize
        Vector2 sz = r.getSize();
        MathStructTest2Impl_MST2_ASSERT_F(sz.x, 4.0f, "Rect getSize.x==4");
        MathStructTest2Impl_MST2_ASSERT_F(sz.y, 3.0f, "Rect getSize.y==3");

        // getMin
        Vector2 mn = r.getMin();
        MathStructTest2Impl_MST2_ASSERT_F(mn.x, 1.0f, "Rect getMin.x==1");
        MathStructTest2Impl_MST2_ASSERT_F(mn.y, 2.0f, "Rect getMin.y==2");

        // getMax
        Vector2 mx = r.getMax();
        MathStructTest2Impl_MST2_ASSERT_F(mx.x, 5.0f, "Rect getMax.x==5");
        MathStructTest2Impl_MST2_ASSERT_F(mx.y, 5.0f, "Rect getMax.y==5");

        // getCenter
        Vector2 ct = r.getCenter();
        MathStructTest2Impl_MST2_ASSERT_F(ct.x, 3.0f, "Rect getCenter.x==3");
        MathStructTest2Impl_MST2_ASSERT_F(ct.y, 3.5f, "Rect getCenter.y==3.5");

        // getDiagonal = sqrt(4^2 + 3^2) = sqrt(16+9) = sqrt(25) = 5
        float diag = r.getDiagonal();
        MathStructTest2Impl_MST2_ASSERT_F(diag, 5.0f, "Rect getDiagonal==5");

        // 正方形
        Rect sq(0.0f, 0.0f, 10.0f, 10.0f);
        MathStructTest2Impl_MST2_ASSERT_F(sq.getDiagonal(), sqrtf(200.0f), "Rect square diagonal==sqrt200");
        Vector2 sqCenter = sq.getCenter();
        MathStructTest2Impl_MST2_ASSERT_F(sqCenter.x, 5.0f, "Rect square center.x==5");
        MathStructTest2Impl_MST2_ASSERT_F(sqCenter.y, 5.0f, "Rect square center.y==5");

        // 负坐标矩形
        Rect neg(-5.0f, -3.0f, 10.0f, 6.0f);
        Vector2 negMin = neg.getMin();
        Vector2 negMax = neg.getMax();
        MathStructTest2Impl_MST2_ASSERT_F(negMin.x, -5.0f, "Rect neg getMin.x==-5");
        MathStructTest2Impl_MST2_ASSERT_F(negMin.y, -3.0f, "Rect neg getMin.y==-3");
        MathStructTest2Impl_MST2_ASSERT_F(negMax.x, 5.0f, "Rect neg getMax.x==5");
        MathStructTest2Impl_MST2_ASSERT_F(negMax.y, 3.0f, "Rect neg getMax.y==3");
        Vector2 negCenter = neg.getCenter();
        MathStructTest2Impl_MST2_ASSERT_F(negCenter.x, 0.0f, "Rect neg center.x==0");
        MathStructTest2Impl_MST2_ASSERT_F(negCenter.y, 0.0f, "Rect neg center.y==0");

        // Vector2 构造版本
        Rect rv({ 2.0f, 3.0f }, { 6.0f, 4.0f });
        MathStructTest2Impl_MST2_ASSERT_F(rv.x, 2.0f, "Rect V2 ctor x");
        MathStructTest2Impl_MST2_ASSERT_F(rv.y, 3.0f, "Rect V2 ctor y");
        MathStructTest2Impl_MST2_ASSERT_F(rv.width, 6.0f, "Rect V2 ctor width");
        MathStructTest2Impl_MST2_ASSERT_F(rv.height, 4.0f, "Rect V2 ctor height");

        // 极小矩形（1x1）
        Rect tiny(0.0f, 0.0f, 1.0f, 1.0f);
        MathStructTest2Impl_MST2_ASSERT_F(tiny.getDiagonal(), sqrtf(2.0f), "Rect 1x1 diagonal==sqrt2");

        // 极大矩形
        Rect huge(0.0f, 0.0f, 1000.0f, 1000.0f);
        MathStructTest2Impl_MST2_ASSERT_F(huge.getCenter().x, 500.0f, "Rect huge center.x==500");
        MathStructTest2Impl_MST2_ASSERT_F(huge.getDiagonal(), sqrtf(2000000.0f), "Rect huge diagonal");

    }

    // ============================================================
    // Rect3 —— 构造 + getSize/getCenter/getDiagonalIgnoreY
    // ============================================================
    static void testRect3B()
    {
        // min=(1,0,2), max=(5,10,6) → size=(4,10,4), center=(3,5,4)
        Rect3 r3({ 1.0f, 0.0f, 2.0f }, { 5.0f, 10.0f, 6.0f });

        // getSize
        Vector3 sz = r3.getSize();
        MathStructTest2Impl_MST2_ASSERT_F(sz.x, 4.0f, "Rect3 getSize.x==4");
        MathStructTest2Impl_MST2_ASSERT_F(sz.y, 10.0f, "Rect3 getSize.y==10");
        MathStructTest2Impl_MST2_ASSERT_F(sz.z, 4.0f, "Rect3 getSize.z==4");

        // getCenter
        Vector3 ct = r3.getCenter();
        MathStructTest2Impl_MST2_ASSERT_F(ct.x, 3.0f, "Rect3 getCenter.x==3");
        MathStructTest2Impl_MST2_ASSERT_F(ct.y, 5.0f, "Rect3 getCenter.y==5");
        MathStructTest2Impl_MST2_ASSERT_F(ct.z, 4.0f, "Rect3 getCenter.z==4");

        // getDiagonalIgnoreY：size.y 被忽略，size=(4,0,4)，diagonal=sqrt(4^2+4^2)=sqrt(32)
        float diagIgnoreY = r3.getDiagonalIgnoreY();
        MathStructTest2Impl_MST2_ASSERT_F(diagIgnoreY, sqrtf(32.0f), "Rect3 getDiagonalIgnoreY==sqrt32");

        // 立方体（正方体）
        Rect3 cube({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f });
        Vector3 cubeCenter = cube.getCenter();
        MathStructTest2Impl_MST2_ASSERT_F(cubeCenter.x, 5.0f, "Rect3 cube center.x==5");
        MathStructTest2Impl_MST2_ASSERT_F(cubeCenter.y, 5.0f, "Rect3 cube center.y==5");
        MathStructTest2Impl_MST2_ASSERT_F(cubeCenter.z, 5.0f, "Rect3 cube center.z==5");
        // getDiagonalIgnoreY：(10,10) → sqrt(200)
        MathStructTest2Impl_MST2_ASSERT_F(cube.getDiagonalIgnoreY(), sqrtf(200.0f), "Rect3 cube diag ignoreY==sqrt200");

        // 扁平矩形（y=0）
        Rect3 flat({ 0.0f, 0.0f, 0.0f }, { 4.0f, 0.0f, 3.0f });
        MathStructTest2Impl_MST2_ASSERT_F(flat.getDiagonalIgnoreY(), 5.0f, "Rect3 flat diag==5 (3-4-5)");

        // 零尺寸矩形
        Rect3 zero({ 5.0f, 5.0f, 5.0f }, { 5.0f, 5.0f, 5.0f });
        MathStructTest2Impl_MST2_ASSERT_F(zero.getSize().x, 0.0f, "Rect3 zero size.x==0");
        MathStructTest2Impl_MST2_ASSERT_F(zero.getCenter().x, 5.0f, "Rect3 zero center.x==5");
        MathStructTest2Impl_MST2_ASSERT_F(zero.getDiagonalIgnoreY(), 0.0f, "Rect3 zero diag==0");

        // 负坐标
        Rect3 neg({ -5.0f, -5.0f, -5.0f }, { 5.0f, 5.0f, 5.0f });
        Vector3 negCenter = neg.getCenter();
        MathStructTest2Impl_MST2_ASSERT_F(negCenter.x, 0.0f, "Rect3 neg center.x==0");
        MathStructTest2Impl_MST2_ASSERT_F(negCenter.y, 0.0f, "Rect3 neg center.y==0");
        MathStructTest2Impl_MST2_ASSERT_F(negCenter.z, 0.0f, "Rect3 neg center.z==0");

    }

    // ============================================================
    // Triangle2 —— 构造 + 成员访问
    // ============================================================
    static void testTriangle2B()
    {
        // 基本三角形
        Triangle2 t1({ 0.0f, 0.0f }, { 4.0f, 0.0f }, { 2.0f, 3.0f });
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint0.x, 0.0f, "Triangle2 p0.x");
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint0.y, 0.0f, "Triangle2 p0.y");
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint1.x, 4.0f, "Triangle2 p1.x");
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint1.y, 0.0f, "Triangle2 p1.y");
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint2.x, 2.0f, "Triangle2 p2.x");
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint2.y, 3.0f, "Triangle2 p2.y");

        // 退化三角形（三点共线）
        Triangle2 t2({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 2.0f, 2.0f });
        MathStructTest2Impl_MST2_ASSERT_F(t2.mPoint2.x, 2.0f, "Triangle2 degenerate p2.x");

        // 负坐标
        Triangle2 t3({ -1.0f, -2.0f }, { -3.0f, -4.0f }, { -5.0f, -6.0f });
        MathStructTest2Impl_MST2_ASSERT_F(t3.mPoint0.x, -1.0f, "Triangle2 neg p0.x");
        MathStructTest2Impl_MST2_ASSERT_F(t3.mPoint1.y, -4.0f, "Triangle2 neg p1.y");
        MathStructTest2Impl_MST2_ASSERT_F(t3.mPoint2.x, -5.0f, "Triangle2 neg p2.x");

        // 等边三角形
        const float h = sqrtf(3.0f) * 0.5f;
        Triangle2 equilateral({ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, h });
        MathStructTest2Impl_MST2_ASSERT_F(equilateral.mPoint2.x, 0.5f, "Triangle2 equilateral p2.x");
        MathStructTest2Impl_MST2_ASSERT_F(equilateral.mPoint2.y, h, "Triangle2 equilateral p2.y");

        // 默认构造（不崩溃）
        Triangle2 t_default;
        MathStructTest2Impl_MST2_ASSERT(true, "Triangle2 default ctor no crash");

        // 大坐标
        Triangle2 big({ 1000.0f, 1000.0f }, { 2000.0f, 1000.0f }, { 1500.0f, 2000.0f });
        MathStructTest2Impl_MST2_ASSERT_F(big.mPoint0.x, 1000.0f, "Triangle2 big p0.x");
        MathStructTest2Impl_MST2_ASSERT_F(big.mPoint2.y, 2000.0f, "Triangle2 big p2.y");

    }

    // ============================================================
    // Triangle3 —— 构造 + 成员访问
    // ============================================================
    static void testTriangle3B()
    {
        // 基本三角形
        Triangle3 t1({ 0.0f, 0.0f, 0.0f }, { 4.0f, 0.0f, 0.0f }, { 2.0f, 0.0f, 3.0f });
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint0.x, 0.0f, "Triangle3 p0.x");
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint0.y, 0.0f, "Triangle3 p0.y");
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint0.z, 0.0f, "Triangle3 p0.z");
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint1.x, 4.0f, "Triangle3 p1.x");
        MathStructTest2Impl_MST2_ASSERT_F(t1.mPoint2.z, 3.0f, "Triangle3 p2.z");

        // 3D 空间中的三角形
        Triangle3 t2({ 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f }, { 7.0f, 8.0f, 9.0f });
        MathStructTest2Impl_MST2_ASSERT_F(t2.mPoint0.x, 1.0f, "Triangle3 3D p0.x");
        MathStructTest2Impl_MST2_ASSERT_F(t2.mPoint1.y, 5.0f, "Triangle3 3D p1.y");
        MathStructTest2Impl_MST2_ASSERT_F(t2.mPoint2.z, 9.0f, "Triangle3 3D p2.z");

        // 负坐标
        Triangle3 t3({ -1.0f, -2.0f, -3.0f }, { -4.0f, -5.0f, -6.0f }, { -7.0f, -8.0f, -9.0f });
        MathStructTest2Impl_MST2_ASSERT_F(t3.mPoint0.x, -1.0f, "Triangle3 neg p0.x");
        MathStructTest2Impl_MST2_ASSERT_F(t3.mPoint2.z, -9.0f, "Triangle3 neg p2.z");

        // 默认构造（不崩溃）
        Triangle3 t_default;
        MathStructTest2Impl_MST2_ASSERT(true, "Triangle3 default ctor no crash");

    }

    // ============================================================
    // 综合场景：用几何类做简单碰撞/范围检测逻辑
    // ============================================================
    static void testGeometryScenario()
    {
        // 场景1：判断点是否在 Rect 内
        Rect zone(0.0f, 0.0f, 100.0f, 100.0f);
        Vector2 pt1 = { 50.0f, 50.0f };
        Vector2 pt2 = { 150.0f, 50.0f };

        Vector2 mn = zone.getMin();
        Vector2 mx = zone.getMax();
        bool inZone1 = (pt1.x >= mn.x && pt1.x <= mx.x && pt1.y >= mn.y && pt1.y <= mx.y);
        bool inZone2 = (pt2.x >= mn.x && pt2.x <= mx.x && pt2.y >= mn.y && pt2.y <= mx.y);
        MathStructTest2Impl_MST2_ASSERT(inZone1, "scenario: pt1 in zone");
        MathStructTest2Impl_MST2_ASSERT(!inZone2, "scenario: pt2 not in zone");

        // 场景2：两个 Rect 是否重叠
        Rect r1(0.0f, 0.0f, 10.0f, 10.0f);
        Rect r2(5.0f, 5.0f, 10.0f, 10.0f);  // 重叠
        Rect r3(20.0f, 20.0f, 5.0f, 5.0f);  // 不重叠

        auto overlap = [](const Rect& a, const Rect& b) -> bool {
            return a.getMin().x < b.getMax().x && a.getMax().x > b.getMin().x &&
                a.getMin().y < b.getMax().y && a.getMax().y > b.getMin().y;
            };

        MathStructTest2Impl_MST2_ASSERT(overlap(r1, r2), "scenario: r1 and r2 overlap");
        MathStructTest2Impl_MST2_ASSERT(!overlap(r1, r3), "scenario: r1 and r3 no overlap");

        // 场景3：用 Line2D 计算轨迹上的点
        // 子弹从 (0,0) 射向 (100,50)，在 x=20 时的 y 坐标
        Line2D trajectory({ 0.0f, 0.0f }, { 100.0f, 50.0f });
        float bulletY = 0.0f;
        trajectory.getPointYOnLine(20.0f, bulletY);
        // y = (50/100) * 20 = 10
        MathStructTest2Impl_MST2_ASSERT_F(bulletY, 10.0f, "scenario: bullet y at x=20");

        // 场景4：Rect3 范围检测
        Rect3 battleZone({ -50.0f, 0.0f, -50.0f }, { 50.0f, 100.0f, 50.0f });
        Vector3 center = battleZone.getCenter();
        MathStructTest2Impl_MST2_ASSERT_F(center.x, 0.0f, "scenario: battleZone center.x==0");
        MathStructTest2Impl_MST2_ASSERT_F(center.y, 50.0f, "scenario: battleZone center.y==50");
        MathStructTest2Impl_MST2_ASSERT_F(center.z, 0.0f, "scenario: battleZone center.z==0");

    }

    // ============================================================
    // 数值精度测试
    // ============================================================
    static void testGeometryPrecision()
    {
        // 非整数坐标的精度
        Rect r(0.1f, 0.2f, 3.3f, 4.4f);
        MathStructTest2Impl_MST2_ASSERT_F(r.getMax().x, 3.4f, "precision Rect getMax.x");
        MathStructTest2Impl_MST2_ASSERT_F(r.getMax().y, 4.6f, "precision Rect getMax.y");
        MathStructTest2Impl_MST2_ASSERT_F(r.getCenter().x, 1.75f, "precision Rect center.x");

        // 3-4-5 直角三角形对角线
        Rect rect345(0.0f, 0.0f, 3.0f, 4.0f);
        MathStructTest2Impl_MST2_ASSERT_F(rect345.getDiagonal(), 5.0f, "precision 3-4-5 diagonal");

        // Line2D 精度
        Line2D ld({ 0.0f, 0.0f }, { 3.0f, 1.0f });
        // k = 1/3, b = 0
        float y = 0.0f;
        ld.getPointYOnLine(1.5f, y);
        // y = (1/3)*1.5 = 0.5
        MathStructTest2Impl_MST2_ASSERT_F(y, 0.5f, "precision Line2D y at x=1.5");

    }

    // ============================================================
    // 主入口
    // ============================================================
    static void test()
    {
        testLine2B();
        testLine2DSloped();
        testLine2DVertical();
        testLine2DHorizontal();
        testLine2DNegativeSlope();
        testLine3Conversions();
        testRectB();
        testRect3B();
        testTriangle2B();
        testTriangle3B();
        testGeometryScenario();
        testGeometryPrecision();
    }
};

struct MathStructTest3Impl
{

    // ============================================================
    // MathStructTest3 — Vector4 / Vector4Int / Vector2Int 全方法测试
    // MathStructTest 覆盖了 Vector2/Vector3/Color/Quaternion
    // MathStructTest2 覆盖了 Line2/Line3/Rect/Triangle
    // 本文件覆盖剩余未测试的数学结构体：
    //   Vector4 (float): ctor/clear/+/-/+=/−=/unary-
    //   Vector4Int: ctor/clear/+/-/+=/−=/unary-/*/==/!=/< 
    //   Vector2Int: ctor/clear/+/-/+=/−=/unary-/*/==/!=/</ std::hash
    // ============================================================

#define MathStructTest3Impl_MS3_ASSERT(expr, msg) if (!(expr)) { ERROR(string("MathStructTest3 FAILED: ") + (msg)); }
#define MathStructTest3Impl_MS3_FLOAT_EQ(a, b) (fabsf((a) - (b)) < 1e-4f)

// ============================================================
// Vector4 (float)
// ============================================================
    static void testVector4_DefaultCtor()
    {
        Vector4 v;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.x, 0.0f), "V4 default x==0");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.y, 0.0f), "V4 default y==0");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.z, 0.0f), "V4 default z==0");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.w, 0.0f), "V4 default w==0");
    }

    static void testVector4_ParamCtor()
    {
        Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.x, 1.0f), "V4 ctor x==1");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.y, 2.0f), "V4 ctor y==2");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.z, 3.0f), "V4 ctor z==3");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.w, 4.0f), "V4 ctor w==4");
    }

    static void testVector4_Clear()
    {
        Vector4 v(10.0f, 20.0f, 30.0f, 40.0f);
        v.clear();
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.x, 0.0f), "V4 clear x==0");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.y, 0.0f), "V4 clear y==0");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.z, 0.0f), "V4 clear z==0");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.w, 0.0f), "V4 clear w==0");
    }

    static void testVector4_Add()
    {
        Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4 b(10.0f, 20.0f, 30.0f, 40.0f);
        Vector4 c = a + b;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.x, 11.0f), "V4 add x==11");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.y, 22.0f), "V4 add y==22");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.z, 33.0f), "V4 add z==33");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.w, 44.0f), "V4 add w==44");
    }

    static void testVector4_Sub()
    {
        Vector4 a(10.0f, 20.0f, 30.0f, 40.0f);
        Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4 c = a - b;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.x, 9.0f), "V4 sub x==9");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.y, 18.0f), "V4 sub y==18");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.z, 27.0f), "V4 sub z==27");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.w, 36.0f), "V4 sub w==36");
    }

    static void testVector4_AddAssign()
    {
        Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
        a += Vector4(10.0f, 10.0f, 10.0f, 10.0f);
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(a.x, 11.0f), "V4 += x==11");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(a.y, 12.0f), "V4 += y==12");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(a.z, 13.0f), "V4 += z==13");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(a.w, 14.0f), "V4 += w==14");
    }

    static void testVector4_SubAssign()
    {
        Vector4 a(10.0f, 20.0f, 30.0f, 40.0f);
        a -= Vector4(1.0f, 2.0f, 3.0f, 4.0f);
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(a.x, 9.0f), "V4 -= x==9");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(a.y, 18.0f), "V4 -= y==18");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(a.z, 27.0f), "V4 -= z==27");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(a.w, 36.0f), "V4 -= w==36");
    }

    static void testVector4_Negate()
    {
        Vector4 a(1.0f, -2.0f, 3.0f, -4.0f);
        Vector4 n = -a;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(n.x, -1.0f), "V4 negate x==-1");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(n.y, 2.0f), "V4 negate y==2");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(n.z, -3.0f), "V4 negate z==-3");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(n.w, 4.0f), "V4 negate w==4");
    }

    static void testVector4_AddSubChain()
    {
        Vector4 v(1.0f, 1.0f, 1.0f, 1.0f);
        for (int i = 0; i < 10; ++i)
        {
            v += Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        }
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.x, 11.0f), "V4 chain add x==11");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.w, 11.0f), "V4 chain add w==11");
        for (int i = 0; i < 10; ++i)
        {
            v -= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        }
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.x, 1.0f), "V4 chain sub x==1");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.w, 1.0f), "V4 chain sub w==1");
    }

    static void testVector4_NegativeValues()
    {
        Vector4 a(-1.0f, -2.0f, -3.0f, -4.0f);
        Vector4 b(-5.0f, -6.0f, -7.0f, -8.0f);
        Vector4 c = a + b;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.x, -6.0f), "V4 neg add x==-6");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.w, -12.0f), "V4 neg add w==-12");
        Vector4 d = a - b;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(d.x, 4.0f), "V4 neg sub x==4");
    }

    static void testVector4_ZeroAdd()
    {
        Vector4 v(5.0f, 6.0f, 7.0f, 8.0f);
        Vector4 z;
        Vector4 r = v + z;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(r.x, 5.0f), "V4 add zero x unchanged");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(r.w, 8.0f), "V4 add zero w unchanged");
    }

    static void testVector4_SelfAddSub()
    {
        Vector4 a(3.0f, 4.0f, 5.0f, 6.0f);
        Vector4 b = a + a;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(b.x, 6.0f), "V4 self add x==6");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(b.w, 12.0f), "V4 self add w==12");
        Vector4 c = b - a;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(c.x, 3.0f), "V4 self sub x==3");
    }

    static void testVector4_LargeValues()
    {
        Vector4 a(1e6f, 2e6f, 3e6f, 4e6f);
        Vector4 b = a + a;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(b.x / 1e6f, 2.0f), "V4 large x==2M");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(b.w / 1e6f, 8.0f), "V4 large w==8M");
    }

    static void testVector4_AddAssignChain()
    {
        Vector4 v;
        Vector4 step(1.0f, 2.0f, 3.0f, 4.0f);
        for (int i = 0; i < 5; ++i)
        {
            v += step;
        }
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.x, 5.0f), "V4 chain+= x==5");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.y, 10.0f), "V4 chain+= y==10");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.z, 15.0f), "V4 chain+= z==15");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.w, 20.0f), "V4 chain+= w==20");
    }

    static void testVector4_SubAssignChain()
    {
        Vector4 v(100.0f, 100.0f, 100.0f, 100.0f);
        Vector4 step(10.0f, 10.0f, 10.0f, 10.0f);
        for (int i = 0; i < 10; ++i)
        {
            v -= step;
        }
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.x, 0.0f), "V4 chain-= x==0");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(v.w, 0.0f), "V4 chain-= w==0");
    }

    static void testVector4_CopyCtor()
    {
        Vector4 src(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4 dst(src);
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(dst.x, 1.0f), "V4 copy ctor x");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(dst.w, 4.0f), "V4 copy ctor w");
        src.x = 99.0f;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(dst.x, 1.0f), "V4 copy ctor independent");
    }

    static void testVector4_AssignOp()
    {
        Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4 b;
        b = a;
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(b.x, 1.0f), "V4 assign x");
        MathStructTest3Impl_MS3_ASSERT(MathStructTest3Impl_MS3_FLOAT_EQ(b.w, 4.0f), "V4 assign w");
    }

    // ============================================================
    // Vector4Int
    // ============================================================
    static void testVector4Int_DefaultCtor()
    {
        Vector4Int v;
        MathStructTest3Impl_MS3_ASSERT(v.x == 0, "V4I default x==0");
        MathStructTest3Impl_MS3_ASSERT(v.y == 0, "V4I default y==0");
        MathStructTest3Impl_MS3_ASSERT(v.z == 0, "V4I default z==0");
        MathStructTest3Impl_MS3_ASSERT(v.w == 0, "V4I default w==0");
    }

    static void testVector4Int_ParamCtor()
    {
        Vector4Int v(1, 2, 3, 4);
        MathStructTest3Impl_MS3_ASSERT(v.x == 1, "V4I ctor x==1");
        MathStructTest3Impl_MS3_ASSERT(v.y == 2, "V4I ctor y==2");
        MathStructTest3Impl_MS3_ASSERT(v.z == 3, "V4I ctor z==3");
        MathStructTest3Impl_MS3_ASSERT(v.w == 4, "V4I ctor w==4");
    }

    static void testVector4Int_Clear()
    {
        Vector4Int v(10, 20, 30, 40);
        v.clear();
        MathStructTest3Impl_MS3_ASSERT(v.x == 0, "V4I clear x==0");
        MathStructTest3Impl_MS3_ASSERT(v.y == 0, "V4I clear y==0");
        MathStructTest3Impl_MS3_ASSERT(v.z == 0, "V4I clear z==0");
        MathStructTest3Impl_MS3_ASSERT(v.w == 0, "V4I clear w==0");
    }

    static void testVector4Int_Add()
    {
        Vector4Int a(1, 2, 3, 4);
        Vector4Int b(10, 20, 30, 40);
        Vector4Int c = a + b;
        MathStructTest3Impl_MS3_ASSERT(c.x == 11, "V4I add x==11");
        MathStructTest3Impl_MS3_ASSERT(c.y == 22, "V4I add y==22");
        MathStructTest3Impl_MS3_ASSERT(c.z == 33, "V4I add z==33");
        MathStructTest3Impl_MS3_ASSERT(c.w == 44, "V4I add w==44");
    }

    static void testVector4Int_Sub()
    {
        Vector4Int a(100, 200, 300, 400);
        Vector4Int b(1, 2, 3, 4);
        Vector4Int c = a - b;
        MathStructTest3Impl_MS3_ASSERT(c.x == 99, "V4I sub x==99");
        MathStructTest3Impl_MS3_ASSERT(c.y == 198, "V4I sub y==198");
        MathStructTest3Impl_MS3_ASSERT(c.z == 297, "V4I sub z==297");
        MathStructTest3Impl_MS3_ASSERT(c.w == 396, "V4I sub w==396");
    }

    static void testVector4Int_AddAssign()
    {
        Vector4Int a(1, 2, 3, 4);
        a += Vector4Int(10, 10, 10, 10);
        MathStructTest3Impl_MS3_ASSERT(a.x == 11, "V4I += x==11");
        MathStructTest3Impl_MS3_ASSERT(a.y == 12, "V4I += y==12");
        MathStructTest3Impl_MS3_ASSERT(a.z == 13, "V4I += z==13");
        MathStructTest3Impl_MS3_ASSERT(a.w == 14, "V4I += w==14");
    }

    static void testVector4Int_SubAssign()
    {
        Vector4Int a(20, 30, 40, 50);
        a -= Vector4Int(5, 5, 5, 5);
        MathStructTest3Impl_MS3_ASSERT(a.x == 15, "V4I -= x==15");
        MathStructTest3Impl_MS3_ASSERT(a.y == 25, "V4I -= y==25");
        MathStructTest3Impl_MS3_ASSERT(a.z == 35, "V4I -= z==35");
        MathStructTest3Impl_MS3_ASSERT(a.w == 45, "V4I -= w==45");
    }

    static void testVector4Int_Negate()
    {
        Vector4Int a(1, -2, 3, -4);
        Vector4Int n = -a;
        MathStructTest3Impl_MS3_ASSERT(n.x == -1, "V4I negate x==-1");
        MathStructTest3Impl_MS3_ASSERT(n.y == 2, "V4I negate y==2");
        MathStructTest3Impl_MS3_ASSERT(n.z == -3, "V4I negate z==-3");
        MathStructTest3Impl_MS3_ASSERT(n.w == 4, "V4I negate w==4");
    }

    static void testVector4Int_MulScalar()
    {
        Vector4Int a(2, 3, 4, 5);
        Vector4Int r = a * 10;
        MathStructTest3Impl_MS3_ASSERT(r.x == 20, "V4I mul x==20");
        MathStructTest3Impl_MS3_ASSERT(r.y == 30, "V4I mul y==30");
        MathStructTest3Impl_MS3_ASSERT(r.z == 40, "V4I mul z==40");
        MathStructTest3Impl_MS3_ASSERT(r.w == 50, "V4I mul w==50");
    }

    static void testVector4Int_MulByZero()
    {
        Vector4Int a(7, 8, 9, 10);
        Vector4Int r = a * 0;
        MathStructTest3Impl_MS3_ASSERT(r.x == 0, "V4I mul0 x==0");
        MathStructTest3Impl_MS3_ASSERT(r.y == 0, "V4I mul0 y==0");
        MathStructTest3Impl_MS3_ASSERT(r.z == 0, "V4I mul0 z==0");
        MathStructTest3Impl_MS3_ASSERT(r.w == 0, "V4I mul0 w==0");
    }

    static void testVector4Int_MulByOne()
    {
        Vector4Int a(3, 4, 5, 6);
        Vector4Int r = a * 1;
        MathStructTest3Impl_MS3_ASSERT(r.x == 3, "V4I mul1 x unchanged");
        MathStructTest3Impl_MS3_ASSERT(r.w == 6, "V4I mul1 w unchanged");
    }

    static void testVector4Int_MulByNeg()
    {
        Vector4Int a(1, 2, 3, 4);
        Vector4Int r = a * (-3);
        MathStructTest3Impl_MS3_ASSERT(r.x == -3, "V4I mulNeg x==-3");
        MathStructTest3Impl_MS3_ASSERT(r.y == -6, "V4I mulNeg y==-6");
        MathStructTest3Impl_MS3_ASSERT(r.z == -9, "V4I mulNeg z==-9");
        MathStructTest3Impl_MS3_ASSERT(r.w == -12, "V4I mulNeg w==-12");
    }

    static void testVector4Int_Equality()
    {
        Vector4Int a(1, 2, 3, 4);
        Vector4Int b(1, 2, 3, 4);
        Vector4Int c(1, 2, 3, 5);
        MathStructTest3Impl_MS3_ASSERT(a == b, "V4I eq same");
        MathStructTest3Impl_MS3_ASSERT(!(a == c), "V4I eq diff");
        MathStructTest3Impl_MS3_ASSERT(a != c, "V4I neq diff");
        MathStructTest3Impl_MS3_ASSERT(!(a != b), "V4I not neq same");
    }

    static void testVector4Int_LessThan()
    {
        // < 按 x+y+z+w 比较
        Vector4Int a(1, 1, 1, 1); // sum=4
        Vector4Int b(2, 2, 2, 2); // sum=8
        MathStructTest3Impl_MS3_ASSERT(a < b, "V4I < a<b");
        MathStructTest3Impl_MS3_ASSERT(!(b < a), "V4I < !(b<a)");
    }

    static void testVector4Int_LessThan_Equal()
    {
        Vector4Int a(1, 2, 3, 4); // sum=10
        Vector4Int b(4, 3, 2, 1); // sum=10
        MathStructTest3Impl_MS3_ASSERT(!(a < b), "V4I < equal not less");
        MathStructTest3Impl_MS3_ASSERT(!(b < a), "V4I < equal not less reverse");
    }

    static void testVector4Int_NegativeValues()
    {
        Vector4Int a(-10, -20, -30, -40);
        Vector4Int b(10, 20, 30, 40);
        Vector4Int c = a + b;
        MathStructTest3Impl_MS3_ASSERT(c.x == 0, "V4I neg+pos x==0");
        MathStructTest3Impl_MS3_ASSERT(c.y == 0, "V4I neg+pos y==0");
        MathStructTest3Impl_MS3_ASSERT(c.z == 0, "V4I neg+pos z==0");
        MathStructTest3Impl_MS3_ASSERT(c.w == 0, "V4I neg+pos w==0");
    }

    static void testVector4Int_Chain()
    {
        Vector4Int v;
        for (int i = 0; i < 10; ++i)
        {
            v += Vector4Int(1, 2, 3, 4);
        }
        MathStructTest3Impl_MS3_ASSERT(v.x == 10, "V4I chain x==10");
        MathStructTest3Impl_MS3_ASSERT(v.y == 20, "V4I chain y==20");
        MathStructTest3Impl_MS3_ASSERT(v.z == 30, "V4I chain z==30");
        MathStructTest3Impl_MS3_ASSERT(v.w == 40, "V4I chain w==40");
    }

    static void testVector4Int_UseInHashMap()
    {
        // Vector4Int 本身没有 hash，但可以用 Map（有序）
        Map<int, Vector4Int> m;
        m.add(1, Vector4Int(1, 2, 3, 4));
        m.add(2, Vector4Int(5, 6, 7, 8));
        MathStructTest3Impl_MS3_ASSERT(m.contains(1), "V4I in Map contains 1");
        const Vector4Int& v = m.get(1);
        MathStructTest3Impl_MS3_ASSERT(v == Vector4Int(1, 2, 3, 4), "V4I in Map value");
    }

    static void testVector4Int_SumOfComponents()
    {
        Vector4Int a(1, 2, 3, 4);
        int sum = a.x + a.y + a.z + a.w;
        MathStructTest3Impl_MS3_ASSERT(sum == 10, "V4I sum of components==10");
    }

    // ============================================================
    // Vector2Int
    // ============================================================
    static void testVector2Int_DefaultCtor()
    {
        Vector2Int v;
        MathStructTest3Impl_MS3_ASSERT(v.x == 0, "V2I default x==0");
        MathStructTest3Impl_MS3_ASSERT(v.y == 0, "V2I default y==0");
    }

    static void testVector2Int_ParamCtor()
    {
        Vector2Int v(5, 7);
        MathStructTest3Impl_MS3_ASSERT(v.x == 5, "V2I ctor x==5");
        MathStructTest3Impl_MS3_ASSERT(v.y == 7, "V2I ctor y==7");
    }

    static void testVector2Int_Clear()
    {
        Vector2Int v(100, 200);
        v.clear();
        MathStructTest3Impl_MS3_ASSERT(v.x == 0, "V2I clear x==0");
        MathStructTest3Impl_MS3_ASSERT(v.y == 0, "V2I clear y==0");
    }

    static void testVector2Int_Add()
    {
        Vector2Int a(3, 4);
        Vector2Int b(7, 6);
        Vector2Int c = a + b;
        MathStructTest3Impl_MS3_ASSERT(c.x == 10, "V2I add x==10");
        MathStructTest3Impl_MS3_ASSERT(c.y == 10, "V2I add y==10");
    }

    static void testVector2Int_Sub()
    {
        Vector2Int a(10, 15);
        Vector2Int b(3, 5);
        Vector2Int c = a - b;
        MathStructTest3Impl_MS3_ASSERT(c.x == 7, "V2I sub x==7");
        MathStructTest3Impl_MS3_ASSERT(c.y == 10, "V2I sub y==10");
    }

    static void testVector2Int_AddAssign()
    {
        Vector2Int a(1, 2);
        a += Vector2Int(3, 4);
        MathStructTest3Impl_MS3_ASSERT(a.x == 4, "V2I += x==4");
        MathStructTest3Impl_MS3_ASSERT(a.y == 6, "V2I += y==6");
    }

    static void testVector2Int_SubAssign()
    {
        Vector2Int a(10, 20);
        a -= Vector2Int(3, 5);
        MathStructTest3Impl_MS3_ASSERT(a.x == 7, "V2I -= x==7");
        MathStructTest3Impl_MS3_ASSERT(a.y == 15, "V2I -= y==15");
    }

    static void testVector2Int_Negate()
    {
        Vector2Int a(3, -5);
        Vector2Int n = -a;
        MathStructTest3Impl_MS3_ASSERT(n.x == -3, "V2I negate x==-3");
        MathStructTest3Impl_MS3_ASSERT(n.y == 5, "V2I negate y==5");
    }

    static void testVector2Int_MulScalar()
    {
        Vector2Int a(4, 5);
        Vector2Int r = a * 3;
        MathStructTest3Impl_MS3_ASSERT(r.x == 12, "V2I mul x==12");
        MathStructTest3Impl_MS3_ASSERT(r.y == 15, "V2I mul y==15");
    }

    static void testVector2Int_MulByZero()
    {
        Vector2Int a(9, 11);
        Vector2Int r = a * 0;
        MathStructTest3Impl_MS3_ASSERT(r.x == 0, "V2I mul0 x==0");
        MathStructTest3Impl_MS3_ASSERT(r.y == 0, "V2I mul0 y==0");
    }

    static void testVector2Int_MulByNeg()
    {
        Vector2Int a(3, 4);
        Vector2Int r = a * (-2);
        MathStructTest3Impl_MS3_ASSERT(r.x == -6, "V2I mulNeg x==-6");
        MathStructTest3Impl_MS3_ASSERT(r.y == -8, "V2I mulNeg y==-8");
    }

    static void testVector2Int_Equality()
    {
        Vector2Int a(3, 4);
        Vector2Int b(3, 4);
        Vector2Int c(3, 5);
        MathStructTest3Impl_MS3_ASSERT(a == b, "V2I eq same");
        MathStructTest3Impl_MS3_ASSERT(!(a == c), "V2I eq diff");
        MathStructTest3Impl_MS3_ASSERT(a != c, "V2I neq diff");
        MathStructTest3Impl_MS3_ASSERT(!(a != b), "V2I not neq same");
    }

    static void testVector2Int_LessThan()
    {
        // < 按 x+y 比较
        Vector2Int a(1, 1); // sum=2
        Vector2Int b(2, 3); // sum=5
        MathStructTest3Impl_MS3_ASSERT(a < b, "V2I < a<b");
        MathStructTest3Impl_MS3_ASSERT(!(b < a), "V2I < !(b<a)");
    }

    static void testVector2Int_LessThan_Equal()
    {
        Vector2Int a(3, 2); // sum=5
        Vector2Int b(1, 4); // sum=5
        MathStructTest3Impl_MS3_ASSERT(!(a < b), "V2I < eq not less");
        MathStructTest3Impl_MS3_ASSERT(!(b < a), "V2I < eq not less rev");
    }

    static void testVector2Int_StaticZero()
    {
        Vector2Int z = Vector2Int::ZERO;
        MathStructTest3Impl_MS3_ASSERT(z.x == 0 && z.y == 0, "V2I ZERO");
    }

    static void testVector2Int_StdHash()
    {
        // 测试 std::hash<Vector2Int> 可以在 unordered_map 中使用
        unordered_map<Vector2Int, int> hm;
        Vector2Int k1(1, 2);
        Vector2Int k2(3, 4);
        Vector2Int k3(1, 2); // same as k1
        hm[k1] = 100;
        hm[k2] = 200;
        hm[k3] = 300; // 覆盖 k1
        MathStructTest3Impl_MS3_ASSERT(hm.count(k1) == 1, "V2I hash contains k1");
        MathStructTest3Impl_MS3_ASSERT(hm[k1] == 300, "V2I hash k1 updated to 300");
        MathStructTest3Impl_MS3_ASSERT(hm[k2] == 200, "V2I hash k2==200");
    }

    static void testVector2Int_StdHash_Distinct()
    {
        // 不同坐标应有不同哈希（实际上无法保证100%，但常见值不同）
        std::hash<Vector2Int> h;
        size_t h3 = h(Vector2Int(1, 1));
        // h1/h2 可能不同（期望），h3 肯定不等于 h(ZERO)
        size_t h0 = h(Vector2Int(0, 0));
        MathStructTest3Impl_MS3_ASSERT(h3 != h0, "V2I hash (1,1) != (0,0)");
    }

    static void testVector2Int_UseAsMapKey()
    {
        // Vector2Int::operator< 用 x+y 排序，(1,0) 与 (0,1) 的 x+y 均为 1 → 视为相同 key
        // 使用 x+y 互不相同的 key 保证 3 个不同 key
        Map<Vector2Int, int> m;
        m.add(Vector2Int(0, 0), 1);  // x+y=0
        m.add(Vector2Int(1, 0), 2);  // x+y=1
        m.add(Vector2Int(2, 0), 3);  // x+y=2
        MathStructTest3Impl_MS3_ASSERT(m.size() == 3, "V2I map key size==3");
        MathStructTest3Impl_MS3_ASSERT(m.contains(Vector2Int(1, 0)), "V2I map key (1,0) found");
        MathStructTest3Impl_MS3_ASSERT(m.get(Vector2Int(2, 0), -1) == 3, "V2I map key (2,0) val==3");
    }

    static void testVector2Int_Chain()
    {
        Vector2Int v(0, 0);
        for (int i = 1; i <= 5; ++i)
        {
            v += Vector2Int(i, i);
        }
        // sum: 1+2+3+4+5 = 15
        MathStructTest3Impl_MS3_ASSERT(v.x == 15, "V2I chain x==15");
        MathStructTest3Impl_MS3_ASSERT(v.y == 15, "V2I chain y==15");
        for (int i = 1; i <= 5; ++i)
        {
            v -= Vector2Int(i, i);
        }
        MathStructTest3Impl_MS3_ASSERT(v.x == 0, "V2I chain sub x==0");
        MathStructTest3Impl_MS3_ASSERT(v.y == 0, "V2I chain sub y==0");
    }

    static void testVector2Int_NegativeCoords()
    {
        Vector2Int a(-5, -3);
        Vector2Int b(2, 1);
        Vector2Int c = a + b;
        MathStructTest3Impl_MS3_ASSERT(c.x == -3, "V2I negCoords x==-3");
        MathStructTest3Impl_MS3_ASSERT(c.y == -2, "V2I negCoords y==-2");
        Vector2Int n = -a;
        MathStructTest3Impl_MS3_ASSERT(n.x == 5, "V2I negCoords neg x==5");
        MathStructTest3Impl_MS3_ASSERT(n.y == 3, "V2I negCoords neg y==3");
    }

    // ============================================================
    // 主入口
    // ============================================================
    static void test()
    {
        // Vector4
        testVector4_DefaultCtor();
        testVector4_ParamCtor();
        testVector4_Clear();
        testVector4_Add();
        testVector4_Sub();
        testVector4_AddAssign();
        testVector4_SubAssign();
        testVector4_Negate();
        testVector4_AddSubChain();
        testVector4_NegativeValues();
        testVector4_ZeroAdd();
        testVector4_SelfAddSub();
        testVector4_LargeValues();
        testVector4_AddAssignChain();
        testVector4_SubAssignChain();
        testVector4_CopyCtor();
        testVector4_AssignOp();

        // Vector4Int
        testVector4Int_DefaultCtor();
        testVector4Int_ParamCtor();
        testVector4Int_Clear();
        testVector4Int_Add();
        testVector4Int_Sub();
        testVector4Int_AddAssign();
        testVector4Int_SubAssign();
        testVector4Int_Negate();
        testVector4Int_MulScalar();
        testVector4Int_MulByZero();
        testVector4Int_MulByOne();
        testVector4Int_MulByNeg();
        testVector4Int_Equality();
        testVector4Int_LessThan();
        testVector4Int_LessThan_Equal();
        testVector4Int_NegativeValues();
        testVector4Int_Chain();
        testVector4Int_UseInHashMap();
        testVector4Int_SumOfComponents();

        // Vector2Int
        testVector2Int_DefaultCtor();
        testVector2Int_ParamCtor();
        testVector2Int_Clear();
        testVector2Int_Add();
        testVector2Int_Sub();
        testVector2Int_AddAssign();
        testVector2Int_SubAssign();
        testVector2Int_Negate();
        testVector2Int_MulScalar();
        testVector2Int_MulByZero();
        testVector2Int_MulByNeg();
        testVector2Int_Equality();
        testVector2Int_LessThan();
        testVector2Int_LessThan_Equal();
        testVector2Int_StaticZero();
        testVector2Int_StdHash();
        testVector2Int_StdHash_Distinct();
        testVector2Int_UseAsMapKey();
        testVector2Int_Chain();
        testVector2Int_NegativeCoords();
    }
};

struct MathStructTest4Impl
{

    // ============================================================
    // MathStructTest4
    // Testing integer vector types:
    //   Vector2Int / Vector2UInt / Vector2Short / Vector2UShort /
    //   Vector3Int / Vector4Int
    // ============================================================

#define MathStructTest4Impl_MST4_ASSERT(expr) \
	if (!(expr)) { ERROR(string("MathStructTest4 FAIL: ") + #expr); }

    static void test()
    {
        // Vector2Int
        testVector2IntDefault();
        testVector2IntConstruct();
        testVector2IntAdd();
        testVector2IntSub();
        testVector2IntMul();
        testVector2IntNeg();
        testVector2IntAddAssign();
        testVector2IntSubAssign();
        testVector2IntEqual();
        testVector2IntNotEqual();
        testVector2IntLess();
        testVector2IntClear();
        testVector2IntZero();
        testVector2IntHash();
        testVector2IntChainOps();
        testVector2IntBoundary();
        // Vector2UInt
        testVector2UIntDefault();
        testVector2UIntConstruct();
        testVector2UIntAdd();
        testVector2UIntSub();
        testVector2UIntEqual();
        testVector2UIntNotEqual();
        testVector2UIntClear();
        testVector2UIntLarge();
        // Vector2Short
        testVector2ShortDefault();
        testVector2ShortConstruct();
        testVector2ShortAddSub();
        testVector2ShortEqual();
        testVector2ShortBoundary();
        // Vector2UShort
        testVector2UShortDefault();
        testVector2UShortConstruct();
        testVector2UShortAddSub();
        testVector2UShortEqual();
        // Vector3Int
        testVector3IntDefault();
        testVector3IntConstruct();
        testVector3IntAdd();
        testVector3IntSub();
        testVector3IntMul();
        testVector3IntEqual();
        testVector3IntNotEqual();
        testVector3IntClear();
        testVector3IntZero();
        testVector3IntBoundary();
        testVector3IntChain();
        // Vector4Int
        testVector4IntDefault();
        testVector4IntConstruct();
        testVector4IntAdd();
        testVector4IntSub();
        testVector4IntMul();
        testVector4IntEqual();
        testVector4IntNotEqual();
        testVector4IntClear();
        testVector4IntBoundary();
        testVector4IntChain();
        // Mixed
        testMixedIntVectorOps();
        testIntVectorStress();
    }

    // ============================================================
    // Vector2Int tests
    // ============================================================

    static void testVector2IntDefault()
    {
        Vector2Int v;
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
    }

    static void testVector2IntConstruct()
    {
        Vector2Int v(3, -7);
        MathStructTest4Impl_MST4_ASSERT(v.x == 3);
        MathStructTest4Impl_MST4_ASSERT(v.y == -7);
        Vector2Int v2(-100, 200);
        MathStructTest4Impl_MST4_ASSERT(v2.x == -100);
        MathStructTest4Impl_MST4_ASSERT(v2.y == 200);
        Vector2Int v3(0, 0);
        MathStructTest4Impl_MST4_ASSERT(v3.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v3.y == 0);
    }

    static void testVector2IntAdd()
    {
        Vector2Int a(1, 2);
        Vector2Int b(3, 4);
        Vector2Int c = a + b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 4);
        MathStructTest4Impl_MST4_ASSERT(c.y == 6);
        Vector2Int d(-5, 10);
        Vector2Int e(5, -10);
        Vector2Int f = d + e;
        MathStructTest4Impl_MST4_ASSERT(f.x == 0);
        MathStructTest4Impl_MST4_ASSERT(f.y == 0);
        Vector2Int g(0, 0);
        Vector2Int h(7, -3);
        Vector2Int i2 = g + h;
        MathStructTest4Impl_MST4_ASSERT(i2.x == 7);
        MathStructTest4Impl_MST4_ASSERT(i2.y == -3);
    }

    static void testVector2IntSub()
    {
        Vector2Int a(10, 5);
        Vector2Int b(3, 2);
        Vector2Int c = a - b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 7);
        MathStructTest4Impl_MST4_ASSERT(c.y == 3);
        Vector2Int d(0, 0);
        Vector2Int e(4, -4);
        Vector2Int f = d - e;
        MathStructTest4Impl_MST4_ASSERT(f.x == -4);
        MathStructTest4Impl_MST4_ASSERT(f.y == 4);
    }

    static void testVector2IntMul()
    {
        Vector2Int a(3, -4);
        Vector2Int b = a * 5;
        MathStructTest4Impl_MST4_ASSERT(b.x == 15);
        MathStructTest4Impl_MST4_ASSERT(b.y == -20);
        Vector2Int c(7, 2);
        Vector2Int d = c * 0;
        MathStructTest4Impl_MST4_ASSERT(d.x == 0);
        MathStructTest4Impl_MST4_ASSERT(d.y == 0);
        Vector2Int e(-3, -3);
        Vector2Int f = e * (-2);
        MathStructTest4Impl_MST4_ASSERT(f.x == 6);
        MathStructTest4Impl_MST4_ASSERT(f.y == 6);
    }

    static void testVector2IntNeg()
    {
        Vector2Int a(5, -8);
        Vector2Int b = -a;
        MathStructTest4Impl_MST4_ASSERT(b.x == -5);
        MathStructTest4Impl_MST4_ASSERT(b.y == 8);
        Vector2Int c(0, 0);
        Vector2Int d = -c;
        MathStructTest4Impl_MST4_ASSERT(d.x == 0);
        MathStructTest4Impl_MST4_ASSERT(d.y == 0);
    }

    static void testVector2IntAddAssign()
    {
        Vector2Int a(1, 2);
        Vector2Int b(3, 4);
        a += b;
        MathStructTest4Impl_MST4_ASSERT(a.x == 4);
        MathStructTest4Impl_MST4_ASSERT(a.y == 6);
        a += Vector2Int(-4, -6);
        MathStructTest4Impl_MST4_ASSERT(a.x == 0);
        MathStructTest4Impl_MST4_ASSERT(a.y == 0);
    }

    static void testVector2IntSubAssign()
    {
        Vector2Int a(10, 20);
        Vector2Int b(3, 5);
        a -= b;
        MathStructTest4Impl_MST4_ASSERT(a.x == 7);
        MathStructTest4Impl_MST4_ASSERT(a.y == 15);
        a -= Vector2Int(7, 15);
        MathStructTest4Impl_MST4_ASSERT(a.x == 0);
        MathStructTest4Impl_MST4_ASSERT(a.y == 0);
    }

    static void testVector2IntEqual()
    {
        Vector2Int a(5, 7);
        Vector2Int b(5, 7);
        Vector2Int c(5, 8);
        MathStructTest4Impl_MST4_ASSERT(a == b);
        MathStructTest4Impl_MST4_ASSERT(!(a == c));
        Vector2Int d(0, 0);
        Vector2Int e(0, 0);
        MathStructTest4Impl_MST4_ASSERT(d == e);
    }

    static void testVector2IntNotEqual()
    {
        Vector2Int a(1, 2);
        Vector2Int b(1, 3);
        Vector2Int c(1, 2);
        MathStructTest4Impl_MST4_ASSERT(a != b);
        MathStructTest4Impl_MST4_ASSERT(!(a != c));
    }

    static void testVector2IntLess()
    {
        Vector2Int a(1, 1);
        Vector2Int b(2, 2);
        MathStructTest4Impl_MST4_ASSERT(a < b);
        MathStructTest4Impl_MST4_ASSERT(!(b < a));
        Vector2Int c(3, 0);
        Vector2Int d(1, 2);
        // c.x+c.y == 3, d.x+d.y == 3 => not less
        MathStructTest4Impl_MST4_ASSERT(!(c < d));
        MathStructTest4Impl_MST4_ASSERT(!(d < c));
    }

    static void testVector2IntClear()
    {
        Vector2Int v(10, -5);
        v.clear();
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
    }

    static void testVector2IntZero()
    {
        Vector2Int z = Vector2Int::ZERO;
        MathStructTest4Impl_MST4_ASSERT(z.x == 0);
        MathStructTest4Impl_MST4_ASSERT(z.y == 0);
    }

    static void testVector2IntHash()
    {
        Vector2Int a(10, 20);
        Vector2Int b(10, 20);
        Vector2Int c(20, 10);
        std::hash<Vector2Int> h;
        MathStructTest4Impl_MST4_ASSERT(h(a) == h(b));
        // Different coordinates may produce different hash (not guaranteed but typical)
        (void)h(c);
    }

    static void testVector2IntChainOps()
    {
        Vector2Int a(1, 1);
        Vector2Int b = (a + Vector2Int(2, 3)) * 2 - Vector2Int(1, 1);
        // (1+2, 1+3) = (3,4); *2 = (6,8); -(1,1) = (5,7)
        MathStructTest4Impl_MST4_ASSERT(b.x == 5);
        MathStructTest4Impl_MST4_ASSERT(b.y == 7);
    }

    static void testVector2IntBoundary()
    {
        Vector2Int a(2147483647, 2147483647);
        Vector2Int b(-2147483647, -2147483647);
        MathStructTest4Impl_MST4_ASSERT(a.x == 2147483647);
        MathStructTest4Impl_MST4_ASSERT(a.y == 2147483647);
        MathStructTest4Impl_MST4_ASSERT(b.x == -2147483647);
        MathStructTest4Impl_MST4_ASSERT(b.y == -2147483647);
        Vector2Int c = a + Vector2Int(-2147483647, -2147483647);
        MathStructTest4Impl_MST4_ASSERT(c.x == 0);
        MathStructTest4Impl_MST4_ASSERT(c.y == 0);
    }

    // ============================================================
    // Vector2UInt tests
    // ============================================================

    static void testVector2UIntDefault()
    {
        Vector2UInt v;
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
    }

    static void testVector2UIntConstruct()
    {
        Vector2UInt v(100u, 200u);
        MathStructTest4Impl_MST4_ASSERT(v.x == 100u);
        MathStructTest4Impl_MST4_ASSERT(v.y == 200u);
        Vector2UInt v2(0u, 0u);
        MathStructTest4Impl_MST4_ASSERT(v2.x == 0u);
        MathStructTest4Impl_MST4_ASSERT(v2.y == 0u);
    }

    static void testVector2UIntAdd()
    {
        Vector2UInt a(3u, 5u);
        Vector2UInt b(7u, 9u);
        Vector2UInt c = a + b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 10u);
        MathStructTest4Impl_MST4_ASSERT(c.y == 14u);
        Vector2UInt d(0u, 0u);
        Vector2UInt e(100u, 200u);
        Vector2UInt f = d + e;
        MathStructTest4Impl_MST4_ASSERT(f.x == 100u);
        MathStructTest4Impl_MST4_ASSERT(f.y == 200u);
    }

    static void testVector2UIntSub()
    {
        Vector2UInt a(10u, 20u);
        Vector2UInt b(3u, 7u);
        Vector2UInt c = a - b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 7u);
        MathStructTest4Impl_MST4_ASSERT(c.y == 13u);
        Vector2UInt d(5u, 5u);
        Vector2UInt e(5u, 5u);
        Vector2UInt f = d - e;
        MathStructTest4Impl_MST4_ASSERT(f.x == 0u);
        MathStructTest4Impl_MST4_ASSERT(f.y == 0u);
    }

    static void testVector2UIntEqual()
    {
        Vector2UInt a(7u, 8u);
        Vector2UInt b(7u, 8u);
        Vector2UInt c(7u, 9u);
        MathStructTest4Impl_MST4_ASSERT(a == b);
        MathStructTest4Impl_MST4_ASSERT(!(a == c));
    }

    static void testVector2UIntNotEqual()
    {
        Vector2UInt a(1u, 2u);
        Vector2UInt b(1u, 3u);
        Vector2UInt c(1u, 2u);
        MathStructTest4Impl_MST4_ASSERT(a != b);
        MathStructTest4Impl_MST4_ASSERT(!(a != c));
    }

    static void testVector2UIntClear()
    {
        Vector2UInt v(500u, 600u);
        v.clear();
        MathStructTest4Impl_MST4_ASSERT(v.x == 0u);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0u);
    }

    static void testVector2UIntLarge()
    {
        Vector2UInt a(4294967295u, 0u);
        MathStructTest4Impl_MST4_ASSERT(a.x == 4294967295u);
        MathStructTest4Impl_MST4_ASSERT(a.y == 0u);
        Vector2UInt b(0u, 4294967295u);
        MathStructTest4Impl_MST4_ASSERT(b.x == 0u);
        MathStructTest4Impl_MST4_ASSERT(b.y == 4294967295u);
    }

    // ============================================================
    // Vector2Short tests
    // ============================================================

    static void testVector2ShortDefault()
    {
        Vector2Short v;
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
    }

    static void testVector2ShortConstruct()
    {
        Vector2Short v((short)100, (short)-200);
        MathStructTest4Impl_MST4_ASSERT(v.x == 100);
        MathStructTest4Impl_MST4_ASSERT(v.y == -200);
        Vector2Short v2((short)-1, (short)1);
        MathStructTest4Impl_MST4_ASSERT(v2.x == -1);
        MathStructTest4Impl_MST4_ASSERT(v2.y == 1);
    }

    static void testVector2ShortAddSub()
    {
        Vector2Short a((short)10, (short)20);
        Vector2Short b((short)5, (short)-5);
        Vector2Short c = a + b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 15);
        MathStructTest4Impl_MST4_ASSERT(c.y == 15);
        Vector2Short d = a - b;
        MathStructTest4Impl_MST4_ASSERT(d.x == 5);
        MathStructTest4Impl_MST4_ASSERT(d.y == 25);
        Vector2Short e((short)3, (short)4);
        Vector2Short neg = -e;
        MathStructTest4Impl_MST4_ASSERT(neg.x == -3);
        MathStructTest4Impl_MST4_ASSERT(neg.y == -4);
        e += Vector2Short((short)1, (short)1);
        MathStructTest4Impl_MST4_ASSERT(e.x == 4);
        MathStructTest4Impl_MST4_ASSERT(e.y == 5);
        e -= Vector2Short((short)4, (short)5);
        MathStructTest4Impl_MST4_ASSERT(e.x == 0);
        MathStructTest4Impl_MST4_ASSERT(e.y == 0);
    }

    static void testVector2ShortEqual()
    {
        Vector2Short a((short)3, (short)4);
        Vector2Short b((short)3, (short)4);
        Vector2Short c((short)3, (short)5);
        MathStructTest4Impl_MST4_ASSERT(a == b);
        MathStructTest4Impl_MST4_ASSERT(!(a == c));
        MathStructTest4Impl_MST4_ASSERT(a != c);
        MathStructTest4Impl_MST4_ASSERT(!(a != b));
    }

    static void testVector2ShortBoundary()
    {
        Vector2Short a((short)32767, (short)-32767);
        MathStructTest4Impl_MST4_ASSERT(a.x == 32767);
        MathStructTest4Impl_MST4_ASSERT(a.y == -32767);
        Vector2Short b((short)0, (short)0);
        MathStructTest4Impl_MST4_ASSERT(b.x == 0);
        MathStructTest4Impl_MST4_ASSERT(b.y == 0);
        a.clear();
        MathStructTest4Impl_MST4_ASSERT(a.x == 0);
        MathStructTest4Impl_MST4_ASSERT(a.y == 0);
    }

    // ============================================================
    // Vector2UShort tests
    // ============================================================

    static void testVector2UShortDefault()
    {
        Vector2UShort v;
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
    }

    static void testVector2UShortConstruct()
    {
        Vector2UShort v((ushort)1000, (ushort)2000);
        MathStructTest4Impl_MST4_ASSERT(v.x == 1000);
        MathStructTest4Impl_MST4_ASSERT(v.y == 2000);
        Vector2UShort v2((ushort)0, (ushort)65535);
        MathStructTest4Impl_MST4_ASSERT(v2.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v2.y == 65535);
    }

    static void testVector2UShortAddSub()
    {
        Vector2UShort a((ushort)100, (ushort)200);
        Vector2UShort b((ushort)50, (ushort)100);
        Vector2UShort c = a + b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 150);
        MathStructTest4Impl_MST4_ASSERT(c.y == 300);
        Vector2UShort d = a - b;
        MathStructTest4Impl_MST4_ASSERT(d.x == 50);
        MathStructTest4Impl_MST4_ASSERT(d.y == 100);
        a += Vector2UShort((ushort)10, (ushort)20);
        MathStructTest4Impl_MST4_ASSERT(a.x == 110);
        MathStructTest4Impl_MST4_ASSERT(a.y == 220);
        a -= Vector2UShort((ushort)10, (ushort)20);
        MathStructTest4Impl_MST4_ASSERT(a.x == 100);
        MathStructTest4Impl_MST4_ASSERT(a.y == 200);
    }

    static void testVector2UShortEqual()
    {
        Vector2UShort a((ushort)5, (ushort)6);
        Vector2UShort b((ushort)5, (ushort)6);
        Vector2UShort c((ushort)5, (ushort)7);
        MathStructTest4Impl_MST4_ASSERT(a == b);
        MathStructTest4Impl_MST4_ASSERT(!(a == c));
        MathStructTest4Impl_MST4_ASSERT(a != c);
        MathStructTest4Impl_MST4_ASSERT(!(a != b));
        a.clear();
        MathStructTest4Impl_MST4_ASSERT(a.x == 0);
        MathStructTest4Impl_MST4_ASSERT(a.y == 0);
    }

    // ============================================================
    // Vector3Int tests
    // ============================================================

    static void testVector3IntDefault()
    {
        Vector3Int v;
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
        MathStructTest4Impl_MST4_ASSERT(v.z == 0);
    }

    static void testVector3IntConstruct()
    {
        Vector3Int v(1, -2, 3);
        MathStructTest4Impl_MST4_ASSERT(v.x == 1);
        MathStructTest4Impl_MST4_ASSERT(v.y == -2);
        MathStructTest4Impl_MST4_ASSERT(v.z == 3);
        Vector3Int v2(0, 0, 0);
        MathStructTest4Impl_MST4_ASSERT(v2.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v2.y == 0);
        MathStructTest4Impl_MST4_ASSERT(v2.z == 0);
        Vector3Int v3(-100, -200, -300);
        MathStructTest4Impl_MST4_ASSERT(v3.x == -100);
        MathStructTest4Impl_MST4_ASSERT(v3.y == -200);
        MathStructTest4Impl_MST4_ASSERT(v3.z == -300);
    }

    static void testVector3IntAdd()
    {
        Vector3Int a(1, 2, 3);
        Vector3Int b(4, 5, 6);
        Vector3Int c = a + b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 5);
        MathStructTest4Impl_MST4_ASSERT(c.y == 7);
        MathStructTest4Impl_MST4_ASSERT(c.z == 9);
        Vector3Int d(-1, -2, -3);
        Vector3Int e(1, 2, 3);
        Vector3Int f = d + e;
        MathStructTest4Impl_MST4_ASSERT(f.x == 0);
        MathStructTest4Impl_MST4_ASSERT(f.y == 0);
        MathStructTest4Impl_MST4_ASSERT(f.z == 0);
        Vector3Int g(0, 0, 0);
        Vector3Int h(7, -3, 5);
        Vector3Int i2 = g + h;
        MathStructTest4Impl_MST4_ASSERT(i2.x == 7);
        MathStructTest4Impl_MST4_ASSERT(i2.y == -3);
        MathStructTest4Impl_MST4_ASSERT(i2.z == 5);
    }

    static void testVector3IntSub()
    {
        Vector3Int a(10, 20, 30);
        Vector3Int b(1, 2, 3);
        Vector3Int c = a - b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 9);
        MathStructTest4Impl_MST4_ASSERT(c.y == 18);
        MathStructTest4Impl_MST4_ASSERT(c.z == 27);
        Vector3Int d(5, 5, 5);
        Vector3Int e(5, 5, 5);
        Vector3Int f = d - e;
        MathStructTest4Impl_MST4_ASSERT(f.x == 0);
        MathStructTest4Impl_MST4_ASSERT(f.y == 0);
        MathStructTest4Impl_MST4_ASSERT(f.z == 0);
        Vector3Int neg = -Vector3Int(3, -4, 5);
        MathStructTest4Impl_MST4_ASSERT(neg.x == -3);
        MathStructTest4Impl_MST4_ASSERT(neg.y == 4);
        MathStructTest4Impl_MST4_ASSERT(neg.z == -5);
    }

    static void testVector3IntMul()
    {
        Vector3Int a(2, -3, 4);
        Vector3Int b = a * 3;
        MathStructTest4Impl_MST4_ASSERT(b.x == 6);
        MathStructTest4Impl_MST4_ASSERT(b.y == -9);
        MathStructTest4Impl_MST4_ASSERT(b.z == 12);
        Vector3Int c(5, 6, 7);
        Vector3Int d = c * 0;
        MathStructTest4Impl_MST4_ASSERT(d.x == 0);
        MathStructTest4Impl_MST4_ASSERT(d.y == 0);
        MathStructTest4Impl_MST4_ASSERT(d.z == 0);
        Vector3Int e(-1, -2, -3);
        Vector3Int f = e * (-5);
        MathStructTest4Impl_MST4_ASSERT(f.x == 5);
        MathStructTest4Impl_MST4_ASSERT(f.y == 10);
        MathStructTest4Impl_MST4_ASSERT(f.z == 15);
    }

    static void testVector3IntEqual()
    {
        Vector3Int a(1, 2, 3);
        Vector3Int b(1, 2, 3);
        Vector3Int c(1, 2, 4);
        MathStructTest4Impl_MST4_ASSERT(a == b);
        MathStructTest4Impl_MST4_ASSERT(!(a == c));
        a += Vector3Int(1, 1, 1);
        MathStructTest4Impl_MST4_ASSERT(a.x == 2);
        MathStructTest4Impl_MST4_ASSERT(a.y == 3);
        MathStructTest4Impl_MST4_ASSERT(a.z == 4);
        a -= Vector3Int(1, 1, 1);
        MathStructTest4Impl_MST4_ASSERT(a.x == 1);
        MathStructTest4Impl_MST4_ASSERT(a.y == 2);
        MathStructTest4Impl_MST4_ASSERT(a.z == 3);
    }

    static void testVector3IntNotEqual()
    {
        Vector3Int a(1, 1, 1);
        Vector3Int b(1, 1, 2);
        Vector3Int c(1, 1, 1);
        MathStructTest4Impl_MST4_ASSERT(a != b);
        MathStructTest4Impl_MST4_ASSERT(!(a != c));
    }

    static void testVector3IntClear()
    {
        Vector3Int v(5, -3, 8);
        v.clear();
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
        MathStructTest4Impl_MST4_ASSERT(v.z == 0);
    }

    static void testVector3IntZero()
    {
        Vector3Int v(7, 8, 9);
        v.clear();
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
        MathStructTest4Impl_MST4_ASSERT(v.z == 0);
    }

    static void testVector3IntBoundary()
    {
        Vector3Int a(2147483647, -2147483647, 0);
        MathStructTest4Impl_MST4_ASSERT(a.x == 2147483647);
        MathStructTest4Impl_MST4_ASSERT(a.y == -2147483647);
        MathStructTest4Impl_MST4_ASSERT(a.z == 0);
        Vector3Int b(-2147483647, 0, 2147483647);
        Vector3Int c = a + b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 0);
        MathStructTest4Impl_MST4_ASSERT(c.y == -2147483647);
        MathStructTest4Impl_MST4_ASSERT(c.z == 2147483647);
    }

    static void testVector3IntChain()
    {
        Vector3Int a(1, 2, 3);
        Vector3Int b = (a + Vector3Int(1, 1, 1)) * 2 - Vector3Int(1, 1, 1);
        // (2,3,4)*2 = (4,6,8); -(1,1,1) = (3,5,7)
        MathStructTest4Impl_MST4_ASSERT(b.x == 3);
        MathStructTest4Impl_MST4_ASSERT(b.y == 5);
        MathStructTest4Impl_MST4_ASSERT(b.z == 7);
        // less operator: x+y+z
        Vector3Int lo(1, 1, 1);
        Vector3Int hi(2, 2, 2);
        MathStructTest4Impl_MST4_ASSERT(lo < hi);
        MathStructTest4Impl_MST4_ASSERT(!(hi < lo));
    }

    // ============================================================
    // Vector4Int tests
    // ============================================================

    static void testVector4IntDefault()
    {
        Vector4Int v;
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
        MathStructTest4Impl_MST4_ASSERT(v.z == 0);
        MathStructTest4Impl_MST4_ASSERT(v.w == 0);
    }

    static void testVector4IntConstruct()
    {
        Vector4Int v(1, -2, 3, -4);
        MathStructTest4Impl_MST4_ASSERT(v.x == 1);
        MathStructTest4Impl_MST4_ASSERT(v.y == -2);
        MathStructTest4Impl_MST4_ASSERT(v.z == 3);
        MathStructTest4Impl_MST4_ASSERT(v.w == -4);
        Vector4Int v2(0, 0, 0, 0);
        MathStructTest4Impl_MST4_ASSERT(v2.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v2.y == 0);
        MathStructTest4Impl_MST4_ASSERT(v2.z == 0);
        MathStructTest4Impl_MST4_ASSERT(v2.w == 0);
    }

    static void testVector4IntAdd()
    {
        Vector4Int a(1, 2, 3, 4);
        Vector4Int b(10, 20, 30, 40);
        Vector4Int c = a + b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 11);
        MathStructTest4Impl_MST4_ASSERT(c.y == 22);
        MathStructTest4Impl_MST4_ASSERT(c.z == 33);
        MathStructTest4Impl_MST4_ASSERT(c.w == 44);
        Vector4Int d(-1, -2, -3, -4);
        Vector4Int e(1, 2, 3, 4);
        Vector4Int f = d + e;
        MathStructTest4Impl_MST4_ASSERT(f.x == 0);
        MathStructTest4Impl_MST4_ASSERT(f.y == 0);
        MathStructTest4Impl_MST4_ASSERT(f.z == 0);
        MathStructTest4Impl_MST4_ASSERT(f.w == 0);
    }

    static void testVector4IntSub()
    {
        Vector4Int a(10, 20, 30, 40);
        Vector4Int b(1, 2, 3, 4);
        Vector4Int c = a - b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 9);
        MathStructTest4Impl_MST4_ASSERT(c.y == 18);
        MathStructTest4Impl_MST4_ASSERT(c.z == 27);
        MathStructTest4Impl_MST4_ASSERT(c.w == 36);
        Vector4Int neg = -Vector4Int(1, -2, 3, -4);
        MathStructTest4Impl_MST4_ASSERT(neg.x == -1);
        MathStructTest4Impl_MST4_ASSERT(neg.y == 2);
        MathStructTest4Impl_MST4_ASSERT(neg.z == -3);
        MathStructTest4Impl_MST4_ASSERT(neg.w == 4);
    }

    static void testVector4IntMul()
    {
        Vector4Int a(2, -3, 4, -5);
        Vector4Int b = a * 4;
        MathStructTest4Impl_MST4_ASSERT(b.x == 8);
        MathStructTest4Impl_MST4_ASSERT(b.y == -12);
        MathStructTest4Impl_MST4_ASSERT(b.z == 16);
        MathStructTest4Impl_MST4_ASSERT(b.w == -20);
        Vector4Int c(3, 3, 3, 3);
        Vector4Int d = c * 0;
        MathStructTest4Impl_MST4_ASSERT(d.x == 0);
        MathStructTest4Impl_MST4_ASSERT(d.y == 0);
        MathStructTest4Impl_MST4_ASSERT(d.z == 0);
        MathStructTest4Impl_MST4_ASSERT(d.w == 0);
    }

    static void testVector4IntEqual()
    {
        Vector4Int a(1, 2, 3, 4);
        Vector4Int b(1, 2, 3, 4);
        Vector4Int c(1, 2, 3, 5);
        MathStructTest4Impl_MST4_ASSERT(a == b);
        MathStructTest4Impl_MST4_ASSERT(!(a == c));
    }

    static void testVector4IntNotEqual()
    {
        Vector4Int a(1, 2, 3, 4);
        Vector4Int b(1, 2, 3, 5);
        Vector4Int c(1, 2, 3, 4);
        MathStructTest4Impl_MST4_ASSERT(a != b);
        MathStructTest4Impl_MST4_ASSERT(!(a != c));
    }

    static void testVector4IntClear()
    {
        Vector4Int v(7, -8, 9, -10);
        v.clear();
        MathStructTest4Impl_MST4_ASSERT(v.x == 0);
        MathStructTest4Impl_MST4_ASSERT(v.y == 0);
        MathStructTest4Impl_MST4_ASSERT(v.z == 0);
        MathStructTest4Impl_MST4_ASSERT(v.w == 0);
    }

    static void testVector4IntBoundary()
    {
        Vector4Int a(2147483647, -2147483647, 0, 1);
        MathStructTest4Impl_MST4_ASSERT(a.x == 2147483647);
        MathStructTest4Impl_MST4_ASSERT(a.y == -2147483647);
        MathStructTest4Impl_MST4_ASSERT(a.z == 0);
        MathStructTest4Impl_MST4_ASSERT(a.w == 1);
        Vector4Int b(-2147483647, 2147483647, 0, -1);
        Vector4Int c = a + b;
        MathStructTest4Impl_MST4_ASSERT(c.x == 0);
        MathStructTest4Impl_MST4_ASSERT(c.y == 0);
        MathStructTest4Impl_MST4_ASSERT(c.z == 0);
        MathStructTest4Impl_MST4_ASSERT(c.w == 0);
    }

    static void testVector4IntChain()
    {
        Vector4Int a(1, 1, 1, 1);
        a += Vector4Int(2, 2, 2, 2);
        MathStructTest4Impl_MST4_ASSERT(a.x == 3);
        MathStructTest4Impl_MST4_ASSERT(a.y == 3);
        MathStructTest4Impl_MST4_ASSERT(a.z == 3);
        MathStructTest4Impl_MST4_ASSERT(a.w == 3);
        a -= Vector4Int(1, 1, 1, 1);
        MathStructTest4Impl_MST4_ASSERT(a.x == 2);
        MathStructTest4Impl_MST4_ASSERT(a.y == 2);
        MathStructTest4Impl_MST4_ASSERT(a.z == 2);
        MathStructTest4Impl_MST4_ASSERT(a.w == 2);
        Vector4Int b = a * 3;
        MathStructTest4Impl_MST4_ASSERT(b.x == 6);
        MathStructTest4Impl_MST4_ASSERT(b.y == 6);
        MathStructTest4Impl_MST4_ASSERT(b.z == 6);
        MathStructTest4Impl_MST4_ASSERT(b.w == 6);
        // less: x+y+z+w
        Vector4Int lo(1, 1, 1, 1);
        Vector4Int hi(2, 2, 2, 2);
        MathStructTest4Impl_MST4_ASSERT(lo < hi);
        MathStructTest4Impl_MST4_ASSERT(!(hi < lo));
    }

    // ============================================================
    // Mixed tests
    // ============================================================

    static void testMixedIntVectorOps()
    {
        // Store V2I in a Vector container
        Vector<Vector2Int> list;
        list.add(Vector2Int(1, 2));
        list.add(Vector2Int(3, 4));
        list.add(Vector2Int(5, 6));
        MathStructTest4Impl_MST4_ASSERT(list.size() == 3);
        MathStructTest4Impl_MST4_ASSERT(list[0].x == 1);
        MathStructTest4Impl_MST4_ASSERT(list[1].y == 4);
        MathStructTest4Impl_MST4_ASSERT(list[2].x == 5);

        // Store V3I in a vector
        Vector<Vector3Int> v3list;
        v3list.add(Vector3Int(1, 2, 3));
        v3list.add(Vector3Int(4, 5, 6));
        MathStructTest4Impl_MST4_ASSERT(v3list.size() == 2);
        MathStructTest4Impl_MST4_ASSERT(v3list[0].z == 3);
        MathStructTest4Impl_MST4_ASSERT(v3list[1].x == 4);

        // HashMap with V2Int as key
        HashMap<Vector2Int, int> hmap;
        hmap.add(Vector2Int(10, 20), 100);
        hmap.add(Vector2Int(30, 40), 200);
        MathStructTest4Impl_MST4_ASSERT(hmap.contains(Vector2Int(10, 20)));
        MathStructTest4Impl_MST4_ASSERT(hmap.get(Vector2Int(10, 20), 0) == 100);
        MathStructTest4Impl_MST4_ASSERT(hmap.get(Vector2Int(30, 40), 0) == 200);
        MathStructTest4Impl_MST4_ASSERT(!hmap.contains(Vector2Int(0, 0)));
    }

    static void testIntVectorStress()
    {
        // Stress test with many Vector2Int operations
        const int N = 100;
        Vector<Vector2Int> arr;
        for (int i = 0; i < N; ++i)
        {
            arr.add(Vector2Int(i, N - i));
        }
        MathStructTest4Impl_MST4_ASSERT(arr.size() == N);
        for (int i = 0; i < N; ++i)
        {
            MathStructTest4Impl_MST4_ASSERT(arr[i].x == i);
            MathStructTest4Impl_MST4_ASSERT(arr[i].y == N - i);
        }
        // Sum all
        Vector2Int sum;
        for (int i = 0; i < N; ++i)
        {
            sum += arr[i];
        }
        // x sum = 0+1+...+99 = 4950; y sum = 100+99+...+1 = 5050
        MathStructTest4Impl_MST4_ASSERT(sum.x == 4950);
        MathStructTest4Impl_MST4_ASSERT(sum.y == 5050);

        // Vector3Int stress
        Vector<Vector3Int> v3arr;
        for (int i = 0; i < N; ++i)
        {
            v3arr.add(Vector3Int(i, i * 2, i * 3));
        }
        for (int i = 0; i < N; ++i)
        {
            MathStructTest4Impl_MST4_ASSERT(v3arr[i].x == i);
            MathStructTest4Impl_MST4_ASSERT(v3arr[i].y == i * 2);
            MathStructTest4Impl_MST4_ASSERT(v3arr[i].z == i * 3);
        }

        // Vector4Int stress
        Vector<Vector4Int> v4arr;
        for (int i = 0; i < N; ++i)
        {
            v4arr.add(Vector4Int(i, -i, i * 2, -i * 2));
        }
        for (int i = 0; i < N; ++i)
        {
            MathStructTest4Impl_MST4_ASSERT(v4arr[i].x == i);
            MathStructTest4Impl_MST4_ASSERT(v4arr[i].y == -i);
            MathStructTest4Impl_MST4_ASSERT(v4arr[i].z == i * 2);
            MathStructTest4Impl_MST4_ASSERT(v4arr[i].w == -i * 2);
        }
    }
};

struct MathStructTest5Impl
{

#define MathStructTest5Impl_MS5_ASSERT(expr) \
	if (!(expr)) \
	{ \
		LOG("MathStructTest5 FAILED: " #expr); \
	}

    //==============================================================
    // Vector2Short 测试
    //==============================================================

    static void testVector2ShortConstruct()
    {
        Vector2Short v;
        MathStructTest5Impl_MS5_ASSERT(v.x == 0);
        MathStructTest5Impl_MS5_ASSERT(v.y == 0);

        Vector2Short v2((short)3, (short)-5);
        MathStructTest5Impl_MS5_ASSERT(v2.x == 3);
        MathStructTest5Impl_MS5_ASSERT(v2.y == -5);
    }

    static void testVector2ShortArithmetic()
    {
        Vector2Short a((short)10, (short)20);
        Vector2Short b((short)3, (short)7);

        // operator+
        Vector2Short add = a + b;
        MathStructTest5Impl_MS5_ASSERT(add.x == 13);
        MathStructTest5Impl_MS5_ASSERT(add.y == 27);

        // operator-
        Vector2Short sub = a - b;
        MathStructTest5Impl_MS5_ASSERT(sub.x == 7);
        MathStructTest5Impl_MS5_ASSERT(sub.y == 13);

        // operator* scalar
        Vector2Short mul = a * (short)2;
        MathStructTest5Impl_MS5_ASSERT(mul.x == 20);
        MathStructTest5Impl_MS5_ASSERT(mul.y == 40);

        // operator+=
        Vector2Short c((short)5, (short)5);
        c += b;
        MathStructTest5Impl_MS5_ASSERT(c.x == 8);
        MathStructTest5Impl_MS5_ASSERT(c.y == 12);

        // operator-=
        Vector2Short d((short)10, (short)10);
        d -= b;
        MathStructTest5Impl_MS5_ASSERT(d.x == 7);
        MathStructTest5Impl_MS5_ASSERT(d.y == 3);
    }

    static void testVector2ShortCompare()
    {
        Vector2Short a((short)3, (short)4);
        Vector2Short b((short)3, (short)4);
        Vector2Short c((short)5, (short)6);

        MathStructTest5Impl_MS5_ASSERT(a == b);
        MathStructTest5Impl_MS5_ASSERT(a != c);
        MathStructTest5Impl_MS5_ASSERT(a < c);  // 3+4=7 < 5+6=11
        MathStructTest5Impl_MS5_ASSERT(!(c < a));
    }

    static void testVector2ShortClear()
    {
        Vector2Short v((short)100, (short)-200);
        v.clear();
        MathStructTest5Impl_MS5_ASSERT(v.x == 0);
        MathStructTest5Impl_MS5_ASSERT(v.y == 0);
    }

    static void testVector2ShortNegate()
    {
        Vector2Short v((short)5, (short)-3);
        Vector2Short neg = -v;
        MathStructTest5Impl_MS5_ASSERT(neg.x == -5);
        MathStructTest5Impl_MS5_ASSERT(neg.y == 3);
    }

    //==============================================================
    // Vector2UShort 测试
    //==============================================================

    static void testVector2UShortConstruct()
    {
        Vector2UShort v;
        MathStructTest5Impl_MS5_ASSERT(v.x == 0);
        MathStructTest5Impl_MS5_ASSERT(v.y == 0);

        Vector2UShort v2((ushort)100, (ushort)200);
        MathStructTest5Impl_MS5_ASSERT(v2.x == 100);
        MathStructTest5Impl_MS5_ASSERT(v2.y == 200);
    }

    static void testVector2UShortArithmetic()
    {
        Vector2UShort a((ushort)50, (ushort)100);
        Vector2UShort b((ushort)10, (ushort)20);

        Vector2UShort add = a + b;
        MathStructTest5Impl_MS5_ASSERT(add.x == 60);
        MathStructTest5Impl_MS5_ASSERT(add.y == 120);

        Vector2UShort sub = a - b;
        MathStructTest5Impl_MS5_ASSERT(sub.x == 40);
        MathStructTest5Impl_MS5_ASSERT(sub.y == 80);

        Vector2UShort mul = a * (short)2;
        MathStructTest5Impl_MS5_ASSERT(mul.x == 100);
        MathStructTest5Impl_MS5_ASSERT(mul.y == 200);

        Vector2UShort c((ushort)5, (ushort)5);
        c += b;
        MathStructTest5Impl_MS5_ASSERT(c.x == 15);
        MathStructTest5Impl_MS5_ASSERT(c.y == 25);

        Vector2UShort d((ushort)30, (ushort)40);
        d -= b;
        MathStructTest5Impl_MS5_ASSERT(d.x == 20);
        MathStructTest5Impl_MS5_ASSERT(d.y == 20);
    }

    static void testVector2UShortCompare()
    {
        Vector2UShort a((ushort)3, (ushort)4);
        Vector2UShort b((ushort)3, (ushort)4);
        Vector2UShort c((ushort)10, (ushort)10);

        MathStructTest5Impl_MS5_ASSERT(a == b);
        MathStructTest5Impl_MS5_ASSERT(a != c);
        MathStructTest5Impl_MS5_ASSERT(a < c);
        MathStructTest5Impl_MS5_ASSERT(!(c < a));
    }

    static void testVector2UShortClear()
    {
        Vector2UShort v((ushort)500, (ushort)1000);
        v.clear();
        MathStructTest5Impl_MS5_ASSERT(v.x == 0);
        MathStructTest5Impl_MS5_ASSERT(v.y == 0);
    }

    //==============================================================
    // Vector2UInt 测试
    //==============================================================

    static void testVector2UIntConstruct()
    {
        Vector2UInt v;
        MathStructTest5Impl_MS5_ASSERT(v.x == 0u);
        MathStructTest5Impl_MS5_ASSERT(v.y == 0u);

        Vector2UInt v2(1000000u, 2000000u);
        MathStructTest5Impl_MS5_ASSERT(v2.x == 1000000u);
        MathStructTest5Impl_MS5_ASSERT(v2.y == 2000000u);
    }

    static void testVector2UIntArithmetic()
    {
        Vector2UInt a(100u, 200u);
        Vector2UInt b(30u, 50u);

        Vector2UInt add = a + b;
        MathStructTest5Impl_MS5_ASSERT(add.x == 130u);
        MathStructTest5Impl_MS5_ASSERT(add.y == 250u);

        Vector2UInt sub = a - b;
        MathStructTest5Impl_MS5_ASSERT(sub.x == 70u);
        MathStructTest5Impl_MS5_ASSERT(sub.y == 150u);

        Vector2UInt mul = a * (short)3;
        MathStructTest5Impl_MS5_ASSERT(mul.x == 300u);
        MathStructTest5Impl_MS5_ASSERT(mul.y == 600u);

        Vector2UInt c(10u, 10u);
        c += b;
        MathStructTest5Impl_MS5_ASSERT(c.x == 40u);
        MathStructTest5Impl_MS5_ASSERT(c.y == 60u);

        Vector2UInt d(100u, 100u);
        d -= b;
        MathStructTest5Impl_MS5_ASSERT(d.x == 70u);
        MathStructTest5Impl_MS5_ASSERT(d.y == 50u);
    }

    static void testVector2UIntCompare()
    {
        Vector2UInt a(5u, 5u);
        Vector2UInt b(5u, 5u);
        Vector2UInt c(20u, 20u);

        MathStructTest5Impl_MS5_ASSERT(a == b);
        MathStructTest5Impl_MS5_ASSERT(a != c);
        MathStructTest5Impl_MS5_ASSERT(a < c);
    }

    static void testVector2UIntClear()
    {
        Vector2UInt v(9999u, 8888u);
        v.clear();
        MathStructTest5Impl_MS5_ASSERT(v.x == 0u);
        MathStructTest5Impl_MS5_ASSERT(v.y == 0u);
    }

    //==============================================================
    // Quaternion 测试
    //==============================================================

    static void testQuaternionDefault()
    {
        Quaternion q;
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.x) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.y) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.z) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.w - 1.0f) < 1e-4f);
    }

    static void testQuaternionConstruct()
    {
        Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.x - 1.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.y - 2.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.z - 3.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.w - 4.0f) < 1e-4f);

        // 拷贝构造
        Quaternion q2(q);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q2.x - 1.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q2.w - 4.0f) < 1e-4f);
    }

    static void testQuaternionConjugate()
    {
        Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
        Quaternion conj = q.conjugate();
        MathStructTest5Impl_MS5_ASSERT(fabsf(conj.x - (-1.0f)) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(conj.y - (-2.0f)) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(conj.z - (-3.0f)) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(conj.w - 4.0f) < 1e-4f);
    }

    static void testQuaternionDot()
    {
        Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
        Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
        // dot = 1+4+9+16 = 30
        float d = Quaternion::dot(q1, q2);
        MathStructTest5Impl_MS5_ASSERT(fabsf(d - 30.0f) < 1e-4f);

        // dot(q, identity(0,0,0,1)) = w
        Quaternion id;
        float d2 = Quaternion::dot(q1, id);
        MathStructTest5Impl_MS5_ASSERT(fabsf(d2 - (1.0f * 0.0f + 2.0f * 0.0f + 3.0f * 0.0f + 4.0f * 1.0f)) < 1e-4f);
    }

    static void testQuaternionLength()
    {
        Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
        // 单位四元数长度 = 1
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.length() - 1.0f) < 1e-4f);

        Quaternion q2(1.0f, 0.0f, 0.0f, 0.0f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q2.length() - 1.0f) < 1e-4f);
    }

    static void testQuaternionNormalize()
    {
        Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
        Quaternion n = q.normalize();
        float len = n.length();
        MathStructTest5Impl_MS5_ASSERT(fabsf(len - 1.0f) < 1e-4f);
    }

    static void testQuaternionMultiplyScalar()
    {
        Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
        Quaternion r = q * 2.0f;
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.x - 2.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.y - 4.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.z - 6.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.w - 8.0f) < 1e-4f);
    }

    static void testQuaternionDivideScalar()
    {
        Quaternion q(2.0f, 4.0f, 6.0f, 8.0f);
        Quaternion r = q / 2.0f;
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.x - 1.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.y - 2.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.z - 3.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.w - 4.0f) < 1e-4f);
    }

    static void testQuaternionAdd()
    {
        Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
        Quaternion q2(0.5f, 0.5f, 0.5f, 0.5f);
        Quaternion r = q1 + q2;
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.x - 1.5f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.y - 2.5f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.z - 3.5f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(r.w - 4.5f) < 1e-4f);
    }

    static void testQuaternionNegate()
    {
        Quaternion q(1.0f, -2.0f, 3.0f, -4.0f);
        Quaternion neg = -q;
        MathStructTest5Impl_MS5_ASSERT(fabsf(neg.x - (-1.0f)) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(neg.y - 2.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(neg.z - (-3.0f)) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(neg.w - 4.0f) < 1e-4f);
    }

    static void testQuaternionMultiplyAssign()
    {
        Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
        q *= 3.0f;
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.x - 3.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.y - 6.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.z - 9.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.w - 12.0f) < 1e-4f);
    }

    static void testQuaternionDivideAssign()
    {
        Quaternion q(4.0f, 8.0f, 12.0f, 16.0f);
        q /= 4.0f;
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.x - 1.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.y - 2.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.z - 3.0f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.w - 4.0f) < 1e-4f);
    }

    static void testQuaternionAddAssign()
    {
        Quaternion q(1.0f, 1.0f, 1.0f, 1.0f);
        Quaternion add(0.5f, 0.5f, 0.5f, 0.5f);
        q += add;
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.x - 1.5f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.y - 1.5f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.z - 1.5f) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.w - 1.5f) < 1e-4f);
    }

    static void testQuaternionEulerAngles()
    {
        // 单位四元数：欧拉角应接近(0,0,0)
        Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
        Vector3 euler = q.eulerAngles();
        MathStructTest5Impl_MS5_ASSERT(fabsf(euler.x) < 1e-3f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(euler.y) < 1e-3f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(euler.z) < 1e-3f);
    }

    static void testQuaternionLerp()
    {
        Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
        Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
        // lerp(q1, q2, 0.5) = q1 * 0.5 + q2 * 0.5
        Quaternion lr = Quaternion::lerp(q1, q2, 0.5f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(lr.w - 1.0f) < 1e-4f);

        // lerp at 0 = q1, at 1 = q2
        Quaternion l0 = Quaternion::lerp(q1, q2, 0.0f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(l0.w - 1.0f) < 1e-4f);
        Quaternion l1 = Quaternion::lerp(q1, q2, 1.0f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(l1.w - 1.0f) < 1e-4f);
    }

    static void testQuaternionClear()
    {
        Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
        q.clear();
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.x) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.y) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.z) < 1e-4f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.w - 1.0f) < 1e-4f);
    }

    static void testQuaternionFromAxisAngle()
    {
        // 绕 z 轴旋转 90 度
        Vector3 axis(0.0f, 0.0f, 1.0f);
        float angle = 3.14159265f * 0.5f;
        Quaternion q(axis, angle);
        // q = (sin(45)*z, cos(45)) ≈ (0, 0, 0.7071, 0.7071)
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.x) < 1e-3f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.y) < 1e-3f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.z - 0.7071f) < 1e-3f);
        MathStructTest5Impl_MS5_ASSERT(fabsf(q.w - 0.7071f) < 1e-3f);
    }

    static void testQuaternionFromEulerAngles()
    {
        // 绕 z 轴 90 度的欧拉角 (0, 0, PI/2)
        Vector3 euler(0.0f, 0.0f, 3.14159265f * 0.5f);
        Quaternion q = Quaternion::eulerAnglesToQuaterion(euler);
        float len = q.length();
        // 转换后应为单位四元数
        MathStructTest5Impl_MS5_ASSERT(fabsf(len - 1.0f) < 1e-3f);
    }

    static void test()
    {
        // Vector2Short
        testVector2ShortConstruct();
        testVector2ShortArithmetic();
        testVector2ShortCompare();
        testVector2ShortClear();
        testVector2ShortNegate();
        // Vector2UShort
        testVector2UShortConstruct();
        testVector2UShortArithmetic();
        testVector2UShortCompare();
        testVector2UShortClear();
        // Vector2UInt
        testVector2UIntConstruct();
        testVector2UIntArithmetic();
        testVector2UIntCompare();
        testVector2UIntClear();
        // Quaternion
        testQuaternionDefault();
        testQuaternionConstruct();
        testQuaternionConjugate();
        testQuaternionDot();
        testQuaternionLength();
        testQuaternionNormalize();
        testQuaternionMultiplyScalar();
        testQuaternionDivideScalar();
        testQuaternionAdd();
        testQuaternionNegate();
        testQuaternionMultiplyAssign();
        testQuaternionDivideAssign();
        testQuaternionAddAssign();
        testQuaternionEulerAngles();
        testQuaternionLerp();
        testQuaternionClear();
        testQuaternionFromAxisAngle();
        testQuaternionFromEulerAngles();
    }
};

struct MathStructExt2TestImpl
{

#define MathStructExt2TestImpl_MSE2_ASSERT(expr) \
    if (!(expr)) { ERROR("MathStructExt2Test FAILED: " #expr); }

#define MathStructExt2TestImpl_MSE2_NEAR(a, b) (fabsf((float)(a) - (float)(b)) < 1e-4f)

    static void testLine2Construction()
    {
        Line2 line({ 0.0f, 0.0f }, { 1.0f, 1.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mStart.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mStart.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mEnd.x, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mEnd.y, 1.0f));
        Line2 line2({ -3.0f, 5.0f }, { 7.0f, -2.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line2.mStart.x, -3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line2.mStart.y, 5.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line2.mEnd.x, 7.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line2.mEnd.y, -2.0f));
    }

    static void testLine2Members()
    {
        Line2 line({ 1.5f, 2.5f }, { 3.5f, 4.5f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mStart.x, 1.5f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mStart.y, 2.5f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mEnd.x, 3.5f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mEnd.y, 4.5f));
        Vector2 dir = line.mEnd - line.mStart;
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(dir.x, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(dir.y, 2.0f));
    }

    static void testLine2Multiple()
    {
        Line2 horizontal({ 0.0f, 3.0f }, { 10.0f, 3.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(horizontal.mStart.y, horizontal.mEnd.y));
        Line2 vertical({ 5.0f, 0.0f }, { 5.0f, 10.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(vertical.mStart.x, vertical.mEnd.x));
        Line2 diagonal({ 0.0f, 0.0f }, { 5.0f, 5.0f });
        Vector2 diff = diagonal.mEnd - diagonal.mStart;
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diff.x, diff.y));
    }

    static void testLine2ZeroLength()
    {
        Line2 zero({ 3.0f, 4.0f }, { 3.0f, 4.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(zero.mStart.x, zero.mEnd.x));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(zero.mStart.y, zero.mEnd.y));
        Vector2 diff = zero.mEnd - zero.mStart;
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diff.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diff.y, 0.0f));
    }

    static void testTriangle2Construction()
    {
        Triangle2 tri({ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, 1.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint0.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint0.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint1.x, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint1.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint2.x, 0.5f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint2.y, 1.0f));
        Triangle2 tri2({ -1.0f, -1.0f }, { 1.0f, -1.0f }, { 0.0f, 1.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri2.mPoint0.x, -1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri2.mPoint2.y, 1.0f));
    }

    static void testTriangle2Members()
    {
        Triangle2 tri({ 0.0f, 0.0f }, { 4.0f, 0.0f }, { 2.0f, 3.0f });
        Vector2 centroid;
        centroid.x = (tri.mPoint0.x + tri.mPoint1.x + tri.mPoint2.x) / 3.0f;
        centroid.y = (tri.mPoint0.y + tri.mPoint1.y + tri.mPoint2.y) / 3.0f;
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(centroid.x, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(centroid.y, 1.0f));
        Vector2 side1 = tri.mPoint1 - tri.mPoint0;
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(side1.x, 4.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(side1.y, 0.0f));
    }

    static void testTriangle2DefaultInit()
    {
        Triangle2 tri;
        tri.mPoint0 = { 1.0f, 2.0f };
        tri.mPoint1 = { 3.0f, 4.0f };
        tri.mPoint2 = { 5.0f, 6.0f };
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint0.x, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint1.x, 3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint2.x, 5.0f));
    }

    static void testTriangle3Construction()
    {
        Triangle3 tri({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.5f, 1.0f, 0.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint0.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint0.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint0.z, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint1.x, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint2.y, 1.0f));
        Triangle3 tri2({ -1.0f, 0.0f, 2.0f }, { 3.0f, -1.0f, 2.0f }, { 1.0f, 2.0f, 0.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri2.mPoint0.x, -1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri2.mPoint0.z, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri2.mPoint2.y, 2.0f));
    }

    static void testTriangle3Members()
    {
        Triangle3 tri({ 0.0f, 0.0f, 0.0f }, { 4.0f, 0.0f, 0.0f }, { 2.0f, 3.0f, 0.0f });
        Vector3 centroid;
        centroid.x = (tri.mPoint0.x + tri.mPoint1.x + tri.mPoint2.x) / 3.0f;
        centroid.y = (tri.mPoint0.y + tri.mPoint1.y + tri.mPoint2.y) / 3.0f;
        centroid.z = (tri.mPoint0.z + tri.mPoint1.z + tri.mPoint2.z) / 3.0f;
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(centroid.x, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(centroid.y, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(centroid.z, 0.0f));
        Vector3 edge1 = tri.mPoint1 - tri.mPoint0;
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(edge1.x, 4.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(edge1.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(edge1.z, 0.0f));
    }

    static void testTriangle3DefaultInit()
    {
        Triangle3 tri;
        tri.mPoint0 = { 1.0f, 2.0f, 3.0f };
        tri.mPoint1 = { 4.0f, 5.0f, 6.0f };
        tri.mPoint2 = { 7.0f, 8.0f, 9.0f };
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint0.z, 3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint1.z, 6.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(tri.mPoint2.z, 9.0f));
    }

    static void testRectConstruction()
    {
        Rect r(0.0f, 0.0f, 10.0f, 5.0f);
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.width, 10.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.height, 5.0f));
        Rect r2(-3.0f, -2.0f, 6.0f, 4.0f);
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r2.x, -3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r2.y, -2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r2.width, 6.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r2.height, 4.0f));
    }

    static void testRectVector2Constructor()
    {
        Rect r({ 2.0f, 3.0f }, { 8.0f, 6.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.x, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.y, 3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.width, 8.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.height, 6.0f));
        Rect r2({ 0.0f, 0.0f }, { 1.0f, 1.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r2.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r2.width, 1.0f));
    }

    static void testRectGetSize()
    {
        Rect r(1.0f, 2.0f, 5.0f, 3.0f);
        Vector2 sz = r.getSize();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz.x, 5.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz.y, 3.0f));
        Rect r2(0.0f, 0.0f, 10.0f, 10.0f);
        Vector2 sz2 = r2.getSize();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz2.x, 10.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz2.y, 10.0f));
        Rect r3(5.0f, 5.0f, 0.0f, 0.0f);
        Vector2 sz3 = r3.getSize();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz3.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz3.y, 0.0f));
    }

    static void testRectGetMin()
    {
        Rect r(2.0f, 3.0f, 5.0f, 4.0f);
        Vector2 mn = r.getMin();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mn.x, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mn.y, 3.0f));
        Rect r2(-10.0f, -5.0f, 20.0f, 10.0f);
        Vector2 mn2 = r2.getMin();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mn2.x, -10.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mn2.y, -5.0f));
    }

    static void testRectGetMax()
    {
        Rect r(2.0f, 3.0f, 5.0f, 4.0f);
        Vector2 mx = r.getMax();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mx.x, 7.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mx.y, 7.0f));
        Rect r2(-10.0f, -5.0f, 20.0f, 10.0f);
        Vector2 mx2 = r2.getMax();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mx2.x, 10.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mx2.y, 5.0f));
        Rect r3(0.0f, 0.0f, 1.0f, 1.0f);
        Vector2 mx3 = r3.getMax();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mx3.x, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(mx3.y, 1.0f));
    }

    static void testRectGetCenter()
    {
        Rect r(0.0f, 0.0f, 4.0f, 6.0f);
        Vector2 c = r.getCenter();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c.x, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c.y, 3.0f));
        Rect r2(2.0f, 4.0f, 6.0f, 8.0f);
        Vector2 c2 = r2.getCenter();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c2.x, 5.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c2.y, 8.0f));
        Rect r3(-4.0f, -6.0f, 8.0f, 12.0f);
        Vector2 c3 = r3.getCenter();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c3.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c3.y, 0.0f));
    }

    static void testRectGetDiagonal()
    {
        Rect r(0.0f, 0.0f, 3.0f, 4.0f);
        float diag = r.getDiagonal();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diag, 5.0f));
        Rect r2(0.0f, 0.0f, 1.0f, 0.0f);
        float diag2 = r2.getDiagonal();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diag2, 1.0f));
        Rect r3(0.0f, 0.0f, 0.0f, 0.0f);
        float diag3 = r3.getDiagonal();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diag3, 0.0f));
        Rect r4(0.0f, 0.0f, 5.0f, 5.0f);
        float diag4 = r4.getDiagonal();
        MathStructExt2TestImpl_MSE2_ASSERT(diag4 > 7.0f && diag4 < 7.1f);
    }

    static void testRectEdgeCases()
    {
        Rect rUnit(0.0f, 0.0f, 1.0f, 1.0f);
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(rUnit.getDiagonal(), sqrtf(2.0f)));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(rUnit.getCenter().x, 0.5f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(rUnit.getCenter().y, 0.5f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(rUnit.getMax().x, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(rUnit.getMax().y, 1.0f));
        Rect rNeg(-5.0f, -3.0f, 10.0f, 6.0f);
        Vector2 center = rNeg.getCenter();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(center.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(center.y, 0.0f));
    }

    static void testRect3Construction()
    {
        Rect3 r({ 0.0f, 0.0f, 0.0f }, { 10.0f, 5.0f, 8.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.mMin.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.mMin.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.mMin.z, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.mMax.x, 10.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.mMax.y, 5.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r.mMax.z, 8.0f));
        Rect3 r2({ -5.0f, -3.0f, -2.0f }, { 5.0f, 3.0f, 2.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r2.mMin.x, -5.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(r2.mMax.x, 5.0f));
    }

    static void testRect3GetSize()
    {
        Rect3 r({ 0.0f, 0.0f, 0.0f }, { 4.0f, 3.0f, 2.0f });
        Vector3 sz = r.getSize();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz.x, 4.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz.y, 3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz.z, 2.0f));
        Rect3 r2({ 1.0f, 1.0f, 1.0f }, { 4.0f, 6.0f, 11.0f });
        Vector3 sz2 = r2.getSize();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz2.x, 3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz2.y, 5.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz2.z, 10.0f));
    }

    static void testRect3GetCenter()
    {
        Rect3 r({ 0.0f, 0.0f, 0.0f }, { 4.0f, 6.0f, 8.0f });
        Vector3 c = r.getCenter();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c.x, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c.y, 3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c.z, 4.0f));
        Rect3 r2({ -2.0f, -3.0f, -4.0f }, { 2.0f, 3.0f, 4.0f });
        Vector3 c2 = r2.getCenter();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c2.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c2.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c2.z, 0.0f));
    }

    static void testRect3GetDiagonalIgnoreY()
    {
        Rect3 r({ 0.0f, 0.0f, 0.0f }, { 3.0f, 100.0f, 4.0f });
        float diag = r.getDiagonalIgnoreY();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diag, 5.0f));
        Rect3 r2({ 0.0f, 0.0f, 0.0f }, { 0.0f, 50.0f, 5.0f });
        float diag2 = r2.getDiagonalIgnoreY();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diag2, 5.0f));
        Rect3 r3({ 0.0f, 0.0f, 0.0f }, { 3.0f, 0.0f, 0.0f });
        float diag3 = r3.getDiagonalIgnoreY();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diag3, 3.0f));
    }

    static void testRect3EdgeCases()
    {
        Rect3 rZero({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
        Vector3 sz = rZero.getSize();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(sz.z, 0.0f));
        Vector3 c = rZero.getCenter();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(c.z, 0.0f));
        float diag = rZero.getDiagonalIgnoreY();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diag, 0.0f));
    }

    static void testLine3Construction()
    {
        Line3 line({ 0.0f, 0.0f, 0.0f }, { 1.0f, 2.0f, 3.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mStart.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mStart.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mStart.z, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mEnd.x, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mEnd.y, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line.mEnd.z, 3.0f));
        Line3 line2({ -1.0f, 2.0f, -3.0f }, { 4.0f, -5.0f, 6.0f });
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line2.mStart.x, -1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(line2.mEnd.z, 6.0f));
    }

    static void testLine3Members()
    {
        Line3 line({ 1.0f, 2.0f, 3.0f }, { 4.0f, 6.0f, 9.0f });
        Vector3 dir = line.mEnd - line.mStart;
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(dir.x, 3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(dir.y, 4.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(dir.z, 6.0f));
    }

    static void testLine3ZeroLength()
    {
        Line3 zero({ 5.0f, 6.0f, 7.0f }, { 5.0f, 6.0f, 7.0f });
        Vector3 diff = zero.mEnd - zero.mStart;
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diff.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diff.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(diff.z, 0.0f));
    }

    static void testLine3ToLine2IgnoreY()
    {
        Line3 line({ 1.0f, 100.0f, 2.0f }, { 3.0f, 200.0f, 4.0f });
        Line2D l2 = line.toLine2IgnoreY();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2.mStart.x, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2.mStart.y, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2.mEnd.x, 3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2.mEnd.y, 4.0f));
        Line3 line2({ 0.0f, 50.0f, 0.0f }, { 5.0f, 100.0f, 5.0f });
        Line2D l2b = line2.toLine2IgnoreY();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2b.mStart.x, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2b.mStart.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2b.mEnd.x, 5.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2b.mEnd.y, 5.0f));
    }

    static void testLine3ToLine2IgnoreX()
    {
        Line3 line({ 100.0f, 1.0f, 2.0f }, { 200.0f, 3.0f, 4.0f });
        Line2D l2 = line.toLine2IgnoreX();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2.mStart.x, 2.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2.mStart.y, 1.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2.mEnd.x, 4.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2.mEnd.y, 3.0f));
        Line3 line2({ 99.0f, 0.0f, 5.0f }, { 88.0f, 7.0f, 3.0f });
        Line2D l2b = line2.toLine2IgnoreX();
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2b.mStart.x, 5.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2b.mStart.y, 0.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2b.mEnd.x, 3.0f));
        MathStructExt2TestImpl_MSE2_ASSERT(MathStructExt2TestImpl_MSE2_NEAR(l2b.mEnd.y, 7.0f));
    }

    static void test()
    {
        testLine2Construction();
        testLine2Members();
        testLine2Multiple();
        testLine2ZeroLength();
        testTriangle2Construction();
        testTriangle2Members();
        testTriangle2DefaultInit();
        testTriangle3Construction();
        testTriangle3Members();
        testTriangle3DefaultInit();
        testRectConstruction();
        testRectVector2Constructor();
        testRectGetSize();
        testRectGetMin();
        testRectGetMax();
        testRectGetCenter();
        testRectGetDiagonal();
        testRectEdgeCases();
        testRect3Construction();
        testRect3GetSize();
        testRect3GetCenter();
        testRect3GetDiagonalIgnoreY();
        testRect3EdgeCases();
        testLine3Construction();
        testLine3Members();
        testLine3ZeroLength();
        testLine3ToLine2IgnoreY();
        testLine3ToLine2IgnoreX();
    }
};

void MathStructTest::test()
{
    MathStructTestImpl::test();
    MathStructTest2Impl::test();
    MathStructTest3Impl::test();
    MathStructTest4Impl::test();
    MathStructTest5Impl::test();
    MathStructExt2TestImpl::test();
}
