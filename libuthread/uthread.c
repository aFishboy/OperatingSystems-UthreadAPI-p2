#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

queue_t ready_q;
queue_t zombie_q;


typedef struct uthread_tcb {
    uthread_ctx_t *context; // Pointer to the thread context
    void *sp; //pointer to top of stack
    enum {
        RUNNING,
        READY,
        BLOCKED,
        EXITED
    } state;
} tcb_t; // renamed to thread control block

tcb_t *idle_thread;
tcb_t *current_thread;

tcb_t *uthread_current(void)
{
	return current_thread;
}

void uthread_yield(void)
{
	// enqueue the current running thread into the ready queue
    queue_enqueue(ready_q, current_thread); // add current thread to end of queue

	tcb_t *next = NULL;
	queue_dequeue(ready_q, (void**)&next); //pop the next to-run thread
    if (next == NULL){
        printf("next null\n");
    }
    
	tcb_t *curr = current_thread;
    if (curr == NULL){
        printf("curr null\n");
    }
	
	if (curr != next)
	{
		current_thread->state = READY; //state change for context switch
        printf("idle %p\n", idle_thread);
        printf("idle->context %p\n", idle_thread->context);
        printf("current0 %p\n", current_thread);
        printf("current0->context %p\n", current_thread->context);
        printf("next0 %p\n", next);
        printf("next0->context %p\n", next->context);
        printf("curr0 %p\n", curr);
        printf("curr0->context %p\n\n", curr->context);

		next->state = RUNNING;
        printf("idle %p\n", idle_thread);
        printf("idle->context %p\n", idle_thread->context);
        printf("current1 %p\n", current_thread);
        printf("current1->context %p\n", current_thread->context);
        printf("next1 %p\n", next);
        printf("next1->context %p\n", next->context);
        printf("curr1 %p\n", curr);
        printf("curr1->context %p\n", curr->context);
        

		current_thread = next;
        printf("here5\n");
		uthread_ctx_switch(curr->context, next->context);
        printf("here6\n");

	}
}

void uthread_exit(void)
{
	current_thread->state = EXITED;
	uthread_ctx_destroy_stack(current_thread->sp);
	queue_enqueue(zombie_q, current_thread);

	tcb_t *next;
	queue_dequeue(ready_q, (void**)&next); //next will have the dequeued thread
	tcb_t *curr = current_thread;

	current_thread = next;
	current_thread->state = RUNNING;
	uthread_ctx_switch(curr->context, next->context);
}

int uthread_create(uthread_func_t func, void *arg)
{
	// Allocate memory for the thread control block (tcb)
    tcb_t *new_tcb = malloc(sizeof(tcb_t));
    if(new_tcb == NULL){
        return -1;
    }

    // Initialize the thread's context
	new_tcb->context = malloc(sizeof(uthread_ctx_t));
    if(new_tcb->context == NULL){
        return -1;
    }
    // Allocate memory for the thread's stack
    new_tcb->sp = uthread_ctx_alloc_stack();
    // Set the thread state
    new_tcb->state = READY;
    if (uthread_ctx_init(new_tcb->context, new_tcb->sp, func, arg)){
        perror("uthread_ctx_init");
        return -1;
    }
    // Add the new thread to the ready queue
	queue_enqueue(ready_q, &new_tcb);

    return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    if (preempt){

    }
	// Initialize the idle thread
	idle_thread = malloc(sizeof(tcb_t));
    if(idle_thread == NULL){
        return -1;
    }
	idle_thread->context = malloc(sizeof(uthread_ctx_t));
    if(idle_thread->context == NULL){
        return -1;
    }
    idle_thread->state = RUNNING;
    current_thread = idle_thread;

	ready_q = queue_create();
    zombie_q = queue_create();
    if (ready_q == NULL || zombie_q == NULL){
        return -1;
    }

    // Create the initial thread
    if (uthread_create(func, arg)){
        return -1;
    }
    
    // Start the idle thread loop
    while (1) {
        // Check if there are any threads ready to run
        if (queue_length(ready_q) == 0) {
            // If not, exit the loop
            break;
        }
    printf("here\n");

        // Get the next thread from the ready queue by yielding
        uthread_yield();
    }

    return 0;
}


void uthread_block(void)
{
	/* TODO Phase 3 */
}

void uthread_unblock(tcb_t *uthread)
{
    if (uthread){

    }
	/* TODO Phase 3 */
}

