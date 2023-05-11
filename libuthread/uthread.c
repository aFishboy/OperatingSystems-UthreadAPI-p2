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

typedef struct uthread_tcb
{
    uthread_ctx_t *context; // Pointer to the thread context
    void *sp;               // pointer to top of stack
    enum
    {
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
    tcb_t *next_thread;
    tcb_t *prev_thread = current_thread;

    // Add the current running thread to the ready queue
    queue_enqueue(ready_q, current_thread);

    // Dequeue the next thread from the ready queue and store pointer in next thread
    queue_dequeue(ready_q, (void **)&next_thread);

    // If the next thread is not the same as the previous thread, switch to the next thread
    if (prev_thread != next_thread)
    {
        next_thread->state = RUNNING;
        current_thread->state = READY;
        current_thread = next_thread;
        // Switch the context from the previous thread to the next thread
        uthread_ctx_switch(prev_thread->context, next_thread->context);
    }
}

void uthread_exit(void)
{
    tcb_t *next_thread;
    tcb_t *prev_thread = current_thread;

    // Destroy the stack of the current thread
    uthread_ctx_destroy_stack(current_thread->sp);

    // Add the current thread to the zombie queue
    queue_enqueue(zombie_q, current_thread);
    current_thread->state = EXITED;

    // Dequeue the next thread from the ready queue and store pointer in next thread
    queue_dequeue(ready_q, (void **)&next_thread);
    current_thread = next_thread;
    current_thread->state = RUNNING;

    // Switch the context from the previous thread to the next thread
    uthread_ctx_switch(prev_thread->context, next_thread->context);
}

int uthread_create(uthread_func_t func, void *arg)
{
    // Allocate memory for the thread control block (tcb)
    tcb_t *new_tcb = malloc(sizeof(tcb_t));
    if (new_tcb == NULL)
    {
        return -1;
    }

    // Initialize the thread's context
    new_tcb->context = malloc(sizeof(uthread_ctx_t));
    if (new_tcb->context == NULL)
    {
        return -1;
    }
    // Allocate memory for the thread's stack
    new_tcb->sp = uthread_ctx_alloc_stack();
    if (new_tcb->sp == NULL)
    {
        return -1;
    }
    // Set the thread state
    new_tcb->state = READY;
    if (uthread_ctx_init(new_tcb->context, new_tcb->sp, func, arg))
    {
        perror("uthread_ctx_init");
        return -1;
    }
    // Add the new thread to the ready queue
    if (queue_enqueue(ready_q, new_tcb))
    {
        return -1;
    }

    return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    preempt_start(preempt);
    
    // Allocate memory for the idle thread and its context
    idle_thread = malloc(sizeof(tcb_t));
    if (idle_thread == NULL)
    {
        return -1;
    }
    idle_thread->context = malloc(sizeof(uthread_ctx_t));
    if (idle_thread->context == NULL)
    {
        return -1;
    }

    // Set the state of the idle thread to "running" and make it the current thread
    idle_thread->state = RUNNING;
    current_thread = idle_thread;

    // Create the ready queue and the zombie queue
    ready_q = queue_create();
    zombie_q = queue_create();
    if (ready_q == NULL || zombie_q == NULL)
    {
        return -1;
    }

    // Create the initial thread
    if (uthread_create(func, arg))
    {
        return -1;
    }

    // Start the idle thread loop
    while (1)
    {
        // Check if there are any threads ready to run
        if (queue_length(ready_q) == 0)
        {
            // If 0, exit the loop
            break;
        }

        // Get the next thread from the ready queue by yielding
        uthread_yield();
    }
    while (queue_length(zombie_q) > 0) //destroy threads that are in zombie_q 
    {
        void* ptr;
        if (queue_dequeue(zombie_q, (void**) &ptr))
        {
            return -1;
        }
        free(ptr);
    }
    if (queue_destroy(ready_q) || queue_destroy(zombie_q))
    {
        return -1;
    }
    if (preempt)
    {
        preempt_stop();
    }
    return 0;
}

void uthread_block(void)
{
    /* TODO Phase 3 */
    // Block the current thread and take a new thread from the queue
    current_thread->state = BLOCKED;
    tcb_t *curr = current_thread;
    tcb_t *next;

    queue_dequeue(ready_q, (void **)&next);

    if (curr != next)
    {
        next->state = RUNNING;
        current_thread = next;
        uthread_ctx_switch(curr->context, next->context);
    }
}

void uthread_unblock(tcb_t *uthread)
{
    if (uthread != NULL)
    {
        uthread->state = READY;
        queue_enqueue(ready_q, uthread); // Add the thread to the ready queue
    }
}
