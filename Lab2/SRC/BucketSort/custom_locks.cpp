#include "custom_locks.h"

atomic<bool> lock;
//atomic<bool> tas_lock;
atomic<int> next_num (0);
atomic<int> now_serving (0);

pthread_mutex_t mutex;
pthread_barrier_t pthread_barrier;
extern atomic<Node*> tail;

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
