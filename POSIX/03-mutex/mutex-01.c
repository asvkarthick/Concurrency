#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[2];
int isDataAvailable = 0;
pthread_mutex_t lock;

void* Producer(void *arg)
{
    printf("%s entering...", __func__);
    fflush(stdout);

    while(1)
    {
        pthread_mutex_lock(&lock);
        printf("%s done\n", __func__);

        isDataAvailable++;
        printf("%s: data ready %d\n", __func__, isDataAvailable);

        pthread_mutex_unlock(&lock);
        sleep(1);
    }

    return NULL;
}

void* Consumer(void *arg)
{
    printf("%s entering...", __func__);
    fflush(stdout);
    while(1)
    {
        pthread_mutex_lock(&lock);
        printf("%s done\n", __func__);

        printf("%s: checking if data is available.\n", __func__);
        while(isDataAvailable > 0)
        {
            printf("%s: Data read %d\n", __func__, isDataAvailable);
            isDataAvailable--;
        }

        pthread_mutex_unlock(&lock);
        usleep(10000);
    }

    return NULL;
}

int main(void)
{
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    pthread_create(&tid[0], NULL, Producer, NULL);
    pthread_create(&tid[1], NULL, Consumer, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}
