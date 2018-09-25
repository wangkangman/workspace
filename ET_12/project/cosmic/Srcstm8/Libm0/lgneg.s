;	LONG NEGATE IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- arg and result pointed by X
;
	xdef	c_lgneg
	.dcall	"2,0,c_lgneg"
;
c_lgneg:
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
	ret			; and return
;
	end
