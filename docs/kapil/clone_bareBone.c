#define _GNU_SOURCE		// for clone
#include <stdio.h>
#include <fcntl.h>		// for open
#include <unistd.h>     // getpid
#include <stdlib.h>		// for malloc
#include <sched.h>		// for clone
#include <signal.h>     // for SIGCHLD
#include <err.h>		// for errno
#include <sys/wait.h>

int cloneFunc(void *arg){
	printf("C:the pid of child is : %d\n", getpid());
	register void *sp asm ("sp");
	printf("C:%p\n", sp);
	return 10;
}

int main(int argc, char *argv[]){

	int	fd = open("openmp.c", O_RDONLY);
	char *stack;
	char *stackTop;
	
	printf("P:fd of open file : %d\n", fd);
	printf("P:The parent's pid is: %d\n", getpid());
	
	stack = malloc(1024);
	stackTop = stack + 1024;
	printf("P:The addr of stackTop : %p\n", (void *)stackTop);
/*
 * If no signal (i.e., zero) is specified, then the parent process is 
 * not signaled when the child terminates. but then __WALL argument should 
 * be provided in the wait() / waitpid()
 */

/*
 * The clone fails if we pass a null stack
 * maybe bcz calling convention requires it?
 */
	int pid = clone(cloneFunc, stackTop, 0, NULL);
	if(pid == -1){
		printf("P:clone failed");
	}else{
		printf("P:Clone has been created its pid is : %d\n", pid);
		if(waitpid(pid, NULL, __WALL) == -1){
			err(EXIT_FAILURE, "waitpid");
			printf("waitpid() failed");
		}
		printf("P:Cloned child has terminated\n");
	}
	return 0; 
}
