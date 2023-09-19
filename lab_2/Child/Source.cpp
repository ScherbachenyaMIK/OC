#include <windows.h>
#include <conio.h>
#include <iostream>

void MergeSort(int* arr, int left_border, int right_border)	//merge sort
{
	if (left_border == right_border)	//if subarray has only 1 element, recursion ends 
	{
		return;
	}
	int mid = (left_border + right_border) / 2;	//divide the array into 2 parts
	MergeSort(arr, left_border, mid);			//sort each of them
	MergeSort(arr, mid + 1, right_border);
	int* sub_arr = new int[right_border - left_border + 1];	//calculate midddle element
	int i = left_border, j = mid + 1, k = 0;
	while (i <= mid && j <= right_border) //until one of the two parts ends 
	{
		if (arr[i] > arr[j])	//take smaller element between two first not already used elements of subarrays
		{
			sub_arr[k] = arr[j];
			k++;
			j++;
		}
		else
		{
			sub_arr[k] = arr[i];
			k++;
			i++;
		}
	}
	if (i > mid)	//copy remaining elements
	{				//copy subarray to main array
		while (j <= right_border)
		{
			sub_arr[k] = arr[j];
			k++;
			j++;
		}
		for (int i = left_border; i <= right_border; i++)
		{
			arr[i] = sub_arr[i - left_border];
		}
	}
	else
	{
		while (i <= mid)
		{
			sub_arr[k] = arr[i];
			k++;
			i++;
		}
		for (int i = left_border; i <= right_border; i++)
		{
			arr[i] = sub_arr[i - left_border];
		}
	}
	delete[] sub_arr;
}

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