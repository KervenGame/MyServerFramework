#include "FrameHeader.h"

#define TUTE_ASSERT(expr) \
	if (!(expr)) { ERROR(string("TypeUtilityExtTest FAILED: ") + (#expr)); }

// Helper base and derived class for isSubClass tests
struct TuteBase {};
struct TuteDerived : public TuteBase {};
struct TuteUnrelated {};

// ========== isType ==========

static void testTUTE_IsTypeBasic()
{
	TUTE_ASSERT((isType<int, int>()));
	TUTE_ASSERT((isType<float, float>()));
	TUTE_ASSERT((isType<bool, bool>()));
	TUTE_ASSERT((isType<double, double>()));
	TUTE_ASSERT((isType<llong, llong>()));
}

static void testTUTE_IsTypeWithConst()
{
	TUTE_ASSERT((isType<const int, int>()));
	TUTE_ASSERT((isType<const float, float>()));
	TUTE_ASSERT((isType<const bool, bool>()));
}

static void testTUTE_IsTypeWithRef()
{
	TUTE_ASSERT((isType<int&, int>()));
	TUTE_ASSERT((isType<const int&, int>()));
	TUTE_ASSERT((isType<float&, float>()));
}

static void testTUTE_IsTypeDifferent()
{
	TUTE_ASSERT(!(isType<int, float>()));
	TUTE_ASSERT(!(isType<int, llong>()));
	TUTE_ASSERT(!(isType<float, double>()));
	TUTE_ASSERT(!(isType<bool, int>()));
}

// ========== isSubClass ==========

static void testTUTE_IsSubClassTrue()
{
	TUTE_ASSERT((isSubClass<TuteBase, TuteDerived>()));
}

static void testTUTE_IsSubClassFalse()
{
	TUTE_ASSERT(!(isSubClass<TuteBase, TuteUnrelated>()));
	TUTE_ASSERT(!(isSubClass<TuteDerived, TuteBase>()));
	TUTE_ASSERT(!(isSubClass<TuteUnrelated, TuteDerived>()));
}

// ========== isPODType ==========

static void testTUTE_IsPODTypeTrue()
{
	TUTE_ASSERT((isPODType<int>()));
	TUTE_ASSERT((isPODType<float>()));
	TUTE_ASSERT((isPODType<double>()));
	TUTE_ASSERT((isPODType<bool>()));
	TUTE_ASSERT((isPODType<char>()));
	TUTE_ASSERT((isPODType<byte>()));
	TUTE_ASSERT((isPODType<short>()));
	TUTE_ASSERT((isPODType<ushort>()));
	TUTE_ASSERT((isPODType<uint>()));
	TUTE_ASSERT((isPODType<llong>()));
}

static void testTUTE_IsPODTypeFalse()
{
	TUTE_ASSERT(!(isPODType<string>()));
	TUTE_ASSERT(!(isPODType<TuteBase>()));
	TUTE_ASSERT(!(isPODType<TuteDerived>()));
}

// ========== isSignedInteger ==========

static void testTUTE_IsSignedIntegerTrue()
{
	TUTE_ASSERT((isSignedInteger<int>()));
	TUTE_ASSERT((isSignedInteger<llong>()));
	TUTE_ASSERT((isSignedInteger<short>()));
	TUTE_ASSERT((isSignedInteger<char>()));
}

static void testTUTE_IsSignedIntegerFalse()
{
	TUTE_ASSERT(!(isSignedInteger<uint>()));
	TUTE_ASSERT(!(isSignedInteger<ushort>()));
	TUTE_ASSERT(!(isSignedInteger<byte>()));
	TUTE_ASSERT(!(isSignedInteger<float>()));
	TUTE_ASSERT(!(isSignedInteger<double>()));
}

// ========== isUnsignedInteger ==========

static void testTUTE_IsUnsignedIntegerTrue()
{
	TUTE_ASSERT((isUnsignedInteger<uint>()));
	TUTE_ASSERT((isUnsignedInteger<ushort>()));
	TUTE_ASSERT((isUnsignedInteger<byte>()));
}

static void testTUTE_IsUnsignedIntegerFalse()
{
	TUTE_ASSERT(!(isUnsignedInteger<int>()));
	TUTE_ASSERT(!(isUnsignedInteger<llong>()));
	TUTE_ASSERT(!(isUnsignedInteger<short>()));
	TUTE_ASSERT(!(isUnsignedInteger<float>()));
	TUTE_ASSERT(!(isUnsignedInteger<double>()));
}

// ========== mixed primitive types ==========

static void testTUTE_IsTypeMixedPrimitive()
{
	TUTE_ASSERT((isType<uint, uint>()));
	TUTE_ASSERT((isType<ushort, ushort>()));
	TUTE_ASSERT((isType<byte, byte>()));
	TUTE_ASSERT(!(isType<uint, int>()));
	TUTE_ASSERT(!(isType<ushort, short>()));
	TUTE_ASSERT(!(isType<byte, char>()));
}

void TypeUtilityExtTest::test()
{
	testTUTE_IsTypeBasic();
	testTUTE_IsTypeWithConst();
	testTUTE_IsTypeWithRef();
	testTUTE_IsTypeDifferent();
	testTUTE_IsSubClassTrue();
	testTUTE_IsSubClassFalse();
	testTUTE_IsPODTypeTrue();
	testTUTE_IsPODTypeFalse();
	testTUTE_IsSignedIntegerTrue();
	testTUTE_IsSignedIntegerFalse();
	testTUTE_IsUnsignedIntegerTrue();
	testTUTE_IsUnsignedIntegerFalse();
	testTUTE_IsTypeMixedPrimitive();
}
