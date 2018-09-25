;	STRUCTURE COPY IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- source ptr in X and extension
;	- destination ptr in Y and extension
;	- size in A
;
	xdef	c_yxmvf
	xref.b	c_x, c_y
	.dcall	"2,0,c_yxmvf"
;
c_yxmvf:
	ldw	c_x+1,x		; set up
	ldw	c_y+1,y		; pointers
	clrw	x		; size
	ld	xl,a		; becomes offset
bcl:
	decw	x		; decrement count
	ldf	a,([c_x.e],x)	; load byte
	ldf	([c_y.e],x),a	; store byte
	tnzw	x		; test count
	jrne	bcl		; and loop back
	ret			; and return
;
	end
