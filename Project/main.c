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
 * @\file	main.c
 * @\author	Suraj Thite
 * @\brief	This file contains the application code for parallel tree
 */

#include "main.h"

#define MAX_THREAD 15U

vector <range> querry[2];

vector <rangerw> querry_rw[2];

bst_node *global_root;
bst_noderw *global_rw_root;
pthread_mutex_t bst_lock;
pthread_rwlock_t bst_rwlock;

struct timespec start, end1;


/**
 * @\Function : void *add_handler(void *arg)
 * @\brief : Thread function to add an element to the BST from the file
 * @\input : void pointer
 * @\return : void
 */
void *add_handler(void *arg)
{
	int key = 0, val = 0, i = 0;
	struct thread_info *th_info = (struct thread_info *)arg;
	FILE *file_ptr = fopen(th_info->file_name, "r");

	if (!file_ptr)
	{
		printf("Error in opening input file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (file_ptr)))
	{
		fscanf(file_ptr, "%d %d\n", &key, &val);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(file_ptr, "%d %d\n", &key, &val);
		put_node(NULL, key, val, th_info->task_no);
	}

	fclose(file_ptr);
}

/**
 * @\Function : void *get_handler(void *arg)
 * @\brief : This function retrieves the data from the BST
 * @\input : void pointer
 * @\return : void
 */
void *get_handler(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
	bst_node *node;
	int key = 0, i = 0;

	FILE *file_ptr = fopen(th_info->file_name, "r");

	if (!file_ptr)
	{
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (file_ptr)))
	{
		fscanf(file_ptr, "%dn", &key);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(file_ptr, "%d\n", &key);
		node = get_node(NULL, key);

		if (!node)
			printf("Node with key %d not found\n", key);
		 else
			printf("Node with key %d contains value %d\n", node->key, node->value);

		node = NULL;
	}
		fclose(file_ptr);
}

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function parellellize the range function
 * @\input : void pointer
 * @\return : void
 */
void *range_handler(void *arg)
{
		struct thread_info *th_info = (struct thread_info *)arg;
    int start = 0, end = 0, i = 0;

    FILE *file_ptr = fopen(th_info->file_name, "r");

    if (!file_ptr)
	{
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (file_ptr)))
	{
		fscanf(file_ptr, "%d %d\n", &start, &end);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(file_ptr, "%d %d\n", &start, &end);
		if (end > start)
		{
			get_nodes_inrange(NULL, start, end, th_info->task_no);
		}
	}
fclose(file_ptr);
}


/**
 * @\Function : void *put_handler_rw(void *arg)
 * @\brief : This function wrties to the BST implementing reader writer locks
 * @\input : void pointer
 * @\return : void
 */
void *put_handler_rw(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
	int key = 0, val = 0, i = 0;

	FILE *file_ptr = fopen(th_info->file_name, "r");

	if (!file_ptr)
	{
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (file_ptr)))
	{
		fscanf(file_ptr, "%d %d\n", &key, &val);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(file_ptr, "%d %d\n", &key, &val);
		put_node_rw(NULL, key, val, th_info->task_no);
	}

	fclose(file_ptr);
}

/**
 * @\Function : void *get_handler_rw(void *arg)
 * @\brief : This handler retrives the data from the BST Node
 * @\input : void  pointer
 * @\return : void
 */
void *get_handler_rw(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
	bst_noderw *node;
	int key = 0, i = 0;

	FILE *file_ptr = fopen(th_info->file_name, "r");

	if (!file_ptr)
	{
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (file_ptr)))
	{
		fscanf(file_ptr, "%dn", &key);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(file_ptr, "%d\n", &key);
		node = get_node_rw(NULL, key);

		if (!node)
			printf("Node with key %d not found\n", key);
		else
			printf("Node with key %d contains value %d\n", node->key, node->value);

		node = NULL;
	}
    fclose(file_ptr);
}


