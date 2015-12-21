#ifndef _HEAPH_
#define _HEAPH_



#include "Queue.h"
#include "FileIO.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE 101

struct Heap {
    struct Customer customers[SIZE];
    int size;
    int totalNumberOfEvents;
    int numberOfEventsRemaining;
    int serversAvailable;
    int serversTotal;
    struct queue * myQueue;
    
    double totalServiceTime;
    int custWhoHadToWait;
    double timeIdle;
    int custLeftToServiceAfterInitial100;
    double totalSimulationTime;
    
    double timeSpentWaitingInQueue;
    
    double totalTimeSpentInSystem;
};
double findBiggestArrivalTime(struct Heap *);
void constructHeap(struct Heap *, struct parameters * );

void insertHeap(struct Heap *, struct Customer);
struct Customer * newCustomer();
void buildHeap(struct Heap * );
void percolateDown(struct Heap * , int);
void percolateUp(struct Heap * , struct Customer );
struct Customer * deleteFromHeap(struct Heap * );
int factorial(int );
double generatePercentIdleTime(int , int , int );
double getRandInterval(int );
double getAverageNumberInSystem(int , int , int );
double getAverageTimeInSystem(int, int, int);
double utilizationFactor(int , int , int );
double getAverageWaitTime(int, int , int );

double getAverageNumberOfCustomers(int , int , int );

void processNextEvent(struct Heap *, struct parameters *);

void addNewArrival(struct Heap *, struct parameters *);

#endif