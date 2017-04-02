#ifndef DEADLOCK_H_
#define DEADLOCK_H_

#define SEM_INDEX 50
struct AdjListNode
{
    int dest;
    struct AdjListNode* next;
};

struct AdjList
{
    struct AdjListNode *head;
};
struct Graph
{
    int V;
    struct AdjList *array;
};

extern struct Graph* resourceGraph;

struct AdjListNode* newAdjListNode(int dest);
struct Graph* createGraph(int V); 
void addEdge(struct Graph* graph, int src, int dest);
void removeEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
int cycle_util(struct Graph* graph, int v, int *visited, int *intmd);
int is_cycle(struct Graph* graph);
int get_sem_vertex(sid32 sem);

#endif