#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_cond_t cv_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv_consumer = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx_producer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_consumer = PTHREAD_MUTEX_INITIALIZER;

#define NUM_BUFFERS 8
int num_free_buffers = NUM_BUFFERS;
int num_full_buffers = 0;
int producer_done = 0;
int buffer[NUM_BUFFERS];
int producer_buffer_id = 0;
int consumer_buffer_id = 0;

void produce_buffer(int i)
{
    buffer[producer_buffer_id] = i;
    producer_buffer_id = (producer_buffer_id + 1) % NUM_BUFFERS;
}

void consume_buffer(int *i)
{
    *i = buffer[consumer_buffer_id];
    consumer_buffer_id = (consumer_buffer_id + 1) % NUM_BUFFERS;
}

void *producer(void *arg)
{
    int n = *((int*) arg);
    int i;

    for(i = 1; i <= n; i++)
    {
        pthread_mutex_lock(&mtx_producer);
        while(num_free_buffers <= 0)
            pthread_cond_wait(&cv_producer, &mtx_producer);
        --num_free_buffers;
        pthread_mutex_unlock(&mtx_producer);

        produce_buffer(i * i);

        pthread_mutex_lock(&mtx_consumer);
        ++num_full_buffers;
        pthread_cond_signal(&cv_consumer);
        pthread_mutex_unlock(&mtx_consumer);
    }

    pthread_mutex_lock(&mtx_consumer);
    producer_done = 1;
    pthread_cond_broadcast(&cv_consumer);
    pthread_mutex_unlock(&mtx_consumer);

    return NULL;
}

void *consumer(void *arg)
{
    int sum = 0;
    int item;

    while(1)
    {
        pthread_mutex_lock(&mtx_consumer);
        while((num_full_buffers <= 0) && !producer_done)
            pthread_cond_wait(&cv_consumer, &mtx_consumer);
        if(num_full_buffers <= 0 && producer_done)
        {
            pthread_mutex_unlock(&mtx_consumer);
            *((int*) arg) = sum;
            break;
        }
        --num_full_buffers;
        pthread_mutex_unlock(&mtx_consumer);

        consume_buffer(&item);
        sum += item;

        pthread_mutex_lock(&mtx_producer);
        ++num_free_buffers;
        pthread_cond_signal(&cv_producer);
        pthread_mutex_unlock(&mtx_producer);
    }

    return NULL;
}

int main(void)
{
    pthread_t thread_producer;
    pthread_t thread_consumer;
    int i, n, sum = 0, total = 0;

    printf("Enter the number of items : ");
    scanf("%d", &n);

    for(i = 1; i <=n ; i++)
        total += i * i;
    printf("Total = %d\n", total);

    pthread_create(&thread_producer, NULL, producer, (void*) &n);
    pthread_create(&thread_consumer, NULL, consumer, (void*) &sum);

    printf("Producer and consumer threads created.\n");
    pthread_join(thread_producer, NULL);
    printf("Producer completed producing buffers\n");
    pthread_join(thread_consumer, NULL);
    printf("Consumer completed consuming buffers\n");

    printf("Sum = %d\n", sum);

    return 0;
}
