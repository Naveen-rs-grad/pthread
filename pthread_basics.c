#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *worker1(void *arg)
{
    while(1)
    {
        sleep(1);
        printf("Hello from worker 1 \n");
    }
    return NULL;
}

void worker2()
{
    while(1)
    {
        sleep(1);
        printf("Hello from worker 2 \n");
    }
    
}


int main()
{
    pthread_t thread;

    pthread_create(&thread, NULL, worker1, NULL);
    
    // worker2();

    pthread_join(thread, NULL); //This function waits for pthread to finish , without this no print is printed

    return 0;
}