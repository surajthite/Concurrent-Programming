/***************************************************************************************************
MIT License

Copyright (c) 2021 Suraj Thite

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

/**
 * @\file	arg_parser.c
 * @\author	Suraj Thite
 * @\brief	This file contains the application code for parallel tree
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <getopt.h>


#include "arg_parser.h"


/**
 * @\Function : int get_total_count(char *file_name)
 * @\brief : This function determines the total count of numbers from the input file
 * @\input : Pointer to the name of file
 * @\return : count
 */

int get_total_count(char *file_name)
{
    char *buffer = NULL;
    size_t temp = 0;
    int count = 0;
    FILE *fptr;
    fptr = fopen(file_name, "r");

    if (fptr == NULL)
    {
        printf("Error in opening the input file\n");
        return -1;
    }

    while (getline(&buffer, &temp, fptr) != -1)
    {
      count++;
    }

    fclose(fptr);
    free(buffer);
    return count;
}

/**
 * @\Function :int parse_args(int argc, char **argv, struct arg_handler *arg_handler_t)
 * @\brief : This function parses the command line arguments from the command line and returns 1 on success
 * @\input :
 * @\return : void
 */
int parse_args(int argc, char **argv, struct arg_handler *arg_handler_t)
{
  char c;
	char lock[256];

    static struct option opt[] =
    {
        {"input", required_argument, 0, 'i'},
        {"thread", optional_argument, 0, 't'},
        {"search", required_argument, 0, 's'},
        {"range", required_argument, 0, 'r'},
        {"lock", required_argument, 0, 'l'},
        {"help", optional_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    arg_handler_t->is_rw_lock = false;

    while ((c = getopt_long(argc, argv, "hi:s:t:r:", opt, 0)) != -1)
    {
        switch (c)
        {
            case 'h':
                printf("Usage: ./tree -i [insert file_name] -s [search file_name] -r [range querry file_name] -t [Number of threads] --lock=[lock name]\n");
                exit(0);
                break;

            case 'i':
				arg_handler_t->ifile = optarg;
                break;

            case 't':
				arg_handler_t->thread = atoi(optarg);
                break;

            case 'l':
				strcpy(lock, optarg);
				if (strcmp(lock, "rw_lock") == 0)
					arg_handler_t->is_rw_lock = 1;
				else
					arg_handler_t->is_rw_lock = 0;
                break;

            case 's':
                arg_handler_t->sfile = optarg;
                break;

            case 'r':
                arg_handler_t->rfile = optarg;
                break;

            default:
                printf("No args provided\n");
                return -1;
        }

    }

	  arg_handler_t->total_insert_keys = get_total_count(arg_handler_t->ifile);
    arg_handler_t->range_querries = get_total_count(arg_handler_t->rfile);
    arg_handler_t->total_search_keys = get_total_count(arg_handler_t->sfile);

    return 0;
}
