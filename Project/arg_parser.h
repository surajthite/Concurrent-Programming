/**
 * @\file   arg_parser.h
 * @\author Suraj Thite
 * @\brief  This file contains the declaration all the application specific arg_parser functions
 *
 */

#ifndef __HELPER_H__
#define __HELPER_H__


struct arg_handler
{

    char name[16];      //Authors name

    char *ifile;        //Input file name

    char *sfile;        //Search key Input file name

    char *rfile;         //Range Querry Input file

    char *lock;			//lock name

    int total_insert_keys;

    int total_search_keys;

    int range_querries;

	   int thread;			//Number of threads

    bool is_rw_lock_set;
};


int parse_args(int argc, char **argv, struct arg_handler *arg_handler_t);
int get_total_count(char *filename);

#endif
