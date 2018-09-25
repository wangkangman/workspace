/*	FORMATTED PRINT
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DBUF		24
#define DEFPREC		6
#define ALTFORM		1
#define BLANK		2
#define DFLTPR		4
#define RJUST		8
#define SIGN		16
#define MODH		32
#define	MODL		64
#define OXP		128
#define INTNO		0
#define LNGNO		1
#define DBLNO		2
#define LDBLNO		3
#define TXTNO		4
#define SKIPNO		5
#define NOFLT		6
#define CHRNO		7
#define STXTNO		8

#define min(a, b)	((a) < (b) ? (a) : (b))

/*	for checking floating point
 *	this is a Float version of printf
 */
#define FPHARD	0
#define FPSOFT	1

/*	constant tables used for conversion
 */
static const signed char bnum[] = {-10, -10, 10, 8, 16, 16};
static const char keynum[] = {"diuoxX"};

/*	local function declarations
 */
static char _gnum(char **, int *, char ***, char *);
static void _cvtup(char *, int);
static void _putbuf(char **, char *, signed char);

#if FPHARD || FPSOFT
int _dtoe(char *, double *, char, char);
int _dtof(char *, double *, char, char);
int _dtog(char *, double *, char, char, char);
#endif

/*	convert string of max n length to upper case
 */
static void _cvtup(char *s, int n)
	{
	for (; *s && n; ++s, --n)
		*s = toupper(*s);
	}

/*	convert buffer 
 */
static char _gnum(char **qp, int *pnum, char ***ppp, char *pcfill)
	{
	char minus;
	char *q = *qp;
	int num;

	minus = 0;
	if (*q == '-')
		++minus, ++q;
	if (*q == '0')
		{
		if (pcfill)
			*pcfill = '0';
		++q;
		}
	else if (pcfill)
		*pcfill = ' ';
	if (*q == '*')
		{
		++q;
		num = *(int *)*ppp;
		*ppp = (char **)((int *)*ppp + 1);
		if (num < 0)
			++minus, num = -num;
		}
	else
		for (num = 0; isdigit(*q); )
			num = num * 10 + (char)(*q++ - '0');
	*pnum = num;
	*qp = q;
	return (minus);
	}

/*	output text or pad
 */
static void _putbuf(char **qp, char *s, signed char n)
	{
	char *p;

	if (qp)
		{
		p = *qp;
		if (n > 0)
			do
				*p++ = *s++;
			while (--n);
		else
			do
				*p++ = *s;
			while (++n);
		*p = '\0';
		*qp = p;
		}
	else
		{
		if (n > 0)
			do
				putchar(*s++);
			while (--n);
		else
			do
				putchar(*s);
			while (++n);
		}
	}

/*	internal print routine
 */
