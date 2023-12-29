#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct Array
{
	int n;
	char arr[51];
};

void MatchLetters(const Array& arr, Array& result);

namespace AlfavitTests
{
	TEST_CLASS(MatchLettersTest)
	{
	public:

		TEST_METHOD(Test1)
		{
			int n = 10;
			Array arr;
			Array actual_result;
			Array expected_result;
			arr.n = n;
			strcpy(arr.arr, "as54der45t");
			MatchLetters(arr, actual_result);
			expected_result.n = 6;
			strcpy(expected_result.arr, "asdert");
			Assert::AreEqual(expected_result.n, actual_result.n);
			Assert::IsTrue(strcmp(expected_result.arr, actual_result.arr) == 0);
		}
		TEST_METHOD(Test2)
		{
			int n = 10;
			Array arr;
			Array actual_result;
			Array expected_result;
			arr.n = n;
			strcpy(arr.arr, "as&=der%.t");
			MatchLetters(arr, actual_result);
			expected_result.n = 6;
			strcpy(expected_result.arr, "asdert");
			Assert::AreEqual(expected_result.n, actual_result.n);
			Assert::IsTrue(strcmp(expected_result.arr, actual_result.arr) == 0);
		}
		TEST_METHOD(Test3)
		{
			int n = 10;
			Array arr;
			Array actual_result;
			Array expected_result;
			arr.n = n;
			strcpy(arr.arr, "1234567890");
			MatchLetters(arr, actual_result);
			expected_result.n = 0;
			strcpy(expected_result.arr, "");
			Assert::AreEqual(expected_result.n, actual_result.n);
			Assert::IsTrue(strcmp(expected_result.arr, actual_result.arr) == 0);
		}
		TEST_METHOD(Test4)
		{
			int n = 10;
			Array arr;
			Array actual_result;
			Array expected_result;
			arr.n = n;
			strcpy(arr.arr, "aw&=er%.+-");
			MatchLetters(arr, actual_result);
			expected_result.n = 4;
			strcpy(expected_result.arr, "awer");
			Assert::AreEqual(expected_result.n, actual_result.n);
			Assert::IsTrue(strcmp(expected_result.arr, actual_result.arr) == 0);
		}
		TEST_METHOD(Test5)
		{
			int n = 10;
			Array arr;
			Array actual_result;
			Array expected_result;
			arr.n = n;
			strcpy(arr.arr, "abcdefghjk");
			MatchLetters(arr, actual_result);
			expected_result.n = 10;
			strcpy(expected_result.arr, "abcdefghjk");
			Assert::AreEqual(expected_result.n, actual_result.n);
			Assert::IsTrue(strcmp(expected_result.arr, actual_result.arr) == 0);
		}
	};
}
