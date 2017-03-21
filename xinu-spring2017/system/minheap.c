// status newheap() //creates the new heap, return OK or SYSERR. 
// status heapinsert( pid32 pid, int32 key) //insert a process of PID with the given key into the heap 
// pid32 heapgethead() //removes the root node and return its process id.
// int32 heapminkey() //returns the key value at the root node of the heap without removing the node.
// pid32 heapgetitem(pid32) //find and remove the process with the given pid from the heap. Status returns the process id or SYSERR 

#include <xinu.h>


status newheap()
{
    initQueue(&heap_queue);
	return OK;
}

status heapinsert( pid32 pid, int32 key)
{
    heapNode newNode;
    newNode.key = key;
    newNode.pid = pid;
    h_enqueue(newNode, &heap_queue);
    // kprintf("[+] Increasing total_ready_proc with pid: %d\n", pid);
    total_ready_proc++; /* maintain # of entries in the readylist */
    return OK;
}

pid32 heapgethead()
{
    HPQ *hq = &heap_queue;
    // kprintf("[-] Decreasing total_ready_proc\n");
    total_ready_proc--; /* maintain # of entries in the readylist */
    if(hq->size > 0)
    {
        heapNode ret = h_dequeue(&heap_queue);
        return ret.pid;
    }
	return EMPTY;
}

int32 heapminkey()
{
	HPQ *hq = &heap_queue;
    if(hq->size > 0)
    {
        heapNode ret = h_dequeue(&heap_queue);
        h_enqueue(ret, &heap_queue);
        return ret.key;
    }
    return EMPTY;
}

pid32 heapgetitem(pid32 pid)
{
	return h_remove(&heap_queue, pid);
}

 
status h_insert(heapNode aNode, heapNode* heap, int size) 
{
    if(size > NPROC)
    {
        return SYSERR;
    }
    int idx;
    heapNode tmp;
    idx = size + 1;
    heap[idx] = aNode;
    while (heap[idx].key < heap[PARENT(idx)].key && idx > 1) 
    {
        tmp = heap[idx];
        heap[idx] = heap[PARENT(idx)];
        heap[PARENT(idx)] = tmp;
        idx = PARENT(idx);
    }
    return OK;
}
 
void shiftdown(heapNode* heap, int size, int idx) 
{
    int cidx;        //index for child
    heapNode tmp;
    for (;;) 
    {
        cidx = idx*2;
        if (cidx > size) 
        {
            break;   //it has no child
        }
        if (cidx < size) 
        {
            if (heap[cidx].key > heap[cidx+1].key) 
            {
                // pick the smallest child
                ++cidx;
            }
        }
        //swap if necessary
        if (heap[cidx].key < heap[idx].key) 
        {
            tmp = heap[cidx];
            heap[cidx] = heap[idx];
            heap[idx] = tmp;
            idx = cidx;
        } 
        else 
        {
            break;
        }
    }
}
/**
     * Sift up to make sure the heap property is not broken. This method is used
     * when a new element is added to the heap and we need to make sure that it
     * is at the right spot.
*/
void shiftUp(heapNode *heap, int size, int idx)
{
    int parent = PARENT(idx);
    heapNode tmp;
    while (idx > 1 && parent > 0 && heap[idx].key < heap[parent].key )
    {        
        tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;

        idx = parent;
        parent = PARENT(idx);
    }
}
void removeAt(heapNode* heap, int size, int where) 
{
    // This should never happen, you should ensure to call it only with valid indices
    if (size == 0 || where >= size)
    {
        kprintf("HEAP ERROR in removeAt!!! \n");
        return;
    }
        
    // Now for the working cases
    if (where == size ) {
        // removing the final leaf, trivial
        return;
    }
    // other nodes
    // place last leaf into place where deletion occurs
    // printf("%d:%d:%d\n", where, heap[where].key, heap[size].key);
    heap[where] = heap[size];
    // take note that we have now one element less
    --size;
    // the new node here can be smaller than the previous,
    // so it might be smaller than the parent, therefore sift up
    // if that is the case
    if (where > 0 && heap[where].key < heap[PARENT(where)].key ) 
    {
        shiftUp(heap, size, where);
    } 
    else //if (where < size/2) 
    {
        // Now, if where has a child, the new value could be larger
        // than that of the child, therefore sift down
        shiftdown(heap, size, where);
    }
}
 
heapNode removeMin(heapNode* heap, int size) 
{
    heapNode rv = heap[1];
    //printf("%d:%d:%dn", size, heap[1].key, heap[size].key);
    heap[1] = heap[size];
    --size;
    shiftdown(heap, size, 1);
    return rv;
}
pid32 h_remove(HPQ *q, pid32 pid)
{
    if(q->size < 1)
    {
        kprintf("HEAP error! h_remove called on empty heap\n");
        return SYSERR;
    }
	int i;
	int size = q->size;
    for(i = 1; i < size; ++i)
    {
        if (q->heap[i].pid == pid) 
        {
            removeAt(q->heap, size, i);
            break;
        }
    }    
    --q->size;
    return pid;
}
void h_enqueue(heapNode node, HPQ *q) 
{
    h_insert(node, q->heap, q->size);
    ++q->size;
}
 
heapNode h_dequeue(HPQ *q) 
{
    if(q->size > 0)
    {
        heapNode rv = removeMin(q->heap, q->size);
        --q->size;
        return rv;
    }
    heapNode err;
    err.key = ERR_KEY;
    err.pid = -1;
    return err;    
}
 
void initQueue(HPQ *q) 
{
   q->size = 0;
}

