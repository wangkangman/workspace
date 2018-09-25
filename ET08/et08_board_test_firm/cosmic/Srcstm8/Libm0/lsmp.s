;	LONG COMPARE WITH OVERFLOW
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in LONG ACCU
;	- second operand pointed by X
;
	xdef	c_lsmp
	xref.b	c_lreg
	.dcall	"2,0,c_lsmp"
;
c_lsmp:
	ld	a,c_lreg	; compare MSB
	xor	a,(x)		; same sign ?
	jrmi	sdiff		; no, skip
	xor	a,(x)		; restore
	cp	a,(x)		; and continue
	jrne	ok		; not equal, flags set
	ld	a,c_lreg+1
	cp	a,(1,x)
	jrne	ok		; not equal, set flags
	ld	a,c_lreg+2
	cp	a,(2,x)
	jrne	ok		; not equal, set flags
	ld	a,c_lreg+3
	cp	a,(3,x)
ok:
	ret			; return result in carry
sdiff:
	xor	a,(x)		; restore first operand
	scf			; set carry bit
	rlc	a		; set unsigned flags
	ret			; and return
;
	end
