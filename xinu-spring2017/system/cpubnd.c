#include <xinu.h>


/*------------------------------------------------------------------------
 *  cpubnd  -  CPU bound process to test dynamic scheduling
 *------------------------------------------------------------------------
 */

void cpubnd()
{
	uint32 LOOP1 = 999999999;
	uint32 LOOP2 = 5085884; 
	uint32 i, j;
	for (i=0; i < LOOP1; i++) 
	{
		for (j=0; j<LOOP2; j++) 
		{
			// Insert code that performs memory copy (slow) and/or
			// ALU operations (fast).
			int res = i+j*i*j;
			// Note: this loop consumes significant CPU cycles.
		}
		// Using kprintf, print the pid followed by the outer loop count i,		
		// the process's priority, and the remaining time slice (preempt).
		// struct	procent	*prptr;
		// prptr = &proctab[currpid];
		// kprintf("currpid: %d, for_i : %d, priority(cpuused): %u, preempt: %d\n", currpid, i, prptr->prcpuused, preempt);
	}
	// Print the CPU time consumed by the process that is recorded in the
	// prcpuused field of the current process's process table entry.
	return;
}