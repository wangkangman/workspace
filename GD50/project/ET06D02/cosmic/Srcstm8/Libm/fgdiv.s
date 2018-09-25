;	FLOAT DIVISION IN MEMORY
;	Copyright (c) 2001 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	d_fgdiv
	xref	d_fdiv, d_rtol
	xref.b	c_lreg
	.dcall	"11,0,d_fgdiv"
;
d_fgdiv:
	pushw	x		; save address
	ld	a,(x)		; exchange operands
	exg	a,c_lreg
	ld	(x),a
	ld	a,(1,x)
	exg	a,c_lreg+1
	ld	(1,x),a
	ld	a,(2,x)
	exg	a,c_lreg+2
	ld	(2,x),a
	ld	a,(3,x)
	exg	a,c_lreg+3
	ld	(3,x),a
	callf	d_fdiv		; operate
	popw	x		; take back address
	jpf	d_rtol		; copy result and return
;
	end
