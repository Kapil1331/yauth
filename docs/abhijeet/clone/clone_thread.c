#define _GNU_SOURCE 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>

#define STACK_SIZE 1024*1024

int print_ids(void *args){
    printf("Child pid : %d\n", getpid());
    printf("Child tid : %d\n", gettid());

    printf("\nprinting session ids\n");
    printf("child ppid : %d\n", getppid());
    printf("child pgid : %d\n", getpgid(0));
    printf("child sid  : %d\n", getsid(0));
    
    return 0;
}

int main() {
    void *stack = malloc(STACK_SIZE);    

    if(!stack) {
	perror("Malloc Failed");
	exit(0);
    }
    
    int flags = CLONE_THREAD | CLONE_SIGHAND | CLONE_VM;
    pid_t newpid =  clone( &print_ids, (char *)stack + STACK_SIZE, flags, 0);
    
    if(newpid < 0 ){
	perror("New Clone Failed");
	exit(0);
    }

    printf("Parent pid : %d\n", getpid());
    printf("Parent tid : %d\n", gettid());

    
    printf("\nprinting session ids\n");
    printf("parent ppid : %d\n", getppid());
    printf("parent pgid : %d\n", getpgid(0));
    printf("parent sid  : %d\n", getsid(0));
    

    sleep(3);      
    free(stack);

    return 0;
}

