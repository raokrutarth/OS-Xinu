#include <xinu.h>


long net2hostlasm(long tc)
{
	long x = tc;
	asm("mov %1, %0" 
	:"=a"(x) 
	:"r"(tc) ) ; //load parameter in EAX

	asm("bswap %eax"); // Swap bytes
	//asm("movl %ebp, %esp"); 
	return x;
	// asm ("movl %%1, %%eax; movl %%eax, %%0;" 
	// :"=r"(b) /* output */ 
	// :"r"(a) /* input */
	// :"%eax" /* clobbered register */);      
}