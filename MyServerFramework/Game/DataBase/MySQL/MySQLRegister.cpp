// auto generate start
#include "GameHeader.h"

void MySQLRegister::registeAll()
{
	mMySQLAccount = mMySQLManager->registeTable<MySQLAccount>("account");
}
// auto generate end