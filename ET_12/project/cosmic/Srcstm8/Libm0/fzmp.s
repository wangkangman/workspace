;	FLOAT COMPARE TO ZERO
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X
;
	xdef	c_fzmp
	.dcall	"2,0,c_fzmp"
;
c_fzmp:
	ld	a,(x)		; test MSB
	jrne	fini		; not nul, flags set
	ld	a,(1,x)		; test remainder
	srl	a		; force positive
fini:
	ret			; and return
;
	end
