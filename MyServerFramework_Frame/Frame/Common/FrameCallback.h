#pragma once

#include "FrameDefine.h"
#include "Vector.h"
#include "HashMap.h"
#include "FrameCallbackClean.h"

// 这个文件中由于需要一些依赖,所以只放Vector,HashMap这些需要包含头文件的回调定义,其他不需要包含头文件的回调定义放在FrameCallbackClean.h中
typedef function<void(const Vector<string>& value)> StringListCallback;
typedef function<void(const HashMap<llong, string>& value)> LLongStringMapCallback;
typedef function<void(const Vector<MySQLData*>& dataList)> QueryDataListCallback;
template <typename T>
using QueryDataListCallbackT = function<void(const Vector<T*>& dataList)>;
using EventCallback = void(*)(GameEvent*, IEventListener*);