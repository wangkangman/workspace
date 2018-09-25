;	LONG MULTIPLY EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X and extension
;
	xdef	d_flmul
	xref	d_lmulc
	xref.b	c_x
	.dcall	"7,0,d_flmul"
;
d_flmul:
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
	jpf	d_lmulc		; and operate
;
	end
