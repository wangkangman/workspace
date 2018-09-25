/*	STRING TO DOUBLE CONVERSION
 *	Copyright (c) 1995 by COSMIC Software
 */
#define MAXD 32

double strtod(char *s, char **pe)
	{
	extern int _btod(char *, char, double *);
	double d;
	int n;

	n = _btod(s, MAXD, &d);
	if (pe)
		*pe = &s[n];
	return (d);
	}

