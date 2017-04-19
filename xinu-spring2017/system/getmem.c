/* getmem.c - getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;

	struct procent *ptcurr; // used for GC

	mask = disable();
	ptcurr = &proctab[currpid];

	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/

	prev = &memlist;
	curr = memlist.mnext;
	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;

			// track block for GC
			if(trackBlock( &(ptcurr->dmem),  (char *)(curr) , nbytes) != OK)
			{
				// kprintf("[getmem][%d] Unable to track allocated block\n", getpid() );
			}
			// else
				// kprintf("[getmem][%d] tracking block. blkaddr: %u\n", getpid(), curr );
			print_dmem( &(ptcurr->dmem) );

			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			// track block for GC
			if(trackBlock( &(ptcurr->dmem),  (char *)(curr) , nbytes) != OK)
			{
				// kprintf("[getmem][%d] Unable to track allocated block\n", getpid());
			}
			// else
				// kprintf("[getmem][%d] tracking block. blkaddr: %u\n",  getpid(), curr );
			// print_dmem( &(ptcurr->dmem) );

			restore(mask);
			return (char *)(curr);
		} else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	restore(mask);
	return (char *)SYSERR;
}
