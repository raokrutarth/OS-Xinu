/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

void testConversion()
{
	kprintf("\n*** net2hostl test ***\n");
	long toConv = 0xFC85;		
	kprintf("toConvert = %X\n", toConv);
	
	long a = toConv;
	a = net2hostl(a); //make call to assembly function
	kprintf("After bswap = %X\n", a);
	
	long b = toConv;
	b = net2hostlgcc(b);
	kprintf("After net2hostlgcc = %X \n", b); 
	
	long c = toConv;
	c = net2hostlasm(c);
	kprintf("After net2hostlasm = %X", c);
	kprintf("\n");
}

process	main(void)
{
	
	kprintf("\nRunning first XINU app and running function main() in system/main.c.\n");
	kprintf("initiated by nulluser() in system/initialize.c using create().\n");
	kprintf("\nCreating a second XINU app that runs shell() in shell/shell.c as an example.\n");
	// Modification to the welcome message
	kprintf("\nKrutarth Rao - raok - (0027262283)\n");
	
	testConversion();
	printsegaddr();

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	
	return OK;
}
