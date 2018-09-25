/*	FORMATTED OUTPUT TO INTERNAL BUFFER
 *	Copyright (c) 1995 by COSMIC Software
 */
#define NBARGS 3

struct arg {int args[NBARGS];};

int sprintf(char *s, char *fmt, void *args, struct arg dummy)
	{
	extern int _print(char **, char *, void **);

	*s = '\0';
	return (_print(&s, fmt, &args));
	}
