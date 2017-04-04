#ifndef array_queue_h
#define array_queue_h

#define ARRAY_SIZE 512

struct array_queue
{
    int head;       
    int tail;       
    int queue_size;
    int array_size; 
    pid32 array[ARRAY_SIZE];
};

// initialize a queue with the given size (must be a power of 2) - return 0 on failure
int init_queue( struct array_queue *queue);


// add to queue - return 0 on failure
int add_to_queue(struct array_queue *queue, pid32 value);

// read from the queue, removing the value
int read_from_queue(struct array_queue *queue);

#endif