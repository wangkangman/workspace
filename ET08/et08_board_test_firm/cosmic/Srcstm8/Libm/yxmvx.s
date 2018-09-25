;	STRUCTURE COPY IN EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- source ptr in X
;	- destination ptr in Y
;	- size in A
;
	xdef	d_yxmvx
	xref.b	c_x
	.dcall	"5,0,d_yxmvx"
;
d_yxmvx:
	pushw	y		; save destination
	ld	c_x,a		; copy size
bcl:
	ld	a,(x)		; load byte
	ld	(y),a		; store byte
	incw	x		; next
	incw	y		; byte
	dec	c_x		; count down
	jrne	bcl		; and loop back
	popw	y		; restore register
	retf			; and return
;
	end
