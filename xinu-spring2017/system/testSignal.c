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
	sleep(4);
	kprintf("[%d] terminating child processes: %d, %d, %d\n", getpid(), dp1, dp2, dp3);

	kill(dp1);
	sleepms(200);
	kill(dp2);
	sleepms(200);
	kill(dp3);

	sleep(2);
	kill(p1);
	
}

void testMem()
{
	char * f = getmem(23435);
	kprintf("[%d] got block %u\n", getpid(), f);

	char * g = getmem(999);
	kprintf("[%d] got block %u\n", getpid(), g);
	
	kprintf("[%d] freeing %u\n", getpid(), g);
	freemem(f, 23435);

	char * h = getmem(2222222);
	kprintf("[%d] got block %u\n", getpid(), h);
	
	kprintf("[%d] process exiting...\n");
}

void test99()
{
	kprintf("Test Bonus\n");
	
	pid32 p1;
	p1 = create(testMem, 2000, 20, "testMem", 0);
	resume(p1);
	sleep(3);
	kill(p1);
}

void c_d_p1()
{
	if (regcallbacksig(&chldTerm, XINUSIGCHLD, -1) != OK) {
	      kprintf("sig child registration failed\n");
	      return;
	}
	kprintf("[%d] sigchild registered\n", getpid() );
	
	if (regcallbacksig(&wTimeStatus, XINUSIGXTIME, 2500) != OK) {
	      kprintf("wall time handler registration failed\n");
	      return;
	}
	kprintf("[%d] sig wall registered\n", getpid() );
	
	if (regcallbacksig(&msg_recv_cb, XINUSIGRCV, -1) != OK) {
	      kprintf("SIG RECV registration failed\n");
	      return;
	}
	kprintf("[%d] sig recv registered\n", getpid() );
	
	dp1 = create(dummyProc, 2000, 20, "c_d_dp1", 0);
	resume(dp1);
	kprintf("[%d] child proc created & resumed\n", getpid() );
	
	while(1);
	
}

void test4()
{
	kprintf("Test4\n");
	
	resume( create(c_d_p1, 2000, 20, "c_d_p1", 0) );
	sleep(5);
	kprintf("[%d] killing p1's child\n", getpid() );
	kill(dp1);
	return;
}

int response;
int limit;
char init_complete =0;
char stop_server;
int completed_serving;
int nClients;

void init_server()
{
	kprintf("[init] starting server\n");
	response = 999;
	limit = 5;
	nClients = 0;
	stop_server = 0;	
	sleep(2);
	return;
}

void stop_sv()
{
	stop_server = 1;
}
int32 completedService()
{
	completed_serving++;
	return OK;
}

void serve_client()
{
	// intmask	mask;	
	// mask = disable();
	kprintf("[%d] Serving client..\n", getpid() );
	umsg32 msgbuf;
	msgbuf = receive();
	pid32 toSend = (pid32) msgbuf;
	// kprintf("[%d] Client server created.\nmsg: %u\n", getpid(), msgbuf);
	msgbuf = receive();
	send(toSend, response);
	
	// restore(mask);

	return;
}
int32 create_serving_proc()
{
	nClients++;
	resume( create(serve_client, 2000, 20, "serve_client",0 ) );
	return OK;
}

void client(pid32 serverAddr)
{
	sleepms(2000);
	kprintf("[%d] Client sending it's pid to server\n", getpid() );
	send(serverAddr, getpid() );
	umsg32 msgbuf;
	msgbuf = receive();
	kprintf("[%d] Server responded: %u\n", getpid(), msgbuf);
	while(1);
}

void server()
{
	pid32 init_proc = create(init_server, 2000, 20, "server_init",0 );
	// run an init() function to setup  the server to serve clients.
	resume( init_proc );
	pid32 dc = waitchld();

	// wait for init to complete
	if(dc == init_proc)
		kprintf("[%d] server_init complete\n", getpid() );

	// Setup server to recieve new clients and serve them
	if (regcallbacksig(&create_serving_proc, XINUSIGRCV, -1) != OK) {
	      kprintf("sig msg handler registration failed\n");
	      return;
	}

	// register callback to keep track of children that have terminated
	//  That is, the number of clients that have been served
	if (regcallbacksig(&completedService, XINUSIGCHLD, -1) != OK) {
	      kprintf("sig child registration failed\n");
	      return;
	}
	
	// // setup server to begin cleanup after 10 seconds
	// // Similar to timeout, the server starts to wait for the existing 
	// // clients to complete getting served.
	// // It then begins the cleanup routine.
	// if (regcallbacksig(&stop_sv, XINUSIGXTIME, 10000) != OK) {
	//       kprintf("wall time handler registration failed\n");
	//       return;
	// }
	// // busy wait till server-up time limit is reached
	// while(stop_server != 1);
	// kprintf("[%d] 10 seconds server up-time complete. Begin cleanup routine.\n", getpid() );
	
	// // wait till the clients completed serving = total clients
	// while( completed_serving != nClients);
	// kprintf("[%d] server completed serving clients. Terminating...\n", getpid() );
	return;
}

void test5()
{
	pid32 client_p, server_p;
	
	server_p = create(server, 2000, 20, "server", 0 );
	client_p = create(client, 2000, 20, "client", 1, server_p );

	resume(server_p);
	
	resume(client_p);
	sleep(20);
	kprintf("[%d] Killing server proc\n", getpid() );
	kill(server_p);
	kprintf("[%d] Killing client proc\n", getpid() );
	kill(client_p);
	return;
}
	

void testSignal()
{
	kprintf("***	Testing signal handlers		***\n\n");
	
	// test1();
	// test2();
	// test3();
	// test99();
	// test4();
	test5();
	return;
}
