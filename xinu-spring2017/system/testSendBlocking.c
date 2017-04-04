#include <xinu.h>


void s_p1(pid32 toSend)
{
	umsg32 msg = (pid32)getpid();
	kprintf("[s_p1] sending p1's pid (%d) to proc %d\n", msg, toSend);
	
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	int res = sendbk(toSend, msg, (umsg32)100);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	if(res == TIMEOUT)
		kprintf("[p1] TIMEOUT\n");
	else
		kprintf("[p1] sent! \n");
	return;
}

void s_p2()
{
	umsg32 msg = receive();
	kprintf("[s_p2] got message %d\n", msg);	
	return;
}

void test1()
{
	kprintf("***testSendBlocking test 1***\n");
	pid32 s_id2 = create(s_p2, 515, 20, "sw_p2", 0);
	pid32 s_id1 = create(s_p1, 515, 20, "sw_p1", 1, s_id2);
	resume(s_id1);
	resume(s_id2);
	sleep(3);
	kill(s_id1);
	kill(s_id2);
}


void b_p1(pid32 toSend)
{
	umsg32 msg = (pid32)getpid();
	kprintf("[process %d] sending buffer filling msg to %d\n", getpid(), toSend);
	send(toSend, 266271924);
	
	kprintf("[process %d] sending p1's pid (%d) to proc %d\n", getpid(), msg, toSend);
	
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	int res = sendbk(toSend, msg, 1000);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	if(res == TIMEOUT)
		kprintf("[p2] TIMEOUT\n", res);
	else
		kprintf("[p2] sent! \n");
	return;
}

void b_p2()
{
	umsg32 msg = receive();
	kprintf("[process %d] got message %u\n", getpid(), msg);
	sleepms(500);	
	msg = receive();
	kprintf("[process %d] got message %u\n", getpid(), msg);
	return;
}
void test2()
{
	kprintf("***testSendBlocking test 2***\n");
	pid32 s_id2 = create(b_p2, 515, 20, "p2", 0);
	pid32 s_id1 = create(b_p1, 515, 20, "p1", 1, s_id2);
	resume(s_id1);
	resume(s_id2);
	sleep(4);
	kill(s_id1);
	kill(s_id2);
}


void c_p1(pid32 toSend)
{
	umsg32 msg = (pid32)getpid();
	kprintf("[process %d] sending buffer filling msg to %d\n", getpid(), toSend);
	send(toSend, 266271924);
	
	kprintf("[process %d] sending p1's pid (%d) to proc %d\n", getpid(), msg, toSend);
	
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	int res = sendbk(toSend, msg, 200);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	if(res == TIMEOUT)
		kprintf("[p2] TIMEOUT\n", res);
	else
		kprintf("[p2] sent! \n");
	return;
}

void c_p2()
{
	umsg32 msg = receive();
	kprintf("[process %d] got message %u\n", getpid(), msg);
	sleepms(1000);	
	msg = receive();
	kprintf("[process %d] got message %u\n", getpid(), msg);
	return;
}
void test3()
{
	kprintf("***testSendBlocking test 3***\n");

	pid32 s_id2 = create(c_p2, 515, 20, "p2", 0);
	pid32 s_id1 = create(c_p1, 515, 20, "p1", 1, s_id2);
	resume(s_id1);
	resume(s_id2);
	sleep(4);
	kill(s_id1);
	kill(s_id2);
}

void d_p1(pid32 toSend)
{
	umsg32 msg = (pid32)getpid();
	kprintf("[process %d] sending buffer filling msg to %d\n", getpid(), toSend);
	send(toSend, 266271924);
	
	kprintf("[process %d] sending it's pid (%d) to proc %d\n", getpid(), msg, toSend);
	
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	int res = sendbk(toSend, msg, -1);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	if(res == TIMEOUT)
		kprintf("[p2] TIMEOUT\n", res);
	else
		kprintf("[p2] sent! \n");
	return;
}
void d_p3(pid32 toSend)
{
	umsg32 msg = (pid32)getpid();

	kprintf("[process %d] sending it's pid (%d) to proc %d\n", getpid(), msg, toSend);
	
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	int res = sendbk(toSend, msg, -1);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	if(res == TIMEOUT)
		kprintf("[p2] TIMEOUT\n", res);
	else
		kprintf("[p2] sent! \n");
	return;
}

