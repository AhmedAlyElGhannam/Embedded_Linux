#include <stdio.h>

int binary_search(int *arr, int size, int key)
{
	int index = -1;
	int start = 0;
	int end = size - 1;
	int mid;
	
	while (start <= end)
	{
		mid = start + (end - start) / 2;
		
		if (arr[mid] == key)
		{
			index = mid;
			break;
		}
		else if (arr[mid] > key)
		{
			end = mid - 1;
		}
		else if (arr[mid] < key)
		{
			start = mid + 1;
		}
	}
	return index;
}

int main(void)
{
	int arr[] = {1, 12, 23, 34, 45, 56, 67, 78, 89, 100};
	
	printf("Number %d was found @ index %d\n", arr[3], binary_search(arr, sizeof(arr) / sizeof(int), 34));

	return 0;
}
