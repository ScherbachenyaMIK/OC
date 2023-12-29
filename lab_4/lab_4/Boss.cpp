#include <windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>

int main()
{
	HANDLE new_message_prepared = CreateEvent(NULL, TRUE, TRUE, reinterpret_cast <LPCWSTR>("NEW_MESSAGE"));
	HANDLE* events;
	events = new HANDLE[6];
	std::string s = "Event_0";
	for (int i = 0; i < 6; ++i)
	{
		events[i] = CreateEvent(NULL, TRUE, FALSE, reinterpret_cast<LPCWSTR>(s.c_str()));
		s[s.size() - 1]++;					//inc last character to create another event
	}
	
	HANDLE semaphore;
	HANDLE end_semaphore;
	semaphore = CreateSemaphore(NULL, 3, 3, reinterpret_cast <LPCWSTR>("Employees activated process"));
	end_semaphore = CreateSemaphore(NULL, -2, 1, reinterpret_cast <LPCWSTR>("Employees ended event"));

	int n, open_count;
	std::cout << "Enter count of processes Employee: ";
	std::cin >> n;
	open_count = n;
	char lpszAppName[] = "Employee.exe";
	
	int count_of_messages;
	std::cout << "Enter count of messages to employees: ";
	std::cin >> count_of_messages;

	STARTUPINFO* si = new STARTUPINFO[n];
	PROCESS_INFORMATION* piApp = new PROCESS_INFORMATION[n];

	for (int i = 0; i < n; ++i)
	{
		ZeroMemory(&(si[i]), sizeof(STARTUPINFO));
		if (!CreateProcessA(reinterpret_cast <LPCSTR>(lpszAppName), nullptr, nullptr, nullptr, FALSE,		//start process without arguments
			CREATE_NEW_CONSOLE, nullptr, nullptr, reinterpret_cast <LPSTARTUPINFOA>(&(si[i])), &(piApp[i])))//with using ANSI encoding
		{
			_cputs("The new process is not created.\n");
			_cputs("Check a name of the process.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return 0;
		}
	}

	std::string EventSequence;
	std::cout << "Enter message types (like a string, for example '0123'):\n\'0\': Enable event 0\n\'1\': Enable event 1\n\'2\': Enable event 2\n\'3\': Enable event 3\n\'4\': Enable event 4\n\'e\': Close some (3) processes\n";
	std::cin >> EventSequence;					//entering of event sequence
	std::string message;
	std::ofstream fout;
	for (int i = 0; i < count_of_messages; ++i)	//and compute each of them
	{
		fout.open("Exchanger.txt");
		std::cout << "Enter message content:\n";
		std::cin >> message;
		switch (EventSequence[i])
		{
		case '0':
			fout << "Event 0 send next message: " + message;
			fout.close();
			ResetEvent(new_message_prepared);				//occupy an event to prevent processes 
															//that have finished their work from continuing their work
			
			SetEvent(events[0]);							//send type of event
			
			WaitForSingleObject(end_semaphore, INFINITE);	//when all 3 processes finished
			WaitForSingleObject(end_semaphore, 0);			//return semaphore to its original state
			WaitForSingleObject(end_semaphore, 0);			//(counter == -2)
			ResetEvent(events[0]);
			break;
		case '1':
			fout << "Event 1 send next message: " + message;
			fout.close();
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

		Sleep(50);
		SetEvent(new_message_prepared);		//then after a short delay we can release the processes
		if (open_count < 3)
		{
			std::cout << "The number of open processes is less than the number of processes required to continue working\n";
			std::cout << "All processes will be terminated\n";
			std::cout << "Close all consoles and press any key to finish...\n";
			_getch();
			break;
		}
	}

	fout.open("Exchanger.txt");
	fout << "All messages have been forwarded, press any key, to close process...";
	fout.close();
	SetEvent(events[5]);					//close all processes via existing event mechanism
	SetEvent(new_message_prepared);
	for (int i = 0; i < n; ++i)
	{
		WaitForSingleObject(piApp[i].hProcess, INFINITE);
		CloseHandle(piApp[i].hThread);
		CloseHandle(piApp[i].hProcess);
	}
	CloseHandle(semaphore);
	CloseHandle(end_semaphore);
	for (int i = 0; i < 6; ++i)
	{
		CloseHandle(events[i]);
	}
	delete[] events;
	return 0;
}