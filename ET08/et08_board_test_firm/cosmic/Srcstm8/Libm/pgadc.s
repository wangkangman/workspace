;	FAR POINTER ADDITION IN MEMORY WITH BYTE
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in A
;
	xdef	d_pgadc
	.dcall	"3,0,d_pgadc"
;
d_pgadc:
	add	a,(2,x)		; add and
	ld	(2,x),a		; store result
	jrnc	ok		; no carry, exit
	inc	(1,x)		; propagate carry
	jrne	ok		; not zero, exit
	inc	(x)		; propagate carry
ok:
	retf			; and return
;
	end
