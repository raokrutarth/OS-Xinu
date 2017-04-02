#include <xinu.h>

/* Begin graph API */ 
struct AdjListNode* newAdjListNode(int dest)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) getmem(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}
int get_sem_vertex(sid32 sem)
{
	return (sem + SEM_INDEX);
}

struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) getmem(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList*) getmem(V * sizeof(struct AdjList));
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL; 
    return graph;
}
 
void addEdge(struct Graph* graph, int src, int dest)
{
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}
void removeEdge(struct Graph* graph, int src, int dest)
{
	struct AdjListNode* curr = graph->array[src].head;
	if(curr->dest == dest)
	{
		graph->array[src].head = curr->next;
		return;
	}
	while (curr)
    {
    	printf("[-] %d", curr->dest);
        if(curr->next->dest == dest)
        {
        	curr->next = curr->next->next;
        	break;
        }
        curr = curr->next;
    }
     printf("(-)\n");
}

void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->array[v].head;
        if(pCrawl)
        {
        	printf("\n Adjacency list of vertex %d\n head ", v);
        }        
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        if(pCrawl)
        	printf("\n");
    }
}

int cycle_util(struct Graph* graph, int v, int *visited, int *intmd)
{
	if(visited[v] == FALSE)
	{
		visited[v] = TRUE;
		intmd[v] = TRUE;
	    struct AdjListNode* pCrawl = graph->array[v].head;
        while (pCrawl)
        {
            if( visited[pCrawl->dest] == FALSE && cycle_util(graph, pCrawl->dest, visited, intmd) )
            {
            	return TRUE;
            }
            else if (intmd[pCrawl->dest] == TRUE)
            	return TRUE;
            pCrawl = pCrawl->next;
        }
	}
	intmd[v] = FALSE;
	return FALSE;
}

int is_cycle(struct Graph* graph)
{
	int * visited = (int*) getmem(graph->V * sizeof(int) );
	int * intmd = (int*) getmem(graph->V * sizeof(int) );
	int i;
	for(i = 0; i < graph->V; i++)
    {
        visited[i] = FALSE;
        intmd[i] = FALSE;
    }

    for(i = 0; i < graph->V; i++)
        if (cycle_util(graph, i, visited, intmd))
            return TRUE; 
    return FALSE;
}

