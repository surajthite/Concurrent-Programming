
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
File_Name : main.c
Author: Suraj Thite
Description : This file contains the Application code for lab0 exercise
*/

/* Custom Header Files*/


#include "main.h"
#include "sorts.h"
#include <omp.h>
int verbose_flag;

struct merge_sort_task {
    int task_no;
    int task_low;
    int task_high;
};

void threadedMerge(struct merge_sort_task *arg);
#define MAX_NUM_THREADS 10
static int num_threads = 1; // Total number of threads, default to 1 for main thread
static int count = 0; // Variable to store number of elements
int *list = NULL; // Array to store sorted elements
int main (int argc, char **argv)
{
int c = 0;
char *buff = NULL;
size_t temp =0;
//int count =0;
FILE *fptr = NULL;

struct handler handler_t = {"Suraj Bajrang Thite"};
if (!(argc == 2 || argc == 4 )) // Check for number of arguments
{
    printf("Incorrect no of Arguments Passed \nUse format ./mysort [--name] [sourcefile.txt] [-o outputfile.txt] [--alg=<merge,quick>] \n");
    return -1; //Return if incorrect number of arguments passsed
}
  while (1)
    {
      static struct option long_options[] =
        {
          {"name", no_argument, &verbose_flag, 'n'}, //Long option ofr --name
          {"out",  required_argument,  0, 'o'}, //option for output file
          {"in",    required_argument, 0, 'i'}, //option for input file
          {0, 0}
        };
      int option_index = 0;
      c = getopt_long (argc, argv, ":o:",long_options, &option_index);
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
          printf (" with arg %s", optarg);
          printf ("\n");
          break;

       case 'o':
          handler_t.output_file = optarg; //store the output file name in the handler
          break;

        case 'i':
          handler_t.input_file =optarg; //store the input file name in the handler
          break;

        case '?':
          printf("Error!"); //Error options
          break;

        default:
             printf("Incorrect no of Arguments Passed \nUse format ./mysort [--name] [sourcefile.txt] [-o outputfile.txt] [--alg=<merge,quick>] \n"); //Print error message

        }
    }
  for(; optind < argc; optind++){
      handler_t.input_file = argv[optind]; //store the input file name passed as extra argument
  }

  if (verbose_flag)
  {
    printf("%s",handler_t.name);  // print and exit if -- name is passed
    exit (0);
  }
if( handler_t.output_file == NULL )
{
  printf("\nPlease Parse the Output File !! ");
  exit(0);
}
  fptr = fopen(handler_t.input_file,"r");
  if(fptr == NULL)
  {
    printf("\nError in opening the input text file. Please check again !\n");
    return -1;
  }
  while(getline(&buff,&temp,fptr) != -1)
  {
    count ++; //Detect the num of elements in the list
  }
  handler_t.f_size = count;
  //int array_size = ((handler_t.f_size)); //Store the number of elements in varibale in the handler structure
  list = (int *)malloc( handler_t.f_size * sizeof(int));
  //int list[array_size]; //Array to store the data from input file
  if(file_to_array(handler_t,list)!=0)  //Store data from file to array
  {
    printf("\nCannot write to file"); //Print in case of bad FD
    return -1;
  }
  printf("\n************** The Input array is *****************\n");
    for(int i=0;i<handler_t.f_size;i++)
    {
      printf("%d\n",list[i]); //Print the input read from the file
    }

//    int cnt = (array_size - (array_size / 2) + 1);
    struct merge_sort_task *task;

    int len = 0;
    int low = 0;

    struct merge_sort_task tasklist[MAX_NUM_THREADS];

   omp_set_num_threads(MAX_NUM_THREADS);

    #pragma omp parallel
    {
    //  printf("Executing Parallel Section1 ..\n");
        num_threads = omp_get_num_threads();

        #pragma omp master
        {
            if( num_threads != 0 ) {
              // printf("num_elements : %d",num_elements);
              //printf("num_threads : %d",num_threads);
                len = handler_t.f_size / num_threads;
                // printf("Length : %d",len);
            }
        }

        #pragma omp barrier

  //printf("Executing Parallel Section2 ..\n");
        /* splitting input array to smaller arrays */
        #pragma omp master
        {
          for (int i = 0; i < num_threads; i++)
          {
              task = &tasklist[i];
              task->task_no = i;
              task->task_low = low;
              task->task_high = low + len - 1;
              if (i == (handler_t.f_size - 1) || i == num_threads - 1)
                  task->task_high = handler_t.f_size - 1;
              low += len;
            //  printf("Task number = %d\tLow = %d\tHigh = %d\n", task->task_no, task->task_low, task->task_high);

          }
        }

        #pragma omp barrier
        //printf("Executing Parallel Section3 ..\n");

        #pragma omp for
        for (int i = 0; i < num_threads; i++)
        {
            //printf("Num threads = %d\n", num_threads);
            task = &tasklist[i];
            threadedMerge(task);
        }

        #pragma omp barrier


        #pragma omp master
        {
          for (int i = 1; i < num_threads; i++)
          {
            struct merge_sort_task *task1 = &tasklist[i];
            //printf("Task number = %d\tLow = %d\tHigh = %d\n", task1->task_no, task1->task_low, task1->task_high);
            merge(list, 0, task1->task_low - 1, task1->task_high);
          }
        }

    }
    printf("\nWriting Data to the file");
    printf("\n************** The Sorted array is ***************** \n");
      for(int i=0;i<handler_t.f_size;i++)
      {
        printf("%d\n",list[i]); //Print the sorted array !
      }
    array_to_file(handler_t, list); // Store the sorted data to a ouput file passed as argument
    free(list);
  exit (0);
}

/*
Name : file_to_array
Description : Function to read the unsorted data from file to an array
INput: handler and array where the data is to be stored from the file
Return: -1 if error in opening a file , 0 if data has been sucessfully copied from file to an array
*/
int file_to_array(struct handler handler_t, int buffer[])
{
  FILE *file_pointer;
  file_pointer = fopen(handler_t.input_file,"r"); //Open the file in read mode
  if(file_pointer == NULL)
  {
    printf("Error in opening the specified input text file"); //Print in case of error
    return -1;
  }
  for (int i=0 ;i<handler_t.f_size;i++)
  {
    fscanf(file_pointer,"%d",&buffer[i]); //Stor the data in buffer line by line
  }
  fclose(file_pointer);   //Close the file FD
  return 0;
}

/*
Name : array_to_file
Description : Function to write the sorted data to the file*
INput: handler and array where the data is to be written to the file
Return: -1 if error in opening a file , 0 if data has been sucessfully written from array to the specified file
*/

int array_to_file(struct handler handler_t, int buffer[])
{
  FILE *file_pointer;
  file_pointer = fopen(handler_t.output_file,"w+"); //Open the output file in write mode
  if(file_pointer == NULL)
  {
    printf("Error in opening the specified output text file"); //Error in case of bad FD
    return -1;
  }
  for (int i=0 ;i<handler_t.f_size;i++)
  {
    fprintf(file_pointer,"%d\n",buffer[i]); //Write data to the file
  }
  fclose(file_pointer); //Close the FD
  return 0;
}


void threadedMerge(struct merge_sort_task *arg)
{
  struct merge_sort_task *task = arg;
  int low =0,high =0;
  low = task->task_low;
  high = task->task_high;
  //int mid = low + (high - low) / 2;
// printf("Task number = %d\n", task->task_no);
  if (low < high)
  {
      mergesort(list, low, high);
    }
  //  return 0;
}
