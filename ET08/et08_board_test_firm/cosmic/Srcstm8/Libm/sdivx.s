;	SIGNED CHAR DIVISION/MODULUS
;	Copyright (c) 2006 by COSMIC Software
;	- 1st operand in X
;	- 2nd operand in A
;
	xdef	d_sdivx
	xdef	d_smodx
	xref.b	c_x
	.dcall	"3,0,d_sdivx"
	.dcall	"3,0,d_smodx"
;
d_smodx:
	mov	c_x,#$80	; set flag
	jra	suite		; and continue
d_sdivx:
	clr	c_x		; clear flag
suite:
	tnzw	x		; test dividend sign
	jrpl	ok1		; positive, ok
	negw	x		; invert
	bset	c_x,#0		; mark sign
	bset	c_x,#1		; flag
ok1:
	tnz	a		; test divisor sign
	jrpl	ok2		; positive, ok
	neg	a		; invert
	bcpl	c_x,#1		; mark sign flag
ok2:
	div	x,a		; divide
	sra	c_x		; test return type
	jrpl	quot		; set quotient result
	clrw	x		; prepare
	ld	xl,a		; remainder
	jrnc	fin		; no inversion
	negw	x		; invert result
	retf			; and return
quot:
	jreq	fin		; yes, exit
	negw	x
fin:
	retf			; and return
;
	end
