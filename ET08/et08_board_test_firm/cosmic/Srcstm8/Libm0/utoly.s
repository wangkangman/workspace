;	MOVE UNSIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- operand in Y
;	- result in LONG ACCU
;
	xdef	c_uitoly
	xref.b	c_lreg
	.dcall	"2,0,c_uitoly"
;
c_uitoly:
	ldw	c_lreg+2,y	; store LSW
	clr	c_lreg+1	; complete
	clr	c_lreg		; result
	ret			; and return
;
	end
