#include "pch.h"
#include "CppUnitTest.h"
#include "../lab_3/Header.h"

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
		if (abs(a[i] - x) < 1e-6)
		{
			++count;
		}
	}
	return n - count;
}

namespace Lab3Test
{
	TEST_CLASS(Lab3Test)
	{
	public:
		
		TEST_METHOD(TestCount)
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
				std::cout << i << ' ';
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
			DeleteCriticalSection(&cs2);
			DeleteCriticalSection(&cs2);
		}
	};
}
