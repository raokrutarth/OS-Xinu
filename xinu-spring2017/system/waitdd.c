#include <xinu.h>


/*------------------------------------------------------------------------
 *  waitdd  -  Cause current process to wait on a semaphore with deadlock 
 *  detection
 *------------------------------------------------------------------------
 */
syscall	waitdd( sid32		sem		/* Semaphore on which to wait  */ )
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	struct	sentry *semptr;		/* Ptr to sempahore table entry	*/

	mask = disable();
	if (isbadsem(sem)) {
		restore(mask);
		return SYSERR;
	}

	semptr = &semtab[sem];
	if (semptr->sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (--(semptr->scount) < 0) {		/* If caller must block	*/
		addEdge(resourceGraph, currpid, get_sem_vertex(sem) );		
		if( is_cycle(resourceGraph) == TRUE)
		{
			// check if a deadlock will occur			
			removeEdge(resourceGraph, currpid, get_sem_vertex(sem));
			return SYSERR_DD;
		}
		prptr = &proctab[currpid];
		prptr->prstate = PR_WAIT;	/* Set state to waiting	*/
		prptr->prsem = sem;		/* Record semaphore ID	*/
		enqueue(currpid,semptr->squeue);/* Enqueue on semaphore	*/
		// curr proc waiting on sem			
		resched();			/*   and reschedule	*/
	}
	// sem owned by curr proc
	addEdge(resourceGraph, get_sem_vertex(sem), currpid);
	if( is_cycle(resourceGraph) == TRUE)
	{
		removeEdge(resourceGraph, get_sem_vertex(sem), currpid);
		return SYSERR_DD;
	}
	restore(mask);
	return OK;
}






