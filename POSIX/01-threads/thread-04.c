#include <stdio.h>
#include <pthread.h>

void thread_fn()
{
    printf("This is thread function.\n");
}

int main(void)
{
    pthread_t thr;
    pthread_attr_t attr;
    int ret;

    // Use pthread_attr_t to detach the thread rather than doing with pthread_detach
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Create thread with detach state attributes
    ret = pthread_create(&thr, &attr, thread_fn, NULL);
    if(ret < 0)
    {
        fprintf(stderr, "Error: creating thread.\n");
        return -1;
    }

    pthread_attr_destroy(&attr);

    printf("This is main thread.\n");
    sleep(1);
    return 0;
}
