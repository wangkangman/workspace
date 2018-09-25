;	FLOAT COMPARE FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in FLOAT ACCU
;	- second operand pointed by X
;
	xdef	d_ffcmp
	xref.b	c_lreg, c_x
	.dcall	"5,0,d_ffcmp"
;
d_ffcmp:
	pushw	y		; save register
	clr	c_x+2		; set address to keep X
	clr	c_x+1
	ldf	a,([c_x.e],x)	; load first byte
	and	a,c_lreg	; prepare sign byte
	ld	yh,a		; and keep it
	ldf	a,([c_x.e],x)	; get it again
	xor	a,c_lreg	; check if both sign
	jrpl	seq		; equal, and continue if so
	xor	a,c_lreg	; test second sign
	jrpl	negat		; load a negative value
	jra	pos		; else load a positive value
seq:
	ldf	a,([c_x.e],x)	; compare MSB
	sub	a,c_lreg
	neg	a		; restore sign
	jrne	chks		; not equal, check signs
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
	jreq	fin		; equal, flags set
ok:
	jrult	pos		; unsigned -> signed
negat:
	ld	a,#$ff		; set sign bit
	jra	chks		; and continue
pos:
	ld	a,#1		; reset sign
chks:
	tnzw	y		; check
	jrpl	fini		; check sign flag
	neg	a		; inverts sign
fini:
	tnz	a		; restore flags
fin:
	popw	y		; restore register
	rvf			; clear overflow
	retf			; and return
;
	end
