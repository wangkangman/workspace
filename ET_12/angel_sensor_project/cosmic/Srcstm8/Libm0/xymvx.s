;	STRUCTURE COPY IN EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- source ptr in Y
;	- destination ptr in X
;	- size in A
;
	xdef	c_xymvx
	xref.b	c_x
	.dcall	"4,0,c_xymvx"
;
c_xymvx:
	pushw	x		; save destination
	ld	c_x,a		; copy size
bcl:
	ld	a,(y)		; load byte
	ld	(x),a		; store byte
	incw	x		; next
	incw	y		; byte
	dec	c_x		; count down
	jrne	bcl		; and loop back
	popw	x		; restore pointer
	ret			; and return
;
	end
