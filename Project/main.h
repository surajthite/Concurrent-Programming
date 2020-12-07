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
 * @\file	main.h
 * @\author	Suraj Thite
 * @\brief	This file contains the application code for parallel tree
 */

#include "mutex_lock_bst.h"
#include "rw_lock_bst.h"
#include "arg_parser.h"

struct thread_info
{
	int task_no;
	int task_len;
	int task_key;
	char file_name[64];
};

/**
 * @\Function : void *add_handler(void *arg)
 * @\brief : Thread function to add an element to the BST from the file
 * @\input : void pointer
 * @\return : void
 */
void *add_handler(void *arg);


/**
 * @\Function : void *get_handler(void *arg)
 * @\brief : This function retrieves the data from the BST
 * @\input : void pointer
 * @\return : void
 */
void *get_handler(void *arg);

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function parellellize the range function
 * @\input : void pointer
 * @\return : void
 */
void *range_handler(void *arg);


/**
 * @\Function : void *put_handler_rw(void *arg)
 * @\brief : This function wrties to the BST implementing reader writer locks
 * @\input : void pointer
 * @\return : void
 */
void *put_handler_rw(void *arg);

/**
 * @\Function : void *get_handler_rw(void *arg)
 * @\brief : This handler retrives the data from the BST Node
 * @\input : void  pointer
 * @\return : void
 */
void *get_handler_rw(void *arg);

/**
 * @\Function : void *range_handler_rw(void *arg)
 * @\brief :This is thread handler for range-query operation with read-write lock
 * @\input: void pointer
 * @\return : void
 */

void *range_handler_rw(void *arg);
