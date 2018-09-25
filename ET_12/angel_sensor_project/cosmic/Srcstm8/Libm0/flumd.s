;	UNSIGNED LONG MODULUS WITH FAR MEMORY
;	Copyright (c) 2007 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X and extension
;
	xdef	c_flumd
	xref	c_divul
	xref.b	c_lreg, c_x
	.dcall	"14,0,c_flumd"
;
c_flumd:
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
	call	c_divul		; perform division
	addw	sp,#4		; clean stack
	popw	x		; copy
	ldw	c_lreg,x	; result
	popw	x
	ldw	c_lreg+2,x
	ret			; and return
;
	end
