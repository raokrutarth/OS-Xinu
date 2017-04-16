#include <xinu.h>


syscall regcallbacksig( int (* func) (void), uint16 ssig, uint32 oparg )
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	mask = disable();
	
	if( (ssig != XINUSIGRCV && ssig != XINUSIGXTIME && ssig != XINUSIGCHLD ) || func == 0)
	{
		// kprintf("ERR: ssig: %d, func: %d\n", ssig, func);
		return SYSERR;
	}
		
		
	if( ssig == XINUSIGXTIME && oparg < (uint32)1)
	{
		// kprintf("ERR: XINUSIGXTIME, func: %d, oparg: %u\n", ssig, func, oparg);
		return SYSERR;
	}

	prptr = &proctab[ currpid ];

	if( ssig == XINUSIGRCV)
	{		
		prptr->load_msg_callback = 1;
		prptr->msg_cb_func = func;
	}
	else if (ssig == XINUSIGXTIME )
	{
		prptr->wall_time_limit = oparg;
		prptr->wall_time_set = 1;
		prptr->wall_cb_func = func;
	}
	else if(ssig == XINUSIGCHLD)
	{
		prptr->monitor_child = 1;
		prptr->chld_cb_func = func;
	}	
	restore(mask);		/* Restore interrupts */
	return OK;
}

