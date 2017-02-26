/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define PROB  5 //problem #

extern void stacksmashA();
extern void stacksmashV();
extern void takeover(void);

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


process	main(void)
{
	
	kprintf("\nRunning first XINU app and running function main() in system/main.c.\n");
	kprintf("initiated by nulluser() in system/initialize.c using create().\n");
	kprintf("\nCreating a second XINU app that runs shell() in shell/shell.c as an example.\n");
	kprintf("\nKrutarth Rao - raok - (0027262283)\n");
	
	/* run test cases for each problem by changing PROB variable in main.c */
	
	/* Problem 3 */
	if(PROB == 3 || PROB == 4)
	{
		pid32 s_id1 = create(looper, 515, 50, "looper1", 1, 1);
		pid32 s_id2 = create(blocker, 515, 10, "blocker", 0);		
		resume(s_id1);
		resume(s_id2);

		sleepms(5000);
				
		kprintf("[main]Resumed after sleeping for 5 sec @ clktimefine : %d\n", clktimefine);
		struct	procent	*prptr;

		prptr = &proctab[s_id1];
		uint32 sleeperTime =  prptr->prcpuused;
		kprintf("[main] looper1 used %d ms\n", sleeperTime);
		kill(s_id1);
		
		prptr = &proctab[s_id2];
		sleeperTime =  prptr->prcpuused;
		kprintf("[main] blocker used %d ms\n", sleeperTime);
		kill(s_id2);
		
		return OK;			
	}
	else if(PROB == 5)
	{
		/* Problem 5 */

		// Initilize the attacker stack on top of victim
		uint32 at = create(stacksmashA, 256, 50, "stackmashA", 0) ;

		// Allow victim to print V and call sleep
		resume( create(stacksmashV, 520, 50, "stackmashV", 0) );

		// begin attacking
		resume(at);

		// prevent attacker from getting switched out by making main sleep
		sleep(5);

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
