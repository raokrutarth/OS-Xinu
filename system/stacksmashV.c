#include <xinu.h>

void stacksmashV(void)
{
	putc(stdout, 'V');
	sleepms(500);
	putc(stdout, 'V');
}