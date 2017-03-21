
#ifndef MINHEAP_H
#define MINHEAP_H

#define LEFT(x) (2 * (x) + 1)
#define RIGHT(x) (2 * (x) + 2)
#define PARENT(x) ((x) / 2)


typedef struct heapNode {
    uint32 key;
    pid32 pid;
} heapNode;
 
typedef struct HPQ {
    heapNode heap[NPROC*2];
    int size;
} HPQ;

#define	ERR_KEY	0x80000000	/* error key for book keeping	*/

HPQ heap_queue; // heap queue used for maintaining readylist

status h_insert(heapNode aNode, heapNode* heap, int size);
void shiftdown(heapNode* heap, int size, int idx);
heapNode removeMin(heapNode* heap, int size);
void h_enqueue(heapNode node, HPQ *q);
heapNode h_dequeue(HPQ *q);
void initQueue(HPQ *q);
void shiftUp(heapNode* heap, int size, int index) ;
void removeAt(heapNode* heap, int size, int where) ;
int32 h_remove(HPQ *q, pid32 pid);

status newheap(); //creates the new heap, return OK or SYSERR. 
status heapinsert( pid32 pid, int32 key); //insert a process of PID with the given key into the heap 
pid32 heapgethead(); //removes the root node and return its process id.
int32 heapminkey(); //returns the key value at the root node of the heap without removing the node.
pid32 heapgetitem(pid32); //find and remove the process with the given pid from the heap. Status returns the process id or SYSERR 


#endif




