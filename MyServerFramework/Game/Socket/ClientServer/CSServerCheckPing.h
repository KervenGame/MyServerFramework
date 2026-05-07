#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 客户端回复主动发送延迟检测消息,需要排在前面
class CSServerCheckPing : public PacketTCP
{
	BASE(CSServerCheckPing, PacketTCP);
public:
	int mIndex = 0;
	static CSServerCheckPing mStaticObject;
	static string mPacketName;
public:
	CSServerCheckPing()
	{
		mType = PACKET_TYPE::CSServerCheckPing;
		mShowInfo = false;
	}
	static CSServerCheckPing& get()
	{
		mStaticObject.resetProperty();
		return mStaticObject;
	}
	static const string& getStaticPacketName() { return mPacketName; }
	const string& getPacketName() override { return mPacketName; }
	bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) override
	{
		bool success = true;
		success = success && reader->readSigned(needReadSign, mIndex);
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const override
	{
		bool success = true;
		success = success && writer->writeSigned(needWriteSign, mIndex);
		return success;
	}
	bool generateHasSignInternal() const override
	{
		if (mIndex < 0)
		{
			return true;
		}
		return false;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mIndex = 0;
	}
	void execute() override;
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
};