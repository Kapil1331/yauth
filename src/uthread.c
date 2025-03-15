#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include "uthread.h"

int uthread_create(uthread *thread, void *(*func)(void *), void *arg){

	thread->stack = malloc(1024);

	thread->func = func;
	thread->arg = arg;

	pid_t tid = clone((int (*)(void *)) thread->func, thread->stack + 1024, SIGCHLD, thread->arg);
	if(tid == -1){
		free(thread->stack);
		return -1;
	}

	thread->tid  = (int)tid;
	return 0;
}
