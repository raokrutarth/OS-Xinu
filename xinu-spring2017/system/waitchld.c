#include <xinu.h>


pid32 waitchld()
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	pid32 dead_chld;			/* pid to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->child_pr_killed == NO) {
		prptr->prstate = WAITFORCHLD;
		resched();		/* Block until message arrives	*/
	}
	dead_chld = prptr->child_pr_killed;		/* Retrieve pid	*/
	prptr->child_pr_killed = NO; /* reset child killed flag */
	restore(mask);
	return dead_chld;
}