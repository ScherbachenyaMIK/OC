#include <Windows.h>
#include <process.h>
#include <iostream>

struct Array
{
	int* a;
	int n;
	int result;
	Array(int* a_, int n_) : a(a_), n(n_), result(0) {}
};

//DWORD WINAPI worker(LPVOID param)
unsigned __stdcall worker(void* param)
{
	double x;
	int result = 0;
	std::cout << "Enter x: ";
	std::cin >> x;
	for (int i = 0; i < ((Array*)param)->n; ++i)
	{
		if (((Array*)param)->a[i] == trunc(x))
		{
			++result;
		}
	}
	((Array*)param)->result = result;
	return 0;
}

int main()
{
	HANDLE hThread;
	DWORD IDThread;

	int n;
	std::cout << "Enter count of elements: ";
	std::cin >> n;
	int* a = new int[n];
	std::cout << "Enter elements:\n";
	for(int i = 0; i < n; ++i)
	{
		std::cin >> a[i];
	}
	int time;
	std::cout << "Enter time stop time (s): ";
	std::cin >> time;

	auto param = new Array(a, n);
	//hThread = CreateThread(nullptr, 0, worker, (LPVOID)param, 0, &IDThread);
	hThread = (HANDLE)_beginthreadex(NULL, 0, &worker, (void*)param, 0, (unsigned int*)&IDThread);
	if (hThread == NULL)
		return GetLastError();
	SuspendThread(hThread);
	Sleep(time * 1000);
	ResumeThread(hThread);
	if (WaitForSingleObject(hThread, INFINITE) != WAIT_OBJECT_0)
	{
		std::cout << "Wait for single object failed." << std::endl;
	}
	else
	{
		std::cout << "Result: " << param->result << '\n';
	}
	CloseHandle(hThread);
	return 0;
}