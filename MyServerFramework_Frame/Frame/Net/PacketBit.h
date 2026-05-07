#pragma once

#include "Packet.h"
#include "SerializerBitRead.h"
#include "SerializerBitWrite.h"

class MICRO_LEGEND_FRAME_API PacketBit : public Packet
{
	BASE(PacketBit, Packet);
protected:
	bool mHasSign = false;				// 当前消息包中是否有负数
	bool mHasGenerateSign = false;		// 是否已经计算过了mHasSign
public:
	virtual bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) = 0;
	virtual bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const = 0;
	// 计算当前这个消息包的数据中是否包含负数,如果没有负数,则可以将所有有符号整数的符号全部省略
	bool hasSign()
	{
		if (!mHasGenerateSign)
		{
			mHasGenerateSign = true;
			mHasSign = generateHasSignInternal();
		}
		return mHasSign;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mHasSign = false;
		mHasGenerateSign = false;
	}
	virtual bool generateHasSignInternal() const { return true; }
};