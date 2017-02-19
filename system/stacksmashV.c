#include <xinu.h>

void stacksmashV(void)
{
	putc(stdout, 'V');
	sleepms(200);
	putc(stdout, 'V');
}