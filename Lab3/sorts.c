

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

/*
Name : merge
Description : Function to merge two lists passed as arguments
INput: Array to be sorted , start , mid and end of the list
Return: void
*/
void merge(int array[], int low, int mid, int high)
{
    // printf("Inside merge\n");
    int i, j, k;
    int r1 = mid - low + 1;
    int r2 =  high - mid;

    int lh[r1], uh[r2];

    for (i = 0; i < r1; i++)
        lh[i] = array[low + i];
    for (j = 0; j < r2; j++)
        uh[j] = array[mid + 1+ j];

    i = 0;
    j = 0;
    k = low;

    while (i < r1 && j < r2)
    {
        if (lh[i] <= uh[j])
        {
            array[k] = lh[i];
            i++;
        }
        else
        {
            array[k] = uh[j];
            j++;
        }
        k++;
    }

    while (i < r1)
    {
        array[k] = lh[i];
        i++;
        k++;
    }

    while (j < r2)
    {
        array[k] = uh[j];
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
