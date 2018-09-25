;	UNSIGNED LONG DIVISION WITH FAR MEMORY
;	Copyright (c) 2007 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X and extension
;
	xdef	d_fludv
	xref	d_divul
	xref	c_x
	.dcall	"15,0,d_fludv"
;
d_fludv:
	subw	sp,#4		; open stack frame
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
	ldw	x,sp		; set stack address
	ldf	a,[c_x.e]
	push	a
	callf	d_divul		; perform division
	addw	sp,#8		; clean stack
	retf			; and return
;
	end
