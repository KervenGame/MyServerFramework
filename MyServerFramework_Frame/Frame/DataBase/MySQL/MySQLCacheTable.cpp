#ifdef _MYSQL
#include "FrameHeader.h"

MySQLCacheTable::MySQLCacheTable(MySQLTable* table) :
	mTable(table)
{}

void MySQLCacheTable::destroy()
{
	deleteAllData();
}

ullong MySQLCacheTable::columnToFlag(const int colName) const
{
	ullong flag = 0;
	setBitOne(flag, colName);
	return flag;
}

ullong MySQLCacheTable::columnsToFlag(const Vector<int>& colNames) const
{
	ullong flag = 0;
	for (const int col : colNames)
	{
		setBitOne(flag, col);
	}
	return flag;
}

MySQLData* MySQLCacheTable::getDataByLLong2Or(const int column0, const llong value0, const int column1, const llong value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column0, value0) || item.second->hasLLong(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getDataByLLong2And(const int column0, const llong value0, const int column1, const llong value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column0, value0) && item.second->hasLLong(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getDataByInt(const int column, const int value)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasInt(column, value))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getDataByLLong(const int column, const llong value)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column, value))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getDataByString(const int column, const string& value)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasString(column, value))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getDataByString2And(const int column0, const string& value0, const int column1, const string& value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasString(column0, value0) && item.second->hasString(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getDataByLLongIntAnd(const int column0, const llong value0, const int column1, const int value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column0, value0) && item.second->hasInt(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getDataByLLongStringAnd(const int column0, const llong value0, const int column1, const string& value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column0, value0) && item.second->hasString(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

void MySQLCacheTable::getCacheDataListByInt(const int column0, int value0, Vector<MySQLData*>& dataList)
{
	dataList.clear();
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasInt(column0, value0))
		{
			dataList.add(item.second->getData());
		}
	}
}

void MySQLCacheTable::updateInt(const llong id, const int value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.get(id))
	{
		curData->updateInt(colName, value);
	}
}

void MySQLCacheTable::updateBool(const llong id, const bool value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.get(id))
	{
		curData->updateBool(colName, value);
	}
}

void MySQLCacheTable::updateInt2(const llong id, const int value0, const int colName0, const int value1, const int colName1) const
{
	if (MySQLCacheData* curData = mCacheDataList.get(id))
	{
		curData->updateInt(colName0, value0);
		curData->updateInt(colName1, value1);
	}
}

void MySQLCacheTable::updateFloat(const llong id, const float value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.get(id))
	{
		curData->updateFloat(colName, value);
	}
}

void MySQLCacheTable::updateLLong(const llong id, const llong value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.get(id))
	{
		curData->updateLLong(colName, value);
	}
}

void MySQLCacheTable::updateLLongInt(const llong id, const llong value0, const int colName0, const int value1, const int column1) const
{
	if (MySQLCacheData* curData = mCacheDataList.get(id))
	{
		curData->updateLLong(colName0, value0);
		curData->updateInt(column1, value1);
	}
}

void MySQLCacheTable::updateLLongBool(const llong id, const llong value0, const int colName0, const bool value1, const int column1) const
{
	if (MySQLCacheData* curData = mCacheDataList.get(id))
	{
		curData->updateLLong(colName0, value0);
		curData->updateBool(column1, value1);
	}
}

void MySQLCacheTable::updateString(const llong id, const string& value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.get(id))
	{
		curData->updateString(colName, value);
	}
}

void MySQLCacheTable::updateString2(const llong id, const string& value0, const int colName0, const string& value1, const int colName1) const
{
	if (MySQLCacheData* curData = mCacheDataList.get(id))
	{
		curData->updateString(colName0, value0);
		curData->updateString(colName1, value1);
	}
}

void MySQLCacheTable::updateAllString(const int colName, const string& value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateString(colName, value);
	}
}

void MySQLCacheTable::updateAllInt(const int colName, const int value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateInt(colName, value);
	}
}

void MySQLCacheTable::updateAllLLong(const int colName, const llong value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateLLong(colName, value);
	}
}

void MySQLCacheTable::updateAllFloat(const int colName, const float value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateFloat(colName, value);
	}
}

void MySQLCacheTable::updateAllBool(const int colName, const bool value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateBool(colName, value);
	}
}

void MySQLCacheTable::addOrUpdateData(const MySQLData* data, const ullong validFlag)
{
	if (data == nullptr)
	{
		return;
	}
	// 数据已经有了,则更新数据
	if (MySQLCacheData* curData = mCacheDataList.get(data->mID))
	{
		data->cloneWithFlag(curData->getData(), validFlag);
		curData->addValidFlag(validFlag);
	}
	// 插入新的数据
	else
	{
		MySQLCacheData* cacheData = mMySQLCacheDataPool->newClass();
		cacheData->setTable(mTable);
		data->cloneTo(cacheData->getData());
		cacheData->addValidFlag(validFlag);
		mCacheDataList.add(cacheData->getID(), cacheData);
	}
}

