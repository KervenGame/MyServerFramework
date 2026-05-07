#pragma once

#include "ClassObject.h"

// 对T类型的对象的引用,会自动判断对象是否已经被销毁,如果已经被销毁则返回无效的引用,如果没有被销毁则可以正常访问对象
// 它会通过对象的唯一ID来判断对象是否已经被销毁,所以即使对象被销毁后又被重用,也不会误判为同一个对象
// T需要是ClassObject的子类,因为需要通过ClassObject的接口来判断对象是否已经被销毁
// 此对象可以自由传递引用或者复制对象,不影响其功能
template<class T, typename TypeCheck = typename IsSubClassOf<ClassObject, T>::mType>
class Ref
{
public:
	Ref() = default;
	explicit Ref(T* ptr)
	{
		mPtr = ptr;
		if (mPtr != nullptr && mPtr->isDestroy())
		{
			mPtr = nullptr;
		}
		mAssignID = ptr != nullptr ? ptr->getAssignID() : 0;
	}
	template<class T0, typename TypeCheck0 = typename IsSubClassOf<T, T0>::mType>
	Ref(const Ref<T0>& ptr)
	{
		mPtr = ptr;
		if (mPtr != nullptr && mPtr->isDestroy())
		{
			mPtr = nullptr;
		}
		mAssignID = ptr != nullptr ? ptr->getAssignID() : 0;
	}
	bool isValid() const		{ return mPtr != nullptr && mPtr->getAssignID() == mAssignID; }
	T* operator->() const		{ return mPtr; }
	template<class T0, typename TypeCheck0 = typename IsSubClassOf<T, T0>::mType>
	T0* cast() const			{ return static_cast<T0*>(mPtr); }
	template<class T0, typename TypeCheck0 = typename IsSubClassOf<T, T0>::mType>
	Ref<T0> castRef() const		{ return Ref<T0>(static_cast<T0*>(mPtr)); }
	void clear()
	{
		mPtr = nullptr;
		mAssignID = 0;
	}
public:
	T* mPtr = nullptr;
	llong mAssignID = 0;
	static const Ref<T, TypeCheck> null;
};

template<typename T, typename TypeCheck>
const Ref<T, TypeCheck> Ref<T, TypeCheck>::null;