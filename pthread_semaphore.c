#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>


sem_t sem_dma;
int sem_sensor_value[5];

void *dma_task(void *arg)
{
    (void)arg;

    printf("\nDMA data gathering...");
    sleep(2);
    for(int i=0;i<5;i++)
    {
        sem_sensor_value[i] = i*100;
    }
    printf("\nDMA gather done");
    sem_post(&sem_dma);

    return NULL;
}

void *processor_task(void *arg)
{
    (void)arg;

    printf("\nprocessor task - data not ready");

    sem_wait(&sem_dma);
    printf("\nprocessor task - data ready\n");

    for(int i=0;i<5;i++) printf("%d ", sem_sensor_value[i]);

    printf("\n");

    return NULL;

}


void semaphore_example()
{
    pthread_t thread1, thread2;

    sem_init(&sem_dma,0,0); //intial semaphore value 0

    pthread_create(&thread1,NULL, dma_task, NULL);
    pthread_create(&thread2,NULL, processor_task, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

}