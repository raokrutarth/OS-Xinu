/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define prob 3

int32 msg_recv_cb(void) 
{
	umsg32 msgbuf;
	msgbuf = receive();
	return(OK);
}

extern void testSendBlocking();

process	main(void)
{
	kprintf("\nXINU [Krutarth Rao - 0027262283] \n");	
	
	if( prob == 4)
	{
		if (regcallbacksig(&msg_recv_cb, XINUSIGXTIME, 5000) != OK) {
	      kprintf("wall time handler registration failed\n");
	      return 1;
	   }
	}
	else if (prob == 2)
	{
		if (regcallback(&msg_recv_cb) != OK) {
	      kprintf("callback function registration failed\n");
	      return 1;
	   }
	}
	else if(prob == 3)
	{
		testSendBlocking();
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

