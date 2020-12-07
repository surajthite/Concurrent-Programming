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
 * @\file	rw_lock_bst.c
 * @\author	Suraj Thite
 * @\brief	This file contains the application code for parallel tree
 */

#include "rw_lock_bst.h"

extern bst_noderw *global_rw_root;
extern pthread_rwlock_t bst_rwlock; //rw_lock
extern vector <rangerw> querry_rw[2];

/**
 * @\Function :bst_noderw *get_new_node_rw(int key, int val)
 * @\brief : This function returns pointer to newnode with all the data memebers initialized.
 * @\input : key and value to be inserted.
 * @\return : pointer to initialized node.
 */

bst_noderw *get_new_node_rw(int key, int val)
{
	bst_noderw *temp = (bst_noderw *)malloc(sizeof(bst_noderw));

	temp->key = key;
	temp->value = val;
	temp->left = NULL;
	temp->right = NULL;

	pthread_rwlock_init(&temp->lock, NULL);

	return temp;
}

/**
 * @\Function : void put_node_rw(bst_noderw *root, int key, int value, int thread_num)
 * @\brief : This function insters the value and key to the thread safe BST
 * @\input : pointer to root of BST, key , Value to be inserted , thread_num
 * @\return : void
 */
void put_node_rw(bst_noderw *root, int key, int value, int thread_num)
{
	if (!root)
	{
		pthread_rwlock_wrlock(&bst_rwlock);
		if (!global_rw_root)
		{
			global_rw_root = get_new_node_rw(key, value);
			printf("Root node initialized with key %d and value %d\n", global_rw_root->key, global_rw_root->value);
			pthread_rwlock_unlock(&bst_rwlock);
			return;
		}

		pthread_rwlock_wrlock(&global_rw_root->lock);
		root = global_rw_root;
		pthread_rwlock_unlock(&bst_rwlock);
	}

	if (key < root->key)
	{
		if (!root->left)
		{
			root->left = get_new_node_rw(key, value);
			pthread_rwlock_unlock(&root->lock);
		} else {
			pthread_rwlock_wrlock(&root->left->lock);
			pthread_rwlock_unlock(&root->lock);
			put_node_rw(root->left, key, value, thread_num);
		}
	}
	else if (key > root->key)
    {
        if (!root->right)
        {
            root->right = get_new_node_rw(key, value);
            pthread_rwlock_unlock(&root->lock);
        } else {
            pthread_rwlock_wrlock(&root->right->lock);
            pthread_rwlock_unlock(&root->lock);
            put_node_rw(root->right, key, value, thread_num);
        }
    }
		else
		{
			if (key == root->key)
				root->value = value;
		pthread_rwlock_unlock(&root->lock);
		}
}

/**
 * @\Function : bst_noderw *get_node_rw(bst_noderw *root, int key)
 * @\brief : This function gets the data from the node depending upon the key value passed
 * @\input : pointer to root of BST , key value
 * @\return : void
 */

bst_noderw *get_node_rw(bst_noderw *root, int key)
{
	if (!root)
	{
		pthread_rwlock_rdlock(&bst_rwlock);
		if (!global_rw_root)
		{
			pthread_rwlock_unlock(&bst_rwlock);
			return NULL;
		}

		pthread_rwlock_rdlock(&global_rw_root->lock);
		root = global_rw_root;
		pthread_rwlock_unlock(&bst_rwlock);
	}

	if (key == root->key)
	{
		pthread_rwlock_unlock(&root->lock);
		return root;
	}
	else if (key < root->key)
	{
		if (!root->left)
		{
			pthread_rwlock_unlock(&root->lock);
			return NULL;
		}
		else
		{
			pthread_rwlock_rdlock(&root->left->lock);
			pthread_rwlock_unlock(&root->lock);
			get_node_rw(root->left, key);
		}
	}
	else if(key > root->key)
    {
        if(!root->right)
        {
            //printf("Searched failed for node with key %d\n", key);
            pthread_rwlock_unlock(&root->lock);
            return NULL;
        } else
				{
            pthread_rwlock_rdlock(&root->right->lock);
            pthread_rwlock_unlock(&root->lock);
            get_node_rw(root->right, key);
        }
    }
	else
	{
		pthread_rwlock_unlock(&root->lock);
		return NULL;
	}

}

/**
 * @\Function : void range_querry_rw(bst_noderw *root, int start_key, int end_key, int tid)
 * @\brief : This function does arange querry in between start_key and end_key
 * @\input : pointer to root, start key value , end key value , thread id
 * @\return : void
 */
void range_querry_rw(bst_noderw *root, int start_key, int end_key, int tid)
{
	if (!root)
	{
		return;
	}

	bst_noderw *start_node = get_node_rw(NULL, start_key);
	bst_noderw *end_node = get_node_rw(NULL, end_key);

	if (!start_node)
	{
		printf("Invalid Query. Node with key %d is not present in the tree\n", start_key);
		return;
	} else if (!end_node)
	{
		printf("Invalid Query. Node with key %d is not present in the tree\n", end_key);
		return;
	}
	else
	{
		pthread_rwlock_rdlock(&bst_rwlock);
		pthread_rwlock_rdlock(&root->lock);
		pthread_rwlock_unlock(&bst_rwlock);
		get_nodes_inrange_rw(root, start_key, end_key, tid);
	}
}

/**
 * @\Function : void get_nodes_inrange_rw(bst_noderw *root, int start_key, int end_key, int tid)
 * @\brief : This function returns nodes in range between start_key and end_key
 * @\input :  pointer to root, start key value , end key value , thread id
 * @\return : void
 */
void get_nodes_inrange_rw(bst_noderw *root, int start_key, int end_key, int tid)
{
	if (!root)
	{
		pthread_rwlock_rdlock(&bst_rwlock);
		if (!global_rw_root)
		{
			pthread_rwlock_unlock(&bst_rwlock);
			return;
		}

		pthread_rwlock_rdlock(&global_rw_root->lock);
		root = global_rw_root;
		pthread_rwlock_unlock(&bst_rwlock);
	}

	if (start_key < root->key)
	{
		if (root->left )
		{
			pthread_rwlock_rdlock(&root->left->lock);
			pthread_rwlock_unlock(&root->lock);
			get_nodes_inrange_rw(root->left, start_key, end_key, tid);
			pthread_rwlock_rdlock(&root->lock);
		}
	}

	if ((start_key <= root->key) && (end_key >= root->key))
	{
		querry_rw[tid].push_back({start_key, end_key, root});
	}

	if (end_key > root->key)
    {
        if (root->right)
        {
        	pthread_rwlock_rdlock(&root->right->lock);
    		pthread_rwlock_unlock(&root->lock);
        	get_nodes_inrange_rw(root->right, start_key, end_key, tid);
        	pthread_rwlock_rdlock(&root->lock);
        }
    }

    pthread_rwlock_unlock(&root->lock);
}

/**
 * @\Function : void print_tree_inorder(bst_noderw *root)
 * @\brief : This functions print the BST in inorder order
 * @\input : pointer to the root node of the BST
 * @\return : void
 */
void print_tree_inorder(bst_noderw *root)
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
void free_tree(bst_noderw *root)
{
	if (root)
	{
        free_tree(root->right);
        free_tree(root->left);
        pthread_rwlock_destroy(&root->lock);
        free(root);
    }
 }
