#ifdef _MYSQL
#include "FrameHeader.h"

namespace FrameMySQLUtility
{
	void pushCommandToMySQL(VoidCallback call)
	{
		if (mMySQLManager->getThread() == nullptr)
		{
			ERROR(string("数据库线程还未创建,无法执行数据库命令"));
		}
		mMySQLManager->getThread()->addCall(call);
	}
}
#endif