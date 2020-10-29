
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
File_Name : main.cpp
Author: Suraj Thite
Description : This file contains the Application code for lab0 exercisedsfsd
*/

/* Custom Header Files*/

#include "main.h"
#include "sorts.h"
#include "custom_locks.h"
int verbose_flag;
using namespace std;

/* Function Declaration for Parellel Merge and Bucket Sort*/
int get_bucket_range(vector<int>& arr, int size, int thread);
void *bucketSort(void *arg);
void *fj_merge(void *arg);

pthread_mutex_t bucket_lock = PTHREAD_MUTEX_INITIALIZER; //Static Lock Initialization
pthread_barrier_t pthread_barrier_1;// Barrier for Synchronization
pthread_barrier_t pthread_barrier_2;// Barrier for Synchronization

vector <multiset <int32_t>> Bucket; //Global Bucket to store values
struct timespec start_time, end_time; //Struct to store the timer values

int get_bucket_range(int arr[], int size, int thread);
void *bucketSort(void *arg);
int TEST_NUM=0;
const int FUNC_CNT = 8;
void (*funcs_lock[FUNC_CNT])()  = {
    TAS_lock,
    TTAS_lock,
    Ticket_lock,
    Pthread_lock,
    TAS_unlock,
    TTAS_unlock,
    Ticket_unlock,
    Pthread_unlock
};

const char* func_names_lock[FUNC_CNT/2] = {
    "tas",
    "ttas",
    "ticket",
    "pthread"
};
void (*bar_func)() = NULL;  //Function pointer to store barrier Functions
const int NUM_BAR_FUNCS = 2;
void (*funcs_barrier[NUM_BAR_FUNCS])()  = {
    sense_bar,
    Pthread_barrier
};
const char* func_names_barrier[NUM_BAR_FUNCS] = {
    "sense",
    "pthread"
};
struct handler handler_t = {"Suraj Bajrang Thite"}; //Object for handler for parsing command line arguments
MCSLock my_mcs_lock; //Object for MCS Lock
atomic<Node*> tail{NULL};
int is_barrier_selected = 0; //Flag to deterime if barrier is to be implemented

