#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string.h>

struct Array
{
	int n;
	char arr[51];
};

int main(int argc, char* argv[])
{
	HANDLE hReadChannelReady = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCWSTR)"WRITE_CHANNEL_READY");
	HANDLE hWriteChannelReady = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCWSTR)"READ_CHANNEL_READY");
	//HANDLE hWriteChannelHasBeenRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCWSTR)"READ_CHANNEL_HAS_BEEN_READ");

	HANDLE hWritePipe = (HANDLE)atoi(argv[1]), hReadPipe = (HANDLE)atoi(argv[0]);
	int dwBytesWritten = 0;
	Array arr;

	WaitForSingleObject(hReadChannelReady, INFINITE);

	if (!ReadFile(hReadPipe, (char*)&arr, sizeof(Array), (LPDWORD)&dwBytesWritten, NULL))
	{
		_cputs("Read from file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	Array result;
	int count = 0;
	for (int i = 0; i < arr.n; ++i)
	{
		if (arr.arr[i] >= 'a' && arr.arr[i] <= 'z' || arr.arr[i] >= 'A' && arr.arr[i] <= 'Z')
		{
			result.arr[count++] = arr.arr[i];
		}
	}
	result.arr[count] = 0;
	result.n = count;

	std::cout << "Original array:\nSize: " << arr.n << "\nArray: " << arr.arr << '\n';
	std::cout << "New array:\nSize: " << result.n << "\nArray: " << result.arr << '\n';

	if (!WriteFile(hWritePipe, &(result.n), sizeof(int), (LPDWORD)&dwBytesWritten, NULL))
	{
		_cputs("Write to file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	SetEvent(hWriteChannelReady);

	for (int i = 0; i <= result.n; ++i)
	{
		WaitForSingleObject(hReadChannelReady, INFINITE);
		if (!WriteFile(hWritePipe, &(result.arr[i]), sizeof(result.arr[i]), (LPDWORD)&dwBytesWritten, NULL))
		{
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		SetEvent(hWriteChannelReady);
		
	}

	_cputs("Press any key to finish.\n");
	_getch();
	return 0;
}