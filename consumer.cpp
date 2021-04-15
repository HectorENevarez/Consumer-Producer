#include <pthread.h>
#include <iostream>

#include "consumer.h"
#include "producer.h"

// begins consumption for x consumer
void * begin_x_consumption(void * c){
    CONSUMER *cons = (CONSUMER *)c;
    int total_candies_produced; // total candies produced between all produces
    int candy_consumed; // stores candy consumed from belt
    
    // create array of products to make io output compatible
    ProductType products[NUMBER_OF_CANDIES] = {FrogBite, Escargot};

    // derive total candies produced
    total_candies_produced = cons->candies_prod[CRUNCHY_FROG_BITES] + cons->candies_prod[ESCARGOT_SUCKERS];
    while(total_candies_produced < MAX_PRODUCED_CANDIES || cons->candies_on_conv[CRUNCHY_FROG_BITES] != 0 || cons->candies_on_conv[ESCARGOT_SUCKERS] != 0){
        sem_wait(cons->full); // wait to make sure there is candy on the conveyor
        sem_wait(cons->mutex); // lock bounded buffer

        candy_consumed = cons->bounded_buffer[0]; // grabs first item from buffer
        // move buffer down one to make sure queue is ready
        for(int idx = 1; idx < MAX_CONVEYOR_BELT; idx++){
            cons->bounded_buffer[idx - 1] = cons->bounded_buffer[idx]; // shift every candy down an index
        }
        cons->bounded_buffer[MAX_CONVEYOR_BELT - 1] = NO_CANDY; // create empty slot at last index of belt since candy has been consumed

        cons->candies_on_conv[candy_consumed] -= 1; // decrement specific candy value from conveyor candy count
        cons->candies_cons[candy_consumed] += 1; // increment candy consumed by consumer

        // io output inside lock to make sure no values have changed
        io_remove_type(cons->consumer, products[candy_consumed], cons->candies_on_conv, cons->candies_cons);

        sem_post(cons->empty); // post empty
        sem_post(cons->mutex); // post lock

        usleep(cons->sleep_time * MS_CONVERSION); // sleep based on miliseconds given in command line
        total_candies_produced = cons->candies_prod[CRUNCHY_FROG_BITES] + cons->candies_prod[ESCARGOT_SUCKERS]; // derive total amount of candies consumed
    }
    pthread_barrier_wait(cons->barrier);
    return 0; // removes compiler warning flag
}