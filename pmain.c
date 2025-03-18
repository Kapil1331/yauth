#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *worker(void *arg) {
    int id = *(int *)arg;
    printf("Thread %d (PID: %d) (K-TID: %d) (U-TID: %lu)running\n", id, getpid(), gettid(), pthread_self());
    fflush(stdout);
    return NULL;
}

int main() {
    pthread_t threads[3];
    int args[3] = {1, 2, 3};

    // Create threads
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, worker, &args[i]) == 0) {
            printf("Thread %d created with TID: %lu\n", args[i], threads[i]);
        } else {
            printf("Failed to create thread %d\n", args[i]);
        }
    }

    // Join threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d finished\n", args[i]);
    }

    return 0;
}

