#include <iostream>
#include <windows.h>
#include <conio.h>

struct Array
{
	int n;
	char arr[51];
};

int main()
{
	Array arr;
	std::cout << "Enter array size: ";
	std::cin >> arr.n;
	std::cout << "Enter array of chars:\n";
	std::cin >> arr.arr;

	HANDLE hWritePipe, hReadPipe;
	HANDLE hWritePipe2, hReadPipe2;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;				//set inheritance property to pipe
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	if (!CreatePipe(&hReadPipe2, &hWritePipe2, &sa, 0))
	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	char lpszAppName[] = "Alfavit.exe";
	char lpszParameter[20];
	sprintf(lpszParameter, "%d %d", reinterpret_cast<int>(hReadPipe), reinterpret_cast<int>(hWritePipe2));

	STARTUPINFO si;
	PROCESS_INFORMATION piApp;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	if (!CreateProcessA(reinterpret_cast<LPCSTR>(lpszAppName), lpszParameter, nullptr, nullptr, TRUE, //start process with
		CREATE_NEW_CONSOLE, nullptr, nullptr, reinterpret_cast<LPSTARTUPINFOA>(&si), &piApp))		  //using ANSI encoding
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}

	int dwBytesWritten = 0;
	if (!WriteFile(hWritePipe, reinterpret_cast<char*>(&arr), sizeof(Array), 
		reinterpret_cast<LPDWORD>(&dwBytesWritten), NULL))
	{
		_cputs("Write to file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	Array result;
	if (!ReadFile(hReadPipe2, &(result.n), sizeof(int), 
		reinterpret_cast<LPDWORD>(&dwBytesWritten), NULL))
	{
		_cputs("Read from file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	for (int i = 0; i <= result.n; ++i)
	{
		if (!ReadFile(hReadPipe2, &(result.arr[i]), sizeof(result.arr[i]), 
			reinterpret_cast<LPDWORD>(&dwBytesWritten), NULL))
		{
			_cputs("Read from file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
	}

	std::cout << "Letters:\n";
	for (int i = 0; i < result.n; ++i)
	{

		std::cout << '\'' << result.arr[i] << "\' ";
	}

	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe2);
	CloseHandle(hReadPipe2);
	return 0;
}