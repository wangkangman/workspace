;	FLOAT SUBSTRACT WITH FAR MEMORY
;	Copyright (c) 2007 by COSMIC Software
;	- first operand in float accumulator
;	- second operand pointed by X
;
	xdef	d_ffsub
	xref	d_oksub
	xref.b	c_x
	.dcall	"9,0,d_ffsub"
;
;	float substract
;
d_ffsub:
	ldw	c_x+1,x		; set address
	ldw	x,#3
	ldf	a,([c_x.e],x)	; load operand
	push	a
	decw	x
	ldf	a,([c_x.e],x)
	push	a
	decw	x
	ldf	a,([c_x.e],x)
	push	a
	ldf	a,[c_x.e]
	jpf	d_oksub		; and continue
;
	end
