#include <xinu.h>


/*------------------------------------------------------------------------
 *  takeover  -  inticates stack overflow attack success
 *------------------------------------------------------------------------
 */

void takeover( void )
{
	int i;
	for (i=0; i<6; i++) 
	{
		kprintf("takeover succeeded");
	}
	return;
}