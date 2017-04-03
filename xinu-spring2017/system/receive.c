/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	// no messages/processes waiting to be delivered
	if (prptr->prhasmsg == FALSE && prptr->empty_swq == 1) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	// queue is not empty and no immediate message available
	if(prptr->empty_swq == 0 && prptr->prhasmsg == FALSE) 
	{
		pid32 readFrom = dequeue(prptr->sw_queue);
		if(readFrom == EMPTY) // set empty flag if no more items left in queue
		{
			prptr->empty_swq = 1;
			prptr->prstate = PR_RECV;
			resched();
		}
		else // go to queue to find message
		{
			struct	procent * qp = &proctab[readFrom];
			msg = qp->sndwaitmsg;
			qp->sndflag = 0; /* Reset message flag	to let sending process know	*/
			prptr->prhasmsg = FALSE;
			// wake the process blocked on sendbk
			if (qp->prstate == PR_SNDWAIT) {
				unsleep(readFrom);
				ready(readFrom);
			}
			restore(mask);
		}
	}
	// immediate message is available and queue is empty
	if(prptr->empty_swq == 1 && prptr->prhasmsg == TRUE)
	{
		msg = prptr->prmsg;
		prptr->prhasmsg = FALSE;
		restore(mask);
	}
	return msg;
}
