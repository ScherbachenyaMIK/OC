#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string.h>

struct Array
{
	int n;
	char arr[51];
};

void MatchLetters(const Array& arr, Array& result)
{
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
}

int main(int argc, char* argv[])
{
	HANDLE hWritePipe = reinterpret_cast<HANDLE>(atoi(argv[1])), 
		hReadPipe = reinterpret_cast<HANDLE>(atoi(argv[0]));			//reading arguments from console
	int dwBytesWritten = 0;
	Array arr;

	if (!ReadFile(hReadPipe, (char*)&arr, sizeof(Array), 
		reinterpret_cast<LPDWORD>(&dwBytesWritten), NULL))
	{
		_cputs("Read from file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	Array result;
	MatchLetters(arr, result);											//checking for letters

	std::cout << "Original array:\nSize: " << arr.n << "\nArray: " << arr.arr << '\n';
	std::cout << "New array:\nSize: " << result.n << "\nArray: " << result.arr << '\n';

	if (!WriteFile(hWritePipe, &(result.n), sizeof(int), 
		reinterpret_cast<LPDWORD>(&dwBytesWritten), NULL))
	{
		_cputs("Write to file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	for (int i = 0; i <= result.n; ++i)
	{
		if (!WriteFile(hWritePipe, &(result.arr[i]), sizeof(result.arr[i]), 
			reinterpret_cast<LPDWORD>(&dwBytesWritten), NULL))
		{
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
	}

	_cputs("Press any key to finish.\n");
	_getch();
	return 0;
}