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
 * @\file	mutex_lock_bst.h
 * @\author	Suraj Thite
 * @\brief	This file contains the application code for parallel tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

//Structure for a nodeto BST
typedef struct node
{
	int key;
	int value;
	struct node *right;
	struct node *left;
	pthread_mutex_t lock;
}bst_node;

//Structure for Test
typedef struct Test
{
   int start_key;
   int end_key;
   bst_node *node;
}range;

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function returns pointer to newnode with all the data memebers initialized.
 * @\input : key and value to be inserted.
 * @\return : pointer to initialized node.
 */
bst_node *get_new_node(int key, int val);

/**
 * @\Function : void put_node(bst_node *root, int key, int value, int thread_num)
 * @\brief : This function insters the value and key to the thread safe BST
 * @\input : pointer to root of BST, key , Value to be inserted , thread_num
 * @\return : void
 */
void put_node(bst_node *root, int key, int value, int thread_num);

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function gets the data from the node depending upon the key value passed
 * @\input : pointer to root of BST , key value
 * @\return : void
 */
bst_node *get_node(bst_node *root, int key);

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function does arange querry in between start_key and end_key
 * @\input : pointer to root, start key value , end key value , thread id
 * @\return : void
 */
void range_querry(bst_node *root, int start_key, int end_key, int tid);

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function returns nodes in range between start_key and end_key
 * @\input :  pointer to root, start key value , end key value , thread id
 * @\return : void
 */
void get_nodes_inrange(bst_node *root, int start_key, int end_key, int tid);

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This functions print the BST in inorder order
 * @\input : pointer to the root node of the BST
 * @\return : void
 */
void print_tree_inorder(bst_node *root);

/**
 * @\Function : void free_tree(bst_noderw *root)
 * @\brief : This function frees the tree allocated in the heap section
 * @\input : Pointer to the root node of the BST
 * @\return : void
 */
void free_tree(bst_node *root);
