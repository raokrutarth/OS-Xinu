#include <xinu.h>


/*
	A process registers a user callback function with the kernel 
	requesting that it be executed on behalf of the process when
	a 1-word message arrives 
*/

syscall regcallback( int (* func) (void) )
{

}