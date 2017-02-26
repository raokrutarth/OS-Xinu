/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	if( ptold->prcpuused != MAXKEY) //null proc has MAXKEY as cpuused. keep it that way to avoid overflow
	{
		/* add the the CPU time for which the outgoing process ran */ 
		ptold->prcpuused += (clktimefine - ptold->prctxswstart);
		//  /* new process priority is lower if cpu used is greater */
		// ptold->prprio = MAXKEY - ptold->prcpuused;
	}	

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */

		// kprintf("[1] comparing prio_old (%s): %u, prio_list : %u\n", ptold->prname, ptold->prprio, firstkey(readylist) );
		if ( ptold->prcpuused < firstkey(readylist) ) {
			return;
		}
		/* Old process will no longer remain current */
		ptold->prstate = PR_READY;	
		
		// kprintf("null proc cpu used: %d", ptold->prcpuused);
		// kprintf("[1] Inserting process %s, with prio: %d\n", ptold->prname, ptold->prprio);	
		// insert(currpid, readylist, ptold->prprio);
		insert_new(currpid, readylist, ptold->prcpuused);
	}

	/* Force context switch to highest priority ready process */
	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	
	/* remember current time since boot */
	ptnew->prctxswstart = clktimefine;
	
	// kprintf("[1] Switching (@ clktimefine: %u) to process %s, with prio: %d\n", clktimefine, ptnew->prname, ptnew->prprio);	
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */
	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
