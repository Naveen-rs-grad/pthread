#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//thread function takes void pointer as arg and returns an void pointer
//void pointer is generic - can point to anything
void *worker1(void *arg)
{
    while(1)
    {
        sleep(1);
        printf("\nHello from worker 1");
    }
    return NULL;
}

void *arg_worker(void *arg)
{
    int *iptr = (int *)arg;

    for(int i=0;i<5;i++)
    {
        sleep(1);
        printf("\nargument worker value: %d", *iptr);
        (*iptr)++;
    }

    return NULL;
}

void *arg_prod_value_worker(void *arg)
{
    int *iptr = (int*)malloc(sizeof(int));

    *iptr = 5;

    return iptr;
}

void worker2()
{
    while(1)
    {
        sleep(1);
        printf("Hello from worker 2 \n");
    }
    
}
