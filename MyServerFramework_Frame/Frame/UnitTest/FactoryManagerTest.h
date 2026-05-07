#pragma once

#include "FrameMacro.h"

// FactoryManager模板工厂单元测试
// 覆盖: 注册/创建/查询/计数/清理/SFINAE类型约束/重复注册/不存在key
class MICRO_LEGEND_FRAME_API FactoryManagerTest
{
public:
	static void test();
private:
	// --- 基础生命周期 ---
	static void testCreateSingleFactory();
	static void testCreateMultipleFactories();
	static void testGetFactoryNonExistent();
	static void testGetFactoryCount();
	static void testQuitClearsFactories();

	// --- 创建正确性 ---
	static void testCreateReturnsCorrectType();
	static void testCreateEachFactoryDistinct();

	// --- 边界情况 ---
	static void testOverwriteSameKey();
	static void testZeroKey();

	// --- isFactory 标志 ---
	static void testIsFactoryFlag();
};
