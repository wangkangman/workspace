;	SIGNED LONG DIVISION
;	Copyright (c) 2007 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	d_ldiv
	xref.b	c_lreg, c_x
	xref	d_divsl, d_lneg
	.dcall	"15,0,d_ldiv"
;
d_ldiv:
	subw	sp,#4		; open stack frame
	ld	a,(3,x)		; copy operand
	push	a		; on the stack
	ld	a,(2,x)
	push	a
	ld	a,(1,x)
	push	a
	ld	a,(x)
	ldw	x,sp		; set operand address
	push	a
	callf	d_divsl		; perform division
	addw	sp,#8		; clean stack
	ld	a,c_x		; test sign flag
	jreq	okd		; nul, ok
	cp	a,#3		; both negative, ok
	jreq	okd
	jpf	d_lneg		; else invert
okd:
	retf			; and return
;
	end
