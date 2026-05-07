#include "FrameHeader.h"

void ExcelData::cloneTo(ExcelData* target)
{
	target->mID = mID;
}

void ExcelData::read(SerializerRead* reader)
{
	reader->read(mID);
}