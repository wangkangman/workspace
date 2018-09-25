;	LARGE STRUCTURE COPY IN EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- source ptr in Y
;	- destination ptr in X extension
;	- size in X
;
	xdef	d_xymvxl
	xref.b	c_x, c_y
	.dcall	"3,0,d_xymvxl"
;
d_xymvxl:
	addw	x,c_x		; end address
	ldw	c_y,x		; copy size
	ldw	x,c_x		; destination pointer
bcl:
	ld	a,(y)		; load byte
	ld	(x),a		; store byte
	incw	x		; next
	incw	y		; byte
	cpw	x,c_y		; check last
	jrne	bcl		; and loop back
	ldw	x,c_x		; restore pointer
	retf			; and return
;
	end
