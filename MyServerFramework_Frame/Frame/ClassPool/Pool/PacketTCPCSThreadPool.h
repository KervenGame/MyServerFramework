#pragma once

#include "ClassTypePoolThread.h"
#include "PacketTCPFactoryManager.h"

// 只用于在子线程中创建CSPacket,也就是TCPClient会用到,目的是为了分开不同的对象池
class PacketTCPCSThreadPool : public ClassTypePoolThread<PacketTCP, ushort>
{
protected:
	PacketTCP* create(const ushort type) override
	{
		auto* factory = mPacketTCPFactoryManager->getFactory(type);
		return factory != nullptr ? factory->create() : nullptr;
	}
};