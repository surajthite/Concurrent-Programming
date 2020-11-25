
#include "mutex_lock_bst.h"
#include "rw_lock_bst.h"
#include "arg_parser.h"

struct thread_info
{
	int task_no;
	int task_len;
	int task_key;
	char filename[64];
};