int main (int argc, char **argv)
{
int c = 0;
char *buff = NULL;
size_t temp =0;
int count =0;
FILE *fptr = NULL; //Pointer to access the file
char bar[256];

if ((argc < 2) || (argc > 9) || (argc < 9 && argc >2)) // Check for number of arguments
{
    printf("mysort [--name] [source.txt] [-o out.txt] [-t NUM THREADS] [--alg=<fj,bucket>] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] \n");
    return -1; //Return if incorrect number of arguments passsed
}
if ((strcmp(argv[1], "--name")) != 0) {
  handler_t.input_file = argv[1]; //store the input file name passed as extra argument
   printf("The input filename is %s\n", handler_t.input_file);
}
  while (1)
    {
      static struct option long_options[] =
        {
          {"name", no_argument, &verbose_flag, 'n'}, //Long option ofr --name
          {"alg",  required_argument, 0, 'a'},  //long option for alg = merge/quick
          {"out",  required_argument,  0, 'o'}, //option for output file
          {"thread",    required_argument, 0, 't'}, //option for threads to be implemented
          {"bar",    required_argument, 0, 'b'},
          {"lock",    required_argument, 0, 'l'},
          {0, 0, 0,0}
        };
      int option_index = 0;
      c = getopt_long (argc, argv, "n:o:t:a:b:l:",long_options, 0);
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

        case 'a':
        if(strcmp(optarg,"fj")==0)
          handler_t.merge_sort = 1; //Set the merge sort flag
        if(strcmp(optarg,"bucket")==0)
          handler_t.bucket = 1; //Set the bucket flag
        break;

        case 'b':
        strcpy(bar, optarg);
        is_barrier_selected = 1;
        if (strcmp(bar, "sense") == 0)
            {
              handler_t.sense_barr = 1;
              bar_func = funcs_barrier[0];
            }
        else if (strcmp(bar, "pthread") == 0)
          {
            handler_t.sense_barr = 0;
            bar_func = funcs_barrier[1];
            pthread_barrier_init(&pthread_barrier_1, NULL, handler_t.thread_cnt); //Initialize the barrier
          }
        else
          {
            printf("\nEnter correct arguments for barriers i.e. <sense/pthread>");
            exit(0);
          }
        if(  handler_t.sense_barr == 1 )
        printf("\n Sense Barrier is used");
        else
        printf("\n Pthread Barrier is used\n");
        break;

        case 'o':
        handler_t.output_file = optarg; //store the output file name in the handler
        break;

        case 'l':
          handler_t.lock = optarg;
          for (int i = 0; (i < (FUNC_CNT/2)); i++)
        {
          if (strcmp(handler_t.lock, func_names_lock[i]) == 0)
          TEST_NUM = i;
        }
        break;

        case 't':
          //  printf("The number of threads passed is %d",atoi(optarg));
          handler_t.thread_cnt =  atoi(optarg); //store the thread count in the variable
          printf("The number of threads passed is %d",  handler_t.thread_cnt);
        break;

        case '?':
          break;

        default:
        printf("mysort [--name] [source.txt] [-o out.txt] [-t NUM THREADS] [--alg=<fj,bucket>] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] \n");
    }

}

  if (verbose_flag)
  {
    printf("%s",handler_t.name);  // print and exit if -- name is passed
    exit (0);
  }

  if (handler_t.thread_cnt <= 0)
  {
		handler_t.thread_cnt = 1; //Set thread count to one if invalid thread count is passed
	}

  fptr = fopen(handler_t.input_file,"r");  //Open the file in read mode
  if(fptr == NULL)
  {
    printf("\nError in opening the input text file. Please check again !\n"); //Throw an exception incase of failure
    return -1;
  }
  while(getline(&buff,&temp,fptr) != -1)
  {
    count ++; //Detect the num of elements in the list
  }
  handler_t.f_size = count;
  //printf("\nhandler_t.f_size = %d",  handler_t.f_size);
  int array_size = ((handler_t.f_size)); //Store the number of elements in varibale in the handler structure
  vector<int> list(array_size); //vector to store the numbers
  if(file_to_array(handler_t,list)!=0)  //Store data from file to array
  {
    printf("\nCannot write to file"); //Print in case of bad FD
    return -1;
  }

  int list_array[list.size()];
  std::copy(list.begin(), list.end(), list_array); //Convert the vector to array

  // printf("\n************** The Input array is *****************\n");
  //   for(int i=0;i<handler_t.f_size;i++)
  //   {
  //     printf("%d\n",list[i]); //Print the input read from the file
  //   }

    pthread_t threads[handler_t.thread_cnt];

    if(handler_t.merge_sort)
      {
        int i = 0, low = 0 ,len =0;
        pthread_barrier_init(&pthread_barrier_2, NULL, handler_t.thread_cnt); //Initialize the barrier
        printf("\n Executing Merge Sort !! ");

        struct merge_task *merge_task_args;  // pointer to structure arguments

        struct merge_task merge_args_list[handler_t.thread_cnt]; // list to store arguments to be passed to threads

        len = (array_size / handler_t.thread_cnt); // calculate the length of the elements to be passed into each thread

        for (i = 0; i < handler_t.thread_cnt; i++, low += len)
        {
          merge_task_args = &merge_args_list[i]; // point to arguments of ith thread

          merge_task_args->task_no = i; // Store thread id
          merge_task_args->task_low = low; // store the lower index to be sorted
          merge_task_args->task_high = low + len - 1; //store the higher index of the elements to be sorted
          merge_task_args->list = list_array; //pass the array to the thread

          if (i == (handler_t.thread_cnt - 1))
            merge_task_args->task_high = array_size - 1; // size = arraysize -1 for the last thread
        }

        for (i = 0; i < handler_t.thread_cnt; i++) {
          merge_task_args = &merge_args_list[i];
          if ((pthread_create(&threads[i], NULL, fj_merge, merge_task_args)) != 0) // Create the threads and pass the arguments
          {
            printf("Error:  Couldn't create threads\n");
            exit(1);
          } else
          {
            //printf("Thread %d started ....\n", (i+1));
          }
        }

        for (i = 0; i < handler_t.thread_cnt; i++)
         {
           pthread_join(threads[i], NULL); // join the threads to the main thread
           //printf("Thread %d Joined ...\n", (i+1));
        }

        struct merge_task *task_0 = &merge_args_list[0]; // select the thread 0 arguments

        for (i = 1; i < handler_t.thread_cnt; i++)
        {
          struct merge_task *tsk = &merge_args_list[i]; // merge the arguments in the list
          merge(tsk->list, task_0->task_low, tsk->task_low - 1, tsk->task_high);
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time); // store the end time
        pthread_barrier_destroy(&pthread_barrier_2); //destroy the barrier since its not is use in any more
        printf("\nWriting Data to the file"); //Write the data to the file
        printf("\n************** The Sorted array is ***************** \n");
          for(int i=0;i<handler_t.f_size;i++)
          {
            printf("%d\n",list_array[i]); //Print the sorted array !
          }
       array_to_file(handler_t, list_array); // Store the sorted data to a ouput file passed as argument
      }
    else if(handler_t.bucket)
    {
      printf("Executing Bucket Sort"); //Execute quick algorithm if --al=quick is passed as argument
      //quickSort(list, 0, (array_size - 1));
      int i = 0, k = 0, m = 0;
      struct bucket_task b_task[handler_t.thread_cnt];
      int divider = get_bucket_range(list, array_size, handler_t.thread_cnt); // get the divider
      Bucket.resize(handler_t.thread_cnt); // resize the bucket
      int len = (array_size / handler_t.thread_cnt); // determine the number of elements to be passed to each thread

      pthread_barrier_init(&pthread_barrier_2, NULL, handler_t.thread_cnt); //Initialize the barrier

      for (i = 0; i < handler_t.thread_cnt; i++)
      {
        m = i*len;
        if (i != (handler_t.thread_cnt - 1)) //set arguments for last thread
        {
          b_task[i].t_divider = divider;
          b_task[i].t_id = i;
          b_task[i].t_size = len;
          b_task[i].list = &list[m];
        }
        else  //Set arguments for the remaining threads
        {
          b_task[i].t_divider = divider;
          b_task[i].t_id = i;
          b_task[i].t_size = (array_size - m);
          b_task[i].list = &list[m];
        }

        if ((pthread_create(&threads[i], NULL, bucketSort, (void *)&b_task[i])) != 0) // Create threads and pass the arguments
        {
          printf("Error on creating the thread\n");
          exit(1);
        }
        else
        {
          //printf("Creating thread %d\n", (i+1));
        }
      }

      for (i = 0; i < handler_t.thread_cnt; i++)
      {
      //  printf("Joining thread %d\n", (i+1));
        pthread_join(threads[i], NULL); // Join the threads
      }
      int vec_len = (int)Bucket.size();
      for (i = 0; i < vec_len; i++) {
      for (std::multiset<int>::iterator j = Bucket[i].begin(); j != Bucket[i].end(); j++)
      {
        list[k] = *j;
        k++;
      }
      }
      clock_gettime(CLOCK_MONOTONIC,&end_time); // determine the end time
    if(handler_t.sense_barr ==0)
      pthread_barrier_destroy(&pthread_barrier_1); // Destroy the barrier if pthread barrier was selected
    //  printf("\nWriting Data to the file"); //Write the data to file
      // printf("\n************** The Sorted array is ***************** \n");
      //   for(int i=0;i<handler_t.f_size;i++)
      //   {
      //     printf("%d\n",list[i]); //Print the sorted array !
      //   }
    array_to_file(handler_t, list); // Store the sorted data to a ouput file passed as argument
    }

