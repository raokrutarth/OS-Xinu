#include <xinu.h>

int32 msg_recv_cb(void) 
{
	umsg32 msgbuf;
	msgbuf = receive();
	kprintf("[%d] callback executed.\nmsg: %u\n", getpid(), msgbuf);
	return(OK);
}


int a_p1()
{
	if ( regcallback(&msg_recv_cb ) != OK) {
		kprintf("[p1] callback function registration failed\n");
		return 1;
	}
	kprintf("[%d] callback set!\n", getpid() );
	sleepms(500);	
	while(TRUE);
	return 0;
}
int a_p2(pid32 r)
{
	umsg32 msg = 100;
	kprintf("[%d] sent %d to %d\n", getpid(), msg, r);
	int err = send( r,	msg);
	if (err != OK)
		kprintf("[p2] message not sent\n");	
	return 0;
}

int c_test1()
{
	kprintf("Test 1\n");
	
	pid32 p1, p2;
	p1 = create(a_p1, 2000, 20, "a_p1", 0);	
	p2 = create(a_p2, 2000, 20, "a_p2", 1, p1);
	resume( p1 );
	resume( p2 );
	sleep(4);
	kill(p1);
	// p2 returns
	return 0;
}

int b_p1()
{
	if ( regcallback(&msg_recv_cb ) != OK) {
		kprintf("[p1] callback function registration failed\n");
		return 1;
	}
	kprintf("[%d] callback set!\n", getpid() );
	// sleepms(4000);	
	while(1 );
	return 0;
}
int b_p2(pid32 r)
{
	umsg32 msg = 100;
	kprintf("[%d] sent %d to %d\n", getpid(), msg, r);
	if (send( r,	msg) != OK)
		kprintf("[p2] message not sent\n");
	sleep( 1 );

	msg = 200;
	kprintf("[%d] sent %d to %d\n", getpid(), msg, r);	
	if ( send( r,	msg) != OK)
		kprintf("[p2] message not sent\n");
	sleep( 1 );

	msg = 300;
	kprintf("[%d] sent %d to %d\n", getpid(), msg, r);
	if ( send( r,	msg) != OK)
		kprintf("[p2] message not sent\n");	
	sleep( 1 );


	return 0;
}


int c_test2()
{
	kprintf("Test 2\n");
	
	pid32 p1, p2;
	p1 = create(b_p1, 2000, 20, "b_p1", 0);	
	p2 = create(b_p2, 2000, 20, "b_p2", 1, p1);
	resume( p1 );
	resume( p2 );
	sleep(20);
	kill(p1);
	kprintf("[%d] killed\n", p1);
	// p2 returns
	return 0; 
}

int c_p1()
{
	if ( regcallback(&msg_recv_cb ) != OK) {
		kprintf("[p1] callback function registration failed\n");
		return 1;
	}
	kprintf("[%d] callback set!\n", getpid() );
	// sleepms(4000);	
	while(1 );
	return 0;
}

int c_p2(pid32 r)
{
	umsg32 msg = 900;
	kprintf("[%d] sent %d to %d\n", getpid(), msg, r);
	if (send( r,	msg) != OK)
		kprintf("[p2] message not sent\n");
	sleep( 1 );

	msg = 800;
	kprintf("[%d] sent %d to %d\n", getpid(), msg, r);	
	if ( send( r,	msg) != OK)
		kprintf("[p2] message not sent\n");
	sleep( 1 );

	return 0;
}

int c_p3(pid32 r)
{
	umsg32 msg = 100;
	kprintf("[%d] sent %d to %d\n", getpid(), msg, r);
	if (send( r,	msg) != OK)
		kprintf("[p2] message not sent\n");
	sleep( 1 );

	msg = 200;
	kprintf("[%d] sent %d to %d\n", getpid(), msg, r);	
	if ( send( r,	msg) != OK)
		kprintf("[p2] message not sent\n");
	sleep( 1 );

	return 0;
}

int c_test3()
{
	kprintf("Test 3\n");
	
	pid32 p1, p2, p3;
	p1 = create(c_p1, 2000, 20, "c_p1", 0);	
	p2 = create(c_p2, 2000, 20, "c_p2", 1, p1);
	p3 = create(c_p3, 2000, 20, "c_p3", 1, p1);
	resume( p1 );
	resume( p2 );
	resume( p3 );
	sleep(10);
	kill(p1);
	kprintf("[%d] killed\n", p1);
	// p2 returns
	return 0; 
}

void testCallback()
{
	kprintf("***	Testing Callbacks	***\n\n");
	c_test1();
	c_test2();
	c_test3();
}
