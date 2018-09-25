;	UNSIGNED LONG DIVISION
;	Copyright (c) 2007 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	d_ludv
	xref	d_divul
	xref.b	c_lreg
	.dcall	"15,0,d_ludv"
;
d_ludv:
	subw	sp,#4		; open stack frame
	ld	a,(3,x)		; copy operand
	push	a
	ld	a,(2,x)
	push	a
	ld	a,(1,x)
	push	a
	ld	a,(x)
	ldw	x,sp		; set stack address
	push	a
	callf	d_divul		; perform division
	addw	sp,#8		; clean stack
	retf			; and return
;
	end