unsigned long long elapsed_ns;
elapsed_ns = (end_time.tv_sec-start_time.tv_sec)*1000000000 + (end_time.tv_nsec-start_time.tv_nsec); // calculate the time required for completion of the thread
double elapsed_s = ((double)elapsed_ns)/1000000000.0;
printf("Elapsed (s): (ns) %lf : %llu \n",elapsed_s,elapsed_ns); //Print the time required to sort the array by n thread
exit (0);
}

/*
Name : file_to_array
Description : Function to read the unsorted data from file to an array
INput: handler and array where the data is to be stored from the file and vector where the data to be stored
Return: -1 if error in opening a file , 0 if data has been sucessfully copied from file to an array
*/

int file_to_array(struct handler handler_t, vector<int>& buffer)
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
INput: handler and vector  where the data is to be written to the file
Return: -1 if error in opening a file , 0 if data has been sucessfully written from array to the specified file
*/

int array_to_file(struct handler handler_t, vector<int>& buffer)
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
  fclose(file_pointer); //Clovector<int> listse the FD
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
  fclose(file_pointer); //Clovector<int> listse the FD
  return 0;
}

/*
Name : void *fj_merge(void *arg)
Description : This function implements parellel merge sort to be executed in threads in execution using fork join
INput: merge_task arguments
Return: void
*/

