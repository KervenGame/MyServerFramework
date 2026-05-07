#pragma once

#include "SerializableBitData.h"

// 通用的物品信息结构体
class NetStructItemInfo : public SerializableBitData
{
	BASE(NetStructItemInfo, SerializableBitData);
public:
	int mItemID = 0;
	int mItemCount = 0;
public:
	NetStructItemInfo() = default;
	NetStructItemInfo(int itemID, int itemCount);
	NetStructItemInfo& operator=(const NetStructItemInfo& other);
	bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) override;
	bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const override;
	bool hasSign() const override
	{
		if (mItemID < 0)
		{
			return true;
		}
		if (mItemCount < 0)
		{
			return true;
		}
		return false;
	}
	void resetProperty() override;
};