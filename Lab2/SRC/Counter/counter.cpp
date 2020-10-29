
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
File_Name : counter.cpp
Author: Suraj Thite
Description : This file contains the Application code for lab0 exercisedsfsd
*/

/* Custom Header Files*/

#include "counter.h"
#include "sorts.h"
#include "custom_locks.h"
int verbose_flag;
using namespace std;

/* Function Declaration for Parellel Merge and Bucket Sort*/
void *thread_main(void *args);
pthread_mutex_t bucket_lock = PTHREAD_MUTEX_INITIALIZER; //Static Lock Initialization
pthread_barrier_t pthread_barrier_1;// Barrier for Synchronization
extern bar_t sense_bar_var;
pthread_barrier_t pthread_barrier_2;// Barrier for Synchronization
struct timespec start_time, end_time; //Struct to store the timer values
int cntr = 0;
int get_bucket_range(int arr[], int size, int thread);
void *bucketSort(void *arg);
int TEST_NUM=0;
const int FUNC_CNT = 8;
const int NUM_BAR_FUNCS = 2;

void (*bar_func)() = NULL; //Function pointer for barriers
struct handler handler_t = {"Suraj Bajrang Thite"}; // Handler to  handle command line arguments
MCSLock my_mcs_lock; //Object for MCS Lock
atomic<Node*> tail{NULL};

int is_barrier_selected = 0; //Flag for barrier
int is_lock_selected = 0; //Flag for Lock
int mcs_lock_selected = 0; //Flag for MCS Lock

void (*funcs_lock[FUNC_CNT])()  =
{
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

// Function pointer to store the callback functions for all the barriers
void (*funcs_barrier[NUM_BAR_FUNCS])()  = {
    sense_bar,
    Pthread_barrier
};

const char* func_names_barrier[NUM_BAR_FUNCS] = {
    "sense",
    "pthread"
};


int main (int argc, char **argv)
{
int c = 0;
char bar[256];
FILE *out_fileptr;
if ((argc < 2) || (argc > 8) || (argc < 8 && argc >2)) // Check for number of arguments
{
    printf("counter [--name] [-t NUM THREADS] [-i=NUM ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] [-o out.txt] \n");
    return -1; //Return if incorrect number of arguments passsed
}
if ((strcmp(argv[1], "--name")) != 0)
{
  printf("\n\rExecuting the code !! ");
}
  while (1)
    {
      static struct option long_options[] =
        {
          {"name", no_argument, &verbose_flag, 'n'}, //Long option ofr --name
          {"out",  required_argument,  0, 'o'}, //option for output file
          {"thread",    required_argument, 0, 't'}, //option for threads to be implemented
          {"i", required_argument, NULL, 'i'},
          {"bar",    required_argument, 0, 'b'},
          {"lock",    required_argument, 0, 'l'},
          {0, 0, 0,0}
        };
      int option_index = 0;
      c = getopt_long (argc, argv, "n:t:i:o:b:l:",long_options, 0);
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

        case 'b':
        strcpy(bar, optarg);
        is_barrier_selected = 1;
        break;

        case 'o':
        handler_t.output_file = optarg; //store the output file name in the handler
        break;

        case 'l':
          handler_t.lock = optarg;
          is_lock_selected = 1;
        break;

        case 't':
          //  printf("The number of threads passed is %d",atoi(optarg));
          handler_t.thread_cnt =  atoi(optarg); //store the thread count in the variable
          printf("The number of threads passed is %d",  handler_t.thread_cnt);
        break;

        case 'i':
        //NUM_ITERATIONS = atoi(optarg);
        handler_t.iterations =  atoi(optarg);
        printf("\n\rThe number of iterations ::  %d\n",handler_t.iterations);
        break;

        case '?':
          break;

        default:
        printf("counter [--name] [-t NUM THREADS] [-i=NUM ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] [-o out.txt]\n");
    }

}
  if (verbose_flag)
  {
    printf("%s",handler_t.name);  // print and exit if -- name is passed
    exit (0);
  }
//printf("The selected barrier is %s",bar);
  if (handler_t.thread_cnt <= 0)
  {
		handler_t.thread_cnt = 1; //Set thread count to one if invalid thread count is passed
	}

  if(is_lock_selected == is_barrier_selected) //CHeck whether lock and barrier both are selected
  {
      printf("Lock and barrier selected!\n");
      exit(0);
  }
  else if(is_lock_selected) //Swet the function pointer to corresponding lock
  {
    for (int i = 0; (i < (FUNC_CNT/2)); i++)
  {
    if (strcmp(handler_t.lock, func_names_lock[i]) == 0)
    TEST_NUM = i;
  }
    printf("The lock used is %s\n", handler_t.lock);
    printf("The lock to be executed is %d\n",TEST_NUM);
}else if(is_barrier_selected)   //Set the barrier function
{
  if (strcmp(bar, "sense") == 0)
  {
        handler_t.sense_barr = 1;
        printf("\nFlag for Sense Barrier used is %d\n",handler_t.sense_barr);
        bar_func = funcs_barrier[0];
        //sense_bar_var.cnt = 0;
  }
  else if (strcmp(bar, "pthread") == 0)
  {
      handler_t.sense_barr = 0;
      printf("\nFlag for Sense Barrier used is %d\n",handler_t.sense_barr);
      pthread_barrier_init(&pthread_barrier_1, NULL, handler_t.thread_cnt); //Initialize the barrier
      bar_func = funcs_barrier[1];
  }
    sense_bar_var.cnt = 0;
}

    pthread_t threads[handler_t.thread_cnt];
    int thread_id[handler_t.thread_cnt];
    pthread_barrier_init(&pthread_barrier_2, NULL, handler_t.thread_cnt); //INit the barrier
    for (int i = 0; i < handler_t.thread_cnt; i++)
        {
            thread_id[i] = i;
            pthread_create(&threads[i], NULL, thread_main, (void *)&thread_id[i]);  //Create the thread
        }
    for (int i = 0; i < handler_t.thread_cnt; i++)
      {
            pthread_join(threads[i], NULL); //Joing the threads
      }
    out_fileptr = fopen(handler_t.output_file, "w"); //Open the File
if( !out_fileptr ) {
    printf("Output file open failed\n");
    return -1;
}
fseek(out_fileptr, 0, SEEK_SET);
fprintf(out_fileptr, "%d\n", cntr); //Write the counter to the file

unsigned long long elapsed_ns;
elapsed_ns = (end_time.tv_sec-start_time.tv_sec)*1000000000 + (end_time.tv_nsec-start_time.tv_nsec);
printf("Elapsed (ns): %llu\n",elapsed_ns); //Print the elapsed time

 pthread_barrier_destroy(&pthread_barrier_2); //Destroy the barrier
//
 if(handler_t.sense_barr == 0 && is_barrier_selected) {
  printf("Destroying Barrier!!");
   pthread_barrier_destroy(&pthread_barrier_1);
  }
else if(is_lock_selected && strcmp(handler_t.lock, "pthread") == 0) {
   pthread_mutex_destroy(&bucket_lock);
}

return 0;
}

