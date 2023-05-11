#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore
{
	int count;
	// A queue of threads that are waiting on the semaphore
	queue_t blocked_q;
};

sem_t sem_create(size_t count)
{
	sem_t sem = (sem_t)malloc(sizeof(struct semaphore));
	if (sem != NULL)
	{
		// Create a new queue for the blocked threads
		sem->blocked_q = queue_create();
		if (sem->blocked_q == NULL)
			return NULL;
		// Set the initial count of the semaphore
		sem->count = count;
		
	}
	return sem;
}

int sem_destroy(sem_t sem)
{
	// Check if the semaphore is NULL or has blocked threads
	if (sem == NULL || queue_length(sem->blocked_q) != 0)
	{
		return -1;
	}
	// Check if the semaphore count is negative
	if (sem->count < 0)
	{
		// Return an error
		return -1;
	}
	// Destroy the blocked queue and free the semaphore
	queue_destroy(sem->blocked_q);
	free(sem);
	
	return 0;
}

int sem_down(sem_t sem)
{
	// Check if the semaphore is NULL
	if (sem == NULL)
		return -1;

	preempt_disable();
	// If the count is zero, add the current thread to the blocked queue and block it
	if (sem->count == 0)
	{
		queue_enqueue(sem->blocked_q, uthread_current());
		uthread_block(); 
	}
	sem->count--;

	preempt_enable();
	return 0;
}

int sem_up(sem_t sem)
{
	// Check if the semaphore is NULL
	if (sem == NULL)
		return -1;

	preempt_disable();
	sem->count++;

	// If there are threads waiting and the count is positive, unblock the first thread in the queue
	if(queue_length(sem->blocked_q) != 0)
	{
		struct uthread_tcb* thread_to_unblock;
		if (sem->count > 0)
		{
			queue_dequeue(sem->blocked_q, (void**)&thread_to_unblock);
			uthread_unblock(thread_to_unblock); 
		}
	}

	preempt_enable();

	return 0;
}