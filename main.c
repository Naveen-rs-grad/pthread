#include <stdio.h>
#include <pthread.h>

void *thread_function(void *arg)
{
    int id = *(int *)arg;

    printf("Hello from thread %d\n", id);

    return NULL;
}

int main(void)
{
    pthread_t threads[4];
    int ids[4] = {1, 2, 3, 4};

    // Create 4 threads
    for (int i = 0; i < 4; i++)
    {
        pthread_create(
            &threads[i],
            NULL,
            thread_function,
            &ids[i]
        );
    }

    // Wait for all threads to finish
    for (int i = 0; i < 4; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed!\n");

    return 0;
}