;	SETJMP AND LONGJMP
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_longjmp, _setjmp
	xref.b	c_x
	.dcall	"2,2,_longjmp"
	.dcall	"2,0,_setjmp"
;
_longjmp:
	ldw	y,(3,sp)	; return value
	ldw	c_x,y		; value
	ldw	y,x		; copy address
	ldw	x,(x)		; new SP
	ldw	sp,x		; in place
	ldw	y,(2,y)		; return address
	ldw	(1,sp),y	; in place
	ldw	x,c_x		; get
	jrne	ok		; cannot be zero
	incw	x
ok:
	ret			; and return
;
_setjmp:
	ldw	y,(1,sp)	; return address
	ldw	(2,x),y		; in place
	ldw	y,sp		; get SP
	ldw	(x),y		; in place
	clrw	x		; return value
	ret			; and return
;
	end

