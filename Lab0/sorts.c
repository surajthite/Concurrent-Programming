
#include "sorts.h"

/*
Name : merges
Description : Function to merge two lists passed as arguments
INput: Array to be sorted , start , mid and end of the list
Return: void
*/
void merge (int arr[], int start, int mid, int end)
{

    int len_left = mid - start + 1;
    int len_right = end - mid;
    int temp_left[len_left];
    int temp_right[len_right];
    int i, j, k = 0;
    for (i = 0; i < len_left; i++)
    {
        temp_left[i] = arr[start + i];
    }

    for (j = 0; j < len_right; j++)
    {
        temp_right[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = start;

    while ((i < len_left) && (j < len_right))
    {
        if (temp_left[i] <= temp_right[j])
        {
            arr[k] = temp_left[i];
            i++;
        } else {
            arr[k] = temp_right[j];
            j++;
        }

        k++;
    }

    while (i < len_left)
    {
        arr[k] = temp_left[i];
        i++;
        k++;
    }

    while (i < len_right)
    {
        arr[k] = temp_right[j];
        j++;
        k++;
    }
}


/*
Name : mergesort
Description : Function to print the sorted array using merge sort algorithm
INput: Array, start and end of the lsit
Return: void
*/
void mergesort(int arr[], int start, int end)
{
    if (start < end)
    {
        int mid = ((start + end) / 2);
        mergesort(arr, start, mid);
        mergesort(arr, (mid + 1), end);
        merge(arr, start, mid, end);
    }
}



/*
Name : swap
Description : Function to swap two numbers passed as arguments
INput: Pointer to two numbers
Return: void
*/
void swap(int* x, int* y)
{
    int z = *x;
    *x = *y;
    *y = z;
}

/*
Name : swap
Description : Function to swap two numbers passed as arguments
INput: Pointer to two numbers
Return: void
*/

int partition (int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high- 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/*
Name : file_to_array
Description : Function to read the unsorted data from file to an array
INput: handler and array where the data is to be stored from the file
Return: -1 if error in opening a file , 0 if data has been sucessfully copied from file to an array
*/

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
