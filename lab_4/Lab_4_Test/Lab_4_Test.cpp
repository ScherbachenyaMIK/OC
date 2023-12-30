#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>
#include <fstream>
#include <thread>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void SimulateBossProcess(char* args);
void TestProcess(const std::vector<int>& expected_output_sizes);

int passed_processes;
int enabled_processes_count;

namespace Lab4Test
{
	TEST_CLASS(Lab4Tests)
	{
	public:
		TEST_METHOD(Test1)
		{
			int proceeses = 3; 
			int messages = 4;
			std::string event_sequence = "1234";
			std::string messages_content[] = { "Hello", "World!", "I'm_work", "goodbye!" };
			std::vector<int> expected = { 247 };

			char* input = new char[11 * 2 + messages + messages * 101 + 3];
			char* element = new char[101];

			wsprintfA(input, "%d ", proceeses);
			wsprintfA(element, "%d ", messages);
			strcat(input, element);
			strcat(input, event_sequence.c_str());
			strcat(input, "\n");
			for (int i = 0; i < messages; ++i)
			{
				strcat(input, messages_content[i].c_str());
				strcat(input, "\n");
			}

			passed_processes = 0;
			enabled_processes_count = 0;

			std::thread boss(SimulateBossProcess, input);

			std::vector<std::thread*> employees;
			employees.reserve(proceeses);
			for (int i = 0; i < proceeses; ++i)
			{
				employees.push_back(new std::thread(TestProcess, cref(expected)));
			}

			boss.join();
			for (int i = 0; i < proceeses; ++i)
			{
				employees[i]->join();
			}

			delete[] input;
			delete[] element;

			Assert::AreEqual(3, passed_processes);
		}
		TEST_METHOD(Test2)
		{
			int proceeses = 8;
			int messages = 4;
			std::string event_sequence = "1234";
			std::string messages_content[] = { "Hello", "World!", "I'm_work", "goodbye!" };
			std::vector<int> expected = { 247 };

			char* input = new char[11 * 2 + messages + messages * 101 + 3];
			char* element = new char[101];

			wsprintfA(input, "%d ", proceeses);
			wsprintfA(element, "%d ", messages);
			strcat(input, element);
			strcat(input, event_sequence.c_str());
			strcat(input, "\n");
			for (int i = 0; i < messages; ++i)
			{
				strcat(input, messages_content[i].c_str());
				strcat(input, "\n");
			}

			passed_processes = 0;
			enabled_processes_count = 0;

			std::thread boss(SimulateBossProcess, input);

			std::vector<std::thread*> employees;
			employees.reserve(proceeses);
			for (int i = 0; i < proceeses; ++i)
			{
				employees.push_back(new std::thread(TestProcess, cref(expected)));
			}

			boss.join();
			for (int i = 0; i < proceeses; ++i)
			{
				employees[i]->join();
			}

			delete[] input;
			delete[] element;

			Assert::AreEqual(3, passed_processes);
		}
		TEST_METHOD(Test3)
		{
			int proceeses = 8;
			int messages = 9;
			std::string event_sequence = "012343210";
			std::string messages_content[9];
			std::string content("Do_work_1");
			std::vector<int> expected = { 446 };
			
			for (int i = 0; i < 9; ++i)
			{
				messages_content[i] = content;
				++content[content.size() - 1];
			}

			char* input = new char[11 * 2 + messages + messages * 101 + 3];
			char* element = new char[101];

			wsprintfA(input, "%d ", proceeses);
			wsprintfA(element, "%d ", messages);
			strcat(input, element);
			strcat(input, event_sequence.c_str());
			strcat(input, "\n");
			for (int i = 0; i < messages; ++i)
			{
				strcat(input, messages_content[i].c_str());
				strcat(input, "\n");
			}

			passed_processes = 0;
			enabled_processes_count = 0;

			std::thread boss(SimulateBossProcess, input);

			std::vector<std::thread*> employees;
			employees.reserve(proceeses);
			for (int i = 0; i < proceeses; ++i)
			{
				employees.push_back(new std::thread(TestProcess, cref(expected)));
			}

			boss.join();
			for (int i = 0; i < proceeses; ++i)
			{
				employees[i]->join();
			}

			delete[] input;
			delete[] element;

			Assert::AreEqual(3, passed_processes);
		}
		TEST_METHOD(Test4)
		{
			int proceeses = 8;
			int messages = 3;
			std::string event_sequence = "0e0";
			std::string messages_content[3];
			std::string content("Do_work_1");
			std::vector<int> expected = { 142, 113 };
			
			for (int i = 0; i < 3; ++i)
			{
				messages_content[i] = content;
				++content[content.size() - 1];
			}

			char* input = new char[11 * 2 + messages + messages * 101 + 3];
			char* element = new char[101];

			wsprintfA(input, "%d ", proceeses);
			wsprintfA(element, "%d ", messages);
			strcat(input, element);
			strcat(input, event_sequence.c_str());
			strcat(input, "\n");
			for (int i = 0; i < messages; ++i)
			{
				strcat(input, messages_content[i].c_str());
				strcat(input, "\n");
			}

			passed_processes = 0;
			enabled_processes_count = 0;

			std::thread boss(SimulateBossProcess, input);

			std::vector<std::thread*> employees;
			employees.reserve(proceeses);
			for (int i = 0; i < proceeses; ++i)
			{
				employees.push_back(new std::thread(TestProcess, cref(expected)));
			}

			boss.join();
			for (int i = 0; i < proceeses; ++i)
			{
				employees[i]->join();
			}

			delete[] input;
			delete[] element;

			Assert::AreEqual(6, passed_processes);
		}
		TEST_METHOD(Test5)
		{
			int proceeses = 3;
			int messages = 1;
			std::string event_sequence = "0";
			std::string messages_content[] = { "Do_work_1" };
			std::vector<int> expected = { 142 };

			char* input = new char[11 * 2 + messages + messages * 101 + 3];
			char* element = new char[101];

			wsprintfA(input, "%d ", proceeses);
			wsprintfA(element, "%d ", messages);
			strcat(input, element);
			strcat(input, event_sequence.c_str());
			strcat(input, "\n");
			for (int i = 0; i < messages; ++i)
			{
				strcat(input, messages_content[i].c_str());
				strcat(input, "\n");
			}

			passed_processes = 0;
			enabled_processes_count = 0;

			std::thread boss(SimulateBossProcess, input);

			std::vector<std::thread*> employees;
			employees.reserve(proceeses);
			for (int i = 0; i < proceeses; ++i)
			{
				employees.push_back(new std::thread(TestProcess, cref(expected)));
			}

			boss.join();
			for (int i = 0; i < proceeses; ++i)
			{
				employees[i]->join();
			}

			delete[] input;
			delete[] element;

			Assert::AreEqual(3, passed_processes);
		}

	};
}

