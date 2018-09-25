;	PUT A WORD IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in Y and extension
;	- operand in X:A
;
	xdef	d_putwf
	xref.b	c_y
	.dcall	"3,0,d_putwf"
;
d_putwf:
	ldw	c_y+1,y		; complete pointer
	clrw	y
	exg	a,xl
	ldf	([c_y.e],y),a	; put LSB
	incw	y
	exg	a,xl
	ldf	([c_y.e],y),a	; put MSB
	retf			; and return
;
	end
