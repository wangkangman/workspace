;	SIGNED CHAR DIVISION/MODULUS
;	Copyright (c) 2006 by COSMIC Software
;	- 1st operand in Y
;	- 2nd operand in A
;
	xdef	c_sdivy
	xdef	c_smody
	xref.b	c_y
	.dcall	"2,0,c_sdivy"
	.dcall	"2,0,c_smody"
;
c_smody:
	mov	c_y,#$80	; set flag
	jra	suite		; and continue
c_sdivy:
	clr	c_y		; clear flag
suite:
	tnzw	y		; test dividend sign
	jrpl	ok1		; positive, ok
	negw	y		; invert
	bset	c_y,#0		; mark sign
	bset	c_y,#1		; flag
ok1:
	tnz	a		; test divisor sign
	jrpl	ok2		; positive, ok
	neg	a		; invert
	bcpl	c_y,#1		; mark sign flag
ok2:
	div	y,a		; divide
	sra	c_y		; test return type
	jrpl	quot		; set quotient result
	clrw	y		; prepare
	ld	yl,a		; remainder
	jrnc	fin		; no inversion
	negw	y		; invert result
	ret			; and return
quot:
	jreq	fin		; yes, exit
	negw	y
fin:
	ret			; and return
;
	end
