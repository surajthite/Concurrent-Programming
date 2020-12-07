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
 * @\file	mutex_lock_bst.c
 * @\author	Suraj Thite
 * @\brief	This file contains the application code for parallel tree
 */


#include "mutex_lock_bst.h"

extern bst_node *global_root;
extern pthread_mutex_t bst_lock;
extern vector <range> querry[2];

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function returns pointer to newnode with all the data memebers initialized.
 * @\input : key and value to be inserted.
 * @\return : pointer to initialized node.
 */

bst_node *get_new_node(int key, int val)
{
	bst_node *temp = (bst_node *)malloc(sizeof(bst_node));
	temp->key = key;
	temp->value = val;
	temp->left = NULL;
	temp->right = NULL;
	pthread_mutex_init(&temp->lock, NULL);
	return temp;
}


/**
 * @\Function : void put_node(bst_node *root, int key, int value, int thread_num)
 * @\brief : This function insters the value and key to the thread safe BST
 * @\input : pointer to root of BST, key , Value to be inserted , thread_num
 * @\return : void
 */

void put_node(bst_node *root, int key, int value, int thread_num)
{
	if (!root)
	{
		pthread_mutex_lock(&bst_lock);
		if (!global_root)
		{
			global_root = get_new_node(key, value);
			printf("New Node added to the tree with key(%d) and value(%d) \n", global_root->key, global_root->value);
			pthread_mutex_unlock(&bst_lock);
			return;
		}

		pthread_mutex_lock(&global_root->lock);
		root = global_root;
		pthread_mutex_unlock(&bst_lock);
	}

	if (key < root->key)
	{
		if (!root->left)
		{
			root->left = get_new_node(key, value);
			pthread_mutex_unlock(&root->lock);
		}
		else
		{
			pthread_mutex_lock(&root->left->lock);
			pthread_mutex_unlock(&root->lock);
			put_node(root->left, key, value, thread_num);
		}
	}
	else if (key > root->key)
    {
        if (!root->right)
        {
            root->right = get_new_node(key, value);
            pthread_mutex_unlock(&root->lock);
        }
				else
				{
            pthread_mutex_lock(&root->right->lock);
            pthread_mutex_unlock(&root->lock);
            put_node(root->right, key, value, thread_num);
        }
    }
		else
	{
		if (root->key == key)
			root->value = value;
		pthread_mutex_unlock(&root->lock);
	}
}

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function gets the data from the node depending upon the key value passed
 * @\input : pointer to root of BST , key value
 * @\return : void
 */

bst_node *get_node(bst_node *root, int key)
{
	if (!root)
	{
		pthread_mutex_lock(&bst_lock);
		if (!global_root)
		{
			pthread_mutex_unlock(&bst_lock);
			return NULL;
		}

		pthread_mutex_lock(&global_root->lock);
		root = global_root;
		pthread_mutex_unlock(&bst_lock);
	}

	if (key == root->key)
	{
		pthread_mutex_unlock(&root->lock);
		return root;
	}
	else if (key < root->key)
	{
		if (!root->left)
		{
			pthread_mutex_unlock(&root->lock);
			return NULL;
		}
		else
		{
			pthread_mutex_lock(&root->left->lock);
			pthread_mutex_unlock(&root->lock);
			get_node(root->left, key);
		}
	}
	else if (key > root->key)
    {
        if (!root->right)
        {
            pthread_mutex_unlock(&root->lock);
            return NULL;
        }
				else
				{
            pthread_mutex_lock(&root->right->lock);
            pthread_mutex_unlock(&root->lock);
            get_node(root->right, key);
        }
    }
		else
		{
			pthread_mutex_unlock(&root->lock);
			return NULL;
		}
}

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function does arange querry in between start_key and end_key
 * @\input : pointer to root, start key value , end key value , thread id
 * @\return : void
 */
void range_querry(bst_node *root, int start_key, int end_key, int tid)
{
	if (!root)
	{
		return;
	}

	bst_node *start_node = get_node(NULL, start_key);
	bst_node *end_node = get_node(NULL, end_key);

	if (!start_node)
	{
		printf("Invalid Query. Node with key %d is not present in the tree\n", start_key);
	}
	else if (!end_node)
	{
		printf("Invalid Query. Node with key %d is not present in the tree\n", end_key);
	}
	else
	{
		pthread_mutex_lock(&bst_lock);
		pthread_mutex_lock(&root->lock);
		pthread_mutex_unlock(&bst_lock);
		get_nodes_inrange(root, start_key, end_key, tid);
	}
}

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This function returns nodes in range between start_key and end_key
 * @\input :  pointer to root, start key value , end key value , thread id
 * @\return : void
 */
void get_nodes_inrange(bst_node *root, int start_key, int end_key, int tid)
{
	if (!root)
	{
		pthread_mutex_lock(&bst_lock);
		if (!global_root)
		{
			pthread_mutex_unlock(&bst_lock);
			return;
		}
		pthread_mutex_lock(&global_root->lock);
		root = global_root;
		pthread_mutex_unlock(&bst_lock);
	}

	if (start_key < root->key)
	{
		if (root->left)
		{
			pthread_mutex_lock(&root->left->lock);
			pthread_mutex_unlock(&root->lock);
			get_nodes_inrange(root->left, start_key, end_key, tid);
			pthread_mutex_lock(&root->lock);
		}

	}

	if ((start_key <= root->key) && (end_key >= root->key))
	{
		querry[tid].push_back({start_key, end_key, root});
	}

	if (end_key > root->key)
    {
        if (root->right)
        {
        	pthread_mutex_lock(&root->right->lock);
        	pthread_mutex_unlock(&root->lock);
        	get_nodes_inrange(root->right, start_key, end_key, tid);
        	pthread_mutex_lock(&root->lock);
        }
    }
    pthread_mutex_unlock(&root->lock);
}

/**
 * @\Function : void BSTtoArray(bst_node *root, int A[])
 * @\brief : This functions print the BST in inorder order
 * @\input : pointer to the root node of the BST
 * @\return : void
 */
void print_tree_inorder(bst_node *root)
{
	if (!root)
		return;

	print_tree_inorder(root->left);
	printf("%d %d\n", root->key, root->value);
	print_tree_inorder(root->right);

}


/**
 * @\Function : void free_tree(bst_noderw *root)
 * @\brief : This function frees the tree allocated in the heap section
 * @\input : Pointer to the root node of the BST
 * @\return : void
 */
void free_tree(bst_node *root)
{
	if (root)
	 {
        free_tree(root->right);
        free_tree(root->left);
        pthread_mutex_destroy(&root->lock);
        free(root);
    }
 }
