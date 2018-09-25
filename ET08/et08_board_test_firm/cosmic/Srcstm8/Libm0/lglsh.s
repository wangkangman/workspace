;	LONG SHIFT LEFT IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg pointed by X
;	- 2nd arg in A
;
	xdef	c_lglsh
	xref.b	c_lreg
	.dcall	"2,0,c_lglsh"
;
c_lglsh:
	tnz	a		; test zero count
	jreq	fini		; yes, exit
bcl:
	sll	(3,x)		; shift left
	rlc	(2,x)
	rlc	(1,x)
	rlc	(x)
	dec	a		; count down
	jrne	bcl		; and loop back
fini:
	ret			; and return
;
	end
