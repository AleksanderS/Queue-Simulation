
#include "Queue.h"
#include "Heap.h"
#include "FileIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void constructHeap(struct Heap * myHeap, struct parameters *params){
    int i;
    
    myHeap->totalTimeSpentInSystem = 0;
    myHeap->custLeftToServiceAfterInitial100 = params->numberOfArrivalsToSimulate - 100;
    myHeap->custWhoHadToWait = 0;
    myHeap->timeIdle = 0;
    myHeap->totalServiceTime = 0;
    myHeap->myQueue = createQueue();
    myHeap->serversTotal = params->M;
    myHeap->serversAvailable = params->M;
    myHeap->size = 0;
    myHeap->numberOfEventsRemaining = params->numberOfArrivalsToSimulate;
    myHeap->totalNumberOfEvents = params->numberOfArrivalsToSimulate;
   
    myHeap->customers[1] = *(newCustomer());
    myHeap->customers[1].ArrivalTime = 0;
    myHeap->customers[1].PQTime = 0;
    for (i = 2; i < SIZE; i++){
        myHeap->customers[i] = *(newCustomer());
        
        myHeap->customers[i].ArrivalTime = myHeap->customers[i-1].ArrivalTime + getRandInterval(params->lambda);
        myHeap->customers[i].PQTime = myHeap->customers[i].ArrivalTime;
        
        
        
        myHeap->size += 1;
        myHeap->numberOfEventsRemaining--;
    }
        
    buildHeap(myHeap);
    
    return;
}

struct Customer * newCustomer(){
    struct Customer *newCust = malloc(sizeof(struct Customer));
    
    newCust->PQTime = 0;
    newCust->ArrivalTime = 0;
    newCust->DepartureTime = -1;
    newCust->StartofServiceTime = 0;
    newCust->next = NULL;
    
    return newCust;
}

void buildHeap(struct Heap * myHeap){
    int i;
    for(i = SIZE/2; i> 0 ; i--)
        percolateDown(myHeap, i);
}



void percolateDown(struct Heap * myHeap, int slot){ 
    int child;
    struct Customer tmp = myHeap->customers[slot];
    
    while (slot * 2 <= myHeap->size){
        child = slot * 2;
        if(child != myHeap->size && myHeap->customers[child + 1].PQTime < myHeap->customers[child].PQTime)
            child++;
        if(myHeap->customers[child].PQTime < tmp.PQTime)
            myHeap->customers[slot] = myHeap->customers[child];
        else
            break;
        slot = child;
    }
    myHeap->customers[slot] = tmp;
}

void percolateUp(struct Heap * myHeap, struct Customer  custToInsert){ //adding a departure back into the PQ
    myHeap->customers[0] = custToInsert;
    
    int slot = ++(myHeap->size);
    while( custToInsert.PQTime < myHeap->customers[slot / 2].PQTime){
        myHeap->customers[slot] = myHeap->customers[slot / 2];
        slot /= 2;
    }
    myHeap->customers[slot] = custToInsert;
}

//even though it just calls percolateUp, it's good for a naming convention.
void insertHeap(struct Heap * myHeap, struct Customer custToInsert){
    percolateUp(myHeap, custToInsert);
}


struct Customer * deleteFromHeap(struct Heap * myHeap){
    
    struct Customer * custToReturn = malloc(sizeof(struct Customer));
    *custToReturn = myHeap->customers [1];
    myHeap->customers[1] = myHeap->customers[myHeap->size];
    myHeap->size = myHeap->size - 1;
    percolateDown(myHeap, 1);
    return custToReturn;
    
}

double getRandInterval(int avg){
    double f = rand() / ((double) RAND_MAX);
    double intervalTime = -1 * (1.0/avg) * (log(f));
    return intervalTime;
}


// Po
double generatePercentIdleTime(int M, int lambda, int mu){
    double PO;
    
    double denominatorLeft = 0;
    int i;
    /*summation*/
    for(i = 0; i<= M - 1; i++){
        denominatorLeft += (pow( ((double)lambda/mu) , (double) i)/factorial(i)) ;
    }
    double denominatorRight = 0;
    denominatorRight = pow((double)lambda/mu, (double) M) * M * mu / factorial(M) / (M * mu - lambda);
            
    
    
    PO = 1/(denominatorLeft + denominatorRight);
    return PO;
}

// L
double getAverageNumberInSystem(int M, int lambda, int mu){
    double L = 0;

    L = (double) lambda / mu;
    L += generatePercentIdleTime(M, lambda, mu) * mu * lambda * pow((double) lambda/mu , M)/factorial(M - 1) / (M * mu - lambda) / (M * mu - lambda);
    return L;
}

