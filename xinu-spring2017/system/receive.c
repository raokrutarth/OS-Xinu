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
		
	pid32 in_q = read_from_queue(& (prptr->sw_queue) );
	if(in_q != EMPTY) // processes blocked to send to this process
	{
		struct	procent * qp = &proctab[in_q];
		kprintf("read %d's message from %d's swq\n", in_q, currpid);
		msg = qp->sndwaitmsg;
		qp->sndflag = 0; /* Reset message flag	to let sending process know the message was read */
		// wake the process blocked on sendbk
		if (qp->prstate == PR_SNDWAIT) {
			unsleep(in_q);
			ready(in_q);
		}
		restore(mask);
	}
	else
	{
		kprintf("%d's swq(qid: %p) empty. reading from buffer\n", currpid, &(prptr->sw_queue) );
		// prptr->empty_swq = 1; /* the waiting queue is empty */
		// immediate message is available and queue is empty
		if(prptr->prhasmsg == TRUE)
		{
			msg = prptr->prmsg;
			prptr->prhasmsg = FALSE;
			restore(mask);
		}
		else
		{
			// no messages/processes waiting to be delivered
			prptr->prstate = PR_RECV;
			resched();
		}
	}	
	return msg;
}

// 
// 		
// 		