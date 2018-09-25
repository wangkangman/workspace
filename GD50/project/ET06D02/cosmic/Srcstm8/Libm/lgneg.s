;	LONG NEGATE IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- arg and result pointed by X
;
	xdef	d_lgneg
	.dcall	"3,0,d_lgneg"
;
d_lgneg:
	cpl	(x)		; invert value
	cpl	(1,x)
	cpl	(2,x)
	neg	(3,x)		; negate LSB
	jrne	ok		; not nul, exit
	inc	(2,x)		; propagate carry
	jrne	ok
	inc	(1,x)
	jrne	ok
	inc	(x)
ok:
	retf			; and return
;
	end
