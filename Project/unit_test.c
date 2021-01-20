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
 * @\file	unit_test.c
 * @\author	Suraj Thite
 * @\brief	This file contains the application code for parallel tree
 */

#include "mutex_lock_bst.h"
#include <stdbool.h>


vector <range> querry[2];

bst_node *global_root;
pthread_mutex_t bst_lock;

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function converts the BST and stores it in the integer array pointer passed as argument.
 * @\input : pointer to the root node of the binary search tree , integer value.
 * @\return : void
 */
void BSTtoArray(bst_node *root, int A[])
{
    static int pos = 0;
    if(root == NULL) return;

    BSTtoArray(root->left, A);
    A[pos++] = root->key;
    BSTtoArray(root->right, A);

}

/**
 * @\Function : bool put_function_test()
 * @\brief : This function tests the put_function
 * @\input : void
 * @\return : bool
 */
bool put_function_test()
{
	int i = 0;
	for (i = 0; i < 50; i++)
  {
		put_node(NULL, i, (2*i), 0);
	}

	int *A = new int[50];
    BSTtoArray(global_root, A);

    for (i = 0; i < 50; i++)
    {
    	assert(i == A[i]);
	  }
	free(A);
	return true;
}

/**
 * @\Function : bool get_function_test()
 * @\brief : This function tests the get_function
 * @\input :
 * @\return : bool
 */

bool get_function_test()
{
	int i = 0;
	bst_node *node;

	for (i = 0; i < 50; i++)
  {
		node = get_node(NULL, i);
		if (node == NULL)
			return false;
		assert(node->value == (2*i));
	}
	return true;
}

/**
 * @\Function : bool range_function_test()
 * @\brief : This function tests the range_querry function and compares it with the expected output for 50 values
 * @\input : void
 * @\return : bool
 */
bool range_function_test()
{
	range_querry(global_root, 0, 49, 0);
	int i = 0;
	for (i = 0; i < 50; i++)
  {
    	assert(i == querry[0][i].node->key);
	}

	return true;
}


/**
 * @\Function : int main()
 * @\brief : This is the main function where the execution starts
 * @\input : void
 * @\return : int
 */
int main()
{
  printf("*********Starting Unit Test *************\n");
	if (put_function_test())
  {
		printf("1.Put function test passed !!\n");
	}

	if (get_function_test())
  {
		printf("2.Get function test passed !!\n");
	}

	if (range_function_test())
		printf("3.Range Querry function test passed\n");

	return 0;
}
