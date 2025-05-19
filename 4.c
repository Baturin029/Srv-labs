#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>

#define SEC_NSEC 1000000000LL // 1 sekynda billion nanosekynd

void *long_thread(void *notused)
{
    printf("Etot potok vipolnaetsa bolee 10 sekynd \n");
    sleep(26); // Simulate a task that takes 26 seconds
    return NULL;
}

int main(void)
{
    uint64_t timeout;           // Timeout duration in nanoseconds
    struct sigevent event;      // Event structure for timeout notification
    int rval;                   // Return value for pthread_join
    pthread_t thread_id;        // Thread identifier

    printf("Program with timer.\n");

    // Initialize the event to unblock the thread on timeout
    event.sigev_notify = SIGEV_UNBLOCK;

    // Create a new thread to execute the long_thread function
    pthread_create(&thread_id, NULL, long_thread, NULL);

    // Set a timeout of 10 seconds
    timeout = 10LL * SEC_NSEC;

    // Set a timeout for joining the thread
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);
    rval = pthread_join(thread_id, NULL);

    // Check if the thread timed out after 10 seconds
    if (rval == ETIMEDOUT)
    {
        printf("10 seconds elapsed, thread %ld is still running!\n", thread_id);
    }

    // Wait for an additional 5 seconds
    sleep(5);

    // Set another timeout for joining the thread
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);
    rval = pthread_join(thread_id, NULL);

    // Check if the thread timed out after 25 seconds
    if (rval == ETIMEDOUT)
    {
        printf("Thread %ld is running for more than 25 seconds!\n", thread_id);
    }
    else
    {
        printf("Thread %ld finished as expected.\n", thread_id);
    }

    // Print author information
    printf("Baturin Artyom Andreevich i914B.\n");

    return 0; // Return 0 to indicate successful execution
}