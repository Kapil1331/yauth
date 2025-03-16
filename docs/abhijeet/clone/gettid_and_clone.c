#define _GNU_SOURCE 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>

#define STACK_SIZE 1024*1024

int print_gettid(){
    printf("Child pid : %d\n", getpid());
    printf("Child tid : %d\n", gettid());
    
    return 0;
}

int main() {
    void *stack = malloc(STACK_SIZE);    

    if(!stack) {
	perror("Malloc Failed");
	exit(0);
    }
    
    int flags = CLONE_VM;
    pid_t pid =  clone( &print_gettid, (char *)stack + STACK_SIZE, flags, 0);
    
    if(pid < 0 ){
	perror("Clone Failed");
	exit(0);
    }

    printf("Parent pid : %d\n", getpid());
    printf("Parent tid : %d\n", gettid());

    sleep(1);      
    free(stack);

    return 0;
}

