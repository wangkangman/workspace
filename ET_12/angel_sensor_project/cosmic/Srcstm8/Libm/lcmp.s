;	LONG COMPARE
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in LONG ACCU
;	- second operand pointed by X
;
	xdef	d_lcmp
	xref.b	c_lreg
	.dcall	"3,0,d_lcmp"
;
d_lcmp:
	ld	a,c_lreg	; compare MSB
	cp	a,(x)
	jrne	fini		; not equal, flags set
	ld	a,c_lreg+1
	cp	a,(1,x)
	jrne	ok		; not equal, set flags
	ld	a,c_lreg+2
	cp	a,(2,x)
	jrne	ok		; not equal, set flags
	ld	a,c_lreg+3
	cp	a,(3,x)
	jreq	fini		; equal, flags set
ok:
	rvf			; no overflow
	jruge	pos		; unsigned -> signed
	ld	a,#$ff		; set sign bit
	retf			; and return
pos:
	ld	a,#1		; reset sign
fini:
	retf			; and return
;
	end
