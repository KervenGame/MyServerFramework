#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 请求服务器检查网络消息版本号是否正确,此消息应该排第一个
class CSCheckPacketVersion : public PacketTCP
{
	BASE(CSCheckPacketVersion, PacketTCP);
public:
	string mPacketVersion;
	static CSCheckPacketVersion mStaticObject;
	static string mPacketName;
public:
	CSCheckPacketVersion()
	{
		mType = PACKET_TYPE::CSCheckPacketVersion;
		mShowInfo = true;
	}
	static CSCheckPacketVersion& get()
	{
		mStaticObject.resetProperty();
		return mStaticObject;
	}
	static const string& getStaticPacketName() { return mPacketName; }
	const string& getPacketName() override { return mPacketName; }
	bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) override
	{
		bool success = true;
		success = success && reader->readString(mPacketVersion);
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const override
	{
		bool success = true;
		success = success && writer->writeString(mPacketVersion);
		return success;
	}
	bool generateHasSignInternal() const override
	{
		return false;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mPacketVersion.clear();
	}
	void execute() override;
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
};