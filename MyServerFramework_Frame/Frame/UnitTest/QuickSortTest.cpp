#include "FrameHeader.h"

namespace
{
#define QS_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("QuickSortTest FAILED: ") + (msg)); } } while(0)

	// ========== helper: verify sorted (ascending) ==========
	template<typename T>
	static bool isSortedAscend(const T* arr, int count)
	{
		for (int i = 1; i < count; ++i)
		{
			if (arr[i] < arr[i - 1])
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	static bool isSortedDescend(const T* arr, int count)
	{
		for (int i = 1; i < count; ++i)
		{
			if (arr[i] > arr[i - 1])
			{
				return false;
			}
		}
		return true;
	}

	// ---------- quickSortAscend: 基本升序 ----------
	static void testQuickSortAscend_Basic()
	{
		int arr[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend basic");
		QS_ASSERT(arr[0] == 1 && arr[n - 1] == 9, "ascend range");
	}

	// ---------- quickSortAscend: 已有序（最优路径）----------
	static void testQuickSortAscend_AlreadySorted()
	{
		int arr[] = {1, 2, 3, 4, 5, 6, 7};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		int copy[n];
		MEMCPY(copy, sizeof(copy), arr, sizeof(arr));
		MathUtility::quickSortAscend(copy, 0, n - 1);
		QS_ASSERT(isSortedAscend(copy, n), "ascend already sorted stable");
	}

	// ---------- quickSortAscend: 逆序（最坏情况之一）----------
	static void testQuickSortAscend_Reverse()
	{
		int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend reverse");
	}

	// ---------- quickSortAscend: 全部相同元素 ----------
	static void testQuickSortAscend_AllSame()
	{
		int arr[] = {5, 5, 5, 5, 5};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend all same");
	}

	// ---------- quickSortAscend: 单元素 / 两元素 ----------
	static void testQuickSortAscend_TrivialSize()
	{
		int a1[] = {42};
		MathUtility::quickSortAscend(a1, 0, 0);
		QS_ASSERT(a1[0] == 42, "single element");

		int a2[] = {10, 5};
		MathUtility::quickSortAscend(a2, 0, 1);
		QS_ASSERT(a2[0] == 5 && a2[1] == 10, "two elements");
	}

	// ---------- quickSortAscend: 含负数 ----------
	static void testQuickSortAscend_Negative()
	{
		int arr[] = {-3, 5, -1, 0, -7, 2};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend negative");
		QS_ASSERT(arr[0] == -7 && arr[n - 1] == 5, "ascend negative range");
	}

	// ---------- quickSortAscend: float 类型 ----------
	static void testQuickSortAscend_Float()
	{
		float arr[] = {3.14f, -1.5f, 0.0f, 2.71f, -0.01f};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend float");
	}

	// ---------- quickSortDescend: 基本降序 ----------
	static void testQuickSortDescend_Basic()
	{
		int arr[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortDescend(arr, 0, n - 1);
		QS_ASSERT(isSortedDescend(arr, n), "descend basic");
		QS_ASSERT(arr[0] == 9 && arr[n - 1] == 1, "descend range");
	}

	// ---------- quickSortDescend: 负数 ----------
	static void testQuickSortDescend_Negative()
	{
		int arr[] = {-3, 5, -1, 0, -7, 2};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		quickSortDescend(arr, 0, n - 1);
		QS_ASSERT(isSortedDescend(arr, n), "descend negative");
		QS_ASSERT(arr[0] == 5 && arr[n - 1] == -7, "descend negative range");
	}

	// ---------- quickSortAscendPtr: 指针数组升序 ----------
	static void testQuickSortAscendPtr_Basic()
	{
		int values[] = {30, 10, 20, 50, 40};
		int* ptrs[5];
		FOR(5)
		{
			ptrs[i] = &values[i];
		}
		quickSortAscendPtr(ptrs, 0, 4);
		// ptrs should now point to values in ascending order
		for (int i = 1; i < 5; ++i)
		{
			QS_ASSERT(*ptrs[i] >= *ptrs[i - 1], "ascend ptr order");
		}
		QS_ASSERT(*ptrs[0] == 10 && *ptrs[4] == 50, "ascend ptr range");
	}

	// ---------- quickSortAscendPtr: 相同值 ----------
	static void testQuickSortAscendPtr_DuplicateValues()
	{
		int values[] = {20, 10, 20, 10, 20};
		int* ptrs[5];
		FOR(5)
		{
			ptrs[i] = &values[i];
		}
		MathUtility::quickSortAscendPtr(ptrs, 0, 4);
		for (int i = 1; i < 5; ++i)
		{
			QS_ASSERT(*ptrs[i] >= *ptrs[i - 1], "ascend ptr dup order");
		}
	}

	// ---------- quickSortDescendPtr: 指针数组降序 ----------
	static void testQuickSortDescendPtr_Basic()
	{
		int values[] = {30, 10, 20, 50, 40};
		int* ptrs[5];
		FOR(5)
		{
			ptrs[i] = &values[i];
		}
		MathUtility::quickSortDescendPtr(ptrs, 0, 4);
		for (int i = 1; i < 5; ++i)
		{
			QS_ASSERT(*ptrs[i] <= *ptrs[i - 1], "descend ptr order");
		}
		QS_ASSERT(*ptrs[0] == 50 && *ptrs[4] == 10, "descend ptr range");
	}

	// ---------- compare 函数指针版本: ascend ----------
	static int compareInt(const int& a, const int& b)
	{
		if (a < b)
		{
			return -1;
		}
		if (a > b)
		{
			return 1;
		}
		return 0;
	}

	static void testQuickSortAscend_Compare()
	{
		int arr[] = {5, 3, 8, 1, 9, 2};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1, compareInt);
		QS_ASSERT(isSortedAscend(arr, n), "ascend compare basic");
	}

	// ---------- compare 函数指针版本: descend ----------
	static void testQuickSortDescend_Compare()
	{
		int arr[] = {5, 3, 8, 1, 9, 2};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortDescend(arr, 0, n - 1, compareInt);
		QS_ASSERT(isSortedDescend(arr, n), "descend compare basic");
	}

	// ---------- compare 函数指针: nullptr 安全 ----------
	static void testQuickSort_CompareNullptr()
	{
		int arr[] = {3, 1, 2};
		// 不应崩溃，直接返回
		quickSortAscend(arr, 0, 2, (CompareTCallback<int>)nullptr);
		quickSortDescend(arr, 0, 2, (CompareTCallback<int>)nullptr);
		QS_ASSERT(true, "nullptr compare does not crash");

		// NonRecursive 版本也测一下
		int pivot = MathUtility::quickSortNonRecursiveAscend(arr, 0, 2, (CompareTCallback<int>)nullptr);
		QS_ASSERT(pivot == -1, "nonrecursive ascend null returns -1");
		pivot = MathUtility::quickSortNonRecursiveDescend(arr, 0, 2, (CompareTCallback<int>)nullptr);
		QS_ASSERT(pivot == -1, "nonrecursive descend null returns -1");
	}

	// ---------- 大量数据稳定性 ----------
	static void testQuickSort_LargeDataset()
	{
		const int N = 1000;
		int* arr = new int[N];
		FOR(N)
		{
			arr[i] = N - i; // 逆序
		}
		MathUtility::quickSortAscend(arr, 0, N - 1);
		QS_ASSERT(isSortedAscend(arr, N), "large dataset ascend");
		QS_ASSERT(arr[0] == 1 && arr[N - 1] == N, "large dataset range");

		// 再排降序
		MathUtility::quickSortDescend(arr, 0, N - 1);
		QS_ASSERT(isSortedDescend(arr, N), "large dataset descend");
		delete[] arr;
	}

	// ---------- quickSortNonRecursive 返回 pivot ----------
	static void testQuickSortNonRecursive_PivotReturn()
	{
		int arr[] = {5, 3, 8, 1, 9};
		int pivot = MathUtility::quickSortNonRecursiveAscend(arr, 0, 4);
		QS_ASSERT(pivot >= 0 && pivot <= 4, "nonrecur pivot valid range");
		// pivot 左侧 <= arr[pivot]，右侧 > arr[pivot]
		FOR(pivot)
		{
			QS_ASSERT(arr[i] <= arr[pivot], "nonrecur left side <= pivot");
		}
		for (int i = pivot + 1; i <= 4; ++i)
		{
			QS_ASSERT(arr[i] > arr[pivot], "nonrecur right side > pivot");
		}
	}
}

void QuickSortTest::test()
{
	testQuickSortAscend_Basic();
	testQuickSortAscend_AlreadySorted();
	testQuickSortAscend_Reverse();
	testQuickSortAscend_AllSame();
	testQuickSortAscend_TrivialSize();
	testQuickSortAscend_Negative();
	testQuickSortAscend_Float();
	testQuickSortDescend_Basic();
	testQuickSortDescend_Negative();
	testQuickSortAscendPtr_Basic();
	testQuickSortAscendPtr_DuplicateValues();
	testQuickSortDescendPtr_Basic();
	testQuickSortAscend_Compare();
	testQuickSortDescend_Compare();
	testQuickSort_CompareNullptr();
	testQuickSort_LargeDataset();
	testQuickSortNonRecursive_PivotReturn();
}
