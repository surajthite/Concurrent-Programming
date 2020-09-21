
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

int verbose_flag;
using namespace std;


int get_bucket_range(vector<int>& arr, int size, int thread);
void *bucketSort(void *arg);

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t bar;
vector <multiset <int32_t>> Bucket;
struct timespec start_time, end_time;

int main (int argc, char **argv)
{
int c = 0;
char *buff = NULL;
size_t temp =0;
int count =0;
FILE *fptr = NULL;

struct handler handler_t = {"Suraj Bajrang Thite"};
if ((argc < 2) || (argc > 7) || (argc < 7 && argc >2)) // Check for number of arguments
{
    printf("Incorrect no of Arguments Passed \nUse format ./mysort [--name] [sourcefile.txt] [-o outputfile.txt] [--alg=<fjmerge,lkbucket>] \n");
    return -1; //Return if incorrect number of arguments passsed
}
  while (1)
    {
      static struct option long_options[] =
        {
          {"name", no_argument, &verbose_flag, 'n'}, //Long option ofr --name
          {"alg",  required_argument, 0, 'a'},  //long option for alg = merge/quick
          {"out",  required_argument,  0, 'o'}, //option for output file
          {"thread",    required_argument, 0, 't'}, //option for input file
          {0, 0, 0,0}
        };
      int option_index = 0;
      c = getopt_long (argc, argv, ":o:t:",long_options, &option_index);
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
        if(strcmp(optarg,"fjmerge")==0)
          handler_t.merge_sort = 1; //Set the merge sort flag
        if(strcmp(optarg,"lkbucket")==0)
          handler_t.bucket = 1; //Set the quick sort flag
        break;

       case 'o':
          handler_t.output_file = optarg; //store the output file name in the handler
          break;

        case 'i':
          handler_t.input_file =optarg; //store the input file name in the handler
          break;
        case 't':
          //  printf("The number of threads passed is %d",atoi(optarg));
          handler_t.thread_cnt =  atoi(optarg);
          printf("The number of threads passed is %d",  handler_t.thread_cnt);

        case '?':
          // printf("Error!"); //Error options
          break;

        default:
             printf("Incorrect no of Arguments Passed \nUse format ./mysort [--name] [sourcefile.txt] [-o outputfile.txt] [--alg=<fjmerge, lkbucket>] \n"); //Print error message

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

  if (handler_t.thread_cnt <= 0) {
		handler_t.thread_cnt = 1;
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
  //printf("\nhandler_t.f_size = %d",  handler_t.f_size);
  int array_size = ((handler_t.f_size)); //Store the number of elements in varibale in the handler structure
  vector<int> list(array_size);

  pthread_t threads[handler_t.thread_cnt];
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

    if(handler_t.merge_sort)
      {
        printf("Executing Merge Sort");
        mergesort(list, 0, (array_size - 1));  //Execute merge algorithm if --al=merge is passed as argument
      }
    else if(handler_t.bucket)
    {
      printf("Executing Bucket Sort"); //Execute quick algorithm if --al=quick is passed as argument
      //quickSort(list, 0, (array_size - 1));
      int i = 0, k = 0;
      struct bucket_task btsk[handler_t.thread_cnt];
      int divider = get_bucket_range(list, array_size, handler_t.thread_cnt);
      Bucket.resize(handler_t.thread_cnt);
      int len = (array_size / handler_t.thread_cnt);
      int m = 0;

      pthread_barrier_init(&bar, NULL, handler_t.thread_cnt);

      for (i = 0; i < handler_t.thread_cnt; i++)
      {
        m = i*len;
        if (i == (handler_t.thread_cnt - 1))
        {
          btsk[i].t_divider = divider;
          btsk[i].t_id = i;
          btsk[i].t_size = (array_size - m);
          btsk[i].list = &list[m];
        }
        else
        {
          btsk[i].t_divider = divider;
          btsk[i].t_id = i;
          btsk[i].t_size = len;
          btsk[i].list = &list[m];
        }

        if ((pthread_create(&threads[i], NULL, bucketSort, (void *)&btsk[i])) != 0)
        {
          printf("Error on creating the thread\n");
          exit(1);
        }
        else
        {
          printf("Creating thread %d\n", (i+1));
        }
      }

      for (i = 0; i < handler_t.thread_cnt; i++)
      {
        printf("Joining thread %d\n", (i+1));
        pthread_join(threads[i], NULL);
      }
      int vec_len = (int)Bucket.size();
      for (i = 0; i < vec_len; i++) {
      for (std::multiset<int>::iterator j = Bucket[i].begin(); j != Bucket[i].end(); ++j)
      {
        list[k] = *j;
        k++;
      }
      }
      clock_gettime(CLOCK_MONOTONIC,&end_time);
      pthread_barrier_destroy(&bar);
    }
    printf("\nWriting Data to the file");
    printf("\n************** The Sorted array is ***************** \n");
      for(int i=0;i<handler_t.f_size;i++)
      {
        printf("%d\n",list[i]); //Print the sorted array !
      }
    array_to_file(handler_t, list); // Store the sorted data to a ouput file passed as argument
unsigned long long elapsed_ns;
elapsed_ns = (end_time.tv_sec-start_time.tv_sec)*1000000000 + (end_time.tv_nsec-start_time.tv_nsec);
double elapsed_s = ((double)elapsed_ns)/1000000000.0;
printf("Elapsed (s): (ns) %lf : %llu \n",elapsed_s,elapsed_ns);
  exit (0);
}

/*
Name : file_to_array
Description : Function to read the unsorted data from file to an array
INput: handler and array where the data is to be stored from the file
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
INput: handler and array where the data is to be written to the file
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
Name : array_to_file
Description : Function to write the sorted data to the file*
INput: handler and array where the data is to be written to the file
Return: -1 if error in opening a file , 0 if data has been sucessfully written from array to the specified file
*/

void *bucketSort(void *arg)
{
  struct bucket_task *b_task = (struct bucket_task *) arg;
	int64_t i = 0, j = 0;
	//pthread_barrier_wait(&bar);
  if(b_task->t_id == 0)
  {
      clock_gettime(CLOCK_MONOTONIC,&start_time);
  }
  //pthread_barrier_wait(&bar);
	printf("Executing thread %d\n",(b_task->t_id + 1));
	for (i = 0; i < b_task->t_size; i++)
  {
		j=floor( b_task->list[i] / b_task->t_divider );
		pthread_mutex_lock(&lock1);
		Bucket[j].insert((b_task->list)[i]);
		pthread_mutex_unlock(&lock1);
	}
	//pthread_barrier_wait(&bar);
	return 0;
}
