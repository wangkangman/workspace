;	FLOAT COMPARE
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in FLOAT ACCU
;	- second operand pointed by X
;
	xdef	c_fcmp
	xref.b	c_lreg, c_x
	.dcall	"2,0,c_fcmp"
;
c_fcmp:
	ld	a,c_lreg	; prepare sign flag
	and	a,(x)		; for both negative
	ld	c_x+1,a		; and keep it
	ld	a,c_lreg	; check if signs
	xor	a,(x)		; are different
	jrpl	seq		; no, continue
	xor	a,(x)		; test first sign
	jrmi	negat		; load negative value
	jra	pos		; else load positive
seq:
	ld	a,c_lreg	; compare MSB
	sub	a,(x)
	jrne	chks		; not equal, test signs
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
	jruge	pos		; unsigned -> signed
negat:
	ld	a,#$ff		; set sign bit
	jra	chks		; and continue
pos:
	ld	a,#1		; reset sign
chks:
	btjf	c_x+1,#7,fini	; test sign flag
	neg	a		; invert sign
fini:
	rvf			; clear overflow
	ret			; and return
;
	end
