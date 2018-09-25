;	LONG MULTIPLY 16 x 8 -> 32
;	Copyright (c) 2006 by COSMIC Software
;	- 1st operand in Y
;	- 2nd operand in A
;	- result in long accumulator
;
	xdef	c_cmuly
	xref.b	c_lreg
	.dcall	"2,0,c_cmuly"
;
c_cmuly:
	ldw	c_lreg,y	; save operand
	mul	y,a		; L * B
	ldw	c_lreg+2,y	; save result
	ld	yl,a
	ld	a,c_lreg	; load LSB
	mul	y,a		; H * B
	clr	c_lreg+1	; prepare
	addw	y,c_lreg+1	; accumulate
	ldw	c_lreg+1,y	; save result
	clr	c_lreg		; complete result
	ret			; and return
;
	end
