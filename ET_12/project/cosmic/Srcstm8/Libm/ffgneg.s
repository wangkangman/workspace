;	FLOAT NEGATE IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X
;
	xdef	d_ffgneg
	xref.b	c_x
	.dcall	"3,0,d_ffgneg"
;
d_ffgneg:
	clr	c_x+2		; set pointer
	clr	c_x+1
	ldf	a,([c_x.e],x)	; test MSB
	jrne	ok		; not nul, invert
	inc	c_x+2
	push	a		; save first byte
	ldf	a,([c_x.e],x)	; test next byte
	pop	a		; restore byte
	jreq	fini		; nul, exit
	dec	c_x+2
ok:
	xor	a,#$80		; invert sign
	ldf	([c_x.e],x),a	; and store
fini:
	retf			; and return
;
	end
