;	LONG SUBSTRACT IN MEMORY WITH BYTE
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in A
;
	xdef	d_lgsbc
	.dcall	"3,0,d_lgsbc"
;
d_lgsbc:
	neg	a		; invert input
	add	a,(3,x)		; start with LSB
	ld	(3,x),a		; store result
	jrc	ok		; carry, exit
	ld	a,(2,x)		; continue
	dec	(2,x)		; with next byte
	tnz	a		; check value
	jrne	ok		; not zero, exit
	ld	a,(1,x)		; continue
	jrne	okl		; not zero, skip last
	dec	(x)		; decrement last
okl:
	dec	(1,x)		; propagate
ok:
	retf			; and return
;
	end
