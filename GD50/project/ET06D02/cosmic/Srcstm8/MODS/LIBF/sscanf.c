/*	FORMATTED READ FROM STRING
 *	Copyright (c) 2001 by COSMIC Software
 */
int sscanf(char *s, char *fmt, char *arg)
	{
	extern int _scan(char *, char *, void **);

	return (_scan(s, fmt, &arg));
	}
