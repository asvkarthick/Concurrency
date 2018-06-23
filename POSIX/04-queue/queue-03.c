#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

struct Queue {
        int buffer_index;
        struct Queue* next;
};

struct Queue* front_producer = NULL;
struct Queue* rear_producer = NULL;
struct Queue* front_consumer = NULL;
struct Queue* rear_consumer = NULL;
bool running = false;
int num_buffers;

pthread_t producer_t;
pthread_t consumer_t;
pthread_mutex_t producer_lock;
pthread_mutex_t consumer_lock;
pthread_cond_t producer_cond;
pthread_cond_t consumer_cond;

void enqueue_producer(int buffer_index)
{
        struct Queue* temp = (struct Queue*) malloc(sizeof(struct Queue));

        temp->buffer_index = buffer_index;
        temp->next = NULL;

        pthread_mutex_lock(&producer_lock);
        if(front_producer == NULL && rear_producer == NULL) {
                front_producer = rear_producer = temp;
                pthread_mutex_unlock(&producer_lock);
                pthread_cond_signal(&producer_cond);
                return;
        }

        rear_producer->next = temp;
        rear_producer = temp;
        pthread_mutex_unlock(&producer_lock);
        pthread_cond_signal(&producer_cond);
}

void enqueue_consumer(int buffer_index)
{
        struct Queue* temp = (struct Queue*) malloc(sizeof(struct Queue));

        temp->buffer_index = buffer_index;
        temp->next = NULL;

        pthread_mutex_lock(&consumer_lock);
        if(front_consumer == NULL && rear_consumer == NULL) {
                front_consumer = rear_consumer = temp;
                pthread_mutex_unlock(&consumer_lock);
                pthread_cond_signal(&consumer_cond);
                return;
        }

        rear_consumer->next = temp;
        rear_consumer = temp;
        pthread_mutex_unlock(&consumer_lock);
        pthread_cond_signal(&consumer_cond);
}

void dequeue_buffer_producer()
{
        struct Queue* temp = front_producer;

        if(front_producer == NULL) {
                printf("Producer Queue is Empty\n");
                return;
        }

        if(front_producer == rear_producer)
                front_producer = rear_producer = NULL;
        else
                front_producer = front_producer->next;

        free(temp);
}

void dequeue_buffer_consumer()
{
        struct Queue* temp = front_consumer;

        if(front_consumer == NULL) {
                printf("Consumer Queue is Empty\n");
                return;
        }

        if(front_consumer == rear_consumer)
                front_consumer = rear_consumer = NULL;
        else
                front_consumer = front_consumer->next;

        free(temp);
}

int front_element_producer()
{
        if(front_producer == NULL)
                return -1;

        return front_producer->buffer_index;
}

int front_element_consumer()
{
        if(front_consumer == NULL)
                return -1;

        return front_consumer->buffer_index;
}

int dequeue_producer()
{
        int buffer_index = front_element_producer();

        pthread_mutex_lock(&producer_lock);

        while(buffer_index == -1 && running == true) {
            pthread_cond_wait(&producer_cond, &producer_lock);
            buffer_index = front_element_producer();
        }

        if(buffer_index != -1)
                dequeue_buffer_producer();
        pthread_mutex_unlock(&producer_lock);

        return buffer_index;
}

int dequeue_consumer()
{
        int buffer_index = front_element_consumer();

        pthread_mutex_lock(&consumer_lock);

        while(buffer_index == -1 && running == true) {
                pthread_cond_wait(&consumer_cond, &consumer_lock);
                buffer_index = front_element_consumer();
        }

        if(buffer_index != -1)
                dequeue_buffer_consumer();
        pthread_mutex_unlock(&consumer_lock);

        return buffer_index;
}

void *producer_thread()
{
        int counter = 0;
        int i;

        while(running) {
                i = dequeue_producer();
                if(counter % num_buffers != i) {
                    printf("Producer counter issue - exp: %d, recd = %d\n", counter % num_buffers, i);
                    while(running);
                }
                ++counter;
                enqueue_consumer(i);
                usleep(100);

                if(counter % 100 == 0) printf("Producer: %d\n", counter);
        }

        return NULL;
}

void *consumer_thread()
{
        int counter = 0;
        int i;

        while(running) {
                i = dequeue_consumer();
                if(counter % num_buffers != i) {
                    printf("Consumer counter issue - exp: %d, recd = %d\n", counter % num_buffers, i);
                    while(running);
                }
                ++counter;
                enqueue_producer(i);
                usleep(100);

                if(counter % 100 == 0) printf("Consumer: %d\n", counter);
        }

        return NULL;
}

int main(int argc, char *argv[])
{
        int i;

        if(argc < 2)
                num_buffers = 8;
        else
                num_buffers = atoi(argv[1]);

        // Initialize condition variables
        pthread_cond_init(&producer_cond, NULL);
        pthread_cond_init(&consumer_cond, NULL);

        // Initialize mutex
        if(pthread_mutex_init(&producer_lock, NULL) != 0) {
                printf("Producer mutex failed to create.\n");
                return -1;
        }
        if(pthread_mutex_init(&consumer_lock, NULL) != 0) {
                printf("Consumer mutex failed to create.\n");
                return -1;
        }

        printf("Main queueing buffers to producer queue.\n");

        for(i = 0; i < num_buffers; i++)
                enqueue_producer(i);

        printf("Main creating producer and consumer thread.\n");

        pthread_create(&producer_t, NULL, producer_thread, NULL);
        pthread_create(&consumer_t, NULL, consumer_thread, NULL);

        running = true;
        getchar();
        running = false;

        pthread_join(producer_t, NULL);
        pthread_join(consumer_t, NULL);
        pthread_mutex_destroy(&producer_lock);
        pthread_mutex_destroy(&consumer_lock);
        pthread_cond_destroy(&producer_cond);
        pthread_cond_destroy(&consumer_cond);

        return 0;
}
