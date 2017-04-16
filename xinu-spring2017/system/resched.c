/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

void checkWallCB(struct procent *ptcurr )
{
	intmask	mask;	
	mask = disable();
	if(currpid < 3 )
	{
		restore(mask);		
		return;
	}
		
	if(ptcurr->wall_time_set == 1) // XINUSIGXTIME registered
	{
		/* kprintf("[%d] clktimefine : %u, pr_start = %u, (clktimefine - ptcurr->pr_start) = %u\n", currpid,
					clktimefine, ptcurr->pr_start, (clktimefine - ptcurr->pr_start)  );
		kprintf("wall_time_limit: %u\n", ptcurr->wall_time_limit ); */
		if( (clktimefine - ptcurr->pr_start) > ptcurr->wall_time_limit )
		{
			// process has run for more than wall time for XINUSIGXTIME
			ptcurr->wall_cb_func() ;
			ptcurr->wall_time_set = 0; // reset the flag 
			restore(mask);		
			return;
		}
	}
	restore(mask);		
	return;
	
}

void checkCallback(struct procent *pte)
{
	intmask	mask;	
	mask = disable();
	struct procent *ptcurr;
	ptcurr = pte;	

	if(currpid < 3 )
	{
		restore(mask);		
		return; // null proc, rdld & main proc do not have calllback functionality
	}
	
	if( ptcurr->prhasmsg == TRUE)
	{
		// kprintf("[%d] Calling callback at address %d\n", currpid, ptcurr->callback_func);
		if( ptcurr->load_msg_callback == 1) // XINUSIGRCV registered
		{
			
			ptcurr->msg_cb_func() ;
			// ptcurr->load_callback = 0; // once executed, reset callback	
			// kprintf("[%d] returned from callback\n", currpid);
			restore(mask);		
			return;
		}						
		// else
		// 	kprintf("[%d] Callback function not set\n", currpid);
						
	}
	
	checkWallCB(ptcurr);
	
	if (ptcurr->monitor_child == 1) // XINUSIGCHLD registered
	{
		if(ptcurr->child_pr_killed > NO ) //
		{
			ptcurr->child_pr_killed = NO;
			ptcurr->chld_cb_func() ;
			restore(mask);		
			return;
		}
	}
	restore(mask);		/* Restore interrupts */
	return;
}
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

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			checkWallCB(ptold);
			return;
		}
		/* Old process will no longer remain current */
		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */
	// Callback checks
	checkCallback(ptold);
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
