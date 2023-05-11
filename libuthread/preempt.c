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
sigset_t signal_set;			// 信号集
struct itimerval time_interval; // 时间间隔
struct sigaction signal_act;	// 信号处理

void sig_handler()
{
	// 到达时间间隔
	uthread_yield();
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	// 开启抢占
	sigset_t oldSet;
	sigprocmask(SIG_BLOCK, &signal_set, &oldSet);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	// 关闭抢占
	sigset_t oldSet;
	sigprocmask(SIG_UNBLOCK, &signal_set, &oldSet);
}

void preempt_start(bool preempt)
{
	if (preempt)
	{
		// 设置 100hz 每秒的间隔
		time_interval.it_interval.tv_sec = 0;
		time_interval.it_interval.tv_usec = SECOND / HZ;
		time_interval.it_value.tv_sec = 0;
		time_interval.it_value.tv_usec = SECOND / HZ;
		sigemptyset(&signal_set);		   // 初始化信号集
		sigaddset(&signal_set, SIGVTALRM); // 设置virtual alarm

		signal_act.sa_handler = &sig_handler; // 设置信号处理函数

		sigaction(SIGVTALRM, &signal_act, NULL); // 设置信号处理

		setitimer(ITIMER_VIRTUAL, &time_interval, NULL); // 设置定时器
	}
	/* TODO Phase 4 */
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
	// 取消设置的定时器，将时间间隔设置为 0
	time_interval.it_interval.tv_sec = 0;
	time_interval.it_interval.tv_usec = 0;
	time_interval.it_value.tv_sec = 0;
	time_interval.it_value.tv_usec = 0;
	setitimer(ITIMER_VIRTUAL, &time_interval, NULL); // 设置定时器
}
