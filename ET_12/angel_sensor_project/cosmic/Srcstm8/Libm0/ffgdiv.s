;	FLOAT DIVISION IN EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	c_ffgdiv
	xref	c_ffdiv, c_rtofl
	xref.b	c_x, c_lreg
	.dcall	"10,0,c_ffgdiv"
;
c_ffgdiv:
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
	call	c_ffdiv		; operate
	pop	c_x		; restore operand address
	popw	x
	jp	c_rtofl		; copy result and return
;
	end
