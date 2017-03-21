#include <xinu.h>

extern void takeover(void);
static unsigned long volatile	*esp;
unsigned long volatile	*sp;
extern void stacksmashV();

void stacksmashA()
{
	// kprintf("takeover() address: %X\n", takeover);
	// kprintf("sleepms() address: %X\n", sleepms);
	// kprintf("STACKMAGIC: %X\n", STACKMAGIC);
	// kprintf("stacksmashV address: %8X\n", stacksmashV);
	// kprintf("stacksmashA address: %8X\n", stacksmashA);

	int i = 0;
	asm("movl %esp,esp");
	sp = esp;

	// keep overriting stack wi address of takeover. 108 iterations works for the current setup.
	while( i < 108) 
	{
		//kprintf("[before] sp = %X, *sp = %X\n", sp, *sp);
		*sp = takeover;
		// kprintf("[after] sp = %X, *sp = %X\n", sp, *sp);
		sp--;
		i++;
	}
	return;
}


