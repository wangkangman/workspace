/*	CONVERT INT AND LONG TO BUFFER
 *	Copyright (c) 1993 by COSMIC Software
 */
static const char digstr[] = {"0123456789abcdefghijklmnopqrstuvwxyz"};

#define ND (int)(sizeof(digstr) - 1)

/*	convert short integer
 */
int _itob(char *is, int val, signed char ibase)
	{
	char *s, *p;
	struct dv {
		unsigned int uval;
		unsigned char base;
		} v;
	char c;
	int _udiv(struct dv *);

	if (ibase < -ND || ND < ibase)
		return (0);
	v.base = (ibase < 0) ? -ibase : ibase;
	if (v.base <= 1)
		v.base = 10;
	s = is;
	if (ibase <= 0 && val < 0)
		{
		val = -val;
		*s++ = '-';
		}
	v.uval = val;
	p = s;
	do	{
		*s++ = digstr[_udiv(&v)];
		} while (v.uval);
	v.uval = s - is;
	while (p < --s)
		{
		c = *p;
		*p++ = *s;
		*s = c;
		}
	return (v.uval);
	}

/*	convert long integer
 */
int _ltob(char *is, long *pval, signed char ibase)
	{
	char *s, *p;
	struct dvl {
		unsigned long uval;
		unsigned char base;
		} v;
	char c;
	int _uldiv(struct dvl *);

	if (ibase < -ND || ND < ibase)
		return (0);
	v.base = (ibase < 0) ? -ibase : ibase;
	if (v.base <= 1)
		v.base = 10;
	s = is;
	if (*pval < 0 && ibase <= 0)
		{
		v.uval = -*pval;
		*s++ = '-';
		}
	else
		v.uval = *pval;
	p = s;
	do	{
		*s++ = digstr[_uldiv(&v)];
		} while (v.uval);
	v.base = s - is;
	while (p < --s)
		{
		c = *p;
		*p++ = *s;
		*s = c;
		}
	return (v.base);
	}
