#include <stdio.h>
#include <unistd.h>
#include "include/uthread.h"

void *worker(void *arg) {
    int id = *(int *)arg;
    printf("Thread %d (PID: %d) (TID : %d)running\n", id, getpid(), gettid());
    fflush(stdout);
    return NULL;
}

int main() {
    uthread threads[3];
    int args[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++) {
        if (uthread_create(&threads[i], worker, &args[i]) == 0) {
            printf("Thread %d created with TID: %d\n", args[i], threads[i].tid);
        } else {
            printf("Failed to create thread %d\n", args[i]);
        }
    }

    for (int i = 0; i < 3; i++) {
        uthread_join(threads[i]);
        printf("Thread %d finished\n", args[i]);
    }

    return 0;
}

