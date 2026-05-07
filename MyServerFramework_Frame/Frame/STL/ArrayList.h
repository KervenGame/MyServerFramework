#pragma once

#include "GameLogWrap.h"

template<int Length, class T>
class ArrayList
{
public:
	// 不使用ArrayList() = default; 因为这样仍然会允许使用初始化列表
	ArrayList(initializer_list<T>) = delete;
	ArrayList() {}
	// 如果设置的size比当前size小,则不会生效
	void setSize(int size) 
	{
		FOR(size - mSize)
		{
			mValue[mSize++] = mDefault;
		}
	}
	const T* data() const			{ return mValue; }
	T* data()						{ return mValue; }
	int size() const				{ return mSize; }
	bool isEmpty() const			{ return mSize == 0; }
	constexpr int maxSize() const	{ return Length; }
	void resize(int size)			{ mSize = size; }
	// 下面这些函数是用于支持for range循环的迭代器函数
	T* begin()						{ return mValue; }
	T* end()						{ return mValue + mSize; }
	const T* begin() const			{ return mValue; }
	const T* end() const			{ return mValue + mSize; }
	const T* cbegin() const			{ return mValue; }
	const T* cend() const			{ return mValue + mSize; }
	// 添加元素
	bool add(const T& value0, const T& value1, const T& value2, const T& value3, const T& value4, const T& value5)
	{
		if (mSize + 5 >= Length)
		{
			return false;
		}
		mValue[mSize] = value0;
		mValue[mSize + 1] = value1;
		mValue[mSize + 2] = value2;
		mValue[mSize + 3] = value3;
		mValue[mSize + 4] = value4;
		mValue[mSize + 5] = value5;
		mSize += 6;
		return true;
	}
	// 添加元素
	bool add(const T& value0, const T& value1, const T& value2, const T& value3, const T& value4)
	{
		if (mSize + 4 >= Length)
		{
			return false;
		}
		mValue[mSize] = value0;
		mValue[mSize + 1] = value1;
		mValue[mSize + 2] = value2;
		mValue[mSize + 3] = value3;
		mValue[mSize + 4] = value4;
		mSize += 5;
		return true;
	}
	// 添加元素
	bool add(const T& value0, const T& value1, const T& value2, const T& value3)
	{
		if (mSize + 3 >= Length)
		{
			return false;
		}
		mValue[mSize] = value0;
		mValue[mSize + 1] = value1;
		mValue[mSize + 2] = value2;
		mValue[mSize + 3] = value3;
		mSize += 4;
		return true;
	}
	// 添加元素
	bool add(const T& value0, const T& value1, const T& value2)
	{
		if (mSize + 2 >= Length)
		{
			return false;
		}
		mValue[mSize] = value0;
		mValue[mSize + 1] = value1;
		mValue[mSize + 2] = value2;
		mSize += 3;
		return true;
	}
	// 添加元素
	bool add(const T& value0, const T& value1)
	{
		if (mSize + 1 >= Length)
		{
			return false;
		}
		mValue[mSize] = value0;
		mValue[mSize + 1] = value1;
		mSize += 2;
		return true;
	}
	// 添加元素
	bool add(const T& value)
	{
		if (mSize >= Length)
		{
			return false;
		}
		mValue[mSize++] = value;
		return true;
	}
	// 如果condition为true则添加元素,如果condition为false则不添加元素,并且返回condition
	bool addIf(const T& value, bool condition)
	{
		if (condition)
		{
			add(value);
			return true;
		}
		return false;
	}
	// 如果condition为true并且列表中不存在该元素则添加元素,并且返回是否添加成功
	bool addUniqueIf(const T& value, bool condition)
	{
		if (condition)
		{
			return addUnique(value);
		}
		return false;
	}
	// 添加一个元素,如果value和other相等则不添加,并且返回是否添加成功
	bool addNotEqual(const T& value, const T& other)
	{
		if (mSize >= Length)
		{
			return false;
		}
		if (value == other)
		{
			return false;
		}
		mValue[mSize++] = value;
		return true;
	}
	// 添加一个元素,如果列表中已经存在该元素则不添加,并且返回是否添加成功
	bool addUnique(const T& value)
	{
		if (mSize >= Length)
		{
			return false;
		}
		if (contains(value))
		{
			return false;
		}
		mValue[mSize++] = value;
		return true;
	}
	// 添加数据
	bool add(T&& value)
	{
		if (mSize >= Length)
		{
			return false;
		}
		mValue[mSize++] = move(value);
		return true;
	}
	// 移除所有指定值的元素
	void removeAll(const T& element)
	{
		int curCount = mSize;
		FOR_INVERSE(curCount)
		{
			if (mValue[i] == element)
			{
				removeAt(i);
			}
		}
	}
	// 移除最后一个指定值的元素,并返回是否移除成功
	bool removeLast(const T& element)
	{
		FOR_INVERSE(mSize)
		{
			if (mValue[i] == element)
			{
				removeAt(i);
				return true;
			}
		}
		return false;
	}
	// 移除第一个指定值的元素,并返回是否移除成功
	bool remove(const T& element)
	{
		FOR(mSize)
		{
			if (mValue[i] == element)
			{
				removeAt(i);
				return true;
			}
		}
		return false;
	}
	// 移除指定下标的元素
	void removeAt(const int index)
	{
		if (index < 0 || index >= mSize)
		{
			ERROR("数组越界");
			return;
		}

		const int moveCount = mSize - index - 1;
		if (moveCount > 0)
		{
			if constexpr (is_trivially_copyable_v<T>)
			{
				memmove(mValue + index, mValue + index + 1, sizeof(T) * moveCount);
			}
			else
			{
				for (int i = index; i < mSize - 1; ++i)
				{
					mValue[i] = move(mValue[i + 1]);
				}
			}
		}
		--mSize;
	}
	// 是否包含指定元素
	bool contains(const T& element) const
	{
		FOR(mSize)
		{
			if (mValue[i] == element)
			{
				return true;
			}
		}
		return false;
	}
	// 判断数组中的元素是否全部都等于指定元素
	bool isAll(const T& element) const
	{
		FOR(mSize)
		{
			if (mValue[i] != element)
			{
				return false;
			}
		}
		return true;
	}
	// 在拷贝过程中不会调用拷贝构造,所以不适用于T带拷贝构造的情况
	template<int SourceLength>
	bool addRange(const ArrayList<SourceLength, T>& src)
	{
		return addRange(src.data(), src.size());
	}
	// 设置列表数据,会先清空再添加
	bool setRange(const T* src, const int copyCount)
	{
		clear();
		return addRange(src, copyCount);
	}
	// 添加列表数据
	bool addRange(const T* src, const int copyCount)
	{
		if (copyCount <= 0)
		{
			return true;
		}

		if (mSize + copyCount > Length)
		{
			LOG("拷贝数组太长");
			return false;
		}

		if constexpr (is_trivially_copyable_v<T>)
		{
			MEMCPY((char*)mValue + sizeof(T) * mSize, sizeof(T) * (Length - mSize), src, sizeof(T) * copyCount);
		}
		else
		{
			FOR(copyCount)
			{
				mValue[mSize + i] = src[i];
			}
		}
		mSize += copyCount;
		return true;
	}
	T* data(const int offset)
	{
		if (offset >= mSize)
		{
			ERROR("数组越界");
			return nullptr;
		}
		return mValue + offset; 
	}
	const T* data(const int offset) const
	{
		if (offset >= mSize)
		{
			ERROR("数组越界");
			return nullptr;
		}
		return mValue + offset;
	}
	constexpr const T& operator[](int index) const
	{
		if (index < 0 || index >= mSize)
		{
			ERROR("数组越界");
			index = mSize - 1;
		}
		return mValue[index];
	}
	constexpr T& operator[](int index)
	{
		if (index < 0 || index >= mSize)
		{
			ERROR("数组越界");
			index = mSize - 1;
		}
		return mValue[index];
	}
	constexpr const T& get(int index) const
	{
		if (index < 0 || index >= mSize)
		{
			return mDefault;
		}
		return mValue[index];
	}
	// 随机获取一个元素
	const T& random() const
	{
		if (mSize == 0)
		{
			return mDefault;
		}
		return get((((rand() & 0x7FFF) << 15) + (rand() & 0x7FFF)) % mSize);
	}
	// 将数组的内容重置为0,但是需要外部确保将所有字节设置为0不会引起问题,如果T类型是string或者其他的类则不允许使用此函数
	// 因为那会将虚表也重置为0,引起崩溃
	void zero()
	{
		static_assert(is_trivially_copyable_v<T>, "zero() only supports trivially copyable types");
		memset(mValue, 0, sizeof(T) * Length);
	}
	// 将数组的每一个元素都设置为value
	void setAll(const T& value)
	{
		FOR(Length)
		{
			mValue[i] = value;
		}
	}
	// 将数组的每一个元素都设置为value
	void setAll(const int start, const T& value)
	{
		for (int i = start; i < Length; ++i)
		{
			mValue[i] = value;
		}
	}
	// 清空数组长度
	void clear()
	{
		mSize = 0;
	}
public:
	T mValue[Length]{};
	int mSize = 0;
	static const ArrayList<Length, T> mDefaultList;
	static const T mDefault;
};

template<int Length, typename T>
const T ArrayList<Length, T>::mDefault = T();
template<int Length, typename T>
const ArrayList<Length, T> ArrayList<Length, T>::mDefaultList;