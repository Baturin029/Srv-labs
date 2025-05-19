#include <stdio.h>
#include <time.h>
#include <sync.h>
#include <pthread.h>
#include <sys/neutrino.h>

// Barrier for thread synchronization
barrier_t barrier;

// Thread function for thread1
void *thread1(void *not_used) {
    char buf[27];
    time_t now;

    // Log the start time of thread1
    time(&now);
    printf("Thread 1: Start time %s", ctime_r(&now, buf));
    sleep(3); // Simulate some work

    // Wait at the barrier
    barrier_wait(&barrier);

    // Log the time after passing the barrier
    time(&now);
    printf("Thread 1: Passed the barrier at %s", ctime_r(&now, buf));
    return NULL;
}

// Thread function for thread2
void *thread2(void *not_used) {
    char buf[27];
    time_t now;

    // Log the start time of thread2
    time(&now);
    printf("Thread 2: Start time %s", ctime_r(&now, buf));
    sleep(6); // Simulate some work

    // Wait at the barrier
    barrier_wait(&barrier);

    // Log the time after passing the barrier
    time(&now);
    printf("Thread 2: Passed the barrier at %s", ctime_r(&now, buf));
    return NULL;
}

int main() {
    printf("Baturin Artyom Andreevich i914B\n");
    char buf[27];
    time_t now;

    // Initialize the barrier for 3 threads (main thread + 2 worker threads)
    barrier_init(&barrier, NULL, 3);

    // Create thread1 and thread2
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    // Main thread waits at the barrier
    barrier_wait(&barrier);

    // Log the time after passing the barrier in the main thread
    time(&now);
    printf("Main(): Passed the barrier at %s", ctime_r(&now, buf));

    // Wait for threads to complete
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Destroy the barrier
    barrier_destroy(&barrier);

    return 0;
}