void d_p2()
{
	
	while(TRUE)
		sleepms(500);	
	umsg32 msg = receive();
	kprintf("[process %d] got message %u\n", getpid(), msg);
	return;
}
void test4()
{
	kprintf("***testSendBlocking test 4***\n");

	pid32 s_id2 = create(d_p2, 515, 20, "p2", 0);
	pid32 s_id1 = create(d_p1, 515, 20, "p1", 1, s_id2);
	pid32 s_id3 = create(d_p3, 515, 20, "p3", 1, s_id2);
	resume(s_id1);
	resume(s_id3);
	resume(s_id2);
	sleep(5);
	struct	procent *prptr;
	prptr = &proctab[s_id2];
	int value = read_from_queue(&(prptr->sw_queue) );
	while (EMPTY != value )
	{
		kprintf("dequed %d from recipient sw_queue\n", value);
		value = read_from_queue( &(prptr->sw_queue));
	}
	kill(s_id1);
	kill(s_id2);
	kill(s_id3);
}

void e_p1(pid32 toSend)
{
	umsg32 msg = (pid32)getpid();
	kprintf("[process %d] sending buffer filling msg to %d\n", getpid(), toSend);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	int res = send(toSend, 989587);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	sleepms(150);

	kprintf("[process %d] sending buffer overwriting msg to %d\n", getpid(), toSend);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	res = sendbk(toSend, msg, 0);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	if(res == TIMEOUT)
		kprintf("[p1] TIMEOUT\n", res);
	else
		kprintf("[p1] sent! \n");
	return;
}
void e_p3(pid32 toSend)
{
	umsg32 msg = (pid32)getpid();

	kprintf("[process %d] sending it's pid (%d) to proc %d with enough time to wait\n", getpid(), msg, toSend);
	
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	int res = sendbk(toSend, msg, 5000);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	if(res == TIMEOUT)
		kprintf("[p3] TIMEOUT\n", res);
	else
		kprintf("[p3] sent! \n");
	return;
}
void e_p4(pid32 toSend)
{
	umsg32 msg = (pid32)getpid();

	kprintf("[process %d] sending it's pid (%d) to proc %d WITHOUT enough time to wait\n", getpid(), msg, toSend);
	
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	int res = sendbk(toSend, msg, 1);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	if(res == TIMEOUT)
		kprintf("[p4] TIMEOUT\n", res);
	else
		kprintf("[p4] sent! \n");
	return;
}

void e_p2()
{
	umsg32 msg = receive();
	kprintf("[process %d] got message %u\n", getpid(), msg);

	sleepms(1500);

	msg = receive();
	kprintf("[process %d] got message %u\n", getpid(), msg);

	sleepms(1500);

	msg = receive();
	kprintf("[process %d] got message %u\n", getpid(), msg);

	sleepms(1500);
	msg = receive();
	kprintf("[process %d] got message %u\n", getpid(), msg);
	return;
}
void test5()
{
	kprintf("***testSendBlocking test 5***\n");
	
	pid32 s_id2 = create(e_p2, 515, 20, "p2", 0);
	pid32 s_id1 = create(e_p1, 515, 20, "p1", 1, s_id2);
	pid32 s_id3 = create(e_p3, 515, 20, "p3", 1, s_id2);
	pid32 s_id4 = create(e_p4, 515, 20, "p4", 1, s_id2);
	resume(s_id1);
	resume(s_id3);
	resume(s_id4);
	resume(s_id2);
	sleep(5);

	kill(s_id1);
	kill(s_id2);
	kill(s_id3);
	kill(s_id4);
}
void testSendBlocking()
{
	test1();
	test2();
	test3();
	test4();
	test5();
}