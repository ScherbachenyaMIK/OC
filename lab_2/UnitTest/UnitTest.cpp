#include "pch.h"
#include "CppUnitTest.h"
#include <algorithm>
#include "../Child/MergeSort.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(SortTest)
	{
	public:
		
		TEST_METHOD(Test1)
		{
			int n = 5;
			int a[] = { 1, 1, 5, -3, 6 };
			int b[] = { -3, 1, 1, 5, 6 };
			MergeSort(a, 0, n - 1);
			for (int i = 0; i < n; ++i)
			{
				Assert::AreEqual(b[i], a[i]);
			}
		}
		TEST_METHOD(Test2)
		{
			int n = 5;
			int a[] = { -1, -1, -5, -3, -6 };
			int b[] = { -6, -5, -3, -1, -1 };
			MergeSort(a, 0, n - 1);
			for (int i = 0; i < n; ++i)
			{
				Assert::AreEqual(b[i], a[i]);
			}
		}
		TEST_METHOD(Test3)
		{
			int n = 5;
			int a[] = { 0, 0, 0, 0, 0 };
			int b[] = { 0, 0, 0, 0, 0 };
			MergeSort(a, 0, n - 1);
			for (int i = 0; i < n; ++i)
			{
				Assert::AreEqual(b[i], a[i]);
			}
		}
		TEST_METHOD(Test4)
		{
			int n = 5;
			int a[] = { 100, 151, 11, -5, 14 };
			int b[] = { -5, 11, 14, 100, 151 };
			MergeSort(a, 0, n - 1);
			for (int i = 0; i < n; ++i)
			{
				Assert::AreEqual(b[i], a[i]);
			}
		}
		TEST_METHOD(Test5)
		{
			int n = 15;
			int a[] = { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
			int b[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
			MergeSort(a, 0, n - 1);
			for (int i = 0; i < n; ++i)
			{
				Assert::AreEqual(b[i], a[i]);
			}
		}
	};
}
