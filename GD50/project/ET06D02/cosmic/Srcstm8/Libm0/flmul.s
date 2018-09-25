;	LONG MULTIPLY EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X and extension
;
	xdef	c_flmul
	xref	c_lmulc
	xref.b	c_x
	.dcall	"6,0,c_flmul"
;
c_flmul:
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
	push	a
	jp	c_lmulc		; and operate
;
	end
