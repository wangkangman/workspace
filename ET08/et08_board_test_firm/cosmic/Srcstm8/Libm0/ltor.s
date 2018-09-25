;	LOAD LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X
;
	xdef	c_ltor
	xref.b	c_lreg
	.dcall	"2,0,c_ltor"
;
c_ltor:
	push	a		; save register
	ld	a,(x)		; copy value
	ld	c_lreg,a
	ld	a,(1,x)
	ld	c_lreg+1,a
	ld	a,(2,x)
	ld	c_lreg+2,a
	ld	a,(3,x)
	ld	c_lreg+3,a
	pop	a		; restore register
	ret			; and return
;
	end
