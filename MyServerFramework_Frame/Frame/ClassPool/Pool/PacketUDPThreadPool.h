#pragma once

#include "ClassTypePoolThread.h"
#include "PacketTCPFactoryManager.h"

// 用于创建UDP的消息包,虽然也是PacketTCP类型的,目的是为了分开不同的对象池
class PacketUDPThreadPool : public ClassTypePoolThread<PacketTCP, ushort>
{
protected:
	PacketTCP* create(const ushort type) override
	{
		auto* factory = mPacketTCPFactoryManager->getFactory(type);
		return factory != nullptr ? factory->create() : nullptr;
	}
};