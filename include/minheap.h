
#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2

typedef struct node {
    int data ;
    int pid;
} node ;

typedef struct minHeap {
    int size;
    node elem[NPROC];
} minHeap ;


status newheap(); //creates the new heap, return OK or SYSERR. 
status heapinsert( pid32 pid, int32 key); //insert a process of PID with the given key into the heap 
pid32 heapgethead(); //removes the root node and return its process id.
int32 heapminkey(); //returns the key value at the root node of the heap without removing the node.
pid32 heapgetitem(pid32); //find and remove the process with the given pid from the heap. Status returns the process id or SYSERR 


