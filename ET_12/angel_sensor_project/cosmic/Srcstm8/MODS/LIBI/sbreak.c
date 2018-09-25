/*	SET SYSTEM BREAK
 *	Copyright (c) 2001 by COSMIC Software
 */
#include <stdlib.h>

void *sbreak(unsigned int size)
	{
	extern @near char _memory, _endmem;
	static char *_brk = NULL;	/* memory break */
	char *obrk;

	if (!_brk)
		_brk = &_memory;
	obrk = _brk;
	_brk += size;
	if (&_endmem <= _brk || _brk < &_memory)
		{
		_brk = obrk;
		return (NULL);
		}
	return (obrk);
	}
