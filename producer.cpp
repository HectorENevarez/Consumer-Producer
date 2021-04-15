#include <iostream>

#include "producer.h"
#include "io.h"

// begins production for x produer
void *begin_x_production(void * p){
    PRODUCER *prod = (PRODUCER *)p;
    int total_candies_produced; // total candies produced between all producers
    int type = prod->producer_type; // producer enumeration value
    
    // derive total candies produced
    total_candies_produced = prod->candies_prod[CRUNCHY_FROG_BITES] + prod->candies_prod[ESCARGOT_SUCKERS];
    while(total_candies_produced < MAX_PRODUCED_CANDIES){        
        if (prod->candies_on_conv[type] < prod->max_on_conveyor){ // make sure max amount of candies aren't on belt
            sem_wait(prod->empty); // wait for a slot on the conveyor belt
            sem_wait(prod->mutex); //lock conveyor belt

            for(int idx = 0; idx < MAX_CONVEYOR_BELT; idx++){
                if (prod->bounded_buffer[idx] == NO_CANDY){ // find where there is no candy
                    prod->bounded_buffer[idx] = prod->producer_type; // add candy type to conveyor
                    break; // break to not modify belt anymore
                }
            }
            // increment values for tracking production
            prod->candies_on_conv[type] += 1;
            prod->candies_prod[type] += 1;

            //io output inside lock to ensure no values have changed
            io_add_type(prod->producer_type, prod->candies_on_conv, prod->candies_prod);

            sem_post(prod->mutex); // free lock
            sem_post(prod->full); // post full
        }
        usleep(prod->sleep_time * MS_CONVERSION); // sleep based on miliseconds given in command line
        total_candies_produced = prod->candies_prod[CRUNCHY_FROG_BITES] + prod->candies_prod[ESCARGOT_SUCKERS]; // derive total amount of candies consumed
    }
    pthread_barrier_wait(prod->barrier);
    return 0; // removes compiler warning flag
}