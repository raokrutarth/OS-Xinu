/* signaldd.c - signaldd */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  signaldd  -  Signal a semaphore, releasing a process if one is waiting
 *	with deadlock detection
 *------------------------------------------------------------------------
 */
syscall	signaldd(
	  sid32		sem		/* ID of semaphore to signal	*/
	)
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	sentry *semptr;		/* Ptr to sempahore table entry	*/

	mask = disable();
	if (isbadsem(sem)) {
		restore(mask);
		return SYSERR;
	}
	semptr= &semtab[sem];
	if (semptr->sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}
	//sem not held by process anymore.
	removeEdge(resourceGraph, get_sem_vertex(sem), currpid); 

	if ((semptr->scount++) < 0) {	/* Release a waiting process */
		pid32 toReady = dequeue(semptr->squeue);

		// toRem proc not waiting on sem anymore
		removeEdge(resourceGraph, toReady, get_sem_vertex(sem) );
		ready(toReady);		
	}
	restore(mask);
	return OK;
}