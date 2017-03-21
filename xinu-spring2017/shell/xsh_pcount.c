/* xsh_pcount.c - xsh_pcount */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_pcount - shell command to print the no. of processes
 *------------------------------------------------------------------------
 */
shellcmd xsh_pcount(int nargs, char *args[])
{
	struct	procent	*prptr;		/* pointer to process		*/
	int32	i;			/* index into proctabl		*/
	
	/* For argument '--help', emit help about the 'ps' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays the no. of running processes\n");
		printf("Options:\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 1) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}

	/* Print first line : */
	printf("No. of active processes: ");

	/* Output information for each process */
	int32 count = 0;
	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		if (prptr->prstate == PR_FREE) {  /* skip unused slots	*/
			continue;
		}
		count++;
	}
	printf("%d\n", count);
	return 0;
}
