#include <windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>

int main()
{
	HANDLE new_message_prepared = OpenEvent(EVENT_ALL_ACCESS, TRUE, (LPCWSTR)"NEW_MESSAGE");
	HANDLE* events;
	events = new HANDLE[6];
	std::string s = "Event_0";
	
	HANDLE semaphore;
	HANDLE end_semaphore;
	semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, (LPCWSTR)"Employees activated process");
	end_semaphore = OpenSemaphore(SEMAPHORE_MODIFY_STATE, TRUE, (LPCWSTR)"Employees ended event");

	for (int i = 0; i < 6; ++i)
	{
		events[i] = OpenEvent(EVENT_ALL_ACCESS, TRUE, (LPCWSTR)s.c_str());
		s[s.size() - 1]++;
	}
	WaitForSingleObject(semaphore, INFINITE);
	bool q = 0;
	std::ifstream fin;
	while (!q)
	{
		WaitForSingleObject(new_message_prepared, INFINITE);
		switch (WaitForMultipleObjects(6, events, FALSE, INFINITE) - WAIT_OBJECT_0)
		{
		case 0:
			fin.open("Exchanger.txt");
			getline(fin, s);
			std::cout << s << '\n';
			fin.close();
			ReleaseSemaphore(end_semaphore, 1, NULL);
			break;
		case 1:
			fin.open("Exchanger.txt");
			getline(fin, s);
			std::cout << s << '\n';
			fin.close();
			ReleaseSemaphore(end_semaphore, 1, NULL);
			break;
		case 2:
			fin.open("Exchanger.txt");
			getline(fin, s);
			std::cout << s << '\n';
			fin.close();
			ReleaseSemaphore(end_semaphore, 1, NULL);
			break;
		case 3:
			fin.open("Exchanger.txt");
			getline(fin, s);
			std::cout << s << '\n';
			fin.close();
			ReleaseSemaphore(end_semaphore, 1, NULL);
			break;
		case 4:
			fin.open("Exchanger.txt");
			getline(fin, s);
			std::cout << s << '\n';
			fin.close();
			ReleaseSemaphore(end_semaphore, 1, NULL);
			break;
		case 5:
			fin.open("Exchanger.txt");
			getline(fin, s);
			std::cout << s;
			std::cout << "\npress any key, to close process...";
			q = 1;
			fin.close();
			ReleaseSemaphore(semaphore, 1, NULL);
			_getch();
			ReleaseSemaphore(end_semaphore, 1, NULL);
			return 0;
			break;
		default:
			break;
		}
	}
	_getch();
	ReleaseSemaphore(semaphore, 1, NULL);
	return 0;
}