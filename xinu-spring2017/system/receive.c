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
	
	if(prptr->prhasmsg == FALSE)
	{

		pid32 in_q = read_from_queue(& (prptr->sw_queue) );
		if(in_q != EMPTY) // processes blocked to send to this process
		{
			struct	procent * qp = &proctab[in_q];
			// kprintf("read %d's message from %d's swq\n", in_q, currpid);
			msg = qp->sndwaitmsg;
			qp->sndflag = 0; /* Reset message flag	to let sending process know the message was read */
			// wake the process blocked on sendbk
			if (qp->prstate == PR_SNDWAIT) {
				unsleep(in_q);
				ready(in_q);
			}		
		}
	}
	else
	{
		// prptr->empty_swq = 1; /* the waiting queue is empty */
		// immediate message is available and queue is empty
		if(prptr->prhasmsg == TRUE)
		{
			// kprintf("%d's swq(qid: %p) empty. reading from buffer\n", currpid, &(prptr->sw_queue) );
			msg = prptr->prmsg;
			prptr->prhasmsg = FALSE;			
		}
		else
		{
			// no messages/processes waiting to be delivered
			// kprintf("%d's swq(qid: %p) empty. empty buffer going to PR_RECV state\n", currpid );
			prptr->prstate = PR_RECV;
			resched();
			if(prptr->prhasmsg == TRUE)
			{
				msg = prptr->prmsg;
				prptr->prhasmsg = FALSE;
			}			
			// kprintf("%d returning from PR_RECV state. msg: %u\n", currpid, msg );
		}
	}
	restore(mask);	
	return msg;
}

// 
// 		
// 		