int _print(char **ps, char *f, char **pp)
	{
	char *q;
	char status, type;
	int i, n, prec, width, nchars;
	char f1, f2, cfill, *s;
	char buf[DBUF];

	for (nchars = 0; ; f = q + 1)
		{
		for (q = f; *q && *q != '%'; ++q)
			;
		if (i = q - f)
			{
			_putbuf(ps, f, i);
			nchars += i;
			}
		if (!*q++)
			return (nchars);
		status = DFLTPR|RJUST;
		type = INTNO;
		s = buf;
		prec = 0;
		for (;;)
			{
			if (*q == '-')
				status &= ~RJUST;
			else if (*q == '+')
				status |= SIGN;
			else if (*q == ' ')
				status |= BLANK;
			else if (*q == '#')
				status |= ALTFORM;
			else
				break;
			++q;
			}
		cfill = 0;
		if (_gnum(&q, &width, &pp, &cfill))
			status &= ~RJUST;
		if (*q == '.')
			{
			status &= ~DFLTPR;
			++q;
			if (_gnum(&q, &prec, &pp, NULL))
				prec = 0;
			}
		if (*q == 'l' || *q == 'L')
			status |= MODL, ++q;
		else if (*q == 'h')
			status |= MODH, ++q;
		switch (*q)
			{
		case 'c':
			*s = *(char *)pp;
			n = 1;
			type = CHRNO;
			break;
		case 'o':
		case 'x':
		case 'X':
			status |= OXP;
		case 'd':
		case 'i':
		case 'u':
			for (f1 = 0, f2 = *q; f2 != keynum[f1]; ++f1)
				;
			if (status & MODL)
				{
				n = _ltob(s, (long *)pp, bnum[f1]);
				type = LNGNO;
				}
			else
				n = _itob(s, *(int *)pp, bnum[f1]);
			if (*q == 'X')
				_cvtup(s, n);
			break;
#if !FPHARD && !FPSOFT
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			type = NOFLT;
			break;
#else
		case 'e':
		case 'E':
			f1 = (status & DFLTPR) ? DEFPREC : min(DBUF-7, prec);
			n = _dtoe(s, (double *)pp, 1, f1);
			if (*q == 'E')
				_cvtup(s, n);
			type = DBLNO;
			break;
		case 'f':
			f1 = (status & DFLTPR) ? DEFPREC : min(DBUF-3, prec);
			n = _dtof(s, (double *)pp, (DBUF-2) - f1, f1);
			type = DBLNO;
			break;
		case 'g':
		case 'G':
			f1 = (status & DFLTPR) ? DEFPREC : min(DBUF-7, prec);
			n = _dtog(s, (double *)pp, (DBUF-2) - f1, f1, (status & ALTFORM) ^ ALTFORM);
			if (*q == 'G')
				_cvtup(s, n);
			type = DBLNO;
			break;
#endif
		case 'n':
			**(int **)pp = nchars;
			n = width = prec = 0;
			type = TXTNO;
			break;
		case 'p':
			f1 = (status & ALTFORM) ? 16 : 10;
			status |= (RJUST|OXP);
			if (sizeof(int) < sizeof(char *) || status & MODL)
				{
				type = LNGNO;
				n = _ltob(s, (long *)pp, f1);
				}
			else
				n = _itob(s, *(int *)pp, f1);
			break;
		case 'S':
			f = *(char **)pp;
			if (prec)
				{
				for (n = 0; n < prec; ++n)
					if (!f[n])
						{
						prec = n;
						break;
						}
				}
			else
				n = strlen(f);
			s = NULL;
			type = TXTNO;
			break;
		case 's':
			s = *pp;
			if (prec)
				{
				for (n = 0; n < prec; ++n)
					if (!s[n])
						{
						prec = n;
						break;
						}
				}
			else
				n = strlen(s);
			if (!n)
				status &= ~RJUST;
			type = (sizeof(char *) == 1) ? STXTNO : TXTNO;
			break;
		default:
			*s = *q;
			n = 1;
			type = SKIPNO;
			}
		if (0 < n)
			{
			f1 = f2 = 0;
			if (*q == 'd' || *q == 'i' || type == DBLNO)
				{
				if (*s == '-')
					{
					f1 = *s++;
					--n;
					}
				else if (status & (SIGN|BLANK))
					f1 = (status & SIGN) ? '+' : ' ';
				if (f1)
					{
					--width;
					if (status & RJUST && cfill == '0')
						{
						_putbuf(ps, &f1, 1);
						f1 = 0;
						}
					}
				}
			else if (status & ALTFORM && status & OXP)
				{
				if (*q != 'o')
					{
					width -= 2;
					f1 = '0';
					f2 = (*q == 'p') ? 'x' : *q;
					}
				else if (*s != '0')
					{
					--width;
					f1 = '0';
					}
				}
			if (status & RJUST)
				{
				i = width - n;
				if (i > (width - prec))
					i = width - prec;
				if (i > 0)
					{
					_putbuf(ps, &cfill, -i);
					width -= i;
					nchars += i;
					}
				}
			while (f1)
				{
				_putbuf(ps, &f1, 1);
				++nchars;
				f1 = f2;
				f2 = 0;
				}
			if ((type != TXTNO && type != STXTNO) ||
				*q != 'p')
				cfill = '0';
			if (*q == 'g' || *q == 'G')
				;
			else if ((i = n - prec) < 0)
				{
				_putbuf(ps, &cfill, i);
				nchars -= i;	/* i negative */
				width += i;
				}
			if (0 < n)
				{
				_putbuf(ps, (s) ? s : f, n);
				nchars += n;
				}
			}
		if (!(status & RJUST))
			{
			cfill = ' ';
			i = n - width;
			if (i < 0)
				{
				_putbuf(ps, &cfill, i);
				width += i;
				nchars -= i;	/* i negative */
				}
			}
		if (type == CHRNO || type == STXTNO)
			pp = (char **)((char *)pp + 1);
		else if (type == INTNO || type == TXTNO)
			pp = (char **)((int *)pp + 1);
		else if (type == LNGNO)
			pp = (char **)((long *)pp + 1);
		else if (type != SKIPNO)
			pp = (char **)((double *)pp + 1);
		}
	}
