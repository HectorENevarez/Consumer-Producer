#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include "semaphore.h"
#include "producer.h"
#include "consumer.h"
#include "io.h"

#define NUM_THREADS 4 // 4 threads; 2 producers 2 consumers

int main(int argc, char **argv){
    int Option;

    // used to record sleep times given by command line argument
    int ethel_s = 0; // ethel sleep
    int lucy_s = 0; // lucy sleep
    int crunchy_s = 0; // crunchy frog bites sleep
    int escargot_s = 0; // everlasting escargot suckers sleep

    while ((Option = getopt(argc, argv, "f:e:E:L:")) != -1) {
        switch (Option) {
            case 'f':
                crunchy_s = atoi(optarg);
                break;
            case 'e':
                escargot_s = atoi(optarg);
                break;
            case 'E':
                ethel_s = atoi(optarg);
                break;
            case 'L':
                lucy_s = atoi(optarg);
                break;             
        }
    }

    // shared semaphores used for accessing bounded buffer
    sem_t mutex;
    sem_t full;
    sem_t empty;

    // contains all candies currently on belt
    int bounded_buffer[MAX_CONVEYOR_BELT] = {NO_CANDY, NO_CANDY, NO_CANDY, // initializing belt with no candy
                                             NO_CANDY, NO_CANDY, NO_CANDY, 
                                             NO_CANDY, NO_CANDY, NO_CANDY, NO_CANDY};
    
    // used to track how many candies of each type are on the conveyor
    int candies_on_conveyor[NUMBER_OF_CANDIES] = {0};

    // tracks how many of each type of candy have been produced
    int candies_produced[NUMBER_OF_CANDIES] = {0};
    
    //tracks amount of candies consumed for every consumer
    int **candies_consumed = new int *[NUMBER_OF_CONSUMERS];
    candies_consumed[LUCY] = new int [NUMBER_OF_CANDIES]{0};
    candies_consumed[ETHEL] = new int [NUMBER_OF_CANDIES]{0};

    // initializing all sempaphores
    if (sem_init(&mutex, 0, 1) == -1){
        printf("unable to initialize mutex semaphore, report faulure");    
    }

    if (sem_init(&full, 0, 0) == -1){
        printf("unable to initialize full semaphore, report faulure");    
    }

    if (sem_init(&empty, 0, MAX_CONVEYOR_BELT) == -1){
        printf("unable to initialize empty semaphore, report faulure");    
    }

    pthread_barrier_t barrier; // used for stopping program
    pthread_barrier_init(&barrier, NULL, NUM_THREADS); 

    // initializing producers in proper order
    PRODUCER frog = {FrogBite, bounded_buffer, &mutex, &empty, &full, MAX_CRUNCHY_FROG_BITES, candies_on_conveyor, candies_produced, crunchy_s, &barrier};
    PRODUCER escargot = {Escargot, bounded_buffer, &mutex, &empty, &full, MAX_EVERLASTING_ESCARGOT_SUCKERS, candies_on_conveyor, candies_produced, escargot_s, &barrier};

    // initializing consumers in proper order
    CONSUMER lucy = {Lucy, bounded_buffer, &mutex, &empty, &full, candies_on_conveyor, candies_produced, candies_consumed[LUCY], lucy_s, &barrier};
    CONSUMER ethel = {Ethel, bounded_buffer, &mutex, &empty, &full, candies_on_conveyor, candies_produced, candies_consumed[ETHEL], ethel_s, &barrier};

    pthread_t pthread_id[NUM_THREADS]; // Initialize all thread ID's
    
    // begin crunchy frog bites production
    pthread_create( &pthread_id[0], NULL,
                    &begin_x_production, (void *)&frog);

    // begin everlasting escargot sucker production
    pthread_create( &pthread_id[1], NULL,
                    &begin_x_production, (void *)&escargot);                   
    // Lucy
    pthread_create( &pthread_id[1], NULL,
                    &begin_x_consumption, (void *)&lucy); 
    // Ethel
    pthread_create( &pthread_id[1], NULL,
                    &begin_x_consumption, (void *)&ethel); 

    pthread_barrier_wait(&barrier);
    io_production_report(candies_produced, candies_consumed); // final io print

    // free memory
    delete [] candies_consumed[LUCY];
    delete [] candies_consumed[ETHEL];
    delete [] candies_consumed;
}
