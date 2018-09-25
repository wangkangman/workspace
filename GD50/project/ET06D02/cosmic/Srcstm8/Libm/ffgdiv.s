;	FLOAT DIVISION IN EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	d_ffgdiv
	xref	d_ffdiv, d_rtofl
	xref.b	c_x, c_lreg
	.dcall	"12,0,d_ffgdiv"
;
d_ffgdiv:
	pushw	x		; save operand address
	push	c_x
	clr	c_x+2		; set pointer
	clr	c_x+1
	ldf	a,([c_x.e],x)	; exchange operands
	exg	a,c_lreg
	ldf	([c_x.e],x),a
	inc	c_x+2
	ldf	a,([c_x.e],x)
	exg	a,c_lreg+1
	ldf	([c_x.e],x),a
	inc	c_x+2
	ldf	a,([c_x.e],x)
	exg	a,c_lreg+2
	ldf	([c_x.e],x),a
	inc	c_x+2
	ldf	a,([c_x.e],x)
	exg	a,c_lreg+3
	ldf	([c_x.e],x),a
	callf	d_ffdiv		; operate
	pop	c_x		; restore operand address
	popw	x
	jpf	d_rtofl		; copy result and return
;
	end
