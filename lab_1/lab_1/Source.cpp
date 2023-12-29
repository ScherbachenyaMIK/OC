#include <Windows.h>
#include <process.h>
#include <iostream>

struct Array
{
	int* a;
	int n;
	int result = 0;		//the result will be stored in this variable
	Array(int* a_, int n_) : a(a_), n(n_) {}
};

//DWORD WINAPI worker(LPVOID param)					//name of function, if we use "another way"
unsigned __stdcall worker(void* param)
{
	double x;
	int result = 0;
	std::cout << "Enter x: ";
	std::cin >> x;
	for (int i = 0; i < (reinterpret_cast<Array*>(param))->n; ++i)
	{
		if ((reinterpret_cast<Array*>(param))->a[i] == static_cast<int>(trunc(x)))		//compare elements of array with rounded down x value
		{
			++result;
		}
	}
	(reinterpret_cast<Array*>(param))->result = result;
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
	/*hThread = CreateThread(nullptr,							//another way to open thread
							0, worker, 
							reinterpret_cast<LPVOID>(param), 
							0, &IDThread);*/
	hThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, &worker,
									reinterpret_cast<void*>(param), 0,
									reinterpret_cast<unsigned int*>(&IDThread)));
	if (hThread == nullptr)
		return GetLastError();
	SuspendThread(hThread);
	Sleep(time * 1000);			//suspend thread for the time entered by the user
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
	delete[] a;
	delete param;
	return 0;
}