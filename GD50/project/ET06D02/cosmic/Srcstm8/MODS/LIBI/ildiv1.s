;	LONG DIVISION WITH QUOTIENT AND REMAINDER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f__uldiv
	xref	d_divul, d_ltor, d_rtol
	xref.b	c_x
	.dcall	"8,0,f__uldiv"
;
f__uldiv:
	pushw	x		; save pointer
	callf	d_ltor		; copy dividend
	ld	a,(4,x)		; divisor
	subw	sp,#4		; open stack frame
	push	a		; prepare divisor MSW
	clr	a
	push	a
	push	a
	ldw	x,sp
	push	a
	callf	d_divul		; perform division
	addw	sp,#6		; clean stack
	popw	y		; remainder
	popw	x		; restore pointer
	callf	d_rtol		; copy result
	ldw	x,y		; remainder in place
	retf			; and return
;
	end
