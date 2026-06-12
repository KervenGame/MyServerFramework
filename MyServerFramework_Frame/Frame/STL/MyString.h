#pragma once

#include "GameLogWrap.h"

// char的数组,相当于Array<Length, char>,只是经常使用,所以单独写成一个类
template<int Length>
class MyString
{
public:
	// 不使用MyString() = default; 因为这样仍然会允许使用初始化列表
	MyString() {}
	// 为了方便查代码,将返回指针的函数命名为具体的名字
	const char* str() const				{ return mValue; }
	char* data()						{ return mValue; }
	bool isEmpty() const				{ return mLength == 0; }
	constexpr int getMaxLength() const	{ return Length; }
	int length() const					{ return mLength; }
	void truncate(int length)
	{
		mLength = length;
		mValue[mLength] = '\0';
	}
	void clear()						
	{
		mLength = 0; 
		mValue[0] = '\0'; 
	}
	constexpr const char& operator[](int index) const
	{
		if (index < 0 || index >= Length)
		{
			ERROR("数组越界");
			index = Length - 1;
		}
		return mValue[index];
	}
	// 将数组的内容重置为0
	void zero()
	{
		memset(mValue, 0, sizeof(char) * Length);
		mLength = 0;
	}
	void set(const char* str, int strLength)
	{
		mLength = strLength;
		if (mLength >= Length)
		{
			mLength = Length - 1;
		}
		copy(str, mLength);
		mValue[mLength] = '\0';
	}
	void set(const char* str)
	{
		mLength = (int)::strlen(str);
		if (mLength >= Length)
		{
			mLength = Length - 1;
		}
		copy(str, mLength);
		mValue[mLength] = '\0';
	}
	void set(const string& str)
	{
		mLength = (int)str.length();
		copy(str.c_str(), mLength);
		mValue[mLength] = '\0';
	}
	void set(const string& str, const int length)
	{
		mLength = length;
		copy(str.c_str(), mLength);
		mValue[mLength] = '\0';
	}
	void set(const char c)
	{
		mLength = 1;
		mValue[0] = c;
		mValue[mLength] = '\0';
	}
	bool add(const string& str, int length)
	{
		if (length == 0)
		{
			return true;
		}
		if (mLength + length >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str, length);
		mLength += length;
		mValue[mLength] = '\0';
		return true;
	}
	bool add(const string& str)
	{
		const int len = (int)str.length();
		if (len == 0)
		{
			return true;
		}
		if (mLength + len >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str);
		mLength += len;
		mValue[mLength] = '\0';
		return true;
	}
	bool add(const char* str)
	{
		if (str == nullptr)
		{
			return true;
		}
		const int len = (int)::strlen(str);
		if (len == 0)
		{
			return true;
		}
		if (mLength + len >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str, len);
		mLength += len;
		mValue[mLength] = '\0';
		return true;
	}
	bool add(const char* str0, const char* str1)
	{
		if (str0 == nullptr || str1 == nullptr)
		{
			return true;
		}
		const int len0 = (int)::strlen(str0);
		const int len1 = (int)::strlen(str1);
		if (len0 + len1 == 0)
		{
			return true;
		}
		if (mLength + len0 + len1 >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str0, len0);
		mLength += len0;
		copy(mLength, str1, len1);
		mLength += len1;
		mValue[mLength] = '\0';
		return true;
	}
	bool add(const char* str0, const char* str1, const char* str2)
	{
		if (str0 == nullptr || str1 == nullptr || str2 == nullptr)
		{
			return true;
		}
		const int len0 = (int)::strlen(str0);
		const int len1 = (int)::strlen(str1);
		const int len2 = (int)::strlen(str2);
		if (len0 + len1 + len2 == 0)
		{
			return true;
		}
		if (mLength + len0 + len1 + len2 >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str0, len0);
		mLength += len0;
		copy(mLength, str1, len1);
		mLength += len1;
		copy(mLength, str2, len2);
		mLength += len2;
		mValue[mLength] = '\0';
		return true;
	}
	bool add(const char* str0, const char* str1, const char* str2, const char* str3)
	{
		if (str0 == nullptr || str1 == nullptr || str2 == nullptr || str3 == nullptr)
		{
			return true;
		}
		const int len0 = (int)::strlen(str0);
		const int len1 = (int)::strlen(str1);
		const int len2 = (int)::strlen(str2);
		const int len3 = (int)::strlen(str3);
		if (len0 + len1 + len2 + len3 == 0)
		{
			return true;
		}
		if (mLength + len0 + len1 + len2 + len3 >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str0, len0);
		mLength += len0;
		copy(mLength, str1, len1);
		mLength += len1;
		copy(mLength, str2, len2);
		mLength += len2;
		copy(mLength, str3, len3);
		mLength += len3;
		mValue[mLength] = '\0';
		return true;
	}
	bool add(const char* str0, const char* str1, const char* str2, const char* str3, const char* str4)
	{
		if (str0 == nullptr || str1 == nullptr || str2 == nullptr || str3 == nullptr || str4 == nullptr)
		{
			return true;
		}
		const int len0 = (int)::strlen(str0);
		const int len1 = (int)::strlen(str1);
		const int len2 = (int)::strlen(str2);
		const int len3 = (int)::strlen(str3);
		const int len4 = (int)::strlen(str4);
		if (len0 + len1 + len2 + len3 + len4 == 0)
		{
			return true;
		}
		if (mLength + len0 + len1 + len2 + len3 + len4 >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str0, len0);
		mLength += len0;
		copy(mLength, str1, len1);
		mLength += len1;
		copy(mLength, str2, len2);
		mLength += len2;
		copy(mLength, str3, len3);
		mLength += len3;
		copy(mLength, str4, len4);
		mLength += len4;
		mValue[mLength] = '\0';
		return true;
	}
	bool add(const char* str0, const char* str1, const char* str2, const char* str3, const char* str4, const char* str5)
	{
		if (str0 == nullptr || str1 == nullptr || str2 == nullptr || str3 == nullptr || str4 == nullptr || str5 == nullptr)
		{
			return true;
		}
		const int len0 = (int)::strlen(str0);
		const int len1 = (int)::strlen(str1);
		const int len2 = (int)::strlen(str2);
		const int len3 = (int)::strlen(str3);
		const int len4 = (int)::strlen(str4);
		const int len5 = (int)::strlen(str5);
		if (len0 + len1 + len2 + len3 + len4 + len5 == 0)
		{
			return true;
		}
		if (mLength + len0 + len1 + len2 + len3 + len4 + len5 >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str0, len0);
		mLength += len0;
		copy(mLength, str1, len1);
		mLength += len1;
		copy(mLength, str2, len2);
		mLength += len2;
		copy(mLength, str3, len3);
		mLength += len3;
		copy(mLength, str4, len4);
		mLength += len4;
		copy(mLength, str5, len5);
		mLength += len5;
		mValue[mLength] = '\0';
		return true;
	}
	bool add(const char* str0, const char* str1, const char* str2, const char* str3, const char* str4, const char* str5, const char* str6)
	{
		if (str0 == nullptr || str1 == nullptr || str2 == nullptr || str3 == nullptr || str4 == nullptr || str5 == nullptr || str6 == nullptr)
		{
			return true;
		}
		const int len0 = (int)::strlen(str0);
		const int len1 = (int)::strlen(str1);
		const int len2 = (int)::strlen(str2);
		const int len3 = (int)::strlen(str3);
		const int len4 = (int)::strlen(str4);
		const int len5 = (int)::strlen(str5);
		const int len6 = (int)::strlen(str6);
		if (len0 + len1 + len2 + len3 + len4 + len5 + len6 == 0)
		{
			return true;
		}
		if (mLength + len0 + len1 + len2 + len3 + len4 + len5 + len6 >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str0, len0);
		mLength += len0;
		copy(mLength, str1, len1);
		mLength += len1;
		copy(mLength, str2, len2);
		mLength += len2;
		copy(mLength, str3, len3);
		mLength += len3;
		copy(mLength, str4, len4);
		mLength += len4;
		copy(mLength, str5, len5);
		mLength += len5;
		copy(mLength, str6, len6);
		mLength += len6;
		mValue[mLength] = '\0';
		return true;
	}
	void add(const char** sourceArray, const int sourceCount)
	{
		FOR(sourceCount)
		{
			add(sourceArray[i]);
		}
	}
	template<int SourceLength>
	void add(const Array<SourceLength, const char*>& sourceArray)
	{
		for (const char* curSource : sourceArray)
		{
			add(curSource);
		}
	}
	bool add(const char* str, const int length)
	{
		if (str == nullptr)
		{
			return true;
		}
		if (length == 0)
		{
			return true;
		}
		if (mLength + length >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		copy(mLength, str, length);
		mLength += length;
		mValue[mLength] = '\0';
		return true;
	}
	bool add(const char c)
	{
		if (mLength + 1 >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		mValue[mLength++] = c;
		mValue[mLength] = '\0';
		return true;
	}
	bool addCount(const char c, const int count)
	{
		if (mLength + count >= Length)
		{
			LOG("添加的字符串太长");
			return false;
		}
		FOR(count)
		{
			mValue[mLength++] = c;
		}
		mValue[mLength] = '\0';
		return true;
	}
	bool setCount(const char c, const int count)
	{
		clear();
		return addCount(c, count);
	}
	void sprintf(const char* format, ...)
	{
		if (format == nullptr)
		{
			return;
		}

		va_list args;
		va_start(args, format);
		const int ret = vsnprintf(mValue, Length, format, args);
		va_end(args);
		if (ret < 0)
		{
			clear();
		}
		else
		{
			mLength = ret >= Length ? Length - 1 : ret;
		}
	}
protected:
	void copy(const char* src) const
	{
		const int length = (int)::strlen(src);
		if (length > Length)
		{
			LOG("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, Length, src, ::strlen(src));
	}
	void copy(const char* src, const int copyCount) const
	{
		if (copyCount > Length)
		{
			LOG("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, Length, src, copyCount);
	}
	void copy(const string& src, const int copyCount) const
	{
		if (copyCount > Length)
		{
			LOG("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, Length, src.c_str(), copyCount);
	}
	void copy(const string& src, const int srcOffset, const int copyCount) const
	{
		if (copyCount > Length)
		{
			LOG("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, Length, src.c_str() + srcOffset, copyCount);
	}
	void copy(const int destOffset, const char* src, const int copyCount)
	{
		if (copyCount + destOffset > Length)
		{
			LOG("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue + destOffset, Length - destOffset, src, copyCount);
	}
	void copy(const int destOffset, const string& src)
	{
		if (destOffset + src.length() > Length)
		{
			LOG("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue + destOffset, Length - destOffset, src.c_str(), src.length());
	}
	void copy(const int destOffset, const string& src, const int copyLength)
	{
		if (destOffset + copyLength > Length)
		{
			LOG("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue + destOffset, Length - destOffset, src.c_str(), copyLength);
	}
protected:
	char mValue[Length]{};
	int mLength = 0;
};