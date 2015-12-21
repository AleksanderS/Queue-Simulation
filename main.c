/* 
 * File:   main.c
 * Author: Alex
 *
 * Created on September 25, 2015, 2:17 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
#include "Heap.h"
#include "FileIO.h"

int main(int argc, char** argv) {

    
   
    
    
    struct Heap * myHeap = malloc(sizeof(struct Heap));
    struct parameters *params = malloc(sizeof(struct parameters));
    getParameters(params);
    constructHeap(myHeap, params);
  
    
    while(myHeap->size > 0){
        
        processNextEvent(myHeap, params);
        if(myHeap->custLeftToServiceAfterInitial100 > 0 && myHeap->size <= params->M + 1){
            if(myHeap->custLeftToServiceAfterInitial100 > SIZE - (params->M + 1)){
                int i;
                for(i = 0; i < 50; i++){
                    addNewArrival(myHeap, params);
                    
                    myHeap->custLeftToServiceAfterInitial100--;
                }
            }
            else{
                int i;
                int j = myHeap->custLeftToServiceAfterInitial100;
                for(i = 0 ; i < j; i++){
                    addNewArrival(myHeap, params);
                    myHeap->custLeftToServiceAfterInitial100--;
                }
            }
            
         }
    }
    
    
   
    fprintf(stderr, "Po  = %lf\n", generatePercentIdleTime(params->M, params->lambda, params->mu));
    fprintf(stderr, "L   = %lf\n", getAverageNumberInSystem(params->M, params->lambda, params->mu));
    fprintf(stderr, "W   = %lf\n", getAverageTimeInSystem(params->M, params->lambda, params->mu));
    fprintf(stderr, "Lq  = %lf\n", getAverageNumberOfCustomers(params->M, params->lambda, params->mu));
    fprintf(stderr, "Wq  = %lf\n", getAverageWaitTime(params->M, params->lambda, params->mu));
    fprintf(stderr, "rho = %lf\n\n", utilizationFactor(params->M, params->lambda, params->mu));
    
    
    
    fprintf(stderr, "Actual Po  = %lf\n", myHeap->timeIdle/myHeap->totalSimulationTime);
    fprintf(stderr, "Actual W   = %lf\n", myHeap->totalTimeSpentInSystem/myHeap->totalNumberOfEvents);
    fprintf(stderr, "Actual Wq  = %lf\n", myHeap->timeSpentWaitingInQueue / myHeap->totalNumberOfEvents);
    fprintf(stderr, "Actual rho = %lf\n", myHeap->totalServiceTime/(params->M * myHeap->totalSimulationTime));
    free(myHeap);
     
    return 0;
}

