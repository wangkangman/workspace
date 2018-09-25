;	LONG NEGATE
;	Copyright (c) 2006 by COSMIC Software
;	- arg and result in LONG ACCU
;
	xdef	d_lneg
	xref.b	c_lreg
	.dcall	"3,0,d_lneg"
;
d_lneg:
	cpl	c_lreg		; invert accumulator
	cpl	c_lreg+1
	cpl	c_lreg+2
	neg	c_lreg+3	; negate LSB
	jrne	ok		; not nul, exit
	inc	c_lreg+2	; propagate carry
	jrne	ok
	inc	c_lreg+1
	jrne	ok
	inc	c_lreg
ok:
	retf			; and return
;
	end
