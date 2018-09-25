;	LONG DIVISION WITH QUOTIENT AND REMAINDER
;	Copyright (c) 2007 by COSMIC Software
;
	xdef	__uldiv
	xref	c_divul, c_ltor, c_rtol
	xref.b	c_x
	.dcall	"6,0,__uldiv"
;
__uldiv:
	pushw	x		; save pointer
	call	c_ltor		; copy dividend
	ld	a,(4,x)		; divisor
	subw	sp,#4		; open frame
	push	a		; prepare divisor MSW
	clr	a
	push	a
	push	a
	ldw	x,sp		; set pointer
	push	a
	call	c_divul		; perform division
	addw	sp,#6		; clean stack
	popw	y		; remainder
	popw	x		; restore pointer
	call	c_rtol		; copy result
	ldw	x,y		; remainder in place
	ret			; and return
;
	end
