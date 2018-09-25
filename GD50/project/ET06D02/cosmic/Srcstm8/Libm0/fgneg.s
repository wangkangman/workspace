;	FLOAT NEGATE IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand and result in FLOAT ACCU
;
	xdef	c_fgneg
	.dcall	"2,0,c_fgneg"
;
c_fgneg:
	ld	a,(x)		; test MSB
	jrne	ok		; not nul, invert
	tnz	(1,x)		; test next byte
	jreq	fini		; nul, exit
ok:
	xor	a,#$80		; invert sign
	ld	(x),a		; and store
fini:
	ret			; and return
;
	end
