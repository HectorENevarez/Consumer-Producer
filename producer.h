#ifndef PRODUCER_H
#define PRODUCER_H

#include <unistd.h>
#include "semaphore.h"

#include "io.h"

// defines how many different types of candies there are being produced
#define NUMBER_OF_CANDIES                   2

// Different types of producer values
#define NO_CANDY                            -1
#define CRUNCHY_FROG_BITES                  0
#define ESCARGOT_SUCKERS                    1

// Max values
#define MAX_CRUNCHY_FROG_BITES              3
#define MAX_EVERLASTING_ESCARGOT_SUCKERS    10
#define MAX_PRODUCED_CANDIES                100
#define MAX_CONVEYOR_BELT                   10

//conversion factor for command line argument
#define MS_CONVERSION                       1000  

typedef struct{
    ProductType producer_type;      // contains enumerated value of producer
    int *bounded_buffer;            // Bounder buffer shares data with all other producers and consumers for accessing
    sem_t *mutex;                   // mutex used to lock for access to bounding buffer 
    sem_t *empty;                   // available slots
    sem_t *full;                    // empty slots
    int max_on_conveyor;            // Max amount of type of candy on conveyor belt at once
    int *candies_on_conv;           // Array pointer that tracks how many of each candy are on the conveyor
    int *candies_prod;              // Array pointer that tracks how many of each candy have been produced
    int sleep_time;                 // Amount of time to sleep after candy has been consumed
    pthread_barrier_t *barrier;     // barrier makes sure exitting of thread is successful
}PRODUCER;

/*
* This thread will begin the production of a specific type of candy for a consumer to eat. 
* It will continue until the max amount of candies are produced
*
* void * p:
* expects a PRODUCER struct
*/
void * begin_x_production(void * p);

#endif // PRODUCER_H