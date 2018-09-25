/*	BUFFER TO DOUBLE CONVERSION
 *	Copyright (c) 1995 by COSMIC Software
 */
double atof(char *nptr)
	{
	extern int _btod(char *, char, double *);
	double num;

	return (_btod(nptr, strlen(nptr), &num) ? num : 0.0);
	}
