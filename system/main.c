/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define PROB  3

extern void cpubnd(void);
extern void iobnd(void);


void print_proc_cpu();
void test_cpubnd_same();
void test_cpubnd_var();
void test_iobnd_var();
void test_hybrid();
void test_minheap();

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
		// test_cpubnd_same();
		// test_cpubnd_var();
		// test_iobnd_var();
		test_hybrid();	
	}
	else if(PROB == 4)
	{
		test_minheap();
	}
	else
	{
		/* run shell program as usual */	
		recvclr();
		resume(create(shell, 8192, 50, "shell", 1, CONSOLE));	
		/* Wait for shell to exit and recreate it */		
		while (TRUE) 
		{
			receive();
			sleepms(200);
			kprintf("\n\nMain process recreating shell\n\n");
			resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
		}		
	}
	return OK;
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
		kprintf("(PID: %u)[%s] -> cputime: %u\n", i, prptr->prname, prptr->prcpuused );
	}
}

void test_cpubnd_same()
{
	struct	procent	*prptr;
	kprintf("[main] Begin main @ clktimefine : %d, total_cpu_usage:  %u, total_ready_proc : %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	print_proc_cpu();
	
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id1 = create(cpubnd, 515, 1, "cpubnd1", 0);		
	resume(s_id1);

	print_proc_cpu();
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id2 = create(cpubnd, 515, 1, "cpubnd2", 0);	
	resume(s_id2);

	print_proc_cpu();
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id3 = create(cpubnd, 515, 1, "cpubnd3", 0);		
	resume(s_id3);

	sleepms(5000);
			
	kprintf("[main] Resumed after sleeping for 5 sec @ clktimefine : %d\n", clktimefine);
	print_proc_cpu();

	prptr = &proctab[s_id1];
	uint32 sleeperTime =  prptr->prcpuused;
	kprintf("[main] iobnd1 prcpuused: %u ms\n", sleeperTime);
	kill(s_id1);
	
	prptr = &proctab[s_id2];
	sleeperTime = prptr->prcpuused;
	kprintf("[main] iobnd2 prcpuused: %u ms\n", sleeperTime);
	kill(s_id2);

	prptr = &proctab[s_id3];
	sleeperTime =  prptr->prcpuused;
	kprintf("[main] iobnd3 prcpuused: %u ms\n", sleeperTime);
	kill(s_id3);
	return;
}

void test_cpubnd_var()
{
	struct	procent	*prptr;
	kprintf("[main] Begin main @ clktimefine : %d, total_cpu_usage:  %u, total_ready_proc : %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id1 = create(cpubnd, 515, 1, "cpubnd1", 0);		
	resume(s_id1);

	sleepms(2000);
	print_proc_cpu();
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id2 = create(cpubnd, 515, 1, "cpubnd2", 0);	
	resume(s_id2);

	sleepms(2000);
	print_proc_cpu();
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id3 = create(cpubnd, 515, 1, "cpubnd3", 0);		
	resume(s_id3);
	sleepms(2000);

	kprintf("[main] Resumed @ clktimefine : %d\n", clktimefine);
	print_proc_cpu();

	prptr = &proctab[s_id1];
	uint32 sleeperTime =  prptr->prcpuused;
	kprintf("[main] iobnd1 prcpuused: %u ms\n", sleeperTime);
	kill(s_id1);
	
	prptr = &proctab[s_id2];
	sleeperTime = prptr->prcpuused;
	kprintf("[main] iobnd2 prcpuused: %u ms\n", sleeperTime);
	kill(s_id2);

	prptr = &proctab[s_id3];
	sleeperTime =  prptr->prcpuused;
	kprintf("[main] iobnd3 prcpuused: %u ms\n", sleeperTime);
	kill(s_id3);
	return;
}

void test_iobnd_var()
{
	struct	procent	*prptr;
	kprintf("[main] Begin main @ clktimefine : %d, total_cpu_usage:  %u, total_ready_proc : %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id1 = create(iobnd, 515, 1, "iobnd1", 0);		
	resume(s_id1);

	sleepms(2000);
	print_proc_cpu();
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id2 = create(iobnd, 515, 1, "iobnd2", 0);	
	resume(s_id2);

	sleepms(2000);
	print_proc_cpu();
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id3 = create(iobnd, 515, 1, "iobnd3", 0);	
	resume(s_id3);
	sleepms(2000);

	kprintf("[main] Resumed @ clktimefine : %d\n", clktimefine);
	print_proc_cpu();

	prptr = &proctab[s_id1];
	uint32 sleeperTime =  prptr->prcpuused;
	kprintf("[main] iobnd1 prcpuused: %u ms\n", sleeperTime);
	kill(s_id1);
	
	prptr = &proctab[s_id2];
	sleeperTime = prptr->prcpuused;
	kprintf("[main] iobnd2 prcpuused: %u ms\n", sleeperTime);
	kill(s_id2);

	prptr = &proctab[s_id3];
	sleeperTime =  prptr->prcpuused;
	kprintf("[main] iobnd3 prcpuused: %u ms\n", sleeperTime);
	kill(s_id3);

	return;
}
void test_hybrid()
{
	struct	procent	*prptr;
	kprintf("[main] Begin main @ clktimefine : %d, total_cpu_usage:  %u, total_ready_proc : %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id1 = create(iobnd, 515, 1, "iobnd1", 0);		
	resume(s_id1);

	sleepms(2000);
	print_proc_cpu();
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id3 = create(cpubnd, 515, 1, "cpubnd1", 0);	
	resume(s_id3);

	sleepms(2000);
	print_proc_cpu();
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id2 = create(iobnd, 515, 1, "iobnd2", 0);	
	resume(s_id2);	
	

	sleepms(2000);
	print_proc_cpu();
	kprintf("[main] Resuming new process @ clktimefine : %u, total_used time: %u, processes: %u\n", clktimefine, total_cpu_usage, total_ready_proc);
	pid32 s_id4 = create(cpubnd, 515, 1, "cpubnd2", 0);	
	resume(s_id4);

	sleepms(5000);

	kprintf("[main] Resumed @ clktimefine : %d\n", clktimefine);
	print_proc_cpu();

	prptr = &proctab[s_id1];
	uint32 sleeperTime =  prptr->prcpuused;
	kprintf("[main] iobnd1 prcpuused: %u ms\n", sleeperTime);
	kill(s_id1);
	
	prptr = &proctab[s_id2];
	sleeperTime = prptr->prcpuused;
	kprintf("[main] iobnd2 prcpuused: %u ms\n", sleeperTime);
	kill(s_id2);

	prptr = &proctab[s_id3];
	sleeperTime =  prptr->prcpuused;
	kprintf("[main] cpubnd1 prcpuused: %u ms\n", sleeperTime);
	kill(s_id3);

	prptr = &proctab[s_id4];
	sleeperTime =  prptr->prcpuused;
	kprintf("[main] cpubnd2 prcpuused: %u ms\n", sleeperTime);
	kill(s_id4);

	return;
}

void test_minheap()
{
	kprintf("***TESING MIN-HEAP data stucture***\n");
	int n; 
    int i;
    HPQ q;
    heapNode hn;
    n = 10;
    initQueue(&q);
    uint32 a[10] = {2, 584, 4, 3561, 7, 34, 1355, 1, 13245, 5436};

    for (i = 0; i < n; ++i) 
    {
        hn.key = a[i];
        hn.pid = i;
        printf("enqueue node with key: %u, pid: %d\n", hn.key, hn.pid);
        h_enqueue(hn, &q);
    }
    pid32 to_rem =  0;
    h_remove(&q, to_rem);
    printf("[+] removed process with pid: %d\n", to_rem);

    to_rem =  4;
    h_remove(&q, to_rem);
    printf("[+] removed process with pid: %d\n", to_rem);

    printf("dequeue all values:\n");
    n = q.size;
    for (i = 0; i < n; i++) 
    {
        hn = h_dequeue(&q);
        printf("dequeued node with key: %u, pid: %d (queue size: %d)\n", hn.key, hn.pid, q.size);
    }
    return;
}
