#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>

#define STACK_SIZE 1024*1024

int goto_stack_limit(){
    printf("Child pid : %d\n", getpid());

    void *alloc_space = malloc(STACK_SIZE);

    if(!alloc_space){
	perror("new stack failed!");
    }
    else {
	printf("malloc in child successful. why ?\n");
    }

    free(alloc_space);
    return 0;
}

int main() {
    void *stack = malloc(STACK_SIZE);    

    if(!stack) {
	perror("Malloc Failed");
	exit(0);
    }

    pid_t pid =  clone( &goto_stack_limit, (char *)stack + STACK_SIZE, CLONE_VM, 0);
    
    if(pid < 0 ){
	perror("Clone Failed");
	exit(0);
    }

    printf("Parent pid : %d\n", getpid());

    sleep(1);      
    free(stack);

    return 0;
}

