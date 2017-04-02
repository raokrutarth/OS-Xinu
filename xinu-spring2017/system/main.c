/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define prob 3

volatile int global_ctr = 0;
static sid32 sem_dd1;
static sid32 sem_dd2;

void p1(volatile int *a)
{
	kprintf("p1 started\n");
	waitdd(sem_dd1);
	sleepms(5);
	//signaldd(sem_dd1);
	waitdd(sem_dd2);
	sleepms(5);
	
	kprintf("p1 finished\n");
	return;
}

void p2(volatile int *a)
{
	kprintf("p2 started\n");
	waitdd(sem_dd2);
	sleepms(5);
	//signaldd(sem_dd2);
	waitdd(sem_dd1);
	sleepms(5);
	
	kprintf("p2 finished\n");
	return;
}


process	main(void)
{
	kprintf("\nXINU [Krutarth Rao - 0027262283] \n");
	sem_dd1 = semcreate(1);
	sem_dd2 = semcreate(1);
	if(prob == 3)
	{
		pid32 s_id1 = create(p1, 515, 20, "p1", 1, &global_ctr);
		pid32 s_id2 = create(p2, 515, 20, "p2", 1, &global_ctr);		
		resume(s_id1);
		resume(s_id2);
		sleep(3);
		kill(s_id1);
		kill(s_id2);
		printGraph(resourceGraph);
	}
	else if (prob == 2)
	{
		int V = 6;
	    struct Graph* graph = createGraph(V);
	    addEdge(graph, 0, 1);
	    addEdge(graph, 1, 2);
	    addEdge(graph, 1, 3);
	    addEdge(graph, 2, 3);
	    addEdge(graph, 3, 4);
	    addEdge(graph, 4, 1);
	    addEdge(graph, 5, 2);
	    addEdge(graph, 5, 1);
	    addEdge(graph, 0, 5);
	 
	    printGraph(graph);
	    int res = is_cycle(graph);
	    printf("\nHas cycle: %d\n", res);

	    removeEdge(graph, 2, 3);
	    removeEdge(graph, 5, 1);
	    removeEdge(graph, 4, 1);
	    printGraph(graph);
	    res = is_cycle(graph);
	    printf("\nHas cycle: %d\n", res);
	}
	else if( prob == 4)
	{

	}
	else
	{
		recvclr();
		resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
		/* Wait for shell to exit and recreate it */
		while (TRUE) {
			receive();
			sleepms(200);
			kprintf("\n\nMain process recreating shell\n\n");
			resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
		}
	}
	return OK;
}

