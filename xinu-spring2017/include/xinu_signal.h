

#define   XINUSIGRCV   10
#define   XINUSIGXTIME   11
#define   XINUSIGCHLD   12

#define NO -1

// in file regcallbacksig.c
syscall regcallbacksig( int (* func) (void), uint16 ssig, uint32 oparg );

// in file regcallback.c
syscall regcallback( int (* func) (void) );


int waitchld(void);