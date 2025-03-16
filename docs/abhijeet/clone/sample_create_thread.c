#define _GNU_SOURCE
#include <sched.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>


// 1 mb stack size
#define STACK_SIZE 1024*1024


int thread_wrapper(void *arg) {
    void (*thread_function)(void *) = ((void **)arg)[0];
    void *actual_arg = ((void **)arg)[1];

    free(arg); 
    thread_function(actual_arg);

    return 0;  
}

pid_t create_thread(void (*function)(void *), void *arg) {

    void *stack = malloc(STACK_SIZE);
    if (!stack) {
        perror("Failed to allocate stack");
        return -1;
    }

    void **args = malloc(2 * sizeof(void *));
    if (!args) {
        perror("Failed to allocate memory for arguments");
        free(stack);
        return -1;
    }

    args[0] = function; 
    args[1] = arg;  

    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;

    pid_t tid = clone(thread_wrapper, (char *)stack + STACK_SIZE, flags, args);

    if (tid == -1) {
        perror("clone failed");
        free(stack);
        free(args);
        return -1;
    }

    return tid; 
}

void my_thread_function(void *arg) {
    int num = *((int *)arg);
    for (int i = 0; i < 5; i++) {
        printf("Thread running: %d. value = %d, thread PID = %d\n", i, num, getpid());
    }
}

int main() {
    int value = 42;

    pid_t tid = create_thread(my_thread_function, &value);
    if (tid == -1) {
        printf("Thread creation failed!\n");
        return 1;
    }

    printf("Main process: created thread with ID = %d\n", tid);

    waitpid(tid, NULL, __WCLONE);

    return 0;
}
