#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define NUM_SAMPLES 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

/* Variables shared*/
int finished = 0;
int data_ready = 0;
float sensor_value = 0.0;

void* producer_task(void *arg)
{
    (void)arg;

    for(int i=0;i<NUM_SAMPLES;i++)
    {
        sleep(1);
        pthread_mutex_lock(&mutex);

        // Wait until previous sample is consumed.
        while (data_ready)
        {
            pthread_cond_wait(&condition, &mutex);
        }

        sensor_value = i + 0.5;
        data_ready = 1;

        printf("\nConsumer data ready - sample : %d", i);

        pthread_cond_signal(&condition); //signal wake one thread
        
        pthread_mutex_unlock(&mutex);
    }

    /* Acquires the crictal section to signal producer has finsihed producing*/
    pthread_mutex_lock(&mutex);
    finished = 1;
    pthread_cond_broadcast(&condition); // broadcast signals all the waiting thread
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* consumer_task(void *arg)
{
    (void)arg;

    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(!data_ready && !finished)
        {
            /*
            The pthread_cond_wait does 3 things:
                1. Checks for the condition is signalled or not
                2. If not release the mutex immediately
                3. The thread will perform a blocked wait in this statment until condition signalled 
            */
            pthread_cond_wait(&condition, &mutex);
        }

        if(!data_ready && finished)
        {
            pthread_mutex_unlock(&mutex);
            printf("\nProducer task finisihed");
            break;
        }

        float sample = sensor_value;
        data_ready = 0;

        // Wake producer if it is waiting for space.
        pthread_cond_signal(&condition);

        pthread_mutex_unlock(&mutex);

        printf("\nProcessor: Processed %.1f\n", sample);

        sleep(1);
    }

    return NULL;
}

/*
Example output

    Consumer data ready - sample : 0
    Processor: Processed 0.5

    Consumer data ready - sample : 1
    Processor: Processed 1.5

    Consumer data ready - sample : 2
    Processor: Processed 2.5

    Consumer data ready - sample : 3
    Processor: Processed 3.5

    Consumer data ready - sample : 4
    Processor: Processed 4.5
*/

void pthread_producer_consumer()
{
    pthread_t producer_thread, consumer_thread;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&producer_thread, NULL, producer_task, NULL);
    pthread_create(&consumer_thread, NULL, consumer_task, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&mutex);
}
