;	LARGE STRUCTURE COPY IN FAR MEMORY
;	Copyright (c) 2007 by COSMIC Software
;	- source ptr in X extension
;	- destination ptr in Y and extension
;	- size in X
;
	xdef	c_yxmvfl
	xref.b	c_x, c_y
	.dcall	"2,0,c_yxmvfl"
;
c_yxmvfl:
	ldw	c_y+1,y		; set up pointers
bcl:
	decw	x		; decrement count
	ldf	a,([c_x.e],x)	; load byte
	ldf	([c_y.e],x),a	; store byte
	tnzw	x		; test count
	jrne	bcl		; and loop back
	ret			; and return
;
	end
