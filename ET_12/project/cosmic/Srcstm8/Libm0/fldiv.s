;	SIGNED LONG DIVISION WITH FAR MEMORY
;	Copyright (c) 2007 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X and extension
;
	xdef	c_fldiv
	xref	c_divsl
	xref.b	c_x
	.dcall	"14,0,c_fldiv"
;
c_fldiv:
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
	call	c_divsl		; perform division
	addw	sp,#8		; clean stack
	ret			; and return
;
	end
