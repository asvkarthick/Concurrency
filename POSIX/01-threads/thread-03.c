#include <stdio.h>
#include <pthread.h>

void thread_fn()
{
    printf("This is thread function.\n");
}

int main(void)
{
    pthread_t thr;
    int ret;

    ret = pthread_create(&thr, NULL, thread_fn, NULL);
    if(ret < 0)
    {
        fprintf(stderr, "Error: creating thread.\n");
        return -1;
    }
    printf("This is main thread.\n");
    pthread_detach(thr);
    return 0;
}
