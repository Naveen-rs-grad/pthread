#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


pthread_mutex_t i2c_mutex;

void* temperature_task(void* arg)
{
    (void)arg;

    int rc = pthread_mutex_lock(&i2c_mutex); //blocking mutex lock - the calling thread waits for the lock to be released 

    if (rc != 0) {
        fprintf(stderr, "lock: %s\n", strerror(rc));
        return NULL;
    }

    printf("TEMP: I2C bus acquired\n");
    printf("TEMP: Reading sensor 0x48\n");

    sleep(2);  // Simulated slow transaction

    printf("TEMP: Read completed\n");

    pthread_mutex_unlock(&i2c_mutex);

    return NULL;
}

void* imu_task(void* arg)
{
    (void)arg;

    int rc = pthread_mutex_trylock(&i2c_mutex); //Non - blocking mutex lock - returns busy if lock is owned by another task , retry to check again

    if (rc == EBUSY) //thread check if the lock is acquirred by another thread
    {
        printf("IMU: Bus busy, trying timed lock\n");

        struct timespec deadline; //here the deadline is absoluet and not relative
        clock_gettime(CLOCK_REALTIME, &deadline);
        deadline.tv_sec += 3; //Try decreasing the timeout to check for timeout case

        rc = pthread_mutex_timedlock(
            &i2c_mutex,
            &deadline
        ); //It waits for the mutex only until the specified absolute deadline.
    }

    if (rc == 0)
    {
        printf("IMU: I2C bus acquired\n");
        printf("IMU: Reading sensor 0x68\n");
        printf("IMU: Read completed\n");

        pthread_mutex_unlock(&i2c_mutex);
    }
    else if (rc == ETIMEDOUT)
    {
        printf("IMU: Timed out waiting for bus\n");
    }
    else
    {
        fprintf(stderr, "IMU lock: %s\n", strerror(rc));
    }

    return NULL;
}

/*
Example output
    TEMP: I2C bus acquired
    TEMP: Reading sensor 0x48
    IMU: Bus busy, trying timed lock
    TEMP: Read completed
    IMU: I2C bus acquired
    IMU: Reading sensor 0x68
    IMU: Read completed
*/

void pthreads_mutex_example()
{
    pthread_t pthread1, pthread2;

    pthread_mutexattr_t attr;
    

    pthread_mutexattr_init(&attr);

    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);


    pthread_mutex_init(&i2c_mutex, &attr);
    pthread_mutexattr_destroy(&attr);


    pthread_create(&pthread1, NULL, temperature_task, NULL);

    sleep(1); // Give temperature task time to acquire bus

    pthread_create(&pthread2, NULL, imu_task, NULL);

    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);

    pthread_mutex_destroy(&i2c_mutex);

}