void TestProcess(const std::vector<int>& expected_output_sizes)
{
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
	si.hStdInput = hReadPipe;
	si.hStdError = hWriteTestPipe;
	si.hStdOutput = hWriteTestPipe;
	si.dwFlags |= STARTF_USESTDHANDLES;

	if (!CreateProcessA(reinterpret_cast<LPCSTR>("Employee.exe"), NULL, NULL, NULL,
		TRUE, 0, NULL, NULL, reinterpret_cast<LPSTARTUPINFOA>(&si), &pi)) {
		Assert::Fail(L"Failed to create process");
	}

	CloseHandle(hReadPipe);
	CloseHandle(hWriteTestPipe);

	DWORD dwBytesWritten;
	WriteFile(hWritePipe, "\n", 1, &dwBytesWritten, NULL);

	++enabled_processes_count;

	WaitForSingleObject(pi.hProcess, INFINITE);

	--enabled_processes_count;

	char buf[4096];
	DWORD dwBytesRead;

	ReadFile(hReadTestPipe, buf, sizeof(buf), &dwBytesRead, NULL);

	for (int i = 0; i < expected_output_sizes.size(); ++i)
	{
		if (expected_output_sizes[i] == static_cast<int>(strlen(buf)))
		{
			++passed_processes;
		}
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void SimulateBossProcess(char* args)
{
	HANDLE new_message_prepared = CreateEvent(NULL, TRUE, TRUE, reinterpret_cast <LPCWSTR>("NEW_MESSAGE"));
	HANDLE* events;
	events = new HANDLE[6];
	std::string s = "Event_0";
	for (int i = 0; i < 6; ++i)
	{
		events[i] = CreateEvent(NULL, TRUE, FALSE, reinterpret_cast<LPCWSTR>(s.c_str()));
		s[s.size() - 1]++;
	}

	std::stringstream cin(args);

	HANDLE semaphore;
	HANDLE end_semaphore;
	semaphore = CreateSemaphore(NULL, 3, 3, reinterpret_cast <LPCWSTR>("Employees activated process"));
	end_semaphore = CreateSemaphore(NULL, -2, 1, reinterpret_cast <LPCWSTR>("Employees ended event"));

	int n, open_count;
	cin >> n;
	open_count = n;
	char lpszAppName[] = "Employee.exe";

	int count_of_messages;
	cin >> count_of_messages;

	while (enabled_processes_count != n);

	std::string EventSequence;
	cin >> EventSequence;
	std::string message;
	std::ofstream fout;
	for (int i = 0; i < count_of_messages; ++i)
	{
		fout.open("Exchanger.txt");
		cin >> message;
		switch (EventSequence[i])
		{
		case '0':
			fout << "Event 0 send next message: " + message;
			fout.close();

			Sleep(250);
			ResetEvent(new_message_prepared);

			SetEvent(events[0]);

			WaitForSingleObject(end_semaphore, INFINITE);
			WaitForSingleObject(end_semaphore, 0);
			WaitForSingleObject(end_semaphore, 0);
			ResetEvent(events[0]);
			break;
		case '1':
			fout << "Event 1 send next message: " + message;
			fout.close();

			Sleep(250);
			ResetEvent(new_message_prepared);

			SetEvent(events[1]);

			WaitForSingleObject(end_semaphore, INFINITE);
			WaitForSingleObject(end_semaphore, 0);
			WaitForSingleObject(end_semaphore, 0);
			ResetEvent(events[1]);
			break;
		case '2':
			fout << "Event 2 send next message: " + message;
			fout.close();

			Sleep(250);
			ResetEvent(new_message_prepared);

			SetEvent(events[2]);

			WaitForSingleObject(end_semaphore, INFINITE);
			WaitForSingleObject(end_semaphore, 0);
			WaitForSingleObject(end_semaphore, 0);
			ResetEvent(events[2]);
			break;
		case '3':
			fout << "Event 3 send next message: " + message;
			fout.close();

			Sleep(250);
			ResetEvent(new_message_prepared);

			SetEvent(events[3]);

			WaitForSingleObject(end_semaphore, INFINITE);
			WaitForSingleObject(end_semaphore, 0);
			WaitForSingleObject(end_semaphore, 0);
			ResetEvent(events[3]);
			break;
		case '4':
			fout << "Event 4 send next message: " + message;
			fout.close();

			Sleep(250);
			ResetEvent(new_message_prepared);

			SetEvent(events[4]);

			WaitForSingleObject(end_semaphore, INFINITE);
			WaitForSingleObject(end_semaphore, 0);
			WaitForSingleObject(end_semaphore, 0);
			ResetEvent(events[4]);
			break;
		case 'e':
			open_count -= 3;
			fout << "Exit event send next message: " + message;
			fout.close();

			Sleep(250);
			ResetEvent(new_message_prepared);

			SetEvent(events[5]);

			WaitForSingleObject(end_semaphore, INFINITE);
			WaitForSingleObject(end_semaphore, 0);
			WaitForSingleObject(end_semaphore, 0);
			ResetEvent(events[5]);
			break;
		default:
			break;
		}

		Sleep(250);
		SetEvent(new_message_prepared);
		if (open_count < 3)
		{
			break;
		}
	}

	fout.open("Exchanger.txt");
	fout << "All messages have been forwarded, press any key, to close process...";
	fout.close();
	SetEvent(events[5]);
	SetEvent(new_message_prepared);

	while (enabled_processes_count != 0);

	CloseHandle(semaphore);
	CloseHandle(end_semaphore);
	for (int i = 0; i < 6; ++i)
	{
		CloseHandle(events[i]);
	}
	delete[] events;
}