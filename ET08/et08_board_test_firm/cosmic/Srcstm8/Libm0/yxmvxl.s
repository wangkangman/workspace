;	LARGE STRUCTURE COPY IN EXTERNAL MEMORY
;	Copyright (c) 2007 by COSMIC Software
;	- source ptr in X extension
;	- destination ptr in Y
;	- size in X
;
	xdef	c_yxmvxl
	xref.b	c_x, c_y
	.dcall	"4,0,c_yxmvxl"
;
c_yxmvxl:
	pushw	y		; save destination
	addw	x,c_x		; end address
	ldw	c_y,x		; in memory
	ldw	x,c_x		; source address
bcl:
	ld	a,(x)		; load byte
	ld	(y),a		; store byte
	incw	x		; next
	incw	y		; byte
	cpw	x,c_y		; check last
	jrne	bcl		; and loop back
	popw	y		; restore register
	ret			; and return
;
	end
