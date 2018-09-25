;	SMALL INTEGER TO FLOAT CONVERSION
;	Copyright (c) 2006 by COSMIC Software
;	- integer in A
;	- result in FLOAT ACCU
;
	xdef	d_ctof
	xref	d_itof
	xref.b	c_lreg
	.dcall	"8,0,d_ctof"
d_ctof:
	pushw	x		; save register
	clrw	x		; complete
	ld	xl,a		; value
	callf	d_itof		; convert
	popw	x		; restore register
	retf			; and return
;
	end
