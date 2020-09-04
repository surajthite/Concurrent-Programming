#ifndef __SORTS_H__
#define __SORTS_H__

/*
Name : merge
Description : Function to write the sorted data to the file*
INput: handler and array where the data is to be written to the file
Return: -1 if error in opening a file , 0 if data has been sucessfully written from array to the specified file
*/
void merge (int arr[], int start, int mid, int end);
/*
Name : mergesort
Description : Function to print the sorted array using merge sort algorithm
INput: Array, start and end of the lsit
Return: void
*/
void mergesort(int arr[], int start, int end);
/*
Name : swap
Description : Function to swap two numbers passed as arguments
INput: Pointer to two numbers
Return: void
*/
void swap(int* a, int* b);
/*
Name : partition
Description : Function to partition the given set of data passed as arguments
INput: Array, high and low values
Return: index to an array
*/
int partition (int arr[], int low, int high);
/*
Name : quickSort
Description : Function to do a recursive quick sort
INput: Array, High and low values
Return: void
*/
void quickSort(int arr[], int low, int high);

#endif
