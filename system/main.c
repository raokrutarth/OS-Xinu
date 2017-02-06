/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

void testConversion();
char app1(char ch);
int func1(int n);


static unsigned long volatile	*esp;
unsigned long volatile	*sp;
struct procent	*proc;
unsigned long volatile* proc_sp;

process	main(void)
{
	
	kprintf("\nRunning first XINU app and running function main() in system/main.c.\n");
	kprintf("initiated by nulluser() in system/initialize.c using create().\n");
	kprintf("\nCreating a second XINU app that runs shell() in shell/shell.c as an example.\n");
	// Modification to the welcome message
	kprintf("\nKrutarth Rao - raok - (0027262283)\n");
	
	testConversion();
	printsegaddr();

	/* printing ESP (top of stack) */
	kprintf("Printing top of stack frame (ESP) \n");
	
	asm("movl %esp,esp");
	sp = esp;
	kprintf("[1] Before create() : sp [%X] : %X\n", sp, *sp);

	/* second way to get ESP value */
	proc = &proctab[getpid()];
	proc_sp = (unsigned long *)proc->prstkptr;
	kprintf("[A]proctab->prstkptr before create(): sp [%X] : %X\n", proc_sp, *proc_sp);

	/* app1() created */
	pid32 ap1_id = create(app1, 1024, 20, "app1", 1, 'm');

	asm("movl %esp,esp");
	sp = esp;
	kprintf("[2a] After create() & before resume() : sp [%X] : %X\n", sp, *sp);
	proc = &proctab[getpid()];
	proc_sp = (unsigned long *)proc->prstkptr;
	kprintf("[B]proctab->prstkptr after create(): sp [%X] : %X\n", proc_sp, *proc_sp);

	/* app1() resumed */
	resume(ap1_id);

	asm volatile("movl %esp,esp");
	sp = esp;
	kprintf("[6a] After resume() : sp [%X] : %X\n", sp, *sp);

	/* second way to get ESP value */
	// struct procent	*proc = &proctab[getpid()];
	// sp = (unsigned long *)proc->prstkptr;
	// kprintf("[6b] After resume() : sp [%X] : %X\n", sp, *sp);
	// recvclr();
	// resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	/* Wait for shell to exit and recreate it */

	// while (TRUE) {
	// 	receive();
	// 	sleepms(200);
	// 	kprintf("\n\nMain process recreating shell\n\n");
	// 	resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	// }
	
	return OK;
}

char app1(char ch)
{
	
	/*printing to of stack */
	asm volatile ("movl %esp, esp");
	sp = esp;
	kprintf("[3] in app1() before func1() : sp [%X] : %X\n", sp, *sp);
	proc = &proctab[getpid()];
	proc_sp = (unsigned long *)proc->prstkptr;
	kprintf("[C]proctab->prstkptr before func1(): sp [%X] : %X\n", proc_sp, *proc_sp);


	int n = func1(5);

	asm volatile ("movl %esp, esp");
	sp = esp;
	kprintf("[5] after func1() returns: sp [%X] : %X\n", sp, *sp);
	
	return ch;
}
int func1(int n)
{
	asm volatile ("movl %esp, esp");
	sp = esp;
	kprintf("[4] in func1() before it returns : sp [%X] : %X\n", sp, *sp);

	return n;
}


void testConversion()
{
	kprintf("\n*** net2hostl test ***\n");
	long toConv = 0xFC8598;		
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
