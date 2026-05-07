#pragma once

// 如果Value是非基础数据类型,则尽量设置为指针,否则在同步时可能无法获取正确的结果
template<typename Key, typename Value>
struct MapModify
{
	Key mKey;
	Value mValue;
	bool mAdd;
	// erase 操作不需要 mValue，用默认构造避免 (Value)(0) 对非基础类型（如 string）崩溃
	explicit MapModify(const Key& key):
		mKey(key),
		mValue(),
		mAdd(false)
	{}
	MapModify(const Key& key, const Value& value):
		mKey(key),
		mValue(value),
		mAdd(true)
	{}
};