void *thread_main(void *args)
 {
    int thread_id = *(int *)args;
    // function pointer to store lock function
    void (*lock)() = funcs_lock[TEST_NUM];
    // function pointer to store unlock function
    void (*unlock)() = funcs_lock[TEST_NUM + 4];
    pthread_barrier_wait(&pthread_barrier_2);
    // Init mutex lock if selected
    if( thread_id == 0 )
    {
      if(strcmp(func_names_lock[TEST_NUM], "pthread"))
        {
          if(pthread_mutex_init(&bucket_lock, NULL) != 0)
            {
              printf("\n mutex init has failed\n");
            }
        }
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    }

    pthread_barrier_wait(&pthread_barrier_2);

    Node *mynode = new Node;

    for(int i = 0; i <handler_t.iterations * handler_t.thread_cnt; i++)
    {
        if(i%handler_t.thread_cnt==thread_id){
            // code block for counter increment if mcs lock is selected
            if (mcs_lock_selected) {
                my_mcs_lock.acquire(mynode);
                cntr++;
                my_mcs_lock.release(mynode);
            }
            // code block for counter increment if any other lock is selected
            else if(is_lock_selected)
            {
                lock();
                cntr++;
                unlock();
            }
            else
            {
              cntr++;
            }

    }
    if(is_barrier_selected)
        bar_func();

    }
    delete mynode;

    pthread_barrier_wait(&pthread_barrier_2);

    if( thread_id  == 0 )
    {
        clock_gettime(CLOCK_MONOTONIC, &end_time);
    }
    return 0;
  }
