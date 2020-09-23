

/***************************************************************************************************
MIT License

Copyright (c) 2018

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************************************/

/*
File_Name : sorts.c
Author: Suraj Thite
Description : This file contains the functions for merge sort and quick sort required for lab0
Reference : The algorithms have been referenced from https://www.geeksforgeeks.org/quick-sort/ , https://www.geeksforgeeks.org/merge-sort/
*/


#include "sorts.h"
using namespace std;
/*
Name : merge
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
        temp_left[i] = arr[start + i]; //Copy the data into left array
    }

    for (j = 0; j < len_right; j++)
    {
        temp_right[j] = arr[mid + 1 + j]; //Copy the data into right array
    }

    i = 0;
    j = 0;
    k = start;

    while ((i < len_left) && (j < len_right))
    {
        if (temp_left[i] <= temp_right[j]) //if left data < right data store temp[left] in array else store temp[right]
        {
            arr[k] = temp_left[i];
            i++;
        } else {
            arr[k] = temp_right[j];
            j++;
        }

        k++;
    }

    while (i < len_left) //Copy the remaining data from temp_left
    {
        arr[k] = temp_left[i];
        i++;
        k++;
    }

    while (i < len_right) //Copy the remaining data from temp_right
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
        int mid = ((start + end) / 2); //Detemine the mid of the array
        mergesort(arr, start, mid); //Merge sort the bottom half
        mergesort(arr, (mid + 1), end); //Merge sort the upper healf
        merge(arr, start, mid, end); //merge both the orted arrays
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
Name : partition
Description : Function to partition the given set of data passed as arguments
INput: Array, high and low values
Return: index to an array
*/

int partition (vector<int>& arr, int low, int high)
{
    int pivot = arr[high]; //Detemine the pivoot
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
Name : quickSort
Description : Function to do a recursive quick sort
INput: Array, High and low values
Return: void
*/

void quickSort(vector<int>& arr, int head, int tail)
{
    if (head < tail)
    {
        int p = partition(arr, head, tail); // Partition the array with head and tail as arguments
        quickSort(arr, head, p - 1);  //Quick Sort the first partition
        quickSort(arr, p + 1, tail); //Quick sort the second parition
    }
}
