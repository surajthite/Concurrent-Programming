#include "rw_lock_bst.h"

extern bst_noderw *g_rw_root;
extern pthread_rwlock_t bst_rwlock; //rw_lock
extern vector <rangerw> querryrw[2];

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

void update_val(bst_noderw *node, int val)
{
	node->value = val;
}

void put_node_rw(bst_noderw *root, int key, int value, int thread_num)
{
	if (root == NULL)
	{
		pthread_rwlock_wrlock(&bst_rwlock);  //rwlock_wrlock
		if (g_rw_root == NULL)
		{
			g_rw_root = get_new_node_rw(key, value);
			printf("Root node initialized with key %d and value %d\n", g_rw_root->key, g_rw_root->value);
			pthread_rwlock_unlock(&bst_rwlock);
			return;
		}

		pthread_rwlock_wrlock(&g_rw_root->lock);
		root = g_rw_root;
		pthread_rwlock_unlock(&bst_rwlock);
	}

	if (key < root->key)
	{
		if (root->left == NULL)
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
        if (root->right == NULL)
        {
            root->right = get_new_node_rw(key, value);
            pthread_rwlock_unlock(&root->lock);
        } else {
            pthread_rwlock_wrlock(&root->right->lock);
            pthread_rwlock_unlock(&root->lock);
            put_node_rw(root->right, key, value, thread_num);
        }
    } else {
		if (key == root->key) {
			update_val(root, value);
		}
		pthread_rwlock_unlock(&root->lock);
	}
}

bst_noderw *get_node_rw(bst_noderw *root, int key)
{
	if (root == NULL)
	{
		pthread_rwlock_rdlock(&bst_rwlock);
		if (g_rw_root == NULL)
		{
			//printf("Searched failed for node with key %d\n", key);
			pthread_rwlock_unlock(&bst_rwlock);
			return NULL;
		}

		pthread_rwlock_rdlock(&g_rw_root->lock);
		root = g_rw_root;
		pthread_rwlock_unlock(&bst_rwlock);
	}

	if (key == root->key)
	{
		pthread_rwlock_unlock(&root->lock);
		return root;
	}
	else if (key < root->key)
	{
		if (root->left == NULL)
		{
			//printf("Searched failed for node with key %d\n", key);
			pthread_rwlock_unlock(&root->lock);
			return NULL;
		} else {
			pthread_rwlock_rdlock(&root->left->lock);
			pthread_rwlock_unlock(&root->lock);
			get_node_rw(root->left, key);
		}
	}
	else if (key > root->key)
    {
        if (root->right == NULL)
        {
            //printf("Searched failed for node with key %d\n", key);
            pthread_rwlock_unlock(&root->lock);
            return NULL;
        } else {
            pthread_rwlock_rdlock(&root->right->lock);
            pthread_rwlock_unlock(&root->lock);
            get_node_rw(root->right, key);
        }
    } else {
		pthread_rwlock_unlock(&root->lock);
		return NULL;
	}

	//return NULL;
}

void range_querry_rw(bst_noderw *root, int start_key, int end_key, int tid)
{
	if (root == NULL) {
		return;
	}

	bst_noderw *start_node = get_node_rw(NULL, start_key);
	bst_noderw *end_node = get_node_rw(NULL, end_key);

	if (start_node == NULL) {
		printf("Invalid Query. Node with key %d is not present in the tree\n", start_key);
		return;
	} else if (end_node == NULL) {
		printf("Invalid Query. Node with key %d is not present in the tree\n", end_key);
		return;
	} else {
		pthread_rwlock_rdlock(&bst_rwlock);
		pthread_rwlock_rdlock(&root->lock);
		pthread_rwlock_unlock(&bst_rwlock);
		//printf("Range query result for %d to %d\n", start_key, end_key);
		get_nodes_inrange_rw(root, start_key, end_key, tid);
	}
}

void get_nodes_inrange_rw(bst_noderw *root, int start_key, int end_key, int tid)
{
	if (root == NULL)
	{
		pthread_rwlock_rdlock(&bst_rwlock);
		if (g_rw_root == NULL)
		{
			//printf("Searched failed for node with key %d\n", key);
			pthread_rwlock_unlock(&bst_rwlock);
			return;
		}

		pthread_rwlock_rdlock(&g_rw_root->lock);
		root = g_rw_root;
		pthread_rwlock_unlock(&bst_rwlock);
	}

	if (start_key < root->key)
	{
		if (root->left != NULL)
		{
			pthread_rwlock_rdlock(&root->left->lock);
			pthread_rwlock_unlock(&root->lock);
			get_nodes_inrange_rw(root->left, start_key, end_key, tid);
			pthread_rwlock_rdlock(&root->lock);
		}
	}

	if ((start_key <= root->key) && (end_key >= root->key))
	{
		querryrw[tid].push_back({start_key, end_key, root});
	}

	if (end_key > root->key)
    {
        if (root->right != NULL)
        {
        	pthread_rwlock_rdlock(&root->right->lock);
    		pthread_rwlock_unlock(&root->lock);
        	get_nodes_inrange_rw(root->right, start_key, end_key, tid);
        	pthread_rwlock_rdlock(&root->lock);
        }
    }

    pthread_rwlock_unlock(&root->lock);
}

void print_tree(bst_noderw *root)
{
	if (root == NULL)
		return;

	print_tree(root->left);
	printf("%d %d\n", root->key, root->value);
	print_tree(root->right);

}


void free_tree(bst_noderw *root)
{
	if (root != NULL) {
        free_tree(root->right);
        free_tree(root->left);
        pthread_rwlock_destroy(&root->lock);
        free(root);
    }
 }
