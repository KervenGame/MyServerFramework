#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 通知服务器攻击
class CSAttack : public PacketTCP
{
	BASE(CSAttack, PacketTCP);
public:
	Vector<llong> mTargetGUID;
	int mSkillID = 0;
	llong mTimeStamp = 0;
	static CSAttack mStaticObject;
	static string mPacketName;
public:
	CSAttack()
	{
		mType = PACKET_TYPE::CSAttack;
		mShowInfo = false;
	}
	static CSAttack& get()
	{
		mStaticObject.resetProperty();
		return mStaticObject;
	}
	static const string& getStaticPacketName() { return mPacketName; }
	const string& getPacketName() override { return mPacketName; }
	bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) override
	{
		bool success = true;
		success = success && reader->readSignedList(needReadSign, mTargetGUID);
		success = success && reader->readSigned(needReadSign, mSkillID);
		success = success && reader->readSigned(needReadSign, mTimeStamp);
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const override
	{
		bool success = true;
		success = success && writer->writeSignedList(needWriteSign, mTargetGUID);
		success = success && writer->writeSigned(needWriteSign, mSkillID);
		success = success && writer->writeSigned(needWriteSign, mTimeStamp);
		return success;
	}
	bool generateHasSignInternal() const override
	{
		for (const llong item : mTargetGUID)
		{
			if (item < 0)
			{
				return true;
			}
		}
		if (mSkillID < 0)
		{
			return true;
		}
		if (mTimeStamp < 0)
		{
			return true;
		}
		return false;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mTargetGUID.clear();
		mSkillID = 0;
		mTimeStamp = 0;
	}
	void execute() override;
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
};