// W
double getAverageTimeInSystem(int M, int lambda, int mu){
    return getAverageNumberInSystem(M, lambda, mu) / lambda;
}

// Lq
double getAverageNumberOfCustomers(int M, int lambda, int mu){
    return getAverageNumberInSystem(M, lambda, mu) - (double)lambda/mu;
}

// Wq
double getAverageWaitTime(int M, int lambda, int mu){
    return getAverageNumberOfCustomers(M, lambda, mu) / lambda ;
}

// rho
double utilizationFactor(int M, int lambda, int mu){
    return (double)lambda / M / mu ;
}

//used for the analytic statistics
int factorial(int x){
    if( x == 0)
        return 1;
    return (x*factorial(x-1));
}




void processNextEvent(struct Heap *myHeap, struct parameters *params){
    if(myHeap->customers[1].DepartureTime == -1){ //arrival event 
        if(myHeap->serversAvailable > 0){
            myHeap->serversAvailable--;
            myHeap->customers[1].StartofServiceTime = myHeap->customers[1].ArrivalTime;
            double timeInterval = getRandInterval(params->mu);
            myHeap->customers[1].DepartureTime = myHeap->customers[1].StartofServiceTime + timeInterval;
            myHeap->totalServiceTime += timeInterval;
            myHeap->customers[1].PQTime = myHeap->customers[1].DepartureTime;
            struct Customer *tmp = malloc(sizeof(struct Customer));
            tmp = deleteFromHeap(myHeap);
            insertHeap(myHeap, *tmp);
            free(tmp);
        }
        else{
            struct Customer *custToInsert = malloc(sizeof(struct Customer));
            *custToInsert = myHeap->customers[1];
            struct Customer *tmp = deleteFromHeap(myHeap);
            free(tmp);
            insertQueue(myHeap->myQueue, custToInsert);
            
            
        }
    }
    else{ //departure Event
       
        myHeap->serversAvailable++;
        
        //if there are people in the FIFO queue
        if(myHeap->myQueue->head != NULL){
            
            myHeap->custWhoHadToWait++;
            struct Customer *cust = serve(myHeap->myQueue);
            cust->StartofServiceTime = myHeap->customers[1].DepartureTime;
            double interval = getRandInterval(params->mu);
            myHeap->timeSpentWaitingInQueue += interval;
            
            myHeap->totalTimeSpentInSystem += (cust->StartofServiceTime - cust->ArrivalTime) + (interval);
            
            cust->DepartureTime = cust->StartofServiceTime + interval;
            cust->PQTime = cust->DepartureTime;
            insertHeap(myHeap, *cust);
            myHeap->serversAvailable--;
        }
        else{
            //calc idle time
            if(myHeap->serversAvailable == myHeap->serversTotal){
          
                double idle = 0;
                int i;
                double smallest = myHeap->customers[2].ArrivalTime;
                //heap isn't sorted so we have to find the next chronological event
                for(i = 2; i <= myHeap->size; i++){
                    if( myHeap->customers[i].ArrivalTime <= smallest)
                        smallest = myHeap->customers[i].ArrivalTime;
                }
                idle = smallest - myHeap->customers[1].DepartureTime;
                myHeap->timeIdle += idle;
             }
            
            //last departure, need to get total sim time
            if(myHeap->size == 1)
                myHeap->totalSimulationTime = myHeap->customers[1].DepartureTime;
            double timeSpentInSystem = 0;
            timeSpentInSystem = myHeap->customers[1].DepartureTime - myHeap->customers[1].ArrivalTime;
            myHeap->totalTimeSpentInSystem += timeSpentInSystem;
            struct Customer * tmp = deleteFromHeap(myHeap);
            free(tmp);
        }
        
    }
}



// adds more arrivals after the initial 100
void addNewArrival(struct Heap * myHeap, struct parameters * params){
    
    struct Customer custToInsert;
    custToInsert.ArrivalTime = findBiggestArrivalTime(myHeap) + getRandInterval(params->lambda);
    custToInsert.DepartureTime = -1;
    custToInsert.PQTime = custToInsert.ArrivalTime;
    custToInsert.StartofServiceTime = 0;
    
    custToInsert.next = NULL;
    
    insertHeap(myHeap, custToInsert);
   
}

// to know what the next arrivals ArrivalTime will be we need the arrival time of the last event.
double findBiggestArrivalTime(struct Heap * myHeap){
    double biggest = 0;
    int i;
    for(i = 1; i <= myHeap->size; i++){
        if(myHeap->customers[i].ArrivalTime >= biggest)
            biggest = myHeap->customers[i].ArrivalTime;
    }
    return biggest;
}