#pragma once

#include "FrameComponent.h"

class MICRO_LEGEND_FRAME_API ExcelManager : public FrameComponent
{
	BASE(ExcelManager, FrameComponent);
public:
	void quit() override;
	ExcelTableBase* getTable(const string& name) const { return mTableList.get(name); }
	template<typename T, typename TypeCheck = typename IsSubClassOf<ExcelTableBase, T>::mType>
	T* registeExcel(const char* tableName)
	{
		T* table = new T();
		table->init(tableName);
		mTableList.add(tableName, table);
		return table;
	}
	void checkAll();
public:
	HashMap<string, ExcelTableBase*> mTableList;
};