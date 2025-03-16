#include <stdio.h>
#include <pthread.h>
#include <assert.h>

void *child_thread(void *args){

    int *argptr = (int *)args;
    int arg = *argptr;

    printf("Argument is %d\n", arg);
    pthread_exit(NULL);
}

int main (){

    pthread_t child[10];

    for(int i = 1; i <= 10; i++){
	assert(pthread_create(&child[i], NULL, child_thread, &i) == 0);
    }

    return 0;
}

/*
 - prove simultaneous operations in threads
 - access at same time for some threads accordingly

*/
