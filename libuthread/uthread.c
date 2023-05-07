#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

queue_t ready_q;

typedef struct uthread_context {
    void *pc;      // Program counter 
    void *sp;      // Stack pointer 
    void *regs[8]; // General-purpose registers 
} thread_ctx;

typedef struct uthread_tcb {
    thread_ctx *context; // Pointer to the thread context
    enum {
        RUNNING,
        READY,
        BLOCKED,
        EXITED
    } state;
} tcb_t; // renamed to thread control block

tcb_t *uthread_current(void)
{
	/* TODO Phase 2/3 */
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	// Allocate memory for the thread control block (tcb)
    tcb_t *new_tcb = malloc(sizeof(tcb_t));
	new_tcb->context = malloc(sizeof(thread_ctx));

    // Allocate memory for the thread's stack
    void *new_stack = uthread_ctx_alloc_stack();

    // Initialize the thread's context
    if (uthread_ctx_init(&(new_tcb->context), new_stack, func, arg) < 0) {
        perror("uthread_ctx_init");
        return -1;
    }

    // Set the thread state
    new_tcb->state = READY;

    // Add the new thread to the ready queue
	queue_enqueue(ready_q, &new_tcb);

    return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	int num_threads;
	int *ptr;
	// Initialize the uthread library
    uthread_create(func, arg);
	tcb_t *idle_thread = uthread_current();
    idle_thread->context = malloc(sizeof(thread_ctx));
    getcontext(idle_thread->context);
    idle_thread->state = RUNNING;

	ready_q = queue_create();

    // Create the initial thread
    uthread_create(func, arg);
    // Set the initial thread as the current thread
    tcb_t *current_thread = get_next_ready_thread();
    uthread_ctx_switch(&(idle_thread->context), &(current_thread->context));

    // Start the idle thread loop
    while (1) {
        // Check if there are any threads ready to run
        if (queue_length(ready_q) == 0) {
            // If not, exit the loop
            break;
        }

        // Get the next thread from the ready queue
        
		tcb_t *next_thread = queue_dequeue(ready_q, (void**)&ptr);

        // If the next thread is the idle thread, continue with the loop
        if (next_thread == idle_thread) {
            continue;
        }

        // Set the next thread as the current thread
        uthread_ctx_switch(&current_thread->context, &next_thread->context);

        // If the current thread has exited, free its TCB and remove it from the scheduler
        if (current_thread->state == EXITED) {
			uthread_ctx_destroy_stack(&(current_thread->context->sp));
            free(current_thread);
            num_threads--;
        }

        // Set the next thread as the current thread
        current_thread = next_thread;
    }

    // Free the idle thread stack and TCB
	uthread_ctx_destroy_stack(&(idle_thread));
    //free(idle_thread->context.uc_stack.ss_sp);
    free(idle_thread);
    num_threads--;

    // Reset the uthread library
    //uthread_reset(); type function?

    return 0;
}


void uthread_block(void)
{
	/* TODO Phase 3 */
}

void uthread_unblock(tcb_t *uthread)
{
	/* TODO Phase 3 */
}

