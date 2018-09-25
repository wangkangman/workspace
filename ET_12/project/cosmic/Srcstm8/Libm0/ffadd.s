;	FLOAT ADDITION WITH FAR MEMORY
;	Copyright (c) 2007 by COSMIC Software
;	- first operand in float accumulator
;	- second operand pointed by X
;
	xdef	c_ffadd
	xref	c_okadd
	xref.b	c_x
	.dcall	"8,0,c_ffadd"
;
;	float addition
;
c_ffadd:
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
	jp	c_okadd		; and continue
;
	end
