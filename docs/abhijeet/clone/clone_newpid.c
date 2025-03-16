#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>

#define STACK_SIZE 1024*1024

int new_pid_process(){
    printf("Child pid : %d\n", getpid());
    return 0;
}

int main() {
    void *stack = malloc(STACK_SIZE);    

    if(!stack) {
	perror("Malloc Failed");
	exit(0);
    }

    int flags = CLONE_VM | CLONE_NEWPID;

    pid_t pid =  clone( &new_pid_process, (char *)stack + STACK_SIZE, flags, 0);
    
    if(pid < 0 ){
	perror("Clone Failed");
	exit(0);
    }

    printf("Parent pid : %d\n", getpid());
    printf("childs returned pid : %d\n", pid);

    sleep(1);      
    free(stack);

    return 0;
}

