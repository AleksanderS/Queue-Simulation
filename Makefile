a.out: Heap.c main.c FIFO_Queue.c FileIO.c
	gcc main.c Heap.c FIFO_Queue.c FileIO.c -lm