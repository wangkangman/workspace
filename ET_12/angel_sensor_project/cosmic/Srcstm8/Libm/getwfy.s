;	GET A WORD FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in Y and extension
;	- result in X:A
;
	xdef	d_getwfy
	xref.b	c_y
	.dcall	"4,0,d_getwfy"
;
d_getwfy:
	ldw	c_y+1,y		; store address
	ldw	x,#1
	ldf	a,([c_y.e],x)	; get LSB
	push	a		; save
	ldf	a,([c_y.e],x)	; get MSB
	ld	xl,a		; in place
	pop	a		; restore register
	retf			; and return
;
	end
