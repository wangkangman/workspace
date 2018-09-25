;	PUT A WORD IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in Y and extension
;	- operand in X:A
;
	xdef	c_putwf
	xref.b	c_y
	.dcall	"2,0,c_putwf"
;
c_putwf:
	ldw	c_y+1,y		; complete pointer
	clrw	y
	exg	a,xl
	ldf	([c_y.e],y),a	; put LSB
	incw	y
	exg	a,xl
	ldf	([c_y.e],y),a	; put MSB
	ret			; and return
;
	end
