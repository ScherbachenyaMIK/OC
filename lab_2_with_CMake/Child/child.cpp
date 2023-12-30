#include <windows.h>
#include <conio.h>
#include <iostream>
#include "MergeSort.h"

int main(int argc, char* argv[])
{
	int n = atoi(argv[0]);
	int* a = new int[n];
	for (int i = 0; i < n; ++i)
	{
		a[i] = atoi(argv[i + 1]);
	}
	MergeSort(a, 0, n - 1);
	std::cout << "Array after sorting:\n";
	for (int i = 0; i < n; ++i)
	{
		std::cout << a[i] << ' ';
	}
	std::cout << "\n\nPress any key to finish.\n";
	_getch();
	delete[] a;
	return 0;
}