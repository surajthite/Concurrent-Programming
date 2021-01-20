#ifndef __LOCKS_H__
#define __LOCKS_H__

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <sstream>
#include<pthread.h>
#include <counter.h>
using namespace std;

typedef struct Barrier {
    atomic<int> cnt;
    atomic<int> sense;
    int N;
}bar_t;

class Node
{
public:
  atomic<bool> wait;
	atomic<Node*> next;
};

extern atomic<Node*> tail;


class MCSLock
{
public:
	void acquire(Node *myNode)
  {
	   Node *oldTail = tail.load();
	   myNode->next.store(NULL, memory_order_relaxed);
	   while (!tail.compare_exchange_strong(oldTail, myNode))
    {
		    oldTail = tail.load();
    }

	if(oldTail != NULL)
  {
		myNode->wait.store(true, memory_order_relaxed);
		oldTail->next.store(myNode);
		while (myNode->wait.load()) {}
	}
	}

	void release(Node *myNode)
  {
		Node* temp_node = myNode;
		if (tail.compare_exchange_strong(temp_node, NULL))
    {

		}
    else
    {
			while (myNode->next.load() == NULL) {}
			myNode->next.load()->wait.store(false);
		}
	}
};



void TAS_lock();
void TAS_unlock();

void TTAS_lock();
void TTAS_unlock();

void Ticket_lock();
void Ticket_unlock();

void Pthread_lock();
void Pthread_unlock();

void Pthread_barrier();
void sense_bar() ;

#endif
