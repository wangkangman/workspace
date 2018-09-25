;	LONG MULTIPLY 16 x 8 -> 32
;	Copyright (c) 2006 by COSMIC Software
;	- 1st operand in X
;	- 2nd operand in A
;	- result in long accumulator
;
	xdef	d_cmulx
	xref.b	c_lreg
	.dcall	"3,0,d_cmulx"
;
d_cmulx:
	ldw	c_lreg,x	; save operand
	mul	x,a		; L * B
	ldw	c_lreg+2,x	; save result
	ld	xl,a
	ld	a,c_lreg	; load LSB
	mul	x,a		; H * B
	clr	c_lreg+1	; prepare
	addw	x,c_lreg+1	; accumulate
	ldw	c_lreg+1,x	; save result
	clr	c_lreg		; complete result
	retf			; and return
;
	end
