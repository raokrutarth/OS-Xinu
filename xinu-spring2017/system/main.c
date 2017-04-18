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
		// while(TRUE)
		// {
		// 	kprintf("clktimefine: %u\n", clktimefine);
		// 	sleep(2);
		// }
		struct allocated_mem tm;
		initMemRecord( &tm );

		int i;
		for( i = 0; i < 15; i++)
		{
			kprintf("Tracking block: addr = %d, size: %u\n", (i+50435) , (uint32) i*25);
			trackBlock( &tm, (char*) (i+50435) , (uint32) i*25);
		}
		kprintf("\nTracking %d blocks. tm.trackedBlocks: %d\n \n", i, tm.trackedBlocks);

		int u = 0;
		kprintf("Untracking block addr = %d, size: %u\n", (u+50435) , (uint32) u*25 );
		if( SYSERR == untrackBlock( &tm, (char*) (u+50435) ) )
			kprintf("Unable to untrack block\n");
		u = 5;
		kprintf("Untracking block addr = %d, size: %u\n", (u+50435) , (uint32) u*25 );
		if( SYSERR == untrackBlock( &tm, (char*) (u+50435) ) )
			kprintf("Unable to untrack block\n");
		u = 14;
		kprintf("Untracking block addr = %d, size: %u\n", (u+50435) , (uint32) u*25 );
		if( SYSERR == untrackBlock( &tm, (char*) (u+50435) ) )
			kprintf("Unable to untrack block\n");


		kprintf("\nPopping remaining blocks...\n");
		struct allocated_block t;
		t = popMemRecord( &tm );
		int c = 0;
		while(tm.trackedBlocks > 0)
		{
			kprintf("Popped block. addr: %d, size: %u\n", t.blkAddr, t.size );
			t = popMemRecord( &tm );
			c++;
		}
		// kprintf("t.blkAddr: %d\n", t.blkAddr );
		kprintf("\nPopped %d blocks. tm.trackedBlocks: %d\n", c, tm.trackedBlocks);

		// for(i = 0; i < RECORD_SIZE; i++)
		// 	kprintf("%d ", tm.blocks[i].blkAddr);
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
