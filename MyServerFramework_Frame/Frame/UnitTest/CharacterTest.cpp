#include "FrameHeader.h"

#define CH_ASSERT(expr, msg) \
	if (!(expr)) { ERROR(string("CharacterExtTest FAIL: ") + (msg)); }

// =====================================================================
// Character GUID
// =====================================================================

static void testChGuidDefault()
{
	Character ch;
	CH_ASSERT(ch.getGUID() == 0, "GUID default 0");
}

static void testChGuidPositive()
{
	Character ch;
	ch.setGUID(12345);
	CH_ASSERT(ch.getGUID() == 12345, "GUID positive");
}

static void testChGuidNegative()
{
	Character ch;
	ch.setGUID(-1);
	CH_ASSERT(ch.getGUID() == -1, "GUID negative");
}

static void testChGuidLarge()
{
	Character ch;
	ch.setGUID(9223372036854775807LL); // LLONG_MAX safe
	CH_ASSERT(ch.getGUID() == 9223372036854775807LL, "GUID large positive");
}

static void testChGuidZero()
{
	Character ch;
	ch.setGUID(999);
	ch.setGUID(0);
	CH_ASSERT(ch.getGUID() == 0, "GUID reset zero");
}

static void testChGuidSequential()
{
	Character ch;
	for (int i = 0; i < 20; ++i)
	{
		ch.setGUID(i * 10000LL);
		CH_ASSERT(ch.getGUID() == i * 10000LL, "GUID sequential");
	}
}

static void testChGuidMultipleCharacters()
{
	Character ch1, ch2, ch3;
	ch1.setGUID(1);
	ch2.setGUID(2);
	ch3.setGUID(3);
	CH_ASSERT(ch1.getGUID() == 1, "GUID multiple[0]");
	CH_ASSERT(ch2.getGUID() == 2, "GUID multiple[1]");
	CH_ASSERT(ch3.getGUID() == 3, "GUID multiple[2]");
	// modify
	ch2.setGUID(100);
	CH_ASSERT(ch2.getGUID() == 100, "GUID multiple modify");
	CH_ASSERT(ch1.getGUID() == 1, "GUID multiple unchanged[0]");
	CH_ASSERT(ch3.getGUID() == 3, "GUID multiple unchanged[2]");
}

// =====================================================================
// Character Type
// =====================================================================

static void testChTypeDefault()
{
	Character ch;
	CH_ASSERT(ch.getType() == 0, "type default 0");
}

static void testChTypePositive()
{
	Character ch;
	ch.setType(5);
	CH_ASSERT(ch.getType() == 5, "type positive");
}

static void testChTypeMax()
{
	Character ch;
	ch.setType(255);
	CH_ASSERT(ch.getType() == 255, "type max byte");
}

static void testChTypeZero()
{
	Character ch;
	ch.setType(127);
	ch.setType(0);
	CH_ASSERT(ch.getType() == 0, "type reset zero");
}

static void testChTypeSequential()
{
	Character ch;
	for (int i = 0; i <= 10; ++i)
	{
		ch.setType((byte)i);
		CH_ASSERT(ch.getType() == (byte)i, "type sequential");
	}
}

// =====================================================================
// Character GUID + Type 组合
// =====================================================================

static void testChGUIDAndType()
{
	Character ch;
	ch.setGUID(1000);
	ch.setType(3);
	CH_ASSERT(ch.getGUID() == 1000 && ch.getType() == 3, "GUID+type combined");
}

static void testChGUIDAndTypeSwapOrder()
{
	Character ch;
	ch.setType(7);
	ch.setGUID(5000);
	CH_ASSERT(ch.getType() == 7 && ch.getGUID() == 5000, "GUID+type swap order");
}

// =====================================================================
// Character ResetProperty
// =====================================================================

static void testChResetPropertyGuid()
{
	Character ch;
	ch.setGUID(8888);
	ch.resetProperty();
	CH_ASSERT(ch.getGUID() == 0, "resetProperty GUID zero");
}

static void testChResetPropertyType()
{
	Character ch;
	ch.setType(10);
	ch.resetProperty();
	CH_ASSERT(ch.getType() == 0, "resetProperty type zero");
}

static void testChResetPropertyBoth()
{
	Character ch;
	ch.setGUID(7777);
	ch.setType(20);
	ch.resetProperty();
	CH_ASSERT(ch.getGUID() == 0 && ch.getType() == 0, "resetProperty both zero");
}

static void testChResetPropertyMultiple()
{
	Character ch;
	for (int i = 0; i < 5; ++i)
	{
		ch.setGUID(i * 100);
		ch.setType((byte)(i * 10));
		ch.resetProperty();
		CH_ASSERT(ch.getGUID() == 0 && ch.getType() == 0, "resetProperty cycle");
	}
}

static void testChResetThenSet()
{
	Character ch;
	ch.setGUID(100);
	ch.setType(5);
	ch.resetProperty();
	ch.setGUID(200);
	ch.setType(6);
	CH_ASSERT(ch.getGUID() == 200, "reset then set GUID");
	CH_ASSERT(ch.getType() == 6, "reset then set type");
}

// =====================================================================
// Character 继承链
// =====================================================================

static void testChInheritsMovableObjectPosition()
{
	Character ch;
	ch.setPosition(Vector3(1.0f, 2.0f, 3.0f));
	const Vector3& pos = ch.getPosition();
	CH_ASSERT(pos.x == 1.0f && pos.y == 2.0f && pos.z == 3.0f, "inherits position");
}

static void testChInheritsCommandReceiverName()
{
	Character ch;
	ch.setName("test_char");
	CH_ASSERT(ch.getUTF8Name() == "test_char", "inherits name");
}

// =====================================================================
// 主入口
// =====================================================================
void CharacterTest::test()
{
	testChGuidDefault();
	testChGuidPositive();
	testChGuidNegative();
	testChGuidLarge();
	testChGuidZero();
	testChGuidSequential();
	testChGuidMultipleCharacters();

	testChTypeDefault();
	testChTypePositive();
	testChTypeMax();
	testChTypeZero();
	testChTypeSequential();

	testChGUIDAndType();
	testChGUIDAndTypeSwapOrder();

	testChResetPropertyGuid();
	testChResetPropertyType();
	testChResetPropertyBoth();
	testChResetPropertyMultiple();
	testChResetThenSet();

	testChInheritsMovableObjectPosition();
	testChInheritsCommandReceiverName();
}
