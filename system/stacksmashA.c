#include <xinu.h>

int weapon(int n)
{
	if(n <1)
		return 3;
	else
		return weapon(n-1) + weapon(n-2) + weapon(n-3);
}


void stacksmashA()
{
	int err = weapon(100);
	return;
}