void *fj_merge(void *arg)
{
  int low =0,high =0 , mid=0;
	struct merge_task *merge_args = (struct merge_task *)arg;

	pthread_barrier_wait(&pthread_barrier_2);

	if(merge_args->task_no == 0){
		clock_gettime(CLOCK_MONOTONIC,&start_time);
	}

	pthread_barrier_wait(&pthread_barrier_2);

	printf("Executing thread %d\n", (merge_args->task_no + 1));

  low = merge_args->task_low;
  high = merge_args->task_high;
  mid = low + (high - low) / 2;

  if (low < high)
  {
      mergesort(merge_args->list, low, mid);
      mergesort(merge_args->list, mid + 1, high);
      merge(merge_args->list, low, mid, high);
    }
    return 0;
}

/*
Name : int get_bucket_range(vector<int>& arr, int size, int thread)
Description : This fucntion detemines the divider depending upon the maximum value in the array
Input: Input array to be sorted, size and number of thread
Return:  divider
*/

int get_bucket_range(vector<int>& arr, int size, int thread)
{
    int max = 0, bucket = 0, divider = 0;

	   bucket = thread;
     int temp[arr.size()];
	   std::copy(arr.begin(), arr.end(), temp);
    //find max
    max = *max_element(temp,temp + size);
    divider = ceil(float(max + 1) / bucket);

    return divider;
}

/*
Name : void *bucketSort(void *arg)
Description : This function is executed in the thread using Bucket Sort.
Input: bucket_task structure argument.
Return: void
*/

void *bucketSort(void *arg)
{
  struct bucket_task *b_task = (struct bucket_task *) arg;
  void (*lock)() = funcs_lock[TEST_NUM];
  void (*unlock)() = funcs_lock[TEST_NUM + 4];
  if ((strcmp(func_names_lock[TEST_NUM], "pthread") == 0))
    if (pthread_mutex_init(&bucket_lock, NULL) != 0)
      printf("Mutex init failed\n");

	int64_t i = 0, j = 0;
  if(b_task->t_id == 0)
    clock_gettime(CLOCK_MONOTONIC,&start_time);

if(is_barrier_selected)
  bar_func();
	for (i = 0; i < b_task->t_size; i++)
  {
		j=floor( b_task->list[i] / b_task->t_divider );
  if (strcmp(handler_t.lock, "mcs") == 0)
  {
    Node *mynode = new Node;
    my_mcs_lock.acquire(mynode);
    Bucket[j].insert((b_task->list)[i]);
    my_mcs_lock.release(mynode);
  }
else
{
  lock();
  Bucket[j].insert((b_task->list)[i]);
  unlock();
}
	}
  if(is_barrier_selected)
    bar_func();
	return 0;
}
