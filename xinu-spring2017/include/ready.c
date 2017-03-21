/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	// prptr->prprio = MAXKEY - prptr->prcpuused;
	// kprintf("[2] Inserting process %s, with prio: %u, cpuused: %d, MAXKEY: %u\n", prptr->prname, prptr->prprio, prptr->prcpuused, MAXKEY);
	// insert_new(pid, readylist, prptr->prcpuused);
	heapinsert(pid, prptr->prcpuused);
	total_ready_proc++; /* maintain # of entries in the readylist */
	
	resched();

	return OK;
}
