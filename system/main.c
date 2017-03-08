/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define PROB  4 //problem #

extern void cpubnd(void);
extern void iobnd(void);


void looper(int n)
{
	while(TRUE)
		kprintf("[looper%d] \n",n);
	return;
}
void blocker()
{
	while(TRUE)
	{
		int i;
		kprintf("[blocker]\n");
		for(i = 0; i < 500000; i++)
			kprintf("[blocker_for%d]\n", i);
		sleepms(12);
	}		
	return;
}
void print_proc_cpu()
{
	struct	procent	*prptr;
	kprintf("Printing cputime of existing processes...\n");
	int i;
	for (i = 1; i < NPROC; i++) 
	{
		prptr = &proctab[i];
		if (prptr->prstate == PR_FREE ) //|| i == currpid) 
		{  /* skip unused slots	*/
			continue;
		}
		kprintf("(PID: %u) -> cputime: %u\n", i, prptr->prcpuused );
	}
}


process	main(void)
{
	
	kprintf("\nRunning first XINU app and running function main() in system/main.c.\n");
	kprintf("initiated by nulluser() in system/initialize.c using create().\n");
	kprintf("\nCreating a second XINU app that runs shell() in shell/shell.c as an example.\n");
	kprintf("\nKrutarth Rao - raok - (0027262283)\n");
	
	/* run test cases for each problem by changing PROB variable in main.c */
	
	/* Problem 3 */
	if(PROB == 3)
	{
		struct	procent	*prptr;
		int i = 0;
		kprintf("[main]Begin main @ clktimefine : %d\n", clktimefine);
		print_proc_cpu();
		
		kprintf("[main]resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
		print_proc_cpu();
		pid32 s_id1 = create(iobnd, 515, 50, "iobnd1", 1, 1);		
		resume(s_id1);

		kprintf("[main]resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
		print_proc_cpu();
		pid32 s_id2 = create(iobnd, 515, 40, "iobnd2", 0);		
		resume(s_id2);

		kprintf("[main]resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
		print_proc_cpu();
		pid32 s_id3 = create(iobnd, 515, 30, "iobnd3", 0);		
		resume(s_id3);


		sleepms(5000);
				
		kprintf("[main]Resumed after sleeping for 5 sec @ clktimefine : %d\n", clktimefine);
		kprintf("Printing cputime of existing processes...\n");
		for (i = 1; i < NPROC; i++) 
		{
			prptr = &proctab[i];
			if (prptr->prstate == PR_FREE || i == currpid) {  /* skip unused slots	*/
				continue;
			}
			kprintf("(PID: %u) -> cputime: %u\n", i, prptr->prcpuused );
		}

		prptr = &proctab[s_id1];
		uint32 sleeperTime =  prptr->prcpuused;
		kprintf("[main] iobnd1 used %u ms\n", sleeperTime);
		kill(s_id1);
		
		prptr = &proctab[s_id2];
		sleeperTime = prptr->prcpuused;
		kprintf("[main] iobnd1 used %d ms\n", sleeperTime);
		kill(s_id2);

		prptr = &proctab[s_id3];
		sleeperTime =  prptr->prcpuused;
		kprintf("[main] iobnd3 used %d ms\n", sleeperTime);
		kill(s_id3);

		return OK;			
	}
	else if(PROB == 4)
	{
		int n; 
	    int i;
	    HPQ q;
	    heapNode hn;
	    n = 15;
	    initQueue(&q, n);
	    uint32 a[15] = {99, 1123, 645, 12, 723,74599, 7361, 55, 9876, 854, 47, 1, 3, 87, 748};

	    for (i = 0; i < n; ++i) 
	    {
	        hn.key = a[i];
	        hn.pid = i;
	        printf("enqueue node with key: %u, pid: %d\n", hn.key, hn.pid);
	        h_enqueue(hn, &q);
	    }
		
		printf("[+] all items added, queue size: %d\n", q.size);
	    pid32 to_rem =  3;
	    h_remove(&q, to_rem);
	    printf("[+] removed process with pid: %d\n", to_rem);

	    to_rem =  0;
	    h_remove(&q, to_rem);
	    printf("[+] removed process with pid: %d\n", to_rem);

	    printf("dequeue all values:\n");
	    n = q.size;
	    for (; q.size > 0; ) 
	    {
	        hn = h_dequeue(&q);
	        printf("dequeued node with key: %u, pid: %d (queue size: %d)\n", hn.key, hn.pid, q.size);
	    }

	    return OK;
	}
	else
	{
		/* run shell program as usual */	
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
