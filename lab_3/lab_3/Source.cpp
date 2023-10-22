#include <iostream>
#include <windows.h>
#include <vector>
#include <process.h>
#include <ctime>
#include <algorithm>
#include "Header.h"
#include "Variables.h"

//extern CRITICAL_SECTION cs1, cs2;
//
//extern HANDLE hWorkEvent;
//extern HANDLE hCountEnterSectionEvent;
//extern HANDLE hWorkEnterSectionEvent;
//extern HANDLE count_element;
//extern HANDLE work;
//
//extern int t;
//extern double x;
//extern int result;
//extern int n;

DWORD WINAPI Work(LPVOID lpParam) 
{
	EnterCriticalSection(&cs1);
	EnterCriticalSection(&cs2);
	SetEvent(hWorkEnterSectionEvent);
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
	LeaveCriticalSection(&cs2);
	LeaveCriticalSection(&cs1);
	return 0;
}

DWORD WINAPI CountElement(LPVOID lpParam) 
{
	WaitForSingleObject(hWorkEvent, INFINITE);
	EnterCriticalSection(&cs2);
	SetEvent(hCountEnterSectionEvent);
	if (abs(((std::vector<double>*)lpParam)->at(0) - x) > 1e-6)
	{
		result = n;
		LeaveCriticalSection(&cs2);
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
	hCountEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hWorkEnterSectionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
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
	work = CreateThread(NULL, 0, Work, &a, 0, NULL);
	count_element = CreateThread(NULL, 0, CountElement, &a, 0, NULL);
	
	
	std::cout << "Enter element for searching: ";
	std::cin >> x;

	WaitForSingleObject(hWorkEnterSectionEvent, INFINITE);
	SetEvent(hWorkEvent);
	EnterCriticalSection(&cs1);
	for (auto i : a)
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';
	LeaveCriticalSection(&cs1);

	WaitForSingleObject(hCountEnterSectionEvent, INFINITE);
	EnterCriticalSection(&cs2);
	std::cout << "Count of elements not equals to X: " << result << '\n';
	LeaveCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	return 0;
}