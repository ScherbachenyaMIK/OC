#include <windows.h>
#include <conio.h>

int main()
{
	char lpszAppName[] = "Parent.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if (!CreateProcessA(reinterpret_cast<LPCSTR>(lpszAppName), nullptr, nullptr, nullptr, FALSE,	//in all solution we use ANSI encoding
		CREATE_NEW_CONSOLE, nullptr, nullptr, reinterpret_cast<LPSTARTUPINFOA>(&si), &piApp))		//start the parent process
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	WaitForSingleObject(piApp.hProcess, INFINITE);								//and wait while it works
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	return 0;
}