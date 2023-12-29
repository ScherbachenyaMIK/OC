#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>
#include <process.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Lab1Tests
{
	TEST_CLASS(Lab1Tests)
	{
	public:
		void TestProcess(int n, int* a, int time, int x, int expected_result) {
			HANDLE hReadPipe, hWritePipe;
			HANDLE hReadTestPipe, hWriteTestPipe;
			SECURITY_ATTRIBUTES saAttr;

			saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			saAttr.bInheritHandle = TRUE;
			saAttr.lpSecurityDescriptor = NULL;

			if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
				Assert::Fail(L"Error while creating pipe");
			}

			if (!CreatePipe(&hReadTestPipe, &hWriteTestPipe, &saAttr, 0)) {
				Assert::Fail(L"Error while creating pipe");
			}

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.hStdError = hWriteTestPipe;
			si.hStdOutput = hWriteTestPipe;
			si.hStdInput = hReadPipe;
			si.dwFlags |= STARTF_USESTDHANDLES;

			if (!CreateProcessA(reinterpret_cast<LPCSTR>("lab_1.exe"), NULL, NULL, NULL,
				TRUE, 0, NULL, NULL, reinterpret_cast<LPSTARTUPINFOA>(&si), &pi)) {
				Assert::Fail(L"Failed to create process");
			}

			CloseHandle(hReadPipe);
			CloseHandle(hWriteTestPipe);

			char* input = new char[10 * (n + 3) + n + 3 + 1];
			char* element = new char[11];

			wsprintfA(input, "%d ", n);
			for (int i = 0; i < n; ++i)
			{
				wsprintfA(element, "%d ", a[i]);
				strcat(input, element);
			}
			wsprintfA(element, "%d ", time);
			strcat(input, element);
			wsprintfA(element, "%d ", x);
			strcat(input, element);

			DWORD dwBytesWritten;

			if (!WriteFile(hWritePipe, input, strlen(input),
				reinterpret_cast<LPDWORD>(&dwBytesWritten), NULL))
			{
				Assert::Fail(L"Error while writing pipe");
			}

			delete[] input;
			delete[] element;

			WaitForSingleObject(pi.hProcess, INFINITE);

			char buf[4096];
			DWORD dwBytesRead;

			ReadFile(hReadTestPipe, buf, sizeof(buf), &dwBytesRead, NULL);
			int i = 85;
			char result[10];
			for (; buf[i] != '\\'; ++i)
			{
				result[i - 85] = buf[i];
			}

			Assert::AreEqual(expected_result, atoi(result));

			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		TEST_METHOD(Test1)
		{
			int n = 5;
			int a[] = { 1, 5, -3, -3, 5 };
			int time = 0;
			int x = -3;
			int expected_result = 2;
			
			TestProcess(n, a, time, x, expected_result);
		}

		TEST_METHOD(Test2)
		{
			int n = 5;
			int a[] = { 1, 5, -3, -3, 5 };
			int time = 0;
			int x = 5;
			int expected_result = 2;

			TestProcess(n, a, time, x, expected_result);
		}

		TEST_METHOD(Test3)
		{
			int n = 5;
			int a[] = { 1, 5, -3, -3, 5 };
			int time = 0;
			int x = 1;
			int expected_result = 1;

			TestProcess(n, a, time, x, expected_result);
		}

		TEST_METHOD(Test4)
		{
			int n = 5;
			int a[] = { 1, 5, -3, -3, 5 };
			int time = 0;
			int x = 0;
			int expected_result = 0;

			TestProcess(n, a, time, x, expected_result);
		}

		TEST_METHOD(Test5)
		{
			int n = 15;
			int a[] = { 1, 1, -3, -3, 1, 1, 1, -3, -3, 1, 1, 1, -3, -3, 1 };
			int time = 0;
			int x = 1;
			int expected_result = 9;

			TestProcess(n, a, time, x, expected_result);
		}

		TEST_METHOD(Test6)
		{
			int n = 15;
			int a[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int time = 0;
			int x = 0;
			int expected_result = 15;

			TestProcess(n, a, time, x, expected_result);
		}
	};
}
