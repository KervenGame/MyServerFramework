#pragma once

#include "Array.h"
#include "ArrayList.h"
#include "IsPODType.h"
#include "FrameCallbackClean.h"

template<typename T, typename TypeCheck = enable_if_t<IsNotPodAndPointerType<T>::mValue>>
static void copyVector(const vector<T>& source, const int sourceCount, vector<T>& target, const int targetOldCount)
{
	target.reserve(target.size() + sourceCount);
	FOR(sourceCount)
	{
		target.emplace_back(source[i]);
	}
}

template<typename T, typename TypeCheck = enable_if_t<IsNotPodAndPointerType<T>::mValue>>
static void copyVector(const T* source, const int sourceCount, vector<T>& target, const int targetOldCount)
{
	target.reserve(targetOldCount + sourceCount);
	FOR(sourceCount)
	{
		target.emplace_back(source[i]);
	}
}

template<typename T, typename TypeCheck = enable_if_t<IsPodOrPointerType<T>::mValue>>
static void memcpyVector(const vector<T>& source, const int sourceCount, vector<T>& target, const int targetOldCount)
{
	const int newSize = targetOldCount + sourceCount;
	target.resize(newSize);
	MEMCPY((char*)target.data() + targetOldCount * sizeof(T), newSize * sizeof(T), source.data(), sizeof(T) * sourceCount);
}

template<typename T, typename TypeCheck = enable_if_t<IsPodOrPointerType<T>::mValue>>
static void memcpyVector(const T* source, const int sourceCount, vector<T>& target, const int targetOldCount)
{
	const int newSize = targetOldCount + sourceCount;
	target.resize(newSize);
	MEMCPY((char*)target.data() + targetOldCount * sizeof(T), newSize * sizeof(T), source, sizeof(T) * sourceCount);
}

