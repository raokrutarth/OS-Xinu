/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/
	static	uint32	count1 = 1;	/* Count to 1 ms	*/

	/* Decrement the ms counter, and see if a second has passed */

	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */
		clktime++;
		/* Reset the local ms counter for the next second */
		count1000 = 1000;
	}
	if((--count1) <= 0) 
	{
		/* One millisecond has passed, so increment seconds count */
		clktimefine++;
		/* Reset the local ms counter for the next second */
		count1 = 1;
	}
	/* Handle sleeping processes if any exist */
	if(!isempty(sleepq)) 
	{
		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/
		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();			
		}
	}
	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */
	if((--preempt) <= 0) {
		preempt = QUANTUM;
		resched();
	}
}
