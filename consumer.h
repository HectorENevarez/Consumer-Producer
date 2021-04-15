#ifndef CONSUMER_H
#define CONSUMER_H

#include <unistd.h> //usleep
#include "semaphore.h"

#include "io.h"

// constant value defining number of consumers
#define NUMBER_OF_CONSUMERS 2

// enumerating consumers
#define LUCY    0
#define ETHEL   1

typedef struct{
    ConsumerType consumer;          // contains enumerated value of consumer
    int *bounded_buffer;            // Bounder buffer shares data with all other producers and consumers for accessing
    sem_t *mutex;                   // mutex used to lock for access to bounding buffer
    sem_t *empty;                   // available slots
    sem_t *full;                    // empty slots
    int *candies_on_conv;           // Array pointer that tracks how many of each candy are on the conveyor 
    int *candies_prod;              // Array pointer that tracks how many of each candy have been produced
    int *candies_cons;              // Array pointer that tracks how many of each candy have been consumed by a specific consumer
    int sleep_time;                 // Amount of time to sleep after candy has been consumed
    pthread_barrier_t *barrier;     // barrier makes sure exitting of thread is successful
}CONSUMER;

/*
* This thread is used by the consumers in order to consume the candy and captures critical metadata used for outputing results. It will consume candy
* off the belt until the max amount of candyon the belt is reached. It well then return.
*
* void * c:
* expects a CONSUMER struct
*/
void * begin_x_consumption(void * c);

#endif // CONSUMER_H