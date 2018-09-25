;	STORE LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X
;
	xdef	d_rtol
	xref.b	c_lreg
	.dcall	"4,0,d_rtol"
;
d_rtol:
	push	a		; save register
	ld	a,c_lreg	; copy value
	ld	(x),a
	ld	a,c_lreg+1
	ld	(1,x),a
	ld	a,c_lreg+2
	ld	(2,x),a
	ld	a,c_lreg+3
	ld	(3,x),a
	pop	a
	retf			; and return
;
	end
