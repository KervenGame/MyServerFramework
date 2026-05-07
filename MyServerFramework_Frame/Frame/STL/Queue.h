#pragma once

#include <queue>

using std::queue;

template<typename T>
class Queue
{
public:
	void addRange(const Vector<T*>& list)
	{
		for (T* item : list)
		{
			mQueue.push(item);
		}
	}
	void add(const T& value) { mQueue.push(value); }
	T pop(T defaultValue)
	{
		if (isEmpty())
		{
			return defaultValue;
		}
		T value = front();
		mQueue.pop();
		return value;
	}
	T& front() { return mQueue.front(); }
	void popOnly() { mQueue.pop(); }
	int size() const { return (int)mQueue.size(); }
	bool isEmpty() const { return (int)mQueue.size() == 0; }
	void clear() { while (!mQueue.empty()) { mQueue.pop(); } }
public:
	queue<T> mQueue;
};