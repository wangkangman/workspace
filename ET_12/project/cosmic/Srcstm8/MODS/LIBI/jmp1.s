;	SETJMP AND LONGJMP
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_longjmp, f_setjmp
	xref.b	c_x
	.dcall	"3,2,f_longjmp"
	.dcall	"3,0,f_setjmp"
;
f_longjmp:
	ldw	y,(4,sp)	; copy
	ldw	c_x,y		; return value
	ldw	y,x
	ldw	y,(y)		; new SP
	ldw	sp,y		; in place
	ld	a,(2,x)		; return address MSB
	ld	(1,sp),a	; in place
	ldw	x,(3,x)
	ldw	(2,sp),x	; return address LSW
	ldw	x,c_x		; return value
	jrne	ok		; cannot be zero
	incw	x
ok:
	retf			; and return
;
f_setjmp:
	ld	a,(1,sp)	; return address MSB
	ld	(2,x),a		; in place
	ldw	y,(2,sp)
	ldw	(3,x),y		; return address LSW
	ldw	y,sp		; get SP
	ldw	(x),y		; in place
	clrw	x		; return value
	retf			; and return
;
	end

