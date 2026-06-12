#include "FrameHeader.h"

namespace SQLUtility
{
	void sqlAddString(char* queryStr, const int size, const char* str, const bool addComma)
	{
		if (checkSQLString(str))
		{
			strcat_t(queryStr, size, "'", str, addComma ? "'," : "'");
		}
		else
		{
			strcat_t(queryStr, size, "'", addComma ? "'," : "'");
		}
	}
	
	void sqlAddStringUTF8(char* queryStr, const int size, const char* str, const bool addComma)
	{
		if (str[0] != '\0')
		{
			if (checkSQLString(str))
			{
				CharArrayScopeThread utf8String(size);
				ANSIToUTF8(str, utf8String.mArray, size, false);
				strcat_t(queryStr, size, "'", utf8String.mArray, addComma ? "'," : "'");
			}
			else
			{
				strcat_t(queryStr, size, "'", addComma ? "'," : "'");
			}
		}
		else
		{
			strcat_t(queryStr, size, "'", addComma ? "'," : "'");
		}
	}

	void sqlAddStringUTF8(string& queryStr, const string& str, const bool addComma)
	{
		if (str[0] != '\0')
		{
			if (checkSQLString(str))
			{
				queryStr += "'";
				queryStr += ANSIToUTF8(str);
				queryStr += addComma ? "'," : "'";
			}
			else
			{
				queryStr += "'";
				queryStr += addComma ? "'," : "'";
			}
		}
		else
		{
			queryStr += "'";
			queryStr += addComma ? "'," : "'";
		}
	}

	void sqlAddInt(char* queryStr, const int size, const int value, const bool addComma)
	{
		INT_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, size, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, size, temp.str());
		}
	}

	void sqlAddUInt(char* queryStr, const int size, const int value, const bool addComma)
	{
		UINT_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, size, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, size, temp.str());
		}
	}

	void sqlAddFloat(char* queryStr, const int size, const float value, const bool addComma)
	{
		FLOAT_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, size, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, size, temp.str());
		}
	}

	void sqlAddULLong(char* queryStr, const int size, const ullong value, const bool addComma)
	{
		ULLONG_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, size, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, size, temp.str());
		}
	}

	void sqlAddLLong(char* queryStr, const int size, const llong value, const bool addComma)
	{
		LLONG_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, size, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, size, temp.str());
		}
	}

	void sqlAddVector2Int(char* queryStr, const int size, Vector2Int value, const bool addComma)
	{
		MyString<32> temp;
		V2IToS(temp, value);
		sqlAddString(queryStr, size, temp.str(), addComma);
	}

	void sqlAddVector2UInt(char* queryStr, const int size, Vector2UInt value, const bool addComma)
	{
		MyString<32> temp;
		V2UIToS(temp, value);
		sqlAddString(queryStr, size, temp.str(), addComma);
	}

	void sqlAddVector2Short(char* queryStr, const int size, Vector2Short value, const bool addComma)
	{
		MyString<16> temp;
		V2SToS(temp, value);
		sqlAddString(queryStr, size, temp.str(), addComma);
	}

	void sqlAddVector2UShort(char* queryStr, const int size, Vector2UShort value, const bool addComma)
	{
		MyString<16> temp;
		V2USToS(temp, value);
		sqlAddString(queryStr, size, temp.str(), addComma);
	}

	void sqlUpdateString(char* updateInfo, const int size, const string& col, const char* str, const bool addComma)
	{
		if (checkSQLString(str))
		{
			strcat_t(updateInfo, size, col.c_str(), " = '", str, addComma ? "'," : "'");
		}
		else
		{
			strcat_t(updateInfo, size, col.c_str(), " = '", addComma ? "'," : "'");
		}
	}

	void sqlUpdateStringUTF8(char* updateInfo, const int size, const string& col, const char* str, const bool addComma)
	{
		if (checkSQLString(str))
		{
			CharArrayScopeThread utf8String(size);
			ANSIToUTF8(str, utf8String.mArray, size, false);
			strcat_t(updateInfo, size, col.c_str(), " = '", utf8String.mArray, addComma ? "'," : "'");
		}
		else
		{
			strcat_t(updateInfo, size, col.c_str(), " = '", addComma ? "'," : "'");
		}
	}

	void sqlUpdateStringUTF8(string& updateInfo, const string& col, const string& str, const bool addComma)
	{
		if (checkSQLString(str))
		{
			updateInfo += col;
			updateInfo += " = '";
			updateInfo += ANSIToUTF8(str);
			updateInfo += addComma ? "'," : "'";
		}
		else
		{
			updateInfo += col;
			updateInfo += " = '";
			updateInfo += addComma ? "'," : "'";
		}
	}
}