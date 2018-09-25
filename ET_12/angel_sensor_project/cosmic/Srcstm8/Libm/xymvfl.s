;	LARGE STRUCTURE COPY IN FAR MEMORY
;	Copyright (c) 2007 by COSMIC Software
;	- source ptr in Y and extension
;	- destination ptr in X extension
;	- size in X
;
	xdef	d_xymvfl
	xref.b	c_x, c_y
	.dcall	"3,0,d_xymvfl"
;
d_xymvfl:
	ldw	c_y+1,y		; set up pointers
bcl:
	decw	x		; decrement count
	ldf	a,([c_y.e],x)	; load byte
	ldf	([c_x.e],x),a	; store byte
	tnzw	x		; test count
	jrne	bcl		; and loop back
	ldw	x,c_x+1		; restore pointer
	retf			; and return
;
	end
