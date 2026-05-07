#include "FrameHeader.h"
struct ArrayScopeDeepTestImpl
{

#define ArrayScopeDeepTestImpl_ASD_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("ArrayScopeDeepTest FAILED: ") + (msg)); } } while(0)


    static void test()
    {
        testArrayScopeIntBasic();
        testArrayScopeZeroMemory();
        testArrayScopeNoZeroMemory();
        testArrayScopeSize1();
        testArrayScopeSize128();
        testArrayScopeLargeAlloc();
        testArrayScopeWriteRead();
        testArrayScopeFloat();
        testArrayScopeDouble();
        testArrayScopeConsecutive();
        testArrayScopeShort();
        testArrayScopeByte();
        testArrayScopeMultipleScopes();
        testArrayScopeNestedScopes();
        testCharArrayScopeBasic();
        testCharArrayScopeZeroMemory();
        testCharArrayScopeWriteString();
        testCharArrayScopeSize1();
        testCharArrayScopeSize512();
        testCharArrayScopeNullTerminate();
        testCharArrayScopeConsecutive();
        testCharArrayScopeLargeAlloc();
        testArrayScopeThreadIntBasic();
        testArrayScopeThreadZeroMemory();
        testArrayScopeThreadWriteRead();
        testArrayScopeThreadFloat();
        testArrayScopeThreadSize256();
        testArrayScopeThreadConsecutive();
    }

