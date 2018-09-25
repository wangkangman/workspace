;	DIVISION WITH QUOTIENT AND REMAINDER
;	Copyright (c) 2007 by COSMIC Software
;
	xdef	f__udiv
	.dcall	"5,0,f__udiv"
;
f__udiv:
	pushw	x		; save pointer
	ld	a,(2,x)		; divisor
	ldw	x,(x)		; dividend
	div	x,a		; operate
	exgw	x,y		; for efficiency
	popw	x		; load pointer
	ldw	(x),y		; quotient in place
	clrw	x		; remainder
	ld	xl,a		; place
	retf			; and return
;
	end
