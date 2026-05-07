#include "GameHeader.h"

// auto generate start
SCAttack SCAttack::mStaticObject;
string SCAttack::mPacketName = STR(SCAttack);
// auto generate end

void SCAttack::send(CharacterPlayer* player)
{
	sendPacketTCP(&get(), player->getClient());
}