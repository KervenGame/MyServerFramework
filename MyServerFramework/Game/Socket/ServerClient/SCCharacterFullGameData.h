#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 角色在Game层的完整数据
class SCCharacterFullGameData : public PacketTCP
{
	BASE(SCCharacterFullGameData, PacketTCP);
public:
	enum class Field : byte
	{
		HP = 0,
		MaxHP = 1,
		Name = 2,
	};
public:
	int mHP = 0;
	int mMaxHP = 0;
	string mName;
private:
	static SCCharacterFullGameData mStaticObject;
	static string mPacketName;
public:
	SCCharacterFullGameData()
	{
		mType = PACKET_TYPE::SCCharacterFullGameData;
		mShowInfo = false;
	}
	static SCCharacterFullGameData& get()
	{
		mStaticObject.resetProperty();
		return mStaticObject;
	}
	const string& getPacketName() override { return mPacketName; }
	static const string& getStaticPacketName() { return mPacketName; }
	static constexpr ushort getStaticType() { return PACKET_TYPE::SCCharacterFullGameData; }
	static constexpr bool hasMember() { return true; }
	bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) override
	{
		bool success = true;
		if (isFieldValid(Field::HP))
		{
			success = success && reader->readSigned(needReadSign, mHP);
		}
		if (isFieldValid(Field::MaxHP))
		{
			success = success && reader->readSigned(needReadSign, mMaxHP);
		}
		if (isFieldValid(Field::Name))
		{
			success = success && reader->readString(mName);
		}
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const override
	{
		bool success = true;
		if (isFieldValid(Field::HP))
		{
			success = success && writer->writeSigned(needWriteSign, mHP);
		}
		if (isFieldValid(Field::MaxHP))
		{
			success = success && writer->writeSigned(needWriteSign, mMaxHP);
		}
		if (isFieldValid(Field::Name))
		{
			success = success && writer->writeString(mName);
		}
		return success;
	}
	bool generateHasSignInternal() const override
	{
		if ((mFieldFlag & (1ULL << (byte)Field::HP)) != 0 && mHP < 0)
		{
			return true;
		}
		if ((mFieldFlag & (1ULL << (byte)Field::MaxHP)) != 0 && mMaxHP < 0)
		{
			return true;
		}
		return false;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mHP = 0;
		mMaxHP = 0;
		mName.clear();
	}
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
	static void send(CharacterPlayer* player);
};