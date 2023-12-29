#include "pch.h"
#include "CppUnitTest.h"
#include "../lab_6/Header.h"
#include <Windows.h>
#include "../lab_6/Header.h"
#include "../lab_6/Variables.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern CRITICAL_SECTION cs1, cs2;

extern HANDLE hWorkEvent;
extern HANDLE hCountEnterSectionEvent;
extern HANDLE hWorkEnterSectionEvent;
extern HANDLE count_element;
extern HANDLE work;

extern int t;
extern double x;
extern int result;
extern int n;

int CountElement2(std::vector<double>& a)
{
	int count = 0;
	for (int i = 0; i < a.size(); ++i)
	{
		if (abs(a[i] - x) < 1e-5)
		{
			++count;
		}
	}
	return n - count;
}

namespace Lab3Test
{
	TEST_CLASS(WorkFunctionTests)
	{
	public:
		TEST_METHOD(Test1)
		{
			hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hCountEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hWorkEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			srand(time(NULL));
			std::vector<double> a = { 1.1, 1.87, 3.141592, 2.81, 1.71, 1.41, 1.73 };
			n = a.size();
			t = 0;
			x = 1.87;
			InitializeCriticalSection(&cs1);
			InitializeCriticalSection(&cs2);

			SetEvent(hWorkEvent);
			Work(&a);
			for (int i = 0; i < 1; ++i)
			{
				Assert::IsTrue(abs(a[i] - x) < 1e-6);
			}
			for (int i = 1; i < n; ++i)
			{
				Assert::IsFalse(abs(a[i] - x) < 1e-6);
			}
		}
		TEST_METHOD(Test2)
		{
			hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hCountEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hWorkEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			srand(time(NULL));
			std::vector<double> a = { 1.1, 1.87, 3.141592, 2.81, 1.71, 1.41, 1.73, 1.87, 1.87, 1.87, 1.87, 1.87, 1.87, 1.111 };
			n = a.size();
			t = 0;
			x = 1.87;
			InitializeCriticalSection(&cs1);
			InitializeCriticalSection(&cs2);

			SetEvent(hWorkEvent);
			Work(&a);
			for (int i = 0; i < 7; ++i)
			{
				Assert::IsTrue(abs(a[i] - x) < 1e-6);
			}
			for (int i = 7; i < n; ++i)
			{
				Assert::IsFalse(abs(a[i] - x) < 1e-6);
			}
		}
		TEST_METHOD(Test3)
		{
			hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hCountEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hWorkEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			srand(time(NULL));
			std::vector<double> a = { 1.1 };
			n = a.size();
			t = 0;
			x = 1.87;
			InitializeCriticalSection(&cs1);
			InitializeCriticalSection(&cs2);

			SetEvent(hWorkEvent);
			Work(&a);
			for (int i = 0; i < n; ++i)
			{
				Assert::IsFalse(abs(a[i] - x) < 1e-6);
			}
		}
	};
	TEST_CLASS(CountFunctionTests)
	{
	public:
		TEST_METHOD(Test1)
		{
			hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hCountEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hWorkEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			srand(time(NULL));
			std::vector<double> a = { 1.87, 1.1, 3.141592, 2.81, 1.71, 1.41, 1.73 };
			n = a.size();
			t = 0;
			x = 1.87;
			InitializeCriticalSection(&cs1);
			InitializeCriticalSection(&cs2);

			SetEvent(hWorkEvent);
			CountElement(&a);
			Assert::AreEqual(6, result);
		}
		TEST_METHOD(Test2)
		{
			hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hCountEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hWorkEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			srand(time(NULL));
			std::vector<double> a = { 1.87, 1.87, 1.87, 1.87, 1.87, 1.87, 1.87, 1.1, 3.141592, 2.81, 1.71, 1.41, 1.73, 1.111 };
			n = a.size();
			t = 0;
			x = 1.87;
			InitializeCriticalSection(&cs1);
			InitializeCriticalSection(&cs2);

			SetEvent(hWorkEvent);
			CountElement(&a);
			Assert::AreEqual(7, result);
		}
		TEST_METHOD(Test3)
		{
			hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hCountEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hWorkEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			srand(time(NULL));
			std::vector<double> a = { 1.1 };
			n = a.size();
			t = 0;
			x = 1.87;
			InitializeCriticalSection(&cs1);
			InitializeCriticalSection(&cs2);

			SetEvent(hWorkEvent);
			CountElement(&a);
			Assert::AreEqual(1, result);
		}
	};
	TEST_CLASS(Lab3Test)
	{
	public:
		TEST_METHOD(Test1)
		{
			hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hCountEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hWorkEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			srand(time(NULL));
			std::vector<double> a = { 1.1, 1.87, 3.141592, 2.81, 1.71, 1.41, 1.73 };
			n = a.size();
			t = 0;
			x = 1.87;
			int expected_result = 6;
			InitializeCriticalSection(&cs1);
			InitializeCriticalSection(&cs2);
			HANDLE work = CreateThread(NULL, 0, Work, &a, 0, NULL);
			HANDLE count_element = CreateThread(NULL, 0, CountElement, &a, 0, NULL);

			WaitForSingleObject(hWorkEnterSectionEvent, INFINITE);
			SetEvent(hWorkEvent);

			EnterCriticalSection(&cs1);
			for (auto i : a)
			{
				Sleep(5);
			}
			std::cout << '\n';
			LeaveCriticalSection(&cs1);

			WaitForSingleObject(hCountEnterSectionEvent, INFINITE);
			EnterCriticalSection(&cs2);
			Assert::AreEqual(result, expected_result);
			LeaveCriticalSection(&cs2);
			for (int i = 0; i < n - result; ++i)
			{
				Assert::IsTrue(abs(a[i] - x) < 1e-6);
			}
			for (int i = n - result; i < n; ++i)
			{
				Assert::IsFalse(abs(a[i] - x) < 1e-6);
			}
			DeleteCriticalSection(&cs2);
			DeleteCriticalSection(&cs2);
		}
		TEST_METHOD(TestGenerator)
		{
			hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hCountEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			hWorkEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			srand(time(NULL));
			std::vector<double> a;
			n = rand() % 10 + 5;
			for (int i = 0; i < n; ++i) {
				a.push_back(rand() % 31);
			}
			t = 0;
			x = a[rand() % n];
			int expected_result = CountElement2(a);
			InitializeCriticalSection(&cs1);
			InitializeCriticalSection(&cs2);
			HANDLE work = CreateThread(NULL, 0, Work, &a, 0, NULL);
			HANDLE count_element = CreateThread(NULL, 0, CountElement, &a, 0, NULL);

			WaitForSingleObject(hWorkEnterSectionEvent, INFINITE);
			SetEvent(hWorkEvent);

			EnterCriticalSection(&cs1);
			for (auto i : a)
			{
				Sleep(5);
			}
			std::cout << '\n';
			LeaveCriticalSection(&cs1);

			WaitForSingleObject(hCountEnterSectionEvent, INFINITE);
			EnterCriticalSection(&cs2);
			Assert::AreEqual(result, expected_result);
			LeaveCriticalSection(&cs2);
			for (int i = 0; i < n - result; ++i)
			{
				Assert::IsTrue(abs(a[i] - x) < 1e-6);
			}
			for (int i = n - result; i < n; ++i)
			{
				Assert::IsFalse(abs(a[i] - x) < 1e-6);
			}
			DeleteCriticalSection(&cs2);
			DeleteCriticalSection(&cs2);
		}
	};
}
