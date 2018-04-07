#include <stdio.h>
#include <pthread.h>
#include <string.h>

void* thread_findstringlen(void* str)
{
    printf("This is thread function.\n");
    return (void*) strlen((char*)str);
}

int main(void)
{
    pthread_t thr;
    int ret;

    ret = pthread_create(&thr, NULL, thread_findstringlen, (void*)"asvkarthick");
    if(ret < 0)
    {
        fprintf(stderr, "Error: creating thread.\n");
        return -1;
    }

    printf("This is main thread.\n");
    pthread_join(thr, (void**) &ret);
    printf("Return value from thread = %d\n", ret);

    return 0;
}
