;	FLOAT COMPARE TO ZERO FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X
;
	xdef	d_ffzmp
	xref.b	c_x
	.dcall	"3,0,d_ffzmp"
;
d_ffzmp:
	clr	c_x+2		; prepare address
	clr	c_x+1
	ldf	a,([c_x.e],x)	; test MSB
	jrne	fini		; not nul, flags set
	inc	c_x+2		; next address
	ldf	a,([c_x.e],x)	; test remainder
	srl	a		; force positive
fini:
	retf			; and return
;
	end
