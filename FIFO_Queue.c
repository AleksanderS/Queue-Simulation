#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
#include "FileIO.h"
#include "Heap.h"




struct queue * createQueue(){
    struct queue *myQueue = malloc(sizeof(struct queue));
    myQueue->head = NULL;
    myQueue->tail = NULL;
    myQueue->size = 0;
    myQueue->numberOfCustomersThatHadToWait = 0;
    return myQueue;
            
}


void insertQueue(struct queue * myQueue, struct Customer * customerToInsert){
    if(myQueue->head == NULL && myQueue->tail == NULL){
        myQueue->head = myQueue->tail = customerToInsert;
        return;
    }
    myQueue->tail->next = customerToInsert;
    myQueue->tail = customerToInsert;
    myQueue->size++;
    myQueue->numberOfCustomersThatHadToWait++;
    return;
}

struct Customer * serve(struct queue * myQueue){
    if(myQueue->head == NULL){
        fprintf(stderr, "List is empty.");
        return NULL;
    }
    if(myQueue->head == myQueue->tail){
        struct Customer * tmp = myQueue->head;
        myQueue->head = myQueue->tail =  NULL;
        myQueue->size--;
        return tmp;
        
    }
    struct Customer * tmp = myQueue->head;
    myQueue->head = myQueue->head->next;
    
    myQueue->size--;
    return tmp;
}

// not needed as Heap has its own
struct Customer * createCustomer(){
    struct Customer * tmp = malloc(sizeof(struct Customer));
    tmp->ArrivalTime = 0;
    tmp->DepartureTime = 0;
    tmp->PQTime = 0;
    tmp ->StartofServiceTime = 0;
    tmp->next = NULL;
    return tmp;
}
//wont be needed as anything in the queue will be removed.
void freeList(struct queue * my_queue){
    while(my_queue->head != NULL){
        struct Customer * tmp = my_queue->head->next;
        free(my_queue->head);
        my_queue->head = tmp;
    }
}



