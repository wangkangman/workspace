;	STRUCTURE COPY
;	Copyright (c) 2006 by COSMIC Software
;	- source ptr in Y
;	- destination ptr in X
;	- size in A
;
	xdef	d_xymov
	xref.b	c_x
	.dcall	"5,0,d_xymov"
;
d_xymov:
	pushw	x		; save dest pointer
	ld	c_x,a		; copy size size
bcl:
	ld	a,(y)		; load byte
	ld	(x),a		; store byte
	incw	x		; increment
	incw	y		; pointers
	dec	c_x		; decrement count
	jrne	bcl		; and loop back
	popw	x		; restore pointer
	retf			; and return
;
	end
