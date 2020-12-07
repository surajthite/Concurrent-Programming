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
 * @\file	arg_parser.h
 * @\author	Suraj Thite
 * @\brief	This file contains the application code for parallel tree
 */
#ifndef __HELPER_H__
#define __HELPER_H__


struct arg_handler
{

    char name[16]; // Name of the student
    char *ifile; // Insert file name
    char *sfile; // Search File name
    char *rfile; // Read File name
    char *lock;
    int total_insert_keys; // Total number of insert keys
    int total_search_keys; // Total number of search keys
    int range_querries; // Total number of range queries
	   int thread;// Variable to store the thread count
    bool is_rw_lock; // Flag to determine the lock
};

/**
 * @\Function :int parse_args(int argc, char **argv, struct arg_handler *arg_handler_t)
 * @\brief : This function parses the command line arguments from the command line and returns 1 on success
 * @\input :
 * @\return : void
 */

int parse_args(int argc, char **argv, struct arg_handler *arg_handler_t);

/**
 * @\Function : int get_total_count(char *file_name)
 * @\brief : This function determines the total count of numbers from the input file
 * @\input : Pointer to the name of file
 * @\return : count
 */

int get_total_count(char *file_name);

#endif
