#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include "uthread.h"

#define STACK_SIZE 1024

int uthread_create(uthread *thread, void *(*func)(void *), void *arg){

	thread->stack = malloc(STACK_SIZE);
	void *stack_top = thread->stack + STACK_SIZE;

	thread->func = func;
	thread->arg = arg;

	pid_t tid = clone((int (*)(void *)) thread->func, stack_top, SIGCHLD, thread->arg);
	if(tid == -1){
		perror("clone failed");
		free(thread->stack);
		return -1;
	}

	thread->tid  = (int)tid;
	return 0;
}

int uthread_join(uthread thread){
	int wstatus;
	pid_t w = waitpid(thread.tid, &wstatus, 0);
	if(w == -1){
		perror("waitpid failed");
		return -1;
	}

	free(thread.stack);
	return wstatus;
}
