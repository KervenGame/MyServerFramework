#pragma once

#include "TypeUtility.h"
#include "ThreadLock.h"

namespace BinaryUtility
{
	// constexpr的定义只能写在头文件中,否则在其他地方使用时会提示计算结果不是常量,无法使定义结果为constexpr
	// 不过将定义写到头文件中带来的影响就是每个cpp文件中的常量地址是不一样的

	// 下标是sizeof(T),value是此T类型的bit长度所需要的长度表示
	// 比如1个char的取值范围是-127~127,去除符号以后是0~127,127占7个bit,可以使用3个bit存储这个7,所以长度位就是用3个bit来表示,所以下标1的值是3
	constexpr byte SIGNED_LENGTH_MAX_BIT[9]{0, 3, 4, 0, 5, 0, 0, 0, 6};
	// 下标是sizeof(T),value是此T类型的bit长度所需要的长度表示
	// 比如1个byte的取值范围是0~255,255占8个bit,原本8的二进制是4个bit(1000),但是大多数情况下几乎不会超过127,所以大多数时候最多到7,也就是(111)即可,所以只用3个bit来表示,所以下标1(也就是1byte)对应的值是3(表示长度位固定为3bit)
	// 但是也不能不考虑超过127的情况,所以当长度位是7(111)时,就直接读取8个bit的数据位
	// 其作用于mBitCountTable类似,只不过不需要计算
	constexpr byte UNSIGNED_LENGTH_MAX_BIT[9]{ 0, 3, 4, 0, 5, 0, 0, 0, 6 };
	// 0到65535的每个数中的最高位1的下标,也就是需要用几个bit来表示,1可以用1个bit表示,5可以使用3个bit表示,8可以使用5个bit表示
	MICRO_LEGEND_FRAME_API extern byte mBitCountTable[65536];
	MICRO_LEGEND_FRAME_API extern ThreadLock mBitCountTableLock;
	MICRO_LEGEND_FRAME_API extern const ushort crc16_table[256];
	// 各个基础数据类型的类型hash值
	MICRO_LEGEND_FRAME_API extern const int mStringType;
	MICRO_LEGEND_FRAME_API extern const int mBoolType;
	MICRO_LEGEND_FRAME_API extern const int mCharType;
	MICRO_LEGEND_FRAME_API extern const int mWCharType;
	MICRO_LEGEND_FRAME_API extern const int mByteType;
	MICRO_LEGEND_FRAME_API extern const int mShortType;
	MICRO_LEGEND_FRAME_API extern const int mUShortType;
	MICRO_LEGEND_FRAME_API extern const int mIntType;
	MICRO_LEGEND_FRAME_API extern const int mUIntType;
	MICRO_LEGEND_FRAME_API extern const int mFloatType;
	MICRO_LEGEND_FRAME_API extern const int mLLongType;
	MICRO_LEGEND_FRAME_API extern const int mULLongType;
	MICRO_LEGEND_FRAME_API extern const int mBoolPtrType;
	MICRO_LEGEND_FRAME_API extern const int mCharPtrType;
	MICRO_LEGEND_FRAME_API extern const int mBytePtrType;
	MICRO_LEGEND_FRAME_API extern const int mShortPtrType;
	MICRO_LEGEND_FRAME_API extern const int mUShortPtrType;
	MICRO_LEGEND_FRAME_API extern const int mIntPtrType;
	MICRO_LEGEND_FRAME_API extern const int mUIntPtrType;
	MICRO_LEGEND_FRAME_API extern const int mFloatPtrType;
	MICRO_LEGEND_FRAME_API extern const int mLLongPtrType;
	MICRO_LEGEND_FRAME_API extern const int mULLongPtrType;
	MICRO_LEGEND_FRAME_API extern const int mBoolListType;
	MICRO_LEGEND_FRAME_API extern const int mCharListType;
	MICRO_LEGEND_FRAME_API extern const int mByteListType;
	MICRO_LEGEND_FRAME_API extern const int mShortListType;
	MICRO_LEGEND_FRAME_API extern const int mUShortListType;
	MICRO_LEGEND_FRAME_API extern const int mIntListType;
	MICRO_LEGEND_FRAME_API extern const int mUIntListType;
	MICRO_LEGEND_FRAME_API extern const int mFloatListType;
	MICRO_LEGEND_FRAME_API extern const int mLLongListType;
	MICRO_LEGEND_FRAME_API extern const int mULLongListType;
	MICRO_LEGEND_FRAME_API extern const int mVector2Type;
	MICRO_LEGEND_FRAME_API extern const int mVector2IntType;
	MICRO_LEGEND_FRAME_API extern const int mVector2UIntType;
	MICRO_LEGEND_FRAME_API extern const int mVector2ShortType;
	MICRO_LEGEND_FRAME_API extern const int mVector2UShortType;
	MICRO_LEGEND_FRAME_API extern const int mVector3Type;
	MICRO_LEGEND_FRAME_API extern const int mVector3IntType;
	MICRO_LEGEND_FRAME_API extern const int mVector2ListType;
	MICRO_LEGEND_FRAME_API extern const int mVector2IntListType;
	MICRO_LEGEND_FRAME_API extern const int mVector3ListType;
	MICRO_LEGEND_FRAME_API extern const int mVector3IntListType;

