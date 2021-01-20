#include "custom_locks.h"

#define NUM_THREADS  100

atomic<bool> lock;
atomic<int> next_num (0);
atomic<int> now_serving (0);

extern pthread_mutex_t bucket_lock;
extern pthread_barrier_t pthread_barrier_1;

extern atomic<Node*> tail;
extern struct handler handler_t;
bar_t sense_bar_var;

/*
Name : void TAS_lock()
Description : Implements the locking functionality for TAS Lock
Input: void
Return: void
*/

void TAS_lock()
{

	while(lock.exchange(true));
}

/*
Name : void TAS_unlock()
Description : Implements the un;ocking functionality for TAS Unlock
Input: void
Return: void
*/
void TAS_unlock()
{
	lock.exchange(false);
}

/*
Name : void TTAS_lock()
Description : Implements the locking functionality for TAS Lock
Input: void
Return: void
*/

void TTAS_lock()
{
	while(lock.load() == true || lock.exchange(true));
}

/*
Name : void TTAS_unlock()
Description : Implements the unlocking functionality for TTAS Lock
Input: void
Return: void
*/
void TTAS_unlock()
{
	lock.exchange(false);
}

/*
Name : void  Ticket_lock()
Description : Implements the locking functionality for TAS Lock
Input: void
Return:void
*/
void  Ticket_lock()
{
//	printf("Executing Ticket Lock");
	int my_num = next_num.fetch_add(1);
	while (now_serving.load() != my_num) { }
}

/*
Name : void  Ticket_unlock()
Description :Implements the un;ocking functionality for TAS Unlock
Input: void
Return:  void
*/
void  Ticket_unlock()
{
	now_serving.fetch_add(1);
}

/*
Name : void  Pthread_lock()
Description : Implements the locking functionality for TAS Lock
Input:void
Return:void
*/
void  Pthread_lock()
{
	pthread_mutex_lock(&bucket_lock);
}
/*
Name : void Pthread_unlock()
Description : Implements the un;ocking functionality for TAS Lock
Input: void
Return: void
*/
void Pthread_unlock()
{
	pthread_mutex_unlock(&bucket_lock);
}
/*
Name : void Pthread_barrier()
Description : Implements the pthread barrier
Input: void
Return: void
*/
void Pthread_barrier()
{
	pthread_barrier_wait(&pthread_barrier_1);
}
/*
Name : void sense_bar()
Description : Implements the sense reversal barrier
Input: void
Return: void
*/
void sense_bar()
{
    thread_local bool my_sense;
    my_sense = (my_sense == 0) ? 1 : 0;

    int cnt_cpy = sense_bar_var.cnt.fetch_add(1);
    if(cnt_cpy+1 == handler_t.thread_cnt) {

        sense_bar_var.cnt.store(0, memory_order_relaxed);
        sense_bar_var.sense.store(my_sense);
    }
    else {
        while(sense_bar_var.sense.load() != my_sense);
    }
}
