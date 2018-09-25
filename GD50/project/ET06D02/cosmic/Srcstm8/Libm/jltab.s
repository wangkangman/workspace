;	SWITCH LONG
;	Copyright (c) 2006 by COSMIC Software
;	- table address in X
;	- value in ACCU
;
	xdef	d_jltab
	xref.b	c_x, c_lreg
;
d_jltab:
	ldw	y,x		; keep address
	ldw	x,(x)		; get count
bcl0:
	ldw	c_x,x		; save count
	ldw	x,y
	ldw	x,(2,x)		; get value MSW
	cpw	x,c_lreg	; compare
	jrne	next		; not equal, skip
	ldw	x,y
	ldw	x,(4,x)		; get value LSW
	cpw	x,c_lreg+2	; compare
	jreq	go		; equal, jump
next:
	addw	y,#6		; increment address
	ldw	x,c_x		; get count
	decw	x		; decrement count
	jrne	bcl0		; and loop back
	subw	y,#4		; adjust address
go:
	ldw	x,y
	ldw	x,(6,x)		; get address MSB
	ldw	(2,sp),x	; store on stack
	retf			; and go
;
	end
