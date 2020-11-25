/**
 * @\file   arg_parser.c
 * @\author SuraJ Thite
 * @\brief  This file contains the definition all the application specific arg_parser functions
 *
 */

/* System Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <getopt.h>

/* Own Headers */
#include "arg_parser.h"



int get_total_count(char *filename)
{
    char *buffer = NULL;
    size_t temp = 0;
    int count = 0;
    FILE *fptr;

    fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        printf("Error in opening the input file\n");
        return -1;
    }

    while (getline(&buffer, &temp, fptr) != -1)
      count++;

    fclose(fptr);
    free(buffer);
    return count;
}


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

    arg_handler_t->is_rw_lock_set = false;

    while ((c = getopt_long(argc, argv, "hi:s:t:r:", opt, 0)) != -1)
    {
        switch (c)
        {
            case 'h':
                printf("Usage: ./tree -i [insert filename] -s [search filename] -r [range querry filename] -t [Number of threads] --lock=[lock name]\n");
                exit(0);
                break;

            case 'i':
				arg_handler_t->ifile = optarg;
                //printf("The input insert filename is %s\n", arg_handler_t->ifile);
                break;

            case 't':
				arg_handler_t->thread = atoi(optarg);
				//printf("The number of thread is %d\n", arg_handler_t->thread);
                break;

            case 'l':
				strcpy(lock, optarg);
				if (strcmp(lock, "rw_lock") == 0) {
					arg_handler_t->is_rw_lock_set = 1;
				} else {
					arg_handler_t->is_rw_lock_set = 0;
				}
				//printf("Algorithm used is %d\n", arg_handler_t->is_rw_lock_set);
                break;

            case 's':
                arg_handler_t->sfile = optarg;
                //printf("The input search filename is %s\n", arg_handler_t->sfile);
                break;

            case 'r':
                arg_handler_t->rfile = optarg;
                //printf("The input range querry filename is %s\n", arg_handler_t->rfile);
                break;

            default:
                printf("No args provided\n");
                return -1;
        }

    }

	   arg_handler_t->total_insert_keys = get_total_count(arg_handler_t->ifile);
	//printf("Total number of insert elements = %d\n", arg_handler_t->total_insert_keys);

      arg_handler_t->range_querries = get_total_count(arg_handler_t->rfile);
    //printf("Total number of range querries = %d\n", arg_handler_t->range_querries);

      arg_handler_t->total_search_keys = get_total_count(arg_handler_t->sfile);
    //printf("Total number of search elements = %d\n", arg_handler_t->total_search_keys);

    return 0;
}
