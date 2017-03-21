#include <xinu.h>

void stacksmashV(void)
{
	putc(stdout, 'V');
	sleepms(800);
	putc(stdout, 'V');
}