	//------------------------------------------------------------------------------------------------------------------------------
	// protected
	void MICRO_LEGEND_FRAME_API initBitCountTable();
	byte MICRO_LEGEND_FRAME_API internalGenerateBitCount(ushort value);
	//------------------------------------------------------------------------------------------------------------------------------
	// public
	// 因为当前类中需要使用过,如果写到FrameUtility中,会有头文件互相包含的问题,所以即使下面的函数于二进制操作无关,也写到这里
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	T getListMax(const Vector<T>& list)
	{
		const int count = list.size();
		if (count == 0)
		{
			return 0;
		}

		T maxValue = list[0];
		for (int i = 1; i < count; ++i)
		{
			if (maxValue < list[i])
			{
				maxValue = list[i];
			}
		}
		return maxValue;
	}
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	T getListMax(const T* list, const int count)
	{
		if (count == 0)
		{
			return 0;
		}

		T maxValue = list[0];
		for (int i = 1; i < count; ++i)
		{
			if (maxValue < list[i])
			{
				maxValue = list[i];
			}
		}
		return maxValue;
	}
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	T getListMaxAbs(const Vector<T>& list)
	{
		const int count = list.size();
		if (count == 0)
		{
			return 0;
		}

		T maxAbsValue = list[0] > 0 ? list[0] : -list[0];
		for (int i = 1; i < count; ++i)
		{
			T absValue = list[i] > 0 ? list[i] : -list[i];
			if (maxAbsValue < absValue)
			{
				maxAbsValue = absValue;
			}
		}
		return maxAbsValue;
	}
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	T getListMaxAbs(const T* list, const int count)
	{
		if (count == 0)
		{
			return 0;
		}

		T maxAbsValue = list[0] > 0 ? list[0] : -list[0];
		for (int i = 1; i < count; ++i)
		{
			T absValue = list[i] > 0 ? list[i] : -list[i];
			if (maxAbsValue < absValue)
			{
				maxAbsValue = absValue;
			}
		}
		return maxAbsValue;
	}
	float MICRO_LEGEND_FRAME_API getFloatListMaxAbs(const float* list, int count);
	double MICRO_LEGEND_FRAME_API getDoubleListMaxAbs(const double* list, int count);
	// 设置value的指定位置pos的字节的值为byte,并且不影响其他字节
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setByte(T& value, const byte b, const int pos) { value = (value & ~(0xFFLL << (8 * pos))) | ((T)b << (8 * pos)); }
	// 获得value的指定位置pos的字节的值
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr byte getByte(T value, const int pos) { return ((value) & (0xFFLL << (8 * pos))) >> (8 * pos); }
	// 指定下标的位是否为1
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr bool hasBit(T value, const int pos) { return (value & ((T)1 << pos)) != 0; }
	// 获取指定位的值，返回 0 或 1
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr int getBit(T value, const int pos) { return (int)(((typename std::make_unsigned<T>::type)value >> pos) & 1); }
	// 设置指定位的值
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setBit(T& value, const int pos, const int bit)
	{
		if (bit != 0)
		{
			value |= (T)1 << pos;
		}
		else
		{
			value &= ~((T)1 << pos);
		}
	}
	// 注意,负数由于存储是使用补码存储的,所以不能直接按位或
	// 将指定下标的位设置为1
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setBitOne(T& value, const int pos) { value |= (T)1 << pos; }
	// 将指定下标的位设置为0
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setBitZero(T& value, const int pos) { value &= ~((T)1 << pos); }
	// 获取最高位
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr int getHighestBit(T value) { return (int)(((typename std::make_unsigned<T>::type)value) >> (sizeof(T) * 8 - 1)); }
	// 设置最高位
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setHighestBit(T& value, const bool bit)
	{
		if (bit)
		{
			value |= (T)1 << (sizeof(T) * 8 - 1);
		}
		else
		{
			value &= ~((T)1 << (sizeof(T) * 8 - 1));
		}
	}
	// 获取最低位
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr int getLowestBit(T value) { return value & (T)1; }
	// 设置最低位
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setLowestBit(T& value, const bool bit)
	{
		if (bit)
		{
			value |= (T)1;
		}
		else
		{
			value &= ~(T)1;
		}
	}
	// 将value中截取从start开始的count个位的值
	constexpr byte getBits(const byte value, const byte start, const byte count) { return (value & (((1 << count) - 1) << start)) >> start; }
	// 将位的数量转换为字节的数量,比如9个bit就是占2个字节
	constexpr int bitCountToByteCount(const int bitCount) { return (bitCount & 7) != 0 ? (bitCount >> 3) + 1 : bitCount >> 3; }
	inline void setBufferBitOne(char* buffer, const int bitIndex) { setBitOne(buffer[bitIndex >> 3], bitIndex & 7); }
	inline bool getBufferBit(const char* buffer, const int bitIndex) { return hasBit(buffer[bitIndex >> 3], bitIndex & 7); }
	// 将sourceValue中sourceStart开始的writeBitCount个位写入到destValue从destStart开始的位下标上
	inline void copyBits(byte& destValue, const byte destStart, const byte sourceValue, const byte sourceStart, const byte writeBitCount)
	{
		destValue |= (getBits(sourceValue, sourceStart, writeBitCount) << destStart);
	}
	inline void copyBits(char& destValue, const byte destStart, const byte sourceValue, const byte sourceStart, const byte writeBitCount)
	{
		destValue |= (getBits(sourceValue, sourceStart, writeBitCount) << destStart);
	}
	constexpr void copyBits(byte& destValue, const byte destStart, const byte sourceValue) { destValue |= (sourceValue << destStart); }
	constexpr void copyBits(char& destValue, const byte destStart, const byte sourceValue) { destValue |= (sourceValue << destStart); }
	void logErrorInternal(string&& info);
	// 计算有效位数,也就是最高位1的下标+1,比如输入8,返回值就是4,意义是可以使用4个bit就可以表示数字8
	inline byte generateBitCount(const byte value)
	{
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		return mBitCountTable[value];
	}
	inline byte generateBitCount(const ushort value)
	{
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		return mBitCountTable[value];
	}
	inline byte generateBitCount(const uint value)
	{
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		const ushort part1 = (ushort)((value & 0xFFFF0000) >> 16);
		return part1 > 0 ? mBitCountTable[part1] + 16 : mBitCountTable[value & 0x0000FFFF];
	}
	inline byte generateBitCount(const ullong value)
	{
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		if ((value & 0xFFFFFFFF00000000) > 0)
		{
			const ushort part3 = (ushort)((value & 0xFFFF000000000000) >> 48);
			if (part3 > 0)
			{
				return mBitCountTable[part3] + 16 * 3;
			}
			return mBitCountTable[(ushort)((value & 0x0000FFFF00000000) >> 32)] + 16 * 2;
		}
		else
		{
			const ushort part1 = (ushort)((value & 0x00000000FFFF0000) >> 16);
			return part1 > 0 ? mBitCountTable[part1] + 16 * 1 : mBitCountTable[value & 0x000000000000FFFF];
		}
	}
	// 从buffer的bitIndex的位下标处读取writeBitCount个位,位数最多不超过8位
	inline void readByteBits(const char* buffer, int& bitIndex, byte& value, const byte readBitCount)
	{
		value = ((*((short*)(buffer + (bitIndex >> 3)))) >> (bitIndex & 7)) & ((1 << readBitCount) - 1);
		bitIndex += readBitCount;
	}
	// 读取长度位
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsignedLengthBit(const char* buffer, const int bufferSize, int& bitIndex, byte& bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		readByteBits(buffer, bitIndex, bitCount, TYPE_LENGTH_MAX_BIT);
		// 因为长度位的存储会忽略一个bit,所以当长度位的bit数达到此类型的最大时,会加1
		// 比如byte类型为8bit,长度位存储只用了3bit,只能表示0到7,所以当读取到的长度位值为7时,就需要读8bit的数据位,不然就会丢失数据
		if (bitCount == (1 << TYPE_LENGTH_MAX_BIT) - 1)
		{
			++bitCount;
		}
		return true;
	}
	// 读取长度位
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSignedLengthBit(const char* buffer, const int bufferSize, int& bitIndex, byte& bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		readByteBits(buffer, bitIndex, bitCount, TYPE_LENGTH_MAX_BIT);
		return true;
	}
	// 将一个字节的前writeBitCount个位写入到buffer的bitIndex的位下标处
	inline void writeByteBits(char* buffer, int& bitIndex, const byte value, const byte writeBitCount)
	{
		*((short*)(buffer + (bitIndex >> 3))) |= ((short)value & ((1 << writeBitCount) - 1)) << (bitIndex & 7);
		bitIndex += writeBitCount;
	}
	// 写入无符号整数长度位
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedLengthBit(char* buffer, const int bufferSize, int& bitIndex, byte& bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		// 因为长度位的存储会忽略一个bit,所以当长度位的bit数达到此类型的最大时,会加1
		// 比如byte类型为8bit,长度位存储只用了3bit,只能表示0到7,所以当需要写入的长度位值为8时,就只写7进去,但是实际写入数据时仍然是写入8bit
		// 但是如果写入的长度位值为7,则长度位写入7,数据位仍然是写入8bit
		byte writeBitCount = bitCount;
		if (bitCount == 1 << TYPE_LENGTH_MAX_BIT)
		{
			writeBitCount = bitCount - 1;
		}
		else if (bitCount == (1 << TYPE_LENGTH_MAX_BIT) - 1)
		{
			writeBitCount = bitCount;
			++bitCount;
		}
		writeByteBits(buffer, bitIndex, writeBitCount, TYPE_LENGTH_MAX_BIT);
		return true;
	}
	// 写入有符号整数长度位
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedLengthBit(char* buffer, const int bufferSize, int& bitIndex, const byte bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		// 先写入TYPE_LENGTH_MAX_BIT个bit
		writeByteBits(buffer, bitIndex, bitCount, TYPE_LENGTH_MAX_BIT);
		return true;
	}
	ushort MICRO_LEGEND_FRAME_API crc16(ushort crc, const char* buffer, int len, int bufferOffset = 0);
	inline ushort crc16Byte(const ushort crc, const byte data) { return (ushort)((crc >> 8) ^ crc16_table[(crc ^ data) & 0xFF]); }
	template<typename T>
	void zeroArray(T* ptr, const int size) { memset(ptr, 0, sizeof(T) * size); }
	bool MICRO_LEGEND_FRAME_API readBuffer(const char* buffer, int bufferSize, int& index, char* dest, int destSize, int readSize);
	bool MICRO_LEGEND_FRAME_API writeBuffer(char* buffer, int bufferSize, int& destOffset, const char* source, int writeSize);
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool read(const char* buffer, const int bufferSize, int& index, T& value)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return false;
		}
		value = *((T*)(buffer + index));
		index += valueSize;
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr T read(const char* buffer, const int bufferSize, int& index)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return 0;
		}
		const T value = *((T*)(buffer + index));
		index += valueSize;
		return value;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr T readInverse(const char* buffer, const int bufferSize, int& index)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return 0;
		}
		T finalValue = (T)0;
		char* ptr = (char*)&finalValue;
		FOR_INVERSE(valueSize)
		{
			ptr[i] = buffer[index++];
		}
		return finalValue;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool readInverse(const char* buffer, const int bufferSize, int& index, T& value)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return false;
		}
		char* ptr = (char*)&value;
		FOR_INVERSE(valueSize)
		{
			ptr[i] = buffer[index++];
		}
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr void readArray(const char* buffer, const int bufferSize, int& index, T* dest, const int arrayLength)
	{
		FOR(arrayLength)
		{
			dest[i] = read<T>(buffer, bufferSize, index);
		}
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr void readArrayInverse(const char* buffer, const int bufferSize, int& index, T* dest, const int arrayLength)
	{
		FOR(arrayLength)
		{
			dest[i] = readInverse<T>(buffer, bufferSize, index);
		}
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool write(char* buffer, const int bufferSize, int& index, const T value)
	{
		constexpr int writeSize = sizeof(T);
		if (bufferSize < index + writeSize)
		{
			return false;
		}
		*((T*)(buffer + index)) = value;
		index += writeSize;
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool writeInverse(char* buffer, const int bufferSize, int& index, const T value)
	{
		constexpr int writeSize = sizeof(T);
		if (bufferSize < index + writeSize)
		{
			return false;
		}
		const char* ptr = (char*)&value;
		FOR_INVERSE(writeSize)
		{
			buffer[index++] = ptr[i];
		}
		return true;
	}
	inline bool readBoolBit(const char* buffer, const int bufferSize, int& bitIndex, bool& value)
	{
		// bool固定读取1位
		if (bitCountToByteCount(bitIndex + 1) > bufferSize)
		{
			return false;
		}
		value = getBufferBit(buffer, bitIndex++);
		return true;
	}
	bool MICRO_LEGEND_FRAME_API readBufferBit(const char* buffer, int bufferSize, int& bitIndex, char* dest, int destSize, int readSize);
	// 可以按位读取带符号的整数
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSignedIntegerBit(const char* buffer, const int bufferSize, int& bitIndex, T& value, bool needReadSign)
	{
		value = 0;
		// 读取长度位
		byte bitCount = 0;
		if (!readSignedLengthBit<T>(buffer, bufferSize, bitIndex, bitCount))
		{
			return false;
		}
		if (bitCount == 0)
		{
			return true;
		}
		--bitCount;
		if (bitCountToByteCount(bitIndex + bitCount + (needReadSign ? 1 : 0)) > bufferSize)
		{
			return false;
		}

		// 读符号位
		bool isNegative = false;
		if (needReadSign)
		{
			isNegative = getBufferBit(buffer, bitIndex++);
		}

		// 读取值
		if (bitCount > 0)
		{
			const int byteCount = bitCountToByteCount(bitCount);
			FOR(byteCount - 1)
			{
				readByteBits(buffer, bitIndex, ((byte*)&value)[i], 8);
			}
			readByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], bitCount - ((byteCount - 1) << 3));
			// 注意,负数由于存储是使用补码存储的,所以不能直接按位或
			// 读取完所有位再将最高位的1加上
			setBitOne(value, bitCount);
			if (isNegative)
			{
				value = -value;
			}
		}
		else
		{
			value = isNegative ? -1 : 1;
		}
		return true;
	}
	// 可以按位读取无符号的整数
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsignedIntegerBit(const char* buffer, const int bufferSize, int& bitIndex, T& value)
	{
		value = 0;
		// 读取长度位
		byte bitCount = 0;
		if (!readUnsignedLengthBit<T>(buffer, bufferSize, bitIndex, bitCount))
		{
			return false;
		}
		if (bitCount == 0)
		{
			return true;
		}
		// 因为写入的时候,最高位的1是不需要写入的,所以实际读取的位数会少一位
		if (bitCount < (1 << sizeof(value)) - 1)
		{
			--bitCount;
		}
		if (bitCountToByteCount(bitIndex + bitCount) > bufferSize)
		{
			return false;
		}

		// 读取值
		if (bitCount > 0)
		{
			const int byteCount = bitCountToByteCount(bitCount);
			FOR(byteCount - 1)
			{
				readByteBits(buffer, bitIndex, ((byte*)&value)[i], 8);
			}
			readByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], bitCount - ((byteCount - 1) << 3));
			// 读取完所有位再将最高位的1加上
			// 但是如果写入位的数量到达最大位数时,就不能再加上最高位了,否则会混淆
			if (bitCount < (1 << sizeof(value)) - 1)
			{
				setBitOne(value, bitCount);
			}
		}
		else
		{
			value = 1;
		}
		return true;
	}
	bool MICRO_LEGEND_FRAME_API readFloatBit(const char* buffer, int bufferSize, int& bitIndex, float& value, bool needReadSign, const int precision = 3);
	bool MICRO_LEGEND_FRAME_API readDoubleBit(const char* buffer, int bufferSize, int& bitIndex, double& value, bool needReadSign, const int precision = 4);
	template<typename T, int Count, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, T*>& list, const bool needReadSign)
	{
		// 使用统一的长度位
		if (getBufferBit(buffer, bitIndex++))
		{
			// 读取长度位
			byte bitCount = 0;
			if (!readSignedLengthBit<T>(buffer, bufferSize, bitIndex, bitCount))
			{
				return false;
			}
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + ((needReadSign ? 1 : 0) + bitCount) * Count) > bufferSize)
			{
				return false;
			}
			// 读取所有元素位
			const int byteCount = bitCountToByteCount(bitCount);
			FOR(Count)
			{
				T& value = *list[i];
				value = 0;
				// 读符号位
				bool isNegative = false;
				if (needReadSign)
				{
					isNegative = getBufferBit(buffer, bitIndex++);
				}
				// 读取值
				FOR_J(byteCount - 1)
				{
					readByteBits(buffer, bitIndex, ((byte*)&value)[j], 8);
				}
				readByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], bitCount - ((byteCount - 1) << 3));
				if (isNegative)
				{
					value = -value;
				}
			}
		}
		// 每个元素使用独立的长度位
		else
		{
			FOR(Count)
			{
				readSignedIntegerBit(buffer, bufferSize, bitIndex, *list[i], needReadSign);
			}
		}
		return true;
	}
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Vector<T>& list, const bool needReadSign)
	{
		// 读元素个数位
		ushort count = 0;
		if (!readUnsignedIntegerBit(buffer, bufferSize, bitIndex, count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}

		list.clear();
		list.resize(count);

		// 使用统一的长度位
		if (getBufferBit(buffer, bitIndex++))
		{
			// 读取长度位
			byte bitCount = 0;
			if (!readSignedLengthBit<T>(buffer, bufferSize, bitIndex, bitCount))
			{
				return false;
			}
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + ((needReadSign ? 1 : 0) + bitCount) * count) > bufferSize)
			{
				return false;
			}
			// 读取所有元素位
			const int byteCount = bitCountToByteCount(bitCount);
			FOR(count)
			{
				T& value = list[i];
				value = 0;
				// 读符号位
				bool isNegative = false;
				if (needReadSign)
				{
					isNegative = getBufferBit(buffer, bitIndex++);
				}
				// 读取值
				FOR_J(byteCount - 1)
				{
					readByteBits(buffer, bitIndex, ((byte*)&value)[j], 8);
				}
				readByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], bitCount - ((byteCount - 1) << 3));
				if (isNegative)
				{
					value = -value;
				}
			}
		}
		// 每个元素使用独立的长度位
		else
		{
			FOR(count)
			{
				readSignedIntegerBit(buffer, bufferSize, bitIndex, list[i], needReadSign);
			}
		}
		return true;
	}
	template<typename T, int Count, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, T*>& list)
	{
		// 使用统一的长度位
		if (getBufferBit(buffer, bitIndex++))
		{
			// 读取长度位
			byte bitCount = 0;
			if (!readUnsignedLengthBit<T>(buffer, bufferSize, bitIndex, bitCount))
			{
				return false;
			}
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + bitCount * Count) > bufferSize)
			{
				return false;
			}
			// 读取所有元素位
			const int byteCount = bitCountToByteCount(bitCount);
			FOR(Count)
			{
				T& value = *list[i];
				value = 0;
				// 读取值
				FOR_J(byteCount - 1)
				{
					readByteBits(buffer, bitIndex, ((byte*)&value)[j], 8);
				}
				readByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], bitCount - ((byteCount - 1) << 3));
			}
		}
		// 每个元素使用独立的长度位
		else
		{
			FOR(Count)
			{
				readUnsignedIntegerBit(buffer, bufferSize, bitIndex, *list[i]);
			}
		}
		return true;
	}
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Vector<T>& list)
	{
		// 读元素个数位
		ushort count = 0;
		if (!readUnsignedIntegerBit(buffer, bufferSize, bitIndex, count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}

		list.clear();
		list.resize(count);

		// 使用统一的长度位
		if (getBufferBit(buffer, bitIndex++))
		{
			// 读取长度位
			byte bitCount = 0;
			if (!readUnsignedLengthBit<T>(buffer, bufferSize, bitIndex, bitCount))
			{
				return false;
			}
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + bitCount * count) > bufferSize)
			{
				return false;
			}
			// 读取所有元素位
			const int byteCount = bitCountToByteCount(bitCount);
			FOR(count)
			{
				T& value = list[i];
				value = 0;
				// 读取值
				FOR_J(byteCount - 1)
				{
					readByteBits(buffer, bitIndex, ((byte*)&value)[j], 8);
				}
				readByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], bitCount - ((byteCount - 1) << 3));
			}
		}
		// 每个元素使用独立的长度位
		else
		{
			FOR(count)
			{
				readUnsignedIntegerBit(buffer, bufferSize, bitIndex, list[i]);
			}
		}
		return true;
	}
	bool MICRO_LEGEND_FRAME_API readFloatListBit(const char* buffer, int bufferSize, int& bitIndex, Vector<float>& list, bool needReadSign, int precision = 3);
	bool MICRO_LEGEND_FRAME_API readFloatListBit(const char* buffer, int bufferSize, int& bitIndex, float** list, int count, bool needReadSign, int precision = 3);
	template<int Count>
	bool readFloatListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, float*>& list, const bool needReadSign, const int precision = 3)
	{
		return readFloatListBit(buffer, bufferSize, bitIndex, list.data(), Count, needReadSign, precision);
	}
	bool MICRO_LEGEND_FRAME_API readDoubleListBit(const char* buffer, int bufferSize, int& bitIndex, Vector<double>& list, bool needReadSign, int precision = 4);
	bool MICRO_LEGEND_FRAME_API readDoubleListBit(const char* buffer, int bufferSize, int& bitIndex, double** list, int count, bool needReadSign, int precision = 4);
	template<int Count>
	bool readDoubleListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, double*>& list, const bool needReadSign, const int precision = 4)
	{
		return readDoubleListBit(buffer, bufferSize, bitIndex, list.data(), Count, needReadSign, precision);
	}
	inline bool writeBoolBit(char* buffer, const int bufferSize, int& bitIndex, const bool value)
	{
		if (bitCountToByteCount(bitIndex + 1) > bufferSize)
		{
			return false;
		}
		// 固定只写入1位
		if (value)
		{
			setBufferBitOne(buffer, bitIndex);
		}
		++bitIndex;
		return true;
	}
	void MICRO_LEGEND_FRAME_API fillZeroToByteEnd(char* buffer, int& bitIndex);
	bool MICRO_LEGEND_FRAME_API writeBufferBit(char* buffer, int bufferSize, int& bitIndex, const char* source, int writeSize);
	// 可以按位写入带符号的整数
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedIntegerBit(char* buffer, const int bufferSize, int& bitIndex, T value, const bool needWriteSign)
	{
		const bool isNegative = value < 0;
		if (isNegative)
		{
			value = -value;
		}
		// 需要多少位来存储这个值
		byte bitCount = generateBitCount((typename ToUnsignedInteger<T>::Type)value);
		if ((value != 0 && bitCount == 0) || bitCount > sizeof(value) * 8)
		{
			return false;
		}
		if (!writeSignedLengthBit<T>(buffer, bufferSize, bitIndex, bitCount))
		{
			return false;
		}
		if (value == 0)
		{
			return true;
		}
		if (bitCountToByteCount(bitIndex + 1 + bitCount - 1) > bufferSize)
		{
			return false;
		}

		// 写入符号位
		if (needWriteSign)
		{
			if (isNegative)
			{
				setBufferBitOne(buffer, bitIndex);
			}
			++bitIndex;
		}
		// 将最高位的1去掉,不需要写入
		setBitZero(value, --bitCount);
		if (bitCount > 0)
		{
			// 再写入值的所有位
			// 计算出要写多少个字节,按每个字节去写入
			const byte byteCount = bitCountToByteCount(bitCount);
			FOR(byteCount - 1)
			{
				writeByteBits(buffer, bitIndex, ((byte*)&value)[i], 8);
			}
			writeByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], bitCount - ((byteCount - 1) << 3));
		}
		return true;
	}
	// 可以按位写入无符号的整数
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedIntegerBit(char* buffer, const int bufferSize, int& bitIndex, T value)
	{
		byte bitCount = generateBitCount(value);
		if ((value != 0 && bitCount == 0) || bitCount > sizeof(value) * 8)
		{
			return false;
		}
		if (!writeUnsignedLengthBit<T>(buffer, bufferSize, bitIndex, bitCount))
		{
			return false;
		}
		if (value == 0)
		{
			return true;
		}
		if (bitCountToByteCount(bitIndex + bitCount - 1) > bufferSize)
		{
			return false;
		}
		// 将最高位的1去掉,不需要写入
		// 但是如果写入位的数量到达最大位数时,就不能再去掉最高位了,否则会混淆
		if (bitCount < (1 << sizeof(value)) - 1)
		{
			setBitZero(value, --bitCount);
		}
		if (bitCount > 0)
		{
			// 再写入值的所有位
			// 计算出要写多少个字节,按每个字节去写入
			const byte byteCount = bitCountToByteCount(bitCount);
			FOR(byteCount - 1)
			{
				writeByteBits(buffer, bitIndex, ((byte*)&value)[i], 8);
			}
			writeByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], bitCount - ((byteCount - 1) << 3));
		}
		return true;
	}
	bool MICRO_LEGEND_FRAME_API writeFloatListBit(char* buffer, int bufferSize, int& bitIndex, const float* list, int count, bool needWriteCount, bool needWriteSign, int precision = 3);
	template<int Count>
	bool writeFloatListBit(char* buffer, const int bufferSize, int& bitIndex, const Array<Count, float>& list, const bool needWriteSign, const int precision = 3)
	{
		return writeFloatListBit(buffer, bufferSize, bitIndex, list.data(), Count, false, needWriteSign, precision);
	}
	inline bool writeFloatListBit(char* buffer, const int bufferSize, int& bitIndex, const Vector<float>& list, const bool needWriteSign, const int precision = 3)
	{
		return writeFloatListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), true, needWriteSign, precision);
	}
	bool MICRO_LEGEND_FRAME_API writeDoubleListBit(char* buffer, int bufferSize, int& bitIndex, const double* list, const int count, bool needWriteCount, bool needWriteSign, int precision = 4);
	template<int Count>
	bool writeDoubleListBit(char* buffer, const int bufferSize, int& bitIndex, const Array<Count, double>& list, const bool needWriteSign, const int precision = 4)
	{
		return writeDoubleListBit(buffer, bufferSize, bitIndex, list.data(), Count, false, needWriteSign, precision);
	}
	inline bool writeDoubleListBit(char* buffer, const int bufferSize, int& bitIndex, const Vector<double>& list, const bool needWriteSign, const int precision = 4)
	{
		return writeDoubleListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), true, needWriteSign, precision);
	}
	// 可以按位写入带符号的整数的列表
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const T* list, const int count, const bool needWriteCount, const bool needWriteSign)
	{
		// 写入长度
		if (needWriteCount && !writeUnsignedIntegerBit(buffer, bufferSize, bitIndex, (ushort)count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}

		// 计算出最大值的位数量,所有的值都使用这个位数量来存储
		const byte maxBitCount = generateBitCount((typename ToUnsignedInteger<T>::Type)getListMaxAbs(list, count));
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		// 如果使用一个统一的位数来表示写入位个数所占用的总位数
		int bitCountUnity;
		if (maxBitCount > 0)
		{
			if (needWriteSign)
			{
				bitCountUnity = (maxBitCount + 1) * count + TYPE_LENGTH_MAX_BIT;
			}
			else
			{
				bitCountUnity = maxBitCount * count + TYPE_LENGTH_MAX_BIT;
			}
		}
		else
		{
			bitCountUnity = TYPE_LENGTH_MAX_BIT;
		}
		// 如果每个值都使用自己的实际位数来表示写入位个数所占用的总位数
		// 先加上每个元素的长度位的位数量
		int bitCountSingle = TYPE_LENGTH_MAX_BIT * count;
		// 写入独立长度所占空间小于统一个数时才会继续计算值所占的空间,如果长度部分已经大于了,则不需要再继续计算了
		if (bitCountSingle < bitCountUnity)
		{
			FOR(count)
			{
				T thisAbsValue = abs(list[i]);
				// 值为0时不会写入符号位和数据,不为0时才会写入
				if (thisAbsValue > 0)
				{
					// 每个元素绝对值所占用的位数,最高位固定是1,所以减去1位
					bitCountSingle += generateBitCount((typename ToUnsignedInteger<T>::Type)thisAbsValue) - 1;
					// 还要加上一个符号位
					if (needWriteSign)
					{
						bitCountSingle += 1;
					}
					if (bitCountSingle > bitCountUnity)
					{
						break;
					}
				}
			}
		}

		// 使用统一的长度位占空间更小
		if (bitCountSingle >= bitCountUnity)
		{
			// 写入1表示使用统一的长度位
			setBufferBitOne(buffer, bitIndex++);
			// 先写入长度位
			if (!writeSignedLengthBit<T>(buffer, bufferSize, bitIndex, maxBitCount))
			{
				return false;
			}
			// 长度位为0,则不需要再继续写入
			if (maxBitCount == 0)
			{
				return true;
			}
			int willWriteBitCount;
			if (needWriteSign)
			{
				willWriteBitCount = (maxBitCount + 1) * count;
			}
			else
			{
				willWriteBitCount = maxBitCount * count;
			}
			if (bitCountToByteCount(bitIndex + willWriteBitCount) > bufferSize)
			{
				return false;
			}
			const byte byteCount = bitCountToByteCount(maxBitCount);
			// 再写入所有值的每一位
			FOR(count)
			{
				T value = list[i];
				const bool isNegative = value < 0;
				if (isNegative)
				{
					value = -value;
				}
				// 写入符号位
				if (needWriteSign)
				{
					if (isNegative)
					{
						setBufferBitOne(buffer, bitIndex);
					}
					++bitIndex;	
				}
				// 再写入值的所有位
				FOR_J(byteCount - 1)
				{
					writeByteBits(buffer, bitIndex, ((byte*)&value)[j], 8);
				}
				writeByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], maxBitCount - ((byteCount - 1) << 3));
			}
		}
		// 使用每个元素独立的长度位占用空间更小
		else
		{
			// 写入0表示使用独立的长度位,只是下标跳一位,不会实际写入
			++bitIndex;
			FOR(count)
			{
				writeSignedIntegerBit(buffer, bufferSize, bitIndex, list[i], needWriteSign);
			}
		}
		return true;
	}
	// 可以按位写入带符号的整数的列表,Array类型
	template<typename T, int Count, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const Array<Count, T>& list, const bool needWriteSign)
	{
		return writeSignedIntegerListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), false, needWriteSign);
	}
	// 可以按位写入带符号的整数的列表,Vector类型
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const Vector<T>& list, const bool needWriteSign)
	{
		return writeSignedIntegerListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), true, needWriteSign);
	}
	// 可以按位写入无符号的整数的列表
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const T* list, const int count, const bool needWriteCount)
	{
		// 写入长度
		if (needWriteCount && !writeUnsignedIntegerBit(buffer, bufferSize, bitIndex, (ushort)count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}

		// 计算出最大值的位数量,所有的值都使用这个位数量来存储
		byte maxBitCount = generateBitCount(getListMax(list, count));
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		int bitCountUnity;
		// 这里的判断与writeUnsignedLengthBit中的逻辑有关,writeUnsignedLengthBit会计算出写入到长度位的值,以及实际需要写入的数据位的位数
		if (maxBitCount == (1 << TYPE_LENGTH_MAX_BIT) - 1)
		{
			bitCountUnity = (maxBitCount + 1) * count + TYPE_LENGTH_MAX_BIT;
		}
		else
		{
			bitCountUnity = maxBitCount * count + TYPE_LENGTH_MAX_BIT;
		}
		// 如果使用一个统一的位数来表示写入位个数所占用的总位数
		// 如果每个值都使用自己的实际位数来表示写入位个数所占用的总位数
		// 先加上每个元素的长度位的位数量
		int bitCountSingle = TYPE_LENGTH_MAX_BIT * count;
		// 写入独立长度所占空间小于统一个数时才会继续计算值所占的空间,如果长度部分已经大于了,则不需要再继续计算了
		if (bitCountSingle < bitCountUnity)
		{
			// 每个元素绝对值所占用的位数,最高位固定是1,所以减去1位
			FOR(count)
			{
				T thisValue = list[i];
				if (thisValue > 0)
				{
					byte thisBitCount = generateBitCount(thisValue);
					if (thisBitCount == (1 << TYPE_LENGTH_MAX_BIT) - 1)
					{
						++thisBitCount;
					}
					bitCountSingle += thisBitCount - 1;
					if (bitCountSingle > bitCountUnity)
					{
						break;
					}
				}
			}
		}

		// 使用统一的长度位占空间更小
		if (bitCountSingle >= bitCountUnity)
		{
			// 写入1表示使用统一的长度位
			setBufferBitOne(buffer, bitIndex++);
			// 先写入长度位
			if (!writeUnsignedLengthBit<T>(buffer, bufferSize, bitIndex, maxBitCount))
			{
				return false;
			}
			// 长度位为0,则不需要再继续写入
			if (maxBitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + maxBitCount * count) > bufferSize)
			{
				return false;
			}
			const byte byteCount = bitCountToByteCount(maxBitCount);
			FOR(count)
			{
				const T& value = list[i];
				// 写入值的所有位
				FOR_J(byteCount - 1)
				{
					writeByteBits(buffer, bitIndex, ((byte*)&value)[j], 8);
				}
				writeByteBits(buffer, bitIndex, ((byte*)&value)[byteCount - 1], maxBitCount - ((byteCount - 1) << 3));
			}
		}
		// 使用每个元素独立的长度位占用空间更小
		else
		{
			// 写入0表示使用独立的长度位,只是下标跳一位,不会实际写入
			++bitIndex;
			FOR(count)
			{
				writeUnsignedIntegerBit(buffer, bufferSize, bitIndex, list[i]);
			}
		}
		return true;
	}
	// 可以按位写入无符号的整数的列表,Array类型
	template<typename T, int Count, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const Array<Count, T>& list)
	{
		return writeUnsignedIntegerListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), false);
	}
	// 可以按位写入无符号的整数的列表,Vector类型
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const Vector<T>& list)
	{
		return writeUnsignedIntegerListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), true);
	}
	inline bool writeVector2(char* buffer, const int bufferSize, int& index, const Vector2 value)
	{
		return write(buffer, bufferSize, index, value.x) && write(buffer, bufferSize, index, value.y);
	}
	inline bool writeVector2Int(char* buffer, const int bufferSize, int& index, const Vector2Int value)
	{
		return write(buffer, bufferSize, index, value.x) && write(buffer, bufferSize, index, value.y);
	}
	inline bool writeVector2IntInverse(char* buffer, const int bufferSize, int& index, const Vector2Int value)
	{
		return writeInverse(buffer, bufferSize, index, value.x) && writeInverse(buffer, bufferSize, index, value.y);
	}
	inline bool writeVector3(char* buffer, const int bufferSize, int& index, const Vector3& value)
	{
		return write(buffer, bufferSize, index, value.x) &&
			   write(buffer, bufferSize, index, value.y) &&
			   write(buffer, bufferSize, index, value.z);
	}
	inline bool writeVector3Int(char* buffer, const int bufferSize, int& index, const Vector3Int& value)
	{
		return write(buffer, bufferSize, index, value.x) &&
			   write(buffer, bufferSize, index, value.y) &&
			   write(buffer, bufferSize, index, value.z);
	}
	inline bool writeVector3Inverse(char* buffer, const int bufferSize, int& index, const Vector3& value)
	{
		return writeInverse(buffer, bufferSize, index, value.x) &&
			   writeInverse(buffer, bufferSize, index, value.y) &&
			   writeInverse(buffer, bufferSize, index, value.z);
	}
	inline bool writeVector3IntInverse(char* buffer, const int bufferSize, int& index, const Vector3Int& value)
	{
		return writeInverse(buffer, bufferSize, index, value.x) &&
			   writeInverse(buffer, bufferSize, index, value.y) &&
			   writeInverse(buffer, bufferSize, index, value.z);
	}
	inline bool writeVector4(char* buffer, const int bufferSize, int& index, const Vector4& value)
	{
		return write(buffer, bufferSize, index, value.x) &&
			   write(buffer, bufferSize, index, value.y) &&
			   write(buffer, bufferSize, index, value.z) &&
			   write(buffer, bufferSize, index, value.w);
	}
	inline bool writeVector4Inverse(char* buffer, const int bufferSize, int& index, const Vector4& value)
	{
		return writeInverse(buffer, bufferSize, index, value.x) &&
			   writeInverse(buffer, bufferSize, index, value.y) &&
			   writeInverse(buffer, bufferSize, index, value.z) &&
			   writeInverse(buffer, bufferSize, index, value.w);
	}
	inline bool writeColor(char* buffer, int bufferSize, int& index, const Color& value)
	{
		return write(buffer, bufferSize, index, value.r) &&
			   write(buffer, bufferSize, index, value.g) &&
			   write(buffer, bufferSize, index, value.b) &&
			   write(buffer, bufferSize, index, value.a);
	}
	inline bool writeColorInverse(char* buffer, const int bufferSize, int& index, const Color& value)
	{
		return writeInverse(buffer, bufferSize, index, value.r) &&
			   writeInverse(buffer, bufferSize, index, value.g) &&
			   writeInverse(buffer, bufferSize, index, value.b) &&
			   writeInverse(buffer, bufferSize, index, value.a);
	}
	inline bool readVector2(const char* buffer, const int bufferSize, int& index, Vector2& value)
	{
		return read(buffer, bufferSize, index, value.x) && read(buffer, bufferSize, index, value.y);
	}
	inline bool readVector2Inverse(const char* buffer, const int bufferSize, int& index, Vector2& value)
	{
		return readInverse(buffer, bufferSize, index, value.x) && readInverse(buffer, bufferSize, index, value.y);
	}
	inline bool readVector2Int(const char* buffer, const int bufferSize, int& index, Vector2Int& value)
	{
		return read(buffer, bufferSize, index, value.x) && read(buffer, bufferSize, index, value.y);
	}
	inline bool readVector2IntInverse(const char* buffer, const int bufferSize, int& index, Vector2Int& value)
	{
		return readInverse(buffer, bufferSize, index, value.x) && readInverse(buffer, bufferSize, index, value.y);
	}
	inline bool readVector3(const char* buffer, const int bufferSize, int& index, Vector3& value)
	{
		return read(buffer, bufferSize, index, value.x) &&
			   read(buffer, bufferSize, index, value.y) &&
			   read(buffer, bufferSize, index, value.z);
	}
	inline bool readVector3Int(const char* buffer, const int bufferSize, int& index, Vector3Int& value)
	{
		return read(buffer, bufferSize, index, value.x) && 
			   read(buffer, bufferSize, index, value.y) &&
			   read(buffer, bufferSize, index, value.z);
	}
	inline bool readVector3Inverse(const char* buffer, const int bufferSize, int& index, Vector3& value)
	{
		return readInverse(buffer, bufferSize, index, value.x) &&
			   readInverse(buffer, bufferSize, index, value.y) &&
			   readInverse(buffer, bufferSize, index, value.z);
	}
	inline bool readVector4(const char* buffer, const int bufferSize, int& index, Vector4& value)
	{
		return read(buffer, bufferSize, index, value.x) &&
			   read(buffer, bufferSize, index, value.y) &&
			   read(buffer, bufferSize, index, value.z) &&
			   read(buffer, bufferSize, index, value.w);
	}
	inline bool readVector4Inverse(const char* buffer, const int bufferSize, int& index, Vector4& value)
	{
		return readInverse(buffer, bufferSize, index, value.x) &&
			   readInverse(buffer, bufferSize, index, value.y) &&
			   readInverse(buffer, bufferSize, index, value.z) &&
			   readInverse(buffer, bufferSize, index, value.w);
	}
	inline bool readColor(const char* buffer, const int bufferSize, int& index, Color& value)
	{
		return read(buffer, bufferSize, index, value.r) &&
			   read(buffer, bufferSize, index, value.g) &&
			   read(buffer, bufferSize, index, value.b) &&
			   read(buffer, bufferSize, index, value.a);
	}
	inline bool readColorInverse(const char* buffer, const int bufferSize, int& index, Color& value)
	{
		return readInverse(buffer, bufferSize, index, value.r) &&
			   readInverse(buffer, bufferSize, index, value.g) &&
			   readInverse(buffer, bufferSize, index, value.b) &&
			   readInverse(buffer, bufferSize, index, value.a);
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool writeArray(char* buffer, const int bufferSize, int& index, T* valueArray, const int arrayLength)
	{
		FOR(arrayLength)
		{
			if (!write(buffer, bufferSize, index, valueArray[i]))
			{
				return false;
			}
		}
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool writeArrayInverse(char* buffer, const int bufferSize, int& index, T* valueArray, const int arrayLength)
	{
		FOR(arrayLength)
		{
			if (!writeInverse(buffer, bufferSize, index, valueArray[i]))
			{
				return false;
			}
		}
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr void swapByte(T& value, const int pos0, const int pos1)
	{
		const char byte0 = getByte(value, pos0);
		const char byte1 = getByte(value, pos1);
		setByte(value, byte0, pos1);
		setByte(value, byte1, pos0);
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr void inverseByte(T& value)
	{
		constexpr int typeSize = sizeof(T);
		constexpr int halfSize = typeSize >> 1;
		FOR(halfSize)
		{
			swapByte(value, i, typeSize - 1 - i);
		}
	}
	inline void setString(char* buffer, const int bufferSize, const string& str)
	{
		const int length = (int)str.length();
		MEMCPY(buffer, bufferSize, str.c_str(), length);
		buffer[length] = '\0';
	}
}

using BinaryUtility::hasBit;
using BinaryUtility::setBitOne;
using BinaryUtility::setBitZero;
using BinaryUtility::getByte;
using BinaryUtility::bitCountToByteCount;
using BinaryUtility::setBit;
using BinaryUtility::getBit;
using BinaryUtility::setString;
using BinaryUtility::crc16;
using BinaryUtility::writeBuffer;
using BinaryUtility::readBuffer;
using BinaryUtility::writeVector2;
using BinaryUtility::writeVector3;
using BinaryUtility::writeVector4;
using BinaryUtility::writeBoolBit;
using BinaryUtility::writeSignedIntegerBit;
using BinaryUtility::writeUnsignedIntegerBit;
using BinaryUtility::writeBufferBit;
using BinaryUtility::writeSignedIntegerListBit;
using BinaryUtility::writeUnsignedIntegerListBit;
using BinaryUtility::writeFloatListBit;
using BinaryUtility::writeDoubleListBit;
using BinaryUtility::readBoolBit;
using BinaryUtility::readBufferBit;
using BinaryUtility::readSignedIntegerBit;
using BinaryUtility::readUnsignedIntegerBit;
using BinaryUtility::readSignedIntegerListBit;
using BinaryUtility::readUnsignedIntegerListBit;
using BinaryUtility::readFloatBit;
using BinaryUtility::readDoubleBit;
using BinaryUtility::readFloatListBit;
using BinaryUtility::readDoubleListBit;
using BinaryUtility::mStringType;
using BinaryUtility::mBoolType;
using BinaryUtility::mCharType;
using BinaryUtility::mWCharType;
using BinaryUtility::mByteType;
using BinaryUtility::mShortType;
using BinaryUtility::mUShortType;
using BinaryUtility::mIntType;
using BinaryUtility::mUIntType;
using BinaryUtility::mFloatType;
using BinaryUtility::mLLongType;
using BinaryUtility::mULLongType;
using BinaryUtility::mBoolPtrType;
using BinaryUtility::mCharPtrType;
using BinaryUtility::mBytePtrType;
using BinaryUtility::mShortPtrType;
using BinaryUtility::mUShortPtrType;
using BinaryUtility::mIntPtrType;
using BinaryUtility::mUIntPtrType;
using BinaryUtility::mFloatPtrType;
using BinaryUtility::mLLongPtrType;
using BinaryUtility::mULLongPtrType;
using BinaryUtility::mBoolListType;
using BinaryUtility::mCharListType;
using BinaryUtility::mByteListType;
using BinaryUtility::mShortListType;
using BinaryUtility::mUShortListType;
using BinaryUtility::mIntListType;
using BinaryUtility::mUIntListType;
using BinaryUtility::mFloatListType;
using BinaryUtility::mLLongListType;
using BinaryUtility::mULLongListType;
using BinaryUtility::mVector2Type;
using BinaryUtility::mVector2IntType;
using BinaryUtility::mVector2UIntType;
using BinaryUtility::mVector2ShortType;
using BinaryUtility::mVector2UShortType;
using BinaryUtility::mVector3Type;
using BinaryUtility::mVector3IntType;
using BinaryUtility::mVector2ListType;
using BinaryUtility::mVector2IntListType;
using BinaryUtility::mVector3ListType;
using BinaryUtility::mVector3IntListType;
using BinaryUtility::readVector2;
using BinaryUtility::readVector3;
using BinaryUtility::readVector4;
using BinaryUtility::writeColor;
using BinaryUtility::readInverse;
using BinaryUtility::read;
using BinaryUtility::write;
using BinaryUtility::writeInverse;
using BinaryUtility::inverseByte;
using BinaryUtility::swapByte;
using BinaryUtility::copyBits;
using BinaryUtility::getLowestBit;
using BinaryUtility::getHighestBit;
using BinaryUtility::setByte;
using BinaryUtility::getByte;
using BinaryUtility::getBits;
using BinaryUtility::generateBitCount;
using BinaryUtility::readColor;