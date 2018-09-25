;	LONG SHIFT LEFT
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg in A
;
	xdef	c_llsh
	xref.b	c_lreg
	.dcall	"2,0,c_llsh"
;
c_llsh:
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
	ret			; and return
;
	end
