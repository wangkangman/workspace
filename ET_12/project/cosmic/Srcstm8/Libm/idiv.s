;	INTEGER DIVISION
;	Copyright (c) 2006 by COSMIC Software
;	- 1st operand in X
;	- 2nd operand in Y
;
	xdef	d_idiv
	.dcall	"4,0,d_idiv"
;
d_idiv:
	push	a		; save register
	clr	a		; clear sign flag
	tnzw	x		; test dividend sign
	jrpl	ok1		; positive, ok
	inc	a		; mark sign flag
	negw	x		; otherwise invert
ok1:
	tnzw	y		; test divisor sign
	jrpl	ok2		; positive, ok
	add	a,#2		; mark sign flag
	negw	y		; invert
ok2:
	divw	x,y		; divide
	tnz	a		; test signs
	jreq	fin		; all positive, exit
	srl	a		; test remainder sign
	jrnc	qinv		; no, invert quotient
	negw	y		; invert
	srl	a		; both negative
	jrc	fin		; yes, exit
qinv:
	negw	x		; invert
fin:
	pop	a		; restore register
	retf			; and return
;
	end
