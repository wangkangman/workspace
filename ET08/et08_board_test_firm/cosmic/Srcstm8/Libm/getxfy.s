;	GET A WORD FROM EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in Y and extension
;	- result in X
;
	xdef	d_getxfy
	xref.b	c_y
	.dcall	"4,0,d_getxfy"
;
d_getxfy:
	push	a		; save register
	ldw	c_y+1,y		; store address
	ldw	x,#1
	ldf	a,([c_y.e],x)	; get LSB
	ld	xl,a		; in place
	ldf	a,[c_y.e]	; get MSB
	ld	xh,a		; in place
	pop	a		; restore register
	retf			; and return
;
	end