/**
 * @\Function : void *range_handler_rw(void *arg)
 * @\brief :This is thread handler for range-query operation with read-write lock
 * @\input: void pointer
 * @\return : void
 */
void *range_handler_rw(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
    int start = 0, end = 0, i = 0;

    FILE *file_ptr = fopen(th_info->file_name, "r");

    if (!file_ptr)
	{
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (file_ptr)))
	{
		fscanf(file_ptr, "%d %d\n", &start, &end);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(file_ptr, "%d %d\n", &start, &end);
		if (end > start)
		{
			get_nodes_inrange_rw(NULL, start, end, th_info->task_no);
		}
	}

	fclose(file_ptr);
}

/**
 * @\Function : int main(int argc, char **argv)
 * @\brief :This isthe main fucntion where the execution begins
 * @\input: scommand line arguments
 * @\return : int
 */

int main(int argc, char **argv)
{
	struct arg_handler arg;
	parse_args(argc, argv, &arg);	//parse the command line argument

	if (arg.total_insert_keys < arg.thread)
		arg.thread = (arg.total_insert_keys / 2);

	if (arg.thread < 6)
		arg.thread = 6;

	struct thread_info th_info[arg.thread];
	pthread_t th[arg.thread];
	int i = 0;

	int length = (arg.total_insert_keys / (arg.thread - 4));
	int m = 0, j = 0;

	//Check for the lock i.e. Reader Writers Lock is selected or not.
	if (!arg.is_rw_lock )
	{
		printf("Mutex Selected !!\n");
		pthread_mutex_init(&bst_lock, NULL);
		clock_gettime(CLOCK_MONOTONIC,&start);

		for (i = 0; i < (arg.thread - 4); i++)
		{
			m = i*length;
			strcpy(th_info[i].file_name, arg.ifile);

			if (i == (arg.thread - 5))
			{
				th_info[i].task_no = i;
				th_info[i].task_len = (arg.total_insert_keys - m);
				th_info[i].task_key = m;
			}
			else
			{
				th_info[i].task_no = i;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}

			if (pthread_create(&th[i], NULL, add_handler, (void *)&th_info[i]) != 0)
			{
				printf("Error on creating the thread\n");
				exit(0);
			}
		}

		length = (arg.total_search_keys/ 2);
		m = 0;
		j = 0;

		for (; i < (arg.thread - 2); i++) {
			strcpy(th_info[i].file_name, arg.sfile);
			m = j*length;

			if (i == (arg.thread - 3)) {
				th_info[i].task_no = i;
				th_info[i].task_len = (arg.total_search_keys - m);
				th_info[i].task_key = m;
			} else {
				th_info[i].task_no = i;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}

			//invoke the get threads
			if (pthread_create(&th[i], NULL, get_handler, (void *)&th_info[i]) != 0)
			{
					printf("Error on creating the thread\n");
					exit(0);
			}
			j++;
		}

		length = (arg.range_querries/ 2);
		m = 0;
		j = 0;
		for (; i < (arg.thread); i++)
		{
			strcpy(th_info[i].file_name, arg.rfile);
			m = j*length;

			if (i == (arg.thread - 1))
			{
				th_info[i].task_no = j;
				th_info[i].task_len = (arg.range_querries - m);
				th_info[i].task_key = m;
			}
			else
			{
				th_info[i].task_no = j;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}

			//invoke the range querry threads
			if (pthread_create(&th[i], NULL, range_handler, (void *)&th_info[i]) != 0)
			{
					printf("Error on creating the thread\n");
					exit(0);
			}
			j++;
		}

		for (i = 0; i < arg.thread; i++)
		{
			pthread_join(th[i], NULL);
		}

		clock_gettime(CLOCK_MONOTONIC,&end1);
		pthread_mutex_destroy(&bst_lock);
		printf("\n\n************Result for Range Query************\n");

		for (i = 0; i < 2; i++)
		 {
			for (j = 0; j < querry[i].size(); j++)
			{
				printf("Range Querry by thread %d for %d to %d = %d\n",i, querry[i][j].start_key, querry[i][j].end_key, querry[i][j].node->key);
			}
		}

		printf("\nInorder Traversal of the Tree\n");
		print_tree_inorder(global_root);
		free_tree(global_root);

		unsigned long long elapsed_ns;
		elapsed_ns = (end1.tv_sec-start.tv_sec)*1000000000 + (end1.tv_nsec-start.tv_nsec);
		printf("Elapsed Time (ns): %llu\n",elapsed_ns);
		double elapsed_s = ((double)elapsed_ns)/1000000000.0;
		printf("Elapsed Time (s): %lf\n",elapsed_s);
	}

	else
	{
		pthread_rwlock_init(&bst_rwlock, NULL);
		clock_gettime(CLOCK_MONOTONIC,&start);
		for (i = 0; i < (arg.thread - 4); i++)
		{
			m = i*length;
			strcpy(th_info[i].file_name, arg.ifile);
			if (i == (arg.thread - 5))
			{
				th_info[i].task_no = i;
				th_info[i].task_len = (arg.total_insert_keys - m);
				th_info[i].task_key = m;
			} else
			{
				th_info[i].task_no = i;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}

			if (pthread_create(&th[i], NULL, put_handler_rw, (void *)&th_info[i]) != 0)
			{
				printf("Error on creating the thread\n");
				exit(0);
			}
		}

		length = (arg.total_search_keys/ 2);
		m = 0;
		j = 0;

		for (; i < (arg.thread - 2); i++)
		{
			strcpy(th_info[i].file_name, arg.sfile);
			m = j*length;

			if (i == (arg.thread - 3))
			{
				th_info[i].task_no = i;
				th_info[i].task_len = (arg.total_search_keys - m);
				th_info[i].task_key = m;
			} else
			{
				th_info[i].task_no = i;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}


			if (pthread_create(&th[i], NULL, get_handler_rw, (void *)&th_info[i]) != 0)
			{
					printf("Error on creating the thread\n");
					exit(0);
			}
			j++;
		}

		length = (arg.range_querries/ 2);
		m = 0;
		j = 0;
		for (; i < (arg.thread); i++)
		{

			strcpy(th_info[i].file_name, arg.rfile);
			m = j*length;

			if (i == (arg.thread - 1))
			{
				th_info[i].task_no = j;
				th_info[i].task_len = (arg.range_querries - m);
				th_info[i].task_key = m;
			}
			else
			{
				th_info[i].task_no = j;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}

			if (pthread_create(&th[i], NULL, range_handler_rw, (void *)&th_info[i]) != 0)
			{
					printf("Error on creating the thread\n");
					exit(0);
			}
			j++;
		}

		for (i = 0; i < arg.thread; i++) {
			pthread_join(th[i], NULL);
		}

		clock_gettime(CLOCK_MONOTONIC,&end1);

		pthread_rwlock_destroy(&bst_rwlock);

		printf("\n\n************Result for Range Query************\n");
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < querry_rw[i].size(); j++)
			{
				printf("Range Querry by thread %d for %d to %d = %d\n",i, querry_rw[i][j].start_key, querry_rw[i][j].end_key, querry_rw[i][j].node->key);
			}
		}
		printf("\nInorder Tree Traversal\n");
		print_tree_inorder(global_rw_root);

		free_tree(global_rw_root);

		unsigned long long elapsed_ns;
		elapsed_ns = (end1.tv_sec-start.tv_sec)*1000000000 + (end1.tv_nsec-start.tv_nsec);
		printf("Elapsed Time  for rw (ns): %llu\n",elapsed_ns);
		double elapsed_s = ((double)elapsed_ns)/1000000000.0;
		printf("Elapsed Time for rw(s): %lf\n",elapsed_s);
	}
	return 0;
}
