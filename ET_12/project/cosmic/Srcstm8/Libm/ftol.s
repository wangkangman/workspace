;	FLOAT TO INTEGER CONVERSION
;	Copyright (c) 2006 by COSMIC Software
;	- operand in FLOAT ACCU
;	- result in X for ints, or LONG ACCU for longs
;
	xdef	d_ftol, d_ftoi
	xref.b	c_lreg
	.dcall	"5,0,d_ftol"
	.dcall	"8,0,d_ftoi"
;
d_ftoi:
	callf	d_ftol		; convert to long
	ldw	x,c_lreg+2	; load value
	retf			; and return
;
d_ftol:
	pushw	x		; save register
	ldw	x,c_lreg	; load MSW
	jreq	fini
panul:
	clr	a
	sllw	x		; align exponent
	rlwa	x,a		; get exponent
	sub	a,#127		; remove offset
	jrpl	valid		; test underflow
zero:
	clrw	x		; clear
	ldw	c_lreg,x	; result
	ldw	c_lreg+2,x
	jra	fini		; and exit
valid:
	pushw	y		; save register
	cp	a,#55		; (32 + 23)
	jrpl	zero		; test overflow
	rrcw	x		; insert hidden bit (C set)
	swapw	x		; set MSW
	ldw	y,c_lreg+2	; set LSW
	sub	a,#23		; result ok if exp == 23
	jreq	signe		; ok, continue
	jrpl	posit		; left shift
	neg	a		; count positive
	cp	a,#16		; fast shift
	jrmi	slow1		; no, slow
	ldw	y,x		; word
	clrw	x		; shift
	sub	a,#16
	jreq	signe		; finished
slow1:
	srlw	x		; shift
	rrcw	y		; right
	dec	a		; count down
	jrne	slow1		; and loop back
	jra	signe		; and continue
posit:
	cp	a,#16		; fast shift
	jrmi	slow2		; no, slow
	ldw	x,y		; word
	clrw	y		; shift
	sub	a,#16
	jreq	signe		; finished
slow2:
	sllw	y		; shift
	rlcw	x		; left
	dec	a		; count down
	jrne	slow2		; and loop back
signe:
	tnz	c_lreg		; test sign
	jrpl	sok		; positive, ok
	cplw	x		; otherwise
	negw	y		; invert value
	jrne	sok
	incw	x
sok:
	ldw	c_lreg+2,y	; store
	ldw	c_lreg,x	; result
	popw	y		; restore register
fini:
	popw	x		; restore register
	retf			; and return
;
	end