// 对于bool类型可能不太适用,在std::vector内部有对bool类型的特例化,可能某些函数无法正常编译通过
template<typename T>
class Vector
{
public:
	typedef typename vector<T>::iterator iterator;
	typedef typename vector<T>::reverse_iterator reverse_iterator;
	typedef typename vector<T>::const_iterator const_iterator;
public:
	Vector()
	{
		mVector.reserve(4);
	}
	explicit Vector(const int reserveSize)
	{
		mVector.reserve(reserveSize);
	}
	Vector(const Vector<T>& other) :
		mVector(other.mVector)
	{
		mSize = other.size();
	}
	Vector(Vector<T>&& other) noexcept :
		mVector(move(other.mVector))
	{
		mSize = other.size();
		other.mSize = 0;
	}
	explicit Vector(initializer_list<T> _Ilist)
	{
		mVector.insert(mVector.begin(), _Ilist);
		mSize = (int)mVector.size();
	}
	Vector<T>& operator=(Vector<T>&& other) noexcept
	{
		mVector = move(other.mVector);
		mSize = other.size();
		other.mSize = 0;
		return *this;
	}
	Vector<T>& operator=(const Vector<T>& other)
	{
		mVector = other.mVector;
		mSize = other.mSize;
		return *this;
	}
	bool operator==(const Vector<T>& other)
	{
		if (mSize != other.size())
		{
			return false;
		}
		FOR(mSize)
		{
			if (mVector[i] != other[i])
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const Vector<T>& other)
	{
		if (mSize != other.size())
		{
			return true;
		}
		FOR(mSize)
		{
			if (mVector[i] != other[i])
			{
				return true;
			}
		}
		return false;
	}
	virtual ~Vector()				{ mVector.clear(); }
	T* data() const					
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		return (T*)mVector.data(); 
	}
	int size() const				{ return mSize; }
	bool isEmpty() const			{ return mSize <= 0; }
	// 用于支持范围for循环
	iterator begin()				{ return mVector.begin(); }
	iterator end()					{ return mVector.end(); }
	const_iterator begin() const	{ return mVector.begin(); }
	const_iterator end()  const		{ return mVector.end(); }
	reverse_iterator rbegin()		{ return mVector.rbegin(); }
	reverse_iterator rend()			{ return mVector.rend(); }
	const_iterator cbegin() const	{ return mVector.cbegin(); }
	const_iterator cend() const		{ return mVector.cend(); }
	// 添加或者删除一个指定元素
	bool addOrRemove(const T& value, bool isAdd)
	{
		if (isAdd)
		{
			add(value);
		}
		else
		{
			remove(value);
		}
		return isAdd;
	}
	// 返回最后一个元素,并且将该元素移除,如果当前列表为空,则返回defaultValue
	T popBack(const T& defaultValue)
	{
		if (mSize == 0)
		{
			return defaultValue;
		}
		const T value = mVector[--mSize];
		mVector.pop_back();
		return value;
	}
	// 通过逐个拷贝的方式去添加,仅限非基础数据类型
	void addRangeCopy(const T* values, const int count)
	{
		if (values == nullptr || count == 0)
		{
			return;
		}
		copyVector(values, count, mVector, mSize);
		mSize += count;
	}
	// 通过逐个拷贝的方式去添加,仅限非基础数据类型
	void setRangeCopy(const T* values, const int count)
	{
		clear();
		if (values == nullptr || count == 0)
		{
			return;
		}
		copyVector(values, count, mVector, mSize);
		mSize += count;
	}
	// 通过逐个拷贝的方式去添加,仅限非基础数据类型
	void addRangeCopy(const Vector<T>& values)
	{
		const int sourceCount = values.size();
		if (sourceCount == 0)
		{
			return;
		}
		copyVector(values.mVector, sourceCount, mVector, mSize);
		mSize += sourceCount;
	}
	// 通过逐个拷贝的方式去添加,仅限非基础数据类型
	void setRangeCopy(const Vector<T>& values)
	{
		clear();
		const int sourceCount = values.size();
		if (sourceCount == 0)
		{
			return;
		}
		copyVector(values.mVector, sourceCount, mVector, mSize);
		mSize += sourceCount;
	}
	// 通过直接内存拷贝的方式进行添加,仅限基础数据类型
	void addRange(const T* values, const int count)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		if (count == 0)
		{
			return;
		}
		memcpyVector(values, count, mVector, mSize);
		mSize += count;
	}
	// 通过直接内存拷贝的方式进行添加,仅限基础数据类型
	void setRange(const T* values, const int count)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		clear();
		if (count == 0)
		{
			return;
		}
		memcpyVector(values, count, mVector, mSize);
		mSize += count;
	}
	// 通过直接内存拷贝的方式进行添加,仅限基础数据类型
	template<int Length>
	void addRange(const ArrayList<Length, T>& values)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		const int count = values.size();
		if (count == 0)
		{
			return;
		}
		memcpyVector(values.data(), count, mVector, mSize);
		mSize += count;
	}
	// 通过直接内存拷贝的方式进行添加,仅限基础数据类型
	template<int Length>
	void setRange(const ArrayList<Length, T>& values)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		clear();
		const int count = values.size();
		if (count == 0)
		{
			return;
		}
		memcpyVector(values.data(), count, mVector, mSize);
		mSize += count;
	}
	// 通过直接内存拷贝的方式进行添加,仅限基础数据类型
	template<int Length>
	void addRange(const Array<Length, T>& values, const int count)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		if (count == 0)
		{
			return;
		}
		memcpyVector(values.data(), count, mVector, mSize);
		mSize += count;
	}
	// 通过直接内存拷贝的方式进行添加,允许T是指针,且T0是T的子类
	template<typename T0>
	void addRangePtr(const Vector<T0*>& values)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		const int sourceCount = values.size();
		if (sourceCount == 0)
		{
			return;
		}
		// 这里如果调用memcpyVector,会匹配失败,即使加了重载,也会使编译器无法识别调用哪个函数,所以直接写上了
		const int newSize = mSize + sourceCount;
		mVector.resize(newSize);
		MEMCPY((char*)mVector.data() + mSize * sizeof(T0*), newSize * sizeof(T0*), values.mVector.data(), sizeof(T0*) * sourceCount);

		mSize += sourceCount;
	}
	// 通过直接内存拷贝的方式进行添加,仅限基础数据类型
	void addRange(const Vector<T>& values)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		const int sourceCount = values.size();
		if (sourceCount == 0)
		{
			return;
		}
		memcpyVector(values.mVector, sourceCount, mVector, mSize);
		mSize += sourceCount;
	}
	// 通过直接内存拷贝的方式进行添加,仅限基础数据类型
	void setRange(const Vector<T>& values)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		clear();
		const int sourceCount = values.size();
		if (sourceCount == 0)
		{
			return;
		}
		memcpyVector(values.mVector, sourceCount, mVector, mSize);
		mSize += sourceCount;
	}
	// 通过直接内存拷贝的方式进行添加,仅限基础数据类型,虽然这里仍然使用的是拷贝
	void addRange(Vector<T>&& values)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		const int sourceCount = values.size();
		if (sourceCount == 0)
		{
			return;
		}
		memcpyVector(values.mVector, sourceCount, mVector, mSize);
		mSize += sourceCount;
		values.clear();
	}
	// 如果condition为true则添加元素,如果condition为false则不添加元素,并且返回condition
	bool addIf(const T& value, bool condition)
	{
		if (condition)
		{
			add(value);
		}
		return condition;
	}
	// 如果condition为true则添加元素,如果condition为false则不添加元素,并且返回condition
	bool addIf(T&& value, bool condition)
	{
		if (condition)
		{
			add(value);
		}
		return condition;
	}
	// 通过直接内存拷贝的方式进行添加,仅限基础数据类型,虽然这里仍然使用的是拷贝
	void setRange(Vector<T>&& values)
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		clear();
		const int sourceCount = values.size();
		if (sourceCount == 0)
		{
			return;
		}
		memcpyVector(values.mVector, sourceCount, mVector, mSize);
		mSize += sourceCount;
		values.clear();
	}
	// 设置元素列表
	void setData(const T* values, const int count)
	{
		mVector.clear();
		mSize = count;
		mVector.reserve(mSize);
		FOR(mSize)
		{
			mVector.emplace_back(values[i]);
		}
	}
	// 将所有元素设置为默认值,仅限基础数据类型
	void setAllDefaultMemorySet()
	{
		static_assert(!is_same<typename decay<T>::type, bool>::value, "not support Vector<bool>");
		memset(mVector.data(), mDefaultValue, mVector.size() * sizeof(T));
	}
	// 添加元素
	void add()
	{
		++mSize;
		mVector.emplace_back(mDefaultValue);
	}
	// 如果元素不存在则添加,如果元素已经存在则不添加,并且返回false,如果添加成功则返回true
	bool addUnique(const T& elem)
	{
		if (!contains(elem))
		{
			++mSize;
			mVector.emplace_back(elem);
			return true;
		}
		return false;
	}
	// 添加元素
	template <class... ParamList>
	void emplace(ParamList&&... _Val)
	{
		++mSize;
		mVector.emplace_back(forward<ParamList>(_Val)...);
	}
	// 一般都是基础数据类型或者指针才会使用此方法,因此不使用引用
	void addNotEqual(T elem, const T other)
	{
		if (elem == other)
		{
			return;
		}
		++mSize;
		mVector.emplace_back(elem);
	}
	// 如果add为true则添加元素,如果add为false则移除元素,如果添加或者移除成功则返回true,如果添加失败或者移除失败则返回false
	void addUniqueOrRemove(const T& elem, bool add)
	{
		if (add)
		{
			addUnique(elem);
		}
		else
		{
			remove(elem);
		}
	}
	// 虽然这里仍然使用的是拷贝,但是可以减少调用add的次数,因此效率会更高一些
	void add(const T& elem)
	{
		++mSize;
		mVector.emplace_back(elem);
	}
	// 虽然这里仍然使用的是拷贝,但是可以减少调用add的次数,因此效率会更高一些
	void add(T&& elem)
	{
		++mSize;
		mVector.emplace_back(move(elem));
	}
	// 虽然这里仍然使用的是拷贝,但是可以减少调用add的次数,因此效率会更高一些
	void add(const T& elem0, const T& elem1)
	{
		mSize += 2;
		mVector.reserve(mSize);
		mVector.emplace_back(elem0);
		mVector.emplace_back(elem1);
	}
	// 虽然这里仍然使用的是拷贝,但是可以减少调用add的次数,因此效率会更高一些
	void add(const T& elem0, const T& elem1, const T& elem2)
	{
		mSize += 3;
		mVector.reserve(mSize);
		mVector.emplace_back(elem0);
		mVector.emplace_back(elem1);
		mVector.emplace_back(elem2);
	}
	// 虽然这里仍然使用的是拷贝,但是可以减少调用add的次数,因此效率会更高一些
	void add(const T& elem0, const T& elem1, const T& elem2, const T& elem3)
	{
		mSize += 4;
		mVector.reserve(mSize);
		mVector.emplace_back(elem0);
		mVector.emplace_back(elem1);
		mVector.emplace_back(elem2);
		mVector.emplace_back(elem3);
	}
	// 虽然这里仍然使用的是拷贝,但是可以减少调用add的次数,因此效率会更高一些
	void add(const T& elem0, const T& elem1, const T& elem2, const T& elem3, const T& elem4)
	{
		mSize += 5;
		mVector.reserve(mSize);
		mVector.emplace_back(elem0);
		mVector.emplace_back(elem1);
		mVector.emplace_back(elem2);
		mVector.emplace_back(elem3);
		mVector.emplace_back(elem4);
	}
	// 虽然这里仍然使用的是拷贝,但是可以减少调用add的次数,因此效率会更高一些
	void add(const T& elem0, const T& elem1, const T& elem2, const T& elem3, const T& elem4, const T& elem5)
	{
		mSize += 6;
		mVector.reserve(mSize);
		mVector.emplace_back(elem0);
		mVector.emplace_back(elem1);
		mVector.emplace_back(elem2);
		mVector.emplace_back(elem3);
		mVector.emplace_back(elem4);
		mVector.emplace_back(elem5);
	}
	// 移除指定迭代器位置的元素,并返回下一个元素的迭代器,如果下标不合法则返回end迭代器
	iterator remove(const iterator& iter)
	{
		iterator retIter = mVector.erase(iter);
		mSize = (int)mVector.size();
		return retIter;
	}
	// 移除指定范围内的元素,并返回第一个被移除元素的下一个元素的迭代器,如果下标不合法则返回end迭代器
	iterator remove(const iterator& iter, const iterator& end)
	{
		iterator retIter = mVector.erase(iter, end);
		mSize = (int)mVector.size();
		return retIter;
	}
	// 移除指定下标的元素,并返回下一个元素的迭代器,如果下标不合法则返回end迭代器
	iterator removeAt(const int index)
	{
		if (index < 0 || index >= mSize)
		{
			return mVector.end();
		}
		iterator iter = mVector.erase(mVector.begin() + index);
		mSize = (int)mVector.size();
		return iter;
	}
	// 移除指定下标的元素,如果count大于1则移除多个元素,并返回第一个被移除元素的下标,如果下标不合法则返回-1
	iterator removeRange(const int index, const int count)
	{
		if (index < 0 || index >= mSize)
		{
			return mVector.end();
		}
		iterator iter = mVector.erase(mVector.begin() + index, mVector.begin() + (index + count));
		mSize = (int)mVector.size();
		return iter;
	}
	// 移除指定下标的元素,并返回该元素的值,如果下标不合法则返回默认值
	const T& removeAtAndGet(const int index)
	{
		if (index < 0 || index >= mSize)
		{
			return mDefaultValue;
		}
		const T& value = mVector[index];
		mVector.erase(mVector.begin() + index);
		mSize = (int)mVector.size();
		return value;
	}
	// 移除最后一个元素
	void removeLast()
	{
		if (mSize == 0)
		{
			return;
		}
		--mSize;
		mVector.pop_back();
	}
	// 移除第一个指定值的元素,并返回是否移除成功
	bool remove(const T& value)
	{
		FOR(mSize)
		{
			if (mVector[i] == value)
			{
				mVector.erase(mVector.begin() + i);
				--mSize;
				return true;
			}
		}
		return false;
	}
	void remove(const Vector<T>& valueList)
	{
		for (const T& value : valueList)
		{
			remove(value);
		}
	}
	// 移除所有指定值的元素,并返回实际移除的数量
	int removeAll(const T& value)
	{
		mVector.erase(std::remove(mVector.begin(), mVector.end(), value), mVector.end());
		const int eraseCount = mSize - (int)mVector.size();
		mSize = (int)mVector.size();
		return eraseCount;
	}
	// 将第一个指定值的元素替换为默认值
	bool resetElement(const T& value)
	{
		FOR(mSize)
		{
			if (mVector[i] == value)
			{
				mVector[i] = mDefaultValue;
				return true;
			}
		}
		return false;
	}
	// 移除指定下标的元素,并将该位置的元素替换为默认值,如果下标不合法则返回false
	bool resetElementAt(const int index)
	{
		if (index < 0 || index >= mSize)
		{
			return false;
		}
		mVector[index] = mDefaultValue;
		return true;
	}
	// 移除所有指定值的元素,如果count大于0则只移除count个,如果count小于等于0则移除所有,并返回实际移除的数量
	void clearDefaultElement(int count = 0)
	{
		FOR_INVERSE(mSize)
		{
			if (mVector[i] == mDefaultValue)
			{
				mVector.erase(mVector.begin() + i);
				--mSize;
				if (--count == 0)
				{
					return;
				}
			}
		}
	}
	// 清空列表,如果disposeMemory为true则同时释放内存,如果disposeMemory为false则保留内存,如果当前列表已经为空则不作任何事情
	void clear(bool disposeMemory = false)
	{
		if (mSize > 0)
		{
			mSize = 0;
			mVector.clear();
		}
		if (disposeMemory)
		{
			dispose();
		}
	}
	// 清空列表并且预设容量,如果当前的最大容量已经大于等于要设置的容量则不作任何事情
	void clearAndReserve(const int capacity)
	{
		clear();
		reserve(capacity);
	}
	// 插入一个元素
	void insert(const int index, const T& elem)
	{
		mVector.emplace(mVector.begin() + index, elem);
		++mSize;
	}
	// 插入一个元素
	void insert(const int index, T&& elem)
	{
		mVector.emplace(mVector.begin() + index, move(elem));
		++mSize;
	}
	// 在迭代器位置插入元素,并返回该元素的迭代器,如果下标不合法则返回end迭代器
	void insert(const iterator& iter, const T& elem)
	{
		mVector.emplace(iter, elem);
		++mSize;
	}
	// 根据下标获取元素,如果下标不合法则返回默认值
	decltype(auto) operator[](const int i) const
	{
		if (i < 0 || i >= mSize)
		{
			ERROR("vector index out of range! index:" + to_string(i) + ", vector size:" + to_string(mSize));
		}
		return mVector[i];
	}
	// 根据下标获取元素,如果下标不合法则返回默认值
	decltype(auto) get(const int index) const
	{
		if (index < 0 || index >= mSize)
		{
			return mVector.empty() ? mDefaultValue : mDefaultValue;
		}
		return mVector[index];
	}
	// 根据下标获取元素,如果下标不合法则返回defaultValue,返回值类型写成decltype(auto)可以兼容Vector<bool>,否则会编译报错
	decltype(auto) get(const int index, const T& defaultValue) const
	{
		if (index < 0 || index >= mSize)
		{
			return defaultValue;
		}
		return mVector[index];
	}
	decltype(auto) operator[](const int i)
	{
		if (i < 0 || i >= mSize)
		{
			ERROR("vector index out of range! index:" + to_string(i) + ", vector size:" + to_string(mSize));
		}
		return mVector[i];
	}
	// 同时修改capacity和size
	void resize(const int size)
	{
		mVector.resize(size, mDefaultValue);
		mSize = size;
	}
	// 只增加capacity,不改变当前size,如果当前的最大容量已经大于等于要设置的容量则不作任何事情
	void reserve(const int capacity)
	{
		if (capacity <= 0)
		{
			return;
		}
		mVector.reserve(capacity);
	}
	// 判断列表中是否包含指定元素
	bool contains(const T& value) const
	{
		return mSize > 0 && std::find(mVector.begin(), mVector.end(), value) != mVector.end();
	}
	// 是否包含满足条件的元素,比普通遍历查找要慢,仅在非高频路径下使用
	template<typename Predicate>
	bool containsT(Predicate&& pred) const
	{
		FOR(mSize)
		{
			if (invoke(pred, mVector[i]))
			{
				return true;
			}
		}
		return false;
	}
	// 判断列表中的元素是否全部都等于指定元素
	bool isAll(const T& element) const
	{
		FOR(mSize)
		{
			if (mVector[i] != element)
			{
				return false;
			}
		}
		return true;
	}
	// 查找指定元素的下标,如果没有找到则返回-1
	int findIndex(const T& value, const int startIndex = 0) const
	{
		for (int i = startIndex; i < mSize; ++i)
		{
			if (mVector[i] == value)
			{
				return i;
			}
		}
		return -1;
	}
	// 查找指定元素的数量
	int findCount(const T& value) const
	{
		int elementCount = 0;
		FOR(mSize)
		{
			if (mVector[i] == value)
			{
				++elementCount;
			}
		}
		return elementCount;
	}
	// 交换两个元素的位置
	void swapIndex(int index0, int index1)
	{
		T temp = move(mVector[index0]);
		mVector[index0] = move(mVector[index1]);
		mVector[index1] = move(temp);
	}
	// 随机移除一个元素并返回该元素,如果列表为空则返回默认值
	const T& randomRemove()
	{
		const int count = size();
		if (count == 0)
		{
			return mDefaultValue;
		}
		const int index = (((rand() & 0x7FFF) << 15) + (rand() & 0x7FFF)) % count;
		const T& value = get(index);
		removeAt(index);
		return value;
	}
	// 随机获取一个元素,如果列表为空则返回默认值
	const T& random() const
	{
		const int count = size();
		if (count == 0)
		{
			return mDefaultValue;
		}
		return get((((rand() & 0x7FFF) << 15) + (rand() & 0x7FFF)) % count);
	}
	// 添加克隆函数的目的是为了显式调用拷贝,而非自动调用拷贝,可以避免可以使用移动构造而没有使用的情况
	void cloneTo(Vector<T>& target) const
	{
		target.mVector = mVector;
		target.mSize = mSize;
	}
	void shrink_to_fit()
	{
		mVector.shrink_to_fit();
	}
	// 清空列表并且释放内存
	void dispose()
	{
		vector<T> temp;
		mVector.swap(temp);
	}
	// 对元素进行排序, 需要保证元素类型重载了 < 运算符
	void sort()
	{
		std::sort(mVector.begin(), mVector.end());
	}
public:
	vector<T> mVector;
	static const Vector<T> mDefaultList;
protected:
	int mSize = 0;			// 因为vector的size()非常耗时,所以单独使用一个变量记录元素数量,不过实测好像没有快多少
private:
	static const T mDefaultValue;
};

template<typename T>
const T Vector<T>::mDefaultValue = T();

template<typename T>
const Vector<T> Vector<T>::mDefaultList;