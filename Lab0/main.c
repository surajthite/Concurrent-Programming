#include "main.h"
#include "sorts.h"



int main(int argc, char **argv)
{
struct handler handler_t = {"Suraj Thite"};
if ((argc < 2) || (argc > 6))
{
    printf("Incorrect no of Arguments Passed \nUse format ./mysort [--name] [sourcefile.txt] [-o outputfile.txt] [--alg=<merge,quick>] \n");
    return -1;
}

if ((strcmp(argv[1], "--name")) == 0)
{
    printf("Name = %s", handler_t.name);
    return 1;
}
else if(validate_text_file(argv[1]) == false)
{
  printf("Enter valid file .txt file name \n");
  return -1;
}
else
{
    char *buff = NULL;
    size_t temp =0;
    int count =0;
    handler_t.input_file = argv[1];
    FILE *fptr;
    fptr = fopen(handler_t.input_file,"r");
    if(fptr == NULL)
    {
      printf("\nError in opening the input text file. Please check again !\n");
      return -1;
    }
    else
    {
      printf("Valid Text File Found!");
    }
    while(getline(&buff,&temp,fptr) != -1)
    {
      count ++;
    }
    handler_t.f_size = count;
    printf("\nhandler_t.f_size = %d",  handler_t.f_size);
}

if((argc >= 2) && (argc <=5))
{
  printf("\nScanning Output file is passed");
  if ((strcmp(argv[2], "-o") == 0) ||  (strcmp(argv[2], "-O") == 0))
    {
      if(validate_text_file(argv[3]) == true)
      {
        handler_t.output_file = argv[3];
        printf("\nOutput file Valid!");
      }
      else
      {
      printf("Enter valid file output text file \n");
      return -1;
      }
    }
  else
  {
    printf("Enter a vaid flag - '-o' to specify the output flag\n");
    return -1;
  }
}


int array_size = ((handler_t.f_size));
int list[array_size];

if(file_to_array(handler_t,list)!=0)
{
  printf("\nCannot write to file");
  exit -1;
}

printf("\n************** The Input array is *****************\n");
  for(int i=0;i<handler_t.f_size;i++)
  {
    printf("%d\n",list[i]);
  }

if(argc == 5)
{
  if(strcmp(argv[4],"--alg=merge")==0)
  {
    printf("\nExecuting Merge Sort");
    mergesort(list, 0, (array_size - 1));
  }
  else if(strcmp(argv[4],"--alg=quick")==0 )
  {
      printf("\nExecuting Quick Sort");
    quickSort(list, 0, (array_size - 1));
  }
  else
    {
      printf("\nEnter the last argument either --alg=merge or --alg=quick");
      return -1;
    }
}
else
{
    printf("\nEnter the last argument either --alg=merge or --alg=quick");
    return -1;
}
printf("\nWriting Data to the file");
printf("\n************** The Sorted array is ***************** \n");
  for(int i=0;i<handler_t.f_size;i++)
  {
    printf("%d\n",list[i]);
  }
array_to_file(handler_t, list);

return 0;
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
  file_pointer = fopen(handler_t.input_file,"r");
  if(file_pointer == NULL)
  {
    printf("Error in opening the specified input text file");
    return -1;
  }
  for (int i=0 ;i<handler_t.f_size;i++)
  {
    fscanf(file_pointer,"%d",&buffer[i]);
  }
  fclose(file_pointer);
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
  file_pointer = fopen(handler_t.output_file,"w+");
  if(file_pointer == NULL)
  {
    printf("Error in opening the specified output text file");
    return -1;
  }
  for (int i=0 ;i<handler_t.f_size;i++)
  {
    fprintf(file_pointer,"%d\n",buffer[i]);
  }
  fclose(file_pointer);
  return 0;
}

/*
Name : validate_text_file.
Description :Function to check whether the input file is a valid text file or not.
Input: Pinter to the string of file name.
Return: True if file is valid and false if invalid.
*/

bool validate_text_file (char *s)
{
  int len = strlen(s);
  char f_name[len];
  strcpy(f_name,s);
  char *handle = strtok(f_name,".txt");
  if((strcmp(handle,s)) == 0)
    return false;
  return true;
}