void MySQLCacheTable::addOrUpdateDataList(const Vector<MySQLData*>& dataList, const ullong validFlag)
{
	for (const MySQLData* data : dataList)
	{
		if (data == nullptr)
		{
			continue;
		}
		// 数据已经有了,则更新数据
		if (MySQLCacheData* curData = mCacheDataList.get(data->mID))
		{
			data->cloneWithFlag(curData->getData(), validFlag);
			curData->addValidFlag(validFlag);
		}
		// 插入新的数据
		else
		{
			MySQLCacheData* cacheData = mMySQLCacheDataPool->newClass();
			cacheData->setTable(mTable);
			data->cloneTo(cacheData->getData());
			cacheData->addValidFlag(validFlag);
			mCacheDataList.add(cacheData->getID(), cacheData);
		}
	}
}

void MySQLCacheTable::deleteAllData()
{
	mMySQLCacheDataPool->destroyClassList(mCacheDataList);
}

void MySQLCacheTable::deleteData(const llong id)
{
	MySQLCacheData* data = nullptr;
	if (mCacheDataList.remove(id, data))
	{
		mMySQLCacheDataPool->destroyClass(data);
	}
}

void MySQLCacheTable::deleteDataInIDRange(const llong minID, const llong maxID)
{
	mCacheDataList.remove([=](llong key, MySQLCacheData* data) { return key >= minID && key <= maxID; },
						  [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
}

void MySQLCacheTable::deleteDataByColumnBool(const int column, const bool value, bool onlyOnce)
{
	if (onlyOnce)
	{
		mCacheDataList.removeFirst([=](llong key, MySQLCacheData* data) { return data->hasBool(column, value); },
								   [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
	else
	{
		mCacheDataList.remove([=](llong key, MySQLCacheData* data) { return data->hasBool(column, value); },
							  [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
}

void MySQLCacheTable::deleteDataByColumnInt(const int column, const int value, bool onlyOnce)
{
	if (onlyOnce)
	{
		mCacheDataList.removeFirst([=](llong key, MySQLCacheData* data) { return data->hasInt(column, value); },
								   [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
	else
	{
		mCacheDataList.remove([=](llong key, MySQLCacheData* data) { return data->hasInt(column, value); },
							  [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
}

void MySQLCacheTable::deleteDataByColumnFloat(const int column, const float value, bool onlyOnce)
{
	if (onlyOnce)
	{
		mCacheDataList.removeFirst([=](llong key, MySQLCacheData* data) { return data->hasFloat(column, value); },
								   [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
	else
	{
		mCacheDataList.remove([=](llong key, MySQLCacheData* data) { return data->hasFloat(column, value); },
							  [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
}

void MySQLCacheTable::deleteDataByColumnLLong(const int column, const llong value, bool onlyOnce)
{
	if (onlyOnce)
	{
		mCacheDataList.removeFirst([=](llong key, MySQLCacheData* data) { return data->hasLLong(column, value); },
								   [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
	else
	{
		mCacheDataList.remove([=](llong key, MySQLCacheData* data) { return data->hasLLong(column, value); },
							  [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
}

void MySQLCacheTable::deleteDataByColumnString(const int column, const string& value, bool onlyOnce)
{
	if (onlyOnce)
	{
		mCacheDataList.removeFirst([=](llong key, MySQLCacheData* data) { return data->hasString(column, value); },
								   [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
	else
	{
		mCacheDataList.remove([=](llong key, MySQLCacheData* data) { return data->hasString(column, value); },
							  [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
}

void MySQLCacheTable::deleteDataByColumnLLong2And(const int column0, const llong value0, const int column1, const llong value1, bool onlyOnce)
{
	if (onlyOnce)
	{
		mCacheDataList.removeFirst([=](llong key, MySQLCacheData* data) { return data->hasLLong(column0, value0) && data->hasLLong(column1, value1); },
								   [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
	else
	{
		mCacheDataList.remove([=](llong key, MySQLCacheData* data) { return data->hasLLong(column0, value0) && data->hasLLong(column1, value1); },
							  [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
}

void MySQLCacheTable::deleteDataByColumnLLongIntAnd(const int column0, const llong value0, const int column1, const int value1, bool onlyOnce)
{
	if (onlyOnce)
	{
		mCacheDataList.removeFirst([=](llong key, MySQLCacheData* data) { return data->hasLLong(column0, value0) && data->hasInt(column1, value1); },
								   [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
	else
	{
		mCacheDataList.remove([=](llong key, MySQLCacheData* data) { return data->hasLLong(column0, value0) && data->hasInt(column1, value1); },
							  [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
}

void MySQLCacheTable::deleteDataByColumnLLong2Or(const int column0, const llong value0, const int column1, const llong value1, bool onlyOnce)
{
	if (onlyOnce)
	{
		mCacheDataList.removeFirst([=](llong key, MySQLCacheData* data) { return data->hasLLong(column0, value0) || data->hasLLong(column1, value1); },
								   [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
	else
	{
		mCacheDataList.remove([=](llong key, MySQLCacheData* data) { return data->hasLLong(column0, value0) || data->hasLLong(column1, value1); },
							  [](MySQLCacheData* data) { mMySQLCacheDataPool->destroyClass(data); });
	}
}
#endif