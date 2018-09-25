;	LONG COMPARE AGAINST ZERO
;	Copyright (c) 2006 by COSMIC Software
;	- operand in LONG ACCU
;
	xdef	d_lrzmp
	xref.b	c_lreg
	.dcall	"3,0,d_lrzmp"
;
d_lrzmp:
	tnz	c_lreg		; test MSB
	jrne	fini		; not equal, flags set
	tnz	c_lreg+1
	jrne	ok		; not equal, set flags
	tnz	c_lreg+2
	jrne	ok		; not equal, set flags
	tnz	c_lreg+3
	jreq	fini		; equal, flags set
ok:
	ld	a,#1		; clear N and Z, keep C
fini:
	retf			; and return
;
	end
