;	FLOAT NEGATE IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand and result in FLOAT ACCU
;
	xdef	d_fgneg
	.dcall	"3,0,d_fgneg"
;
d_fgneg:
	ld	a,(x)		; test MSB
	jrne	ok		; not nul, invert
	tnz	(1,x)		; test next byte
	jreq	fini		; nul, exit
ok:
	xor	a,#$80		; invert sign
	ld	(x),a		; and store
fini:
	retf			; and return
;
	end
