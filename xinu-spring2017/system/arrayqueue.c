#include <xinu.h>

// initialize a queue - return 0 on failure
int init_queue(struct array_queue *queue)
{
    queue->queue_size = 0;
   	queue->head = 0;
    queue->tail = -1;       // starts at -1 to make add logic easier - always allowed to increment this
    queue->array_size = ARRAY_SIZE;
    return OK;
}

// add to queue - return 0 on failure
int add_to_queue(struct array_queue *queue, pid32 value)
{
    // determine the new head
    if(queue->queue_size == queue->array_size)
    {
        return SYSERR;
    }
    else
    {
        // we have enough space, but tail is at the end of array
        queue->tail ++;
        
        // with queue size being a power of 2, we can increment, then mask to loop back to index 0 if needed
        queue->tail &= (queue->array_size - 1);
        queue->array[queue->tail] = value;
    }    
    queue->queue_size ++;
    return OK;
}

// read from the queue, removing the value
// return 0 if no item found
int read_from_queue(struct array_queue *queue)
{
    if(0 == queue->queue_size)
    {
        // nothing in queue
        return EMPTY;
    }
    pid32 value = queue->array[queue->head++];
    
    // with queue size being a power of 2, we can increment, then mask to loop back to index 0 if needed
    queue->head &= (queue->array_size - 1);
    queue->queue_size--;    
    return value;
}

