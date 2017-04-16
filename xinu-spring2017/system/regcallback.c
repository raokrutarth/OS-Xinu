#include <xinu.h>


// For problem 3 only. Use regcallbacksig() for grading

syscall regcallback( int (* func) (void)  )
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	mask = disable();
	
	prptr = &proctab[ currpid ];
	
	prptr->msg_cb_func = func;
	prptr->load_msg_callback = 1;
	
	restore(mask);		/* Restore interrupts */
	return OK;
}

