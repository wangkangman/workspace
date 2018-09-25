;	LONG SHIFT LEFT
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg in A
;
	xdef	d_llsh
	xref.b	c_lreg
	.dcall	"3,0,d_llsh"
;
d_llsh:
	tnz	a		; test zero count
	jreq	fini		; yes, exit
bcl:
	sll	c_lreg+3	; slow shift left
	rlc	c_lreg+2
	rlc	c_lreg+1
	rlc	c_lreg
	dec	a		; count down
	jrne	bcl		; and loop back
fini:
	retf			; and return
;
	end
