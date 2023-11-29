#include <iostream>
#include <windows.h>
#include <vector>
#include <process.h>
#include <ctime>
#include <algorithm>
#include "Header.h"
#include "Variables.h"

DWORD WINAPI Work(LPVOID lpParam) 
{
	EnterCriticalSection(&cs1);
	EnterCriticalSection(&cs2);				//First we occupy the sections
	SetEvent(hWorkEnterSectionEvent);		//Then release main
	WaitForSingleObject(hWorkEvent, INFINITE);
	int j = 0, k = n - 1;
	std::vector<double> b(n);
	for (int i = 0; i < n; ++i)
	{
		if (abs(((std::vector<double>*)lpParam)->at(i) - x) < 1e-6)	//copy all elements equals to x at
		{															//beginning of array
			b[j] = ((std::vector<double>*)lpParam)->at(i);
			++j;
		}
		else														//others at the end
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
	if (abs(((std::vector<double>*)lpParam)->at(0) - x) > 1e-6)		//if first element not equals return 0
	{
		result = n;
		LeaveCriticalSection(&cs2);
		return 0;
	}
	int count = 0;
	for (int i = 0; i < n; ++i)										//while vector[i] equals to x count	 
	{																//elements
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

	WaitForSingleObject(hWorkEnterSectionEvent, INFINITE);	//Waiting while 'work' thread enter sections
	SetEvent(hWorkEvent);									//Starting of processes
	EnterCriticalSection(&cs1);								//Waiting while thread 'work' finishes
	for (auto i : a)
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';
	LeaveCriticalSection(&cs1);

	WaitForSingleObject(hCountEnterSectionEvent, INFINITE);	//Waiting while 'count' thread enter sections
	EnterCriticalSection(&cs2);								//Waiting while thread 'count' finishes
	std::cout << "Count of elements not equals to X: " << result << '\n';
	LeaveCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	return 0;
}