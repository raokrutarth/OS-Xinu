

#define   XINUSIGRCV   10
#define   XINUSIGXTIME   11
#define   XINUSIGCHLD   12


// in file regcallbacksig.c
syscall regcallbacksig( int (* func) (void), uint16 ssig, uint32 oparg );

int waitchld(void);
