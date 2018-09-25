;	INTEGER MULTIPLY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st operand in X
;	- 2nd operand in Y
;
	xdef	d_imul
	xref.b	c_x
	.dcall	"4,0,d_imul"
;
d_imul:
	push	a		; save register
	ldw	c_x+1,x		; save operand
	ld	a,yl
	swapw	x
	mul	x,a		; H * L
	ldw	c_x,x		; save in memory
	ldw	x,c_x+1		; load LSB
	ld	a,yh		; load MSB
	mul	x,a		; L * H
	addw	x,c_x		; accumulate
	ldw	c_x,x		; and store
	ldw	x,c_x+1		; load LSB
	ld	a,yl		; load LSB
	mul	x,a		; L * L
	clr	c_x+2		; prepare and
	addw	x,c_x+1		; accumulate
	pop	a		; restore register
	retf			; and return
;
	end
