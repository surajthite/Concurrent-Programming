
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
int verbose_flag;
int main (int argc, char **argv)
{
int c;

struct handler handler_t = {"Suraj Thite"};
if ((argc < 2) || (argc > 5) || (argc < 5 && argc >2)) // Check for number of arguments
{
    printf("Incorrect no of Arguments Passed \nUse format ./mysort [--name] [sourcefile.txt] [-o outputfile.txt] [--alg=<merge,quick>] \n");
    return -1; //Return if incorrect number of arguments passsed
}
  while (1)
    {
      static struct option long_options[] =
        {
          {"name", no_argument, &verbose_flag, 'n'}, //Long option ofr --name
          {"alg",  required_argument, 0, 'a'},  //long option for alg = merge/quick
          {"out",  required_argument,  0, 'o'}, //option for output file
          {"in",    required_argument, 0, 'i'}, //option for input file
          {0, 0, 0}
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

        case 'a':
        if(strcmp(optarg,"merge")==0)
          handler_t.merge_sort = 1; //Set the merge sort flag
        if(strcmp(optarg,"quick")==0)
          handler_t.quick_sort = 1; //Set the quick sort flag
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
  // printf("inputfile is:%s\n",handler_t.input_file);
  // printf("outputfile is:%s\n",handler_t.output_file);
  // printf("The Merge Flag is :%d\n",handler_t.merge_sort);
  // printf("The Quick Flag is :%d\n",handler_t.quick_sort);

  char *buff = NULL;
  size_t temp =0;
  int count =0;
  FILE *fptr;
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
  printf("\nhandler_t.f_size = %d",  handler_t.f_size);
  int array_size = ((handler_t.f_size)); //Store the number of elements in varibale in the handler structure
  int list[array_size]; //Array to store the data from input file
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
    else if(handler_t.quick_sort)
    {
      printf("Executing Quick Sort"); //Execute quick algorithm if --al=quick is passed as argument
      quickSort(list, 0, (array_size - 1));
    }
    printf("\nWriting Data to the file");
    printf("\n************** The Sorted array is ***************** \n");
      for(int i=0;i<handler_t.f_size;i++)
      {
        printf("%d\n",list[i]); //Print the sorted array !
      }
    array_to_file(handler_t, list); // Store the sorted data to a ouput file passed as argument
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
