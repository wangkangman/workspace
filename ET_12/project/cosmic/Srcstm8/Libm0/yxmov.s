;	STRUCTURE COPY
;	Copyright (c) 2006 by COSMIC Software
;	- source ptr in X
;	- destination ptr in Y
;	- size in A
;
	xdef	c_yxmov
	xref.b	c_x
	.dcall	"4,0,c_yxmov"
;
c_yxmov:
	pushw	y		; save dest pointer
	ld	c_x,a		; copy size size
bcl:
	ld	a,(x)		; load byte
	ld	(y),a		; store byte
	incw	x		; increment
	incw	y		; pointers
	dec	c_x		; decrement count
	jrne	bcl		; and loop back
	popw	y		; restore pointer
	ret			; and return
;
	end
