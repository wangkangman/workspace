;	SIGNED LONG MODULUS
;	Copyright (c) 2007 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	c_lmod
	xref.b	c_lreg, c_x
	xref	c_divsl, c_lneg
	.dcall	"14,0,c_lmod"
;
c_lmod:
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
	call	c_divsl		; perform division
	addw	sp,#4		; clean stack
	popw	x		; copy
	ldw	c_lreg,x	; result
	popw	x
	ldw	c_lreg+2,x
	btjf	c_x,#1,okd	; no sign, skip
	jp	c_lneg		; else invert
okd:
	ret			; and return
;
	end
