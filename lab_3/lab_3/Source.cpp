#include <iostream>
#include <windows.h>
#include <vector>
#include <process.h>
#include <ctime>
#include <algorithm>

CRITICAL_SECTION cs1, cs2;

HANDLE hWorkEvent;

int t;
double x;
int result = 1;
int n;

DWORD WINAPI Work(LPVOID lpParam) 
{
	EnterCriticalSection(&cs1);
	EnterCriticalSection(&cs2);
	WaitForSingleObject(hWorkEvent, INFINITE);
	int j = 0, k = n - 1;
	std::vector<double> b(n);
	for (int i = 0; i < n; ++i)
	{
		if (abs(((std::vector<double>*)lpParam)->at(i) - x) < 1e-6)
		{
			b[j] = ((std::vector<double>*)lpParam)->at(i);
			++j;
		}
		else
		{
			b[k] = ((std::vector<double>*)lpParam)->at(i);
			--k;
		}
	}
	swap(*((std::vector<double>*)lpParam), b);
	for (int i = 0; i < n; ++i)
	{
		std::cout << ((std::vector<double>*)lpParam)->at(i) << ' ';
		Sleep(t * 1000);
	}
	std::cout << '\n';
	LeaveCriticalSection(&cs1);
	LeaveCriticalSection(&cs2);
	return 0;
}

DWORD WINAPI CountElement(LPVOID lpParam) 
{
	EnterCriticalSection(&cs2);
	WaitForSingleObject(hWorkEvent, INFINITE);
	if (abs(((std::vector<double>*)lpParam)->at(0) - x) > 1e-6)
	{
		result = n;
		return 0;
	}
	int count = 0;
	for (int i = 0; i < n; ++i)
	{
		if (abs(((std::vector<double>*)lpParam)->at(i) - x) > 1e-6)
		{
			break;
		}
		++count;
	}
	result = n - count;
	LeaveCriticalSection(&cs2);
	return 0;
}

int main() 
{	
	hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hWorkEvent == NULL)
		return GetLastError();

	srand(time(NULL));
	std::vector<double> a;
	//cout << "Enter array size: ";
	n = rand() % 10 + 5;
	//cin >> n;
	//cout << "Enter elements: ";
	for (int i = 0; i < n; ++i) {
		//double element;
		//cin >> element;
		a.push_back(rand() % 31);
	}
	std::cout << "Array size: " << n << '\n';
	std::cout << "Array: ";
	for (int i = 0; i < n; ++i) {
		std::cout << a[i] << " ";
	}
	std::cout << '\n';
	std::cout << "Enter time to sleep after processing one element (s): ";
	std::cin >> t;
	InitializeCriticalSection(&cs1);
	InitializeCriticalSection(&cs2);
	HANDLE work = CreateThread(NULL, 0, Work, &a, 0, NULL);
	HANDLE count_element = CreateThread(NULL, 0, CountElement, &a, 0, NULL);
	
	
	std::cout << "Enter element for searching: ";
	std::cin >> x;

	SetEvent(hWorkEvent);
	
	EnterCriticalSection(&cs1);
	for (auto i : a)
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';
	LeaveCriticalSection(&cs1);

	EnterCriticalSection(&cs2);
	std::cout << "Count of elements not equals to X: " << result << '\n';
	LeaveCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	return 0;
}