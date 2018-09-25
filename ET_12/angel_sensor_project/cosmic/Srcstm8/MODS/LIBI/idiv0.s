;	DIVISION WITH QUOTIENT AND REMAINDER
;	Copyright (c) 2007 by COSMIC Software
;
	xdef	__udiv
	.dcall	"4,0,__udiv"
;
__udiv:
	pushw	x		; save pointer
	ld	a,(2,x)		; divisor
	ldw	x,(x)		; dividend
	div	x,a		; operate
	exgw	x,y		; for efficiency
	popw	x		; load pointer
	ldw	(x),y		; quotient in place
	clrw	x		; remainder
	ld	xl,a		; in place
	ret			; and return
;
	end
