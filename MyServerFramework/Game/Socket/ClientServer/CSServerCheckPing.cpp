#include "GameHeader.h"

// auto generate start
CSServerCheckPing CSServerCheckPing::mStaticObject;
string CSServerCheckPing::mPacketName = STR(CSServerCheckPing);
// auto generate end

void CSServerCheckPing::execute()
{
	SCServerCheckPing::send(mClient, mIndex);
}