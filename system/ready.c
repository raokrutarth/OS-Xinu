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
	if()
	prptr->prprio = MAXKEY - prptr->prcpuused;
	kprintf("[2] Inserting process %s, with prio: %u, cpuused: %d, MAXKEY: %u", prptr->prname, prptr->prprio, prptr->prcpuused, MAXKEY);
	insert(pid, readylist, prptr->prprio);
	resched();

	return OK;
}
