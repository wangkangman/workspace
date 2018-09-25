/*	FORMATTED OUTPUT
 *	Copyright (c) 1995 by COSMIC Software
 */
#define NBARGS	5

struct arg {int args[NBARGS];};

int printf(char *fmt, void *args, struct arg dummy)
	{
	extern int _print(char **, char *, void **);

	return (_print((char **)0, fmt, &args));
	}
