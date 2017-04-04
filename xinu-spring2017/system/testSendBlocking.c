#include <xinu.h>


void s_p1(pid32 toSend)
{
	umsg32 msg = (pid32)getpid();
	kprintf("[s_p1] sending p1's pid (%d) to proc %d\n", msg, toSend);
	
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	int res = sendbk(toSend, msg, 100);
	kprintf("pid: %d clktimefine: %u\n", getpid(), clktimefine);
	
	if(res == TIMEOUT)
		kprintf("[p2] TIMEOUT\n", res);
	else
		kprintf("[p2] sent! \n");
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
	kprintf("[process %d] sending buffer filling msg (88) to %d\n", getpid(), msg, toSend);
	send(toSend, 88);
	
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
	kprintf("[process %d] got message %d\n", getpid(), msg);
	sleepms(500);	
	msg = receive();
	kprintf("[process %d] got message %d\n", getpid(), msg);
	return;
}
void test2()
{
	pid32 s_id2 = create(b_p2, 515, 20, "p2", 0);
	pid32 s_id1 = create(b_p1, 515, 20, "p1", 1, s_id2);
	resume(s_id1);
	resume(s_id2);
	sleep(4);
	kill(s_id1);
	kill(s_id2);
}

void testSendBlocking()
{
	// test1();
	test2();
}