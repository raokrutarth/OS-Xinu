/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define PROB  3 //problem #

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
		for(i = 0; i < 500000; i++);
			kprintf("[blocker_for%d]\n", i);
		sleepms(12);
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
		pid32 s_id1 = create(looper, 515, 19, "looper1", 1, 1);
		pid32 s_id2 = create(looper, 515, 19, "looper2", 1, 2);
		pid32 s_id3 = create(looper, 585, 19, "looper3", 1, 3);
		resume(s_id1);
		resume(s_id2);
		resume(s_id3);
		sleepms(5000);
		kill(s_id1);

		
		kprintf("[main] sleeping for 5 sec @ clktimefine : %d\n", clktimefine);
		struct	procent	*prptr;
		prptr = &proctab[s_id1];
		uint32 sleeperTime =  prptr->prcpuused;
		kprintf("[main] looper1 used %d ms\n", sleeperTime);
		
		prptr = &proctab[s_id2];
		sleeperTime =  prptr->prcpuused;
		kprintf("[main] looper2 used %d ms\n", sleeperTime);
		
		prptr = &proctab[s_id3];
		sleeperTime =  prptr->prcpuused;
		kprintf("[main] blocker used %d ms\n", sleeperTime);			
	}
	/* Problem 4 */
	if(PROB == 4)
	{
		
	}
	else if(PROB == 5)
	{
		/* Problem 5 */
		
	}
	else if(PROB == 6)
	{
		/* problem 6 */
		
	}
	else if(PROB == 7)
	{
		/*use rcreate to start looper */
		
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
