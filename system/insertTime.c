/* insert.c - insert */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  insert_new  -  Insert a process into a queue in ascending cputime order
 *------------------------------------------------------------------------
 */
status	insert_new(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* Key for the inserted process	*/
	)
{
	int16	curr;			/* Runs through items in a queue*/
	int16	prev;			/* Holds previous node index	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}
	
	int x = MAXKEY - key;
	
	//kprintf("[+] 1\n");
	curr = firstid(q);
	
	while (  queuetab[curr].qkey >= x) {
		curr = queuetab[curr].qnext;
	}
	//kprintf("[+] 2\n");
	/* Insert process between curr node and previous node */

	prev = queuetab[curr].qprev;	/* Get index of previous node	*/
	queuetab[pid].qnext = curr;
	queuetab[pid].qprev = prev;
	queuetab[pid].qkey = key;
	queuetab[prev].qnext = pid;
	queuetab[curr].qprev = pid;
	return OK;
}