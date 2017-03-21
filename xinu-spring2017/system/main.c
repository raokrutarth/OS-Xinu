/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define PROB  3

volatile int global_ctr = 0;

void p1(volatile int *a)
{
	kprintf("p1 \n");
	while(TRUE)
	{
		*a = *a + 1;
		
	}		
	return;
}

void p2(volatile int *a)
{
	kprintf("p2 \n");
	while(TRUE)
	{
		*a = *a - 1;
	}
	
	return;
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
		pid32 s_id1 = create(p1, 515, 10, "p1", 1, &global_ctr);
		pid32 s_id2 = create(p2, 515, 10, "p2", 1, &global_ctr);		
		resume(s_id1);
		resume(s_id2);
		while(TRUE)
		{
			kprintf("ctr: %d\n", global_ctr );
			struct	procent	*prptr1;
			struct	procent	*prptr2;
			prptr1 = &proctab[s_id1];
			prptr2 = &proctab[s_id2];
			uint32 sleeperTime1 =  prptr1->prcpuused;
			uint32 sleeperTime2 =  prptr2->prcpuused;
			kprintf("[main] p1 used %u ms & p2 used %u ms\n", sleeperTime1, sleeperTime2);
		}
			
	}
	else if(PROB == 4)
	{
		
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

