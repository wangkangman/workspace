;	LONG COMPARE FAR MEMORY WITH OVERFLOW
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in LONG ACCU
;	- second operand pointed by X and extension
;
	xdef	d_flsmp
	xref.b	c_lreg, c_x
	.dcall	"3,0,d_flsmp"
;
d_flsmp:
	clr	c_x+2		; set address to keep X
	clr	c_x+1
	ldf	a,([c_x.e],x)	; compare MSB
	xor	a,c_lreg	; are they equal
	jrmi	sdiff		; no, skip
	xor	a,c_lreg	; restore MSB
	sub	a,c_lreg
	jrne	ok		; not equal, flags set
	inc	c_x+2
	ldf	a,([c_x.e],x)
	sub	a,c_lreg+1
	jrne	ok		; not equal, set flags
	inc	c_x+2
	ldf	a,([c_x.e],x)
	sub	a,c_lreg+2
	jrne	ok		; not equal, set flags
	inc	c_x+2
	ldf	a,([c_x.e],x)
	sub	a,c_lreg+3
ok:
	neg	a		; restore sign
	retf			; and return
sdiff:
	ld	a,c_lreg	; reload MSB
	scf			; set carry flag
	rlc	a		; set unsigned flags
	retf			; and return
;
	end
