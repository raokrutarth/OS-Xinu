#include <xinu.h>

extern int32 msg_recv_cb(void);

int32 wTimeStatus(void)
{
	kprintf("[%d] Crossed wall time @ clktimefine: %u\n", getpid(), clktimefine );
	return OK;
}

int32 chldTerm(void)
{
	kprintf("[%d] callback executed. A child process was terminated\n", getpid() );
	return OK;
}

void dummyProc()
{
	kprintf("[%d] process created\n", getpid() );
	while(TRUE)
	{
		uint32 n = ~0;
		while( --n > 0 );
		sleepms(200);
	}
	return;
}
int32 chldStatus(void)
{
	kprintf("[%d] callback executed.\n", getpid() );
	pid32 dc = waitchld();
	kprintf("[%d] child with pid: %d terminated\n", getpid(), dc);
	return OK;
}

void c_a_p1()
{
	if (regcallbacksig(&wTimeStatus, XINUSIGXTIME, 5000) != OK) {
	      kprintf("wall time handler registration failed\n");
	      return;
	}
	while(TRUE)
	{
		sleepms(100);
		uint32 n = ~0;
		while( --n > 0 );
	}
	return;
}

void test1()
{
	kprintf("Test 1\n");
	pid32 p1;
	p1 = create(c_a_p1, 2000, 20, "a_p1", 0);
	kprintf("[%d] resuming process with pid: %d @ clktimefine: \n", getpid(), p1, clktimefine);
	resume(p1);
	sleep(20);
	kill(p1);
	kprintf("[%d] treminated proc: %d @ clktimefine: %u\n", getpid(), p1, clktimefine);
	return;
}


void c_b_p1()
{
	if (regcallbacksig(&msg_recv_cb, XINUSIGRCV, 5000) != OK) {
	      kprintf("wall time handler registration failed\n");
	      return;
	}
	sleepms(150);
	while(TRUE);
}

void test2()
{
	pid32 p1;
	p1 = create(c_b_p1, 2000, 20, "b_p1", 0);
	resume(p1);
	sleepms(150);
	kprintf("[%d] Sending 98 to proc: %d\n", getpid(), p1); 
	send(p1, 98);
	sleep(5);
	kill(p1);	
}

pid32 dp1, dp2, dp3;
void c_c_p1()
{
	if (regcallbacksig(&chldTerm, XINUSIGCHLD, -1) != OK) {
	      kprintf("wall time handler registration failed\n");
	      return;
	}
	kprintf("[%d] creating child processes\n", getpid() );
	dp1 = create(dummyProc, 500, 20, "dp1", 0) ;
	dp2 =  create(dummyProc, 500, 20, "dp2", 0) ;
	dp3 = create(dummyProc, 500, 20, "dp3", 0) ;
	resume(dp1);
	resume(dp2);
	resume(dp3);
	while(TRUE);
}

void test3()
{
	kprintf("Test3\n");
	pid32 p1;
	p1 = create(c_c_p1, 2000, 20, "c_p1", 0);
	resume(p1);	
	sleep(10);
	char * f = getmem(7650);
	kprintf("[%d] got block: %u\n",getpid() , f);
	kprintf("[%d] terminating child processes: %d, %d, %d\n", getpid(), dp1, dp2, dp3);

	kill(dp1);
	sleepms(200);
	kill(dp2);
	sleepms(200);
	kill(dp3);
	
}



	

void testSignal()
{
	kprintf("***	Testing signal handlers		***\n\n");
	
	// test1();
	// test2();
	test3();
	return;
}
