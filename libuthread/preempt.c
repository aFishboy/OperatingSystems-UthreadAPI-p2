#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100
#define SECOND 1000
// int preempt;
sigset_t signal_set;
struct itimerval time_interval;
struct sigaction signal_act;

void sig_handler()
{
	// Interval between reaching preemption, preemption process
	uthread_yield();
}

void preempt_disable(void)
{
	// enable preemption, set SIG_BLOCK
	sigset_t oldSet;							  // return value
	sigprocmask(SIG_BLOCK, &signal_set, &oldSet); // set to SIG_BLOCK
}

void preempt_enable(void)
{
	// disable preemption, set SIG_BLOCK
	sigset_t oldSet;								// return value
	sigprocmask(SIG_UNBLOCK, &signal_set, &oldSet); // set to SIG_UNBLOCK
}

void preempt_start(bool preempt)
{
	if (preempt)
	{
		// Set interval of 100hz per second
		// Set how many executions per interval
		time_interval.it_interval.tv_sec = 0;
		time_interval.it_interval.tv_usec = SECOND / HZ;
		// Set the execution time after the first delay
		time_interval.it_value.tv_sec = 0;
		time_interval.it_value.tv_usec = SECOND / HZ;

		sigemptyset(&signal_set);		   // Initialize signal set
		sigaddset(&signal_set, SIGVTALRM); // set virtual alarm

		signal_act.sa_handler = &sig_handler; // Set the signal handling function and use the sig_handler function to process the signal

		sigaction(SIGVTALRM, &signal_act, NULL); // Set up signal handling, use signal_act to handle signals

		setitimer(ITIMER_VIRTUAL, &time_interval, NULL); // set timer
	}
}

void preempt_stop(void)
{
	// Cancel the set timer and set the time interval to 0, which means no timing
	// set how many intervals during execution
	time_interval.it_interval.tv_sec = 0;
	time_interval.it_interval.tv_usec = 0;
	// Set the execution time after the first delay
	time_interval.it_value.tv_sec = 0;
	time_interval.it_value.tv_usec = 0;
	setitimer(ITIMER_VIRTUAL, &time_interval, NULL); // cancel timer
}
