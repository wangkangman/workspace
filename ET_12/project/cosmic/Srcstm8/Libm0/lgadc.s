;	LONG ADDITION IN MEMORY WITH BYTE
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in A
;
	xdef	c_lgadc
	.dcall	"2,0,c_lgadc"
;
c_lgadc:
	add	a,(3,x)		; add and
	ld	(3,x),a		; store result
	jrnc	ok		; no carry, exit
	inc	(2,x)		; propagate carry
	jrne	ok		; not zero, exit
	inc	(1,x)		; propagate carry
	jrne	ok		; not zero, exit
	inc	(x)		; propagate carry
ok:
	ret			; and return
;
	end
