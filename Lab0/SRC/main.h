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

/* Structure to store the input arguments from the terminal */

struct handler
{
  char name[19];  // Array to store the Authors name [Suraj Bajrang Thite]
  char *input_file; //Pointer for input file name
  int f_size; // Size of input file
  char *output_file; // Pointer to output file name.
  int merge_sort;
  int quick_sort;
};


/*
Name : file_to_array
Description : Function to read the unsorted data from file to an array
INput: handler and array where the data is to be stored from the file
Return: -1 if error in opening a file , 0 if data has been sucessfully copied from file to an array
*/

int file_to_array(struct handler handler_t, int buffer[]);

/*
Name : array_to_file
Description : Function to write the sorted data to the file*
INput: handler and array where the data is to be written to the file
Return: -1 if error in opening a file , 0 if data has been sucessfully written from array to the specified file
*/

int array_to_file(struct handler handler_t, int buffer[]);


#endif
