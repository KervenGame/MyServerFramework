#pragma once

#include "ClassTypePoolThread.h"
#include "PacketWebSocketFactoryManager.h"

class PacketWebSocketPool : public ClassTypePool<PacketWebSocket, ushort>
{
protected:
	PacketWebSocket* create(const ushort type) override
	{
		auto* factory = mPacketWebSocketFactoryManager->getFactory(type);
		return factory != nullptr ? factory->create() : nullptr;
	}
};