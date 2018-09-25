;	FLOAT COMPARE TO ZERO
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X
;
	xdef	d_fzmp
	.dcall	"3,0,d_fzmp"
;
d_fzmp:
	ld	a,(x)		; test MSB
	jrne	fini		; not nul, flags set
	ld	a,(1,x)		; test remainder
	srl	a		; force positive
fini:
	retf			; and return
;
	end