    static void testArrayScopeIntBasic()
    {
        ArrayScope<int> scope(10, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "int scope not null");
        for (int i = 0; i < 10; ++i)
        {
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[i] == 0, "zeroMemory init to 0");
        }
    }

    static void testArrayScopeZeroMemory()
    {
        ArrayScope<int> scope(5, true);
        for (int i = 0; i < 5; ++i)
        {
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[i] == 0, "zero memory int");
        }
    }

    static void testArrayScopeNoZeroMemory()
    {
        ArrayScope<int> scope(4, false);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "no-zero scope not null");
        scope.mArray[0] = 42;
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == 42, "write then read ok");
    }

    static void testArrayScopeSize1()
    {
        ArrayScope<int> scope(1, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "size 1 not null");
        scope.mArray[0] = 999;
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == 999, "size 1 write ok");
    }

    static void testArrayScopeSize128()
    {
        ArrayScope<int> scope(128, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "size 128 not null");
        for (int i = 0; i < 128; ++i)
        {
            scope.mArray[i] = i * 2;
        }
        for (int i = 0; i < 128; ++i)
        {
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[i] == i * 2, "size 128 values ok");
        }
    }

    static void testArrayScopeLargeAlloc()
    {
        ArrayScope<int> scope(4096, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "large alloc not null");
        scope.mArray[0] = 1;
        scope.mArray[4095] = 2;
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == 1, "large alloc first");
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[4095] == 2, "large alloc last");
    }

    static void testArrayScopeWriteRead()
    {
        ArrayScope<int> scope(8, false);
        for (int i = 0; i < 8; ++i)
        {
            scope.mArray[i] = i * i;
        }
        for (int i = 0; i < 8; ++i)
        {
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[i] == i * i, "write-read int*int");
        }
    }

    static void testArrayScopeFloat()
    {
        ArrayScope<float> scope(6, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "float scope not null");
        for (int i = 0; i < 6; ++i)
        {
            scope.mArray[i] = 0.1f * i;
        }
        ArrayScopeDeepTestImpl_ASD_ASSERT(fabsf(scope.mArray[5] - 0.5f) < 1e-5f, "float scope value ok");
    }

    static void testArrayScopeDouble()
    {
        ArrayScope<double> scope(4, true);
        scope.mArray[0] = 3.14159265358979;
        scope.mArray[3] = -2.71828;
        ArrayScopeDeepTestImpl_ASD_ASSERT(fabs(scope.mArray[0] - 3.14159265358979) < 1e-10, "double scope ok");
        ArrayScopeDeepTestImpl_ASD_ASSERT(fabs(scope.mArray[3] - (-2.71828)) < 1e-10, "double scope neg ok");
    }

    static void testArrayScopeConsecutive()
    {
        for (int round = 0; round < 20; ++round)
        {
            ArrayScope<int> scope(16, true);
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "consecutive scope not null");
            scope.mArray[0] = round;
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == round, "consecutive scope value");
        }
    }

    static void testArrayScopeShort()
    {
        ArrayScope<short> scope(10, true);
        for (int i = 0; i < 10; ++i)
        {
            scope.mArray[i] = (short)(i * 100);
        }
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[9] == 900, "short scope value ok");
    }

    static void testArrayScopeByte()
    {
        ArrayScope<byte> scope(8, true);
        for (int i = 0; i < 8; ++i)
        {
            scope.mArray[i] = (byte)(i * 10);
        }
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[7] == 70, "byte scope value ok");
    }

    static void testArrayScopeMultipleScopes()
    {
        ArrayScope<int> s1(4, true);
        ArrayScope<int> s2(8, true);
        ArrayScope<float> s3(3, true);
        s1.mArray[0] = 10;
        s2.mArray[0] = 20;
        s3.mArray[0] = 3.0f;
        ArrayScopeDeepTestImpl_ASD_ASSERT(s1.mArray[0] == 10, "multiple scopes s1 ok");
        ArrayScopeDeepTestImpl_ASD_ASSERT(s2.mArray[0] == 20, "multiple scopes s2 ok");
        ArrayScopeDeepTestImpl_ASD_ASSERT(fabsf(s3.mArray[0] - 3.0f) < 1e-5f, "multiple scopes s3 ok");
    }

    static void testArrayScopeNestedScopes()
    {
        ArrayScope<int> outer(10, true);
        outer.mArray[0] = 1;
        {
            ArrayScope<int> inner(5, true);
            inner.mArray[0] = 2;
            ArrayScopeDeepTestImpl_ASD_ASSERT(outer.mArray[0] == 1, "nested outer not clobbered");
            ArrayScopeDeepTestImpl_ASD_ASSERT(inner.mArray[0] == 2, "nested inner ok");
        }
        ArrayScopeDeepTestImpl_ASD_ASSERT(outer.mArray[0] == 1, "nested outer after inner release ok");
    }

    static void testCharArrayScopeBasic()
    {
        CharArrayScope scope(10);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "char scope not null");
    }

    static void testCharArrayScopeZeroMemory()
    {
        CharArrayScope scope(8, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "char zero scope not null");
        for (int i = 0; i < 8; ++i)
        {
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[i] == 0, "char zero init ok");
        }
    }

    static void testCharArrayScopeWriteString()
    {
        CharArrayScope scope(16, true);
        const char* msg = "Hello";
        memcpy(scope.mArray, msg, 6);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == 'H', "char scope H");
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[4] == 'o', "char scope o");
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[5] == '\0', "char scope null term");
    }

    static void testCharArrayScopeSize1()
    {
        CharArrayScope scope(1, false);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "char size 1 not null");
        scope.mArray[0] = 'Z';
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == 'Z', "char size 1 write ok");
    }

    static void testCharArrayScopeSize512()
    {
        CharArrayScope scope(512, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "char size 512 not null");
        scope.mArray[0] = 'A';
        scope.mArray[511] = 'B';
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == 'A', "char 512 first ok");
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[511] == 'B', "char 512 last ok");
    }

    static void testCharArrayScopeNullTerminate()
    {
        CharArrayScope scope(64, true);
        const char* src = "TestString123";
        int len = (int)strlen(src);
        memcpy(scope.mArray, src, len + 1);
        ArrayScopeDeepTestImpl_ASD_ASSERT(strcmp(scope.mArray, "TestString123") == 0, "char scope strcmp ok");
    }

    static void testCharArrayScopeConsecutive()
    {
        for (int i = 0; i < 20; ++i)
        {
            CharArrayScope scope(32, true);
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "consecutive char scope not null");
            scope.mArray[0] = (char)(i + 65);
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == (char)(i + 65), "consecutive char scope value");
        }
    }

    static void testCharArrayScopeLargeAlloc()
    {
        CharArrayScope scope(8192, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "large char alloc not null");
        scope.mArray[0] = 'X';
        scope.mArray[8191] = 'Y';
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == 'X', "large char first ok");
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[8191] == 'Y', "large char last ok");
    }

    static void testArrayScopeThreadIntBasic()
    {
        ArrayScopeThread<int> scope(10, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "thread scope int not null");
        for (int i = 0; i < 10; ++i)
        {
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[i] == 0, "thread scope zero init");
        }
    }

    static void testArrayScopeThreadZeroMemory()
    {
        ArrayScopeThread<int> scope(5, true);
        for (int i = 0; i < 5; ++i)
        {
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[i] == 0, "thread zero memory");
        }
    }

    static void testArrayScopeThreadWriteRead()
    {
        ArrayScopeThread<int> scope(8, false);
        for (int i = 0; i < 8; ++i)
        {
            scope.mArray[i] = i + 100;
        }
        for (int i = 0; i < 8; ++i)
        {
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[i] == i + 100, "thread scope write read");
        }
    }

    static void testArrayScopeThreadFloat()
    {
        ArrayScopeThread<float> scope(4, true);
        scope.mArray[0] = 1.5f;
        scope.mArray[3] = -2.5f;
        ArrayScopeDeepTestImpl_ASD_ASSERT(fabsf(scope.mArray[0] - 1.5f) < 1e-5f, "thread float scope ok");
        ArrayScopeDeepTestImpl_ASD_ASSERT(fabsf(scope.mArray[3] - (-2.5f)) < 1e-5f, "thread float neg ok");
    }

    static void testArrayScopeThreadSize256()
    {
        ArrayScopeThread<int> scope(256, true);
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "thread size 256 not null");
        for (int i = 0; i < 256; ++i)
        {
            scope.mArray[i] = i;
        }
        ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[255] == 255, "thread size 256 last ok");
    }

    static void testArrayScopeThreadConsecutive()
    {
        for (int round = 0; round < 15; ++round)
        {
            ArrayScopeThread<int> scope(32, true);
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray != nullptr, "thread consecutive not null");
            scope.mArray[0] = round * 7;
            ArrayScopeDeepTestImpl_ASD_ASSERT(scope.mArray[0] == round * 7, "thread consecutive value");
        }
    }
};

