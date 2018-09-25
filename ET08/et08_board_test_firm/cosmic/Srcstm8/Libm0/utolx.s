;	MOVE UNSIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- operand in X
;	- result in LONG ACCU
;
	xdef	c_uitolx
	xref.b	c_lreg
	.dcall	"2,0,c_uitolx"
;
c_uitolx:
	ldw	c_lreg+2,x	; store LSW
	clr	c_lreg+1	; complete
	clr	c_lreg		; result
	ret			; and return
;
	end
