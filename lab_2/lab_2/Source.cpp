#include <windows.h>
#include <conio.h>
#include <iostream>

int main()
{
	int n;
	std::cout << "Enter count of elements: ";
	std::cin >> n;
	auto a = new int[n];
	std::cout << "Enter elements:\n";
	for (int i = 0; i < n; ++i)
	{
		std::cin >> a[i];

	}
	std::cout << "Enter:\n\"B\" to set blue background\n\"R\" to set red background\n\"G\" to set green background\nelse black background will be set\n";
	char c;
	std::cin >> c;
	char lpszAppName[] = "Child.exe";
	char args[1000], element[10];
	wsprintfA(args, "%d ", n);
	for (int i = 0; i < n; ++i)
	{
		wsprintfA(element, "%d ", a[i]);	//write down number to the string
		strcat_s(args, element);			//then concatenate them
	}
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USEFILLATTRIBUTE;	//set flag
	switch (c)								//and then set parameter
	{
	case 'B':
		si.dwFillAttribute = BACKGROUND_BLUE;
		break;
	case 'G':
		si.dwFillAttribute = BACKGROUND_GREEN;
		break;
	case 'R':
		si.dwFillAttribute = BACKGROUND_RED;
		break;
	default:
		break;
	}
	if (!CreateProcessA(reinterpret_cast<LPCSTR>(lpszAppName), args, nullptr, nullptr, FALSE,		//in this case first argument is size of array
		CREATE_NEW_CONSOLE, nullptr, nullptr, reinterpret_cast<LPSTARTUPINFOA>(&si), &piApp))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	delete[] a;
	return 0;
}