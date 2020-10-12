#ifndef __MAIN_H__
#define __MAIN_H__

/* System Header Files*/
#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include <getopt.h>
#include <stdint.h>
#include <pthread.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>
#include <iterator>


/* Structure to store the input arguments from the terminal */

struct handler
{
  char name[20];  // Array to store the Authors name [Suraj Bajrang Thite].
  char *input_file; //Pointer for input file name.
  int f_size; // Size of input file.
  char *output_file; // Pointer to output file name.
  int merge_sort; //Flag to determine whether the sort is merge sort.
  int bucket; //Flag to determine whehter bucket sort is to be implemented.
  int thread_cnt; //Variable  to store the thread count.
//  bool algo; //0  for Merge Sort and 1 for Buck Sort Implementation.
  char * lock; // String for storing type of lock.
  bool sense_barr; //Varibale for sense barrier functionality.
};

/* Structure to store the data set for each task */
struct bucket_task
 {
  int t_id; //varibale to store thread id associated with the bucket
  int t_divider; //Varaible to store the divider task
  int t_size; //varibale to store the size of bucket indicated by the thread
  int *list; //pointer to the starting element of list
};

/*structure to store the data set for merge thread*/
struct merge_task {
	int task_no; //varibale to store thread id associated with the merge thread
	int task_low; //Varibale to store  the lowest index of the data set to be sorted by the thread
	int task_high;//Varibale to store the highest index of the data set to be sorted by the thread
	int *list; //pointer to the starting element of list
};

/*
Name : file_to_array
Description : Function to read the unsorted data from file to an array
INput: handler and array where the data is to be stored from the file
Return: -1 if error in opening a file , 0 if data has been sucessfully copied from file to an array
*/

int file_to_array(struct handler handler_t, std::vector<int>& buffer);

/*
Name : array_to_file
Description : Function to write the sorted data to the file* using input vector
INput: handler and array where the data is to be written to the file
Return: -1 if error in opening a file , 0 if data has been sucessfully written from array to the specified file
*/

int array_to_file(struct handler handler_t,  std::vector<int>& buffer);

/*
Name : array_to_file
Description : Over Riding Function to write the sorted data to the file* using input array
INput: handler and array where the data is to be written to the file
Return: -1 if error in opening a file , 0 if data has been sucessfully written from array to the specified file
*/


int array_to_file(struct handler handler_t,  int buffer[]);


#endif
