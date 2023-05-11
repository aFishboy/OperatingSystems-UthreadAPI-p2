#include <stdio.h>
#include <stdlib.h>
#include<signal.h>
#include <sys/time.h>

#include "uthread.h"
#include "private.h"

void busy_wait() {
    struct timeval start_time, current_time;
    gettimeofday(&start_time, NULL);
    while (1) {
        gettimeofday(&current_time, NULL);
        if ((current_time.tv_sec - start_time.tv_sec) >= 1) {
            break;
        }
    }
}

void thread3(void* arg)
{
    printf("thread3\n");
    if (arg){}
}

void thread2(void* arg)
{
    busy_wait();
    printf("thread2\n");
    if (arg){}
}

void thread1(void* arg)
{
    uthread_create(thread2, NULL);
    uthread_create(thread3, NULL);
    if (arg){}
}



int main(void)
{
    printf("Should print thread3 then thread2\n");
	uthread_run(true, thread1, NULL);
    printf("\n");
    printf("Should print thread2 then thread3\n");
    uthread_run(false, thread1, NULL);
	return 0;
}
