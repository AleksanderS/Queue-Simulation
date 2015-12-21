#ifndef _QUEUEH_
#define _QUEUEH_
struct Customer {
    /*Priority Queue Time*/double PQTime; // = arrival time
    double ArrivalTime; // = to previous customers departure time // pass lambda
    double StartofServiceTime; // = to arrival time if coming from priority queue and there is a server available; then DTime becomes SOSTime + randomInterval; then PQTime = DTime.
                               // if no server is available, then push it into the FIFO queue. Once a departure from PQ is process, a server is freed, then the first person in the FIFO queue gets service. Then SOSTime = departure time of previous customer.
                               // pass mu
    double DepartureTime; // if -1, they haven't departed. used to tell if departure or arrival event.
    
    struct Customer * next;
    
};

struct queue {
    struct Customer * head;
    struct Customer * tail;
    int size;
    int numberOfCustomersThatHadToWait;
};

struct queue * createQueue();
void insertQueue(struct queue *, struct Customer *);
struct Customer * serve(struct queue * );
struct Customer * createCustomer();
void freeList(struct queue * );
void freeList(struct queue * );

#endif