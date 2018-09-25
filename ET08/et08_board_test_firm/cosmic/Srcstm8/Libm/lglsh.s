;	LONG SHIFT LEFT IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg pointed by X
;	- 2nd arg in A
;
	xdef	d_lglsh
	xref.b	c_lreg
	.dcall	"3,0,d_lglsh"
;
d_lglsh:
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
	retf			; and return
;
	end
