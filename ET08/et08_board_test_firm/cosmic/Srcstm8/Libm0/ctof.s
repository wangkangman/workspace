;	SMALL INTEGER TO FLOAT CONVERSION
;	Copyright (c) 2006 by COSMIC Software
;	- integer in A
;	- result in FLOAT ACCU
;
	xdef	c_ctof
	xref	c_itof
	xref.b	c_lreg
	.dcall	"6,0,c_ctof"
c_ctof:
	pushw	x		; save register
	clrw	x		; complete
	ld	xl,a		; value
	call	c_itof		; convert
	popw	x		; restore register
	ret			; and return
;
	end
