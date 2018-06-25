// Program to measure context switch time

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>

bool running = false;

void *context_switch_profile_thread(void *arg)
{
    struct timeval start, end, diff;
    int sleep_time = *((int*) arg);

    gettimeofday(&start, NULL);
    while(running) {

        usleep(sleep_time);

    }
    gettimeofday(&end, NULL);

    timersub(&end, &start, &diff);
    printf("Thread execution time = %ld,%ld useconds\n", diff.tv_sec, diff.tv_usec);

    return NULL;
}

int main(void)
{
    int sleep_time = 100000; // 100 ms
    int sleep_main = 5; // 5 s
    pthread_t thr;
    struct timeval start, end, diff;

    running = true;
    gettimeofday(&start, NULL);
    pthread_create(&thr, NULL, context_switch_profile_thread, (void*) &sleep_time);

    sleep(sleep_main);

    running = false;
    pthread_join(thr, NULL);
    gettimeofday(&end, NULL);
    timersub(&end, &start, &diff);
    printf("Main execution time = %ld,%ld useconds\n", diff.tv_sec, diff.tv_usec);

    return 0;
}
