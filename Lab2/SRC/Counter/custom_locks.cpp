#include "custom_locks.h"

#define NUM_THREADS  100

atomic<bool> lock;
//atomic<bool> tas_lock;
atomic<int> next_num (0);
atomic<int> now_serving (0);

pthread_mutex_t mutex;
pthread_barrier_t pthread_barrier;
extern atomic<Node*> tail;
extern struct handler handler_t;
bar_t sense_bar_var;

void TAS_lock()
{
	bool ex_val, changed_val;
    do{
        ex_val = false;
        changed_val = true;
    }while(!lock.compare_exchange_strong(ex_val,changed_val));
}

void TAS_unlock()
{
	lock.store(false);
}

void TTAS_lock()
{
    do {
    	while (lock.load()) continue;
    } while (lock.exchange(true)); // actual atomic locking
    return;
}

void TTAS_unlock()
{
	lock.store(false);
}

void  Ticket_lock()
{
	int my_num = next_num.fetch_add(1);
	while (now_serving.load() != my_num) { }
}

void  Ticket_unlock()
{
	now_serving.fetch_add(1);
}

void  Pthread_lock()
{
	pthread_mutex_lock(&mutex);
}

void Pthread_unlock()
{
	pthread_mutex_unlock(&mutex);
}

void Pthread_barrier()
{
	pthread_barrier_wait(&pthread_barrier);
}

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
