/*	FORMATTED READ FROM INPUT
 *	Copyright (c) 2001 by COSMIC Software
 */
int scanf(char *fmt, void *arg)
	{
	extern int _scan(char *, char *, void **);

	return (_scan(0, fmt, &arg));
	}
