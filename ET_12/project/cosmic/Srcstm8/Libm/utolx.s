;	MOVE UNSIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- operand in X
;	- result in LONG ACCU
;
	xdef	d_uitolx
	xref.b	c_lreg
	.dcall	"3,0,d_uitolx"
;
d_uitolx:
	ldw	c_lreg+2,x	; store LSW
	clr	c_lreg+1	; complete
	clr	c_lreg		; result
	retf			; and return
;
	end
