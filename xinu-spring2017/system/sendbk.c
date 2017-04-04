/* sendbk.c - sendbk */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  sendbk  -  Pass a message to a process and start recipient if waiting
 *				with timed blocking
 *------------------------------------------------------------------------
 */
syscall	sendbk(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg,		/* Contents of message		*/
	  int32 maxwait   		/* max time the send will wait to deliver the message */
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/

	mask = disable();
	if (isbadpid(pid)) 
	{
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[pid];
	if (prptr->prstate == PR_FREE ) 
	{
		restore(mask);
		return SYSERR;
	}
	if( maxwait == 0 || prptr->prhasmsg == FALSE) // overwrite or recipient buffer is empty
	{
		prptr->prmsg = msg;
		prptr->prhasmsg = TRUE;
		// If recipient waiting or in timed-wait make it ready 
		if (prptr->prstate == PR_RECV) 
		{
			ready(pid);
		} 
		else if (prptr->prstate == PR_RECTIM) 
		{
			unsleep(pid);
			ready(pid);
		}
		restore(mask);
		return OK;
	}
	else // need to enter the queue to wait for the recipient
	{
		struct	procent *pr_curr = &proctab[currpid];
		// put the current process in the sleep queue to 
		// wake up after maxwait time if maxtime is positive.
		if(maxwait > 0)
			insertd(currpid, sleepq, maxwait);
		// put the sending process in the send wait queue
		// for the receiving process.		
		add_to_queue(&(prptr->sw_queue), currpid );
		// kprintf("added %d to %d's swq(qid: %d)\n", currpid, pid, prptr->sw_queue);
		kprintf("added %d to %d's swq(qid: %p)\n", currpid, pid, &(prptr->sw_queue) );

		pr_curr->prstate = PR_SNDWAIT;
		pr_curr->sndwaitmsg = msg;
		pr_curr->sndflag = 1;
		resched();
		// process has returned after maxwait time. 
		// Check if receive consumed the message.
		if(pr_curr->sndflag != 0 && maxwait > 0)
		{
			restore(mask);
			return TIMEOUT;
		}
		else // maxwait is negative and message was not yet read
		{
			while(pr_curr->sndflag != 0)
			{
				resched(); // block indefinitely till reader resets flag
			}
		}
		restore(mask);
		return OK;	
	}
}