struct ArrayScopeDeepTest2Impl
{

#define ArrayScopeDeepTest2Impl_ASD2_ASSERT(expr) \
    do { if (!(expr)) { ERROR("ArrayScopeDeepTest2 FAIL: " #expr); } } while(0)

    static void test()
    {
        testArrayDefaultConstruct();
        testArrayFillArray();
        testArrayFillArrayPartial();
        testArraySetElement();
        testArraySize();
        testArrayData();
        testArrayContains();
        testArrayIterator();
        testArrayCopyConstruct();
        testArrayAssign();
        testArrayEqualManual();
        testArrayNotEqualManual();
        testArrayOutOfBounds();
        testArrayLargeType();
        testArrayIntArray();
        testArrayFloatArray();
        testArrayStringArray();
        testArrayZero();
        testArraySortAscend();
        testArraySortDescend();
        testArraySortWithData();
        testArrayBinarySearch();
        testArrayFindAndModify();
    }

    static void testArrayDefaultConstruct()
    {
        Array<8, int> arr;
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr.size() == 8);
    }

    static void testArrayFillArray()
    {
        Array<5, int> arr;
        arr.setAll(99);
        for (int i = 0; i < 5; ++i)
        {
            ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[i] == 99);
        }
    }

    static void testArrayFillArrayPartial()
    {
        Array<10, int> arr;
        arr.setAll(0);
        arr.setAll(3, 7);
        for (int i = 3; i < 10; ++i)
        {
            ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[i] == 7);
        }
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[0] == 0);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[2] == 0);
    }

    static void testArraySetElement()
    {
        Array<4, int> arr;
        arr.setAll(0);
        arr[2] = 42;
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[2] == 42);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[0] == 0);
    }

    static void testArraySize()
    {
        Array<16, float> arr;
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr.size() == 16);
    }

    static void testArrayData()
    {
        Array<3, int> arr;
        arr.setAll(5);
        int* ptr = arr.data();
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(ptr != nullptr);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(ptr[0] == 5);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(ptr[1] == 5);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(ptr[2] == 5);
    }

    static void testArrayContains()
    {
        Array<5, int> arr;
        arr.setAll(0);
        arr[2] = 10;
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr.contains(10) == true);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr.contains(99) == false);
    }

    static void testArrayIterator()
    {
        Array<4, int> arr;
        arr[0] = 1;
        arr[1] = 2;
        arr[2] = 3;
        arr[3] = 4;
        int sum = 0;
        for (auto& v : arr)
        {
            sum += v;
        }
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(sum == 10);
    }

    static void testArrayCopyConstruct()
    {
        Array<3, int> arr;
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        Array<3, int> copyArr = arr;
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(copyArr[0] == 10);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(copyArr[1] == 20);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(copyArr[2] == 30);
    }

    static void testArrayAssign()
    {
        Array<3, int> arr;
        arr[0] = 1;
        arr[1] = 2;
        arr[2] = 3;
        Array<3, int> arr2;
        arr2 = arr;
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr2[0] == 1);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr2[1] == 2);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr2[2] == 3);
    }

    static void testArrayEqualManual()
    {
        Array<3, int> a;
        Array<3, int> b;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        b[0] = 1;
        b[1] = 2;
        b[2] = 3;
        bool eq = true;
        for (int i = 0; i < 3; ++i)
        {
            if (a[i] != b[i])
            {
                eq = false;
            }
        }
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(eq);
    }

    static void testArrayNotEqualManual()
    {
        Array<3, int> a;
        Array<3, int> b;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        b[0] = 1;
        b[1] = 2;
        b[2] = 9;
        bool diff = false;
        for (int i = 0; i < 3; ++i)
        {
            if (a[i] != b[i])
            {
                diff = true;
            }
        }
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(diff);
    }

    static void testArrayOutOfBounds()
    {
        Array<4, int> arr;
        arr.setAll(0);
        arr[3] = 5;
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[3] == 5);
    }

    static void testArrayLargeType()
    {
        Array<64, int> big;
        big.setAll(42);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(big[0] == 42);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(big[63] == 42);
    }

    static void testArrayIntArray()
    {
        Array<6, int> arr;
        for (int i = 0; i < 6; ++i)
        {
            arr[i] = i * i;
        }
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[0] == 0);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[3] == 9);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[5] == 25);
    }

    static void testArrayFloatArray()
    {
        Array<4, float> arr;
        arr[0] = 1.1f;
        arr[1] = 2.2f;
        arr[2] = 3.3f;
        arr[3] = 4.4f;
        float sum = 0.0f;
        for (int i = 0; i < 4; ++i)
        {
            sum += arr[i];
        }
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(fabsf(sum - 11.0f) < 1e-3f);
    }

    static void testArrayStringArray()
    {
        Array<3, string> arr;
        arr[0] = "hello";
        arr[1] = "world";
        arr[2] = "test";
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[0] == "hello");
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[1] == "world");
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[2] == "test");
    }

    static void testArrayZero()
    {
        Array<5, int> arr;
        arr.setAll(100);
        arr.zero();
        for (int i = 0; i < 5; ++i)
        {
            ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[i] == 0);
        }
    }

    static void testArraySortAscend()
    {
        Array<5, int> arr;
        arr[0] = 5;
        arr[1] = 3;
        arr[2] = 1;
        arr[3] = 4;
        arr[4] = 2;
        MathUtility::quickSort(arr.data(), arr.size());
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[0] == 1);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[4] == 5);
        for (int i = 0; i < 4; ++i)
        {
            ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[i] <= arr[i + 1]);
        }
    }

    static void testArraySortDescend()
    {
        Array<5, int> arr;
        arr[0] = 1;
        arr[1] = 5;
        arr[2] = 3;
        arr[3] = 2;
        arr[4] = 4;
        MathUtility::quickSort(arr.data(), arr.size(), false);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[0] == 5);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[4] == 1);
        for (int i = 0; i < 4; ++i)
        {
            ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[i] >= arr[i + 1]);
        }
    }

    static void testArraySortWithData()
    {
        Array<4, int> arr;
        arr[0] = 4;
        arr[1] = 1;
        arr[2] = 3;
        arr[3] = 2;
        MathUtility::quickSort(arr.data(), arr.size());
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[0] == 1);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[1] == 2);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[2] == 3);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[3] == 4);
    }

    static void testArrayBinarySearch()
    {
        Array<5, int> arr;
        arr[0] = 1;
        arr[1] = 3;
        arr[2] = 5;
        arr[3] = 7;
        arr[4] = 9;
        bool f5 = arr.contains(5);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(f5 == true);
        bool f6 = arr.contains(6);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(f6 == false);
    }

    static void testArrayFindAndModify()
    {
        Array<4, int> arr;
        arr.setAll(0);
        for (int i = 0; i < 4; ++i)
        {
            if (arr[i] == 0)
            {
                arr[i] = i + 1;
            }
        }
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[0] == 1);
        ArrayScopeDeepTest2Impl_ASD2_ASSERT(arr[3] == 4);
    }
};

void ArrayScopeDeepTest::test()
{
    ArrayScopeDeepTestImpl::test();
    ArrayScopeDeepTest2Impl::test();
}
