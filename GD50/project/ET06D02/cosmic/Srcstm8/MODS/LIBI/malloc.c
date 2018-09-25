/*	HEAP MANAGEMENT FUNCTIONS
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <stdlib.h>

#define MAXCORE	512
#define SIZHDR	((unsigned int)&(((HEAP *)0)->x))
#define SIZPTR	(sizeof(char *))
#define MINELEM	(SIZHDR+SIZPTR)

#define max(a, b)	((a) > (b) ? (a) : (b))

/*	heap link structure
 */
typedef struct _heap
	{
	unsigned int hsize;
	union {
		double d;
		struct _heap *next;
		} x;
	} HEAP;

/*	memory allocation variables
 */
static unsigned int _hunk = 0;
static void *_hbot = NULL;
static void *_htop = NULL;
void *_heap = NULL;

/*	free arb chunk forcing it to be acceptable
 *	keep track of holes between chunks here later
 */
static void _addheap(HEAP *h, unsigned int n)
	{
	if (n < MINELEM)
		return;
	h->hsize = n - MINELEM;
	if (!_hbot || h < (HEAP *)_hbot)
		_hbot =  h;
	if ((HEAP *)_htop < (HEAP *)((char *)h + n))
		_htop = ((char *)h + n);
	free(&h->x.next);
	}

/*	break off a cell
 */
HEAP *_breakup(HEAP *p, unsigned int size)
	{
	HEAP *q = (HEAP *)((char *)p + size);

	q->hsize = p->hsize - size;
	q->x.next = p->x.next;
	p->hsize = size - MINELEM;
	return (q);
	}

/*	merge a cell with the next if contiguous
 */
static void _hmerge(HEAP *p, HEAP *q)
	{
	HEAP *ph = (HEAP *)((char *)p + p->hsize + MINELEM);

	if (q < ph && p < (HEAP *)((char *)q + q->hsize + MINELEM))
		;
	else if (ph == q)
		{
		p->hsize += q->hsize + MINELEM;
		p->x.next = q->x.next;
		}
	else
		p->x.next = q;
	}

/*	add this buffer to the free list without complaint
 */
void free(void *addr)
	{
	HEAP *o, *p, *q;

	p = (HEAP *)((char *)addr - SIZHDR);
	if (!_heap)
		{
		p->x.next = NULL;
		_heap = p;
		}
	else if (p < (HEAP *)_heap)
		{
		_hmerge(p, _heap);
		_heap = p;
		}
	else
		{
		for (o = (HEAP *)_heap; (q = o->x.next) && q < p; o = q)
			;
		if (q)
			_hmerge(p, q);
		else
			p->x.next = NULL;
		_hmerge(o, p);
		}
	}

/*	allocate need bytes
 */
void *malloc(unsigned int need)
	{
	extern HEAP *sbreak();
	HEAP **qb, *q;
	unsigned int n;

	need = ((max(need, SIZPTR) + (SIZHDR-1)) & ~(SIZHDR-1)) + SIZHDR;
	for (;;)
		{
		for (qb = (HEAP **)&_heap; q = *qb; qb = &q->x.next)
			if (need <= q->hsize + MINELEM)
				{
				*qb = (q->hsize < need) ? q->x.next :
					_breakup(q, need);
				return (&q->x.next);
				}
		if (!_hunk)
			_hunk = MAXCORE;
		else if (_hunk != MAXCORE)
			_hunk >>= 1;
		while (need <= _hunk && !(q = sbreak(n = _hunk)))
			_hunk >>= 1;
		if (!q && !(q = sbreak(n = need)))
			return (NULL);
		_addheap(q, n);
		}
	}
