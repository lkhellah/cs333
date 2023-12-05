/*Write a program that creates 4 threads using the PThreads API and joins with the threads
when done. The thread function should sleep for 5 seconds before exiting.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4

void *threadFunction(void *arg) {
    // Cast argument to avoid compiler warnings
    int threadNumber = *((int *)arg);

    // Sleep for 5 seconds
    sleep(5);

    printf("Thread %d is done sleeping.\n", threadNumber);

    // Exit the thread
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int threadArgs[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadArgs[i] = i;
        if (pthread_create(&threads[i], NULL, threadFunction, (void *)&threadArgs[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    printf("All threads have completed.\n");

    return 0;
}

