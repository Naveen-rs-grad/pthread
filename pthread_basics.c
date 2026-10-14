#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "inc/pthread_worker.h"
#include "inc/pthread_mutex.h"


/* 
Example 1 - output

    Hello from worker 1
    argument worker value: 0
    Hello from worker 1
    argument worker value: 1
    Hello from worker 1
    argument worker value: 2
    Hello from worker 1
    argument worker value: 3
    Hello from worker 1
    argument worker value: 4
    Hello from worker 1
    Hello from worker 1
    Hello from worker 1

*/

void pthread_example1()
{
    pthread_t thread;
    pthread_t argthread;
    int arg_worker_data = 0;

    pthread_create(&thread, NULL, worker1, NULL);
    pthread_create(&argthread, NULL, arg_worker, (void*)&arg_worker_data);
    
    // worker2();

    pthread_join(thread, NULL); //This function waits for pthread to finish , without this no print is printed

    //thread join also allows the thread to return value
    pthread_join(thread, NULL);
}


/*
Example 1 - output

    the result produced by arg worker: 5
*/
void pthread_example2()
{
    pthread_t thread;
    int *result;

    pthread_create(&thread, NULL, arg_prod_value_worker, NULL);

    //thread join also allows the thread to return value
    pthread_join(thread, (void*)&result); //pointer to pointer requirred

    printf("the result produced by arg worker: %d\n", *result);
}

int main()
{
    // pthread_example1();

    // pthread_example2();

    pthreads_mutex_example();

    return 0;
}

// compilation : gcc -Wall -Wextra -g -pthread -Iinc *.c -o exe/pthread_basics