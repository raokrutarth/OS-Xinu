/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define prob 4


extern void testCallback();
extern void testSignal();

process	main(void)
{
	kprintf("\nXINU [Krutarth Rao - 0027262283] \n");	
	
	if( prob == 4)
	{
		testSignal();
	}
	else if(prob == 3)
	{
		// in file testCallback.c
		testCallback();		
	}
	else if (prob == 2)
	{
		while(TRUE)
		{
			kprintf("clktimefine: %u\n", clktimefine);
			sleep(2);
		}
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
