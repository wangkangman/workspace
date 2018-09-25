;	LONG COMPARE FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in LONG ACCU
;	- second operand pointed by X and extension
;
	xdef	c_flcmp
	xref.b	c_lreg, c_x
	.dcall	"2,0,c_flcmp"
;
c_flcmp:
	clr	c_x+2		; set address to keep X
	clr	c_x+1
	ldf	a,([c_x.e],x)	; compare MSB
	sub	a,c_lreg
	neg	a		; restore sign
	jrne	fini		; not equal, flags set
	inc	c_x+2
	ldf	a,([c_x.e],x)
	cp	a,c_lreg+1
	jrne	ok		; not equal, set flags
	inc	c_x+2
	ldf	a,([c_x.e],x)
	cp	a,c_lreg+2
	jrne	ok		; not equal, set flags
	inc	c_x+2
	ldf	a,([c_x.e],x)
	cp	a,c_lreg+3
	jreq	fini		; equal, flags set
ok:
	rvf			; no overflow
	ccf			; invert carry
	jruge	pos		; unsigned -> signed
	ld	a,#$ff		; set sign bit
	jra	fini		; and continue
pos:
	ld	a,#1		; reset sign
fini:
	ret			; and return
;
	end
